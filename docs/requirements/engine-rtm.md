# Requirements Traceability Matrix (RTM)

Project Name: Campistoria Engine
Version: 1.1 (Detailed Design Mapped)
Date (YYYY-MM-DD): 2026-09-17
Maintained By: CodeMonki
Status: Approved
Requirement Version Reference: [Engine SRS](engine-srs.md) v1.0 (Approved)
Architecture Version Reference: [Engine HLA](../architecture/engine-hla.md) v1.0 (Approved)
Design Version Reference: [Engine Detailed Design](../design/engine-detailed-design.md) v1.0 (Approved)
Test Plan Version Reference: Not yet created

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
- Test Plan aligned? **No.**
- Advancement to next phase authorized? **Yes** — Detailed Design and Traceability Consolidation are approved; Test Planning is authorized. Implementation remains unauthorized.

This RTM records the approved Architecture-phase requirement-to-component mapping and approved Detailed Design artifact mapping. Implementation, Test, Packaging, and Orchestration columns remain unpopulated until those later phases produce approved artifacts.

---

<a id="2-core-traceability-matrix"></a>
# 2. Core Traceability Matrix <sup>[↩](#table-of-contents "Back to ToC")</sup>

The HLA Component ID column is populated from the approved [Engine HLA](../architecture/engine-hla.md) v1.0. The DD Artifact column is populated from approved component-level Detailed Design artifacts and the parent Detailed Design baseline candidate. Implementation, Test, Packaging, and Orchestration columns remain **N/A — pending approved artifact** and will be populated as those phases produce approved artifacts.

