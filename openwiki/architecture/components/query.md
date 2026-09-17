---
type: architecture-component
title: "HLA-QUERY: Query, POV Resolution, and Presentation"
description: "How HLA-QUERY is the sole authority over what is presentable, resolving POV against Campaign Reality and Observer Knowledge and materializing Projection-Strategy Presentation Models."
tags: [architecture, hla, facade, strategy, pov-resolution, presentation]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-8ec29c60f5a2e905e0e7d239
    resource: repo://docs/design/components/hla-query-dd.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-QUERY: Query, POV Resolution, and Presentation

HLA-QUERY answers read requests against Campaign Reality and Observer Knowledge, determines what information is valid to present for a requested observer/context/time, and materializes caller-appropriate Presentation Models and synchronization deltas. It is the engine-side presentation authority: **clients render what HLA-QUERY returns rather than deriving visibility, hidden-information rules, or campaign truth themselves.**

## Why Facade + Strategy

FR-028 requires the *engine*, not the client, to decide what's presentable. A **Facade** concentrates that authority in one place rather than letting each Projection type independently reimplement POV filtering — which would risk one Projection leaking data another correctly withholds.

**Strategy** fits the Projection layer because Map, Timeline, Roster, Journal, and Relationship View projections (FR-030) share almost no algorithmic structure with each other beyond a common input/output shape — they are fully interchangeable algorithms selected by projection type, not variations on one fixed skeleton.

**Visitor** was considered and rejected for traversing Campaign Reality's object graph per projection: Visitor assumes a fixed, closed set of element types known when the Visitor is written, but Entity/Relationship/Fact types are Package-extensible and open-ended — exactly what Visitor is unsuited for. **Template Method** was also rejected for Projection generation: the projection types don't share enough of a common algorithmic skeleton to justify a fixed base-class structure with varying steps.

## Internal decomposition

HLA-QUERY is architecturally one HLA component, but its design mandates internal decomposition so responsibilities stay separately testable even if a future implementation collapses modules into fewer files:

| Module | Responsibility | Not responsible for |
|---|---|---|
| `QueryFacade` | Receives authorized requests from HLA-CONTRACT, orchestrates the flow | Public authorization or transport |
| `QueryContextNormalizer` | Validates/normalizes observer, context, time, projection type, caller/sync context | Domain truth or final presentability |
| `AuthoritativeReadCoordinator` | Reads Campaign Reality, Observer Knowledge, package metadata, archived data through owning components | Owning or mutating read data |
| `POVResolver` | Determines what candidate information is valid to present | Projection-specific layout |
| `ProjectionStrategyRegistry` | Selects projection strategies by type | Hardcoding all future package semantics |
| `PresentationMaterializer` | Builds Presentation Models from POV-approved information | Client rendering |
| `PresentationRevisionTracker` | Derives non-authoritative revision identifiers deterministically | Replacing Campaign Reality/Observer Knowledge revision streams |
| `DeltaMaterializer` | Computes caller-visible deltas/compacted deltas/unchanged/full-state fallbacks | HLA-CONTRACT envelope vocabulary |
| `ArchiveBridge` | Requests archived history through HLA-PERSIST | Archival storage mechanics |
| `ToolingInspectionViewBuilder` | Builds separately-authorized authoring/GM/diagnostic/test views | Ordinary client reads or raw store dumps |
| `QueryDiagnosticsBuilder` | Produces structured diagnostics | Suppressing security-required diagnostic detail |

## POV Resolution: truth, knowledge, and presentation are three different things

HLA-QUERY preserves a strict three-way distinction:

- **Truth** — what [HLA-CORE](core.md) records as Campaign Reality.
- **Knowledge** — what [HLA-OBSERVER](observer.md) records for the Observer.
- **Presentation** — what HLA-QUERY decides may be shown in *this* context.

HLA-QUERY may present Observer Knowledge that is false, stale, or incomplete relative to Campaign Reality when that's what the Observer validly knows — it never "corrects" Observer Knowledge merely because Campaign Reality differs. It may present Campaign Reality directly only when the requested POV and caller context make that content valid to present; it never exposes raw Campaign Reality as a shortcut around POV Resolution.

