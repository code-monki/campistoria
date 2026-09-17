# HLA-OBSERVER Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-OBSERVER
Version: 1.0 (Component Baseline)
Date (YYYY-MM-DD): 2026-09-14
Author(s): CodeMonki
Status: Approved
Architecture Version Reference: `docs/architecture/engine-hla.md` v1.0 (Approved)
Requirement Version Reference: `docs/requirements/engine-srs.md` v1.0 (Approved)
Parent Design Reference: `docs/design/engine-detailed-design.md` v1.0 (Approved)
Glossary Reference: `docs/glossary.md` v0.1 (Working Glossary)

---

## Table of Contents

- [1. Design Authority Declaration](#1-design-authority-declaration)
- [2. Scope](#2-scope)
- [3. Architectural Conformance](#3-architectural-conformance)
- [4. Responsibilities](#4-responsibilities)
- [5. Internal Decomposition](#5-internal-decomposition)
- [6. Observer Identity Model](#6-observer-identity-model)
- [7. Observer Knowledge Model](#7-observer-knowledge-model)
- [8. Observer Event Model](#8-observer-event-model)
- [9. Information Source Model](#9-information-source-model)
- [10. Observation Update Semantics](#10-observation-update-semantics)
- [11. Query and POV Support](#11-query-and-pov-support)
- [12. Snapshot and Checkpoint Participation](#12-snapshot-and-checkpoint-participation)
- [13. Persistence and Asset Boundary](#13-persistence-and-asset-boundary)
- [14. Interface Contracts](#14-interface-contracts)
- [15. Failure Semantics](#15-failure-semantics)
- [16. Non-Functional Derivation](#16-non-functional-derivation)
- [17. Testing Alignment](#17-testing-alignment)
- [18. Traceability Summary](#18-traceability-summary)
- [19. Open Questions](#19-open-questions)
- [20. Phase Gate Declaration](#20-phase-gate-declaration)

---

<a id="1-design-authority-declaration"></a>
# 1. Design Authority Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Architecture phase approved? **Yes** — `engine-hla.md` v1.0, approved 2026-09-12.
- Architectural Component ID stable? **Yes** — HLA-OBSERVER.
- Parent Detailed Design phase opened? **Yes** — [Engine Detailed Design](../engine-detailed-design.md) v1.0 candidate.
- Advancement to implementation authorized? **No.**

This document refines HLA-OBSERVER only. It does not authorize implementation and does not introduce a new architectural component.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-OBSERVER — Observer Knowledge.

**Component purpose:** HLA-OBSERVER owns Observers and Observer Knowledge as authoritative state separate from Campaign Reality. It records what an observer knows, believes, suspects, remembers, has been told, has mapped, has inferred, or has misunderstood, with Information Source provenance, so the engine can preserve incomplete, false, stale, approximate, ambiguous, or private knowledge without contaminating Campaign Reality.

**Primary requirements:** FR-015, FR-016, FR-017, FR-018.

**Related requirements/NFRs:** FR-027, FR-028, FR-031, FR-032, FR-043, NFR-003, NFR-004, NFR-006.

**In scope:**

- Observer identity and lifecycle within a Campaign.
- Observer Knowledge records and event streams.
- Information Source provenance for every unit of Observer Knowledge.
- Observation update semantics that affect Observer Knowledge only.
- Separation from Campaign Reality and HLA-CORE Event history.
- Internal read surfaces for HLA-QUERY POV resolution and Presentation Models.
- Snapshot/checkpoint participation for HLA-STATE.
- Durable backing and archival retrieval boundaries with HLA-PERSIST.
- Opaque asset references inside Observer Knowledge when knowledge includes maps, images, documents, audio, or other media references.

**Out of scope:**

- Campaign Reality ownership, which belongs to HLA-CORE.
- POV Resolution and Presentation Model generation, which belong to HLA-QUERY.
- Public authorization and caller/observer mapping, which belong to HLA-CONTRACT.
- Package schema, rule, procedure, and Oracle definitions, which belong to HLA-PACKAGE and HLA-RESOLUTION.
- Binary asset payload storage, which belongs to HLA-PERSIST.
- Multiplayer identity/account/invitation flows, which remain deferred.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-OBSERVER SHALL preserve the approved HLA constraints:

- It owns Observer identities, Observer Knowledge records, Observer Knowledge Events, and Information Source provenance.
- It uses Event Sourcing as an independently keyed stream per Observer.
- It never merges Observer Knowledge Events with HLA-CORE Campaign Reality Events.
- It may reference Campaign Reality objects, but it does not inherit Campaign Reality values.
- It reads HLA-CORE only to verify or cross-reference subjects.
- It never writes to HLA-CORE.
- Observation updates only Observer Knowledge and never directly alter Campaign Reality.
- Observer Knowledge may be incomplete, approximate, stale, ambiguous, private, or incorrect relative to Campaign Reality.
- HLA-QUERY, not HLA-OBSERVER, owns final POV Resolution and Presentation Model materialization.

Any design that implements Observer Knowledge as a filtered view of Campaign Reality, stores it in the same stream as Campaign Reality, or automatically reconciles Observer Knowledge with Campaign Reality violates the approved HLA.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-OBSERVER SHALL:

- Create and maintain Observers within a Campaign.
- Record Observer Knowledge independently from Campaign Reality.
- Require an Information Source for every unit of Observer Knowledge.
- Preserve Observer Knowledge history through Observer-specific Event streams.
- Represent knowledge that may be true, false, incomplete, approximate, ambiguous, stale, inferred, or private.
- Support Observation updates that affect only the acting Observer's knowledge.
- Provide internal read surfaces for HLA-QUERY.
- Provide snapshot payloads and boundaries to HLA-STATE for campaign-level Checkpoints.
- Provide Observer Knowledge event segments to HLA-PERSIST for durable backing and archival retrieval.
- Preserve observer isolation for future multiplayer and current POV behavior.

HLA-OBSERVER SHALL NOT:

- Own Campaign Reality.
- Mutate HLA-CORE Events.
- Resolve Unresolved State.
- Invoke Oracles.
- Decide final client-presentable content.
- Treat lack of Observer Knowledge as proof that Campaign Reality is false.
- Treat Campaign Reality changes as automatic Observer Knowledge changes.
- Expose one Observer's private knowledge to another Observer except through authorized HLA-CONTRACT/HLA-QUERY behavior.

---

<a id="5-internal-decomposition"></a>
# 5. Internal Decomposition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-OBSERVER SHALL be decomposed internally:

| Internal Unit | Responsibility | Boundary |
|---|---|---|
| `ObserverRegistry` | Creates, resolves, and tracks Observer identities within a Campaign. | Does not own Campaign Reality or account identity. |
| `ObserverEventAppender` | Appends Observer Knowledge Events atomically to the Observer's stream. | Does not append HLA-CORE Events. |
| `ObserverKnowledgeFoldEngine` | Folds Observer Knowledge Events into current or historical Observer Knowledge views. | Does not perform final POV presentation. |
| `InformationSourceRegistry` | Records and resolves Information Source provenance. | Does not validate Campaign Reality truth. |
| `ObservationRecorder` | Normalizes observation updates into Observer Knowledge Events. | Does not mutate Campaign Reality. |
| `ObserverReferenceIndex` | Tracks known subjects and direct references from Observer Knowledge to Campaign Reality objects or assets. | Derived from Observer Events; not authoritative over HLA-CORE. |
| `ObserverSnapshotOriginator` | Produces and restores HLA-OBSERVER snapshot payloads for HLA-STATE. | Does not decide checkpoint cadence or restore policy. |
| `ObserverArchivePort` | Supplies Observer Event segments and snapshot material to HLA-PERSIST. | Does not choose storage mechanics. |
| `ObserverQueryPort` | Supplies Observer Knowledge reads to HLA-QUERY. | Does not decide final visibility or presentation shape. |

---

<a id="6-observer-identity-model"></a>
# 6. Observer Identity Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

An Observer is a viewpoint within a Campaign whose knowledge may differ from Campaign Reality.

Conceptual Observer:

```text
Observer {
  observerRef
  campaignRef
  observerKind
  displayName?
  subjectRef?
  createdBy
  createdAt
  status
}
```

`observerKind` MAY include:

- `character`
- `party`
- `faction`
- `playerPerspective`
- `sensorPlatform`
- `map`
- `documentCollection`
- `custom`

`subjectRef` may link the Observer to a Campaign Reality object when one exists, such as a character, faction, sensor platform, or organization. This link is referential only; the Observer's knowledge does not inherit that object's Campaign Reality State.

An Observer is a viewpoint, not a player identity. A player, account, session, or caller may be mapped to one or more Observers by HLA-CONTRACT/HLA-QUERY policy, especially in future multiplayer scope, but that mapping is distinct from Observer identity. The Observer provides the viewpoint basis; Observer Knowledge records what that viewpoint knows, believes, remembers, or otherwise holds.

---

<a id="7-observer-knowledge-model"></a>
# 7. Observer Knowledge Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Observer Knowledge is authoritative within the Observer Knowledge domain. It represents what an Observer knows, believes, suspects, remembers, has been told, has mapped, has inferred, or has misunderstood.

Conceptual Observer Knowledge unit:

```text
ObserverKnowledgeUnit {
  knowledgeRef
  observerRef
  subjectRef?
  knowledgeKind
  content
  confidence?
  validityRange?
  informationSourceRef
  sourceEventRef
}
```

`knowledgeKind` MAY include:

- `known`
- `believed`
- `suspected`
- `remembered`
- `rumor`
- `mapped`
- `inferred`
- `misunderstood`
- `ambiguous`
- `unknown`

HLA-OBSERVER SHALL NOT require Observer Knowledge to match Campaign Reality. A Campaign Reality Fact may exist while an Observer's knowledge about the same subject is absent, false, stale, incomplete, or approximate.

---

<a id="8-observer-event-model"></a>
# 8. Observer Event Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The Observer Knowledge Event stream is the authoritative history for an Observer's knowledge.

```text
ObserverEvent {
  observerEventId
  campaignRef
  observerRef
  eventType
  campaignTime?
  operationTime?
  sourceRef
  affectedKnowledgeRefs[]
  subjectRefs[]
  payload
  informationSourceRef
  provenance
  priorObserverEventRef?
}
```

Initial Observer Event categories SHALL include:

- Observer created.
- Knowledge recorded.
- Knowledge revised.
- Knowledge retired.
- Information Source recorded.
- Knowledge-source link changed.
- Observer snapshot boundary recorded.

Observer Event invariants:

- Each Event belongs to exactly one Observer stream.
- Each Knowledge mutation carries an Information Source.
- Subject references to Campaign Reality are references only; they do not copy or inherit Campaign Reality values.
- Subject references SHALL resolve to HLA-CORE Campaign Reality objects at mutation time.
- Events are foldable into current or historical Observer Knowledge views.
- Events are reconstructible independently from HLA-CORE Events.

---

<a id="9-information-source-model"></a>
# 9. Information Source Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Every unit of Observer Knowledge SHALL have an Information Source.

Conceptual Information Source:

```text
InformationSource {
  informationSourceRef
  observerRef
  sourceKind
  sourceDescription?
  sourceRefs[]
  observedAtCampaignTime?
  recordedAtOperationTime
  provenance
}
```

`sourceKind` MAY include:

- `directObservation`
- `map`
- `rumor`
- `npcStatement`
- `document`
- `sensorReading`
- `oracleResult`
- `inference`
- `memory`
- `playerNote`
- `importedKnowledge`

Information Source provenance prevents Observer Knowledge from contaminating Campaign Reality. It explains why an Observer believes or records something; it does not prove that the content is true in Campaign Reality.

---

<a id="10-observation-update-semantics"></a>
# 10. Observation Update Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Observation updates only Observer Knowledge.

Conceptual Observation request:

```text
ObservationRequest {
  observerRef
  subjectRef?
  informationSource
  content
  campaignTime?
  operationContext
}
```

Observation flow:

1. HLA-CONTRACT authorizes the caller for `observer.record.self` or `observer.record.other`.
2. HLA-OBSERVER resolves the Observer.
3. HLA-OBSERVER records or resolves the Information Source.
4. HLA-OBSERVER appends an Observer Event.
5. HLA-OBSERVER updates derived Observer Knowledge indexes.
6. HLA-CORE Campaign Reality remains unchanged.

Observation SHALL NOT:

- Create or update Campaign Reality Facts.
- Resolve Unresolved State.
- Automatically correct Observer Knowledge to match Campaign Reality.
- Record Observer Knowledge without an Information Source.
- Leak one Observer's private knowledge into another Observer's stream.

If an Observation should also establish Campaign Reality, that requires a separate Resolution path through HLA-RESOLUTION.

---

<a id="11-query-and-pov-support"></a>
# 11. Query and POV Support

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-OBSERVER supports HLA-QUERY by providing Observer Knowledge reads. HLA-QUERY owns POV Resolution and Presentation Model materialization.

Conceptual read operations:

```text
knowledgeOf(observerRef, subjectRef, campaignTime?) -> ObserverKnowledgeView
knowledgeHistory(observerRef, subjectRef?, range?) -> ObserverKnowledgeHistory
informationSourceOf(knowledgeRef) -> InformationSource
observerRevision(observerRef) -> ObserverRevision
```

HLA-OBSERVER SHALL return Observer Knowledge and Information Source provenance to authorized internal consumers. It SHALL NOT decide final client-presentable content. HLA-QUERY must combine Campaign Reality, Observer Knowledge, context, projection type, caller context, and capability/POV rules to decide what may be presented.

---

<a id="12-snapshot-and-checkpoint-participation"></a>
# 12. Snapshot and Checkpoint Participation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-STATE Checkpoints are campaign-level consistency boundaries and include Observer Knowledge. HLA-OBSERVER owns Observer Knowledge snapshot content.

Conceptual Observer snapshot payload:

```text
ObserverSnapshotPayload {
  campaignRef
  observerRef
  observerEventBoundary
  campaignTimeBoundary?
  observerSchemaVersion
  foldedKnowledgeState
  sourceIndexState
  referenceIndexState
}
```

Snapshot rules:

- HLA-OBSERVER SHALL provide snapshot payloads for Observer streams participating in a Checkpoint.
- Snapshot payloads SHALL be derived from committed Observer Events.
- Snapshot payloads SHALL identify the Observer Event boundary they summarize.
- HLA-OBSERVER SHALL be able to resume folding from a valid Observer snapshot boundary plus subsequent Observer Events.
- HLA-OBSERVER SHALL reject restore when the snapshot boundary does not match the Observer Event stream it claims to summarize.
- HLA-STATE coordinates checkpoint boundary and restore policy.
- HLA-PERSIST provides durable backing for snapshot payloads.

Restoring a Checkpoint SHALL NOT leave Campaign Reality and Observer Knowledge at different logical checkpoint boundaries.

Observer isolation applies to snapshot and restore mechanics. A checkpoint may contain multiple Observer snapshots, but restore, diagnostics, export, and future synchronization behavior must not expose one Observer's knowledge to another Observer or player except through authorized HLA-CONTRACT/HLA-QUERY behavior.

---

<a id="13-persistence-and-asset-boundary"></a>
# 13. Persistence and Asset Boundary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-OBSERVER owns Observer Knowledge Events and Observer snapshots. HLA-PERSIST owns durable backing, archival segmentation, import/export representation, and asset payload management.

HLA-OBSERVER MAY reference assets using opaque `AssetRef` values inside Observer Knowledge content or Information Sources. It SHALL NOT embed binary payloads as authoritative Observer Knowledge state.

HLA-OBSERVER SHALL carry only semantic asset reference metadata needed to identify and reason about knowledge references. Storage locators, byte layout, export packaging paths, content digests, and retrieval mechanics belong to HLA-PERSIST-owned manifests or backing metadata.

---

<a id="14-interface-contracts"></a>
# 14. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Public access occurs only through HLA-CONTRACT. HLA-OBSERVER provides internal component interfaces.

```text
createObserver(campaignRef, observerKind, subjectRef?) -> ObserverRef
recordObservation(request: ObservationRequest) -> ObservationResult
knowledgeOf(observerRef, subjectRef, campaignTime?) -> ObserverKnowledgeView
knowledgeHistory(observerRef, subjectRef?, range?) -> ObserverKnowledgeHistory
informationSourceOf(knowledgeRef) -> InformationSource
createObserverSnapshotPayload(observerRef, boundary) -> ObserverSnapshotPayload
restoreObserverSnapshotPayload(payload) -> RestoreResult
observerEventSegment(observerRef, range) -> ObserverEventSegment
```

Required HLA-CONTRACT capabilities before forwarding writes include:

- `observer.record.self`
- `observer.record.other`

Observer reads may require query capabilities and POV/caller context, as defined by HLA-CONTRACT and HLA-QUERY.

---

<a id="15-failure-semantics"></a>
# 15. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Failure Trigger | Required Behavior |
|---|---|
| Observer not found | Reject Observation or read; return diagnostic. |
| Missing Information Source | Reject Observer Knowledge mutation. |
| Information Source malformed | Reject mutation or return diagnostic; do not create knowledge without provenance. |
| Subject reference cannot be resolved | Reject Observer Knowledge mutation with diagnostics. |
| Attempt to mutate Campaign Reality through Observation | Reject or route caller to Resolution flow; do not mutate HLA-CORE. |
| Observer Event append fails | Leave prior Observer Knowledge stream unchanged. |
| Observer snapshot boundary mismatch | Reject restore; return diagnostic. |
| Archived Observer Event range unavailable | Return unavailable-range diagnostic; do not fabricate knowledge. |
| Unauthorized cross-observer access | Reject through HLA-CONTRACT/HLA-QUERY authorization and POV rules. |

---

<a id="16-non-functional-derivation"></a>
# 16. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-OBSERVER Design Response |
|---|---|
| NFR-003 Reliability | Observer Knowledge is event-sourced, snapshot-capable, and independently recoverable with HLA-STATE/HLA-PERSIST coordination. |
| NFR-004 Scalability | Observer streams can be segmented, snapshotted, and archived independently while remaining retrievable on demand. |
| NFR-005 Maintainability | Observer Knowledge stays separate from Campaign Reality, Presentation Models, persistence mechanics, and public authorization. |
| NFR-006 Auditability | Every Observer Knowledge unit requires Information Source provenance and is reconstructible from Observer Events. |

No quantitative performance or storage target is introduced here.

---

<a id="17-testing-alignment"></a>
# 17. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL include HLA-OBSERVER coverage for:

- Creating Observers independently of Campaign Reality mutation.
- Recording Observer Knowledge without altering Campaign Reality.
- Rejecting Observer Knowledge without Information Source provenance.
- Preserving false, stale, incomplete, approximate, ambiguous, or private knowledge without automatic reconciliation.
- Keeping Observer streams separate from HLA-CORE Event streams.
- Preventing cross-observer knowledge leakage.
- Folding Observer Events into current and historical Observer Knowledge views.
- Returning Information Source provenance for queried knowledge.
- Snapshot creation and restore for Observer streams.
- Checkpoint restore preserving Campaign Reality and Observer Knowledge logical boundary alignment.
- Archived Observer Event retrieval through HLA-PERSIST.
- AssetRef preservation without embedding payload bytes.

---

<a id="18-traceability-summary"></a>
# 18. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Design Coverage |
|---|---|
| FR-015 | `Observer`, `ObserverRegistry`, observer creation/query interfaces. |
| FR-016 | Separate Observer Event streams, no automatic reconciliation with Campaign Reality. |
| FR-017 | Observation flow updates Observer Knowledge only and never writes HLA-CORE. |
| FR-018 | `InformationSource`, mandatory source on every Observer Knowledge unit. |
| FR-027 | Internal read operations for HLA-QUERY. |
| FR-028 | Boundary statement that HLA-QUERY owns POV Resolution and final presentability. |
| FR-031 | Durable backing boundary through HLA-PERSIST. |
| FR-032 | Export/import participation through HLA-PERSIST-owned representation. |
| FR-043 | Archived Observer Event retrieval boundary. |
| NFR-006 | Information Source provenance and event-sourced Observer Knowledge history. |

---

<a id="19-open-questions"></a>
# 19. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

No HLA-OBSERVER-owned open questions remain at this design level.

Resolved during Detailed Design review:

- Observer Knowledge separation — HLA-OBSERVER SHALL maintain independent per-Observer Event streams and SHALL NOT implement Observer Knowledge as a filtered Campaign Reality view.
- Checkpoint participation — Observer Knowledge SHALL participate in campaign-level Checkpoints; HLA-OBSERVER owns Observer snapshot payloads while HLA-STATE coordinates checkpoint boundaries.
- Knowledge about unverified subjects — HLA-OBSERVER SHALL reject Observer Knowledge mutations whose subject reference cannot be resolved to HLA-CORE Campaign Reality. If future designs require rumors or notes about entities not yet established in Campaign Reality, those subjects must first be represented through an explicit Campaign Reality object, Unresolved State, or another approved reference mechanism rather than as dangling Observer Knowledge references.
- Observer lifecycle — Observers with knowledge history SHALL be retired/deactivated rather than deleted. Their Observer Knowledge streams remain available for provenance, replay, checkpoint restore, and historical queries. Player/account/session access to an Observer is a separate HLA-CONTRACT/HLA-QUERY policy concern and does not define Observer identity.
- Snapshot scope — HLA-STATE/HLA-OBSERVER MAY snapshot only active or changed Observer streams when doing so preserves logical checkpoint consistency. A Checkpoint need not physically duplicate every Observer stream each time, but restore SHALL preserve a coherent Campaign Reality and Observer Knowledge boundary.

---

<a id="20-phase-gate-declaration"></a>
# 20. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-OBSERVER Detailed Design is **approved at the component-design level**.

This approval does not authorize implementation by itself. It approves the HLA-OBSERVER Detailed Design baseline for continued Detailed Design work and later planning.

This design establishes Observer identity, Observer Knowledge, Information Source provenance, Observer Event streams, Observation update semantics, checkpoint participation, persistence boundaries, and HLA-QUERY support surfaces. No HLA-OBSERVER-owned open questions remain at this design level.

Implementation, testing, future multiplayer rescoping, or later component designs may reveal a need to revisit this design. Any material change SHALL be handled through the project's normal lifecycle change-control process.
