---
type: architecture-component
title: "HLA-CORE: Campaign Reality and History"
description: "How HLA-CORE owns authoritative Campaign Reality as an Event-Sourced log at the center of the engine, with append/fold semantics and no outward dependencies."
tags: [architecture, hla, event-sourcing, campaign-reality, core]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-a085a2d5f2cd5e287023c54b
    resource: repo://docs/design/components/hla-core-dd.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-CORE: Campaign Reality and History

HLA-CORE owns authoritative **Campaign Reality** — Entities, Locations, Properties, Relationships, Facts, State, and Campaign Time — and the Event history sufficient to reconstruct it. It is the center of the engine's hexagonal architecture: it has no outward dependencies on any other HLA component, and every other component that touches Campaign Reality does so through HLA-CORE's internal interfaces rather than owning any of that data itself.

## Why Event Sourcing

The architecture applies Event Sourcing because every functional requirement in HLA-CORE's group either directly requires historical reconstruction (FR-011), temporally-bounded state queries (FR-009), or Fact provenance/temporal validity (FR-008). Event Sourcing satisfies all three by construction: "state at time T" is a fold over the event log up to T, and "history" is the log itself rather than a separately maintained artifact that can drift out of sync with current state.

The rejected alternative was a mutable CRUD store with a bolted-on audit log. That was rejected because maintaining current state and a separate audit trail as two artifacts risks them drifting inconsistent under partial failure — directly threatening NFR-003 (Reliability) and NFR-006 (Auditability). Event Sourcing makes the log the single source of truth, structurally preventing that drift rather than relying on discipline to prevent it.

## Internal decomposition

To avoid becoming a God Component, HLA-CORE is decomposed into named internal units, each with an explicit boundary:

| Internal Unit | Responsibility | Does not own |
|---|---|---|
| `CampaignObjectRegistry` | Assigns/resolves stable object identities | State values or persistence |
| `EventAppender` | Accepts admitted Events, enforces append preconditions, commits atomically | Resolution Command construction or write authorization |
| `RealityFoldEngine` | Folds ordered Events into current/historical State views | Presentation Models or Observer filtering |
| `FactIndex` | Derived Fact/provenance/temporal-validity lookups | Source of truth (derived from Events) |
| `RelationshipIndex` | Derived Relationship participant lookups | Relationship semantics |
| `CampaignTimeIndex` | Orders Events by Campaign Time, supports range lookup | Package-specific calendars/units |
| `HistoryReader` | Returns Event history ranges and reconstruction inputs | Public API exposure |
| `SnapshotOriginator` | Produces/consumes component-owned snapshot payloads for HLA-STATE | Checkpoint cadence or recovery policy |
| `AssetReferenceRegistry` | Tracks semantic `AssetRef` values | Asset bytes or storage locators |

## Campaign Reality model

The model is intentionally ruleset-independent. A `CampaignObjectRef` (`campaignId`, `objectId`, `objectKind`) covers at least `entity`, `location`, `relationship`, `fact`, `event`, and `state`. A **Location is an Entity with spatial relationship capability** — the engine does not hardcode a single spatial model; containment, topology, coordinate placement, and other spatial semantics are package-defined relationship/property semantics stored through this same generic model. Package-defined values such as `entityType`, `relationshipType`, `propertyType`, and `stateType` are admitted through HLA-PACKAGE and HLA-VALIDATE; HLA-CORE stores them as opaque semantic values without owning their domain meaning.

## Event model, append, and fold

Every `CoreEvent` carries a stable `eventId`, a `campaignRef` binding it to exactly one Campaign, a `campaignTime` for ordering, a `sourceRef` identifying the internal cause (a Resolution Record, lifecycle instantiation, migration, Retcon, or Recovery action), `affectedRefs`, a `payload` scoped to what's needed to fold the Event, and `provenance` sufficient for FR-008/FR-011/NFR-006.

Key rules:

- Events are appended **atomically**; a failed append leaves the prior Event stream unchanged.
- HLA-CORE rejects Events that reference a different Campaign, duplicate an existing Event identity, or violate ordering/structural invariants — but it performs no Package validation itself; semantic validity must already be established before the write reaches HLA-CORE.
- **Ordinary play mutations after Campaign instantiation enter HLA-CORE only through HLA-RESOLUTION.** No other component writes to Campaign Reality directly.
- Current Campaign Reality is a fold over the Event stream through the latest committed Event; historical Campaign Reality is a fold through a requested Campaign Time or Event boundary. Derived indexes may accelerate folds but must remain reconstructible from Events — a cached/materialized current-state view never becomes authoritative over the Event stream.
- **Undo does not rewrite committed Event history**, and **Retcon is represented as a provenance-preserving correction, not a mutation of prior Events** — preserving Event Sourcing's append-only guarantee even for corrective operations owned by HLA-STATE.

## Interfaces

HLA-CORE exposes internal component interfaces only (no public caller surface — all external access goes through HLA-CONTRACT):

- **Write:** `appendEvent(event) -> AppendResult`. On success, the Event is committed atomically, derived indexes are updated, and a new Campaign Reality revision becomes available downstream. On failure, no partial Event is committed and prior Campaign Reality remains authoritative.
- **Read:** `stateAt`, `propertiesAt`, `relationshipsAt`, `factsAt`, `history(range)`, `reconstructionInputs(snapshotRef?, range)` — these return Campaign Reality data only; caller-specific visibility and response shaping stay outside HLA-CORE.
- **Snapshot:** `createSnapshotPayload(boundary)` / `restoreFromSnapshotPayload(payload)` — HLA-CORE supplies component-owned snapshot payloads; HLA-STATE owns checkpoint cadence, identity, and restore policy.
- **Persistence:** `eventSegment(range)` / `loadArchivedSegment(segmentRef)` — HLA-PERSIST owns durable representation, archival segmentation, and retrieval mechanics; HLA-CORE only consumes restored segments as reconstruction inputs.

## Asset references

Campaign Reality may reference assets as opaque semantic values (`AssetRef`: `assetId`, `assetOrigin`, `ownerScope`, `assetTypeHint?`, `portabilityClass`, `provenanceRef?`) embedded in Properties, Facts, State, or Events. HLA-CORE explicitly does not store binary payloads, storage paths, object-store keys, URLs, export packaging paths, or compression details — that remains HLA-PERSIST's concern.

## Dependency relationships

- **Written to exclusively by** [HLA-RESOLUTION](resolution.md) for ordinary post-instantiation mutations.
- **Read by** [HLA-QUERY](query.md) (Campaign Reality queries) and [HLA-OBSERVER](observer.md) (cross-referencing subjects only — Observer Knowledge is never merged into Campaign Reality).
- **Cooperates with** [HLA-STATE](state.md) as the Memento *originator* for Checkpoints, and with [HLA-PERSIST](persist.md) for durable Event segment storage and archival retrieval, without ceding ownership of Campaign Reality data to either.

## Non-functional derivation

- **NFR-003 (Reliability):** atomic append, Event stream authority, replayable fold semantics, and snapshot boundaries reduce silent corruption risk — there is no mutable-current-state authority to drift out of sync.
- **NFR-004 (Scalability):** Event segmentation, snapshot reconstruction inputs, derived indexes, and active-window awareness bound ordinary operation while still allowing on-demand archival retrieval.
- **NFR-006 (Auditability):** Event provenance, source references, affected references, and Campaign Time ordering provide after-the-fact explanation of every consequential Campaign Reality change.

HLA-CORE does not invent new performance, storage-size, replay-duration, or indexing targets — quantitative targets must come from approved Requirements, Architecture, Test Planning, or later measured implementation evidence.
