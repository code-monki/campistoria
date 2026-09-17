# Campistoria Engine Test Plan

Project Name: Campistoria Engine
Version: 1.0 (Test Plan Baseline)
Date (YYYY-MM-DD): 2026-09-17
Author(s): CodeMonki
Status: Approved
Requirement Version Reference: [Engine SRS](../requirements/engine-srs.md) v1.0 (Approved)
Architecture Version Reference: [Engine HLA](../architecture/engine-hla.md) v1.0 (Approved)
Detailed Design Version Reference: [Engine Detailed Design](../design/engine-detailed-design.md) v1.0 (Approved)
RTM Version Reference: [Engine RTM](../requirements/engine-rtm.md) v1.2 (Test Planned Candidate)
Test Case Inventory Reference: [Engine Test Case Inventory](engine-test-cases.md) v0.1 (Candidate)

---

## Table of Contents

- [1. Test Authority Declaration](#1-test-authority-declaration)
- [2. Test Scope](#2-test-scope)
- [3. Test Objectives](#3-test-objectives)
- [4. Test Strategy](#4-test-strategy)
- [5. Traceability Enforcement](#5-traceability-enforcement)
- [6. Test Environment](#6-test-environment)
- [7. Test Data Governance](#7-test-data-governance)
- [8. Entry Criteria](#8-entry-criteria)
- [9. Exit Criteria](#9-exit-criteria)
- [10. Defect Governance](#10-defect-governance)
- [11. Automation and Orchestration Integration](#11-automation-and-orchestration-integration)
- [12. Metrics and Reporting](#12-metrics-and-reporting)
- [13. Risk Assessment](#13-risk-assessment)
- [14. Phase Gate Declaration](#14-phase-gate-declaration)
- [Approval](#approval)

---

<a id="1-test-authority-declaration"></a>
# 1. Test Authority Declaration

- Requirements approved? **Yes** — Engine SRS v1.0.
- Architecture approved? **Yes** — Engine HLA v1.0.
- Detailed Design approved? **Yes** — Engine Detailed Design v1.0.
- RTM initialized and design-mapped? **Yes** — Engine RTM v1.1.
- Advancement to Test Planning authorized? **Yes** — approved on 2026-09-17.
- Implementation authorized? **No** — authorization still requires approval of the Test Case Inventory and the test mappings in RTM v1.2.

This plan defines validation intent before implementation. It does not select an implementation language, test framework, storage technology, deployment platform, or CI provider.

---

<a id="2-test-scope"></a>
# 2. Test Scope

**In scope:**

- FR-001 through FR-044 and NFR-001 through NFR-006 from the approved SRS.
- All ten approved HLA components and their v1.0 Detailed Design baselines.
- Public-contract behavior, component contracts, cross-component flows, state transitions, failure behavior, recovery, authorization, validation, provenance, and portability.
- Deterministic-probabilistic containment at DPB-001, including Oracle invocation, validation, human/GM acceptance, fallback, provenance, and prevention of direct state mutation.
- Clean-environment execution and packaging-equivalent validation once implementation and packaging artifacts exist.

**Out of scope:**

- Multiplayer, networking, hosted identity, concurrent-user behavior, cloud services, and remote latency.
- AI behavior, model quality, prompt evaluation, or model drift; AI is not an engine capability.
- Package-specific game correctness beyond conformance to declared Package contracts.
- Automatic Retcon consequence propagation beyond direct-reference identification.
- Concrete test runner, language, database, archive format, deployment system, and CI/CD product selection.
- Release execution, which remains gated by implementation, packaging, orchestration, and test evidence.

---

<a id="3-test-objectives"></a>
# 3. Test Objectives

Testing SHALL prove that:

- authoritative Campaign Reality remains deterministic, reconstructible, inspectable, and protected from unauthorized or probabilistic direct writes;
- Observer Knowledge remains separate by Observer and cannot leak across POV boundaries;
- Package composition, migration, persistence, archival retrieval, checkpoints, Undo, Recovery, and Retcon preserve atomicity and provenance;
- first-party and third-party callers use the same public contract and capability enforcement;
- malformed or hostile external content fails closed before state admission;
- every approved FR and NFR has explicit validation criteria and a stable Test Case ID;
- failures produce diagnosable results without silent corruption or partial authoritative state;
- implementation evidence can later be recorded without redefining the intended behavior.

Release readiness requires completed implementation, passing required tests, no unresolved Critical or High defects, complete RTM evidence, packaging validation, clean-environment execution, and explicit human approval.

---

<a id="4-test-strategy"></a>
# 4. Test Strategy

## 4.1 Testing Levels

| Level | Purpose | Planning Posture |
|---|---|---|
| Unit | Validate deterministic folds, validators, policies, projections, serializers, and invariant checks in isolation. | Required where a component-owned rule can be isolated. |
| Component | Validate each HLA component through its approved ports with controlled collaborators. | Required for all ten components. |
| Integration | Validate approved cross-component paths, atomicity, rejection, and failure propagation. | Required for every multi-component contract and DPB-001. |
| System | Validate complete public-contract workflows against representative Campaigns and Packages. | Required for all FR acceptance criteria. |
| Regression | Preserve approved behavior after requirement, design, or implementation change. | Required; automated where feasible. |
| Performance | Measure local, single-process Transaction latency for NFR-001. | Required after a representative implementation and workload exist. |
| Security | Validate capability checks, POV isolation, trust-boundary rejection, hostile input, and non-bypassability. | Required. |
| Reliability / Resilience | Exercise interrupted writes, corruption, dependency outages, restore, Recovery, and rollback. | Required. |
| User Acceptance | Confirm Campaign behavior and diagnostics are understandable and preserve human authority. | Required for gate closure; project owner performs or delegates. |
| Packaging / Clean Environment | Execute the approved suite from packaged artifacts in a clean environment. | Planned now; executable after packaging exists. |

## 4.2 Deterministic-Probabilistic Validation

DPB-001 SHALL be tested as a complete boundary, not merely as isolated functions:

1. Oracle invocation may produce variable output but cannot mutate Campaign Reality.
2. Oracle output must conform to the engine-owned adapter envelope and Package-declared contract.
3. HLA-VALIDATE rejection leaves the target Unresolved and produces structured diagnostics.
4. Validation establishes eligibility only; explicit human/GM acceptance is required before Resolution use.
5. Only an authorized HLA-RESOLUTION Command may append the resulting authoritative Event.
6. Accepted and rejected Oracle invocations retain the required provenance without making rejected invocations HLA-CORE Events.
7. A fixed Campaign seed may be used to test repeatability within one conformant implementation; cross-implementation PRNG equivalence is not asserted.
8. Drift testing is not applicable because no model-backed probabilistic component is in scope.

## 4.3 Functional Validation

Each FR maps to one or more cases in the Test Case Inventory. Cases specify nominal behavior and incorporate boundary, negative, failure, and state-transition checks where those are part of the requirement or Detailed Design contract. Shared failure paths receive dedicated cross-cutting cases rather than being duplicated implicitly.

## 4.4 Non-Functional Validation

| NFR | Validation Strategy | Acceptance Posture |
|---|---|---|
| NFR-001 Performance | Measure representative local, single-process public Transactions after warm-up using a documented workload and percentile calculation. Network time is excluded. | Provisional target: at least 95% complete within 500 ms. Report distribution, sample size, environment, and outliers; recalibration requires SRS/RTM change control. |
| NFR-002 Security | Submit malformed, non-conformant, hostile, and unauthorized inputs at every approved trust boundary; attempt public and internal bypass paths. | No prohibited input affects authoritative state; rejection is structured and observable. |
| NFR-003 Reliability | Inject interrupted writes, corrupt snapshots, unavailable archives, and restore failures. Compare recovered state and provenance with the last valid boundary. | Full recovery to last valid state or a specific diagnosable failure; no silent corruption. |
| NFR-004 Scalability | Exercise ordinary queries inside a Package-defined approximately 30-day active window and explicit queries outside it while observing archive access. | Ordinary active-window queries do not require archived history; older data remains retrievable on demand. Exact event/entity thresholds remain calibration data, not invented requirements. |
| NFR-005 Maintainability | Perform controlled design-level change-impact tests using a representative new Package semantic role and projection/adapter substitutions. | The change does not require redefining Campaign Reality/Observer Knowledge separation or unrelated public contracts. |
| NFR-006 Auditability | Apply each consequential change mechanism and retrieve its provenance. | Records identify what changed, applicable Campaign and operation time, source mechanism, and relevant actor/source references. |

---

<a id="5-traceability-enforcement"></a>
# 5. Traceability Enforcement

- Every approved Requirement ID SHALL map to at least one Test Case ID in the RTM.
- Every Test Case ID SHALL map back to at least one Requirement ID and one Detailed Design artifact.
- Implementation references and execution evidence remain `N/A` until those artifacts exist.
- Test evidence SHALL identify the Test Case ID, implementation version, environment, result, timestamp, and defect reference when failed or blocked.
- Requirement, Architecture, Design, or Test changes SHALL trigger immediate impact review of the RTM and affected cases.
- Orphaned requirements, designs, implementations, or tests block phase advancement.

---

<a id="6-test-environment"></a>
# 6. Test Environment

Concrete environment values remain pending technology selection. Before execution, the implementation phase SHALL record:

- hardware architecture and relevant resource limits;
- operating system and version;
- runtime, compiler, dependency, and test-runner versions;
- storage adapter and configuration;
- deterministic locale, timezone, and clock controls where relevant;
- seed and pseudorandom implementation identity for reproducibility tests;
- active-window and checkpoint configuration;
- clean local environment procedure;
- CI environment, when introduced;
- packaged-runtime environment, when introduced.

Tests SHALL control Campaign Time independently of wall-clock time. Tests involving wall-clock checkpoint cadence SHALL use a controllable clock rather than real waiting. External services are not required in the current scope.

---

<a id="7-test-data-governance"></a>
# 7. Test Data Governance

- Test Packages, Scenarios, Campaigns, assets, imports, Oracle results, and corruption fixtures SHALL be versioned test assets.
- Minimal fixtures SHALL isolate one behavior; representative fixtures SHALL exercise cross-component workflows.
- Fixtures SHALL cover empty, minimum, typical, boundary, invalid, conflicting, stale, archived, corrupt, and hostile cases.
- Deterministic cases SHALL use stable identities, Campaign Time values, and seeds where relevant.
- Probabilistic tests SHALL assert envelope, containment, provenance, and acceptance behavior rather than a particular random narrative value unless testing same-implementation seeded repeatability.
- No production, private, regulated, or user campaign data is required.
- Failed-test artifacts SHALL be retained long enough to reproduce the failure; permanent retention policy is deferred to implementation and CI planning.

---

<a id="8-entry-criteria"></a>
# 8. Entry Criteria

Test implementation and execution may begin when:

- this Test Plan and Test Case Inventory are approved;
- RTM v1.2 maps every FR/NFR to Test Case IDs;
- implementation technology and executable schemas needed by the relevant cases are selected through authorized lifecycle work;
- test environment and fixture conventions are documented;
- the target implementation unit is traceable to its Requirement, HLA component, Detailed Design artifact, and Test Case IDs.

---

<a id="9-exit-criteria"></a>
# 9. Exit Criteria

Test execution is complete only when:

- every required case has Passed or has an explicitly approved exception;
- all FRs and NFRs have recorded evidence;
- DPB-001 containment tests pass;
- no unresolved Critical or High defect remains;
- recovery, rollback, trust-boundary, authorization, POV-isolation, and provenance cases pass;
- the full required suite passes in a clean environment;
- packaging-equivalent validation passes once packaging exists;
- the RTM contains implementation and evidence references;
- affected system documentation is current;
- the project owner grants explicit phase-gate approval.

---

<a id="10-defect-governance"></a>
# 10. Defect Governance

| Severity | Definition | Gate Effect |
|---|---|---|
| Critical | Authoritative-state corruption, containment bypass, unauthorized privileged mutation, cross-observer disclosure, unrecoverable data loss, or unusable core workflow. | Immediate stop; root-cause analysis and lifecycle rollback assessment required. |
| High | Approved requirement fails, atomicity or provenance is violated, recovery is unreliable, or a trust boundary can be bypassed without demonstrated state corruption. | Blocks phase advancement and release. |
| Medium | Material behavior or diagnostic defect with a viable controlled workaround and no boundary violation. | Requires disposition and re-test; release exception requires explicit approval. |
| Low | Cosmetic, documentation, or low-impact usability issue that does not violate an approved requirement. | Track and prioritize; does not automatically block. |

Every fix SHALL identify affected Requirement and Test Case IDs, receive focused re-test, and run the applicable regression set. A defect revealing an incorrect or missing requirement, architecture boundary, or design contract SHALL trigger formal rollback to the earliest affected lifecycle phase.

---

<a id="11-automation-and-orchestration-integration"></a>
# 11. Automation and Orchestration Integration

- Deterministic unit, component, integration, system, security, and regression cases SHALL be automated where feasible.
- The eventual suite SHALL have one documented command for the default required run and separately invocable focused groups.
- CI SHALL gate on required deterministic tests once CI exists.
- Performance tests SHALL run in a controlled environment and SHALL NOT use noisy shared-runner results as sole approval evidence.
- Fault injection and corruption fixtures SHALL be reproducible.
- Manual UAT and approval steps SHALL remain explicit and shall not be silently converted into automated approval.
- Packaging and orchestration checks remain planned but cannot execute before their artifacts exist.

---

<a id="12-metrics-and-reporting"></a>
# 12. Metrics and Reporting

Required planning and execution metrics are:

- requirement-to-test mapping coverage: 100%;
- test-to-requirement reverse mapping coverage: 100%;
- required-case execution coverage: 100% at test closure;
- FR and NFR pass coverage reported separately;
- defect counts by severity and lifecycle origin;
- DPB-001 case pass/fail status reported explicitly;
- NFR measurements reported with environment and sample definition;
- flaky or nondeterministic deterministic-test rate: zero accepted without documented defect and disposition.

Code-coverage percentages are implementation diagnostics, not substitutes for requirement coverage, and no arbitrary code-coverage threshold is imposed by this plan.

---

<a id="13-risk-assessment"></a>
# 13. Risk Assessment

| Risk | Priority | Required Validation Emphasis |
|---|---|---|
| HLA-VALIDATE bypass at a state-admitting boundary | High | Negative path and bypass tests across Package, import, migration, and Oracle flows. |
| Package code or semantics gain direct engine-core access | High | Microkernel boundary and declarative-validator isolation tests. |
| HLA-CONTRACT develops a first-party or privileged shortcut | High | Equivalent-caller contract tests and internal-port access rejection. |
| Capability checks are omitted from mutating operations | High | Denial-with-no-side-effect tests for every privileged operation group. |
| Observer Knowledge or Campaign Reality leaks across POV/observer boundaries | High | Cross-observer, hidden-field, tooling-view, and archived-query isolation tests. |
| Recovery, Undo, Retcon, or migration violates append-only/provenance semantics | High | Fault injection, atomicity, history, and provenance tests. |
| Oracle output bypasses validation or human acceptance | High | End-to-end DPB-001 containment tests. |
| Active-window assumptions are not representative | Moderate | Instrumented NFR-004 workload and calibration reporting. |
| Provisional 500 ms target is unrealistic or too weak | Moderate | Repeatable benchmark definition and change-controlled calibration. |
| Future multiplayer requirements invalidate single-user assumptions | Deferred | Preserve actor/session/revision dimensions in fixtures; do not test deferred multiplayer behavior now. |

---

<a id="14-phase-gate-declaration"></a>
# 14. Phase Gate Declaration

- Test Strategy defined? **Yes — approved.**
- Test Plan defined? **Yes — approved.**
- Test Case Inventory defined? **Yes — candidate.**
- Requirement-to-Test mapping complete? **Yes — candidate RTM v1.2.**
- NFR validation strategy defined? **Yes — approved.**
- Failure scenario coverage defined? **Yes — approved.**
- Deterministic-probabilistic containment validation defined? **Yes — approved.**
- Human approval granted? **Yes for this Test Plan** — Test Case Inventory and RTM v1.2 approval remain pending.
- Implementation authorized? **No.**

This Test Plan is approved. Approval of the Test Case Inventory and RTM v1.2 remains necessary to clear the Test Planning to Implementation gate. That later gate approval would authorize controlled implementation, not Packaging or Release.

---

<a id="approval"></a>
# Approval

Approved By: CodeMonki
Role: Project Owner
Date: 2026-09-17
Version Incremented: Yes — v1.0

---

End of Test Plan Baseline.
