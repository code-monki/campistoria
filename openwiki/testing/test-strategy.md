---
type: test-strategy
title: "Test Strategy and Case Inventory"
description: "The Campistoria engine's test plan (scope, strategy per testing level, deterministic-probabilistic validation, entry/exit criteria, defect governance) and its Test Case ID conventions mapping back to Requirement IDs."
tags: [testing, test-plan, test-cases, deterministic-probabilistic-validation]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-d40c09115a85fadc9509599b
    resource: repo://docs/test/engine-test-cases.md
  - id: openwiki-source-eac01972d7f3e44c4cc62eb3
    resource: repo://docs/test/engine-test-plan.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Test Strategy and Case Inventory

`docs/test/engine-test-plan.md` (v1.1, Approved) and `docs/test/engine-test-cases.md` (v1.0, Approved) together define how every requirement in the [SRS](../requirements/srs-overview.md) will be validated, before any implementation exists to run those tests against. Both are cross-referenced by the [RTM](../requirements/traceability.md), whose core table cites specific Test Case IDs per requirement row.

## Scope

In scope: FR-001 through FR-045 and NFR-001 through NFR-007 from the approved SRS; all ten approved [HLA components](../architecture/hla-overview.md) and their Detailed Design baselines; public-contract behavior, cross-component flows, state transitions, failure behavior, recovery, authorization, validation, provenance, and portability; and deterministic-probabilistic containment at DPB-001 specifically (Oracle invocation, validation, human/GM acceptance, fallback, provenance, and prevention of direct state mutation).

Out of scope: multiplayer, networking, hosted identity, cloud services, and remote latency (the minimal local Principal and Campaign Authority Binding *are* in scope); AI model quality or drift (AI is not an engine capability); Package-specific game correctness beyond conformance to declared contracts; automatic Retcon consequence propagation beyond direct-reference identification; and concrete test runner/language/database/CI-CD product selection.

## Testing levels

The plan defines ten testing levels, each with a stated planning posture: Unit (required where a component-owned rule can be isolated), Component (required for all ten HLA components), Integration (required for every multi-component contract and DPB-001), System (required for all FR acceptance criteria), Regression (required, automated where feasible), Performance (required once a representative implementation and workload exist, for NFR-001), Security (capability checks, POV isolation, trust-boundary rejection, hostile input), Reliability/Resilience (interrupted writes, corruption, restore, Recovery, rollback), User Acceptance (confirms diagnostics preserve human authority — project owner performs or delegates), and Packaging/Clean Environment (planned now, executable once packaging exists).

## Deterministic–probabilistic validation

DPB-001 is tested as a **complete boundary**, not isolated functions, through an explicit eight-point sequence: Oracle invocation may vary but cannot mutate Campaign Reality; output must conform to the engine-owned adapter envelope and Package-declared contract; HLA-VALIDATE rejection leaves the target Unresolved with structured diagnostics; validation establishes eligibility only — explicit human/GM acceptance is still required before Resolution use; only an authorized [HLA-RESOLUTION](../architecture/components/resolution.md) Command may append the resulting Event; accepted and rejected Oracle invocations both retain required provenance without rejected invocations becoming HLA-CORE Events; a fixed Campaign seed may test repeatability within one conformant implementation without asserting cross-implementation PRNG equivalence; and drift testing is explicitly not applicable because no model-backed probabilistic component is in scope.

## Non-functional validation strategy

Each NFR has an explicit validation strategy and acceptance posture — for example, NFR-001 (Performance) measures representative local, single-process Transactions after warm-up against a provisional target of at least 95% completing within 500ms, with recalibration requiring SRS/RTM change control rather than an ad hoc test tweak; NFR-007 (Authorization) exercises every Campaign-scoped operation group with an active sufficient binding, no binding, a revoked binding, the wrong Campaign, and insufficient capabilities, and specifically attempts caller-supplied role/capability escalation and imported foreign-binding activation — only an active local binding with the required capability may permit forwarding, and every other attempt must fail before any side effect.

## Test Case ID conventions

Test Case IDs use a stable owner prefix tying each case back to its owning component or cross-cutting concern:

| Prefix | Owner / Concern |
|---|---|
| `TC-LIF` | [HLA-LIFECYCLE](../architecture/components/lifecycle.md) |
| `TC-CORE` | [HLA-CORE](../architecture/components/core.md) |
| `TC-RES` | [HLA-RESOLUTION](../architecture/components/resolution.md) |
| `TC-OBS` | [HLA-OBSERVER](../architecture/components/observer.md) |
| `TC-PKG` | [HLA-PACKAGE](../architecture/components/package.md) |
| `TC-QRY` | [HLA-QUERY](../architecture/components/query.md) |
| `TC-PER` | [HLA-PERSIST](../architecture/components/persist.md) |
| `TC-STA` | [HLA-STATE](../architecture/components/state.md) |
| `TC-VAL` | [HLA-VALIDATE](../architecture/components/validate.md) |
| `TC-CON` | [HLA-CONTRACT](../architecture/components/contract.md) |
| `TC-DPB` | Deterministic-probabilistic boundary DPB-001 |
| `TC-NFR` | Cross-cutting non-functional validation |

Each case in the inventory records a Requirement ID (or IDs — some cases like `TC-CON-006` map to more than one, e.g. FR-032 and FR-045 together), a testing Level, a Validation Objective/Pass Criterion, and a Design Reference back to the relevant Detailed Design. The inventory is explicitly a **planning artifact defining behavioral intent and acceptance posture** — it does not prescribe a test framework, and concrete preconditions/fixtures/steps/evidence are recorded once a case is actually implemented.

Representative cases: `TC-LIF-003` (FR-003, System level) instantiates two Campaigns from one Scenario, mutates one, and proves identities/seeds/Event streams/Campaign Reality remain independent. `TC-RES-003` (FR-014, Integration level) generates valid and invalid Oracle results without applying Resolution and proves Campaign Reality remains unchanged in every case. `TC-CON-005` (FR-045, Integration level) creates a Campaign as a locally recognized Principal and proves an active `campaignOwner` binding is established atomically, remains inspectable, and survives save/reload without relying on Session claims — directly testing the [Campaign Authority Binding decision](../reference/decision-log.md).

## Entry and exit criteria

Testing may begin once the Test Plan and Test Case Inventory are approved, the RTM maps every FR/NFR to Test Case IDs, implementation technology needed by relevant cases is selected through authorized lifecycle work, and test environment/fixture conventions are documented. Testing is only complete when every required case has passed (or has an explicitly approved exception), all FRs/NFRs have recorded evidence, DPB-001 containment tests pass, no unresolved Critical or High defect remains, the full required suite passes in a clean environment, and the project owner grants explicit phase-gate approval.

## Defect governance

Defects are triaged into four severities with distinct gate effects: **Critical** (authoritative-state corruption, containment bypass, unauthorized privileged mutation, cross-observer disclosure, unrecoverable data loss) stops work immediately and requires root-cause analysis plus a lifecycle rollback assessment; **High** (an approved requirement fails, atomicity/provenance is violated, a trust boundary can be bypassed) blocks phase advancement and release; **Medium** (workaroundable, no boundary violation) requires disposition and re-test, with release exceptions needing explicit approval; **Low** (cosmetic/documentation) is tracked but doesn't automatically block. A defect revealing an incorrect or missing requirement, architecture boundary, or design contract triggers a **formal rollback to the earliest affected lifecycle phase** — the same mechanism documented for FR-045/NFR-007 in the [decision log](../reference/decision-log.md) and [Lifecycle Governance Discipline](../overview/governance-lifecycle.md).
