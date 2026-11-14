# SpecSyn baseline

We run the SpecSyn pipeline (arXiv 2604.21570, public artifact) on our
benchmarks and score the output with the **unified judge**, identically to
Verdict and Preguss.

## Files

- `run_specsyn.sh` — runner: drives the SpecSyn artifact on one benchmark.
  See the script header for usage; it reads `ANTHROPIC_API_KEY` from `.env`.
- `prepare_inputs.py` — prepares our benchmark `.c` files (carrying the injected
  `rte_N` safety assertions) as SpecSyn inputs; the prepared files are in `inputs/`.
- `api_anthropic.py` — drop-in Anthropic client for the SpecSyn pipeline.
- `score_rte.py` — counts `Assertion 'rte_*'` proved Valid (SpecSyn itself ships
  only an SV-COMP `target_N` scorer). Superseded by the unified judge below;
  kept for reference.

## Reading the scores

Per-cell unified-judge scores (same judge as Verdict, `lib/score_final.py`):

    results/baseline-specsyn-scores/<bench>/<model>-<rep>/judge_final.json

with `rte` and `spec` as `valid` / `total` / `ratio`. Wall-clock time, and the
exact annotated `.c` the judge scored (re-run `lib/score_final.py` on it to
reproduce the score):

    results/baseline-specsyn/<bench>/<model-long>/<rep>/provenance.txt          (wall_min=)
    results/baseline-specsyn/<bench>/<model-long>/<rep>/restored.annotated.c    (SpecSyn output)

## Re-running

SpecSyn itself is **not** redistributed here. Obtain its public artifact
(arXiv 2604.21570), then run `run_specsyn.sh` on a prepared input.
