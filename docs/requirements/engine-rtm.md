# Requirements Traceability Matrix (RTM)

Project Name: Campistoria Engine
Version: 1.2 (Test Planned Candidate)
Date (YYYY-MM-DD): 2026-09-17
Maintained By: CodeMonki
Status: Gate Review
Requirement Version Reference: [Engine SRS](engine-srs.md) v1.0 (Approved)
Architecture Version Reference: [Engine HLA](../architecture/engine-hla.md) v1.0 (Approved)
Design Version Reference: [Engine Detailed Design](../design/engine-detailed-design.md) v1.0 (Approved)
Test Plan Version Reference: [Engine Test Plan](../test/engine-test-plan.md) v1.0 (Approved)
Test Case Inventory Reference: [Engine Test Case Inventory](../test/engine-test-cases.md) v0.1 (Candidate)

---

## Table of Contents

- [1. RTM Authority Declaration](#1-rtm-authority-declaration)
- [2. Core Traceability Matrix](#2-core-traceability-matrix)
- [3. Deterministic–Probabilistic Boundary Traceability](#3-deterministicprobabilistic-boundary-traceability)
- [4. Non-Functional Traceability](#4-non-functional-traceability)
- [5. Coverage Validation Checklist](#5-coverage-validation-checklist)
- [6. Change Control and Lineage](#6-change-control-and-lineage)
- [7. Approval](#7-approval)

---

<a id="1-rtm-authority-declaration"></a>
# 1. RTM Authority Declaration <sup>[↩](#table-of-contents "Back to ToC")</sup>

- Requirements approved? **Yes** — [Engine SRS](engine-srs.md) v1.0 approved by the project owner (CodeMonki) on 2026-09-08.
- Architecture approved? **Yes** — [Engine HLA](../architecture/engine-hla.md) v1.0 approved by the project owner (CodeMonki) on 2026-09-12.
- Detailed Design approved? **Yes** — [Engine Detailed Design](../design/engine-detailed-design.md) v1.0 approved by the project owner (CodeMonki) on 2026-09-17.
- Test Plan aligned? **Partially** — Test Plan v1.0 is approved; Test Case Inventory v0.1 and this RTM v1.2 mapping remain pending gate review.
- Advancement to next phase authorized? **No** — Implementation requires approval of the Test Plan, Test Case Inventory, and this RTM v1.2 mapping.

This RTM records the approved Architecture and Detailed Design mappings plus candidate Test Case mappings. Implementation, Packaging, and Orchestration columns remain unpopulated until those later phases produce approved artifacts. Test execution evidence remains pending Implementation and Test execution.

---

<a id="2-core-traceability-matrix"></a>
# 2. Core Traceability Matrix <sup>[↩](#table-of-contents "Back to ToC")</sup>

The HLA Component ID column is populated from the approved [Engine HLA](../architecture/engine-hla.md) v1.0. The DD Artifact column is populated from approved component-level Detailed Design artifacts. Test Case IDs link to the candidate [Engine Test Case Inventory](../test/engine-test-cases.md). Implementation, Packaging, Orchestration, and test-execution evidence remain **N/A — pending approved artifact or execution**.

| Req ID | Req Type | Requirement | HLA Component ID | DD Artifact | Implementation Unit | Test Case ID | Packaging Ref | Orchestration Ref | Validation Status | Evidence Ref |
|---|---|---|---|---|---|---|---|---|---|---|
| FR-001 | FR | Campaign Creation From Scenario | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | [TC-LIF-001](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| FR-002 | FR | Campaign Identity Permanence | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | [TC-LIF-002](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| FR-003 | FR | Independent Campaign Divergence | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | [TC-LIF-003](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| FR-004 | FR | Entity Identity | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | [TC-CORE-001](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-005 | FR | Location as Specialized Entity | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | [TC-CORE-002](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-006 | FR | Property Assignment | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | [TC-CORE-003](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-007 | FR | First-Class Relationships | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | [TC-CORE-004](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-008 | FR | Fact Recording With Provenance | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | [TC-CORE-005](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-009 | FR | Temporally Bounded State | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | [TC-CORE-006](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-010 | FR | Campaign Time Ordering | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | [TC-CORE-007](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-011 | FR | Event and History Recording | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | [TC-CORE-008](../test/engine-test-cases.md#2-functional-test-cases); [TC-CORE-009](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-012 | FR | Unresolved State Declaration | HLA-RESOLUTION | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | [TC-RES-001](../test/engine-test-cases.md#2-functional-test-cases); [TC-DPB-001](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| FR-013 | FR | Resolution of Unresolved State | HLA-RESOLUTION | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | [TC-RES-002](../test/engine-test-cases.md#2-functional-test-cases); [TC-DPB-003](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases); [TC-DPB-005](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| FR-014 | FR | Oracle Result Non-Authority | HLA-RESOLUTION | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | [TC-RES-003](../test/engine-test-cases.md#2-functional-test-cases); [TC-DPB-001](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases); [TC-DPB-002](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases); [TC-DPB-003](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases); [TC-DPB-004](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| FR-015 | FR | Observer Definition | HLA-OBSERVER | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | [TC-OBS-001](../test/engine-test-cases.md#2-functional-test-cases); [TC-OBS-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| FR-016 | FR | Observer Knowledge Independence | HLA-OBSERVER | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | [TC-OBS-002](../test/engine-test-cases.md#2-functional-test-cases); [TC-OBS-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| FR-017 | FR | Observation Updates Observer Knowledge Only | HLA-OBSERVER | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | [TC-OBS-003](../test/engine-test-cases.md#2-functional-test-cases); [TC-OBS-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| FR-018 | FR | Information Source Provenance | HLA-OBSERVER | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | [TC-OBS-004](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| FR-019 | FR | Package Registration | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-PKG-001](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-020 | FR | Effective Campaign Definition Composition | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-PKG-002](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-021 | FR | Explicit Composition, No Implicit Collision Resolution | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-PKG-003](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-022 | FR | Package Version Pinning | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-PKG-004](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-023 | FR | Explicit Package Migration | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-PKG-005](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-024 | FR | Coexisting Package Versions | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-PKG-006](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-025 | FR | Dependency-Aware Package Removal | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-PKG-007](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-026 | FR | System/Edition Identity Independent of Package Version | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-PKG-008](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-027 | FR | Query Interface | HLA-QUERY | [HLA-QUERY DD](../design/components/hla-query-dd.md) | N/A | [TC-QRY-001](../test/engine-test-cases.md#2-functional-test-cases); [TC-CON-003](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| FR-028 | FR | POV Resolution Authority | HLA-QUERY | [HLA-QUERY DD](../design/components/hla-query-dd.md) | N/A | [TC-QRY-002](../test/engine-test-cases.md#2-functional-test-cases); [TC-CON-003](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| FR-029 | FR | Presentation Model Generation | HLA-QUERY | [HLA-QUERY DD](../design/components/hla-query-dd.md) | N/A | [TC-QRY-003](../test/engine-test-cases.md#2-functional-test-cases); [TC-CON-003](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| FR-030 | FR | Multiple Projection Types From Shared State | HLA-QUERY | [HLA-QUERY DD](../design/components/hla-query-dd.md) | N/A | [TC-QRY-004](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| FR-031 | FR | Durable Persistence | HLA-PERSIST | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) | N/A | [TC-PER-001](../test/engine-test-cases.md#2-functional-test-cases); [TC-CORE-009](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| FR-032 | FR | Implementation-Neutral Import/Export | HLA-PERSIST | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) | N/A | [TC-PER-002](../test/engine-test-cases.md#2-functional-test-cases); [TC-PER-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| FR-033 | FR | Checkpoint Creation and Restoration | HLA-STATE | [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | [TC-STA-001](../test/engine-test-cases.md#2-functional-test-cases); [TC-OBS-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-STA-006](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-034 | FR | Undo Operation | HLA-STATE | [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | [TC-STA-002](../test/engine-test-cases.md#2-functional-test-cases); [TC-STA-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-035 | FR | Recovery Operation | HLA-STATE | [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | [TC-STA-003](../test/engine-test-cases.md#2-functional-test-cases); [TC-STA-006](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-036 | FR | Retcon Operation | HLA-STATE | [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | [TC-STA-004](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-037 | FR | Campaign/Package Consistency Validation | HLA-VALIDATE | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) | N/A | [TC-VAL-001](../test/engine-test-cases.md#2-functional-test-cases); [TC-CON-004](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-VAL-003](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-VAL-004](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-DPB-002](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| FR-038 | FR | Structured Diagnostics | HLA-VALIDATE | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) | N/A | [TC-VAL-002](../test/engine-test-cases.md#2-functional-test-cases); [TC-VAL-004](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| FR-039 | FR | Non-Privileged Public Contracts | HLA-CONTRACT | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) | N/A | [TC-CON-001](../test/engine-test-cases.md#2-functional-test-cases); [TC-CON-002](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-CON-004](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-DPB-004](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |
| FR-040 | FR | Scenario Immutability | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | [TC-LIF-005](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| FR-041 | FR | Failed Migration Rollback | HLA-PACKAGE, HLA-STATE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | [TC-PKG-009](../test/engine-test-cases.md#2-functional-test-cases); [TC-STA-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-042 | FR | Campaign-Local Asset Portability | HLA-PERSIST | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) | N/A | [TC-PER-003](../test/engine-test-cases.md#2-functional-test-cases); [TC-PER-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| FR-043 | FR | On-Demand Access to Archived Campaign History | HLA-PERSIST | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) | N/A | [TC-PER-004](../test/engine-test-cases.md#2-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| FR-044 | FR | Campaign Seed Assignment | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | [TC-LIF-004](../test/engine-test-cases.md#2-functional-test-cases); [TC-DPB-006](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| NFR-001 | NFR | Performance | HLA-CONTRACT | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) | N/A | [TC-NFR-PERF-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |
| NFR-002 | NFR | Security | HLA-VALIDATE | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) | N/A | [TC-NFR-SEC-001](../test/engine-test-cases.md#5-non-functional-test-cases); [TC-CON-002](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-CON-004](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-PER-005](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-VAL-003](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-DPB-002](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| NFR-003 | NFR | Reliability | HLA-CORE, HLA-OBSERVER, HLA-STATE | [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | [TC-NFR-REL-001](../test/engine-test-cases.md#5-non-functional-test-cases); [TC-CORE-009](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases); [TC-STA-006](../test/engine-test-cases.md#3-cross-cutting-contract-and-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) |
| NFR-004 | NFR | Scalability | HLA-PERSIST, HLA-CORE, HLA-OBSERVER | [HLA-PERSIST DD](../design/components/hla-persist-dd.md); [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | [TC-NFR-SCALE-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-PERSIST DD](../design/components/hla-persist-dd.md); [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| NFR-005 | NFR | Maintainability | HLA-CONTRACT, HLA-PACKAGE | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | [TC-NFR-MAINT-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| NFR-006 | NFR | Auditability | HLA-CORE, HLA-OBSERVER, HLA-RESOLUTION | [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | [TC-NFR-AUD-001](../test/engine-test-cases.md#5-non-functional-test-cases); [TC-DPB-005](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | N/A | N/A | Test planned; execution pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |

---

<a id="3-deterministicprobabilistic-boundary-traceability"></a>
# 3. Deterministic–Probabilistic Boundary Traceability <sup>[↩](#table-of-contents "Back to ToC")</sup>

| Req ID | Boundary ID | Validation Harness | Containment Logic | Fallback Ref | Observability Ref | Drift Validation | Test Case ID | Status |
|---|---|---|---|---|---|---|---|---|
| FR-014 | DPB-001 (Oracle result vs. Resolution) | HLA-VALIDATE validation harness ([Engine HLA](../architecture/engine-hla.md) §§4, 6; [HLA-VALIDATE DD](../design/components/hla-validate-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md)) | Oracle Adapter + HLA-RESOLUTION Command boundary; no direct Campaign Reality writes | FR-012 (leave Unresolved) | NFR-006 provenance via HLA-CORE/HLA-RESOLUTION event records and Oracle invocation provenance | N/A — no model drift component in scope | [TC-DPB-001 through TC-DPB-006](../test/engine-test-cases.md#4-deterministic-probabilistic-boundary-cases) | Test planned; execution pending |

---

<a id="4-non-functional-traceability"></a>
# 4. Non-Functional Traceability <sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR ID | Architectural Mechanism | Design Artifact | Test Case | Packaging Impact | Status |
|---|---|---|---|---|---|
| NFR-001 | HLA-CONTRACT preserves in-process Facade path; network adapter remains optional | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) | [TC-NFR-PERF-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | Test planned; execution pending — quantitative target provisional |
| NFR-002 | HLA-VALIDATE Chain of Responsibility gates Package/import content before core state | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) | [TC-NFR-SEC-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | Test planned; execution pending |
| NFR-003 | Event Sourcing plus HLA-STATE Memento checkpoint/recovery | [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) | [TC-NFR-REL-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | Test planned; execution pending |
| NFR-004 | Active-window/archival split through HLA-PERSIST with HLA-CORE/HLA-OBSERVER event logs | [HLA-PERSIST DD](../design/components/hla-persist-dd.md); [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | [TC-NFR-SCALE-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | Test planned; execution pending — calibration provisional |
| NFR-005 | Hexagonal ports plus HLA-PACKAGE Microkernel boundary | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | [TC-NFR-MAINT-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | Test planned; execution pending |
| NFR-006 | Append-only event history plus HLA-RESOLUTION Command provenance | [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | [TC-NFR-AUD-001](../test/engine-test-cases.md#5-non-functional-test-cases) | N/A | Test planned; execution pending |

---

<a id="5-coverage-validation-checklist"></a>
# 5. Coverage Validation Checklist <sup>[↩](#table-of-contents "Back to ToC")</sup>

- 100% FR coverage to design? **Yes** — FR-001 through FR-044 have DD Artifact mappings in Section 2.
- 100% NFR coverage to design? **Yes** — NFR-001 through NFR-006 have DD Artifact mappings in Sections 2 and 4.
- 100% implementation traceability? **N/A — Implementation phase not reached.**
- 100% requirement-to-test coverage? **Yes — candidate** — every FR and NFR maps to at least one Test Case ID in Section 2.
- Deterministic–probabilistic boundaries mapped? **Yes — candidate through Test Planning** — DPB-001 maps to HLA-VALIDATE, HLA-RESOLUTION, containment/fallback/observability mechanisms, and TC-DPB-001 through TC-DPB-006.
- Packaging traceability complete? **N/A.**
- Orchestration traceability complete? **N/A.**
- No orphan artifacts? **Yes** — every HLA component traces to Requirement IDs, every FR/NFR row has Architecture mapping or cross-cutting NFR mapping, and every component-level Detailed Design artifact is referenced by the RTM or parent Detailed Design artifact.

---

<a id="6-change-control-and-lineage"></a>
# 6. Change Control and Lineage <sup>[↩](#table-of-contents "Back to ToC")</sup>

| Date | Change Summary | Impacted IDs | Phase Rollback Required |
|---|---|---|---|
| 2026-09-08 | Initial RTM scaffold created alongside [Engine SRS](engine-srs.md) v0.1 | FR-001–FR-041, NFR-001–NFR-006 | No |
| 2026-09-08 | Added FR-042 (Campaign-Local Asset Portability) following requirements review of Section 10 asset-persistence coverage | FR-042 | No |
| 2026-09-08 | Revised NFR-001 (Performance) to scope to local/in-process Transactions only, deferring network/multi-user latency; added glossary term "Transaction"; added risk entry for single-user assumption rework | NFR-001 | No |
| 2026-09-08 | Revised NFR-004 (Scalability) to describe active-window/archival-transition behavior without prescribing a data structure; added FR-043 (On-Demand Access to Archived Campaign History) to close functional gap exposed by NFR-004 | NFR-004, FR-043 | No |
| 2026-09-08 | Added FR-044 (Campaign Seed Assignment) and resolved Section 6 "Reproducibility expectations" open item, scoped to software-implemented Oracles within a single conformant implementation; cross-implementation PRNG-sharing left open for future SRD | FR-044 | No |
| 2026-09-08 | [Engine SRS](engine-srs.md) approved by project owner (CodeMonki) as v1.0; Requirements→Architecture gate cleared | FR-001–FR-044, NFR-001–NFR-006 | No |
| 2026-09-12 | [Engine HLA](../architecture/engine-hla.md) approved by project owner (CodeMonki) as v1.0; HLA Component ID mappings populated; Architecture→Detailed Design gate cleared | FR-001–FR-044, NFR-001–NFR-006 | No |
| 2026-09-16 | Detailed Design component artifacts mapped into RTM v1.1 candidate; Test Case IDs remain pending Test Planning | FR-001–FR-044, NFR-001–NFR-006 | No |
| 2026-09-17 | Detailed Design baseline and RTM v1.1 mapping approved; Detailed Design and Traceability Consolidation gates cleared; Test Planning authorized | FR-001–FR-044, NFR-001–NFR-006 | No |
| 2026-09-17 | Test Plan and Test Case Inventory v0.1 candidates created; linked Test Case IDs and requirement short descriptions added for RTM v1.2 gate review | FR-001–FR-044, NFR-001–NFR-006, DPB-001 | No |

---

<a id="7-approval"></a>
# 7. Approval <sup>[↩](#table-of-contents "Back to ToC")</sup>

Approved By:
Role:
Date:
Version Incremented: Pending — v1.2 candidate

RTM validation remains required before Implementation completion, Test phase closure, Packaging approval, and Release authorization. This v1.2 candidate contains complete Test Planning mappings but does not authorize Implementation until approved with the Test Plan and Test Case Inventory.

---

End of Requirements Traceability Matrix (Test Planned Candidate v1.2)
