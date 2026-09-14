# HLA-CORE Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-CORE
Version: 0.1 (Component Draft)
Date (YYYY-MM-DD): 2026-09-14
Author(s): CodeMonki
Status: Draft
Architecture Version Reference: `docs/architecture/engine-hla.md` v1.0 (Approved)
Requirement Version Reference: `docs/requirements/engine-srs.md` v1.0 (Approved)
Parent Design Reference: `docs/design/engine-detailed-design.md` v0.1 (Draft)
Glossary Reference: `docs/glossary.md` v0.1 (Working Glossary)

---

## Table of Contents

- [1. Design Authority Declaration](#1-design-authority-declaration)
- [2. Scope](#2-scope)
- [3. Architectural Conformance](#3-architectural-conformance)
- [4. Responsibilities](#4-responsibilities)
- [5. Internal Decomposition](#5-internal-decomposition)
- [6. Campaign Reality Model](#6-campaign-reality-model)
- [7. Event Model](#7-event-model)
- [8. Append and Fold Semantics](#8-append-and-fold-semantics)
- [9. Temporal Query and History](#9-temporal-query-and-history)
- [10. Asset Reference Boundary](#10-asset-reference-boundary)
- [11. Interface Contracts](#11-interface-contracts)
- [12. Snapshot and Persistence Hooks](#12-snapshot-and-persistence-hooks)
- [13. Failure Semantics](#13-failure-semantics)
- [14. Non-Functional Derivation](#14-non-functional-derivation)
- [15. Testing Alignment](#15-testing-alignment)
- [16. Traceability Summary](#16-traceability-summary)
- [17. Open Questions](#17-open-questions)
- [18. Phase Gate Declaration](#18-phase-gate-declaration)

---

<a id="1-design-authority-declaration"></a>
# 1. Design Authority Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Architecture phase approved? **Yes** — `engine-hla.md` v1.0, approved 2026-09-12.
- Architectural Component ID stable? **Yes** — HLA-CORE.
- Parent Detailed Design phase opened? **Yes** — `engine-detailed-design.md` v0.1.
- Advancement to implementation authorized? **No.**

This document refines HLA-CORE only. It does not authorize implementation and does not introduce a new architectural component.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-CORE — Campaign Reality & History.

**Component purpose:** HLA-CORE owns authoritative Campaign Reality and its reconstructible Event history. It preserves stable identities, temporal state, facts, relationships, properties, and Campaign Time while exposing append, fold, and history primitives to internal components without owning resolution, observation, package semantics, persistence, or presentation.

**Primary requirements:** FR-004, FR-005, FR-006, FR-007, FR-008, FR-009, FR-010, FR-011.

**Related NFRs:** NFR-003, NFR-004, NFR-006.

**In scope:**

- Persistent identity for Campaign Reality objects.
- Location representation as a specialized Entity.
- Property attachment to Entities, Relationships, State, and Events.
- First-class Relationships connecting identified campaign objects.
- Fact recording with provenance and temporal validity.
- Temporally bounded State of campaign objects.
- Generic Campaign Time ordering.
- Event recording sufficient to reconstruct Campaign Reality history.
- Atomic append semantics for resolved Campaign Reality changes.
- State fold rules that derive current and historical Campaign Reality views from Events.
- Internal hooks for snapshotting, durable persistence, and archival retrieval without making HLA-CORE own those mechanisms.
- Semantic asset references as opaque authoritative values, without owning asset payload storage.

**Out of scope:**

- Public caller authorization, which is owned by HLA-CONTRACT.
- Package validation, import validation, and Oracle result validation, which are owned by HLA-VALIDATE.
- Unresolved State declaration and Resolution Command construction, which are owned by HLA-RESOLUTION.
- Observer Knowledge, POV isolation, and Information Source tracking, which are owned by HLA-OBSERVER.
- Package schema, rule language, procedure language, and package composition semantics, which are owned by HLA-PACKAGE.
- Presentation Models, caller-visible projections, and reconnect materialization, which are owned by HLA-QUERY and HLA-CONTRACT.
- Checkpoint cadence, Undo execution, Retcon orchestration, and Recovery orchestration, which are owned by HLA-STATE.
- Durable storage technology, export packaging, archival storage, and asset payload management, which are owned by HLA-PERSIST.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CORE SHALL preserve the approved HLA constraints:

- It owns Campaign Reality: Entities, Locations, Properties, Relationships, Facts, State, Events, and Campaign Time.
- It uses Event Sourcing: the Event stream is the authoritative source for reconstructing Campaign Reality history.
- It exposes no public caller surface; all external access remains through HLA-CONTRACT.
- It accepts ordinary post-instantiation Campaign Reality writes only through HLA-RESOLUTION.
- It does not validate Packages, imported Campaign data, or Oracle results except through outcomes already admitted by the owning components.
- It does not persist data by bypassing HLA-PERSIST.
- It does not merge Campaign Reality with Observer Knowledge.
- It does not own Presentation Models or caller-visible projection policy.

Any design that treats a mutable current-state store as the source of truth, permits Package or Oracle output to mutate Campaign Reality directly, or embeds persistence mechanics into Campaign Reality violates the approved HLA.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CORE SHALL:

- Assign and preserve stable identities for Campaign Reality objects.
- Maintain authoritative Events in Campaign Time order.
- Apply accepted Events atomically.
- Derive current and historical Campaign Reality State by folding Events.
- Preserve Fact provenance and temporal validity.
- Preserve Relationship identity and participant references independently of relationship category.
- Represent Locations as Entities with spatial relationship capability.
- Provide history ranges and state-at-time views to internal consumers.
- Carry semantic asset references as opaque values when Properties, Facts, or Events reference assets.
- Provide snapshot-originator data to HLA-STATE without owning checkpoint policy.
- Provide durable event segments to HLA-PERSIST without selecting a storage mechanism.

HLA-CORE SHALL NOT:

- Authorize callers.
- Decide whether an unresolved condition should be resolved.
- Invoke Oracles or accept probabilistic output directly.
- Interpret Package-specific rules beyond storing package-defined types as opaque semantic values.
- Store binary asset payloads.
- Own Observer Knowledge or player-specific visibility.
- Own reconnect delta selection or Presentation Model revision streams.
- Rewrite committed Event history to perform Undo or Retcon.

---

<a id="5-internal-decomposition"></a>
# 5. Internal Decomposition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CORE SHALL be decomposed internally to avoid becoming a God Component:

| Internal Unit | Responsibility | Boundary |
|---|---|---|
| `CampaignObjectRegistry` | Assigns and resolves stable Campaign Reality object identities. | Does not own state values or persistence. |
| `EventAppender` | Accepts admitted Events, enforces append preconditions, and commits Events atomically. | Does not construct Resolution Commands or authorize writers. |
| `RealityFoldEngine` | Folds ordered Events into current or historical State views. | Does not own Presentation Models or Observer filtering. |
| `FactIndex` | Maintains derived lookup structures for Facts, provenance references, and temporal validity ranges. | Derived from Events; never a separate source of truth. |
| `RelationshipIndex` | Maintains derived lookup structures for Relationship participants and package-defined relationship types. | Derived from Events; does not define relationship semantics. |
| `CampaignTimeIndex` | Orders Events by Campaign Time and supports temporal range lookup. | Does not define package-specific calendars or units. |
| `HistoryReader` | Returns Event history ranges and reconstruction inputs. | Does not expose public APIs directly. |
| `SnapshotOriginator` | Produces and consumes component-owned snapshot payloads for HLA-STATE. | Does not decide checkpoint cadence or recovery policy. |
| `AssetReferenceRegistry` | Tracks semantic `AssetRef` values present in Campaign Reality. | Does not store asset bytes or storage locators. |

These units may be implemented as modules, classes, services, or equivalent language-native constructs. The design constraint is separation of ownership, not a prescribed implementation pattern.

---

<a id="6-campaign-reality-model"></a>
# 6. Campaign Reality Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CORE's conceptual model is intentionally ruleset-independent.

```text
CampaignObjectRef {
  campaignId
  objectId
  objectKind
}
```

`objectKind` SHALL include at least:

- `entity`
- `location`
- `relationship`
- `fact`
- `event`
- `state`

A Location is an Entity with spatial relationship capability. The engine SHALL NOT hardcode a single spatial model; containment, topology, coordinate placement, or other spatial semantics are package-defined relationship/property semantics stored through the generic Campaign Reality model.

```text
Entity {
  entityRef
  entityType?
  createdByEventRef
}

Location {
  entityRef
  spatialCapability
  createdByEventRef
}

Relationship {
  relationshipRef
  relationshipType
  participantRefs[]
  validTimeRange?
  createdByEventRef
}

PropertyAssignment {
  targetRef
  propertyType
  propertyValue
  validTimeRange?
  sourceEventRef
}

Fact {
  factRef
  proposition
  subjectRefs[]
  validTimeRange
  provenanceRef
  sourceEventRef
}

StateValue {
  targetRef
  stateType
  stateValue
  validTimeRange
  sourceEventRef
}
```

Package-defined values such as `entityType`, `relationshipType`, `propertyType`, `stateType`, and proposition shape are semantic values admitted through HLA-PACKAGE and HLA-VALIDATE. HLA-CORE stores them without owning their domain meaning.

---

<a id="7-event-model"></a>
# 7. Event Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The Event stream is the authoritative Campaign Reality history.

```text
CoreEvent {
  eventId
  campaignRef
  eventType
  campaignTime
  operationTime?
  sourceRef
  causationRef?
  correlationRef?
  commandRef?
  affectedRefs[]
  payload
  provenance
  priorEventRef?
}
```

**Required Event invariants:**

- `eventId` SHALL be stable within the Campaign.
- `campaignRef` SHALL bind the Event to exactly one Campaign.
- `campaignTime` SHALL order the Event within Campaign Time.
- `sourceRef` SHALL identify the internal source that caused the Event, such as a Resolution Record, lifecycle instantiation action, migration, Retcon, or Recovery action.
- `affectedRefs` SHALL identify Campaign Reality objects affected by the Event.
- `payload` SHALL contain only Campaign Reality data needed to fold the Event.
- `provenance` SHALL be sufficient to satisfy FR-008, FR-011, and NFR-006.
- `priorEventRef` SHOULD be present when needed to support integrity checks over the ordered stream.

Initial Event categories SHALL include:

- Object declaration.
- Location declaration.
- Property assignment or change.
- Relationship declaration or change.
- Fact assertion, update, or retirement.
- State transition.
- Campaign Time advancement.
- Asset reference association.
- Resolution-applied change.
- Lifecycle or migration-applied change.
- Retcon-applied correction.

The exact Event type catalog and payload schema remain Detailed Design work. This section defines the minimum shape and invariants HLA-CORE must preserve.

---

<a id="8-append-and-fold-semantics"></a>
# 8. Append and Fold Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

This document uses **fold** as defined in `docs/glossary.md`: deterministic reconstruction of state by applying an ordered sequence of committed Events to an initial or snapshot state until the requested Event or Campaign Time boundary is reached. The Event stream remains authoritative; any folded current-state or historical-state view is derived from Events and may be rebuilt from them.

**Append semantics:**

- HLA-CORE SHALL append Events atomically.
- A failed append SHALL leave the previous authoritative Event stream unchanged.
- HLA-CORE SHALL reject Events that reference a different Campaign, duplicate an existing Event identity, violate required ordering constraints, or fail component-owned structural invariants.
- HLA-CORE SHALL NOT perform Package validation during append; package-defined semantic validity must already have been established before the write reaches HLA-CORE.
- Ordinary play mutations after Campaign instantiation SHALL enter HLA-CORE through HLA-RESOLUTION.

**Fold semantics:**

- Current Campaign Reality is a fold over the Event stream through the latest committed Event.
- Historical Campaign Reality is a fold over the Event stream through a requested Campaign Time or Event boundary.
- Derived indexes MAY accelerate folds, but indexes SHALL be reconstructible from Events.
- A cached or materialized current-state view SHALL NOT become authoritative over the Event stream.
- Folding SHALL preserve temporal validity for Facts, Properties, Relationships, and State.

**Undo and Retcon boundary:**

- Undo SHALL NOT rewrite HLA-CORE Event history.
- Retcon SHALL be represented as provenance-preserving correction history, not mutation of prior Events.
- The exact HLA-STATE mechanism for Undo, Retcon, and Recovery is deferred to the HLA-STATE Detailed Design. HLA-CORE's obligation is to provide append, fold, and history primitives that can support that mechanism without violating Event Sourcing.

---

<a id="9-temporal-query-and-history"></a>
# 9. Temporal Query and History

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CORE SHALL support internal query primitives sufficient for HLA-QUERY, HLA-STATE, HLA-PERSIST, and HLA-RESOLUTION.

Required internal read behaviors:

- Resolve a Campaign Reality object by stable identity.
- Return the State of a campaign object at a Campaign Time.
- Return the Property value in effect at a Campaign Time.
- Return Relationships involving a campaign object at a Campaign Time.
- Return Facts active at a Campaign Time with provenance.
- Return Events in Campaign Time order for a requested range.
- Return reconstruction inputs from a snapshot boundary plus subsequent Events.

HLA-CORE SHALL NOT decide caller visibility. POV filtering and Presentation Model materialization are HLA-QUERY/HLA-OBSERVER responsibilities.

HLA-CORE SHALL NOT decide archival fetch policy. When a requested Event range crosses the active working set boundary, HLA-CORE may request the necessary segment through an internal persistence port, but HLA-PERSIST owns archival retrieval mechanics.

---

<a id="10-asset-reference-boundary"></a>
# 10. Asset Reference Boundary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Campaign Reality may reference assets as opaque semantic values. HLA-CORE SHALL carry only the reference metadata required for authoritative state to remain meaningful without storage mechanics.

```text
AssetRef {
  assetId
  assetOrigin
  ownerScope
  assetTypeHint?
  portabilityClass
  provenanceRef?
}
```

HLA-CORE MAY store `AssetRef` values inside Properties, Facts, State, or Events when those references are part of Campaign Reality.

HLA-CORE SHALL NOT store:

- Binary asset payloads.
- Storage paths, object-store keys, or URLs.
- Export packaging paths.
- Compression details.
- Content digests unless a later cross-component design explicitly makes the digest a semantic Campaign Reality value rather than storage metadata.

Missing or unavailable asset payloads are diagnosed by HLA-PERSIST or presented by HLA-QUERY, but they do not by themselves authorize mutation or deletion of Campaign Reality references.

---

<a id="11-interface-contracts"></a>
# 11. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CORE exposes internal component interfaces only.

## 11.1 Write Interface

```text
appendEvent(event: CoreEvent) -> AppendResult
```

**Preconditions:**

- Caller is an approved internal write path.
- Event is structurally valid for HLA-CORE.
- Referenced Campaign exists.
- Referenced Campaign Reality object identities are resolvable or are being declared by the Event.

**Postconditions on success:**

- Event is committed atomically.
- Derived indexes are updated or marked for rebuild.
- New Campaign Reality revision is available to downstream components.
- Persistence hook is eligible to durably record the Event.

**Postconditions on failure:**

- No partial Event is committed.
- Prior Campaign Reality remains authoritative.
- Diagnostic result identifies the failing invariant without exposing storage internals.

## 11.2 Read Interface

```text
stateAt(targetRef, campaignTime) -> StateView
propertiesAt(targetRef, campaignTime) -> PropertyView
relationshipsAt(targetRef, campaignTime) -> RelationshipView
factsAt(scope, campaignTime) -> FactView
history(range) -> EventRange
reconstructionInputs(snapshotRef?, range) -> ReconstructionSet
```

Read interfaces SHALL return Campaign Reality data only. Caller-specific visibility, diagnostic shaping, and response envelope formation remain outside HLA-CORE.

## 11.3 Snapshot Interface

```text
createSnapshotPayload(boundary) -> CoreSnapshotPayload
restoreFromSnapshotPayload(payload) -> RestoreResult
```

HLA-CORE supplies component-owned snapshot payloads to HLA-STATE. HLA-STATE owns checkpoint cadence, checkpoint identity, recovery orchestration, and restore policy.

## 11.4 Persistence Interface

```text
eventSegment(range) -> CoreEventSegment
loadArchivedSegment(segmentRef) -> CoreEventSegment
```

HLA-PERSIST owns durable representation, archival segmentation, and retrieval mechanics. HLA-CORE consumes restored Event segments as reconstruction inputs and continues treating the Event stream as authoritative.

---

<a id="12-snapshot-and-persistence-hooks"></a>
# 12. Snapshot and Persistence Hooks

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CORE SHALL cooperate with HLA-STATE and HLA-PERSIST without absorbing their responsibilities.

**Snapshot posture:**

- Snapshot payloads SHALL be derivable from committed Events.
- Snapshot payloads SHALL include enough component-owned state to avoid replaying the full Event stream during Recovery.
- Snapshot payloads SHALL identify the Event boundary they summarize.
- Snapshot payloads SHALL NOT be treated as more authoritative than committed Events.

**Persistence posture:**

- HLA-CORE SHALL expose Event segments and reconstruction inputs to HLA-PERSIST.
- HLA-CORE SHALL accept validated imported Campaign Reality Events only through the import flow defined by HLA-PERSIST, HLA-VALIDATE, and HLA-LIFECYCLE.
- HLA-CORE SHALL NOT select storage technology, archival format, compression, or indexing engine.

---

<a id="13-failure-semantics"></a>
# 13. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Failure Trigger | Required Behavior |
|---|---|
| Duplicate object or Event identity | Reject append; leave prior Event stream unchanged. |
| Invalid Campaign reference | Reject append or read; return diagnostic. |
| Unresolvable object reference | Reject append unless the Event is declaring that object; return diagnostic. |
| Campaign Time ordering violation | Reject append; return ordering diagnostic. |
| Structural Event invariant failure | Reject append; return invariant diagnostic. |
| Package-defined semantic conflict | Do not decide locally; conflict must be resolved before write reaches HLA-CORE or through HLA-PACKAGE/HLA-RESOLUTION rules. |
| Persistence failure after append request | Append outcome SHALL remain atomic; no partially authoritative Event may exist. Exact write-ahead/durable-commit sequencing is an implementation design item constrained by NFR-003. |
| Missing archived segment during read | Return unavailable-range diagnostic; do not fabricate state. |
| Corrupted snapshot payload | Reject snapshot restore and require Event replay or Recovery diagnostics through HLA-STATE. |

---

<a id="14-non-functional-derivation"></a>
# 14. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-CORE Design Response |
|---|---|
| NFR-003 Reliability | Atomic append, Event stream authority, replayable fold semantics, snapshot boundaries, and no mutable-current-state authority reduce silent corruption risk. |
| NFR-004 Scalability | Event segmentation, snapshot reconstruction inputs, derived indexes, and active-window awareness support bounded ordinary operation while allowing archival retrieval. |
| NFR-006 Auditability | Event provenance, source references, affected references, Campaign Time ordering, and immutable history provide after-the-fact explanation of consequential Campaign Reality changes. |

HLA-CORE SHALL NOT invent new performance, storage-size, replay-duration, or indexing targets. Quantitative targets must come from approved Requirements, Architecture, Test Planning, or later measured implementation evidence.

---

<a id="15-testing-alignment"></a>
# 15. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL include HLA-CORE coverage for:

- Entity identity stability across Property, Relationship, and State changes.
- Location behavior as a specialized Entity without hardcoded package-specific spatial semantics.
- Property retrieval at current and historical Campaign Times.
- Relationship identity, participant lookup, and temporal validity.
- Fact retrieval with provenance and validity interval.
- State-at-time reconstruction after later State changes.
- Event ordering by Campaign Time.
- History reconstruction from Events.
- Atomic append success and failure cases.
- Derived index rebuild from Events.
- Snapshot payload creation and replay boundary behavior.
- AssetRef preservation without asset payload embedding.
- Archived range miss diagnostics.

---

<a id="16-traceability-summary"></a>
# 16. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Design Coverage |
|---|---|
| FR-004 | `CampaignObjectRegistry`, `CampaignObjectRef`, Entity identity invariants. |
| FR-005 | Location as specialized Entity with package-defined spatial semantics. |
| FR-006 | `PropertyAssignment`, temporal property fold behavior. |
| FR-007 | `Relationship`, `RelationshipIndex`, participant references. |
| FR-008 | `Fact`, provenance references, Event provenance. |
| FR-009 | `StateValue`, state-at-time fold behavior. |
| FR-010 | `CampaignTimeIndex`, Event Campaign Time ordering. |
| FR-011 | Event Sourcing model, `HistoryReader`, reconstruction inputs. |
| NFR-003 | Atomic append, replayable Events, snapshot cooperation. |
| NFR-004 | Event segmentation, active-window awareness, archival retrieval hook. |
| NFR-006 | Provenance-bearing Events and immutable history. |

---

<a id="17-open-questions"></a>
# 17. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

No HLA-CORE-owned open questions remain at this draft level. Remaining package-schema, migration, and semantic-reference details are tracked in `docs/design/components/hla-package-dd.md`.

Resolved during Detailed Design review:

- Core Event catalog granularity — HLA-CORE SHALL keep the initial Event catalog at category level for v1. A narrower Core Event type SHALL be added only when the distinction changes HLA-CORE-owned behavior: identity, ordering, provenance, fold semantics, temporal validity, recovery, or auditability. Differences that are only package meaning, game meaning, or display meaning belong in package-defined metadata or projection logic. Implementation and testing evidence may refine vague boundaries later through normal lifecycle change control.
- Undo/Retcon write path — HLA-STATE SHALL NOT receive an independent ordinary write contract into HLA-CORE for Undo or Retcon. Undo and Retcon mutations SHALL flow through HLA-CONTRACT authorization, HLA-STATE eligibility/policy evaluation, HLA-RESOLUTION mutation normalization and provenance-bearing command application, and then HLA-CORE append. This preserves HLA-RESOLUTION as the exclusive post-instantiation Campaign Reality write path while ensuring Undo/Retcon intent is authorized rather than self-authorizing. HLA-STATE owns Undo/Retcon intent, eligibility, policy, checkpoint context, and recovery context; HLA-RESOLUTION owns how accepted state-management intent becomes an authoritative Campaign Reality mutation. Technical Recovery that restores valid engine state without representing a semantic Campaign Reality mutation remains deferred to HLA-STATE Detailed Design.
- Snapshot boundary ownership — HLA-CORE SHALL define only snapshot boundary invariants, not the final snapshot/checkpoint payload shape. A Core snapshot boundary SHALL identify the Campaign, committed Event boundary, Campaign Time boundary, and Core schema/model version; any snapshot payload using that boundary SHALL be derived from committed Events; HLA-CORE SHALL be able to resume folding from a valid snapshot boundary plus subsequent Events; and HLA-CORE SHALL reject restoration when the snapshot boundary does not match the Campaign/Event stream it claims to summarize. HLA-STATE owns checkpoint/snapshot payload shape, checkpoint cadence, restore orchestration, Recovery semantics, and lifecycle policy. HLA-PERSIST owns durable representation of snapshot/checkpoint artifacts. Serialization, compression, archival layout, and validation extensions remain outside HLA-CORE.
- Derived index posture — HLA-CORE SHALL require the minimum derived indexes needed for v1 semantic behavior and ordinary internal reads: object identity lookup; Campaign Time/Event ordering lookup; current State/Property lookup; Relationship participant lookup; and Fact/provenance lookup. These indexes are derived from Events or snapshot-plus-Events and SHALL NOT become authoritative state. Specialized temporal interval indexes, asset reverse-reference indexes, advanced graph/path/topology indexes, full-text/search indexes, package-specific optimized indexes, and other measured-performance indexes are deferred candidates. They should be captured for implementation and test-planning consideration, but added only when development evidence, workload profiling, or package needs justify them.
- Package-defined type metadata retention — HLA-CORE Events SHALL retain stable `PackageSemanticRef` values when package-defined types, schemas, or contracts give meaning to Event payloads. HLA-PACKAGE owns interpretation of those references against the Campaign's composition pin and Effective Campaign Definition. HLA-CORE SHALL NOT duplicate full Package definitions into every Event by default.

---

<a id="18-phase-gate-declaration"></a>
# 18. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CORE Detailed Design is **not yet approved for implementation**.

This draft establishes Campaign Reality ownership, Event Sourcing semantics, append/fold behavior, asset-reference boundaries, and cross-component interfaces. Advancement requires review of the open questions above, especially the HLA-STATE interaction model for Undo, Retcon, Recovery, and snapshot payloads.
