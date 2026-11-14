# RQ1 — baseline comparison

**RQ1: On the final benchmark set, how do Verdict's proof rates compare
with Preguss and SpecSyn under matched semantics (same model, same
no-init-const discipline, one unified judge)?**

- `config.sh`            — model, LPT queues, hosts, matched-semantics policy
- `run_pool_specsyn.sh`  — serial pool over baseline-specsyn/run_specsyn.sh
- `run_pool_preguss.sh`  — serial pool over baseline-preguss/run_preguss.sh (amd64 image)
- `run_verdict.sh`       — serial 12h Verdict cells

Scoring: every (tool, bench) artifact goes through
`lib/score_final.py` (Typed+var+int+real, wp-par 8, timeout 30,
alt-ergo/z3/cvc5, main included, extern-contract ledger). All runs are
serial — one bench/cell at a time (POOL_SLOTS = VERDICT_CONCURRENCY = 1) —
so every wall-clock number is trustworthy and the grid runs reliably on any
machine. Each cell still uses its own intra-run parallelism (agents, provers);
only the number of cells in flight is serialized.

Repetitions: every (tool, bench, model) cell is **3 repetitions**, except
**Preguss on x509 `full-module`**, which is **2** — its runs are multi-hour and
the scores are stable across runs, so a third was not warranted. Everything else
is 3 (both baselines and Verdict, all other benches, including Verdict and
SpecSyn on full-module). Aggregates use mean ± std over the available repetitions.

Launch order: for a fully-serial run on a single host, launch the three scripts
one after another (each is internally serial). Prerequisites: SpecSyn inputs
prepared; the run host has the amd64 image, the SpecSyn artifact, and the venv.