For v1, POV Resolution uses a **narrow engine-owned allow/suppress decision model**. Package-provided presentation semantics may inform those decisions, but Packages do not own arbitrary POV policy execution — a deeper package-owned rule evaluator is deferred until concrete needs demonstrate the allow/suppress model is insufficient.

## Presentation Model lifecycle

Presentation Models are **derived, caller-visible outputs — never authoritative state.** The lifecycle: HLA-CONTRACT forwards an authorized request → HLA-QUERY normalizes context → obtains source views from HLA-CORE/HLA-OBSERVER/HLA-PACKAGE/HLA-PERSIST → resolves POV → applies the requested Projection Strategy → materializes a Presentation Model or synchronization response → HLA-CONTRACT wraps the result in the public envelope.

Every Presentation Model shares a stable cross-projection envelope (`presentationRef`, `campaignRef`, `observerRef`, `contextRef`, `campaignTime`, `projectionType`, `content`, `diagnostics`, `provenanceSummary`, `sourceRevisionSet`, `presentationRevision`) — projection-specific structure lives only inside `content`; HLA-QUERY never forces map, timeline, roster, journal, and relationship-view content into one common internal schema beyond that shared envelope. HLA-QUERY may cache derived Presentation Models as an implementation optimization only if invalidated by source revisions — the cache is never treated as authoritative.

## Projection strategies

Minimum supported types from FR-030: `map`, `timeline`, `roster`, `journal`, `relationshipView`. Every strategy receives only POV-approved candidate information, never bypasses `POVResolver`, never reads HLA-CORE/HLA-OBSERVER directly outside the read coordination boundary, never persists authoritative data, and stays open to package-defined semantics without requiring HLA-QUERY to know a closed set of package entity/relationship types.

## Synchronization materialization

HLA-CONTRACT owns synchronization *envelope semantics*; **HLA-QUERY owns the actual materialization** of full state, deltas, compacted deltas, unchanged responses, and unavailable diagnostics. It uses separate revision streams for Campaign Reality (HLA-CORE), Observer Knowledge (HLA-OBSERVER), and Presentation Models (derived by HLA-QUERY itself) — never a single global Campaign delta number. Presentation Model revisions are derived **deterministically** from the source revision set, query basis, projection type, and projection algorithm/version identity; HLA-QUERY does not maintain a durable authoritative Presentation Model event stream for v1 — if exact replay of what a client previously saw is ever needed, that's a separate audit/delivery-log concern, not a reason to make Presentation Models authoritative.

## Archive bridging

Ordinary queries operate against the active working set. When a request explicitly needs data outside that window, `ArchiveBridge` requests it from HLA-PERSIST using explicit campaign/time/scope boundaries, and HLA-QUERY applies **POV Resolution to archived and active data together** — archived data is never a shortcut that bypasses POV Resolution; historical access is not a visibility loophole.

## Failure semantics

HLA-QUERY **fails closed when presentability is uncertain**: if POV Resolution can't determine whether content is presentable, it suppresses the uncertain content and returns a diagnostic rather than guessing; if required source data is unavailable, it returns an unavailable diagnostic rather than fabricating Presentation Model content; any attempt that would expose cross-observer data is rejected or suppressed with only an authorized-detail security diagnostic.

## Dependency relationships

- **Reads** [HLA-CORE](core.md) and [HLA-OBSERVER](observer.md); **never writes** to either.
- **Invoked externally only through** [HLA-CONTRACT](contract.md).
- **Bridges to** HLA-PERSIST for explicit archival queries.
- Owns **no persistent authoritative state** — Presentation Models are computed on demand, not stored.

## Non-functional derivation

- **NFR-001 (Performance):** queries are context-local/scope-local by default; Projection Strategies declare read hints; synchronization supports deltas and unchanged responses.
- **NFR-002 (Security):** HLA-QUERY fails closed on uncertain presentability and never bypasses POV Resolution or exposes raw source stores to clients.
- **NFR-003 (Reliability):** Presentation Models are derived from source revisions and recomputable after restart rather than persisted as authoritative data.
- **NFR-004 (Scalability):** ordinary queries stay within the active working set; explicit archive bridging supports older data on demand.
- **NFR-005 (Maintainability):** POV Resolution, Projection Strategies, revision mapping, archive bridging, and delta materialization are separated internally.
- **NFR-006 (Auditability):** Presentation Models carry source revision sets and authorized provenance summaries explaining what source boundaries informed a result.
