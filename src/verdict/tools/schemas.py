"""JSON schemas and descriptions for agent-facing MCP tools."""

from __future__ import annotations

from typing import Any


TOOL_SCHEMAS: dict[str, dict[str, Any]] = {
    "verify_function_contract": {
        "type": "object",
        "properties": {
            "annotations": {
                "type": "object",
                "description": (
                    "ACSL annotations dict. Keys: "
                    "'preconditions' (list[str]): each string is a full ACSL clause with keyword, e.g. 'requires \\\\valid_read(p);'. "
                    "'postconditions' (list[str]): each string starts with 'ensures', e.g. 'ensures \\\\result >= 0;'. "
                    "'assigns' (list[str]): each string starts with 'assigns', e.g. 'assigns \\\\nothing;'. "
                    "'loop_contracts' (dict[str, list[str]]): key is the loop name from //loop_name comment in source code "
                    "(e.g. 'bufs_differ_loop_0'), value is list of loop clauses, each starting with 'loop invariant', "
                    "'loop assigns', or 'loop variant'."
                ),
                "properties": {
                    "preconditions": {"type": "array", "items": {"type": "string"}},
                    "postconditions": {"type": "array", "items": {"type": "string"}},
                    "assigns": {"type": "array", "items": {"type": "string"}},
                    "loop_contracts": {"type": "object", "additionalProperties": {"type": "array", "items": {"type": "string"}}},
                },
            },
            "callee_contracts": {
                "type": "object",
                "description": (
                    "Optional callee hypothesis: {callee_name: {postconditions: [...], assigns: [...]}}. "
                    "Only postconditions and assigns are accepted."
                ),
            },
            "assume_callee": {
                "type": "array",
                "items": {"type": "string"},
                "description": (
                    "Opt-out list: callee names to verify under YOUR hypothesis "
                    "instead of their published spec. Default empty (published "
                    "wins for soundness). Use when you have an open proof_demand "
                    "and want to keep your own postconditions provable in the "
                    "meantime — each name in this list must also appear in "
                    "`callee_contracts` with your assumed clauses (include the "
                    "published clauses you still rely on: your hypothesis "
                    "REPLACES the published spec, it is not merged). This is "
                    "the ONLY place the assume set is chosen; it is saved into "
                    "your draft and inherited by every other verify tool."
                ),
            },
        },
        "required": ["annotations"],
    },
    "check_acsl_syntax": {
        "type": "object",
        "properties": {
            "annotations": {
                "type": "object",
                "description": (
                    "ACSL annotations dict to syntax-check. Same format as verify_function_contract annotations: "
                    "preconditions (list of 'requires ...;'), postconditions (list of 'ensures ...;'), "
                    "assigns (list of 'assigns ...;'), loop_contracts ({loop_name: [clauses]})."
                ),
            },
        },
        "required": ["annotations"],
    },
    "get_function_context": {
        "type": "object",
        "properties": {
            "depth": {
                "type": "integer",
                "description": "Call-graph traversal depth (default 1).",
                "default": 1,
            },
            "include_callers": {
                "type": "boolean",
                "description": "Whether to include caller information (default false).",
                "default": False,
            },
        },
        "required": [],
    },
    "apply_precondition_weakening": {
        "type": "object",
        "properties": {
            "original_precondition": {
                "type": "string",
                "description": "The precondition to weaken.",
            },
            "weakened_precondition": {
                "type": "string",
                "description": "Optional replacement precondition (omit to remove entirely).",
            },
        },
        "required": ["original_precondition"],
    },
    "check_inbox": {
        "type": "object",
        "properties": {},
        "required": [],
    },
    "send_spec_request": {
        "type": "object",
        "properties": {
            "target_function": {
                "type": "string",
                "description": "Name of the function to request a spec from.",
            },
            "request_type": {
                "type": "string",
                "description": "Type of spec requested: 'precondition', 'postcondition', 'loop_contract', or 'assigns'.",
            },
            "required_property": {
                "type": "string",
                "description": "Description of the property needed.",
            },
            "context": {
                "type": "object",
                "description": "Optional context for the request.",
            },
        },
        "required": ["target_function", "request_type", "required_property"],
    },
    "send_spec_response": {
        "type": "object",
        "properties": {
            "requester_id": {
                "type": "string",
                "description": "Agent ID of the requester.",
            },
            "target_function": {
                "type": "string",
                "description": "Function the spec is about.",
            },
            "request_type": {
                "type": "string",
                "description": "Type of spec: 'precondition', 'postcondition', 'loop_contract', or 'assigns'.",
            },
            "status": {
                "type": "string",
                "description": "Status: 'proven', 'failed', or 'pending'.",
            },
            "specification": {
                "type": "string",
                "description": "The specification text (if proven).",
            },
            "error_message": {
                "type": "string",
                "description": "Error message (if failed).",
            },
        },
        "required": ["requester_id", "target_function", "request_type", "status"],
    },
    "notify_spec_update": {
        "type": "object",
        "properties": {
            "spec_type": {
                "type": "string",
                "description": "Type of spec that changed: 'precondition', 'postcondition', 'loop_contract', or 'assigns'.",
            },
            "old_value": {
                "type": "string",
                "description": "Previous value of the spec.",
            },
            "new_value": {
                "type": "string",
                "description": "New value of the spec.",
            },
        },
        "required": ["spec_type"],
    },
    "submit_proof_demand": {
        "type": "object",
        "properties": {
            "target_function": {
                "type": "string",
                "description": "Owner function that should consider proving the requested property.",
            },
            "property_kind": {
                "type": "string",
                "description": (
                    "Kind of property: 'postcondition', 'assigns', or "
                    "'precondition_weakening' (request callee to relax a precondition)."
                ),
            },
            "property_text": {
                "type": "string",
                "description": "Concrete ACSL-like property text or obligation summary that would help verification.",
            },
            "blocking_vo_ids": {
                "type": "array",
                "items": {"type": "string"},
                "description": "Optional VO IDs currently blocked by the missing property.",
            },
            "rationale": {
                "type": "string",
                "description": "Why this property would help the requester.",
            },
        },
        "required": ["target_function", "property_kind", "property_text"],
    },
    "resolve_proof_demand": {
        "type": "object",
        "properties": {
            "demand_id": {
                "type": "string",
                "description": "ID of the proof demand to resolve (e.g. 'demand_1').",
            },
            "resolution": {
                "type": "string",
                "description": "Resolution status: 'satisfied' or 'rejected'.",
            },
            "message": {
                "type": "string",
                "description": "Optional message explaining the resolution.",
            },
        },
        "required": ["demand_id", "resolution"],
    },
    "mark_vo_as_bug": {
        "type": "object",
        "properties": {
            "vo_id": {
                "type": "string",
                "description": "ID of the VO to mark as bug (e.g. 'rte_3' or 'call_site_0').",
            },
            "reason": {
                "type": "string",
                "description": "Explanation of why this VO cannot be proven (required).",
            },
        },
        "required": ["vo_id", "reason"],
    },
    "verify_rte_assertions": {
        "type": "object",
        "properties": {
            "vo_ids": {
                "type": "array",
                "items": {"type": "string"},
                "description": "List of RTE VO IDs to verify (e.g. ['rte_1', 'rte_3']).",
            },
        },
        "required": ["vo_ids"],
    },
    "verify_callsite_preconditions": {
        "type": "object",
        "properties": {
            "callee_function": {
                "oneOf": [
                    {"type": "string"},
                    {"type": "array", "items": {"type": "string"}},
                ],
                "description": (
                    "Name of the callee function (string) or list of callee "
                    "function names (array of strings) whose preconditions to "
                    "verify at call sites. When a list is passed, all callees "
                    "are verified in one tool call (one WP invocation per "
                    "callee internally) and results are returned per-callee."
                ),
            },
        },
        "required": ["callee_function"],
    },
    "read_function_spec": {
        "type": "object",
        "properties": {
            "function_name": {
                "type": "string",
                "description": "Name of the function whose latest published spec to read.",
            },
        },
        "required": ["function_name"],
    },
    "read_my_spec": {
        "type": "object",
        "properties": {},
        "required": [],
    },
    "build_spec_deps": {
        "type": "object",
        "properties": {
            "dependencies": {
                "type": "object",
                "description": (
                    "Per-callee postcondition dependency declaration: "
                    "{callee_name: [postcondition_clause, ...]}. "
                    "Non-empty list = 'my proof relies on exactly these clauses'. "
                    "Empty list [] = 'my proof does not depend on this callee's "
                    "postconditions at all'. Each clause must be an exact string "
                    "from the callee's published postconditions."
                ),
                "additionalProperties": {
                    "type": "array",
                    "items": {"type": "string"},
                },
            },
        },
        "required": ["dependencies"],
    },
    "update_my_spec": {
        "type": "object",
        "properties": {
            "spec": {
                "type": "object",
                "description": (
                    "FunctionSpec as a dict with keys: function_name (str), "
                    "preconditions (list[str]), postconditions (list[str]), "
                    "loop_contracts (dict), assigns (list[str]), "
                    "status ('draft' or 'published'), callee_contracts (dict, "
                    "optional — your optimistic hypothesis about callees)."
                ),
                "properties": {
                    "function_name": {"type": "string"},
                    "preconditions": {"type": "array", "items": {"type": "string"}},
                    "postconditions": {"type": "array", "items": {"type": "string"}},
                    "loop_contracts": {"type": "object"},
                    "assigns": {"type": "array", "items": {"type": "string"}},
                    "status": {"type": "string"},
                    "based_on_versions": {"type": "object"},
                    "callee_contracts": {"type": "object"},
                },
                "required": ["function_name"],
            },
        },
        "required": ["spec"],
    },
}


