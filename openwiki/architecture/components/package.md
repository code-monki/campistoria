---
type: architecture-component
title: "HLA-PACKAGE: Package Composition and Versioning"
description: "How HLA-PACKAGE composes versioned, game-specific Packages into an Effective Campaign Definition through explicit Microkernel/Builder composition, pinning, and migration."
tags: [architecture, hla, microkernel, builder, package-composition, versioning]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-f5a1f0fc71fa0830d7096989
    resource: repo://docs/design/components/hla-package-dd.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-PACKAGE: Package Composition and Versioning

HLA-PACKAGE is the engine's boundary for package-defined game, setting, scenario, rule, procedure, Oracle, schema, and asset semantics. It is what lets the engine remain ruleset-independent: game-specific meaning lives in versioned Packages, while HLA-PACKAGE provides the registry, composition, pinning, migration, and removal controls needed to use those Packages safely in Campaigns.

## Why Microkernel + Builder

The architecture applies **Microkernel** at the subsystem level because the engine core has no compile-time knowledge of any specific game system, setting, or scenario — Packages are plugins loaded against a fixed core contract, which is Microkernel's defining shape ("the engine knows the meta-model; packages know the game").

Within that, composing one Effective Campaign Definition out of several declared parts, in a defined order, with explicit override/extension application, is a multi-step assembly process — **Builder's** defining shape — rather than a single-step creation (which would rule out Factory Method, used instead in [HLA-LIFECYCLE](lifecycle.md)) or copying an existing instance (which rules out Prototype: there is no existing Effective Campaign Definition to clone when composing one from scratch).

A **plain Strategy** (one algorithm picks the "winning" definition on conflict) was also rejected: FR-021 explicitly requires composition conflicts to fail loudly rather than be silently resolved by picking a winner, which a simple conflict-resolution Strategy would otherwise invite.

## Internal decomposition

| Internal Unit | Responsibility | Boundary |
|---|---|---|
| `PackageRegistry` | Stores registered Package identity, version, metadata, validation status | Not Campaign composition or data |
| `DependencyResolver` | Resolves dependencies, capability requirements, version references | Never silently picks a conflict winner |
| `CompositionBuilder` | Builds Effective Campaign Definitions from declared inputs, extensions, overrides | Does not validate itself — submits to HLA-VALIDATE |
| `ConflictDetector` | Detects collisions, missing overrides, incompatible dependencies | Reports diagnostics only; no human policy decisions |
| `PinManager` | Produces/resolves per-Campaign composition pins | Never mutates Campaign Reality |
| `MigrationPlanner` | Produces explicit migration plans and rollback prerequisites | Does not apply Campaign Reality Events |
| `RemovalGuard` | Identifies referencing Campaigns/archives/exports/pins before removal | Never deletes referenced data without forced acknowledgement |
| `SemanticReferenceCatalog` | Defines stable references for package-defined types, schemas, rules, Oracles, Procedures, assets | Does not own HLA-CORE Events |
| `PackageContractProvider` | Supplies declared contracts to HLA-VALIDATE, HLA-RESOLUTION, HLA-LIFECYCLE, HLA-QUERY | Never executes arbitrary package code |

## Composition is explicit — collision is not composition

Composition follows a fixed, diagnosable order: resolve requested Package identities/versions → resolve dependencies/capabilities → load package-defined definitions/schemas/contracts/scenarios/extensions/overrides/assets → detect collisions → apply explicit extensions → apply explicit overrides → produce an Effective Campaign Definition candidate → submit to HLA-VALIDATE → admit or reject.

**Extension** and **Override** are distinct and both must be explicit: an Extension adds or specializes behavior without replacing the referenced definition; an Override deliberately replaces or supersedes a referenced definition and must identify its target. HLA-PACKAGE never infers an override from name collision, load order, package priority, first/last match, or package author identity — an unresolved collision is always rejected with diagnostics, never silently picked.

## Effective Campaign Definition and composition pins

The **Effective Campaign Definition (ECD)** is the composed, package-defined semantic baseline used to instantiate or migrate a Campaign. It is derived from registered Package artifacts plus explicit composition inputs, validated before use, and reconstructible from pinned Package versions and composition inputs. It never becomes Campaign Reality by itself, never mutates a running Campaign outside the lifecycle/migration flow, and never hides unresolved composition conflicts.

A **`CompositionPin`** records the exact package set and composition context for a Campaign (`effectiveDefinitionId`, `pinnedPackageRefs[]`, `appliedExtensions[]`, `appliedOverrides[]`). This is what lets Campaigns stay pinned to the composition they were instantiated from: registering a new Package version never silently affects an existing Campaign's pin.

## Reconstructibility without duplication

HLA-CORE Events retain stable **`PackageSemanticRef`** values (`packageIdentity`, `packageVersion`, `semanticId`, `semanticKind` — e.g. `entityType`, `rule`, `oracle`, `procedure`, `asset`) rather than copying full Package definitions into every Event. HLA-PACKAGE owns *interpreting* those references against a Campaign's pinned Effective Campaign Definition. This balances reconstructibility against duplication: Events carry lightweight stable references, composition pins identify the exact package set needed to resolve them, and forced Package removal may degrade semantic interpretation availability but never corrupts HLA-CORE Event history.

## Migration and removal

Package migration is **explicit and atomic**: a `MigrationPlan` starts from the Campaign's current composition pin, proposes a target composition, identifies definition/schema/contract/asset/dependency changes and semantic references that may become unresolved, and is submitted to HLA-VALIDATE before being applied. It either applies atomically or leaves the previous composition pin fully intact — there is no mixed composition state. HLA-PACKAGE owns migration *planning* only; applying the resulting Campaign Reality transformations requires coordination with HLA-LIFECYCLE, HLA-STATE, HLA-RESOLUTION, and HLA-CORE, since HLA-PACKAGE never appends Campaign Reality Events directly.

Package removal is **dependency-aware**: a referenced Package version is not removed by default while any current, inactive, archived, or exported Campaign still references it. Forced removal requires explicit HLA-CONTRACT-authorized acknowledgement, and even then it never deletes HLA-CORE Event history — it may only degrade semantic interpretation for affected Campaigns.

## Validation handoff

HLA-PACKAGE invokes [HLA-VALIDATE](validate.md) at three points: before registering a `packageArtifact`, before admitting a composed `effectiveCampaignDefinition` for instantiation/migration use, and before applying a `migrationPlan`. Package-contributed validators remain declarative contracts interpreted by engine-owned validators — HLA-PACKAGE never executes package-supplied validator code.

## Dependency relationships

- **No dependency on** HLA-CORE or HLA-RESOLUTION for package definition — Packages are defined independently of any running Campaign.
- **Depended upon by** [HLA-LIFECYCLE](lifecycle.md) (Scenario/ECD resolution for Campaign creation) and [HLA-RESOLUTION](resolution.md) (Oracle/Procedure/Rule definitions), not the reverse.
- **Coordinates with** HLA-VALIDATE at every registration, composition, and migration admission point.

## Non-functional derivation

- **NFR-002 (Security):** package artifacts, composed definitions, and migration plans all pass HLA-VALIDATE before admission.
- **NFR-003 (Reliability):** atomic migration posture leaves prior composition pins fully intact on failure.
- **NFR-005 (Maintainability):** the Microkernel boundary keeps package-defined semantics separate from engine mechanics; structured package data avoids embedding RPG-system meaning in core code.
- **NFR-006 (Auditability):** composition pins, applied overrides/extensions, migration plans, and removal diagnostics preserve provenance for every package-related change.
