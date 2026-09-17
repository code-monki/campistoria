---
type: architecture-component
title: "HLA-PERSIST: Persistence and Portability"
description: "How HLA-PERSIST durably backs Event-Sourced streams without becoming a second source of truth, via implementation-neutral export/import, strict import admission, and authorized authority rebinding."
tags: [architecture, hla, adapter, persistence, import-export, portability]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-d2f2a598df4dcc4596cf253b
    resource: repo://docs/design/components/hla-persist-dd.md
  - id: openwiki-source-615487f2876b472d6b4fadc3
    resource: repo://docs/project/decision-log.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-PERSIST: Persistence and Portability

HLA-PERSIST provides durable backing, import/export, archival retrieval, and asset portability for data owned by other HLA components. It keeps Campaign data portable and recoverable **without becoming a second source of truth** for Campaign Reality, Observer Knowledge, Package definitions, or Checkpoints.

## Why Adapter, layered over Event Sourcing

Persistence is not a separate storage paradigm — it is the durable backing of the same Event logs already established in [HLA-CORE](core.md)/[HLA-OBSERVER](observer.md), exposed through format-specific Adapters that translate the internal event/snapshot model to and from an external representation, without the internal model needing to know about any specific external format. This satisfies FR-032's "implementation-neutral" representation requirement directly.

The rejected alternative was a dedicated persistence-specific data model, separate from the Event Sourcing log — rejected because maintaining a second representation purely for persistence purposes reintroduces the same drift risk that Event Sourcing was specifically chosen in HLA-CORE to avoid.

## Internal decomposition

| Internal Module | Responsibility | Persistent authority |
|---|---|---|
| `PersistenceCoordinator` | Coordinates save/load/export/import/archive operations | None — delegates to submodules |
| `EventStreamStoreAdapter` | Persists/retrieves HLA-CORE and HLA-OBSERVER event streams | Durable backing representation only |
| `SnapshotStoreAdapter` | Persists/retrieves HLA-STATE snapshot/checkpoint artifacts | Durable backing representation only |
| `AuthorityBindingStoreAdapter` | Persists/retrieves HLA-CONTRACT-owned Campaign Authority Bindings and source-binding provenance | Durable backing only — cannot evaluate or activate grants |
| `ExportAssembler` | Builds implementation-neutral export artifacts/manifests | Export artifact representation |
| `ImportReader` | Reads import envelopes only enough to identify subject and route validation | None — cannot admit data alone |
| `AssetPortabilityManager` | Resolves campaign-local asset inclusion/access guarantees for export/import | Asset payload representation and portability metadata |
| `ArchiveIndex` | Tracks active-window vs. archived segment references | Durable archival index metadata |
| `ArchiveRetriever` | Retrieves archived data on demand | None — retrieval adapter over archive representation |

## Persistence boundary: who owns what

HLA-PERSIST never becomes the authoritative owner of anything it stores:

| Data Class | Authoritative Owner | HLA-PERSIST Role |
|---|---|---|
| Campaign Reality Events | HLA-CORE | Durable backing, export/import, archival segmentation |
| Observer Knowledge Events | HLA-OBSERVER | Durable backing, export/import, archival segmentation |
| Package composition pins | HLA-PACKAGE | Durable backing and export/import representation |
| Campaign Authority Bindings | HLA-CONTRACT | Durable backing; provenance export; import handoff for rebinding |
| Checkpoints/snapshots | HLA-STATE | Durable backing and export/import representation |
| Campaign-local assets | Referenced by HLA-CORE/HLA-OBSERVER | Asset payload storage, portability guarantee |
| Package-sourced assets | HLA-PACKAGE | Referenced, not duplicated, when independently exportable |
| Presentation Models | HLA-QUERY | **Not persisted** as authoritative state at all |

HLA-CORE and HLA-OBSERVER may only carry opaque `AssetRef` values; HLA-PERSIST owns the durable mapping from those references to actual payloads and manifests.

## Strict import admission

