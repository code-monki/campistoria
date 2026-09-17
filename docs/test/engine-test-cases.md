# Campistoria Engine Test Case Inventory

Project Name: Campistoria Engine
Version: 0.1 (Test Planning Candidate)
Date (YYYY-MM-DD): 2026-09-17
Author(s): CodeMonki
Status: Draft
Test Plan Reference: [Engine Test Plan](engine-test-plan.md) v1.0 (Approved)
RTM Reference: [Engine RTM](../requirements/engine-rtm.md) v1.2 (Test Planned Candidate)

---

## 1. Case Conventions

Test Case IDs use a stable owner prefix:

| Prefix | Owner / Concern |
|---|---|
| `TC-LIF` | HLA-LIFECYCLE |
| `TC-CORE` | HLA-CORE |
| `TC-RES` | HLA-RESOLUTION |
| `TC-OBS` | HLA-OBSERVER |
| `TC-PKG` | HLA-PACKAGE |
| `TC-QRY` | HLA-QUERY |
| `TC-PER` | HLA-PERSIST |
| `TC-STA` | HLA-STATE |
| `TC-VAL` | HLA-VALIDATE |
| `TC-CON` | HLA-CONTRACT |
| `TC-DPB` | Deterministic-probabilistic boundary DPB-001 |
| `TC-NFR` | Cross-cutting non-functional validation |

Each case SHALL record concrete preconditions, fixtures, steps, expected results, implementation references, environment, and evidence when it is implemented. This planning inventory defines behavioral intent and acceptance posture; it does not prescribe a test framework.

---

## 2. Functional Test Cases

