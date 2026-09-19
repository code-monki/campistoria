# Campistoria Engine Test Case Inventory

Project Name: Campistoria Engine
Version: 1.0 (Approved)
Date (YYYY-MM-DD): 2026-09-17
Author(s): CodeMonki
Status: Approved
Test Plan Reference: [Engine Test Plan](engine-test-plan.md) v1.1 (Approved)
RTM Reference: [Engine RTM](../requirements/engine-rtm.md) v1.2 (Approved)

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
| TC-CON-005 | FR-045 | Integration | Create a Campaign as a locally recognized Principal; an active `campaignOwner` binding is established atomically, remains inspectable, and survives save/reload without relying on Session claims. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md); [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-CON-006 | FR-032, FR-045 | Security | Import a valid Campaign as an authorized local Principal; create a new active local `campaignOwner` binding while retaining source Principal/binding identifiers only as provenance. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |

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
| TC-NFR-AUTH-001 | NFR-007 | Security | For every Campaign-scoped operation group, compare an active sufficient binding with no binding, revoked binding, wrong-Campaign binding, and insufficient capabilities; only the active sufficient binding reaches normal component validation. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md) |

---

## 6. Feature Negative and Breakage Cases

These cases make negative coverage explicit where the primary functional case is principally nominal. Existing failure-oriented, security, reliability, and boundary cases remain the negative cases for the requirements they already exercise.

