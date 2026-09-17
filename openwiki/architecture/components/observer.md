---
type: architecture-component
title: "HLA-OBSERVER: Observer Knowledge"
description: "How HLA-OBSERVER keeps what an Observer knows, believes, or misunderstands as a structurally separate, independently-keyed Event stream from Campaign Reality."
tags: [architecture, hla, event-sourcing, observer-knowledge, pov]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-42a466507c67aaca32682dff
    resource: repo://docs/design/components/hla-observer-dd.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-OBSERVER: Observer Knowledge

HLA-OBSERVER owns Observers and Observer Knowledge as authoritative state that is structurally separate from Campaign Reality. It records what an Observer knows, believes, suspects, remembers, has been told, has mapped, has inferred, or has misunderstood — each unit carrying Information Source provenance — so the engine can preserve incomplete, false, stale, approximate, ambiguous, or private knowledge without ever contaminating Campaign Reality.

## Why a separate Event-Sourced stream, not a filtered view

The architecture reuses Event Sourcing (already justified for [HLA-CORE](core.md)) so Observer Knowledge gets the same provenance/history properties Campaign Reality gets — but keeps it an **independently-keyed stream per Observer**, never merged with Campaign Core's log. This structurally enforces FR-016 ("Observer Knowledge is separate from Campaign Reality") rather than relying on query-time filtering to fake the separation.

Two alternatives were explicitly rejected, not merely set aside:

- **Decorator or Proxy over Campaign Core** — rejected because both patterns wrap and extend the *same* underlying subject, which directly contradicts FR-016/FR-017's requirement that Observer Knowledge be a genuinely separate store that can diverge arbitrarily from Campaign Reality. Using either pattern would make the required separation cosmetic rather than structural.
- **A single shared event log with type-tagged entries** — rejected because mixing streams risks accidental cross-query leakage between Campaign Reality and Observer Knowledge, undermining the "inspectable authoritative state" constraint by making it possible to query one and get contamination from the other.

## Internal decomposition

| Internal Unit | Responsibility | Boundary |
|---|---|---|
| `ObserverRegistry` | Creates, resolves, tracks Observer identities | Not Campaign Reality or account identity |
| `ObserverEventAppender` | Appends Observer Knowledge Events atomically to an Observer's stream | Never appends HLA-CORE Events |
| `ObserverKnowledgeFoldEngine` | Folds Observer Events into current/historical Observer Knowledge views | Not final POV presentation |
| `InformationSourceRegistry` | Records and resolves Information Source provenance | Does not validate Campaign Reality truth |
| `ObservationRecorder` | Normalizes observation updates into Observer Knowledge Events | Never mutates Campaign Reality |
| `ObserverReferenceIndex` | Tracks known subjects/references from Observer Knowledge to Campaign Reality objects or assets | Derived, not authoritative over HLA-CORE |
| `ObserverSnapshotOriginator` | Produces/restores HLA-OBSERVER snapshot payloads for HLA-STATE | Not checkpoint cadence or restore policy |
| `ObserverArchivePort` | Supplies Observer Event segments/snapshot material to HLA-PERSIST | Not storage mechanics |
| `ObserverQueryPort` | Supplies Observer Knowledge reads to HLA-QUERY | Not final visibility/presentation shape |

## Observer identity vs. player identity

An **Observer** is a viewpoint within a Campaign whose knowledge may differ from Campaign Reality — it may be a `character`, `party`, `faction`, `playerPerspective`, `sensorPlatform`, `map`, `documentCollection`, or `custom` viewpoint. An Observer may carry an optional `subjectRef` linking it to a Campaign Reality object (a character, faction, sensor platform), but that link is **referential only** — the Observer's knowledge never inherits that object's Campaign Reality State. Critically, an Observer is *not* a player identity: a player, account, session, or caller may be mapped to one or more Observers by HLA-CONTRACT/HLA-QUERY policy (especially for future multiplayer), but that mapping is distinct from and does not define Observer identity itself.

## Observer Knowledge and Information Source

