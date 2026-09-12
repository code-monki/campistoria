# Requirements Traceability Matrix (RTM)

Project Name: Campistoria Engine
Version: 1.0 (Architecture Mapped)
Date (YYYY-MM-DD): 2026-09-12
Maintained By: CodeMonki
Status: Approved Architecture Mapping
Requirement Version Reference: `docs/requirements/engine-srs.md` v1.0 (Approved)
Architecture Version Reference: `docs/architecture/engine-hla.md` v1.0 (Approved)
Design Version Reference: `docs/design/engine-detailed-design.md` v0.1 (Draft)
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

- Requirements approved? **Yes** — `engine-srs.md` v1.0 approved by the project owner (CodeMonki) on 2026-09-08.
- Architecture approved? **Yes** — `engine-hla.md` v1.0 approved by the project owner (CodeMonki) on 2026-09-12.
- Detailed Design approved? **No.**
- Test Plan aligned? **No.**
- Advancement to next phase authorized? **Yes** — the Architecture→Detailed Design gate is cleared as of 2026-09-12.

This RTM now records the approved Architecture-phase requirement-to-component mapping. A Detailed Design entry draft exists, but Design, Implementation, Test, Packaging, and Orchestration columns remain unpopulated until those later phases produce approved artifacts.

---

<a id="2-core-traceability-matrix"></a>
# 2. Core Traceability Matrix <sup>[↩](#table-of-contents "Back to ToC")</sup>

The HLA Component ID column is populated from approved `engine-hla.md` v1.0. Design, Implementation, Test, Packaging, and Orchestration columns remain **N/A — pending approved artifact** and will be populated as those phases produce approved artifacts.

| Req ID | Req Type | HLA Component ID | DD Artifact | Implementation Unit | Test Case ID | Packaging Ref | Orchestration Ref | Validation Status | Evidence Ref |
|---|---|---|---|---|---|---|---|---|---|
| FR-001 | FR | HLA-LIFECYCLE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-002 | FR | HLA-LIFECYCLE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-003 | FR | HLA-LIFECYCLE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-004 | FR | HLA-CORE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-005 | FR | HLA-CORE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-006 | FR | HLA-CORE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-007 | FR | HLA-CORE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-008 | FR | HLA-CORE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-009 | FR | HLA-CORE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-010 | FR | HLA-CORE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-011 | FR | HLA-CORE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-012 | FR | HLA-RESOLUTION | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-013 | FR | HLA-RESOLUTION | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-014 | FR | HLA-RESOLUTION | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-015 | FR | HLA-OBSERVER | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-016 | FR | HLA-OBSERVER | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-017 | FR | HLA-OBSERVER | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-018 | FR | HLA-OBSERVER | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-019 | FR | HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-020 | FR | HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-021 | FR | HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-022 | FR | HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-023 | FR | HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-024 | FR | HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-025 | FR | HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-026 | FR | HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-027 | FR | HLA-QUERY | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-028 | FR | HLA-QUERY | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-029 | FR | HLA-QUERY | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-030 | FR | HLA-QUERY | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-031 | FR | HLA-PERSIST | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-032 | FR | HLA-PERSIST | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-033 | FR | HLA-STATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-034 | FR | HLA-STATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-035 | FR | HLA-STATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-036 | FR | HLA-STATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-037 | FR | HLA-VALIDATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-038 | FR | HLA-VALIDATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-039 | FR | HLA-CONTRACT | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-040 | FR | HLA-LIFECYCLE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-041 | FR | HLA-PACKAGE, HLA-STATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-042 | FR | HLA-PERSIST | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-043 | FR | HLA-PERSIST | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| FR-044 | FR | HLA-LIFECYCLE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §6 |
| NFR-001 | NFR | HLA-CONTRACT | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §8 |
| NFR-002 | NFR | HLA-VALIDATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §8 |
| NFR-003 | NFR | HLA-CORE, HLA-OBSERVER, HLA-STATE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §8 |
| NFR-004 | NFR | HLA-PERSIST, HLA-CORE, HLA-OBSERVER | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §8 |
| NFR-005 | NFR | HLA-CONTRACT, HLA-PACKAGE | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §8 |
| NFR-006 | NFR | HLA-CORE, HLA-OBSERVER, HLA-RESOLUTION | N/A | N/A | N/A | N/A | N/A | Architecture mapped; design/test pending | engine-hla.md §8 |

---

<a id="3-deterministicprobabilistic-boundary-traceability"></a>
# 3. Deterministic–Probabilistic Boundary Traceability <sup>[↩](#table-of-contents "Back to ToC")</sup>

