# RQ3 — Coordination-mechanism ablation (demands × assume)

**RQ3: How much of Verdict's convergence and speed comes from its two
inter-agent coordination channels — the proof-demand protocol and
optimistic callee assumption — and how do they interact?**

The two channels are complementary by design:

- **Demands** (`submit_proof_demand` / `resolve_proof_demand`): a caller
  *requests* a missing guarantee (postcondition / assigns) or a
  precondition weakening from a callee. Contracts become two-way
  traffic; missing interface lemmas get created on demand.
- **Assume** (`assume_callee` + the unpublished-callee hypothesis path):
  a caller makes *optimistic progress* under its own hypothesis about a
  callee, with the runtime anchoring the debt (`based_on = 0`) until it
  is discharged against the real published spec.

Removing either leaves a functioning verifier; what degrades — and on
which benchmarks — is the measurement.

## Arms (2×2; the fourth cell is the baseline)

| arm | demands | assume | solve flag |
|---|---|---|---|
| `baseline` | on | on | (none) |
| `no-demands` | **off** | on | `--ablate demands` |
| `no-assume` | on | **off** | `--ablate assume` |
| `no-demands-no-assume` | **off** | **off** | `--ablate demands,assume` |

The baseline cell is run FRESH with identical config (not recycled from
RQ1/RQ2) so all four cells share provenance.

## Hypotheses (falsifiable signatures)

- **H1 (−demands)**: convergence drops on chain-deep benches — callers
  blocked on never-published interface lemmas can only assume forever
  (settle at LOCAL_OK) or fail. The shallow benches are largely unaffected.
  Mechanism check: demand count ≡ 0; settled-assume counts *increase*
  (assume becomes the only escape hatch).
- **H2 (−assume)**: convergence roughly preserved but wall-clock and
  cycle counts rise — deep chains serialize bottom-up (callers idle
  until callees publish). Mechanism check: assume/settle counts ≡ 0;
  demand counts *increase* (demands become the only channel).
