# Global Dependencies Plugin for Frama-C

A Frama-C plugin that extracts minimal global variable dependencies for function lists from k-context extraction.

## Overview

This plugin is designed to work with the RTE project's k-context extraction. Given a list of functions (from k-layer call graph slices), it:

1. Collects all global variables used by those functions
2. Analyzes global variable initializer dependencies
3. Computes the transitive closure of dependencies
4. Outputs the minimal set of global variables needed

## Installation

The plugin is located in `src/plugins/global-deps/` and builds automatically with Frama-C:

```bash
dune build
```

## Usage

### Basic Command

```bash
frama-c -global-deps \
        -global-deps-input <input.json> \
        -global-deps-output <output.json> \
        <source_files.c>
```

### Command-Line Options

- `-global-deps` (or `-gd`): Enable the plugin
- `-global-deps-input <file>` (or `-gd-input`): Input JSON file with function lists (required)
- `-global-deps-output <file>` (or `-gd-output`): Output JSON file (default: `global_deps.json`)

### Help

```bash
frama-c -global-deps-help
```

## Input Format

The input JSON file should contain contexts with function lists:

```json
{
  "contexts": [
    {
      "id": "context_1",
      "functions": ["main", "foo", "bar"]
    },
    {
      "id": "context_2",
      "functions": ["baz", "qux"]
    }
  ]
}
```

## Output Format

The output JSON file contains the analysis results:

```json
{
  "contexts": [
    {
      "id": "context_1",
      "functions": ["main", "foo", "bar"],
      "missing_functions": [],
      "direct_globals": ["g1", "g2"],
      "all_globals": ["g1", "g2", "g3", "g4"],
      "dependencies": {
        "g1": ["g3"],
        "g2": ["g4"],
        "g3": [],
        "g4": []
      }
    }
  ]
}
```

### Output Fields

- `id`: Context identifier (from input)
- `functions`: List of function names (from input)
- `missing_functions`: Functions that couldn't be found in the codebase
- `direct_globals`: Global variables directly used by the functions
- `all_globals`: All globals including transitive dependencies
- `dependencies`: Map of global variable to its initializer dependencies

## Example

### Test Program (`test_global_deps.c`)

```c
int g1 = 10;
int g2 = g1 * 2;  // depends on g1
int g3 = g2 + g1; // depends on g2 and g1
int g4 = 42;

void foo() {
    g1++;
    g2++;
}

void bar() {
    g3++;
}

int main() {
    foo();
    bar();
    return 0;
}
```

### Input (`test_input.json`)

```json
{
  "contexts": [
    {
      "id": "context_1",
      "functions": ["main", "foo", "bar"]
    }
  ]
}
```

### Running the Analysis

```bash
frama-c -global-deps \
        -global-deps-input test_input.json \
        -global-deps-output result.json \
        test_global_deps.c
```

### Expected Output

The plugin will:
1. Find that `foo()` uses `g1` and `g2`
2. Find that `bar()` uses `g3`
3. Detect that `g2` depends on `g1` (initializer)
4. Detect that `g3` depends on `g2` and `g1` (initializer)
5. Compute transitive closure: `{g1, g2, g3}`

## Implementation Details

### Architecture

The plugin consists of several key components:

1. **JSON Input Parser**: Parses the input JSON and extracts function lists
2. **Function Resolver**: Maps function names to Frama-C kernel functions
3. **Global Collector**: Uses visitor pattern to traverse function bodies and collect global variable references
4. **Dependency Analyzer**: Analyzes global variable initializers to find dependencies
5. **Transitive Closure**: Computes the complete set of dependencies using a worklist algorithm
6. **JSON Output Generator**: Formats results as JSON

### Key Algorithms

**Global Variable Collection**:
- Uses a visitor class that traverses the function AST
- Collects all `varinfo` nodes where `vglob = true`
- Handles lvalues, address-of, and start-of expressions

**Initializer Dependency Analysis**:
- Recursively traverses initializer expressions
- Handles both `SingleInit` and `CompoundInit` cases
- Extracts global variables referenced in initializer expressions

**Transitive Closure**:
- Worklist algorithm with visited set
- Starts from direct globals
- Iteratively adds dependencies until fixpoint
- Handles cycles gracefully

## Limitations

1. **Syntactic Analysis**: The plugin uses AST traversal (visitor pattern) rather than semantic analysis. This means:
   - It captures textual references to globals
   - It may miss indirect accesses through pointers
   - It doesn't require Eva value analysis (more portable)

2. **Function Definitions**: Only analyzes functions with definitions (bodies). Declarations without definitions are skipped.

3. **Initializer Complexity**: Complex initializers with function calls are handled, but the called functions' effects are not analyzed.

## Troubleshooting

### "Function not found" warnings
- The plugin warns about functions that can't be resolved
- These are listed in the `missing_functions` field
- Check that function names match exactly (case-sensitive)

### Empty global sets
- Ensure functions have definitions (not just declarations)
- Check that globals are actually referenced in the function bodies

### Build errors
- Ensure Frama-C is properly installed
- Run `dune clean && dune build` to rebuild from scratch

## Integration with RTE Project

This plugin is designed to integrate with the RTE project's k-context extraction:

1. RTE extracts k-layer call graph slices
2. RTE generates JSON with function lists
3. This plugin analyzes global dependencies
4. Output is used for context-aware analysis

## Development

### File Structure

```
src/plugins/global-deps/
├── dune-project          # Project metadata
├── dune                  # Build configuration
└── global_deps.ml        # Main implementation
```

### Building

```bash
dune build src/plugins/global-deps
```

### Testing

```bash
# Run with test files
frama-c -global-deps -gd-input test_input.json test_global_deps.c

# Check help
frama-c -global-deps-help
```

## License

This plugin follows Frama-C's licensing terms.

## Contact

For issues or questions, please refer to the Frama-C documentation or the RTE project maintainers.
