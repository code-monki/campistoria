# Software Requirements Specification (SRS)

Project Name: Campistoria Engine
Version: 1.0 (Approved)
Date (YYYY-MM-DD): 2026-09-08
Author(s): CodeMonki
Status: Approved
Project Primer Version Reference: `docs/agent/engine-primer.md` (current, as approved during Ideation)
RTM Scaffold Version: 0.1 (`docs/requirements/engine-rtm.md`, created alongside this SRS)

**Terminology note:** This project does not use a separate "Project Primer" artifact in the toolkit's generic sense. `docs/agent/engine-primer.md` and `docs/concept/engine-ideation.md` jointly serve that role and are treated as equivalent for the purposes of this declaration.

---

## Table of Contents

- [1. Requirements Authority Declaration](#1-requirements-authority-declaration)
- [2. Introduction](#2-introduction)
  - [2.1 Purpose](#21-purpose)
  - [2.2 Scope](#22-scope)
- [3. System Overview](#3-system-overview)
  - [3.1 Problem Summary](#31-problem-summary)
  - [3.2 Definitions and Terminology](#32-definitions-and-terminology)
- [4. Functional Requirements](#4-functional-requirements)
  - [4.1 Campaign Lifecycle and Identity](#41-campaign-lifecycle-and-identity)
  - [4.2 Campaign Reality](#42-campaign-reality)
  - [4.3 Campaign Time and History](#43-campaign-time-and-history)
  - [4.4 Unresolved State and Resolution](#44-unresolved-state-and-resolution)
  - [4.5 Observers and Observer Knowledge](#45-observers-and-observer-knowledge)
  - [4.6 Package Composition and Versioning](#46-package-composition-and-versioning)
  - [4.7 Query, POV Resolution, and Presentation](#47-query-pov-resolution-and-presentation)
  - [4.8 Persistence and Portability](#48-persistence-and-portability)
  - [4.9 Checkpoint, Undo, Recovery, and Retcon](#49-checkpoint-undo-recovery-and-retcon)
  - [4.10 Validation, Diagnostics, and Public Contracts](#410-validation-diagnostics-and-public-contracts)
- [5. Non-Functional Requirements (NFRs)](#5-non-functional-requirements-nfrs)
- [6. Deterministic–Probabilistic Requirements](#6-deterministicprobabilistic-requirements)
- [7. Constraints](#7-constraints)
- [8. Assumptions](#8-assumptions)
- [9. Interfaces and External Dependencies](#9-interfaces-and-external-dependencies)
- [10. Data Requirements](#10-data-requirements)
- [11. Error Handling and Edge Conditions](#11-error-handling-and-edge-conditions)
- [12. Security and Compliance Requirements](#12-security-and-compliance-requirements)
- [13. Waiting Room (Deferred Scope)](#13-waiting-room-deferred-scope)
- [14. Risk Assessment](#14-risk-assessment)
- [15. Traceability Readiness Declaration](#15-traceability-readiness-declaration)
- [16. Phase Gate Declaration](#16-phase-gate-declaration)
- [Approval](#approval)

---

<a id="1-requirements-authority-declaration"></a>
# 1. Requirements Authority Declaration <sup>[↩](#table-of-contents "Back to ToC")</sup>

- Ideation phase approved? **Yes** — Gate 1→2 reviewed against all seven mandatory exit criteria and approved by the project owner on 2026-09-07 (recorded in `docs/concept/engine-ideation.md`, "Current Lifecycle Status").
- Project Primer stable? **Yes** — `docs/agent/engine-primer.md` and `docs/concept/engine-ideation.md` are unchanged since gate approval.
- Scope boundaries defined? **Yes** — see `docs/concept/engine-ideation.md`, "Scope Baseline" and "Deferred Or Out Of Scope."
- Advancement to Requirements authorized? **Yes** — as of 2026-09-07.

---

<a id="2-introduction"></a>
# 2. Introduction <sup>[↩](#table-of-contents "Back to ToC")</sup>

## 2.1 Purpose

This document specifies the functional and non-functional requirements for the Campistoria Engine: the reusable campaign-state runtime that maintains persistent, evolving, observer-aware campaign reality for solo and related RPG play.

This SRS is intended for use by:

- Whoever performs Architecture and Detailed Design for the engine.
- Whoever implements the engine.
- Whoever authors reference clients, authoring tools, or third-party integrations against the engine's public contracts.

Lifecycle context: Ideation is complete and approved. This document is the Requirements-phase deliverable. It defines intent only; it does not select architecture, technology, or implementation structure.

## 2.2 Scope

**In scope for this document:**

- The campaign engine's core responsibilities: campaign identity and lifecycle, Campaign Reality, Campaign Time, Observers and Observer Knowledge, Resolution, Package composition, persistence, checkpoints/recovery/undo/retcon, query and Presentation Model generation, and validation/diagnostics.
- The public contracts the engine must expose so that reference clients, authoring tools, and third-party tools can be built without privileged access.

**Explicitly out of scope for this document:**

- Detailed functional requirements for reference clients, authoring tools, and sample scenario packages. These are sibling deliverables sharing this repository (see `README.md`, "What This Repository Will Contain") but are conceptually separate projects from the engine. Their own requirements should be specified in their own future SRS documents once their conceptual scope is developed to the same depth as the engine's. This document specifies only the engine capabilities and public contracts those sibling deliverables will depend on (see Section 4, "Public Contracts").
- Rendering technology, UI layout, and client presentation logic (engine/client boundary).
- Full authoring IDE capability (authoring boundary).
- AI, cloud services, multiplayer, and collaboration features (see Section 13, Waiting Room).
- Any specific programming language, storage technology, serialization format, or deployment model (Architecture-phase decisions).

Scope ambiguity beyond what is marked *(OPEN)* in this document does not exist at the time of this draft.

---

<a id="3-system-overview"></a>
# 3. System Overview <sup>[↩](#table-of-contents "Back to ToC")</sup>

## 3.1 Problem Summary

Reference: `docs/concept/engine-ideation.md`, "Problem Statement."

Existing solo-RPG tools solve isolated pieces of play (journals, maps, tables, oracles, encounter generators) but leave the player to manually track deeper campaign continuity: what is true, what is merely suspected, what has changed over time, what remains unresolved, and what each observer knows. Campistoria addresses this by maintaining an implementation-neutral, persistent, observer-aware campaign reality, while leaving game-system meaning to packages and narrative authority to the human player. Primary and secondary stakeholders are recorded in `docs/concept/engine-ideation.md`, "Stakeholders."

No solution structure is described here; see Section 4 for required capabilities.

## 3.2 Definitions and Terminology

The following terms are normative for this document and carry the meanings defined in `docs/concept/engine-ideation.md`, "Vocabulary." They are restated here for traceability convenience; the ideation document is authoritative if any divergence is found.

| Term | Meaning |
|---|---|
| Campaign | A persistent, mutable instance of play with durable identity, campaign reality, history, package composition, observer knowledge, and unresolved state. |
| Campaign Reality | The authoritative record of what is established as true in the campaign. |
| Entity | A campaign object with persistent identity. |
| Location | An entity capable of participating in spatial relationships and projections. |
| Property | A value or attribute associated with an entity, relationship, state, or event. |
| Relationship | A first-class connection between campaign objects. |
| Fact | An established assertion about Campaign Reality, with provenance and temporal validity. |
| State | The current or temporally bounded condition of a campaign object. |
| Campaign Time | The ordering and temporal frame of campaign events and state changes. |
| Event / History | A recorded occurrence or change in Campaign Reality. |
| Unresolved State | Explicitly represented campaign information not yet resolved. |
| Resolution | The process by which Unresolved State becomes established Campaign Reality. |
| Oracle | A defined process producing uncertain or randomized output that does not itself change Campaign Reality. |
| Procedure | A package-defined sequence used during play. |
| Rule | Package-defined semantics interpreting campaign information mechanically. |
| Observer | A viewpoint whose knowledge may differ from Campaign Reality. |
| Observer Knowledge | What an observer knows, believes, suspects, or has been told. |
| Information Source | The provenance of a unit of Observer Knowledge. |
| Package | A versioned, identifiable unit contributing definitions, behavior, or content to a campaign. |
| Scenario | An immutable package-defined starting configuration for campaign instantiation. |
| Projection | A purpose-specific representation of campaign information. |
| Presentation Model | Engine-produced information a client may render for a given observer/context/time/projection. |
| POV (Point of View) | The observer/context basis for determining what may be presented. |
| Checkpoint | A deliberate, recoverable campaign state. |
| Undo | Reversal of an accidental or unwanted operation; not campaign history. |
| Recovery | Restoration of valid state after technical failure; not campaign history. |
| Retcon | Human-authorized revision of established Campaign Reality. |
| Transaction | A single call to a public engine operation (e.g., a query, a Resolution, or a persistence operation), measured from call receipt to result return, exclusive of any caller-side network or transport time. |

No ambiguous terminology remains open at the time of this draft.

---

<a id="4-functional-requirements"></a>
# 4. Functional Requirements <sup>[↩](#table-of-contents "Back to ToC")</sup>

Requirements are grouped by capability area for readability. Grouping does not imply architectural structure.

## 4.1 Campaign Lifecycle and Identity

**Requirement ID:** FR-001
**Title:** Campaign Creation From Scenario
**Description:** The engine shall provide a mechanism to instantiate a new Campaign from a specified Scenario, producing a Campaign with a permanent, unique identity distinct from the Scenario's identity.
**Acceptance Criteria:** Given a valid Scenario, when a Campaign is instantiated from it, the resulting Campaign has a unique identity that does not equal the Scenario's identity, and the Scenario's own content is unchanged after instantiation.
**Priority:** High
**Dependencies:** FR-040
**Constraints:** Scenario content is immutable.
**Notes:** None.

**Requirement ID:** FR-002
**Title:** Campaign Identity Permanence
**Description:** Each Campaign shall retain a permanent, unique identity for its entire lifecycle, independent of storage location, package upgrades, or migration operations.
**Acceptance Criteria:** A Campaign's identity value is unchanged before and after any supported persistence, export/import, or package migration operation.
**Priority:** High
**Dependencies:** FR-001
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-003
**Title:** Independent Campaign Divergence
**Description:** The engine shall allow multiple Campaigns instantiated from the same Scenario to evolve independently, with no automatic propagation of state between them.
**Acceptance Criteria:** A change to Campaign Reality in one Campaign instantiated from Scenario S is not reflected in another Campaign instantiated from the same Scenario S.
**Priority:** High
**Dependencies:** FR-001
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-044
**Title:** Campaign Seed Assignment
**Description:** The engine shall assign and persist a seed value to each Campaign at creation, made available to package-defined Oracles that choose to use it as a basis for reproducible output.
**Acceptance Criteria:** A newly created Campaign has an associated seed value retrievable through the engine's public contracts; the same seed value is returned for that Campaign across save/reload and export/import cycles (FR-002, FR-031, FR-032).
**Priority:** Medium
**Dependencies:** FR-001, FR-002
**Constraints:** The engine guarantees only that a stable seed value is assigned, persisted, and exposed — not that any given Oracle's output is reproducible from it. That depends on the Oracle's own package-defined implementation, and does not apply to Oracles whose output originates from a physical/real-world source (e.g., manually recorded dice rolls or drawn cards), which are inherently non-reproducible regardless of seeding. Whether two different conformant engine implementations must produce identical sequences from the same seed (i.e., a shared PRNG algorithm as a normative interoperability contract) is a separate, unresolved question, left open for the future platform SRD rather than assumed here.
**Notes:** Raised during Requirements review (2026-09-08); resolves the Section 6 "Reproducibility expectations" open item for software-implemented Oracles.

## 4.2 Campaign Reality

**Requirement ID:** FR-004
**Title:** Entity Identity
**Description:** The engine shall assign and preserve a persistent identity for each Entity within a Campaign, distinguishing it from all other Entities regardless of property changes.
**Acceptance Criteria:** An Entity retains the same identity value across property changes, relationship changes, and state changes.
**Priority:** High
**Dependencies:** None.
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-005
**Title:** Location as Specialized Entity
**Description:** The engine shall support Locations as Entities capable of participating in spatial relationships (containment, topology, coordinate placement) distinct from non-spatial Entities.
**Acceptance Criteria:** A Location can be queried for containment and topological relationships to other Locations; a non-Location Entity is not required to support these queries.
**Priority:** High
**Dependencies:** FR-004
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-006
**Title:** Property Assignment
**Description:** The engine shall allow Properties to be associated with Entities, Relationships, State, or Events, with meaning and validation supplied by Packages.
**Acceptance Criteria:** A Property can be attached to at least Entities, Relationships, and Events; retrieving a Property returns the value most recently established for it at a given Campaign Time.
**Priority:** High
**Dependencies:** FR-004, FR-007, FR-009, FR-010
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-007
**Title:** First-Class Relationships
**Description:** The engine shall represent Relationships as first-class objects connecting two or more identified campaign objects, independent of any specific relationship category.
**Acceptance Criteria:** A Relationship can be created between two Entities without the engine requiring the relationship category to be predefined; querying a Relationship returns its participants and package-defined type.
**Priority:** High
**Dependencies:** FR-004
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-008
**Title:** Fact Recording With Provenance
**Description:** The engine shall allow Facts to be recorded as explicit propositions about Campaign Reality, each carrying provenance and temporal validity.
**Acceptance Criteria:** A recorded Fact can be retrieved together with its source (Information Source or Resolution reference) and the Campaign Time at which it became true.
**Priority:** High
**Dependencies:** FR-010, FR-013
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-009
**Title:** Temporally Bounded State
**Description:** The engine shall represent the current or temporally bounded State of a campaign object and allow that State to change as the campaign evolves.
**Acceptance Criteria:** Querying an object's State at a given Campaign Time returns the State value in effect at that time, even after later State changes have occurred.
**Priority:** High
**Dependencies:** FR-010
**Constraints:** None.
**Notes:** None.

## 4.3 Campaign Time and History

**Requirement ID:** FR-010
**Title:** Campaign Time Ordering
**Description:** The engine shall maintain a generic Campaign Time concept that orders Events and State changes independent of any package-specific calendar or time unit.
**Acceptance Criteria:** Two Events recorded at different Campaign Time values can be queried and returned in correct temporal order regardless of package-specific calendar formatting.
**Priority:** High
**Dependencies:** None.
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-011
**Title:** Event and History Recording
**Description:** The engine shall record Events representing meaningful occurrences or changes in Campaign Reality, sufficient to support reconstruction of campaign history.
**Acceptance Criteria:** Given a sequence of campaign changes, the engine can produce an ordered history of Events that reproduces the sequence of changes to Campaign Reality.
**Priority:** High
**Dependencies:** FR-010
**Constraints:** None.
**Notes:** None.

## 4.4 Unresolved State and Resolution

**Requirement ID:** FR-012
**Title:** Unresolved State Declaration
**Description:** The engine shall allow a part of Campaign Reality to be explicitly represented as unresolved, without requiring an answer to exist before it is queried.
**Acceptance Criteria:** A campaign element marked unresolved can be queried and distinguished from a resolved element without the query forcing an answer to be generated.
**Priority:** High
**Dependencies:** None.
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-013
**Title:** Resolution of Unresolved State
**Description:** The engine shall provide a mechanism by which Unresolved State becomes established Campaign Reality through a Resolution action attributable to a human decision, a package-defined procedure, a rule, or an Oracle result.
**Acceptance Criteria:** After a Resolution action is applied to an unresolved element, the element is queryable as resolved Campaign Reality, and the Resolution's source is retrievable.
**Priority:** High
**Dependencies:** FR-012
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-014
**Title:** Oracle Result Non-Authority
**Description:** The engine shall treat an Oracle result as an input that does not, by itself, alter Campaign Reality unless and until a Resolution action is applied.
**Acceptance Criteria:** Generating an Oracle result does not change any queryable Campaign Reality value unless a subsequent Resolution action references that result.
**Priority:** High
**Dependencies:** FR-013
**Constraints:** Deterministic/probabilistic separation (see Section 7 and Section 6).
**Notes:** None.

## 4.5 Observers and Observer Knowledge

**Requirement ID:** FR-015
**Title:** Observer Definition
**Description:** The engine shall allow one or more Observers to be defined within a Campaign, each capable of holding Observer Knowledge distinct from Campaign Reality.
**Acceptance Criteria:** An Observer can be created and queried independently of Campaign Reality; modifying Observer Knowledge does not alter Campaign Reality.
**Priority:** High
**Dependencies:** None.
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-016
**Title:** Observer Knowledge Independence
**Description:** The engine shall maintain Observer Knowledge as separate from Campaign Reality, permitting Observer Knowledge to be incomplete, approximate, stale, ambiguous, or incorrect relative to Campaign Reality.
**Acceptance Criteria:** A Campaign Reality Fact can exist while a given Observer's Observer Knowledge about the same subject differs from or omits it entirely, without the engine reconciling the two automatically.
**Priority:** High
**Dependencies:** FR-008, FR-015
**Constraints:** None.
**Notes:** This is a core differentiator of the engine; see risk assessment (Section 14).

**Requirement ID:** FR-017
**Title:** Observation Updates Observer Knowledge Only
**Description:** The engine shall ensure that an Observation action updates only the acting Observer's Observer Knowledge and never directly alters Campaign Reality.
**Acceptance Criteria:** After an Observation action, Campaign Reality values are unchanged; only the acting Observer's Observer Knowledge reflects the observation.
**Priority:** High
**Dependencies:** FR-015, FR-016
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-018
**Title:** Information Source Provenance
**Description:** The engine shall record an Information Source for each unit of Observer Knowledge, identifying its origin (direct observation, map, rumor, NPC statement, document, sensor reading, Oracle result, or inference).
**Acceptance Criteria:** Querying a unit of Observer Knowledge returns an associated Information Source; Observer Knowledge without a recorded source is not permitted by the engine's data model.
**Priority:** High
**Dependencies:** FR-015
**Constraints:** None.
**Notes:** None.

## 4.6 Package Composition and Versioning

**Requirement ID:** FR-019
**Title:** Package Registration
**Description:** The engine shall allow Packages to be registered with the engine, each carrying a distinct identity and version.
**Acceptance Criteria:** Two Packages with the same identity but different versions can both be registered and individually referenced.
**Priority:** High
**Dependencies:** None.
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-020
**Title:** Effective Campaign Definition Composition
**Description:** The engine shall compose a set of registered Packages, together with explicit extensions and overrides, into an Effective Campaign Definition used to instantiate a Campaign.
**Acceptance Criteria:** Given a defined set of Packages and overrides, the engine produces a single Effective Campaign Definition reflecting all declared dependencies, extensions, and overrides without silent omission.
**Priority:** High
**Dependencies:** FR-019
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-021
**Title:** Explicit Composition, No Implicit Collision Resolution
**Description:** The engine shall require Package composition conflicts to be explicitly resolved and shall not silently select one colliding definition over another.
**Acceptance Criteria:** When two Packages define colliding content without an explicit override or extension declaration, composition fails with a reported conflict rather than completing silently.
**Priority:** High
**Dependencies:** FR-020
**Constraints:** "Composition is explicit; collision is not composition" (Conceptual Constraint).
**Notes:** None.

**Requirement ID:** FR-022
**Title:** Package Version Pinning
**Description:** The engine shall pin a Campaign to the exact effective Package composition from which it was instantiated.
**Acceptance Criteria:** After a newer version of a Package used by a Campaign is registered with the engine, the existing Campaign's effective composition remains unchanged until an explicit migration is performed.
**Priority:** High
**Dependencies:** FR-020
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-023
**Title:** Explicit Package Migration
**Description:** The engine shall require an explicit migration operation to change a Campaign's Package composition and shall not apply a Package upgrade automatically.
**Acceptance Criteria:** A Package upgrade registered with the engine does not alter any existing Campaign's effective composition without an explicit migration operation being invoked for that Campaign.
**Priority:** High
**Dependencies:** FR-022
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-024
**Title:** Coexisting Package Versions
**Description:** The engine shall allow multiple versions of the same Package to be registered and referenced simultaneously by different Campaigns.
**Acceptance Criteria:** Two Campaigns may each reference a different version of the same-identity Package concurrently without the engine requiring uninstallation of either version.
**Priority:** Medium
**Dependencies:** FR-019
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-025
**Title:** Dependency-Aware Package Removal
**Description:** The engine shall prevent removal of a Package version still referenced by any current, inactive, or archived Campaign, unless the removal is explicitly forced with that consequence acknowledged.
**Acceptance Criteria:** Attempting to remove a Package version referenced by an existing Campaign is rejected with identification of the referencing Campaign(s), unless an explicit force-removal parameter is supplied.
**Priority:** Medium
**Dependencies:** FR-019, FR-022
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-026
**Title:** System/Edition Identity Independent of Package Version
**Description:** The engine shall track RPG system and edition identity as metadata independent of Package version numbering.
**Acceptance Criteria:** Two versions of the same Package may declare the same system/edition identity, and two different system/edition identities may exist within Packages sharing an unrelated version number.
**Priority:** Medium
**Dependencies:** FR-019
**Constraints:** None.
**Notes:** None.

## 4.7 Query, POV Resolution, and Presentation

**Requirement ID:** FR-027
**Title:** Query Interface
**Description:** The engine shall provide a query capability allowing a caller to retrieve Campaign Reality and, subject to POV Resolution, Observer Knowledge, filtered by observer, context, and Campaign Time.
**Acceptance Criteria:** A query specifying an Observer, context, and Campaign Time returns only information the engine determines valid to present for that combination.
**Priority:** High
**Dependencies:** FR-015, FR-028
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-028
**Title:** POV Resolution Authority
**Description:** The engine, not the client, shall determine what information is valid to present for a given Observer/context/time combination.
**Acceptance Criteria:** A client request for a Presentation Model returns only engine-approved content; the client cannot obtain Campaign Reality or Observer Knowledge fields the engine has determined invalid to present for the specified POV.
**Priority:** High
**Dependencies:** FR-016
**Constraints:** Inspectable authoritative state (Conceptual Constraint); Client Boundary.
**Notes:** None.

**Requirement ID:** FR-029
**Title:** Presentation Model Generation
**Description:** The engine shall produce Presentation Models representing campaign information for a particular Observer, context, time, and Projection type, without requiring the client to derive campaign truth independently.
**Acceptance Criteria:** A Presentation Model returned by the engine requires no additional visibility, POV, or knowledge-filtering logic from the client before rendering it.
**Priority:** High
**Dependencies:** FR-028
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-030
**Title:** Multiple Projection Types From Shared State
**Description:** The engine shall support producing Presentation Models for multiple Projection types (at minimum: map, timeline, roster, journal, relationship view) from the same underlying Campaign Reality and Observer Knowledge.
**Acceptance Criteria:** Requesting two different Projection types for the same Observer/context/time returns two distinct Presentation Models, each derived from the same underlying Campaign state.
**Priority:** Medium
**Dependencies:** FR-029
**Constraints:** None.
**Notes:** None.

## 4.8 Persistence and Portability

**Requirement ID:** FR-031
**Title:** Durable Persistence
**Description:** The engine shall persist a Campaign's Campaign Reality, Observer Knowledge, Package composition, and history such that the Campaign survives an ordinary close/reopen cycle without loss.
**Acceptance Criteria:** A Campaign saved, then closed, then reopened produces query results identical to those obtained immediately before closing.
**Priority:** High
**Dependencies:** None.
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-032
**Title:** Implementation-Neutral Import/Export
**Description:** The engine shall provide import and export operations that produce and consume a durable Campaign representation not bound to a specific storage engine or client implementation.
**Acceptance Criteria:** An exported Campaign representation can be imported to reproduce an equivalent Campaign without requiring the originating storage technology.
**Priority:** High
**Dependencies:** FR-031
**Constraints:** Portability (Conceptual Constraint).
**Notes:** None.

**Requirement ID:** FR-042
**Title:** Campaign-Local Asset Portability
**Description:** The engine's Campaign export operation shall include, or otherwise guarantee access to, any binary asset referenced by Campaign Reality or Observer Knowledge that is not already guaranteed portable through an independently exportable Package.
**Acceptance Criteria:** Given a Campaign referencing an asset added during play (not sourced from a registered Package), exporting and then importing that Campaign on a system without prior access to that asset reproduces the asset content without loss.
**Priority:** High
**Dependencies:** FR-032
**Constraints:** The export packaging mechanism (embedding, bundling, content-addressed reference, etc.) is an Architecture-phase decision; this requirement establishes only the portability guarantee, not the mechanism.
**Notes:** Raised during Requirements review (2026-09-08). Package-sourced assets already portable via their own Package export are not required to be duplicated into every Campaign export, provided the Package remains independently obtainable.

**Requirement ID:** FR-043
**Title:** On-Demand Access to Archived Campaign History
**Description:** The engine shall provide a mechanism to retrieve Campaign Reality, Observer Knowledge, or Event/History data falling outside the current active working set (e.g., archived or older campaign history), without requiring that data to be loaded for ordinary queries.
**Acceptance Criteria:** A query explicitly requesting information from outside the current active working set (e.g., an Event older than the active window) returns that information, retrieved on demand, without the engine having kept it loaded for prior ordinary queries.
**Priority:** Medium
**Dependencies:** FR-011, FR-027
**Constraints:** The archival storage/retrieval mechanism (e.g., eviction policy, indexing, compression) is an Architecture-phase decision.
**Notes:** Added during Requirements review (2026-09-08) to close a gap exposed by NFR-004: bounding the active working set implies a corresponding functional guarantee that excluded data remains reachable on demand.

## 4.9 Checkpoint, Undo, Recovery, and Retcon

**Requirement ID:** FR-033
**Title:** Checkpoint Creation and Restoration
**Description:** The engine shall allow a Checkpoint to be created representing a known recoverable Campaign state, distinct from Undo, Recovery, and Retcon operations.
**Acceptance Criteria:** A Campaign can be restored to a previously created Checkpoint, reproducing the Campaign state at the time the Checkpoint was created.
**Priority:** High
**Dependencies:** FR-031
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-034
**Title:** Undo Operation
**Description:** The engine shall provide an Undo operation that reverses an accidental or unwanted operation without being recorded as part of Campaign history.
**Acceptance Criteria:** After an Undo operation, the reversed operation does not appear in the Campaign's Event/History record as a distinct historical occurrence beyond its cancellation.
**Priority:** Medium
**Dependencies:** FR-011
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-035
**Title:** Recovery Operation
**Description:** The engine shall provide a Recovery operation restoring valid Campaign state after a technical failure, distinct from Undo and Retcon.
**Acceptance Criteria:** Following a simulated technical failure during a write operation, a Recovery operation restores the Campaign to its last valid persisted state without requiring manual data repair.
**Priority:** High
**Dependencies:** FR-031
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-036
**Title:** Retcon Operation
**Description:** The engine shall provide a Retcon operation allowing human-authorized revision of established Campaign Reality, preserving provenance of the change and identifying directly referencing campaign objects where determinable.
**Acceptance Criteria:** After a Retcon operation revises a Fact, the Campaign records that a Retcon occurred, the prior value, and a list of directly referencing campaign objects the engine could identify as affected.
**Priority:** Medium
**Dependencies:** FR-008
**Constraints:** Automatic consequence rewriting is out of scope (Section 13); only direct-reference identification is required.
**Notes:** None.

## 4.10 Validation, Diagnostics, and Public Contracts

**Requirement ID:** FR-037
**Title:** Campaign/Package Consistency Validation
**Description:** The engine shall validate that a Campaign's Package composition and content remain internally consistent, reporting violations rather than silently ignoring them.
**Acceptance Criteria:** Given a Campaign referencing a Package composition with a missing declared dependency, engine validation reports the missing dependency rather than completing without error.
**Priority:** High
**Dependencies:** FR-020
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-038
**Title:** Structured Diagnostics
**Description:** The engine shall provide structured diagnostic output for validation failures, composition conflicts, and migration issues, sufficient for a caller to identify the affected object and cause.
**Acceptance Criteria:** A diagnostic report includes, at minimum, an identifiable object/campaign reference, a category of failure, and a human-readable description of the failure.
**Priority:** Medium
**Dependencies:** FR-037
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-039
**Title:** Non-Privileged Public Contracts
**Description:** The engine shall expose the same public contracts for querying, constructing, validating, importing, and exporting Packages and Campaigns to first-party and third-party callers alike.
**Acceptance Criteria:** A first-party reference client or authoring tool and a third-party-built equivalent use the same documented engine interface, with no capability available exclusively to the first-party implementation.
**Priority:** High
**Dependencies:** FR-027, FR-032, FR-037
**Constraints:** Open interoperability (Conceptual Constraint / Engine Interoperability Requirement).
**Notes:** This requirement is the basis for the reference-client and authoring-tool sibling deliverables described in `README.md`.

**Requirement ID:** FR-040
**Title:** Scenario Immutability
**Description:** The engine shall prevent modification of a Scenario's authored content once it exists as a starting definition, treating Scenario content as immutable relative to any Campaign instantiated from it.
**Acceptance Criteria:** Attempting to modify Scenario content through the Campaign-facing interface is rejected or has no effect on the Scenario's stored definition.
**Priority:** High
**Dependencies:** None.
**Constraints:** None.
**Notes:** None.

**Requirement ID:** FR-041
**Title:** Failed Migration Rollback
**Description:** If an explicit Package migration operation fails partway, the engine shall leave the Campaign in its prior valid, pinned composition rather than a partially migrated state.
**Acceptance Criteria:** Simulating a failure during migration results in the Campaign's effective composition and Campaign Reality being unchanged from before the migration attempt began.
**Priority:** High
**Dependencies:** FR-023
**Constraints:** None.
**Notes:** Added to close an edge condition identified in Section 11.

---

<a id="5-non-functional-requirements-nfrs"></a>
# 5. Non-Functional Requirements (NFRs) <sup>[↩](#table-of-contents "Back to ToC")</sup>

Several NFRs below contain a quantitative target marked *(OPEN)*. `docs/concept/engine-ideation.md` ("Preliminary Non-Functional Signals") deliberately deferred numeric targets to Requirements or later; per the Hallucination Guardrails governing this document, no numeric target is invented here. Each *(OPEN)* item must be resolved with stakeholder input before Architecture relies on it.

**Requirement ID:** NFR-001
**Category:** Performance
**Description:** Under local, single-process invocation with no network transport involved, engine Transactions (queries, Resolution, persistence operations, and similar public operation calls) shall complete quickly enough not to disrupt the interactive solo play loop.
**Measurement Criteria:** *(Provisional, pending empirical validation)* 95% of Transactions complete within 500ms, measured from call receipt to result return, exclusive of any caller-side network or transport time. This target is a starting point for Test Planning/Implementation measurement, not yet empirically validated, and may be revised once real workloads are profiled.
**Constraints:** This target applies only to engine-local processing time. Network latency, remote-hosting overhead, and any session establishment/teardown cost introduced by a future client-server architecture are explicitly out of scope for this NFR and must be defined as separate NFRs once a deployment architecture is chosen (Architecture phase). Multi-user/concurrent-access performance is out of scope per Section 13 (Waiting Room) unless that scope is explicitly reopened.
**Dependencies:** None.
**Notes:** Multi-user and remote-deployment performance characteristics are intentionally deferred, not foreclosed. If multiplayer or networked deployment is reopened in a later phase, its own NFR(s) should be defined at that time rather than retrofitted, to avoid rework driven by an unstated single-user assumption baked into this NFR (see Section 14 risk entry).

**Requirement ID:** NFR-002
**Category:** Security
**Description:** The engine shall validate Packages, imported Campaign data, and external assets at defined trust boundaries before incorporating them into a Campaign, rather than assuming trustworthiness.
**Measurement Criteria:** Given a malformed or non-conformant Package or import file, the engine's validation step rejects or flags it prior to it affecting Campaign Reality.
**Constraints:** Specific authentication/authorization/sandboxing mechanisms are Architecture-phase decisions.
**Dependencies:** FR-037.

**Requirement ID:** NFR-003
**Category:** Reliability
**Description:** Authoritative Campaign state shall survive ordinary failures (e.g., interrupted write, application crash) without silent corruption or unrecoverable loss of provenance needed to explain the last consequential change.
**Measurement Criteria:** Following a simulated failure during a write operation, the Campaign is either fully recoverable to its last valid state or reports a specific, diagnosable failure; silent corruption is not an acceptable outcome.
**Constraints:** None.
**Dependencies:** FR-035, FR-041.

**Requirement ID:** NFR-004
**Category:** Scalability
**Description:** The engine shall bound the active working set required for ordinary campaign operation to a recent-activity window. As Campaign Time advances, data aging out of that window shall transition to archival storage while remaining retrievable on demand (FR-043), rather than requiring a Campaign's entire historical record to be loaded for ordinary use.
**Measurement Criteria:** *(Provisional, pending empirical validation)* The active window defaults to the most recent activity equivalent to approximately 30 in-game days, with the actual mapping from "day" to Campaign Time increments defined by the active Package/ruleset composition rather than by the engine itself. Ordinary queries scoped to the active window do not require loading data outside it. This default is a starting point for Architecture/Test Planning and may be revised once real workloads are profiled. *(OPEN — exact entity/event count thresholds, and the specific Package-defined increment backing "in-game day," remain to be validated.)*
**Constraints:** The specific working-set management mechanism (eviction/archival-transition policy, caching strategy, indexing) is an Architecture-phase decision; this requirement establishes only the bounding behavior and the on-demand-access guarantee (FR-043), not the mechanism. The engine's generic Campaign Time concept does not itself define "day" (Ruleset Independence, Section 7); the 30-day default is a placeholder pending Package-specific calibration.
**Dependencies:** FR-043.

**Requirement ID:** NFR-005
**Category:** Maintainability
**Description:** Engine mechanics, Package semantics, persistence, projection, validation, diagnostics, and public contracts shall remain separable such that a change to one does not require redefining the others.
**Measurement Criteria:** A defined Package semantics change (e.g., a new Package role) can be accommodated without a change to the Campaign Reality/Observer Knowledge separation or to unrelated public contracts.
**Constraints:** None.
**Dependencies:** None.

**Requirement ID:** NFR-006
**Category:** Auditability
**Description:** Consequential state changes, Package composition changes, migrations, and Retcon operations shall carry sufficient provenance to support after-the-fact inspection and explanation.
**Measurement Criteria:** For any consequential Campaign Reality change, the engine can produce a record identifying what changed, when (Campaign Time and, where applicable, real-world operation time), and through what mechanism (Resolution, migration, Retcon, or Recovery).
**Constraints:** None.
**Dependencies:** FR-008, FR-011, FR-036.

---

<a id="6-deterministicprobabilistic-requirements"></a>
# 6. Deterministic–Probabilistic Requirements <sup>[↩](#table-of-contents "Back to ToC")</sup>

The engine's Oracle and Procedure concepts introduce probabilistic/uncertain output. This section bounds that behavior at the requirement level per governance (`ai-toolkit/02-governance/00-lifecycle-bootstrap.md`, Section 6).

- **Probabilistic behavior expectations:** Oracle-generated results are the only permitted probabilistic output in the engine's meta-model. They must not directly mutate Campaign Reality (FR-014).
- **Acceptable output variability boundaries:** The engine does not bound the content of an Oracle's randomness (that is package-defined); it bounds where in the pipeline probabilistic output can influence state — only through Resolution (FR-013, FR-014).
- **Validation expectations:** Package-declared Oracle and Procedure outputs must conform to the Package's declared contract before being accepted as Resolution input (FR-037).
- **Containment expectations:** No probabilistic subsystem — Oracle, Procedure, or any future AI-assisted tool — may write directly to Campaign Reality. All writes pass through Resolution, which requires an identifiable Resolution source (FR-013).
- **Fallback requirements:** If an Oracle or Procedure fails to produce a valid result, the engine shall leave the associated campaign element Unresolved rather than defaulting or guessing silently (FR-012).
- **Observability requirements:** Every Resolution action must record which Oracle/Procedure/rule/human source produced it (FR-013, NFR-006).
- **Reproducibility expectations:** The engine shall assign and persist a per-Campaign seed value (FR-044) that software-implemented, package-defined Oracles may use as a basis for reproducible output within a single conformant implementation. This does not extend to Oracles whose randomness originates from a physical/real-world source, which cannot be reproduced by definition regardless of seeding, nor does it assume two different conformant implementations share a PRNG algorithm — that cross-implementation question remains *(OPEN)* for the future platform SRD.

Unbounded probabilistic integration (an Oracle or Procedure writing to Campaign Reality without passing through Resolution) is prohibited by this specification.

---

<a id="7-constraints"></a>
# 7. Constraints <sup>[↩](#table-of-contents "Back to ToC")</sup>

Restated and elaborated from `docs/concept/engine-ideation.md`, "Conceptual Constraints":

- **Human authority:** The engine must preserve human authority over consequential narrative interpretation, world-state changes, uncertainty resolution, exceptions, retcons, and canon.
- **Deterministic/probabilistic separation:** Deterministic campaign mechanics and authoritative state changes must remain distinguishable from probabilistic Oracle use, interpretation, or generation (see Section 6).
- **Model independence:** The engine concept must not depend on a particular AI model, provider, or AI runtime. AI is not an engine capability.
- **Ruleset independence:** The engine must not embed one RPG system, setting, genre, edition, or rules vocabulary as intrinsic engine semantics.
- **Inspectable authoritative state:** Campaign Reality, Observer Knowledge, state transitions, package composition, and provenance must be representable and inspectable independently of prose narration or client rendering.
- **Portability:** Durable campaign and package data must remain user-owned and portable outside transient client storage or a single deployment environment.
- **Lifecycle governance:** Decisions materially affecting the approved concept must preserve the distinction between settled conceptual decisions and open implementation decisions unless the project owner explicitly reopens the concept.
- **No mandated technology:** No fixed implementation language, storage technology, database, file format, UI framework, rendering technology, deployment platform, quantitative performance target, delivery schedule, or monetary budget is imposed by Ideation or by this document, except where explicitly marked *(OPEN)* for later resolution.

No regulatory, budgetary, or timeline constraints have been identified for this project at the time of this draft (see Section 8, Assumptions, for the associated unvalidated assumption).

---

<a id="8-assumptions"></a>
# 8. Assumptions <sup>[↩](#table-of-contents "Back to ToC")</sup>

- **Single-campaign, single-user focus at this phase.** The primary usage context is assumed to be one human player/GM operating one Campaign at a time per engine instance. Multiplayer and concurrent multi-actor access are explicitly deferred (Section 13). **Flagged as uncertain** — not yet confirmed against future stress-test scenarios beyond those already run during Ideation (`docs/concept/engine-ideation.md`, "Stress-Test Findings").
- **No regulated data categories.** The engine is assumed not to process data subject to specific regulatory regimes (e.g., payment data, health data, data covered by children's-privacy law) at this phase. **Flagged as uncertain** — should be revisited if scope changes.
- **No fixed budget or timeline.** Per the hobby-project context recorded for this repository, no budget or delivery-date constraint currently applies. This assumption should be revisited if the project's organizational status changes.
- **Trusted-enough Package authorship at this phase.** Structural/schema validation (NFR-002) is assumed to be the primary trust concern for Packages at this phase, rather than adversarial security review, consistent with no networked or cloud-hosted package distribution being in scope yet (Section 13).
- **A durable storage mechanism will exist.** The engine implementation is assumed to have access to some durable storage sufficient to satisfy FR-031/FR-032; the specific technology is undetermined at this phase and is an Architecture-phase decision.
- **A caller exists to invoke engine operations.** The engine is assumed to be invoked by some caller (reference client, authoring tool, or third-party integration) through its public contracts; no specific transport or protocol is assumed at this phase.

---

<a id="9-interfaces-and-external-dependencies"></a>
# 9. Interfaces and External Dependencies <sup>[↩](#table-of-contents "Back to ToC")</sup>

No architectural structure (transport, protocol, or component decomposition) is defined here. The following interface *capabilities* must exist per Section 4:

- A query interface for Campaign Reality, Observer Knowledge (via POV resolution), and Presentation Models (FR-027, FR-028, FR-029, FR-030).
- A Package construction, validation, import, and export interface, available identically to first-party and third-party callers (FR-039).
- A Campaign import/export interface producing an implementation-neutral representation (FR-032).
- A Resolution-invocation interface through which human decisions, procedures, rules, and Oracle results can be submitted (FR-013).
- A diagnostics interface returning structured validation and migration failure information (FR-038).

No hardware interfaces are anticipated. No external system integrations are in scope at this phase (Section 13).

---

<a id="10-data-requirements"></a>
# 10. Data Requirements <sup>[↩](#table-of-contents "Back to ToC")</sup>

- **Campaign Reality data:** entities, locations, properties, relationships, facts, state, unresolved state, and events, each with sufficient provenance to satisfy FR-008, FR-011, and NFR-006.
- **Observer Knowledge data:** per-observer knowledge units, each with a recorded Information Source (FR-018).
- **Package data:** package identity, version, declared dependencies/capabilities, and content, sufficient to satisfy FR-019 through FR-026.
- **Asset references:** Campaign Reality and Observer Knowledge may hold references to binary assets (images, audio, video, or other media) as opaque Property values (FR-006). The engine requires no built-in knowledge of asset formats, encoding, or rendering; it requires only that referenced assets remain portable per FR-042.
- **Retention:** compression, archiving, compaction, and deletion are distinct operations (per `docs/concept/engine-ideation.md`, "Persistence And Portability"); archived data must remain semantically available for reconstruction and search, not merely retained in raw form.
- **Privacy/compliance:** campaign data is user-owned (Section 7); no regulated personal data category is currently in scope (Section 8). This should be re-examined if scope changes.
- **Audit:** provenance for Facts, Resolutions, migrations, and Retcons must be retrievable (NFR-006).

---

<a id="11-error-handling-and-edge-conditions"></a>
# 11. Error Handling and Edge Conditions <sup>[↩](#table-of-contents "Back to ToC")</sup>

- Package composition collision without an explicit override/extension declaration is rejected, not silently resolved (FR-021).
- Removing a Package version still referenced by an existing Campaign is rejected unless explicitly forced (FR-025).
- Querying an Unresolved element returns its unresolved status; it does not force generation of an answer (FR-012).
- An invalid or malformed import file is rejected with structured diagnostics, not partially imported (FR-037, FR-038).
- An interrupted write is recoverable to the last valid state without manual repair (FR-035, NFR-003).
- A Package migration that fails partway leaves the Campaign in its prior valid, pinned composition (FR-041).
- A query for a Presentation Model with no valid content for the given POV returns an explicit empty/insufficient-information result rather than fabricated content (supports FR-028, FR-029).

---

<a id="12-security-and-compliance-requirements"></a>
# 12. Security and Compliance Requirements <sup>[↩](#table-of-contents "Back to ToC")</sup>

- **Validation at trust boundaries:** Packages, imported campaign data, and external assets must be validated before being incorporated into a Campaign (NFR-002).
- **Authorization of presentation:** the engine, not the client, determines what may be presented for a given POV (FR-028); this is the engine's core authorization mechanism at this phase.
- **Audit logging:** consequential changes must be traceable to their source (NFR-006).
- **Authentication:** not required at this phase — no multi-user or networked access is in scope (Section 8, Section 13). This is an assumption to revisit, not a settled exclusion, if multiplayer or cloud services are ever reopened.
- **Regulatory obligations:** none identified at this phase (Section 8).

---

<a id="13-waiting-room-deferred-scope"></a>
# 13. Waiting Room (Deferred Scope) <sup>[↩](#table-of-contents "Back to ToC")</sup>

Restated from `docs/concept/engine-ideation.md`, "Deferred Or Out Of Scope":

- Full authoring IDE (visual editors, map editing, schema editing, oracle/procedure editors, Git integration, asset workflows).
- AI as an engine capability, AI-specific campaign semantics, AI oracle interpretation, or AI GM function.
- Multiplayer implementation (networking, synchronization, authentication, signaling, conflict resolution, host migration).
- Cloud services (accounts, hosted campaigns, cloud sync, telemetry, subscriptions, remote backup).
- Collaboration (shared editing, co-GMs, distributed authoring, simultaneous assistants).
- Branching campaigns / alternate timelines.
- Sophisticated retcon propagation (automatic consequence rewriting beyond direct-reference identification; see FR-036).
- Generic rules inference or expert-system reasoning.
- General-purpose worldbuilding beyond campaign execution.
- Rendering technology and rich media generation/editing.
- Package marketplace, commercial ecosystem, or automatic package updates.

Also deferred from this document specifically (Section 2.2): detailed requirements for reference clients, authoring tools, and sample scenario packages, pending their own future requirements documents.

---

<a id="14-risk-assessment"></a>
# 14. Risk Assessment <sup>[↩](#table-of-contents "Back to ToC")</sup>

| Risk | Category | Level | Related Requirement(s) |
|---|---|---|---|
| Package model becomes too powerful, drifts into arbitrary executable plugins | Scope drift | High | FR-039, Section 7 |
| Observer Knowledge oversimplified/collapsed into global truth during implementation | Requirement ambiguity | High | FR-016, FR-017 |
| Persistence becomes tied to a specific implementation, weakening user ownership | Dependency | High | FR-032, Section 7 |
| Package migration silently alters campaign semantics | Scope drift | High | FR-022, FR-023, FR-041 |
| Public contracts underspecified, making third-party packages/clients second-class | Requirement ambiguity | High | FR-039 |
| Engine absorbs authoring-tool responsibilities | Scope drift | Moderate | Section 2.2, Section 13 |
| Retcon semantics become dangerously automatic | Scope drift | Moderate | FR-036 |
| Project prematurely chooses technology before quality attributes are known | Scope drift | Moderate | Section 7, NFR *(OPEN)* items |
| System expands into general worldbuilding rather than campaign execution | Scope drift | Moderate | Section 13 |
| AI features re-enter as implicit engine behavior | Scope drift | Moderate | Section 7 (Model independence) |
| Quantitative NFR targets remain undefined into Architecture | Requirement ambiguity | Moderate | NFR-001, NFR-004 |
| Probabilistic containment (Section 6) not actually enforced in implementation | Probabilistic integration | Moderate | Section 6 |
| Regulatory posture changes if scope expands (e.g., hosted/cloud data) | Regulatory | Low | Section 8, Section 12 |
| Third-party package/tooling ecosystem does not materialize | Dependency | Low | FR-039 |
| Single-user performance/design assumptions become load-bearing in Architecture, forcing rework if multiplayer/remote deployment is reopened later | Scope drift | Moderate | NFR-001 |

Risk levels are this author's judgment based on the risks already surfaced during Ideation (`docs/concept/engine-ideation.md`, "Risks And Open Questions"), not a measured or externally validated risk score.

---

<a id="15-traceability-readiness-declaration"></a>
# 15. Traceability Readiness Declaration <sup>[↩](#table-of-contents "Back to ToC")</sup>

- All requirements uniquely identified? **Yes.**
- Acceptance criteria defined? **Yes**, for all FRs and NFRs.
- No architectural decisions embedded? **Yes**, self-attested; pending human review.
- No implementation bias present? **Yes**, self-attested; pending human review.
- Waiting room separated? **Yes** (Section 13).
- Probabilistic requirements bounded? **Yes** (Section 6).
- RTM scaffold updated? **Yes** — `docs/requirements/engine-rtm.md` created alongside this draft, listing all FR/NFR IDs with Architecture/Design/Test/Packaging/Orchestration columns marked not-yet-applicable pending later phases.

---

<a id="16-phase-gate-declaration"></a>
# 16. Phase Gate Declaration <sup>[↩](#table-of-contents "Back to ToC")</sup>

- Requirements stable? **Yes** — approved by the project owner (CodeMonki) on 2026-09-08.
- NFRs measurable? **Yes** — every NFR defines an objective measurement criterion. NFR-001 and NFR-004 additionally carry provisional calibration values pending empirical data (no production workload exists yet to calibrate against); this is tracked as a Moderate risk (Section 14) and resolved through normal RTM change control as data becomes available, not treated as a gate blocker.
- Scope boundaries explicit? **Yes.**
- Traceability scaffold prepared? **Yes.**
- Human approval granted? **Yes** — see Approval section below.

All criteria read "Yes." The Requirements→Architecture gate is cleared as of 2026-09-08.

---

<a id="approval"></a>
# Approval <sup>[↩](#table-of-contents "Back to ToC")</sup>

Approved By: CodeMonki
Role: Project Owner
Date: 2026-09-08
Version Incremented: Yes (0.1 → 1.0)

Advancement to High-Level Architecture is authorized as of this approval, per lifecycle governance.

---

End of Requirements Specification (Approved v1.0)