| Test Case ID | Requirement ID(s) | Level | Validation Objective and Pass Criterion | Design Reference |
|---|---|---|---|---|
| TC-LIF-001 | FR-001 | System | Instantiate from a valid Scenario; Campaign and Scenario identities differ, and a before/after comparison proves Scenario content unchanged. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-LIF-002 | FR-002 | Integration | Persist/reload, export/import, and migrate a Campaign; the Campaign identity remains unchanged across every supported transition. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-LIF-003 | FR-003 | System | Instantiate two Campaigns from one Scenario, mutate one, and prove identities, seeds, Event streams, and Campaign Reality remain independent. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-LIF-004 | FR-044 | Integration | Verify every new Campaign has a retrievable seed and that it remains stable across persistence and export/import; verify authorized caller-supplied seed source metadata when used. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-LIF-005 | FR-040 | Component | Attempt Scenario mutation through Campaign-facing operations; the request is rejected or has no effect, and the stored Scenario remains bytewise or semantically equivalent. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-CORE-001 | FR-004 | Component | Apply Property, Relationship, and State changes to an Entity; its identity remains stable through current and historical queries. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-002 | FR-005 | Component | Create Location and non-Location Entities; Location containment/topology queries work without imposing package-specific spatial semantics on ordinary Entities. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-003 | FR-006 | Component | Attach Properties to Entity, Relationship, and Event targets; time-scoped retrieval returns the latest value effective at the requested Campaign Time. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-004 | FR-007 | Component | Create a Package-defined Relationship between Entities without engine-predefined category knowledge; queries return stable identity, participants, and semantic type reference. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-005 | FR-008 | Component | Record and retrieve Facts with Information Source or Resolution provenance and effective Campaign Time; missing required provenance is rejected. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-006 | FR-009 | Component | Change State over multiple Campaign Times and query each boundary; historical values remain correct after later changes. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-007 | FR-010 | Component | Record Events at distinct Campaign Times using package-formatted calendars; engine ordering follows generic Campaign Time, not display formatting or insertion accident. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-008 | FR-011 | Integration | Reconstruct Campaign Reality by folding the ordered Event history and prove equivalence with current authoritative state; failed append leaves no partial Event. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-RES-001 | FR-012 | System | Query an Unresolved element; unresolved status is explicit and no Oracle, Procedure, Rule, or synthesized answer is invoked. | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| TC-RES-002 | FR-013 | Integration | Apply authorized human-, Procedure-, Rule-, and Oracle-sourced Resolution Commands; resulting Campaign Reality is resolved and source provenance is retrievable. | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| TC-RES-003 | FR-014 | Integration | Generate valid and invalid Oracle results without applying Resolution; Campaign Reality remains unchanged in every case. | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| TC-OBS-001 | FR-015 | Component | Create/query an Observer and mutate Observer Knowledge; Campaign Reality and its Event stream remain unchanged. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| TC-OBS-002 | FR-016 | Component | Store false, stale, incomplete, private, and omitted knowledge for an Observer; no automatic reconciliation with Campaign Reality occurs. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| TC-OBS-003 | FR-017 | Integration | Record an Observation for one Observer; only that Observer's knowledge stream changes, with no Campaign Reality or other-Observer mutation. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| TC-OBS-004 | FR-018 | Component | Retrieve Observer Knowledge with Information Source provenance; creation without a source is rejected with no partial record. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| TC-PKG-001 | FR-019 | Component | Register and independently resolve two versions of one Package identity. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-002 | FR-020 | Component | Compose an Effective Campaign Definition from explicit dependencies, extensions, and overrides; all declared inputs are represented with no silent omission. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-003 | FR-021 | Component | Introduce an undeclared collision; composition fails with structured conflict diagnostics and produces no usable partial definition. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-004 | FR-022 | Integration | Register a newer Package version after Campaign creation; the Campaign's composition pin remains unchanged until explicit migration. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-005 | FR-023 | Integration | Prove Package registration alone cannot migrate an existing Campaign; only an explicit authorized migration operation changes its pin. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-006 | FR-024 | System | Operate two Campaigns concurrently in one process with different versions of the same Package identity; both resolve semantics against their own pins. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-007 | FR-025 | Component | Remove a referenced Package version with and without force; default removal is rejected with referencing Campaign IDs, while forced behavior produces required diagnostics. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-008 | FR-026 | Component | Register Packages demonstrating independence of Package version and system/edition identity; lookup and composition preserve both dimensions. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-009 | FR-041 | Integration | Inject failure at migration stages; composition pin and Campaign Reality equal the pre-migration state and recovery diagnostics identify the failure. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) |
| TC-QRY-001 | FR-027 | System | Query by Observer, context, and Campaign Time across allowed, hidden, false, stale, and archived information; only POV-approved information is returned. | [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-QRY-002 | FR-028 | Security | Attempt hidden-field, cross-observer, raw-store, tooling-view, and archived-query disclosure; prohibited content is suppressed or rejected by the engine. | [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-QRY-003 | FR-029 | System | Render a returned Presentation Model using only its public envelope/content; no client-side visibility, POV, or knowledge filtering is required. | [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-QRY-004 | FR-030 | Component | Generate map, timeline, roster, journal, and relationship-view projections for one source state; outputs are distinct but share the same approved source basis. | [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-PER-001 | FR-031 | System | Save, close, and reopen a Campaign; representative public queries before and after are semantically equivalent, including Observer Knowledge and provenance. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-PER-002 | FR-032 | System | Export from one storage adapter and import without that adapter; the resulting Campaign is semantically equivalent and separately reconstructible. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-PER-003 | FR-042 | System | Export/import a Campaign-local asset onto a clean system without prior asset access; identity, bytes, metadata, and references are preserved. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-PER-004 | FR-043 | Integration | Keep older history outside the active working set, prove ordinary queries do not load it, then retrieve it explicitly on demand with POV controls intact. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-STA-001 | FR-033 | System | Create a Checkpoint after known state, mutate further, restore, and prove Campaign Reality and Observer Knowledge match one coherent checkpoint boundary. | [HLA-STATE DD](../design/components/hla-state-dd.md) |
| TC-STA-002 | FR-034 | System | Undo the latest eligible Resolution and a contiguous LIFO range; restored state is correct and no distinct Campaign-history occurrence is added beyond cancellation semantics. | [HLA-STATE DD](../design/components/hla-state-dd.md) |
| TC-STA-003 | FR-035 | Reliability | Interrupt writes at controlled points and invoke Recovery; restore to the last valid persisted state without manual repair or return a specific diagnosable failure. | [HLA-STATE DD](../design/components/hla-state-dd.md) |
| TC-STA-004 | FR-036 | System | Apply an authorized Retcon; append correction history with prior value, provenance, and determinable direct references without rewriting prior Events. | [HLA-STATE DD](../design/components/hla-state-dd.md) |
| TC-VAL-001 | FR-037 | Integration | Validate missing dependency, malformed artifact, semantic violation, and warning-only admission cases; every state-admitting flow rejects before partial state change. | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| TC-VAL-002 | FR-038 | Component | Produce representative validation and migration failures; diagnostics include affected reference, category, severity, and human-readable message where determinable. | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| TC-CON-001 | FR-039 | System | Exercise equivalent first-party and third-party callers against the same operation catalog and capability grants; neither has a private operation or internal-component shortcut. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |

---

## 3. Cross-Cutting Contract and Boundary Cases

| Test Case ID | Requirement ID(s) | Level | Validation Objective and Pass Criterion | Design Reference |
|---|---|---|---|---|
| TC-CON-002 | FR-039, NFR-002 | Security | For each mutating operation group, omit or substitute the required capability; forwarding is denied, diagnostics are returned, and no authoritative or provenance side effect occurs. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |
| TC-CON-003 | FR-027, FR-028, FR-029 | Integration | Reconnect with current, stale, compactable, expired, mismatched, and unsafe stream-scoped revision markers; return unchanged, delta, compacted delta, full state, or diagnostic exactly as specified. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-CON-004 | FR-037, FR-039, NFR-002 | Security | Attempt Package registration, Campaign import, migration, and Oracle result acceptance through every exposed path; none can bypass HLA-VALIDATE or capability enforcement. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| TC-CORE-009 | FR-011, FR-031, NFR-003 | Reliability | Rebuild derived indexes and active state from Events or snapshot-plus-Events; rebuilt queries match the authoritative fold and indexes never become an alternate source of truth. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-OBS-005 | FR-015, FR-016, FR-017, FR-033 | Integration | Snapshot, archive, restore, and query multiple Observer streams; logical checkpoint alignment is preserved without cross-observer disclosure. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md); [HLA-STATE DD](../design/components/hla-state-dd.md) |
| TC-PER-005 | FR-032, FR-042, NFR-002 | Security | Corrupt, truncate, substitute, or remove archive segments and asset payloads; integrity checks reject them before return or admission and identify the affected artifact. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-STA-005 | FR-034, FR-041 | Integration | Reject non-LIFO, ineligible, state-mismatched, or dependency-breaking Undo as Undo and route eligible correction intent through HLA-RESOLUTION rather than direct HLA-CORE mutation. | [HLA-STATE DD](../design/components/hla-state-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| TC-STA-006 | FR-033, FR-035, NFR-003 | Reliability | Trigger Checkpoints by controlled wall-clock threshold, Event-count threshold, and graceful close; corrupt/mismatch each snapshot class and verify fail-closed restore diagnostics. | [HLA-STATE DD](../design/components/hla-state-dd.md) |
| TC-VAL-003 | FR-037, NFR-002 | Security | Supply declarative Package validators that attempt unsupported execution or engine-core access; engine-owned validation remains first-class and the attempt is rejected. | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| TC-VAL-004 | FR-037, FR-038 | Component | Run identical validator chains repeatedly and prove deterministic ordering, stopping behavior, decision, and diagnostic ordering. | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |

---

## 4. Deterministic-Probabilistic Boundary Cases

| Test Case ID | Requirement ID(s) | Level | Validation Objective and Pass Criterion | Design Reference |
|---|---|---|---|---|
| TC-DPB-001 | FR-012, FR-014 | Integration | Oracle invocation alone never changes Campaign Reality; failure or rejection leaves the target explicitly Unresolved. | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| TC-DPB-002 | FR-014, FR-037, NFR-002 | Integration | Invalid Oracle envelopes and contract-violating results are rejected by HLA-VALIDATE before HLA-RESOLUTION acceptance. | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| TC-DPB-003 | FR-013, FR-014 | System | A valid Oracle result remains non-authoritative until explicit human/GM acceptance and an authorized Resolution Command reference it. | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |
| TC-DPB-004 | FR-013, FR-014, FR-039 | Security | Attempt direct Oracle, Procedure, Rule, Package, adapter, and unauthorized caller writes to HLA-CORE; every path is structurally denied. | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md); [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |
| TC-DPB-005 | FR-013, NFR-006 | Integration | Accepted and rejected Oracle invocations retain required provenance; only accepted Resolution effects become HLA-CORE Events. | [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md); [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-DPB-006 | FR-044 | Component | Within one implementation and fixed Package/Oracle version, repeat an approved seeded Oracle sequence under identical invocation context and record repeatability; do not assert cross-implementation equivalence. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md); [HLA-RESOLUTION DD](../design/components/hla-resolution-dd.md) |

---

## 5. Non-Functional Test Cases

| Test Case ID | Requirement ID(s) | Level | Validation Objective and Pass Criterion | Design Reference |
|---|---|---|---|---|
| TC-NFR-PERF-001 | NFR-001 | Performance | Measure a documented representative mix of local, single-process public Transactions after warm-up; at least 95% complete within the provisional 500 ms target, excluding transport time. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |
| TC-NFR-SEC-001 | NFR-002 | Security | Execute malformed, hostile, unauthorized, warning-only, and bypass inputs at all approved trust boundaries; none affect authoritative state before strict acceptance. | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md); [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |
| TC-NFR-REL-001 | NFR-003 | Reliability | Inject interrupted writes and corrupt latest recovery material across Core and Observer streams; recover coherently to the last valid state or emit a specific failure without silent corruption. | [HLA-STATE DD](../design/components/hla-state-dd.md); [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-NFR-SCALE-001 | NFR-004 | Performance | With a Package-defined Campaign Time mapping, advance beyond the provisional active window; ordinary queries avoid archive loads while explicit historical queries retrieve correct data on demand. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md); [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-NFR-MAINT-001 | NFR-005 | Architecture | Introduce a representative Package semantic role and substitute a projection or persistence adapter in a controlled branch; verify unrelated contracts and Reality/Knowledge separation require no redefinition. | [Engine Detailed Design](../design/engine-detailed-design.md) |
| TC-NFR-AUD-001 | NFR-006 | System | Exercise Resolution, migration, Retcon, Recovery, package-composition change, and other consequential mutations; retrieve records identifying what, when, mechanism, and source/actor as applicable. | [Engine Detailed Design](../design/engine-detailed-design.md) |

---

## 6. Execution Status Vocabulary

| Status | Meaning |
|---|---|
| Planned | Case approved but not yet implemented. |
| Implemented | Executable case exists but has no result for the target build. |
| Passed | Expected results satisfied with recorded evidence. |
| Failed | One or more expected results not satisfied; defect reference required. |
| Blocked | Execution prevented by a named dependency or environment condition. |
| Not Applicable | Allowed only after approved scope or requirement change; rationale required. |

All cases in this inventory are **Planned** until implementation references and execution evidence are added through the RTM.

---

## 7. Inventory Validation

- FR-001 through FR-044 mapped? **Yes — candidate.**
- NFR-001 through NFR-006 mapped? **Yes — candidate.**
- DPB-001 invocation, validation, acceptance, containment, fallback, provenance, and reproducibility covered? **Yes — candidate.**
- Failure, recovery, rollback, authorization, POV isolation, and hostile-input cases present? **Yes — candidate.**
- Implementation references available? **No — Implementation not authorized.**
- Execution evidence available? **No — Test execution has not begun.**
- Human approval granted? **No — pending review.**

---

## Approval

Approved By:
Role:
Date:
Version Incremented: Pending — v1.0 candidate

---

End of Test Case Inventory Candidate.
