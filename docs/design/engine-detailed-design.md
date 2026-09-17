# Campistoria Engine Detailed Design

Project Name: Campistoria Engine
Version: 1.1 (Approved)
Date (YYYY-MM-DD): 2026-09-17
Author(s): CodeMonki
Status: Approved
Architecture Version Reference: `docs/architecture/engine-hla.md` v1.1 (Approved)
Requirement Version Reference: `docs/requirements/engine-srs.md` v1.1 (Approved)
RTM Version Reference: `docs/requirements/engine-rtm.md` v1.2 (Approved)
Glossary Reference: `docs/glossary.md` v0.1 (Working Glossary)

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
# 1. Design Authority Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Architecture phase approved? **Yes** — `engine-hla.md` v1.0, approved by the project owner on 2026-09-12.
- Architectural Component IDs stable? **Yes** — HLA-LIFECYCLE, HLA-CORE, HLA-RESOLUTION, HLA-OBSERVER, HLA-PACKAGE, HLA-QUERY, HLA-PERSIST, HLA-STATE, HLA-VALIDATE, and HLA-CONTRACT.
- NFR structural intent documented? **Yes** — `engine-hla.md` §8 and `engine-rtm.md` §4.
- Requirement-to-Architecture mapping complete? **Yes** — `engine-rtm.md` v1.0, Architecture Mapped.
- Advancement to Detailed Design authorized? **Yes** — `engine-hla.md` §15 authorizes Architecture to Detailed Design advancement.

Detailed Design work is therefore permitted. Implementation remains prohibited during this phase.

Terminology used by this document and component-level Detailed Design artifacts should align with `docs/glossary.md`. Approved Requirements and Architecture definitions remain authoritative for their approved phase artifacts unless changed through explicit lifecycle change control.

---

<a id="2-scope-of-this-design"></a>
# 2. Scope of This Design

