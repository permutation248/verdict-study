(* Global Dependencies Plugin - Extract minimal global variable dependencies *)

(* ========================================================================= *)
(* Plugin Registration *)
(* ========================================================================= *)

module Self = Plugin.Register(struct
  let name = "Global Dependencies Extractor"
  let shortname = "global-deps"
  let help = "extract minimal global variable dependencies for function lists"
end)

(* ========================================================================= *)
(* Parameters *)
(* ========================================================================= *)

module Enabled = Self.False(struct
  let option_name = "-global-deps"
  let help = "extract global variable dependencies from function lists"
end)
let () = Enabled.add_aliases ["-gd"]

module InputFile = Self.String(struct
  let option_name = "-global-deps-input"
  let default = ""
  let arg_name = "filename"
  let help = "JSON file with function lists (from RTE k-context extraction)"
end)
let () = InputFile.add_aliases ["-gd-input"]

module OutputFile = Self.String(struct
  let option_name = "-global-deps-output"
  let default = "global_deps.json"
  let arg_name = "filename"
  let help = "output JSON file for global dependencies (default: global_deps.json)"
end)
let () = OutputFile.add_aliases ["-gd-output"]

module COutputDir = Self.String(struct
  let option_name = "-global-deps-c-output"
  let default = ""
  let arg_name = "directory"
  let help = "output directory for C code files (one per context, disabled by default)"
end)
let () = COutputDir.add_aliases ["-gd-c-output"]

(* ========================================================================= *)
(* Data Types *)
(* ========================================================================= *)

(* Input JSON structure *)
type context_input = {
  id : string;
  functions : string list;
}

(* Output structure *)
type context_output = {
  id : string;
  functions : string list;
  missing_functions : string list;
  direct_globals : string list;
  all_globals : string list;
  dependencies : (string * string list) list;
}

(* Varinfo set for efficient operations *)
module VarSet = Cil_datatype.Varinfo.Set

(* ========================================================================= *)
(* Helper Functions *)
(* ========================================================================= *)

(* Resolve function name to Kernel_function.t *)
let resolve_function name =
  try
    Some (Globals.Functions.find_by_name name)
  with Not_found ->
    Self.warning "Function '%s' not found in codebase" name;
    None

(* Visitor to collect global variables used in a function *)
class global_collector globals_ref = object
  inherit Visitor.frama_c_inplace

  method! vvrbl vi =
    (* Only collect global variables, not functions *)
    if vi.Cil_types.vglob && not (Globals.Functions.mem vi) then
      globals_ref := VarSet.add vi !globals_ref;
    Cil.SkipChildren
end

(* Collect global variables used by a single function *)
let collect_globals_for_function kf =
  try
    let globals = ref VarSet.empty in
    let visitor = new global_collector globals in
    let fundec = Kernel_function.get_definition kf in
    ignore (Visitor.visitFramacFunction visitor fundec);
    !globals
  with Kernel_function.No_Definition ->
    (* Function has no body (e.g., declaration only) *)
    VarSet.empty
  | e ->
    Self.warning "Error collecting globals for %s: %s"
      (Kernel_function.get_name kf)
      (Printexc.to_string e);
    VarSet.empty

(* ========================================================================= *)
(* Initializer Dependency Analysis *)
(* ========================================================================= *)

(* Recursively extract global variables from an initializer *)
let extract_globals_from_init init =
  let open Cil_types in
  let globals = ref VarSet.empty in
  let rec visit_init = function
    | SingleInit exp -> visit_exp exp
    | CompoundInit (_typ, offset_init_list) ->
        List.iter (fun (_offset, init) -> visit_init init) offset_init_list
  and visit_exp exp =
    match exp.enode with
    | Lval lv | AddrOf lv | StartOf lv -> visit_lval lv
    | UnOp (_op, e, _typ) -> visit_exp e
    | BinOp (_op, e1, e2, _typ) -> visit_exp e1; visit_exp e2
    | CastE (_typ, e) -> visit_exp e
    | _ -> ()
  and visit_lval (lhost, _offset) =
    match lhost with
    | Var vi when vi.vglob && not (Globals.Functions.mem vi) ->
        globals := VarSet.add vi !globals
    | _ -> ()
  in
  visit_init init;
  !globals

(* Compute initializer dependencies for a single global variable *)
let compute_init_deps vi =
  try
    let initinfo = Globals.Vars.find vi in
    match initinfo.init with
    | None -> VarSet.empty
    | Some init -> extract_globals_from_init init
  with Not_found -> VarSet.empty