- **H3 (−both)**: only passive publish/wake remains ("publish and
  pray"); deep-chain convergence craters; the shallow benches still
  converge.
- The cross-compensation in H1/H2 (each arm's surviving channel usage
  going UP) is itself evidence the channels are complementary, not
  redundant.

Dose-response framing: effect size should correlate with call-chain
depth / measured demand richness, not with benchmark size. Reference
points from production runs: submodule-1/2 raise 4–31 demands per run
(the sum-bound lemma independently demanded by 3–7 callers); x509-full
raises 149–157 (58–74 weakenings); flat math files raise ~0.

## Benchmarks

| bench | role | rationale |
|---|---|---|
| `submodule-1` | chain-deep | depth-8 chains, demand-rich, cheap (20 fns) |
| `submodule-2` | chain-deep | 52 fns, depth-10, the demand-storm bench |
| `jc-voronoi` | medium depth | 21 fns, depth-4, mixed leaf/chain |
| `hypatia` | medium depth | 55 fns, float-heavy geometry library |
| `sokol-shape` | medium depth | 33 fns, struct-heavy shape generation |
| `contiki-aes-ccm` | embedded + wall | 10 fns, lsl shift wall, Contiki source |

## Fixed knobs

Same fixed point as RQ1/RQ2: model `claude-sonnet-4-6`, max-parallel 8,
wp-jobs 8, wp-procs 8, leaf-first, scope global, max-turns 30,
max-cycles 24, sound WP model (no `--wp-model`), 3h timeout. REPS=3.

## Metrics

- Primary: global_ok rate, RTE/spec discharge, wall-clock, total
  cycles.
- Mechanism (sanity + compensation): demand raised/resolved counts,
  assume/settle counts, conditional-OK counts, FAILED-tail composition,
  spec version counts (reformulation churn).

## CLI design — clean decoupling (`--ablate`)

`verdict solve --ablate demands[,assume]` — a comma-separated set;
values validated against {demands, assume}.

Carrier: a frozen `AblationConfig` dataclass (`verdict/core/ablation.py`),
threaded explicitly CLI → `_setup` → `Orchestrator` → agent runner +
tool factory + `ToolContext`. No module-global state (unlike
`wp_resources`, which is process-wide CPU policy, this is semantic
config — explicit threading keeps tests isolated).

**`--ablate demands` — 2 touchpoints, everything else inert by data flow:**

1. Tool factory: omit `submit_proof_demand` / `resolve_proof_demand`
   from the solve profile — agents physically cannot call them.
2. System-prompt text: gate the demand-protocol paragraph.

No other branches needed: with zero demands ever raised,
`open_proof_demands()` is empty forever, so the runner's demand prompt
sections self-vanish (data-driven), the orchestrator's demand-wake and
demand-rescue paths never fire, and the registry demand store stays
empty. The ablation is the *absence of the tools*, not a behavioral
fork — that is the cleanness argument.

**`--ablate assume` — 3 touchpoints, downstream inert by data flow:**

1. Tool schema: drop the `assume_callee` parameter from
   `verify_function_contract` (agents never see it); entry validation
   rejects it if passed anyway, naming the ablation.
2. Resolver (`_resolve_callee_contracts_for_wp`): gains
   `allow_hypotheses`; when False, cc entries for unpublished AGENT
   callees are dropped (weak environment) instead of `hypothesized`,
   with explicit feedback. **Extern callees are exempt**: environment
   axioms are not peer optimism (no publisher exists); ablating them
   would add an unrelated handicap on extern-bearing benches.
3. Wake-guidance text: the "verify under your hypothesis" string gets a
   "wait for the callee's publish" variant.

Downstream: `assumed_callees` stays empty (minus externs), so publish
anchors, debt wakes, settles and CONDITIONAL_OK never occur — again no
behavioral forks.

**Interaction arm**: both switches compose with no additional code.

**Provenance**: arm is encoded in the results path
(`results/rq3/<bench>/<arm>/rep<N>`), the `--ablate` flag is visible in
solve.log, and `run.json` records the ablation set.

## Deliberately out of scope

- `−callsite-gating` (running with baseline-style unvalidated requires)
  is a *soundness-inflation accounting* experiment, not a capability
  ablation — backlogged separately.
- Soundness mechanisms (draft single proof context, anchoring, publish
  gate) are not ablatable: removing them yields unsound successes whose
  numbers are uninterpretable.

## Status

- [x] `--ablate` switch implemented (384-test suite + golden pins +
      end-to-end checks: zero mechanism mentions in a double-ablated run)
- [x] external adversarial audit passed after fixes (see below)
- [ ] `rq3/run_one.sh` smoke on submodule-1 `no-demands`
- [ ] grid run

## External audit

An adversarial review was run against the four claims. Verdicts after
triage and fixes:

- **Default-path behavior: holds.** Prompt bytes, schemas (object
  identity), tool lists verified identical vs pre-change. One deliberate
  delta stands: `run.json` config now always records `ablation: []` and
  `wp_model: null` — additive provenance keys (explicit "no ablation"
  beats ambiguous absence); all readers use tolerant `.get`.
- **Fixed from the audit**: (1) ablation notices are now PREPENDED to
  EVERY tool description that references a removed mechanism
  (read_function_spec, mark_vo_as_bug, ... — not just
  verify_function_contract); (2) mark_vo_as_bug's runtime refusal and
  the orchestrator's assume-debt wake text no longer point to
  submit_proof_demand under `--ablate demands`; (3) contract failure
  feedback's remedy string is now correct in all four arm combinations;
  (4) **`--resume-from` is refused when any ablation is active** —
  seeded specs carry optimistic state (assumed_callees, hypothesis cc,
  restored proof states) that the initial batch skips re-verifying,
  which would bypass the ablated semantics entirely.
- Golden coverage extended: solve profile, debug profile, and
  empty-neighbors rendering are all pinned.