# Tool descriptions for MCP registration.
#
# Each description is self-sufficient: it states what the tool does, when to
# call it, when NOT to call it, the input format, and any gotchas. The system
# prompt does not duplicate this information — the LLM should consult each
# tool's own description before calling it.
TOOL_DESCRIPTIONS: dict[str, str] = {
    # ------------------------------------------------------------------
    # Context + syntax
    # ------------------------------------------------------------------
    "get_function_context": (
        "Return your function's source code (with //loop_name comments marking "
        "each loop), callees, callers, loop metadata, and globals. The "
        "//loop_name values are the keys for loop_contracts in annotations."
    ),
    "check_acsl_syntax": (
        "Fast ACSL syntax check without running WP. Same `annotations` dict "
        "as verify_function_contract. Use to catch typos before a full run.\n\n"
        "COMMON ACSL PITFALLS to remember:\n"
        "  • Implication is `==>`, NOT `=>`.\n"
        "  • Inside `loop invariant`, use `\\at(var, Pre)` NOT `\\old(var)`. "
        "`\\old` only works inside `ensures` clauses.\n"
        "  • Variables inside `requires` must be function parameters or "
        "globals — local-variable names are not in scope.\n"
        "  • Loop names in `loop_contracts` keys must EXACTLY match the "
        "`//function_name_loop_N` comment in the source. Generic names "
        "like `while_loop` will not bind."
    ),
    # ------------------------------------------------------------------
    # Verification
    # ------------------------------------------------------------------
    "verify_function_contract": (
        "Verify your function's contract via Frama-C WP. Runs the worklist "
        "(loop contracts → postconditions → assigns) and returns per-clause "
        "proved/failed status.\n\n"
        "INPUT:\n"
        "- `annotations`: your own {preconditions, postconditions, assigns, "
        "loop_contracts}. All strings must include the ACSL keyword.\n"
        "- `callee_contracts` (optional): optimistic hypothesis ONLY for "
        "callees that have NOT published yet. Format: "
        "{callee_name: {postconditions, assigns}}. Only postconditions and "
        "assigns are accepted (a caller cannot dictate what a callee "
        "requires).\n\n"
        "WHAT MAKES A COMPLETE CONTRACT (apply ALL relevant rules every "
        "time — incomplete contracts force callers to demand more later, "
        "extending convergence by N cycles):\n"
        "  • Globals — state the memory the proof relies on for globals "
        "just as you do for parameters: `requires \\valid_read(tbl + "
        "(0 .. N-1));` (`\\valid` if written). For a 2D array keep the "
        "array type — `tbl + (0..ROWS-1) + (0..COLS-1)` — NOT a "
        "flattened cast like `(uint8_t *)tbl + (0..bytes)`, which the "
        "simplifier cannot exploit. In WP's memory model a mentioned "
        "global becomes its own memory base, so its separation from "
        "pointer params is derived automatically and goals stay small; "
        "initializers of const globals (element values, stored pointers) "
        "are likewise visible to provers.\n"
        "  • `\\separated` — ESSENTIAL and the single most common cause of "
        "WP failure. Add `requires \\separated(...)` whenever the function "
        "has (a) multiple pointer parameters, (b) reads from one pointer "
        "and writes to another, or (c) any pointer params that should not "
        "alias. Even if non-aliasing is 'obvious', WP needs it explicit. "
        "For param-vs-global pairs the validity mention above already "
        "gives separation — a casted byte-range `\\separated(p, "
        "(uint8_t *)tbl+(0..bytes))` restates it in a form the simplifier "
        "cannot exploit.\n"
        "  • `\\result` completeness — `ensures \\result == 0;` is too "
        "strong if the function can return errors. Use implication form "
        "and cover ALL return cases: `ensures (\\result == 0) ==> (...)`, "
        "`ensures \\result == 0 || \\result == -1 || \\result == -2;`, etc. "
        "Bound output-pointer values on success: `ensures \\result == 0 "
        "==> (*eaten >= 1 && *eaten <= len);`. Bound EVERY output param.\n"
        "  • Frame-rule postconditions — if your function advances a "
        "buffer (e.g. `*eaten` consumed), publish a postcondition stating "
        "the unconsumed tail is still valid: `ensures \\result == 0 && "
        "buf != \\null ==> \\valid_read((buf + *eaten) + (0 .. (len - "
        "*eaten) - 1));`. Callers that chain into the next call need this.\n"
        "  • Loop invariants are inductive — must hold initially AND be "
        "preserved. Preconditions do NOT auto-flow into loops. For each "
        "loop: `loop invariant 0 <= i <= len;` (bounds), restate buffer "
        "validity (`loop invariant \\valid_read(buf+(0..len-1));`), state "
        "any tracked relationships (`loop invariant buf == "
        "\\at(buf,Pre) + i;`). Use `\\at(...,Pre)` not `\\old()` inside "
        "loop invariants.\n"
        "  • `loop assigns` lists ALL locations the loop modifies — not "
        "just the iteration variable. If body writes `arr[i]`, write "
        "`loop assigns i, arr[0..n-1];`. Missing entries fail the assigns "
        "check and cascade.\n"
        "  • Function-level `assigns` must include EVERY pointer "
        "dereference and transitively-modified location through callees. "
        "`assigns \\nothing;` only if truly nothing leaks out. Local "
        "variables are NOT included in function-level assigns.\n"
        "  • Callee completeness — every entry in `callee_contracts` MUST "
        "include `assigns` alongside any `postconditions`. Postconditions "
        "alone are insufficient when WP needs to reason about the "
        "callee's frame (which memory the call may modify). NOTE: only "
        "`postconditions` and `assigns` are accepted for callees; "
        "`preconditions` and `loop_contracts` are rejected by the tool.\n"
        "  • Empty-range trick — `\\valid_read(buf+(0..len-1))` with "
        "`len == 0` is the empty range, trivially valid. So `requires "
        "len > 0;` is usually NOT needed to 'protect' a `\\valid_read`. "
        "Don't add over-restrictive guards that exclude edge cases the "
        "function handles internally.\n\n"
        "CALLEE CONTRACT RESOLUTION (the WP context for callees):\n"
        "  1. callee in `assume_callee` → uses YOUR `callee_contracts[callee]` "
        "even if the callee is published. The explicit 'verify under my open "
        "hypothesis' path. Use this when you have an open proof_demand for "
        "the assumed property and don't want to drop your own postconditions "
        "while waiting. You MUST also pass the assumed clauses in "
        "`callee_contracts[callee]`.\n"
        "  2. callee already published (and not assumed) → uses "
        "callee.published_spec (canonical; your `callee_contracts` is "
        "OVERRIDDEN). If you need stronger guarantees, submit_proof_demand AND "
        "add the callee to `assume_callee` to keep verifying meanwhile.\n"
        "  3. callee NOT published + you provided `callee_contracts[callee]` "
        "→ uses your hypothesis (true optimistic case for unpublished callees).\n"
        "  4. callee NOT published + nothing provided → callee stub has no "
        "annotations (weak proof environment; expect post-call RTE failures).\n\n"
        "ON SUCCESS: writes the verified annotations + your RAW "
        "`callee_contracts` (NOT the merged version) + the `assume_callee` "
        "set into your working draft. The draft IS the proof context: "
        "verify_rte_assertions, verify_callsite_preconditions, and "
        "apply_precondition_weakening all inherit the hypothesis AND the "
        "assume set from it, so every proof of one draft lives in one "
        "consistent world. So you do NOT need to call update_my_spec(draft) "
        "afterwards. NOTE: if this draft changes the proof context vs your "
        "previous one (preconditions, loop contracts, callee hypothesis, or "
        "assume set), previously verified RTE/call-site VOs are reset to "
        "PENDING — they were proven in the old context and must be re-run. "
        "Result fields `draft_saved`, `draft_version`, and "
        "`draft_callee_contracts_count` confirm persistence."
    ),
    "verify_rte_assertions": (
        "Verify specific RTE VOs (signed overflow, array bounds, null deref) "
        "via -wp-prop targeting. Updates each VO to VERIFIED or FAILED.\n\n"
        "DATA SOURCE: your working draft (saved by the last successful "
        "verify_function_contract) — your annotations, your raw "
        "`callee_contracts` hypothesis, AND the assume set, all inherited "
        "from it. Published specs win for every callee not in the draft's "
        "assume set (sound). To verify RTEs under a hypothesis (e.g. while "
        "a proof_demand is open), re-run verify_function_contract with "
        "`assume_callee` first so the draft records it."
    ),
    "verify_callsite_preconditions": (
        "Verify that your call sites satisfy a TARGET callee's preconditions. "
        "Pass a single callee name (string) or a list of callee names (array "
        "of strings). When a list is passed, call-site preconditions for ALL "
        "listed callees are verified in one tool call (one WP invocation per "
        "callee internally). Results are returned per-callee so you can see "
        "exactly which callee's preconditions passed or failed at each call "
        "site. For functions with many callees, prefer passing all callees "
        "at once to avoid exhausting turn limits.\n\n"
        "Fetches each target's preconditions from its published spec "
        "automatically. Updates per-(call-site, precondition) child VOs "
        "individually.\n\n"
        "SIBLING CALLEES: when a function calls multiple callees in sequence, "
        "the target callee's call-site precondition often depends on what an "
        "earlier sibling callee left behind (e.g. parse_id_len calls "
        "get_identifier then get_length; proving get_length's precondition "
        "needs get_identifier's frame postcondition). The tool passes "
        "sibling callee contracts to WP automatically, resolved exactly as "
        "in verify_function_contract: published spec by default, your draft "
        "hypothesis for siblings in the draft's assume set.\n\n"
        "DATA SOURCE: your working draft for your own annotations + "
        "callee_contracts + assume set (one proof context per draft); the "
        "target callee's published spec for its preconditions. Returns a "
        "soft skip if the target callee has not published yet."
    ),
    "apply_precondition_weakening": (
        "Weaken or remove one of your own preconditions and compute the "
        "cascade: re-verifies remaining specs and re-checks all previously "
        "VERIFIED VOs. VOs that no longer hold become FAILED.\n\n"
        "WHEN TO WEAKEN — the call-site failure pattern:\n"
        "Your precondition is FAILING at a caller's call site. Two cases:\n"
        "  (a) Your function uses defensive programming for that input "
        "(e.g. `if (len == 0) return -1;`) and the caller passes the edge "
        "value and checks the return → your precondition is OVERLY "
        "RESTRICTIVE. Weaken it.\n"
        "  (b) Your function would actually exhibit undefined behavior "
        "without that input constraint and has NO internal check → "
        "do NOT weaken; the caller needs to satisfy the precondition. "
        "Submit a proof_demand to the caller's caller, or let the caller "
        "tighten its own contract.\n\n"
        "THREE WEAKENING STRATEGIES (pick the one that fits):\n"
        "  1. Minimum-bound — function checks `if (len == 0) return ERR;` "
        "→ change `requires len > 0;` to `requires len >= 0;` (keeps a "
        "reasonable lower bound for unsigned-like params).\n"
        "  2. Implication guard — keep the memory-safety predicate but "
        "guard it: change `requires \\valid_read(buf+(0..len-1));` to "
        "`requires (len > 0 && buf != \\null) ==> \\valid_read(buf+(0..len-1));`. "
        "When the antecedent is false the implication is trivially true, "
        "letting the caller pass the edge case while preserving safety.\n"
        "  3. REMOVE — only when the precondition is fully redundant with "
        "another precondition or the function handles all cases internally "
        "with no memory-safety concern.\n\n"
        "COORDINATE WITH OTHER PRECONDITIONS: weakening one may force "
        "adjustment of another (e.g. weakening `len > 0` to `len >= 0` "
        "may require turning a bare `\\valid_read` into the implication "
        "form above). Think about the contract as a whole."
    ),
    # ------------------------------------------------------------------
    # Cross-agent coordination (runtime-mediated)
    # ------------------------------------------------------------------
    "read_function_spec": (
        "Read another function's latest published spec. Returns "
        "{found, spec | None}. Use to check callee contracts before "
        "verification or before submitting a proof demand."
    ),
    "read_my_spec": (
        "Read your OWN current working spec, including your callee_contracts "
        "hypothesis. Use after a wake-up to inspect what you previously "
        "saved (verify_function_contract auto-saves a draft on success). "
        "Returns {found, spec} where spec includes preconditions, "
        "postconditions, loop_contracts, assigns, callee_contracts, "
        "version, status."
    ),
    "build_spec_deps": (
        "Declare which callee postconditions your proof depends on. "
        "Your entire proof (postconditions, assigns, loop contracts, RTE, "
        "call-site obligations) may depend on callee guarantees. This tool "
        "lets you declare the specific postcondition clauses you rely on, "
        "per callee. The harness verifies the declaration mechanically.\n\n"
        "WHEN: call after verify_function_contract succeeds, before publish. "
        "Declared callees must already be published.\n\n"
        "IF YOU DON'T CALL THIS: all callees get a conservative ALL "
        "dependency — any callee guarantee change forces full re-verification.\n\n"
        "IF YOU DO CALL THIS: only changes to your declared clauses will "
        "wake you — precise discharge, fewer unnecessary re-verifications.\n\n"
        "assigns are always treated as dependencies (upper bound); only "
        "postconditions need explicit declaration.\n\n"
        "SUFFICIENCY TARGET SET: the declared postconditions must be "
        "jointly sufficient to discharge ALL of:\n"
        "  (1) your own spec (postconditions, assigns, loop contracts),\n"
        "  (2) your RTE verification obligations,\n"
        "  (3) every callee's preconditions at each call site (call-site VOs).\n"
        "On rejection the harness reports each failing goal by type — "
        "[spec], [rte], or [call-site] with the callee name and "
        "precondition clause — so you can identify exactly which "
        "postcondition to add to your declaration."
    ),
    "submit_proof_demand": (
        "Ask another function's owner to act on a property you need. "
        "The runtime wakes the target. Accepted property_kind values:\n"
        "- 'postcondition' / 'assigns': request a guarantee from the callee.\n"
        "- 'precondition_weakening': request the callee to weaken or remove "
        "a precondition you cannot satisfy at the call site. property_text "
        "is the specific clause (e.g. 'requires x > 0;')."
    ),
    "resolve_proof_demand": (
        "Resolve a proof demand addressed to you.\n\n"
        "Before resolving, evaluate the demand internally:\n\n"
        "For postcondition/assigns demands (caller needs you to ADD a guarantee):\n"
        "- Check whether your existing spec and code logic already imply the "
        "requested property (it may already hold but not be stated explicitly).\n"
        "- If not already implied, determine what annotation changes you need "
        "to make it provable. Add the clause and verify with "
        "verify_function_contract.\n"
        "- If verified, publish the updated spec and resolve as 'satisfied'.\n\n"
        "For precondition_weakening demands (caller needs you to REMOVE or "
        "RELAX a precondition):\n"
        "- This is the dangerous case — weakening a precondition may break your "
        "internal proof. Re-verify all your verification obligations after "
        "applying the requested change.\n"
        "- If your proof still holds: safe to apply. Publish and resolve as "
        "'satisfied'.\n"
        "- If your proof breaks: determine what part of your proof depends "
        "on the precondition. Consider whether you can "
        "restructure your spec to satisfy both sides.\n"
        "- If no restructuring works: reject with a clear explanation of why "
        "the change cannot be made, and suggest possible alternatives the "
        "requester could try (e.g. a different postcondition formulation "
        "that would help them).\n\n"
        "Chained demands — when providing the property depends on a "
        "guarantee YOU are missing from one of YOUR callees: submit your "
        "own demand upstream and LEAVE THIS DEMAND OPEN until that "
        "resolves. An open demand is what keeps you scheduled — you will "
        "be woken to finish the chain when the upstream answer arrives. "
        "Do NOT reject with a promise to add the clause later: rejection "
        "closes the requester's durable link to you. Reject only what no "
        "upstream answer could make provable.\n\n"
        "Resolution: 'satisfied' (you published the requested change) or "
        "'rejected' (you cannot provide it without breaking your own proof). "
        "The runtime wakes the requester with the result."
    ),
    "mark_vo_as_bug": (
        "Permanently accept a VO as unprovable for this run. BUG VOs count as "
        "discharged for publication but are tracked and reported separately. "
        "This is irreversible — use only when ALL alternatives are exhausted.\n\n"
        "Do NOT mark if any of the following can still change the outcome:\n"
        "- You have not tried verifying under an explicit hypothesis "
        "(verify_function_contract with `assume_callee` + the assumed "
        "clauses in `callee_contracts`; the draft then carries that "
        "context into verify_rte_assertions / verify_callsite_preconditions)\n"
        "- You have not tried apply_precondition_weakening to relax callee "
        "preconditions that are too strong for your call site\n"
        "- You have not tried strengthening or restructuring your own annotations\n"
        "- You have open proof demands awaiting a response — the callee may "
        "still weaken its preconditions or strengthen its postconditions, which "
        "could make the VO provable. Wait for the wake-up, re-verify, then "
        "decide\n"
        "- The failure is caused by an incorrect callee annotation (e.g. WP "
        "aborts on a malformed precondition) — submit a proof demand asking "
        "the callee to fix it instead of accepting a broken spec as a bug\n"
        "Only mark when you have genuinely exhausted these options. Requires "
        "a reason explaining why the VO cannot be proven.\n\n"
        "The tool rejects invalid uses (verified VOs, call-site VOs with "
        "unpublished callees, parent VOs with children)."
    ),
    "update_my_spec": (
        "Save or publish your function's spec.\n\n"
        "- status='draft': working copy for local iteration. Other tools "
        "(verify_rte_assertions, verify_callsite_preconditions) read from "
        "this to build their proof context.\n"
        "- status='published': publish the VERIFIED draft (the annotations "
        "that verify_function_contract checked and auto-saved). The spec "
        "fields you pass are IGNORED for publish — only the draft's "
        "verified annotations are used, ensuring published == verified. "
        "Rejected if any leaf VO is PENDING or FAILED (call-site VOs for "
        "unpublished callees are exempt)."
    ),
    # ------------------------------------------------------------------
    # Debug-mode only (solve mode ignores these)
    # ------------------------------------------------------------------
    "check_inbox": "Debug/chat mode only. Drain pending direct messages.",
    "send_spec_request": "Debug/chat mode only. Direct spec request to another agent.",
    "send_spec_response": "Debug/chat mode only. Reply to a direct spec request.",
    "notify_spec_update": "Debug/chat mode only. Broadcast a spec change hint.",
}