(* Build dependency graph for all global variables *)
let build_dep_graph () =
  let graph = Hashtbl.create 97 in
  Globals.Vars.iter_in_file_order (fun vi _initinfo ->
    let deps = compute_init_deps vi in
    if not (VarSet.is_empty deps) then
      Hashtbl.add graph vi deps
  );
  graph

(* Compute transitive closure of dependencies using worklist algorithm *)
let transitive_closure initial_set dep_graph =
  let result = ref initial_set in
  let worklist = ref (VarSet.elements initial_set) in
  let visited = ref VarSet.empty in

  while !worklist <> [] do
    match !worklist with
    | [] -> ()
    | vi :: rest ->
        worklist := rest;
        if not (VarSet.mem vi !visited) then begin
          visited := VarSet.add vi !visited;
          try
            let deps = Hashtbl.find dep_graph vi in
            VarSet.iter (fun dep ->
              if not (VarSet.mem dep !result) then begin
                result := VarSet.add dep !result;
                worklist := dep :: !worklist
              end
            ) deps
          with Not_found -> ()
        end
  done;
  !result

(* ========================================================================= *)
(* C Code Output - Dependency Collector *)
(* ========================================================================= *)

(* Data structure to track needed definitions for C code output *)
type needed_set = {
  mutable varinfos: VarSet.t;
  mutable compinfos: Cil_datatype.Compinfo.Set.t;
  mutable enuminfos: Cil_datatype.Enuminfo.Set.t;
  mutable typeinfos: Cil_datatype.Typeinfo.Set.t;
  mutable functions: VarSet.t;  (* function varinfos *)
}

let create_needed_set () = {
  varinfos = VarSet.empty;
  compinfos = Cil_datatype.Compinfo.Set.empty;
  enuminfos = Cil_datatype.Enuminfo.Set.empty;
  typeinfos = Cil_datatype.Typeinfo.Set.empty;
  functions = VarSet.empty;
}

(* Recursively collect type dependencies *)
let rec collect_type_deps needed typ =
  let open Cil_types in
  match typ with
  | TNamed(ti, _) ->
      if not (Cil_datatype.Typeinfo.Set.mem ti needed.typeinfos) then begin
        needed.typeinfos <- Cil_datatype.Typeinfo.Set.add ti needed.typeinfos;
        collect_type_deps needed ti.ttype
      end
  | TComp(ci, _) ->
      if not (Cil_datatype.Compinfo.Set.mem ci needed.compinfos) then begin
        needed.compinfos <- Cil_datatype.Compinfo.Set.add ci needed.compinfos;
        (* Collect field types *)
        (match ci.cfields with
         | None -> ()
         | Some fields ->
             List.iter (fun fi -> collect_type_deps needed fi.ftype) fields)
      end
  | TEnum(ei, _) ->
      needed.enuminfos <- Cil_datatype.Enuminfo.Set.add ei needed.enuminfos
  | TPtr(t, _) | TArray(t, _, _) ->
      collect_type_deps needed t
  | TFun(ret, params, _, _) ->
      collect_type_deps needed ret;
      (match params with
       | None -> ()
       | Some plist ->
           List.iter (fun (_, t, _) -> collect_type_deps needed t) plist)
  | _ -> ()

(* Collect dependencies from an initializer *)
let collect_init_deps needed init =
  let open Cil_types in
  let rec visit_init = function
    | SingleInit exp -> visit_exp exp
    | CompoundInit (_typ, offset_init_list) ->
        List.iter (fun (_offset, init) -> visit_init init) offset_init_list
  and visit_exp exp =
    collect_type_deps needed (Cil.typeOf exp);
    match exp.enode with
    | Const (CEnum ei) ->
        needed.enuminfos <- Cil_datatype.Enuminfo.Set.add ei.eihost needed.enuminfos
    | Lval lv | AddrOf lv | StartOf lv -> visit_lval lv
    | UnOp (_op, e, _typ) -> visit_exp e
    | BinOp (_op, e1, e2, _typ) -> visit_exp e1; visit_exp e2
    | CastE (_typ, e) -> visit_exp e
    | _ -> ()
  and visit_lval (lhost, _offset) =
    match lhost with
    | Var vi when vi.vglob ->
        if Cil.isFunctionType vi.vtype then
          needed.functions <- VarSet.add vi needed.functions
        else
          needed.varinfos <- VarSet.add vi needed.varinfos
    | _ -> ()
  in
  visit_init init

