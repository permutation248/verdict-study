# Data → Tables/Figures Pipeline

**Core principle (non-negotiable):** every number in the paper's evaluation is
*extracted from the raw experiment artifacts by these scripts*. Nothing is typed
by hand; the **same extractor and the same computation** feed every table and
every figure, so they can never disagree. Regeneration is deterministic — same
raw data + same scripts ⇒ byte-identical output.

```
   raw artifacts  ──load_raw.py──▶  one in-memory dataset  ──gen_*.py──▶  generated/*.tex,*.csv,*.pdf
   (single source of truth)         (one extractor, one             (every table/figure
                                      set of metric definitions)      reads the same numbers)
```

## The single source of truth (read-only)

`<repo>/scripts/experiments/results/` — per experiment cell:
| artifact | provides |
|----------|----------|
| `judge_final.json` | rte / spec discharge (valid, total, ratio) |
| `run.json` | time, bugs, convergence, #functions, call graph |
| `events.jsonl` | proof demands raised/satisfied, cycles |
| `provenance.txt` | baseline (Preguss/SpecSyn) wall-clock |
| `benchmarks/.../*.c` | lines of code |

**`load_raw.py` is the ONLY reader of these.** Every `gen_*.py` imports it — so
the extraction logic, the metric definitions, and the bench groupings/naming
exist in exactly one place.

## Files

| file | role |
|------|------|
| `load_raw.py` | unified extractor: cells, `bench_meta`, `display_name`/`NAME_MAP`, RQ1/RQ3 groupings, rq2/rq3 series. Repo paths auto-resolved (`_repo_root`), so it runs from anywhere. |
| `paper_style.py` | shared colors + formatting helpers (`meanstd`, `best`, `meanstd(bold=)`, …) + `render()` (compiles a preview PDF and removes all LaTeX build artifacts). |
| `gen_rq1.py`, `gen_rq2.py`, `gen_rq2c.py`, `gen_rq3.py` | one per artifact; each consumes **`load_raw` only** and writes to `generated/`. |
| `gen_all.py` | runs every builder in one process under a single commit/date stamp. |

## Regenerate

From the repo root:

```bash
python3 scripts/figures/gen_all.py        # regenerate everything into generated/
```

Or one artifact at a time (the output directory is a required argument):

```bash
python3 scripts/figures/gen_rq1.py scripts/figures/generated \
    --commit "$(git rev-parse --short HEAD)"
# likewise gen_rq2.py / gen_rq2c.py / gen_rq3.py
```

Each writes `generated/<name>.tex` (the `\input`-able fragment) + `<name>.csv`
(the data behind it) + `<name>.pdf` (a preview). Build leftovers (aux/log/…) are
auto-removed. The whole `generated/` directory is `.gitignore`'d — regenerate it
locally; it is never committed.

## Why this is consistent

1. **One extractor** — a table and the figure of the same data read the same
   in-memory numbers.
2. **One set of metric definitions** (rte / spec / time / convergence /
   weighted-mean / geometric-mean) lives in code, applied identically to RQ1,
   RQ2, and RQ3.
3. **Deterministic** — re-running on the same `results/` reproduces identical
   `.tex`.