Each `ObserverKnowledgeUnit` (`knowledgeKind` one of `known`, `believed`, `suspected`, `remembered`, `rumor`, `mapped`, `inferred`, `misunderstood`, `ambiguous`, `unknown`) carries content, optional confidence and validity range, and a mandatory `informationSourceRef`. **HLA-OBSERVER does not require Observer Knowledge to match Campaign Reality** — a Campaign Reality Fact may exist while an Observer's knowledge about the same subject is absent, false, stale, incomplete, or approximate, and none of that is automatically reconciled.

Every unit of Observer Knowledge has an `InformationSource` (`sourceKind` one of `directObservation`, `map`, `rumor`, `npcStatement`, `document`, `sensorReading`, `oracleResult`, `inference`, `memory`, `playerNote`, `importedKnowledge`). Information Source provenance is what prevents Observer Knowledge from contaminating Campaign Reality: it explains *why* an Observer believes or records something, and explicitly does not prove the content is true in Campaign Reality.

## Observation update semantics

An `ObservationRequest` flows: (1) HLA-CONTRACT authorizes `observer.record.self` or `observer.record.other`; (2) HLA-OBSERVER resolves the Observer; (3) it records/resolves the Information Source; (4) it appends an Observer Event; (5) it updates derived indexes; (6) **HLA-CORE Campaign Reality remains unchanged.** Observation never creates or updates Campaign Reality Facts, never resolves Unresolved State, never auto-corrects Observer Knowledge to match Campaign Reality, and never records knowledge without an Information Source. If an Observation should *also* establish Campaign Reality, that requires a separate path through [HLA-RESOLUTION](resolution.md) — Observation alone cannot do it.

## Checkpoint participation

HLA-STATE Checkpoints are campaign-level consistency boundaries that include Observer Knowledge, but HLA-OBSERVER owns the Observer Knowledge snapshot content itself. Snapshot payloads are derived from committed Observer Events and identify the Event boundary they summarize; restore is rejected if the snapshot boundary doesn't match the stream it claims to summarize. Restoring a Checkpoint must never leave Campaign Reality and Observer Knowledge at different logical checkpoint boundaries. Observer isolation extends to snapshot/restore mechanics — a Checkpoint may bundle multiple Observer snapshots, but nothing in restore, diagnostics, export, or synchronization may expose one Observer's knowledge to another Observer or player except through authorized HLA-CONTRACT/HLA-QUERY behavior.

## Dependency relationships

- **Reads** [HLA-CORE](core.md) only to verify or cross-reference subjects — subject references resolve to Campaign Reality objects at mutation time but never copy or inherit their values. HLA-OBSERVER **never writes to HLA-CORE.**
- **Supplies reads to** [HLA-QUERY](query.md), which owns final POV Resolution and Presentation Model materialization — HLA-OBSERVER itself never decides what is client-presentable.
- **Cooperates with** HLA-STATE for snapshot/checkpoint participation and HLA-PERSIST for durable Observer Event backing and archival retrieval, without ceding ownership of Observer Knowledge data to either.

## Failure semantics

Failures fail closed and never fabricate knowledge: a missing or malformed Information Source rejects the mutation outright (knowledge is never created without provenance), an unresolvable subject reference rejects the mutation, an attempt to mutate Campaign Reality through Observation is rejected or routed to the Resolution flow instead, and an unavailable archived Observer Event range returns a diagnostic rather than fabricated knowledge.

## Non-functional derivation

- **NFR-003 (Reliability):** Observer Knowledge is event-sourced, snapshot-capable, and independently recoverable with HLA-STATE/HLA-PERSIST coordination.
- **NFR-004 (Scalability):** Observer streams can be segmented, snapshotted, and archived independently while remaining retrievable on demand.
- **NFR-005 (Maintainability):** Observer Knowledge stays separate from Campaign Reality, Presentation Models, persistence mechanics, and public authorization.
- **NFR-006 (Auditability):** every Observer Knowledge unit requires Information Source provenance and is reconstructible from Observer Events.