(* Visitor to collect all dependencies from a function body *)
class body_dep_collector needed = object
  inherit Visitor.frama_c_inplace

  method! vvrbl vi =
    if vi.Cil_types.vglob then begin
      if Cil.isFunctionType vi.Cil_types.vtype then
        needed.functions <- VarSet.add vi needed.functions
      else
        needed.varinfos <- VarSet.add vi needed.varinfos
    end;
    Cil.SkipChildren

  method! vexpr exp =
    (match exp.Cil_types.enode with
     | Cil_types.Const (Cil_types.CEnum ei) ->
         needed.enuminfos <- Cil_datatype.Enuminfo.Set.add ei.Cil_types.eihost needed.enuminfos
     | _ -> ());
    Cil.DoChildren

  method! vtype typ =
    collect_type_deps needed typ;
    Cil.SkipChildren
end

(* Unified dependency collection: worklist fixpoint over functions + globals *)
let collect_all_deps needed context_kfs all_globals_varset =
  let processed_funcs = ref VarSet.empty in
  let processed_vars = ref VarSet.empty in

  (* Seed: context functions *)
  List.iter (fun kf ->
    needed.functions <- VarSet.add (Kernel_function.get_vi kf) needed.functions
  ) context_kfs;

  (* Seed: all globals from JSON analysis *)
  VarSet.iter (fun vi ->
    needed.varinfos <- VarSet.add vi needed.varinfos
  ) all_globals_varset;

  (* Iterate until fixpoint *)
  let changed = ref true in
  while !changed do
    changed := false;

    (* Process unprocessed functions *)
    let unprocessed_funcs = VarSet.diff needed.functions !processed_funcs in
    if not (VarSet.is_empty unprocessed_funcs) then begin
      changed := true;
      VarSet.iter (fun vi ->
        processed_funcs := VarSet.add vi !processed_funcs;
        (* Collect signature types *)
        collect_type_deps needed vi.Cil_types.vtype;
        (* Traverse body if available *)
        (try
          let kf = Globals.Functions.get vi in
          let fundec = Kernel_function.get_definition kf in
          (* Local variable types *)
          List.iter (fun lvi ->
            collect_type_deps needed lvi.Cil_types.vtype
          ) fundec.Cil_types.slocals;
          (* Formal parameter types *)
          List.iter (fun fvi ->
            collect_type_deps needed fvi.Cil_types.vtype
          ) fundec.Cil_types.sformals;
          (* Body: globals, called functions, types from expressions *)
          let visitor = new body_dep_collector needed in
          ignore (Visitor.visitFramacFunction visitor fundec)
        with Kernel_function.No_Definition | Not_found -> ())
      ) unprocessed_funcs
    end;

    (* Process unprocessed variables *)
    let unprocessed_vars = VarSet.diff needed.varinfos !processed_vars in
    if not (VarSet.is_empty unprocessed_vars) then begin
      changed := true;
      VarSet.iter (fun vi ->
        processed_vars := VarSet.add vi !processed_vars;
        (* Collect variable type *)
        collect_type_deps needed vi.Cil_types.vtype;
        (* Collect initializer dependencies *)
        (try
          let initinfo = Globals.Vars.find vi in
          match initinfo.Cil_types.init with
          | None -> ()
          | Some init -> collect_init_deps needed init
        with Not_found -> ())
      ) unprocessed_vars
    end
  done

(* ========================================================================= *)
(* C Code Output - Original Order Extractor *)
(* ========================================================================= *)

(* Check if a global is needed *)
let is_global_needed needed g =
  let open Cil_types in
  match g with
  | GType(ti, _) -> Cil_datatype.Typeinfo.Set.mem ti needed.typeinfos
  | GCompTag(ci, _) | GCompTagDecl(ci, _) -> Cil_datatype.Compinfo.Set.mem ci needed.compinfos
  | GEnumTag(ei, _) | GEnumTagDecl(ei, _) -> Cil_datatype.Enuminfo.Set.mem ei needed.enuminfos
  | GVar(vi, _, _) | GVarDecl(vi, _) ->
      if Cil.isFunctionType vi.vtype then
        VarSet.mem vi needed.functions
      else
        VarSet.mem vi needed.varinfos
  | GFun(fundec, _) -> VarSet.mem fundec.svar needed.functions
  | _ -> false

