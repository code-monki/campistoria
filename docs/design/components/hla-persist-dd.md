# HLA-PERSIST Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-PERSIST
Version: 0.1 (Component Draft)
Date (YYYY-MM-DD): 2026-09-13
Author(s): CodeMonki
Status: Approved
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
- [6. Technology Selection Criteria](#6-technology-selection-criteria)
- [7. Persistence Boundary Model](#7-persistence-boundary-model)
- [8. Asset and Artifact Portability](#8-asset-and-artifact-portability)
- [9. Interface Contracts](#9-interface-contracts)
- [10. Import Validation and Admission](#10-import-validation-and-admission)
- [11. Active Window and Archival Retrieval](#11-active-window-and-archival-retrieval)
- [12. Data Design](#12-data-design)
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
- Architectural Component ID stable? **Yes** — HLA-PERSIST.
- Parent Detailed Design phase opened? **Yes** — `engine-detailed-design.md` v0.1.
- Advancement to implementation authorized? **No.**

This document refines HLA-PERSIST only. It does not authorize implementation, select storage technology, or introduce a new architectural component.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-PERSIST — Persistence & Portability.

**Component purpose:** HLA-PERSIST provides durable backing, import/export, archival retrieval, and asset portability for data owned by other HLA components. It keeps campaign data portable and recoverable without becoming a second source of truth for Campaign Reality, Observer Knowledge, Package definitions, or Checkpoints.

**Primary requirements:** FR-031, FR-032, FR-042, FR-043.

**Related NFRs:** NFR-003, NFR-004, NFR-005.

**In scope:**

- Durable storage representation boundary for Campaign Reality, Observer Knowledge, Package composition, Events, snapshots/checkpoints, and archival segments.
- Implementation-neutral import/export envelope model.
- Campaign-local asset portability.
- Opaque asset/artifact references usable by HLA-CORE and HLA-OBSERVER without making them blob stores.
- Active-window versus archival retrieval boundary.
- Import validation handoff to HLA-VALIDATE.
- Failure behavior for missing assets, invalid imports, interrupted persistence, and unavailable archives.
- Selection criteria and adapter boundaries for future storage, export format, archival, compression, indexing, object store, cloud-service, and deployment choices.

**Out of scope:**

- Final selection of database, file format, object store, compression algorithm, indexing engine, cloud service, or deployment model.
- HLA-CORE Event schema and state fold semantics.
- HLA-OBSERVER event schema.
- HLA-STATE Checkpoint and Recovery algorithms.
- HLA-PACKAGE package model and independently exportable Package artifacts.
- HLA-QUERY projection and presentation of assets.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PERSIST SHALL preserve the approved HLA constraints:

- It provides durable storage, import, export, archival retrieval, and asset portability without owning live authoritative state.
- It persists HLA-CORE and HLA-OBSERVER event streams without merging Campaign Reality and Observer Knowledge.
- It treats persistence as durable backing for Event Sourcing, not as a second authoritative data model.
- It depends on HLA-CORE and HLA-OBSERVER for data to persist.
- It invokes HLA-VALIDATE before imported Campaign data can affect Campaign Reality or Observer Knowledge.
- It keeps storage technology, export packaging mechanism, indexing, compression, and archival mechanism open.
- It defines technology selection criteria and adapter boundaries without selecting concrete technologies.

Any design that makes HLA-PERSIST the owner of live Campaign Reality, merges Campaign Reality with Observer Knowledge for storage convenience, or binds portability to one storage engine violates the approved HLA.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PERSIST SHALL:

- Persist and retrieve durable representations of Campaign data sufficient for close/reopen survival.
- Export an implementation-neutral Campaign representation.
- Import an implementation-neutral Campaign representation after validation.
- Preserve Campaign identity, seed, Package composition pins, Campaign Reality history, Observer Knowledge history, and required provenance across save/reload and export/import.
- Include or guarantee access to campaign-local binary assets required by FR-042.
- Provide on-demand retrieval of archived Campaign Reality, Observer Knowledge, and Event/History data outside the active working set.
- Return diagnostics for missing, malformed, unavailable, or non-portable artifacts.

HLA-PERSIST SHALL NOT:

- Own the authoritative live Campaign Reality model.
- Own Observer Knowledge.
- Decide POV Resolution or Presentation Model output.
- Validate imports itself except by coordinating format identification and invoking HLA-VALIDATE.
- Duplicate Package-sourced assets into every Campaign export when an independently exportable Package already guarantees portability.
- Select a concrete storage technology in Detailed Design.

**Invariants:**

- Campaign identity remains stable across persistence, export/import, and migration.
- Campaign Reality and Observer Knowledge remain separately reconstructible.
- Exported Campaign representation is not bound to the originating storage engine.
- Referenced campaign-local assets are portable or produce explicit export/import diagnostics.
- Archived data remains semantically retrievable, not merely retained as inaccessible bytes.

---

<a id="5-internal-decomposition"></a>
# 5. Internal Decomposition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PERSIST SHALL be decomposed internally to avoid becoming a God Component:

| Internal Module | Responsibility | Persistent Authority |
|---|---|---|
| `PersistenceCoordinator` | Coordinates save/load/export/import/archive operations. | None; delegates to submodules. |
| `EventStreamStoreAdapter` | Persists and retrieves HLA-CORE and HLA-OBSERVER event streams. | Durable backing representation only. |
| `SnapshotStoreAdapter` | Persists and retrieves HLA-STATE snapshot/checkpoint artifacts as durable backing. | Durable backing representation only. |
| `ExportAssembler` | Builds implementation-neutral export artifacts and manifests. | Export artifact representation. |
| `ImportReader` | Reads import envelopes enough to identify subject and route validation. | None; cannot admit data alone. |
| `AssetPortabilityManager` | Resolves campaign-local asset inclusion/access guarantees for export/import. | Asset payload representation and portability metadata. |
| `ArchiveIndex` | Tracks active-window and archived segment references. | Durable archival index metadata. |
| `ArchiveRetriever` | Retrieves archived Campaign Reality, Observer Knowledge, and Event/History segments on demand. | None; retrieval adapter over archive representation. |

These are Detailed Design subcomponents, not new HLA components.

---

<a id="6-technology-selection-criteria"></a>
# 6. Technology Selection Criteria

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PERSIST does not select a database, file format, object store, compression algorithm, indexing engine, cloud service, or deployment model. It SHALL define the criteria that future selections must satisfy.

Future persistence technology choices SHALL:

- Preserve append-only Event stream semantics for HLA-CORE and HLA-OBSERVER.
- Preserve separate Campaign Reality and Observer Knowledge streams.
- Support atomic import admission: validated import succeeds as a whole or has no authoritative effect.
- Support implementation-neutral export/import independent of the originating storage engine.
- Support Campaign-local asset portability required by FR-042.
- Support active-window and archival separation required by NFR-004 and FR-043.
- Support integrity verification for archived segments and asset payloads.
- Support local durable operation without requiring cloud services.
- Hide storage-specific locators and mechanics behind HLA-PERSIST-owned metadata.
- Allow export, archive, compression, indexing, and storage adapters to evolve without redefining HLA-CORE, HLA-OBSERVER, HLA-PACKAGE, HLA-STATE, HLA-QUERY, or HLA-CONTRACT.

Future trade studies MAY compare concrete databases, file formats, object stores, compression approaches, indexes, and deployment models against these criteria. Such trade studies are implementation-planning inputs, not decisions made by this draft.

---

<a id="7-persistence-boundary-model"></a>
# 7. Persistence Boundary Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PERSIST stores durable representations of data owned by other components. It does not become a separate source of truth.

**Authoritative owner to persistence boundary:**

| Data Class | Authoritative Owner | HLA-PERSIST Role |
|---|---|---|
| Campaign Reality Events | HLA-CORE | Durable backing, export/import representation, archival segmentation. |
| Observer Knowledge Events | HLA-OBSERVER | Durable backing, export/import representation, archival segmentation. |
| Package composition pins | HLA-PACKAGE / Campaign metadata | Durable backing and export/import representation. |
| Checkpoints/snapshots | HLA-STATE | Durable backing and export/import representation. |
| Campaign-local assets | Referenced by HLA-CORE/HLA-OBSERVER; payload managed by HLA-PERSIST | Asset payload storage, portability guarantee, export/import representation. |
| Package-sourced assets | HLA-PACKAGE | Referenced, not duplicated, when Package export independently guarantees portability. |
| Presentation Models | HLA-QUERY | Not persisted as authoritative state. |

HLA-CORE and HLA-OBSERVER may reference assets using opaque `ArtifactRef` or `AssetRef` values. HLA-PERSIST owns the durable mapping from those references to payloads and export/import manifests. HLA-CORE and HLA-OBSERVER SHALL NOT embed binary payloads as authoritative state.

---

<a id="8-asset-and-artifact-portability"></a>
# 8. Asset and Artifact Portability

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PERSIST SHALL support Campaign-local asset portability without making the engine aware of asset rendering semantics.

Conceptual asset reference:

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

`assetOrigin` is one of:

- `campaignLocal`
- `packageSourced`
- `externalReference`

`portabilityClass` is one of:

- `includedInCampaignExport`
- `guaranteedByPackageExport`
- `externalAccessRequired`
- `missing`

HLA-PERSIST SHALL:

- Include or otherwise guarantee access to `campaignLocal` asset payloads in Campaign export.
- Avoid duplicating `packageSourced` assets in every Campaign export when the Package is independently exportable and obtainable.
- Verify that `packageSourced` assets are independently portable or obtainable through the registered Package composition pins during Campaign export.
- Report diagnostics for `externalReference` assets that cannot be guaranteed portable.
- Preserve stable asset identity across export/import where possible.
- Treat asset payloads as opaque bytes plus metadata; rendering/interpretation belongs to clients and HLA-QUERY Presentation Models.

Missing asset payloads SHALL NOT corrupt Campaign Reality or Observer Knowledge, but SHALL produce explicit diagnostics and degraded presentation behavior through HLA-QUERY where applicable.

Campaign export SHALL fail if any `campaignLocal` asset referenced by Campaign Reality or Observer Knowledge cannot be included or otherwise guaranteed portable. HLA-PERSIST SHALL return structured diagnostics identifying the missing or non-portable asset references. A diagnostic/report-only packaging audit may enumerate portability failures, but it SHALL NOT be treated as a valid Campaign export satisfying FR-032 or FR-042.

Package composition pins identify expected Package dependencies, but they are not sufficient proof that Package-sourced assets are currently portable or obtainable. If a referenced Package-sourced asset cannot be verified as independently portable or obtainable, Campaign export SHALL fail with diagnostics. Duplicating Package-sourced assets into Campaign export is deferred unless later requirements explicitly authorize that export policy.

HLA-CORE and HLA-OBSERVER SHALL carry only the minimum semantic `AssetRef` metadata needed to identify and reason about asset references: stable asset identity, origin, owner scope, optional type hint, portability class, and provenance reference. Storage locators, byte layout, export packaging paths, content digests, and retrieval mechanics belong to HLA-PERSIST-owned manifests or backing metadata, not authoritative Campaign Reality or Observer Knowledge.

---

<a id="9-interface-contracts"></a>
# 9. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Conceptual export request:

```text
export(campaignId, exportOptions) -> ExportArtifact
```

Conceptual import request:

```text
import(importArtifact, importOptions) -> ImportResult
```

Conceptual archival retrieval request:

```text
resolveArchived(campaignId, timeRange, dataScope) -> HistoricalData
```

Conceptual asset resolution request:

```text
resolveAsset(assetRef, accessPurpose) -> AssetResolution
```

**Preconditions:**

- HLA-CONTRACT has authorized the caller for export, import, archival query, or asset access as applicable.
- Import artifacts are submitted to HLA-VALIDATE before admission.
- Export options do not require a storage or packaging technology not selected by the project.

**Postconditions:**

- Export produces an implementation-neutral artifact or a structured diagnostic.
- Import either creates/reconstructs an equivalent Campaign or has no effect.
- Archived retrieval returns requested historical data or explicit unavailability diagnostics.
- Asset resolution returns payload access, portability diagnostics, or missing-asset diagnostics.

---

<a id="10-import-validation-and-admission"></a>
# 10. Import Validation and Admission

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Campaign import artifacts are untrusted external input.

Import flow:

1. HLA-CONTRACT authorizes the import request.
2. `ImportReader` reads only enough of the envelope to identify artifact type, version, manifest, and validation subject.
3. HLA-PERSIST submits the artifact to HLA-VALIDATE as `campaignImportArtifact`.
4. HLA-VALIDATE must return strict `accept` before admission.
5. HLA-PERSIST reconstructs durable backing data and asset payload references.
6. HLA-CORE/HLA-OBSERVER/HLA-PACKAGE/HLA-STATE owners admit reconstructed data through their component contracts.

`acceptWithWarnings`, `reject`, or `indeterminate` SHALL NOT admit Campaign import data. Partial import is prohibited.

---

<a id="11-active-window-and-archival-retrieval"></a>
# 11. Active Window and Archival Retrieval

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PERSIST SHALL support NFR-004 without selecting the implementation mechanism.

Conceptual archival states:

- `active`
- `archived`
- `unavailable`
- `corrupt`

HLA-PERSIST SHALL:

- Track which event/history segments are active versus archived.
- Retrieve archived Campaign Reality, Observer Knowledge, and Event/History data on demand for explicit queries.
- Preserve semantic availability of archived data for reconstruction and search.
- Return diagnostics rather than silent empty results when archives are unavailable or corrupt.
- Avoid loading the entire Campaign history for ordinary active-window queries.

The provisional active window is the SRS-defined default equivalent to approximately 30 in-game days, but exact thresholds and Package-defined time mapping remain calibration inputs for Test Planning and later implementation profiling.

---

<a id="12-data-design"></a>
# 12. Data Design

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Conceptual export artifact:

```text
ExportArtifact {
  manifest
  campaignIdentity
  packageCompositionPins
  eventStreams
  observerStreams
  checkpoints?
  assetManifest
  archiveManifest?
}
```

Conceptual import result:

```text
ImportResult {
  outcome
  campaignId?
  diagnostics[]
  assetDiagnostics[]
}
```

Conceptual archive manifest entry:

```text
ArchiveSegmentRef {
  segmentId
  campaignRef
  streamType
  timeRange
  eventRange?
  storageRef
  integrityRef?
}
```

HLA-PERSIST may define manifests, references, and durable backing metadata. It does not define the internal HLA-CORE Event schema or HLA-OBSERVER knowledge schema.

HLA-PERSIST SHALL maintain enough integrity metadata to detect missing, mismatched, truncated, or corrupted archived segments and asset payloads before returning them as valid. The exact integrity metadata schema is deferred until HLA-CORE, HLA-OBSERVER, and export artifact schemas are refined, but it must include stable identity, ownership/scope, representation version, and a content-integrity check or equivalent verification mechanism.

---

<a id="13-failure-semantics"></a>
# 13. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Failure Trigger | Outcome | Side Effect Posture |
|---|---|---|
| Save interrupted | Recoverable diagnostic or last valid state; no silent corruption. | NFR-003 / HLA-STATE recovery semantics apply. |
| Export cannot include or guarantee a campaign-local asset | Export fails with diagnostics. | No Campaign state change; no valid Campaign export is produced. |
| Import artifact fails validation | Import rejected. | No partial import. |
| Import asset payload missing | Import rejected unless artifact explicitly marks asset as external and caller accepts degraded portability. | No partial authoritative state admission. |
| Archive segment unavailable | Explicit diagnostic. | Active working set remains queryable. |
| Archive segment corrupt | Explicit diagnostic; do not return misleading data. | No mutation. |
| Storage adapter unavailable | Component failure diagnostic. | No silent state loss. |

HLA-PERSIST fails closed for import/admission and fails explicitly for retrieval/export diagnostics.

---

<a id="14-non-functional-derivation"></a>
# 14. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-PERSIST Design Mechanism |
|---|---|
| NFR-002 Security | Imports and external assets pass validation/admission checks before affecting state; failed imports have no partial effect. |
| NFR-003 Reliability | Durable backing preserves last valid state and reports diagnosable failures rather than silent corruption. |
| NFR-004 Scalability | Active-window/archival split and `resolveArchived` allow old data to remain reachable without being loaded for ordinary queries. |
| NFR-005 Maintainability | Persistence adapters isolate storage/export mechanisms from HLA-CORE, HLA-OBSERVER, HLA-PACKAGE, HLA-STATE, and HLA-QUERY. |
| NFR-006 Auditability | Event streams, provenance references, Package composition pins, and archive manifests remain retrievable through durable backing. |

No new quantitative performance target is introduced here.

---

<a id="15-testing-alignment"></a>
# 15. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL include tests proving:

- A saved/closed/reopened Campaign produces equivalent query results.
- Export/import reproduces an equivalent Campaign without requiring the originating storage technology.
- Campaign-local assets added during play survive export/import on a system without prior access to those assets.
- Package-sourced assets are not duplicated into Campaign export when Package portability is independently guaranteed.
- Package-sourced asset portability is verified during Campaign export; pins alone are not treated as proof of availability.
- Malformed or hostile import artifacts are rejected before any partial state admission.
- Ordinary active-window queries do not require loading archived history.
- Explicit archived-history queries retrieve older Campaign Reality, Observer Knowledge, or Event/History data on demand.
- Missing or corrupt archive segments return diagnostics rather than silent empty results.
- Missing, mismatched, truncated, or corrupt asset payloads and archive segments are detected before being returned as valid.
- HLA-CORE and HLA-OBSERVER streams remain separately reconstructible after persistence/export/import.
- Future storage/export/archive technology choices can be evaluated against the criteria in Section 6 without altering HLA component boundaries.

Test IDs are intentionally not assigned until the Test Planning phase.

---

<a id="16-traceability-summary"></a>
# 16. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Coverage in This Draft |
|---|---|
| FR-031 | Durable backing for Campaign Reality, Observer Knowledge, Package composition, history, and snapshots. |
| FR-032 | Implementation-neutral export/import envelope and strict import admission. |
| FR-042 | Campaign-local asset portability model and asset manifest boundary. |
| FR-043 | Active-window/archival split and `resolveArchived` retrieval contract. |
| NFR-004 | Active working set separated from archived segments while preserving on-demand retrieval. |

This design is sufficient to approve the HLA-PERSIST component-design baseline for continued Detailed Design work and later planning.

---

<a id="17-open-questions"></a>
# 17. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

None.

**Resolved during review:**

- Campaign-local asset export completeness — Campaign export fails if any campaign-local asset referenced by Campaign Reality or Observer Knowledge cannot be included or otherwise guaranteed portable. Degraded exports are not valid Campaign exports under FR-032/FR-042; HLA-PERSIST may produce diagnostics or a packaging audit, but not an importable Campaign export claiming completeness.
- Archive and asset integrity posture — HLA-PERSIST must maintain enough integrity metadata to detect missing, mismatched, truncated, or corrupted archived segments and asset payloads before returning them as valid. Exact fields are deferred until HLA-CORE, HLA-OBSERVER, and export artifact schemas are refined, but the eventual schema must include stable identity, ownership/scope, representation version, and a content-integrity check or equivalent verification mechanism.
- Package-sourced asset verification — Campaign export SHALL verify that Package-sourced assets referenced by Campaign Reality or Observer Knowledge are independently portable or obtainable through the registered Package composition pins. Pins identify expected dependencies but are not proof of availability. If verification fails, Campaign export fails with diagnostics. Duplicating Package-sourced assets into Campaign export is deferred unless later requirements explicitly authorize that policy.
- Minimum `AssetRef` metadata — HLA-CORE and HLA-OBSERVER carry semantic reference metadata only: stable asset identity, origin, owner scope, optional type hint, portability class, and provenance reference. Storage locators, byte layout, export packaging paths, content digests, and retrieval mechanics remain HLA-PERSIST-owned metadata and SHALL NOT leak into authoritative state.
- Technology selection posture — HLA-PERSIST discusses selection criteria and adapter boundaries for databases, file formats, object stores, compression, indexing, cloud services, and deployment models, but does not select concrete technologies in this draft.

---

<a id="18-phase-gate-declaration"></a>
# 18. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PERSIST Detailed Design is **approved at the component-design level**.

This approval does not authorize implementation by itself. It approves the HLA-PERSIST Detailed Design baseline for continued Detailed Design work and later planning.

This design establishes persistence and portability boundaries, storage-technology selection criteria, durable backing responsibilities, import/export posture, asset portability guarantees, integrity metadata posture, active-window and archival retrieval behavior, and HLA-VALIDATE admission handoff. No HLA-PERSIST-owned open questions remain at this draft level.

Implementation, testing, package-format SRD work, storage technology selection, archival tuning, or later component designs may reveal a need to revisit this design. Any material change SHALL be handled through the project's normal lifecycle change-control process.
