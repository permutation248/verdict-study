# Preguss baseline

We run Preguss (arXiv 2512.24594, OOPSLA 2026; public artifact on Zenodo record
18768810) on our benchmarks and score the output with the **unified judge**,
identically to Verdict and SpecSyn.

## Files

- `run_preguss.sh` — runner: drives the Preguss docker image on one benchmark.
  See the script header for usage; it reads `ANTHROPIC_API_KEY` from `.env`.
- `prepare_inputs.py` — prepares our benchmark `.c` files as Preguss inputs.
- `docker/framac-wpmodel-wrapper.sh` — injects the matched WP memory model into
  the container's frama-c, so all three tools verify under identical semantics.

## Reading the scores

Per-cell unified-judge scores (same judge as Verdict, `lib/score_final.py`):

    results/baseline-preguss-scores/<bench>/<model>-<rep>/judge_final.json

with `rte` and `spec` as `valid` / `total` / `ratio`. Wall-clock time, and the
exact annotated `.c` the judge scored (re-run `lib/score_final.py` on it to
reproduce the score):

    results/baseline-preguss/<bench>/<model-long>/<rep>/provenance.txt   (wall_min=)
    results/baseline-preguss/<bench>/<model-long>/<rep>/annotated.c      (Preguss output)

## Re-running

Preguss is **not** redistributed here. Obtain its public artifact (Zenodo
18768810) and docker image, then run `run_preguss.sh` on a prepared input.
