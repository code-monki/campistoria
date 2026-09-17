# HLA-QUERY Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-QUERY
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
- [6. Query Context Model](#6-query-context-model)
- [7. POV Resolution Model](#7-pov-resolution-model)
- [8. Presentation Model Lifecycle](#8-presentation-model-lifecycle)
- [9. Projection Strategy Model](#9-projection-strategy-model)
- [10. Synchronization Materialization](#10-synchronization-materialization)
- [11. Archive Bridging](#11-archive-bridging)
- [12. Asset Presentation Boundary](#12-asset-presentation-boundary)
- [13. Interface Contracts](#13-interface-contracts)
- [14. Failure Semantics](#14-failure-semantics)
- [15. Non-Functional Derivation](#15-non-functional-derivation)
- [16. Testing Alignment](#16-testing-alignment)
- [17. Traceability Summary](#17-traceability-summary)
- [18. Open Questions](#18-open-questions)
- [19. Phase Gate Declaration](#19-phase-gate-declaration)

---

<a id="1-design-authority-declaration"></a>
# 1. Design Authority Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Architecture phase approved? **Yes** — `engine-hla.md` v1.0, approved 2026-09-12.
- Architectural Component ID stable? **Yes** — HLA-QUERY.
- Parent Detailed Design phase opened? **Yes** — [Engine Detailed Design](../engine-detailed-design.md) v1.0 candidate.
- Advancement to implementation authorized? **No.**

This document refines HLA-QUERY only. It does not authorize implementation, introduce a new architectural component, or introduce a Virtual Tabletop product surface.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-QUERY — Query, POV Resolution & Presentation.

**Component purpose:** HLA-QUERY answers read requests against Campaign Reality and Observer Knowledge, determines what information is valid to present for a requested observer/context/time, and materializes caller-appropriate Presentation Models and synchronization deltas. It is the engine-side presentation authority; clients render what HLA-QUERY returns rather than deriving visibility, hidden-information rules, or campaign truth themselves.

**Primary requirements:** FR-027, FR-028, FR-029, FR-030.

**Related requirements/NFRs:** FR-011, FR-015, FR-016, FR-017, FR-018, FR-031, FR-043, NFR-001, NFR-002, NFR-003, NFR-004, NFR-005, NFR-006.

**In scope:**

- Query context handling for observer, campaign context, Campaign Time, projection type, and caller/session context received through HLA-CONTRACT.
- POV Resolution authority for determining presentable information.
- Presentation Model materialization.
- Projection strategy selection for map, timeline, roster, journal, relationship view, and future package-defined projection inputs where supported by the engine meta-model.
- Caller-visible full-state, delta, compacted-delta, unchanged, and unavailable response materialization under HLA-CONTRACT synchronization envelope semantics.
- Revision mapping across Campaign Reality, Observer Knowledge, and Presentation Model streams.
- Explicit archival retrieval requests and archive bridging through HLA-PERSIST.
- Diagnostic behavior for unavailable, unauthorized, stale, malformed, or unsatisfiable queries.
- Authorized authoring, GM-facing, diagnostic, and inspection read support where the caller has tooling-specific capability.

**Out of scope:**

- Owning Campaign Reality, which belongs to HLA-CORE.
- Owning Observer Knowledge, Observer identity, or Information Source provenance, which belong to HLA-OBSERVER.
- Public authorization envelope semantics, capability checks, principal/session identity, and response-mode vocabulary, which belong to HLA-CONTRACT.
- Durable storage, archival segmentation, import/export representation, and asset payload retrieval mechanics, which belong to HLA-PERSIST.
- Package artifact schema and package-defined semantic contracts, which belong to HLA-PACKAGE.
- Client rendering, token manipulation UX, live table presence, chat, audio/video, and other Virtual Tabletop play-surface behavior.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-QUERY SHALL preserve the approved HLA constraints:

- It reads from HLA-CORE and HLA-OBSERVER.
- It never writes to HLA-CORE or HLA-OBSERVER.
- It is invoked externally only through HLA-CONTRACT.
- It owns no persistent authoritative state.
- It computes Presentation Models from authoritative state and SHALL NOT persist Presentation Models as authoritative state.
- It holds sole component authority over what content is valid to present for a given observer/context/time/projection request.
- It SHALL NOT require clients to apply additional visibility, hidden-information, POV, knowledge-filtering, or campaign-truth derivation before rendering a returned Presentation Model.
- It SHALL NOT merge Campaign Reality and Observer Knowledge streams for convenience.
- It SHALL NOT become a Virtual Tabletop; rendering and play-surface ergonomics remain client responsibilities.
- Ordinary client reads SHALL be expressed as Presentation Model queries. Tooling and authoring inspection reads, where exposed, SHALL use explicitly authorized tooling operations and SHALL NOT become raw HLA-CORE or HLA-OBSERVER dumps.

Any design that lets a client independently derive hidden-information rules, lets a projection bypass POV Resolution, persists Presentation Models as authoritative state, or exposes raw Campaign Reality/Observer Knowledge contrary to POV Resolution violates the approved HLA.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-QUERY SHALL:

- Accept authorized internal query requests from HLA-CONTRACT.
- Normalize query context before reading authoritative state.
- Resolve the requested POV using Campaign Reality, Observer Knowledge, context, Campaign Time, projection type, and caller/session context.
- Materialize Presentation Models that are sufficient for client rendering without client-side campaign-truth derivation.
- Provide controlled inspection views for authoring tools, GM-facing tools, diagnostics, and test tooling when separately authorized by HLA-CONTRACT.
- Select the appropriate Projection Strategy for the requested projection type.
- Maintain non-authoritative Presentation Model revision metadata sufficient for synchronization.
- Materialize caller-visible full states, deltas, compacted deltas, unchanged responses, or unavailable diagnostics under HLA-CONTRACT envelope semantics.
- Bridge explicit archived-history queries to HLA-PERSIST when requested data falls outside the active working set.
- Produce diagnostics when a query cannot be fulfilled safely.

HLA-QUERY SHALL NOT:

- Mutate Campaign Reality or Observer Knowledge.
- Create, retire, or modify Observers.
- Create Information Sources.
- Authorize callers or assign capabilities.
- Persist Presentation Models as authoritative state.
- Treat tooling inspection views as ordinary client presentation shortcuts.
- Expose one Observer's knowledge to another Observer or caller except through authorized HLA-CONTRACT/HLA-QUERY behavior.
- Own asset payload storage or decide export portability.
- Implement client UI, map rendering UX, token behavior, chat, presence, or other VTT product features.

---

<a id="5-internal-decomposition"></a>
# 5. Internal Decomposition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-QUERY is architecturally one HLA component, but its Detailed Design SHALL decompose implementation responsibilities into explicit internal modules or equivalent interfaces.

Conceptual internal modules:

| Module | Responsibility | Not Responsible For |
|---|---|---|
| `QueryFacade` | Receives authorized query requests from HLA-CONTRACT and orchestrates internal query flow. | Public authorization, capability assignment, or transport. |
| `QueryContextNormalizer` | Validates and normalizes observer, context, Campaign Time, projection type, caller context, and synchronization context. | Domain truth or final presentability. |
| `AuthoritativeReadCoordinator` | Reads required Campaign Reality, Observer Knowledge, package semantic metadata, and archived data through owning components. | Owning or mutating read data. |
| `POVResolver` | Determines what candidate information is valid to present. | Projection-specific layout or rendering. |
| `ProjectionStrategyRegistry` | Selects projection strategies by projection type. | Hardcoding all future package-specific semantics into HLA-QUERY. |
| `PresentationMaterializer` | Builds Presentation Models from POV-approved candidate information. | Client rendering or authoritative state ownership. |
| `PresentationRevisionTracker` | Deterministically derives non-authoritative revision identifiers for Presentation Model synchronization from source revisions, query basis, projection type, and projection algorithm identity. | Replacing Campaign Reality or Observer Knowledge revision streams. |
| `DeltaMaterializer` | Computes caller-visible deltas, compacted deltas, unchanged responses, or full-state fallbacks. | HLA-CONTRACT envelope vocabulary. |
| `ArchiveBridge` | Requests archived history through HLA-PERSIST for explicit archival queries. | Archival storage mechanics. |
| `ToolingInspectionViewBuilder` | Builds separately authorized authoring, GM-facing, diagnostic, or test inspection views. | Serving ordinary client reads or exposing raw authoritative-store dumps. |
| `QueryDiagnosticsBuilder` | Produces structured diagnostics for rejected, degraded, unavailable, or partially satisfiable queries. | Suppressing security-sensitive details required by HLA-CONTRACT diagnostic policy. |

This decomposition is mandatory as a design rule even if a future implementation collapses modules into fewer files or classes. The responsibilities SHALL remain separately testable.

---

<a id="6-query-context-model"></a>
# 6. Query Context Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-QUERY operates on an engine-internal query request forwarded by HLA-CONTRACT after authorization and envelope validation.

Conceptual query request:

```text
QueryRequest {
  campaignRef
  observerRef
  contextRef?
  campaignTime?
  projectionType
  queryScope?
  callerContext
  synchronizationContext?
  diagnosticPreference?
}
```

**`campaignRef`** identifies the Campaign being queried.

**`observerRef`** identifies the viewpoint basis. HLA-QUERY SHALL NOT treat `observerRef` as a player identity; player/account/session mapping remains HLA-CONTRACT policy.

**`contextRef`** identifies the local scope for the query, such as current scene, location, object, encounter, relationship neighborhood, journal scope, or another package-defined context. HLA-QUERY SHALL prefer context-local and scope-local reads over campaign-wide scans.

**`campaignTime`** identifies the requested temporal boundary. If omitted, the current Campaign Time is implied.

**`projectionType`** selects the Presentation Model shape requested.

**`queryScope`** constrains breadth, such as active window, archived range, object subset, relationship depth, or projection-specific filters.

**`callerContext`** carries principal/actor/session/capability context needed to enforce caller-appropriate presentability after HLA-CONTRACT authorization.

**`synchronizationContext`** carries last-known Campaign Reality, Observer Knowledge, Presentation Model revision, or stream-scoped delta number supplied by the caller.

HLA-QUERY SHALL reject a query if required `campaignRef`, `observerRef`, or `projectionType` cannot be resolved or if the request cannot be normalized without ambiguity.

---

<a id="7-pov-resolution-model"></a>
# 7. POV Resolution Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

POV Resolution determines what information is valid to present for a specific observer/context/time/projection request.

POV includes more than line of sight. It may include:

- Observer Knowledge.
- Campaign Reality that is public, directly observable, or otherwise authorized for the requested POV.
- Perception and prior exploration.
- Maps, documents, sensors, communications, rumors, and records.
- Stale, incomplete, false, approximate, ambiguous, or inferred knowledge.
- Package-defined visibility, knowledge, or presentation semantics.
- Caller/session capabilities that affect diagnostic or administrative visibility.

POV Resolution inputs:

```text
POVResolutionInput {
  queryRequest
  campaignRealityView
  observerKnowledgeView
  packagePresentationSemantics?
  callerContext
}
```

POV Resolution output:

```text
POVResolutionResult {
  presentableItems[]
  suppressedItemRefs[]
  provenanceRefs[]
  diagnosticRefs[]
  resolutionBasis
}
```

HLA-QUERY SHALL preserve the distinction between:

- **Truth:** what HLA-CORE records as Campaign Reality.
- **Knowledge:** what HLA-OBSERVER records for the Observer.
- **Presentation:** what HLA-QUERY decides may be shown in this context.

HLA-QUERY MAY present Observer Knowledge that is false, stale, or incomplete relative to Campaign Reality when that is what the Observer validly knows. HLA-QUERY SHALL NOT "correct" Observer Knowledge merely because Campaign Reality differs.

HLA-QUERY MAY present Campaign Reality directly only when the requested POV and caller context make that content valid to present. It SHALL NOT expose raw Campaign Reality as a shortcut around POV Resolution.

For v1 Detailed Design, POV Resolution SHALL use a narrow engine-owned allow/suppress decision model. Package-provided presentation semantics may inform those decisions, but Packages SHALL NOT own arbitrary POV policy execution. Declarative package-supplied POV policy or a deeper engine-owned rule evaluator may be introduced later only if concrete projection or package needs demonstrate that the allow/suppress model is insufficient and the change is approved through lifecycle change control.

---

<a id="8-presentation-model-lifecycle"></a>
# 8. Presentation Model Lifecycle

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Presentation Models are derived, caller-visible outputs. They are not authoritative state.

Conceptual Presentation Model:

```text
PresentationModel {
  presentationRef
  campaignRef
  observerRef
  contextRef?
  campaignTime
  projectionType
  content
  provenanceSummary?
  diagnostics[]
  sourceRevisionSet
  presentationRevision
}
```

**`presentationRef`** identifies this materialized response for diagnostics, caching, or synchronization. It is not an authoritative campaign object.

**`content`** is projection-specific and SHALL contain only POV-approved information.

**`provenanceSummary`** may identify source classes or authorized source references needed for inspectability. It SHALL NOT leak hidden Campaign Reality or cross-observer Observer Knowledge.

**`sourceRevisionSet`** records the Campaign Reality and Observer Knowledge revision boundaries used to create the Presentation Model.

**`presentationRevision`** is a derived synchronization marker for this presentation context. It SHALL be scoped to the Campaign/Observer/context/projection/time basis and SHALL NOT be interpreted as a single global Campaign counter.

The stable cross-projection envelope consists of `presentationRef`, `campaignRef`, `observerRef`, `contextRef`, `campaignTime`, `projectionType`, `content`, `diagnostics`, `provenanceSummary`, `sourceRevisionSet`, and `presentationRevision`. Projection-specific structure begins inside `content`; HLA-QUERY SHALL NOT force map, timeline, roster, journal, and relationship-view content into one common internal schema beyond the shared envelope.

Lifecycle:

1. HLA-CONTRACT forwards an authorized query request.
2. HLA-QUERY normalizes query context.
3. HLA-QUERY obtains required source views from HLA-CORE, HLA-OBSERVER, HLA-PACKAGE, and HLA-PERSIST where applicable.
4. HLA-QUERY resolves POV.
5. HLA-QUERY applies the requested Projection Strategy.
6. HLA-QUERY materializes a Presentation Model or synchronization response.
7. HLA-CONTRACT wraps the result in the public response envelope.

HLA-QUERY MAY cache derived Presentation Models or intermediate projection data as an implementation optimization only if the cache is invalidated by source revisions and is never treated as authoritative state. Opaque cache tokens MAY exist as implementation details, but they are not the semantic synchronization contract.

---

<a id="9-projection-strategy-model"></a>
# 9. Projection Strategy Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-QUERY uses Projection Strategies because projection types vary in output structure and algorithmic needs.

Minimum supported projection types from FR-030:

- `map`
- `timeline`
- `roster`
- `journal`
- `relationshipView`

Conceptual Projection Strategy contract:

```text
ProjectionStrategy {
  supports(projectionType, packageSemantics?) -> boolean
  requiredReadHints(queryRequest) -> ReadHint[]
  materialize(povResolutionResult, queryRequest) -> ProjectionContent
}
```

Projection Strategy invariants:

- Strategies SHALL receive only POV-approved candidate information.
- Strategies SHALL NOT bypass `POVResolver`.
- Strategies SHALL NOT read HLA-CORE or HLA-OBSERVER directly unless mediated by the HLA-QUERY read coordination boundary.
- Strategies SHALL NOT persist authoritative data.
- Strategies SHALL be open to package-defined semantics without requiring HLA-QUERY to know a closed set of package entity or relationship types.
- Strategies SHALL produce renderable data, not client UI behavior.

Projection examples:

| Projection Type | Expected Focus | Explicit Non-Goal |
|---|---|---|
| `map` | Spatially relevant, POV-approved locations, regions, routes, markers, asset refs, or uncertain/known map features. | Token manipulation UX or map renderer implementation. |
| `timeline` | Temporal ordering of POV-approved Events, Facts, knowledge changes, or campaign summaries. | Rewriting history or resolving Retcon consequences. |
| `roster` | POV-approved entities, groups, statuses, affiliations, and known/unknown attributes. | Deciding Campaign Reality truth. |
| `journal` | POV-approved narrative records, notes, facts, knowledge, sources, and diagnostics. | Becoming a full authoring editor. |
| `relationshipView` | POV-approved relationships, graph neighborhoods, confidence, and provenance summaries. | General-purpose graph database exposure. |

---

<a id="10-synchronization-materialization"></a>
# 10. Synchronization Materialization

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CONTRACT owns synchronization envelope semantics and response-mode vocabulary. HLA-QUERY owns caller-visible materialization of full state, deltas, compacted deltas, unchanged responses, and unavailable diagnostics for query/presentation operations.

HLA-QUERY SHALL use separate revision streams for:

- Campaign Reality revisions owned by HLA-CORE.
- Observer Knowledge revisions owned by HLA-OBSERVER.
- Presentation Model revisions derived by HLA-QUERY.

HLA-QUERY SHALL NOT create a single global Campaign delta number.

Presentation Model revisions SHALL be derived deterministically from the source revision set, query basis, projection type, and projection algorithm/version identity. HLA-QUERY SHALL NOT maintain a durable authoritative Presentation Model event stream for v1. If future requirements need exact replay of what a specific client previously saw, that need SHALL be modeled as a separate audit or delivery-log concern rather than by making Presentation Models authoritative state.

Conceptual synchronization input:

```text
PresentationSyncRequest {
  queryRequest
  lastCampaignRevision?
  lastObserverRevision?
  lastPresentationRevision?
  deltaNumber?
  acceptsFullState
  acceptsDelta
}
```

Conceptual synchronization output:

```text
PresentationSyncResult {
  stateTransferMode
  presentationModel?
  presentationDelta?
  compactedDelta?
  currentRevisionSet
  returnedDeltaRange?
  diagnostics[]
}
```

Materialization rules:

- If the caller provides no usable prior state, HLA-QUERY SHALL return a full Presentation Model when the query is valid.
- If the caller provides usable prior revision markers and deltas can be computed safely, HLA-QUERY MAY return a delta.
- If individual deltas are too numerous or no longer individually available but a safe summary can be computed, HLA-QUERY MAY return a compacted delta.
- If the derived Presentation Model would not change from the caller's prior state, HLA-QUERY MAY return `unchanged`.
- If the requested prior revision is unknown, from a different Campaign/Observer/context/projection/time basis, too old for the active/archival window, or unsafe to satisfy, HLA-QUERY SHALL return a diagnostic or full state according to HLA-CONTRACT `acceptsFullState` semantics.

Presentation deltas are caller-visible deltas. They SHALL NOT expose suppressed source data or require the client to run POV filtering.

---

<a id="11-archive-bridging"></a>
# 11. Archive Bridging

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Ordinary queries SHOULD operate against the active working set and the query's local scope. When a request explicitly asks for historical data outside the active working set, HLA-QUERY SHALL bridge to HLA-PERSIST archival retrieval rather than requiring all historical data to be loaded for ordinary operation.

Archive bridging flow:

1. `QueryContextNormalizer` identifies that requested Campaign Time, event range, object history, or Observer Knowledge history crosses the active window.
2. `ArchiveBridge` requests the required archived data from HLA-PERSIST using explicit campaign/time/scope boundaries.
3. HLA-PERSIST returns archived Campaign Reality, Observer Knowledge, or Event/History data without transferring storage mechanics to HLA-QUERY.
4. HLA-QUERY applies POV Resolution to archived and active data together.
5. HLA-QUERY materializes the requested Presentation Model, delta, or diagnostic.

HLA-QUERY SHALL NOT let archived data bypass POV Resolution. Historical access is not a visibility shortcut.

---

<a id="12-asset-presentation-boundary"></a>
# 12. Asset Presentation Boundary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-QUERY may include `AssetRef` values in Presentation Models when assets are valid to present for the requested POV.

HLA-QUERY SHALL:

- Treat asset payload storage and retrieval mechanics as HLA-PERSIST concerns.
- Include only asset references and metadata that are valid to present.
- Expose a moderate asset presentation shape when known and authorized: `assetRef`, `assetKind` or type hint, `availabilityStatus`, optional `label`, optional `mediaType`, optional safe dimensions or duration, optional `previewRef` or `thumbnailRef`, optional `provenanceRef`, optional `portabilityClass`, and diagnostics.
- Surface missing, unavailable, or non-portable asset diagnostics when HLA-PERSIST reports them.
- Avoid embedding storage locators, byte layout, export package paths, or private backing metadata into Presentation Models unless an approved public contract later defines such fields.

HLA-QUERY SHALL NOT:

- Store asset payloads.
- Decide Campaign export completeness.
- Interpret asset content as authoritative Campaign Reality or Observer Knowledge by itself.
- Implement client rendering of images, maps, audio, documents, or media.

---

<a id="13-interface-contracts"></a>
# 13. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Public callers do not invoke HLA-QUERY directly. HLA-CONTRACT forwards authorized requests.

Ordinary client-facing reads SHALL use Presentation Model queries. HLA-QUERY may also support separately authorized tooling and authoring inspection operations for authoring tools, GM-facing tools, diagnostics, validation support, and test tooling. These inspection operations are distinct from ordinary client rendering paths and SHALL NOT expose raw authoritative-store dumps.

Conceptual internal operations:

```text
queryPresentation(request: QueryRequest) -> QueryResult
queryInspectionView(request: QueryRequest) -> QueryResult
getPresentationDelta(request: PresentationSyncRequest) -> PresentationSyncResult
queryArchived(request: QueryRequest) -> QueryResult
```

`queryPresentation` returns a Projection-specific Presentation Model after POV Resolution.

`queryInspectionView` returns separately authorized authoring, GM-facing, diagnostic, or test views over Campaign Reality-derived and Observer Knowledge-derived information. These views may expose more structure than ordinary Presentation Models when the caller has the required tooling capability, but they remain filtered by authority, observer/caller context, diagnostic policy, and safety rules. They are not raw HLA-CORE or HLA-OBSERVER access.

`getPresentationDelta` materializes caller-visible synchronization results.

`queryArchived` retrieves archived source data through HLA-PERSIST and then applies ordinary HLA-QUERY POV/projection behavior.

Conceptual result:

```text
QueryResult {
  resultKind
  presentationModel?
  inspectionView?
  diagnostics[]
  synchronizationState?
}
```

`inspectionView` is a tooling-oriented, authority-filtered view. It is not an ordinary client Presentation Model and is not a raw authoritative-store dump.

---

<a id="14-failure-semantics"></a>
# 14. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-QUERY SHALL fail closed when presentability is uncertain.

| Failure Trigger | Required Behavior |
|---|---|
| Missing or invalid Campaign reference | Reject query with diagnostics. |
| Missing or invalid Observer reference | Reject query with diagnostics. |
| Caller lacks required query capability | Reject through HLA-CONTRACT before HLA-QUERY execution where possible; fail closed if detected internally. |
| Projection type unsupported | Reject query with unsupported-projection diagnostic. |
| POV Resolution cannot determine whether content is presentable | Suppress uncertain content and return diagnostic when authorized. |
| Required active source data unavailable | Return unavailable diagnostic; do not fabricate Presentation Model content. |
| Required archived data unavailable | Return unavailable or partial diagnostic according to query semantics; do not bypass POV Resolution. |
| Prior revision markers do not match request basis | Return full state if allowed; otherwise return diagnostic. |
| Prior revision range no longer available | Return compacted delta or full state if safe and allowed; otherwise return diagnostic. |
| Asset payload unavailable | Return Presentation Model with authorized missing-asset diagnostic where the surrounding content is still valid; do not mutate state. |
| Projection Strategy fails | Return diagnostic and no partially trusted Projection content unless the strategy marks a safe partial result. |
| Cross-observer data would be exposed | Reject or suppress data and return security diagnostic only at authorized diagnostic detail. |

Diagnostics SHALL distinguish:

- Bad request.
- Unsupported projection.
- Unauthorized query.
- Unavailable active source.
- Unavailable archive.
- Unsatisfiable synchronization range.
- Suppressed content.
- Degraded asset presentation.
- Internal projection failure.

---

<a id="15-non-functional-derivation"></a>
# 15. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-QUERY Design Response |
|---|---|
| NFR-001 Performance | Queries are context-local and scope-local by default; Projection Strategies declare read hints; synchronization supports deltas and unchanged responses. |
| NFR-002 Security | HLA-QUERY fails closed on uncertain presentability, never bypasses POV Resolution, and never exposes raw source stores to clients. |
| NFR-003 Reliability | Presentation Models are derived from source revisions and can be recomputed after restart rather than persisted as authoritative data. |
| NFR-004 Scalability | Ordinary queries operate against the active working set; explicit archive bridging supports older data on demand. |
| NFR-005 Maintainability | POV Resolution, Projection Strategies, revision mapping, archive bridging, and delta materialization are separated internally. |
| NFR-006 Auditability | Presentation Models carry source revision sets and authorized provenance summaries sufficient to explain what source boundaries informed a result. |

HLA-QUERY SHALL NOT invent new quantitative performance targets during Detailed Design. Measurable thresholds remain governed by approved requirements and later Test Planning calibration.

---

<a id="16-testing-alignment"></a>
# 16. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL cover at least:

- Query for a valid Observer/context/time returns only POV-approved information.
- Query does not expose hidden Campaign Reality fields to a client.
- Query can present false, stale, or incomplete Observer Knowledge when that is what the Observer validly knows.
- Presentation Model requires no client-side visibility or knowledge filtering before rendering.
- `map`, `timeline`, `roster`, `journal`, and `relationshipView` projections materialize distinct Presentation Models from the same source state.
- Projection-specific content remains inside the shared Presentation Model envelope.
- Authorized tooling inspection views do not become ordinary client Presentation Model shortcuts or raw store dumps.
- Unsupported projection type is rejected with diagnostics.
- Presentation Model revision is scoped to Campaign/Observer/context/projection/time and not treated as a global Campaign counter.
- Presentation Model revision is derived deterministically from source revision set, query basis, projection type, and projection algorithm/version identity.
- Reconnecting caller receives delta, compacted delta, unchanged response, or full state according to available revision history and HLA-CONTRACT semantics.
- Expired, mismatched, or unsafe revision markers fail closed.
- Explicit archived-history query retrieves archived data through HLA-PERSIST and still applies POV Resolution.
- Missing asset payload produces a diagnostic without corrupting Campaign Reality or Observer Knowledge.
- Cross-observer leakage attempts are rejected or suppressed.

---

<a id="17-traceability-summary"></a>
# 17. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Design Coverage |
|---|---|
| FR-027 | Query Context Model, interface contracts, Presentation Model queries, and authorized tooling inspection views. |
| FR-028 | POV Resolution Model, fail-closed presentability, no client-side filtering requirement. |
| FR-029 | Presentation Model Lifecycle and materialization rules. |
| FR-030 | Projection Strategy Model and minimum projection types. |
| FR-011 | Historical query behavior and Campaign Time source boundaries. |
| FR-015 through FR-018 | Observer Knowledge reads and Information Source provenance as HLA-OBSERVER inputs. |
| FR-031 | Recomputable Presentation Models from durable source revisions. |
| FR-043 | Archive Bridging through HLA-PERSIST. |
| NFR-001 | Context-local/scope-local reads, read hints, deltas. |
| NFR-002 | Fail-closed presentability and no raw store exposure. |
| NFR-003 | Derived, recomputable Presentation Models. |
| NFR-004 | Active working set plus explicit archive bridging. |
| NFR-005 | Internal decomposition of query responsibilities. |
| NFR-006 | Source revision sets, deterministic presentation revisions, and authorized provenance summaries. |

---

<a id="18-open-questions"></a>
# 18. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

No HLA-QUERY-owned open questions remain at this design level.

Resolved during Detailed Design review:

- POV Resolution policy — HLA-QUERY SHALL start with a narrow engine-owned allow/suppress decision model for v1. Package presentation semantics may inform decisions, but arbitrary package-owned POV policy execution is deferred until demonstrably necessary.
- Presentation Model envelope — HLA-QUERY SHALL use a stable cross-projection envelope; projection-specific structure begins inside `content`.
- Caller-facing read surface — ordinary client reads SHALL use Presentation Model queries. Authoring, GM-facing, diagnostic, validation, and test tooling needs SHALL be served by separately authorized tooling inspection operations, not by exposing raw `queryReality` or `queryObserverKnowledge` paths as ordinary public client APIs.
- Presentation Model revisions — HLA-QUERY SHALL derive Presentation Model revisions deterministically from source revision set, query basis, projection type, and projection algorithm/version identity. Durable authoritative Presentation Model event streams are rejected for v1.
- Asset presentation metadata — HLA-QUERY SHALL expose a moderate, storage-neutral asset presentation shape and SHALL NOT leak HLA-PERSIST storage locators, byte layout, export paths, backing-store IDs, or private digests unless later approved public contracts define such fields.

---

<a id="19-phase-gate-declaration"></a>
# 19. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-QUERY Detailed Design is **approved at the component-design level**.

This approval does not authorize implementation by itself. It approves the HLA-QUERY Detailed Design baseline for continued Detailed Design work and later planning.

This design establishes the HLA-QUERY component boundary, internal decomposition, query context model, POV Resolution responsibilities, Presentation Model lifecycle, projection strategy model, synchronization materialization posture, archive bridging, asset presentation boundary, and failure semantics. No HLA-QUERY-owned open questions remain at this design level.

Implementation, testing, future multiplayer rescoping, authoring-tool refinement, or later component designs may reveal a need to revisit this design. Any material change SHALL be handled through the project's normal lifecycle change-control process.