| Test Case ID | Requirement ID(s) | Level | Validation Objective and Pass Criterion | Design Reference |
|---|---|---|---|---|
| TC-LIF-006 | FR-001 | Component | Reject missing, unknown, invalid, or unvalidated Scenario references and injected initialization failure; no Campaign identity, partial Reality, or orphan persistence artifact remains. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-LIF-007 | FR-002 | Integration | Import, reload, or migrate data with a missing, substituted, or conflicting Campaign identity; fail closed rather than silently assigning a different identity. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-LIF-008 | FR-003 | Integration | Attempt shared-object, shared-stream, and cross-Campaign reference aliasing between Campaigns from one Scenario; mutation remains isolated or the invalid reference is rejected. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-LIF-009 | FR-040 | Security | Attempt direct, indirect, migration-mediated, and Package-reference-mediated Scenario mutation through Campaign operations; every path is rejected or has no effect. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-LIF-010 | FR-044 | Component | Inject seed generation failure, malformed caller seed metadata, unauthorized caller-supplied seed, and post-creation seed mutation; creation fails atomically or the mutation is rejected. | [HLA-LIFECYCLE DD](../design/components/hla-lifecycle-dd.md) |
| TC-CORE-010 | FR-004 | Component | Attempt duplicate, missing, and reassigned Entity identities during append/import; invalid identity transitions are rejected without altering the existing Entity. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-011 | FR-005 | Component | Submit invalid containment cycles, unknown participants, and Location-only queries against non-Locations; return deterministic diagnostics without inventing package spatial semantics. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-012 | FR-006 | Component | Submit an invalid target, invalid semantic reference, and conflicting same-boundary Property change; reject atomically and preserve the prior effective value. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-013 | FR-007 | Component | Create Relationships with missing, cross-Campaign, retired, or invalid participants; no dangling or partially indexed Relationship is admitted. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-014 | FR-008 | Component | Submit a Fact without required provenance, with invalid temporal bounds, or with an unresolved source reference; reject without a partial Fact or Event. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-015 | FR-009 | Component | Query before object creation, across invalid temporal bounds, and after malformed temporal transitions; return explicit absence/diagnostics rather than a fabricated State. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-016 | FR-010 | Component | Submit invalid or incomparable Campaign Time values and equal-time ordering conflicts; reject or apply the documented stable ordering without consulting package display formatting. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-CORE-017 | FR-011 | Reliability | Remove, reorder, duplicate, truncate, or corrupt Events and replay boundaries; reconstruction fails diagnostically and never returns silently corrupted authoritative state. | [HLA-CORE DD](../design/components/hla-core-dd.md) |
| TC-OBS-006 | FR-015 | Component | Use missing, duplicate, retired, or cross-Campaign Observer identity and attempt Observer operations that mutate Core; reject with no cross-boundary side effect. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| TC-OBS-007 | FR-016 | Security | Attempt automatic reconciliation, shared-record aliasing, and raw Core fallback when Observer Knowledge differs or is absent; preserve independence and explicit absence. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| TC-OBS-008 | FR-017 | Security | Record an Observation with the wrong Observer, unknown subject, invalid source, or cross-Observer target; reject without changing any Observer stream or Campaign Reality. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| TC-OBS-009 | FR-018 | Component | Remove, invalidate, or cross-scope an Information Source reference; knowledge admission fails or corruption is diagnosed rather than returning source-free knowledge as valid. | [HLA-OBSERVER DD](../design/components/hla-observer-dd.md) |
| TC-PKG-010 | FR-019 | Component | Register an exact duplicate, conflicting content under one identity/version, malformed metadata, and an invalid dependency declaration; reject without registry mutation. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-011 | FR-020 | Component | Compose with missing dependencies, invalid extension order, invalid override target, or omitted declared input; composition fails with diagnostics and no partial definition. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-012 | FR-022 | Integration | Attempt implicit pin advancement through registration, reload, query, and save; the existing Campaign pin remains bytewise or semantically unchanged. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-013 | FR-023 | Security | Invoke migration without authorization, without a valid plan, or against incompatible source/target pins; reject before Campaign or pin mutation. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-014 | FR-024 | Integration | Remove, overwrite, or ambiguously resolve one of two concurrently pinned Package versions; the affected operation fails without altering the other Campaign. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-PKG-015 | FR-026 | Component | Supply ambiguous, missing, or conflated Package-version and system/edition identifiers; reject ambiguity and preserve independence of the two identity dimensions. | [HLA-PACKAGE DD](../design/components/hla-package-dd.md) |
| TC-QRY-005 | FR-027 | Component | Query with unknown/retired Observer, invalid context, invalid Campaign Time, or cross-Campaign references; fail closed with no hidden-content fallback. | [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-QRY-006 | FR-029 | Component | Produce missing envelope fields, projection content outside `content`, or output requiring client-side visibility filtering; validation rejects the Presentation Model. | [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-QRY-007 | FR-030 | Component | Request unsupported, malformed, or incompatible Projection types; return structured diagnostics without returning a misleading default projection. | [HLA-QUERY DD](../design/components/hla-query-dd.md) |
| TC-PER-006 | FR-032 | Security | Import malformed, incomplete, incompatible-version, hostile, or integrity-invalid export artifacts; reject before any Campaign, Observer, Package-pin, or asset admission. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-PER-007 | FR-042 | Integration | Export with a missing, unreadable, mismatched, or corrupt Campaign-local asset; fail the export with diagnostics rather than producing a degraded importable artifact. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-PER-008 | FR-043 | Reliability | Query unavailable, missing, corrupt, truncated, and range-mismatched archived history; return diagnostics rather than silent empty or fabricated history. | [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |
| TC-STA-007 | FR-036 | Security | Attempt unauthorized Retcon, invalid target, stale prior value, and undeterminable/cross-Campaign references; reject atomically and preserve prior history. | [HLA-STATE DD](../design/components/hla-state-dd.md) |
| TC-VAL-005 | FR-038 | Component | Force diagnostic production with missing subject, category, severity, or readable message data; reject or normalize the diagnostic so required fields are never silently absent. | [HLA-VALIDATE DD](../design/components/hla-validate-dd.md) |
| TC-CON-007 | FR-045, NFR-007 | Security | Attempt Campaign access using caller-supplied role/capability claims, a missing or revoked binding, another Campaign's binding, and imported foreign Principal/binding identifiers; reject before forwarding with no authoritative, provenance, or binding side effect. | [HLA-CONTRACT DD](../design/components/hla-contract-dd.md); [HLA-PERSIST DD](../design/components/hla-persist-dd.md) |

### 6.1 Per-Feature Negative Coverage Matrix

| Requirement | Negative / Breakage Test Case ID(s) |
|---|---|
| FR-001 | TC-LIF-006 |
| FR-002 | TC-LIF-007 |
| FR-003 | TC-LIF-008 |
| FR-004 | TC-CORE-010 |
| FR-005 | TC-CORE-011 |
| FR-006 | TC-CORE-012 |
| FR-007 | TC-CORE-013 |
| FR-008 | TC-CORE-014 |
| FR-009 | TC-CORE-015 |
| FR-010 | TC-CORE-016 |
| FR-011 | TC-CORE-017 |
| FR-012 | TC-DPB-001 |
| FR-013 | TC-DPB-003, TC-DPB-004 |
| FR-014 | TC-DPB-001, TC-DPB-002, TC-DPB-004 |
| FR-015 | TC-OBS-006 |
| FR-016 | TC-OBS-007 |
| FR-017 | TC-OBS-008 |
| FR-018 | TC-OBS-009 |
| FR-019 | TC-PKG-010 |
| FR-020 | TC-PKG-011 |
| FR-021 | TC-PKG-003 |
| FR-022 | TC-PKG-012 |
| FR-023 | TC-PKG-013 |
| FR-024 | TC-PKG-014 |
| FR-025 | TC-PKG-007 |
| FR-026 | TC-PKG-015 |
| FR-027 | TC-QRY-005 |
| FR-028 | TC-QRY-002, TC-CON-003 |
| FR-029 | TC-QRY-006 |
| FR-030 | TC-QRY-007 |
| FR-031 | TC-STA-003, TC-NFR-REL-001 |
| FR-032 | TC-PER-006 |
| FR-033 | TC-STA-006 |
| FR-034 | TC-STA-005 |
| FR-035 | TC-STA-003, TC-NFR-REL-001 |
| FR-036 | TC-STA-007 |
| FR-037 | TC-VAL-001, TC-CON-004, TC-VAL-003, TC-DPB-002 |
| FR-038 | TC-VAL-005 |
| FR-039 | TC-CON-002, TC-CON-004, TC-DPB-004 |
| FR-040 | TC-LIF-009 |
| FR-041 | TC-PKG-009, TC-STA-005 |
| FR-042 | TC-PER-007 |
| FR-043 | TC-PER-008 |
| FR-044 | TC-LIF-010 |
| FR-045 | TC-CON-007 |

---

## 7. Execution Status Vocabulary

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

## 8. Inventory Validation

- FR-001 through FR-045 mapped? **Yes.**
- NFR-001 through NFR-007 mapped? **Yes.**
- Every FR has an explicit negative, boundary, or breakage mapping? **Yes.**
- DPB-001 invocation, validation, acceptance, containment, fallback, provenance, and reproducibility covered? **Yes.**
- Failure, recovery, rollback, authorization, POV isolation, and hostile-input cases present? **Yes.**
- Implementation references available? **No — controlled Implementation is authorized, but implementation units have not yet been created.**
- Execution evidence available? **No — Test execution has not begun.**
- Human approval granted? **Yes** — approved by the project owner on 2026-09-17.

---

## Approval

Approved By: CodeMonki
Role: Project Owner
Date: 2026-09-17
Version Incremented: Yes — v1.0

---

End of Approved Test Case Inventory v1.0.