| Req ID | Req Type | HLA Component ID | DD Artifact | Implementation Unit | Test Case ID | Packaging Ref | Orchestration Ref | Validation Status | Evidence Ref |
|---|---|---|---|---|---|---|---|---|---|
| FR-001 | FR | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| FR-002 | FR | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| FR-003 | FR | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| FR-004 | FR | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-005 | FR | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-006 | FR | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-007 | FR | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-008 | FR | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-009 | FR | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-010 | FR | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-011 | FR | HLA-CORE | [HLA-CORE DD](../design/components/hla-core-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CORE DD](../design/components/hla-core-dd.md) |
| FR-012 | FR | HLA-RESOLUTION | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| FR-013 | FR | HLA-RESOLUTION | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| FR-014 | FR | HLA-RESOLUTION | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| FR-015 | FR | HLA-OBSERVER | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| FR-016 | FR | HLA-OBSERVER | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| FR-017 | FR | HLA-OBSERVER | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| FR-018 | FR | HLA-OBSERVER | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| FR-019 | FR | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-020 | FR | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-021 | FR | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-022 | FR | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-023 | FR | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-024 | FR | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-025 | FR | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-026 | FR | HLA-PACKAGE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| FR-027 | FR | HLA-QUERY | [HLA-QUERY DD](../design/components/hla-query-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| FR-028 | FR | HLA-QUERY | [HLA-QUERY DD](../design/components/hla-query-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| FR-029 | FR | HLA-QUERY | [HLA-QUERY DD](../design/components/hla-query-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| FR-030 | FR | HLA-QUERY | [HLA-QUERY DD](../design/components/hla-query-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| FR-031 | FR | HLA-PERSIST | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| FR-032 | FR | HLA-PERSIST | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| FR-033 | FR | HLA-STATE | [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-034 | FR | HLA-STATE | [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-035 | FR | HLA-STATE | [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-036 | FR | HLA-STATE | [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-037 | FR | HLA-VALIDATE | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| FR-038 | FR | HLA-VALIDATE | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| FR-039 | FR | HLA-CONTRACT | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |
| FR-040 | FR | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| FR-041 | FR | HLA-PACKAGE, HLA-STATE | [HLA-PACKAGE DD](../design/components/hla-package-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PACKAGE DD](../design/components/hla-package-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) |
| FR-042 | FR | HLA-PERSIST | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| FR-043 | FR | HLA-PERSIST | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| FR-044 | FR | HLA-LIFECYCLE | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §6; [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| NFR-001 | NFR | HLA-CONTRACT | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |
| NFR-002 | NFR | HLA-VALIDATE | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| NFR-003 | NFR | HLA-CORE, HLA-OBSERVER, HLA-STATE | [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) |
| NFR-004 | NFR | HLA-PERSIST, HLA-CORE, HLA-OBSERVER | [HLA-PERSIST DD](../design/components/hla-persist-dd.md); [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-PERSIST DD](../design/components/hla-persist-dd.md); [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| NFR-005 | NFR | HLA-CONTRACT, HLA-PACKAGE | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| NFR-006 | NFR | HLA-CORE, HLA-OBSERVER, HLA-RESOLUTION | [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | N/A | N/A | N/A | Detailed Design mapped; test pending | [Engine HLA](../architecture/engine-hla.md) §8; [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |

---

<a id="3-deterministicprobabilistic-boundary-traceability"></a>
# 3. Deterministic–Probabilistic Boundary Traceability <sup>[↩](#table-of-contents "Back to ToC")</sup>

| Req ID | Boundary ID | Validation Harness | Containment Logic | Fallback Ref | Observability Ref | Drift Validation | Status |
|---|---|---|---|---|---|---|---|
| FR-014 | DPB-001 (Oracle result vs. Resolution) | HLA-VALIDATE validation harness ([Engine HLA](../architecture/engine-hla.md) §§4, 6; [HLA-VALIDATE DD](../design/components/hla-validate-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md)) | Oracle Adapter + HLA-RESOLUTION Command boundary; no direct Campaign Reality writes | FR-012 (leave Unresolved) | NFR-006 provenance via HLA-CORE/HLA-RESOLUTION event records and Oracle invocation provenance | N/A — no model drift component in scope | Detailed Design mapped; test pending |

---

<a id="4-non-functional-traceability"></a>
# 4. Non-Functional Traceability <sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR ID | Architectural Mechanism | Design Artifact | Test Case | Packaging Impact | Status |
|---|---|---|---|---|---|
| NFR-001 | HLA-CONTRACT preserves in-process Facade path; network adapter remains optional | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) | N/A | N/A | Detailed Design mapped; test pending — quantitative target provisional |
| NFR-002 | HLA-VALIDATE Chain of Responsibility gates Package/import content before core state | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) | N/A | N/A | Detailed Design mapped; test pending |
| NFR-003 | Event Sourcing plus HLA-STATE Memento checkpoint/recovery | [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) | N/A | N/A | Detailed Design mapped; test pending |
| NFR-004 | Active-window/archival split through HLA-PERSIST with HLA-CORE/HLA-OBSERVER event logs | [HLA-PERSIST DD](../design/components/hla-persist-dd.md); [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) | N/A | N/A | Detailed Design mapped; test pending — calibration provisional |
| NFR-005 | Hexagonal ports plus HLA-PACKAGE Microkernel boundary | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-PACKAGE DD](../design/components/hla-package-dd.md) | N/A | N/A | Detailed Design mapped; test pending |
| NFR-006 | Append-only event history plus HLA-RESOLUTION Command provenance | [HLA-CORE DD](../design/components/hla-core-dd.md); [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) | N/A | N/A | Detailed Design mapped; test pending |

---

<a id="5-coverage-validation-checklist"></a>
# 5. Coverage Validation Checklist <sup>[↩](#table-of-contents "Back to ToC")</sup>

- 100% FR coverage to design? **Yes** — FR-001 through FR-044 have DD Artifact mappings in Section 2.
- 100% NFR coverage to design? **Yes** — NFR-001 through NFR-006 have DD Artifact mappings in Sections 2 and 4.
- 100% implementation traceability? **N/A — Implementation phase not reached.**
- 100% requirement-to-test coverage? **N/A — Test Planning phase not reached.**
- Deterministic–probabilistic boundaries mapped? **Yes at Architecture and Detailed Design levels** — DPB-001 mapped to HLA-VALIDATE and HLA-RESOLUTION; Test Planning details remain pending.
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

---

<a id="7-approval"></a>
# 7. Approval <sup>[↩](#table-of-contents "Back to ToC")</sup>

Approved By: CodeMonki
Role: Project Owner
Date: 2026-09-17
Version Incremented: Yes — v1.1

RTM validation remains required before Implementation completion, Test phase closure, Packaging approval, and Release authorization. This v1.1 baseline authorizes Test Planning; Test Case IDs remain pending that phase.

---

End of Requirements Traceability Matrix (Detailed Design Mapped v1.1)