Campaign import artifacts are treated as **untrusted external input**. The flow: (1) HLA-CONTRACT authorizes the import request; (2) `ImportReader` reads only enough of the envelope to identify type/version/manifest for validation routing; (3) the artifact is submitted to [HLA-VALIDATE](validate.md) as `campaignImportArtifact`; (4) HLA-VALIDATE must return a strict `accept` before admission; (5) HLA-PERSIST reconstructs durable backing data, treating all exported authority-binding identifiers as provenance only; (6) HLA-CONTRACT creates a **new** active local Campaign Authority Binding for the authorized importing Principal; (7) owning components admit reconstructed data through their own contracts; (8) the imported data and new local binding commit atomically.

**`acceptWithWarnings`, `reject`, and `indeterminate` never admit import data — partial import is prohibited.** A foreign Principal or capability grant carried in an export never becomes active local authority without the explicit HLA-CONTRACT rebinding step. This rebinding behavior implements decision [DEC-001](../../reference/decision-log.md) ("Campaign-Scoped Principal Authority Binding"): exported source-binding identifiers are retained only as non-authoritative provenance, and HLA-PERSIST itself never decides whether a request is authorized — that stays exclusively in HLA-CONTRACT.

## Asset and artifact portability

Assets carry `assetOrigin` (`campaignLocal`, `packageSourced`, `externalReference`) and `portabilityClass` (`includedInCampaignExport`, `guaranteedByPackageExport`, `externalAccessRequired`, `missing`). The key rule: **Campaign export fails if any `campaignLocal` asset referenced by Campaign Reality or Observer Knowledge cannot be included or otherwise guaranteed portable** — a degraded export with missing assets is not a valid Campaign export under FR-032/FR-042, even if HLA-PERSIST can produce a diagnostic/report-only packaging audit. Package-sourced assets are *not* duplicated into every Campaign export when the Package is independently exportable, but composition pins alone are never treated as proof of availability — HLA-PERSIST actively verifies portability during export and fails with diagnostics if it cannot.

## Active window vs. archival retrieval

HLA-PERSIST tracks segments as `active`, `archived`, `unavailable`, or `corrupt`, retrieves archived Campaign Reality/Observer Knowledge/Event history on demand for explicit queries, and returns diagnostics rather than silent empty results when archives are unavailable or corrupt — it never loads the entire Campaign history for ordinary active-window queries. The provisional active window default is the SRS-defined ~30 in-game days, though exact thresholds remain calibration inputs for Test Planning and later profiling.

## Technology selection posture

HLA-PERSIST does not select a database, file format, object store, compression algorithm, indexing engine, cloud service, or deployment model. It instead defines the criteria any future selection must satisfy: preserve append-only Event stream semantics, preserve separate Campaign Reality/Observer Knowledge streams, support atomic import admission, support implementation-neutral export/import, support Campaign-local asset portability, support active-window/archival separation, support integrity verification, and support local durable operation without requiring cloud services. Future trade studies comparing concrete technologies against these criteria are implementation-planning inputs, not decisions this design makes.

## Failure semantics

HLA-PERSIST **fails closed for import/admission** (any validation failure, missing asset payload, or malformed artifact rejects the import with no partial state admission) and **fails explicitly for retrieval/export diagnostics** (an unavailable or corrupt archive segment returns an explicit diagnostic rather than misleading or silently empty data; export that cannot guarantee a campaign-local asset fails outright rather than producing an incomplete export).

## Dependency relationships

- **Depends on** [HLA-CORE](core.md) and [HLA-OBSERVER](observer.md) for the data it persists.
- **Depended upon by** [HLA-CONTRACT](contract.md) only among public-facing components.
- **Coordinates with** HLA-VALIDATE for import admission and with HLA-CONTRACT for authority binding evaluation — HLA-PERSIST stores bindings durably but never evaluates or activates them itself.

## Non-functional derivation

- **NFR-002 (Security):** imports and external assets pass validation/admission checks before affecting state; failed imports have no partial effect.
- **NFR-003 (Reliability):** durable backing preserves last valid state and reports diagnosable failures rather than silent corruption.
- **NFR-004 (Scalability):** the active-window/archival split and `resolveArchived` keep old data reachable without loading it for ordinary queries.
- **NFR-005 (Maintainability):** persistence adapters isolate storage/export mechanisms from every other HLA component.
- **NFR-006 (Auditability):** Event streams, provenance references, composition pins, and archive manifests remain retrievable through durable backing.
