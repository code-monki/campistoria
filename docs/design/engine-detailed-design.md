# Campistoria Engine Detailed Design

Project Name: Campistoria Engine
Version: 0.1 (Detailed Design Entry Draft)
Date (YYYY-MM-DD): 2026-09-12
Author(s): CodeMonki
Status: Draft
Architecture Version Reference: `docs/architecture/engine-hla.md` v1.0 (Approved)
Requirement Version Reference: `docs/requirements/engine-srs.md` v1.0 (Approved)
RTM Version Reference: `docs/requirements/engine-rtm.md` v1.0 (Architecture Mapped)

---

## Table of Contents

- [1. Design Authority Declaration](#1-design-authority-declaration)
- [2. Scope of This Design](#2-scope-of-this-design)
- [3. Architectural Conformance](#3-architectural-conformance)
- [4. Detailed Design Work Breakdown](#4-detailed-design-work-breakdown)
- [5. Cross-Component Design Rules](#5-cross-component-design-rules)
- [6. Carried-Forward Design Questions](#6-carried-forward-design-questions)
- [7. Traceability Plan](#7-traceability-plan)
- [8. Initial Risk Assessment](#8-initial-risk-assessment)
- [9. Phase Gate Declaration](#9-phase-gate-declaration)
- [Approval](#approval)

---

<a id="1-design-authority-declaration"></a>
# 1. Design Authority Declaration <sup>[↩](#table-of-contents "Back to ToC")</sup>

- Architecture phase approved? **Yes** — `engine-hla.md` v1.0, approved by the project owner on 2026-09-12.
- Architectural Component IDs stable? **Yes** — HLA-LIFECYCLE, HLA-CORE, HLA-RESOLUTION, HLA-OBSERVER, HLA-PACKAGE, HLA-QUERY, HLA-PERSIST, HLA-STATE, HLA-VALIDATE, and HLA-CONTRACT.
- NFR structural intent documented? **Yes** — `engine-hla.md` §8 and `engine-rtm.md` §4.
- Requirement-to-Architecture mapping complete? **Yes** — `engine-rtm.md` v1.0, Architecture Mapped.
- Advancement to Detailed Design authorized? **Yes** — `engine-hla.md` §15 authorizes Architecture to Detailed Design advancement.

Detailed Design work is therefore permitted. Implementation remains prohibited during this phase.

---

<a id="2-scope-of-this-design"></a>
# 2. Scope of This Design <sup>[↩](#table-of-contents "Back to ToC")</sup>

This document starts the Detailed Design phase for the Campistoria Engine. It establishes phase authority, shared design constraints, component refinement order, and carried-forward design questions. It does not claim that component-level interface contracts are complete.

**Architectural Component IDs in scope:** all ten approved HLA components: HLA-LIFECYCLE, HLA-CORE, HLA-RESOLUTION, HLA-OBSERVER, HLA-PACKAGE, HLA-QUERY, HLA-PERSIST, HLA-STATE, HLA-VALIDATE, and HLA-CONTRACT.

**Requirement IDs in scope:** FR-001 through FR-044 and NFR-001 through NFR-006 as approved in `engine-srs.md` v1.0.

**Document boundaries:**

- Refines approved HLA structure into a controlled Detailed Design work plan.
- Captures design-wide invariants that every component-level design SHALL preserve.
- Identifies the first design questions that must be settled before implementation planning.
- Defers complete method signatures, schemas, error catalogs, state machines, and test IDs to component-level Detailed Design sections or follow-on artifacts.

**Explicit exclusions:**

- No new architectural components are introduced.
- No system boundary, HLA dependency direction, or deterministic-probabilistic boundary is changed.
- No implementation language, storage technology, UI framework, deployment platform, transport protocol, CI/CD platform, or package manager is selected.
- No code, package layout, build script, runtime configuration, or implementation unit is authorized by this document.

Architectural change requires rollback to Architecture phase and RTM change control.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance <sup>[↩](#table-of-contents "Back to ToC")</sup>

Detailed Design SHALL conform to `engine-hla.md` v1.0 and preserve the following structural constraints:

- HLA-CONTRACT remains the only public entry surface for reference clients, third-party clients, authoring tools, Package content, and future integrations.
- HLA-CONTRACT performs actor/session authorization before forwarding mutating requests to internal components.
- HLA-VALIDATE remains the mandatory validation gate for Package registration, Campaign import, and Oracle result acceptance.
- HLA-RESOLUTION remains the only component that writes resolved Campaign Reality changes to HLA-CORE.
- HLA-CORE and HLA-OBSERVER remain separate Event Sourcing streams; Campaign Reality and Observer Knowledge SHALL NOT be merged for storage convenience.
- HLA-PACKAGE remains a Microkernel boundary; Package content SHALL NOT gain direct access to engine-core internals.
- HLA-QUERY computes Presentation Models from authoritative state and SHALL NOT persist Presentation Models as authoritative data.
- HLA-PERSIST provides durable storage, archival retrieval, import, and export without owning live authoritative state.
- HLA-STATE manages Checkpoints, Undo, Recovery, and Retcon without rewriting historical Events.
- Probabilistic outputs SHALL NOT self-authorize or mutate deterministic state without validation and Resolution.

The project uses Spiral Development. New risks, missed requirements, or boundary conflicts discovered during Detailed Design are expected to be handled through explicit change control, including rollback to Requirements or Architecture when needed.

---

<a id="4-detailed-design-work-breakdown"></a>
# 4. Detailed Design Work Breakdown <sup>[↩](#table-of-contents "Back to ToC")</sup>

The components below SHALL be refined into component-level Detailed Design sections before implementation planning. Priority reflects dependency order and risk, not delivery order.

| Priority | Component | Requirement IDs | Design Focus |
|---|---|---|---|
| 1 | HLA-CONTRACT | FR-039; cross-cutting NFR-001, NFR-005 | Public operation catalog, embedded/hosted adapter neutrality, actor/session identity, capability enforcement, error exposure, request/response conventions. |
| 2 | HLA-VALIDATE | FR-037, FR-038, NFR-002 | Validation subject taxonomy, validator chain ordering, diagnostic schema, import/Package/Oracle validation contracts, XML/SVG-style structural validation posture where applicable. |
| 3 | HLA-RESOLUTION | FR-012, FR-013, FR-014, NFR-006 | Resolution Command model, Oracle Adapter contract, acceptance/rejection criteria, deterministic fallback, undo relationship, provenance records. |
| 4 | HLA-CORE | FR-004 through FR-011, NFR-003, NFR-004, NFR-006 | Campaign Reality event model, state fold rules, Fact/Relationship/Time invariants, atomic append semantics, history reconstruction. |
| 5 | HLA-OBSERVER | FR-015 through FR-018, NFR-003, NFR-004, NFR-006 | Observer Knowledge event model, information-source provenance, isolation from Campaign Reality, POV query support. |
| 6 | HLA-PACKAGE | FR-019 through FR-026, FR-041, NFR-005 | Package registry, version pinning, composition rules, dependency removal, migration contract, conflict diagnostics. |
| 7 | HLA-STATE | FR-033 through FR-036, FR-041, NFR-003 | Checkpoint schema, snapshot cadence configuration, recovery orchestration, undo inverse-command semantics, retcon provenance. |
| 8 | HLA-PERSIST | FR-031, FR-032, FR-042, FR-043, NFR-004 | Storage abstraction, archival boundary, import/export representation, asset portability, active-window retrieval behavior. |
| 9 | HLA-QUERY | FR-027 through FR-030 | Query contract, POV resolution, Projection strategy selection, Presentation Model lifecycle, reconnect delta/snapshot behavior. |
| 10 | HLA-LIFECYCLE | FR-001, FR-002, FR-003, FR-040, FR-044 | Scenario-to-Campaign instantiation, Campaign identity, seed assignment, immutable Scenario reference, branching/divergence semantics. |

Each component-level refinement SHALL define responsibilities, explicit interface contracts, data structures, validation rules, error semantics, preconditions, postconditions, side effects, determinism expectations, failure behavior, NFR derivation, and test alignment.

---

<a id="5-cross-component-design-rules"></a>
# 5. Cross-Component Design Rules <sup>[↩](#table-of-contents "Back to ToC")</sup>

The following rules are mandatory acceptance criteria for component-level Detailed Design and later Test Planning:

1. No external caller, first-party or third-party, may bypass HLA-CONTRACT.
2. Every mutating HLA-CONTRACT operation SHALL declare required capabilities before component forwarding is allowed.
3. Authorization SHALL be capability-enforced. Roles MAY assign capability bundles, but permission checks SHALL resolve to explicit capabilities rather than hardcoded role names.
4. All Package content, imported Campaign data, and Oracle results SHALL pass HLA-VALIDATE before they can influence authoritative state.
5. HLA-RESOLUTION is the exclusive Campaign Reality write path after Campaign instantiation.
6. Campaign Reality, Observer Knowledge, Package definitions, Checkpoints, and exported artifacts SHALL have explicit ownership and lifecycle rules.
7. Campaign Reality and Observer Knowledge SHALL remain independently reconstructable from their own event streams.
8. Undo SHALL reverse an unwanted Resolution without becoming Campaign history; Retcon SHALL append provenance-preserving correction history rather than rewriting Events.
9. Client synchronization SHALL distinguish initial state transfer from subsequent delta transfer; reconnecting clients MAY present a last-known revision or delta number so the engine can choose missing deltas or a fresh snapshot.
10. Performance and scalability designs SHALL derive from NFR-001 and NFR-004, including provisional calibration values, rather than invented targets.
11. Security design SHALL include authentication boundary, authorization enforcement, input validation, diagnostic exposure, and audit boundaries.
12. Component designs SHALL identify failure triggers, error propagation, retry/degradation posture, and recovery behavior before implementation starts.

---

<a id="6-carried-forward-design-questions"></a>
# 6. Carried-Forward Design Questions <sup>[↩](#table-of-contents "Back to ToC")</sup>

The following questions are Detailed Design inputs, not Architecture blockers:

| Question | Origin | Expected Resolution Point |
|---|---|---|
| Does Package composition require a declarative composition language, or is structured data sufficient? | `engine-hla.md` §14 | HLA-PACKAGE Detailed Design |
| Should HLA-CONTRACT's embedded-vs-hosted adapter be decided now or left open through component design? | `engine-hla.md` §14 | HLA-CONTRACT Detailed Design |
| What exact client synchronization contract supports initial snapshot, delta transfer, and reconnect by last-known revision/delta number? | Project owner discussion, `engine-hla.md` §14 | HLA-CONTRACT and HLA-QUERY Detailed Design |
| What role and capability taxonomy is required for mutating operations, especially GM/campaign-owner operations? | Project owner discussion, `engine-hla.md` §14 | HLA-CONTRACT, HLA-RESOLUTION, HLA-STATE Detailed Design |
| What exact archival storage representation supports active-window scaling and implementation-neutral export? | `engine-hla.md` §14 | HLA-PERSIST Detailed Design |
| Are any newly discovered multi-actor collaboration requirements outside the current approved SRS? | Spiral Development risk | Requirements review if needed |

---

<a id="7-traceability-plan"></a>
# 7. Traceability Plan <sup>[↩](#table-of-contents "Back to ToC")</sup>

Traceability SHALL remain continuous from Requirement ID to HLA Component ID to Detailed Design section to later Test Case ID.

| Traceability Item | Current Status | Next Action |
|---|---|---|
| Requirement to HLA Component | Complete in `engine-rtm.md` v1.0 | Preserve unchanged unless change control reopens Architecture. |
| HLA Component to Detailed Design | Started by this document | Populate component-level DD sections/artifacts for all ten HLA components. |
| Detailed Design to Test Case | Not yet created | Assign test IDs during Test Planning after component DD approval. |
| Detailed Design to Implementation Unit | Not yet created | Populate only after implementation planning begins. |

The RTM's DD Artifact column remains pending until component-level Detailed Design sections are approved. This avoids overstating traceability from a phase-entry document.

---

<a id="8-initial-risk-assessment"></a>
# 8. Initial Risk Assessment <sup>[↩](#table-of-contents "Back to ToC")</sup>

The following HLA risks become explicit Detailed Design acceptance concerns:

| Risk | DD Acceptance Concern |
|---|---|
| Event Sourcing replay cost grows unbounded if snapshot cadence is wrong | HLA-CORE and HLA-STATE SHALL define replay complexity expectations, checkpoint trigger semantics, and recovery validation. |
| HLA-VALIDATE becomes a bottleneck or is bypassed | HLA-VALIDATE SHALL define mandatory invocation points, validation cost posture, and diagnostics; tests SHALL prove bypass is unavailable. |
| HLA-PACKAGE Microkernel boundary erodes | HLA-PACKAGE SHALL define Package extension points and explicitly prohibit engine-core access. |
| HLA-CONTRACT gains first-party-only shortcuts | HLA-CONTRACT SHALL enumerate all public operations and reject internal-call paths for reference clients. |
| Mutating operations validate shape but not actor capability | HLA-CONTRACT SHALL define capability checks for every mutating operation. |
| Oracle Adapter boundary is bypassed | HLA-RESOLUTION SHALL define Oracle invocation, validation, rejection, and unresolved fallback contracts. |
| Campaign Reality and Observer Knowledge logs merge for storage convenience | HLA-CORE, HLA-OBSERVER, and HLA-PERSIST SHALL preserve separate streams and ownership. |
| Archival tier design is deferred too long | HLA-PERSIST SHALL refine the active-window/archival contract before implementation planning. |
| Pattern complexity exceeds first-release needs | Component DD SHALL keep internal structures minimal while preserving required boundaries. |
| Deployment deferral produces incompatible contracts | HLA-CONTRACT SHALL keep adapter assumptions transport-neutral unless Requirements or Architecture are reopened. |

---

<a id="9-phase-gate-declaration"></a>
# 9. Phase Gate Declaration <sup>[↩](#table-of-contents "Back to ToC")</sup>

- Detailed Design phase entry authorized? **Yes.**
- Component-level Detailed Design complete? **No.**
- Interface contracts complete? **No.**
- Data schemas complete? **No.**
- Failure semantics complete? **No.**
- NFR derivation complete? **No.**
- Test alignment complete? **No.**
- RTM DD Artifact mappings approved? **No.**
- Implementation authorized? **No.**

This artifact opens and governs Detailed Design. It is not sufficient to advance to Implementation.

---

<a id="approval"></a>
# Approval <sup>[↩](#table-of-contents "Back to ToC")</sup>

Approved By:
Role:
Date:
Version Incremented:

---

End of Detailed Design Entry Draft.