(* Transform GFun to GVarDecl (function declaration without body) *)
let transform_global g =
  let open Cil_types in
  match g with
  | GFun(fundec, loc) -> GVarDecl(fundec.svar, loc)
  | _ -> g

(* Classify a global for ordering: function declarations first,
   then types/enums/structs, then variables, preserving relative
   order within each group.  This prevents struct initializers
   that reference function pointers from appearing before the
   corresponding forward declaration. *)
let global_sort_key g =
  let open Cil_types in
  match g with
  | GType _ | GCompTag _ | GCompTagDecl _ | GEnumTag _ | GEnumTagDecl _ -> 0
  | GVarDecl(vi, _) when Cil.isFunctionType vi.vtype -> 1
  | _ -> 2

(* Extract globals in dependency-safe order *)
let extract_in_order needed =
  let all_globals = (Ast.get()).Cil_types.globals in
  let filtered = List.filter (is_global_needed needed) all_globals in
  let transformed = List.map transform_global filtered in
  (* Stable sort: function decls first, types second, vars/arrays last *)
  let indexed = List.mapi (fun i g -> (i, g)) transformed in
  let sorted = List.sort (fun (i1, g1) (i2, g2) ->
    let k1 = global_sort_key g1 and k2 = global_sort_key g2 in
    if k1 <> k2 then compare k1 k2 else compare i1 i2
  ) indexed in
  List.map snd sorted

(* ========================================================================= *)
(* C Code Output - Code Generation *)
(* ========================================================================= *)

(* Output C code for a context *)
let output_c_code context_id functions globals output_file =
  try
    let oc = open_out output_file in
    let fmt = Format.formatter_of_out_channel oc in

    (* Header comment *)
    Format.fprintf fmt "/* Generated by Frama-C Global Dependencies Plugin */@\n";
    Format.fprintf fmt "/* Context: %s */@\n" context_id;
    Format.fprintf fmt "/* Functions: %s */@\n@\n"
      (String.concat ", " functions);

    (* Output all globals in order *)
    List.iter (fun g ->
      Format.fprintf fmt "%a@\n" Printer.pp_global g
    ) globals;

    Format.pp_print_flush fmt ();
    close_out oc;
    Self.result "C code written to %s" output_file
  with Sys_error msg ->
    Self.error "Failed to write C code: %s" msg

(* ========================================================================= *)
(* JSON Input Parsing *)
(* ========================================================================= *)

(* Parse input JSON file *)
let parse_input_json filename =
  try
    let json = Yojson.Basic.from_file filename in
    match json with
    | `Assoc fields ->
        (match List.assoc_opt "contexts" fields with
         | Some (`List contexts) ->
             List.filter_map (fun ctx ->
               match ctx with
               | `Assoc ctx_fields ->
                   let id = match List.assoc_opt "id" ctx_fields with
                     | Some (`String s) -> s
                     | _ -> "unknown"
                   in
                   let functions = match List.assoc_opt "functions" ctx_fields with
                     | Some (`List funcs) ->
                         List.filter_map (function
                           | `String s -> Some s
                           | _ -> None
                         ) funcs
                     | _ -> []
                   in
                   Some { id; functions }
               | _ -> None
             ) contexts
         | _ ->
             Self.error "Invalid JSON format: missing 'contexts' array";
             [])
    | _ ->
        Self.error "Invalid JSON format: expected object at root";
        []
  with
  | Sys_error msg ->
      Self.error "Failed to read input file: %s" msg;
      []
  | Yojson.Json_error msg ->
      Self.error "JSON parse error: %s" msg;
      []

(* ========================================================================= *)
(* Context Analysis *)
(* ========================================================================= *)

(* Analyze a single context *)
let analyze_context dep_graph (context : context_input)
    : context_output * VarSet.t * Kernel_function.t list =
  Self.feedback "Analyzing context '%s' with %d functions..."
    context.id (List.length context.functions);

  (* Resolve function names *)
  let resolved_funcs = List.filter_map resolve_function context.functions in
  let missing = List.filter (fun name ->
    not (List.exists (fun kf ->
      Kernel_function.get_name kf = name
    ) resolved_funcs)
  ) context.functions in

  (* Collect direct globals *)
  let direct_globals = List.fold_left (fun acc kf ->
    let globals = collect_globals_for_function kf in
    VarSet.union acc globals
  ) VarSet.empty resolved_funcs in

  (* Compute transitive closure *)
  let all_globals = transitive_closure direct_globals dep_graph in

  (* Build dependency list *)
  let dependencies = VarSet.fold (fun vi acc ->
    try
      let deps = Hashtbl.find dep_graph vi in
      let dep_names = List.map (fun v -> v.Cil_types.vname) (VarSet.elements deps) in
      (vi.Cil_types.vname, dep_names) :: acc
    with Not_found ->
      (vi.Cil_types.vname, []) :: acc
  ) all_globals [] in

  let output = {
    id = context.id;
    functions = context.functions;
    missing_functions = missing;
    direct_globals = List.map (fun v -> v.Cil_types.vname) (VarSet.elements direct_globals);
    all_globals = List.map (fun v -> v.Cil_types.vname) (VarSet.elements all_globals);
    dependencies = List.sort (fun (a,_) (b,_) -> String.compare a b) dependencies;
  } in
  (output, all_globals, resolved_funcs)