<sup>[↩](#table-of-contents "Back to ToC")</sup>

This document records the approved v1.1 Detailed Design amendment for the Campistoria Engine. It preserves the approved ten-component structure while adding Campaign Authority Binding behavior discovered through Spiral Development during Test Planning.

**Architectural Component IDs in scope:** all ten approved HLA components: HLA-LIFECYCLE, HLA-CORE, HLA-RESOLUTION, HLA-OBSERVER, HLA-PACKAGE, HLA-QUERY, HLA-PERSIST, HLA-STATE, HLA-VALIDATE, and HLA-CONTRACT.

**Requirement IDs in scope:** FR-001 through FR-045 and NFR-001 through NFR-007 as proposed in `engine-srs.md` v1.1.

**Document boundaries:**

- Refines approved HLA structure into approved component-level Detailed Design artifacts.
- Captures design-wide invariants that every component-level design SHALL preserve.
- Records that component-owned Detailed Design questions have been resolved or intentionally deferred within the approved component artifacts.
- Defers complete implementation method signatures, concrete schemas, final error catalogs, state machines where not yet justified, and test IDs to follow-on artifacts and Test Planning.

**Explicit exclusions:**

- No new architectural components are introduced.
- No system boundary, HLA dependency direction, or deterministic-probabilistic boundary is changed.
- No implementation language, storage technology, UI framework, deployment platform, transport protocol, CI/CD platform, or package manager is selected.
- No code, package layout, build script, runtime configuration, or implementation unit is authorized by this document.
- No Virtual Tabletop product surface is introduced. The engine may support VTT-like clients, but map rendering UX, token manipulation UX, live table presence, chat, audio/video, visual scene composition, and client-side play-surface ergonomics remain outside engine scope unless introduced later through approved lifecycle change control.

Architectural change requires rollback to Architecture phase and RTM change control.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

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

The project uses the Spiral Development model. New risks, missed requirements, or boundary conflicts discovered during Detailed Design are expected to be handled through explicit change control, including rollback to Requirements or Architecture when needed.

---

<a id="4-detailed-design-work-breakdown"></a>
# 4. Detailed Design Work Breakdown

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The components below SHALL be refined into component-level Detailed Design sections before implementation planning. Priority reflects dependency order and risk, not delivery order.

| Priority | Component | Requirement IDs | Design Focus |
|---|---|---|---|
| 1 | HLA-CONTRACT | FR-039, FR-045; cross-cutting NFR-001, NFR-005, NFR-007 | Public operation catalog, embedded/hosted adapter neutrality, actor/session identity, Campaign Authority Binding ownership, capability enforcement, error exposure, request/response conventions. Approved amendment: `docs/design/components/hla-contract-dd.md`. |
| 2 | HLA-VALIDATE | FR-037, FR-038, NFR-002 | Validation subject taxonomy, validator chain ordering, diagnostic schema, import/Package/Oracle validation contracts, XML/SVG-style structural validation posture where applicable. Approved: `docs/design/components/hla-validate-dd.md`. |
| 3 | HLA-RESOLUTION | FR-012, FR-013, FR-014, NFR-006 | Resolution Command model, Oracle Adapter contract, acceptance/rejection criteria, deterministic fallback, undo relationship, provenance records. Approved: `docs/design/components/hla-resolution-dd.md`. |
| 4 | HLA-CORE | FR-004 through FR-011, NFR-003, NFR-004, NFR-006 | Campaign Reality event model, state fold rules, Fact/Relationship/Time invariants, atomic append semantics, history reconstruction. Approved: `docs/design/components/hla-core-dd.md`. |
| 5 | HLA-OBSERVER | FR-015 through FR-018, NFR-003, NFR-004, NFR-006 | Observer Knowledge event model, information-source provenance, isolation from Campaign Reality, POV query support. Approved: `docs/design/components/hla-observer-dd.md`. |
| 6 | HLA-PACKAGE | FR-019 through FR-026, FR-041, NFR-005 | Package registry, version pinning, composition rules, dependency removal, migration contract, conflict diagnostics. Approved: `docs/design/components/hla-package-dd.md`. |
| 7 | HLA-STATE | FR-033 through FR-036, FR-041, NFR-003 | Checkpoint schema, snapshot cadence configuration, recovery orchestration, undo inverse-command semantics, retcon provenance. Approved: `docs/design/components/hla-state-dd.md`. |
| 8 | HLA-PERSIST | FR-031, FR-032, FR-042, FR-043, FR-045, NFR-004, NFR-007 | Storage abstraction, archival boundary, import/export representation, authority-binding durability/rebinding, asset portability, active-window retrieval behavior. Approved amendment: `docs/design/components/hla-persist-dd.md`. |
| 9 | HLA-QUERY | FR-027 through FR-030 | Query contract, POV resolution, Projection strategy selection, Presentation Model lifecycle, reconnect delta/snapshot behavior. Approved: `docs/design/components/hla-query-dd.md`. |
| 10 | HLA-LIFECYCLE | FR-001, FR-002, FR-003, FR-040, FR-044, FR-045 | Scenario-to-Campaign instantiation, Campaign identity, seed assignment, initial Campaign Authority Binding orchestration, immutable Scenario reference, branching/divergence semantics. Approved amendment: `docs/design/components/hla-lifecycle-dd.md`. |

Each component-level refinement defines responsibilities, explicit interface contracts, data structures, validation rules, error semantics, preconditions, postconditions, side effects, determinism expectations, failure behavior, NFR derivation, and test alignment at the level appropriate for Detailed Design. Concrete implementation signatures, executable schemas, and test IDs remain follow-on lifecycle artifacts.

---

<a id="5-cross-component-design-rules"></a>
# 5. Cross-Component Design Rules

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The following rules are mandatory acceptance criteria for component-level Detailed Design and later Test Planning:

1. No external caller, first-party or third-party, may bypass HLA-CONTRACT.
2. Every mutating HLA-CONTRACT operation SHALL declare required capabilities before component forwarding is allowed.
3. Authorization SHALL be capability-enforced. Roles MAY assign capability bundles, but permission checks SHALL resolve to explicit capabilities rather than hardcoded role names.
4. Every Campaign SHALL have at least one active local Campaign Authority Binding. Caller-supplied role names, grants, and imported foreign Principal identifiers SHALL NOT self-authorize.
5. All Package content, imported Campaign data, and Oracle results SHALL pass HLA-VALIDATE before they can influence authoritative state.
6. HLA-RESOLUTION is the exclusive Campaign Reality write path after Campaign instantiation.
7. Campaign Reality, Observer Knowledge, Package definitions, Checkpoints, Campaign Authority Bindings, and exported artifacts SHALL have explicit ownership and lifecycle rules.
8. Campaign Reality and Observer Knowledge SHALL remain independently reconstructible from their own event streams.
9. Undo SHALL reverse an unwanted Resolution without becoming Campaign history; Retcon SHALL append provenance-preserving correction history rather than rewriting Events.
10. Client synchronization SHALL distinguish initial state transfer from subsequent delta transfer; reconnecting clients MAY present a last-known revision or delta number so the engine can choose missing deltas or a fresh snapshot.
11. Performance and scalability designs SHALL derive from NFR-001 and NFR-004, including provisional calibration values, rather than invented targets.
12. Security design SHALL include authentication boundary, Campaign-scoped authorization enforcement, input validation, diagnostic exposure, and audit boundaries.
13. Component designs SHALL identify failure triggers, error propagation, retry/degradation posture, and recovery behavior before implementation starts.
14. HLA components SHALL NOT become "God Components" that own unrelated mechanisms directly. Where a component has coordinator responsibility, component-level Detailed Design SHALL decompose it into explicit internal subcomponents, modules, or equivalent interfaces with clear ownership, while preserving the approved HLA boundary.
15. Component designs SHALL preserve the distinction between the Campistoria Engine and a Virtual Tabletop. The engine owns authoritative campaign state, observer knowledge, validation, resolution, persistence, contracts, and Presentation Models; clients own the play-surface experience used to render and manipulate those outputs.

---

<a id="6-carried-forward-design-questions"></a>
# 6. Carried-Forward Design Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The following questions were Detailed Design inputs, not Architecture blockers. Their component-level disposition is now recorded below:

| Question | Origin | Disposition |
|---|---|---|
| Does Package composition require a declarative composition language, or is structured data sufficient? | `engine-hla.md` §14 | Resolved/deferred in HLA-PACKAGE: structured package artifacts and explicit composition are required; concrete package artifact syntax is deferred to package-format SRD or implementation planning. |
| Should HLA-CONTRACT's embedded-vs-hosted adapter be decided now or left open through component design? | `engine-hla.md` §14 | Resolved in HLA-CONTRACT: contract remains transport-neutral and preserves a local/in-process path; hosted/multiplayer infrastructure remains deferred. |
| What exact client synchronization contract supports initial snapshot, delta transfer, and reconnect by last-known revision/delta number? | Project owner discussion, `engine-hla.md` §14 | Resolved across HLA-CONTRACT and HLA-QUERY: separate revision streams; HLA-CONTRACT owns envelope semantics; HLA-QUERY owns caller-visible materialization. |
| What role and capability taxonomy is required for mutating operations, especially GM/campaign-owner operations? | Project owner discussion, `engine-hla.md` §14 | Resolved in HLA-CONTRACT with capability-based authorization, provisional role bundles, and justified-split capability granularity. |
| What exact archival storage representation supports active-window scaling and implementation-neutral export? | `engine-hla.md` §14 | Resolved/deferred in HLA-PERSIST: active-window/archive boundary, retrieval contract, integrity metadata, and export guarantees are defined; concrete storage/archive technology remains deferred. |
| Are any newly discovered multi-actor collaboration requirements outside the current approved SRS? | Spiral Development risk | No new v1 requirements introduced. Multiplayer remains deferred; component docs identify rescoping impacts if reopened. |

---

<a id="7-traceability-plan"></a>
# 7. Traceability Plan

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Traceability SHALL remain continuous from Requirement ID to HLA Component ID to Detailed Design section to later Test Case ID.

| Traceability Item | Current Status | Next Action |
|---|---|---|
| Requirement to HLA Component | Complete in `engine-rtm.md` v1.0 | Preserve unchanged unless change control reopens Architecture. |
| HLA Component to Detailed Design | Complete for all ten approved HLA components | Preserve links and update only through change control. |
| Detailed Design to Test Case | Not yet created | Assign test IDs during Test Planning after component DD approval. |
| Detailed Design to Implementation Unit | Not yet created | Populate only after implementation planning begins. |

The RTM's DD Artifact column is ready to map each Requirement/NFR to the approved component-level Detailed Design artifact. Test Case IDs remain pending until Test Planning.

---

<a id="8-initial-risk-assessment"></a>
# 8. Initial Risk Assessment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The following HLA risks become explicit Detailed Design acceptance concerns:

| Risk | DD Acceptance Concern |
|---|---|
| Event Sourcing replay cost grows unbounded if snapshot cadence is wrong | HLA-CORE and HLA-STATE SHALL define replay complexity expectations, checkpoint trigger semantics, and recovery validation. |
| HLA-VALIDATE becomes a bottleneck or is bypassed | HLA-VALIDATE SHALL define mandatory invocation points, validation cost posture, and diagnostics; tests SHALL prove bypass is unavailable. |
| HLA-PACKAGE Microkernel boundary erodes | HLA-PACKAGE SHALL define Package extension points and explicitly prohibit engine-core access. |
| HLA-CONTRACT gains first-party-only shortcuts | HLA-CONTRACT SHALL enumerate all public operations and reject internal-call paths for reference clients. |
| Mutating operations validate shape but not actor capability | HLA-CONTRACT SHALL define capability checks for every mutating operation. |
| Campaign ownership is implicit or caller-asserted | HLA-CONTRACT SHALL own active Campaign Authority Bindings; HLA-LIFECYCLE SHALL create the initial binding atomically; HLA-PERSIST SHALL preserve durability and safe import rebinding. |
| Oracle Adapter boundary is bypassed | HLA-RESOLUTION SHALL define Oracle invocation, validation, rejection, and unresolved fallback contracts. |
| Campaign Reality and Observer Knowledge logs merge for storage convenience | HLA-CORE, HLA-OBSERVER, and HLA-PERSIST SHALL preserve separate streams and ownership. |
| Archival tier design is deferred too long | HLA-PERSIST SHALL refine the active-window/archival contract before implementation planning. |
| Pattern complexity exceeds first-release needs | Component DD SHALL keep internal structures minimal while preserving required boundaries. |
| Deployment deferral produces incompatible contracts | HLA-CONTRACT SHALL keep adapter assumptions transport-neutral unless Requirements or Architecture are reopened. |
| Future multiplayer event concurrency is underestimated | Multiplayer remains deferred, but if reopened, HLA-CONTRACT, HLA-CORE, HLA-STATE, and HLA-QUERY SHALL revisit actor-scoped event causality, ordering, conflict detection, authorization, synchronization, and recovery semantics for multiple players mutating Campaign Reality through concurrent or near-concurrent actions. |

---

<a id="9-phase-gate-declaration"></a>
# 9. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Detailed Design phase entry authorized? **Yes.**
- Component-level Detailed Design complete? **Pending v1.1 amendment review** — HLA-CONTRACT, HLA-LIFECYCLE, and HLA-PERSIST are amended.
- Interface contracts complete? **Yes, at Detailed Design level.**
- Data schemas complete? **Yes, conceptual Detailed Design structures defined; executable schemas deferred where explicitly stated.**
- Failure semantics complete? **Yes, at component level.**
- NFR derivation complete? **Yes, at component level.**
- Test alignment complete? **Yes, component-level test planning signals defined; Test Case IDs deferred to Test Planning.**
- RTM DD Artifact mappings approved? **Yes** — RTM v1.2 includes approved FR-045/NFR-007 mappings.
- Implementation authorized? **Yes** — controlled implementation is authorized by the approved Test Planning gate; Packaging and Release remain unauthorized.

The v1.0 baseline remains the prior approved design. The v1.1 amendment must be approved with Requirements, Architecture, RTM, and Test Planning alignment before lifecycle advancement resumes.

---

<a id="approval"></a>
# Approval

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Prior Baseline Approved By: CodeMonki
Prior Baseline Role: Project Owner
Prior Baseline Date: 2026-09-17
Prior Baseline Version: v1.0

Amendment Approved By: CodeMonki
Role: Project Owner
Date: 2026-09-17
Version Incremented: Yes — v1.1

---

End of Approved Detailed Design v1.1.