| Req ID | Boundary ID | Validation Harness | Containment Logic | Fallback Ref | Observability Ref | Drift Validation | Status |
|---|---|---|---|---|---|---|---|
| FR-014 | DPB-001 (Oracle result vs. Resolution) | HLA-VALIDATE validation harness (`engine-hla.md` §§4, 6) | Oracle Adapter + HLA-RESOLUTION Command boundary; no direct Campaign Reality writes | FR-012 (leave Unresolved) | NFR-006 provenance via HLA-CORE/HLA-RESOLUTION event records | N/A — no model drift component in scope | Architecture mapped; design/test pending |

---

<a id="4-non-functional-traceability"></a>
# 4. Non-Functional Traceability <sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR ID | Architectural Mechanism | Design Artifact | Test Case | Packaging Impact | Status |
|---|---|---|---|---|---|
| NFR-001 | HLA-CONTRACT preserves in-process Facade path; network adapter remains optional | N/A | N/A | N/A | Architecture mapped; design/test pending — quantitative target provisional |
| NFR-002 | HLA-VALIDATE Chain of Responsibility gates Package/import content before core state | N/A | N/A | N/A | Architecture mapped; design/test pending |
| NFR-003 | Event Sourcing plus HLA-STATE Memento checkpoint/recovery | N/A | N/A | N/A | Architecture mapped; design/test pending |
| NFR-004 | Active-window/archival split through HLA-PERSIST with HLA-CORE/HLA-OBSERVER event logs | N/A | N/A | N/A | Architecture mapped; design/test pending — calibration provisional |
| NFR-005 | Hexagonal ports plus HLA-PACKAGE Microkernel boundary | N/A | N/A | N/A | Architecture mapped; design/test pending |
| NFR-006 | Append-only event history plus HLA-RESOLUTION Command provenance | N/A | N/A | N/A | Architecture mapped; design/test pending |

---

<a id="5-coverage-validation-checklist"></a>
# 5. Coverage Validation Checklist <sup>[↩](#table-of-contents "Back to ToC")</sup>

- 100% FR coverage to design? **N/A — Design phase not reached.**
- 100% NFR coverage to architecture? **Yes** — NFR-001 through NFR-006 mapped in Section 4.
- 100% implementation traceability? **N/A — Implementation phase not reached.**
- 100% requirement-to-test coverage? **N/A — Test Planning phase not reached.**
- Deterministic–probabilistic boundaries mapped? **Yes at Architecture level** — DPB-001 mapped to HLA-VALIDATE and HLA-RESOLUTION; design/test details pending later phases.
- Packaging traceability complete? **N/A.**
- Orchestration traceability complete? **N/A.**
- No orphan artifacts? **Yes** — every HLA component traces to Requirement IDs, and every FR/NFR row has Architecture mapping or cross-cutting NFR mapping.

---

<a id="6-change-control-and-lineage"></a>
# 6. Change Control and Lineage <sup>[↩](#table-of-contents "Back to ToC")</sup>

| Date | Change Summary | Impacted IDs | Phase Rollback Required |
|---|---|---|---|
| 2026-09-08 | Initial RTM scaffold created alongside `engine-srs.md` v0.1 | FR-001–FR-041, NFR-001–NFR-006 | No |
| 2026-09-08 | Added FR-042 (Campaign-Local Asset Portability) following requirements review of Section 10 asset-persistence coverage | FR-042 | No |
| 2026-09-08 | Revised NFR-001 (Performance) to scope to local/in-process Transactions only, deferring network/multi-user latency; added glossary term "Transaction"; added risk entry for single-user assumption rework | NFR-001 | No |
| 2026-09-08 | Revised NFR-004 (Scalability) to describe active-window/archival-transition behavior without prescribing a data structure; added FR-043 (On-Demand Access to Archived Campaign History) to close functional gap exposed by NFR-004 | NFR-004, FR-043 | No |
| 2026-09-08 | Added FR-044 (Campaign Seed Assignment) and resolved Section 6 "Reproducibility expectations" open item, scoped to software-implemented Oracles within a single conformant implementation; cross-implementation PRNG-sharing left open for future SRD | FR-044 | No |
| 2026-09-08 | `engine-srs.md` approved by project owner (CodeMonki) as v1.0; Requirements→Architecture gate cleared | FR-001–FR-044, NFR-001–NFR-006 | No |
| 2026-09-12 | `engine-hla.md` approved by project owner (CodeMonki) as v1.0; HLA Component ID mappings populated; Architecture→Detailed Design gate cleared | FR-001–FR-044, NFR-001–NFR-006 | No |

---

<a id="7-approval"></a>
# 7. Approval <sup>[↩](#table-of-contents "Back to ToC")</sup>

Approved By: CodeMonki
Role: Project Owner
Date: 2026-09-12
Version Incremented: Yes — v1.0

RTM validation required before Implementation completion, Test phase closure, Packaging approval, and Release authorization — none of which apply at this phase.

---

End of Requirements Traceability Matrix (Architecture Mapped v1.0)