(* ========================================================================= *)
(* JSON Output Generation *)
(* ========================================================================= *)

(* Convert context output to JSON *)
let context_to_json ctx =
  `Assoc [
    ("id", `String ctx.id);
    ("functions", `List (List.map (fun s -> `String s) ctx.functions));
    ("missing_functions", `List (List.map (fun s -> `String s) ctx.missing_functions));
    ("direct_globals", `List (List.map (fun s -> `String s) ctx.direct_globals));
    ("all_globals", `List (List.map (fun s -> `String s) ctx.all_globals));
    ("dependencies", `Assoc (List.map (fun (name, deps) ->
      (name, `List (List.map (fun d -> `String d) deps))
    ) ctx.dependencies));
  ]

(* Write JSON output to file *)
let write_json_output filename contexts =
  try
    let json = `Assoc [
      ("contexts", `List (List.map context_to_json contexts))
    ] in
    let oc = open_out filename in
    let fmt = Format.formatter_of_out_channel oc in
    Yojson.Basic.pretty_print fmt json;
    Format.fprintf fmt "@.";
    close_out oc;
    Self.result "JSON output written to %s" filename
  with Sys_error msg ->
    Self.error "Failed to write output file: %s" msg

(* ========================================================================= *)
(* Main Entry Point *)
(* ========================================================================= *)

let main () =
  if Enabled.get () then begin
    (* Validate input file parameter *)
    let input_file = InputFile.get () in
    if input_file = "" then begin
      Self.error "No input file specified. Use -global-deps-input <file>";
      exit 1
    end;

    (* Ensure AST is computed *)
    Ast.compute ();
    Self.result "AST computed";

    (* Parse input JSON *)
    Self.feedback "Parsing input file: %s" input_file;
    let contexts = parse_input_json input_file in

    if contexts = [] then begin
      Self.error "No contexts found in input file";
      exit 1
    end;

    Self.result "Found %d contexts to analyze" (List.length contexts);

    (* Build global dependency graph once *)
    Self.feedback "Building global variable dependency graph...";
    let dep_graph = build_dep_graph () in
    Self.result "Dependency graph built";

    (* Analyze each context *)
    let results_with_deps = List.map (analyze_context dep_graph) contexts in
    let results = List.map (fun (out, _, _) -> out) results_with_deps in

    (* Write JSON output *)
    let output_file = OutputFile.get () in
    Self.feedback "Writing results to %s..." output_file;
    write_json_output output_file results;

    (* C code output *)
    let c_output_dir = COutputDir.get () in
    if c_output_dir <> "" then begin
      Self.feedback "Generating C code output...";

      (* Create output directory *)
      (try Unix.mkdir c_output_dir 0o755
       with Unix.Unix_error (Unix.EEXIST, _, _) -> ()
          | Unix.Unix_error (e, _, _) ->
              Self.warning "Failed to create directory %s: %s"
                c_output_dir (Unix.error_message e));

      (* For each context *)
      List.iter (fun (ctx_output, all_globals_varset, resolved_funcs) ->
        Self.feedback "Generating C code for context '%s'..." ctx_output.id;

        (* Collect all dependencies: types, globals, functions *)
        let needed = create_needed_set () in
        collect_all_deps needed resolved_funcs all_globals_varset;

        (* Extract in order *)
        let globals = extract_in_order needed in

        (* Output C code *)
        let output_file = Filename.concat c_output_dir (ctx_output.id ^ ".c") in
        output_c_code ctx_output.id ctx_output.functions globals output_file
      ) results_with_deps;

      Self.result "C code generation complete!"
    end;

    Self.result "Analysis complete!"
  end

(* Register with boot system *)
let () = Boot.Main.extend main
