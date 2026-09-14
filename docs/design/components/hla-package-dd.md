# HLA-PACKAGE Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-PACKAGE
Version: 0.1 (Component Draft)
Date (YYYY-MM-DD): 2026-09-14
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
- [6. Package Artifact Model](#6-package-artifact-model)
- [7. Composition Model](#7-composition-model)
- [8. Effective Campaign Definition](#8-effective-campaign-definition)
- [9. Composition Pins and Core Metadata](#9-composition-pins-and-core-metadata)
- [10. Migration and Removal](#10-migration-and-removal)
- [11. Interface Contracts](#11-interface-contracts)
- [12. Validation and Diagnostics](#12-validation-and-diagnostics)
- [13. Asset and Portability Boundary](#13-asset-and-portability-boundary)
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
- Architectural Component ID stable? **Yes** — HLA-PACKAGE.
- Parent Detailed Design phase opened? **Yes** — `engine-detailed-design.md` v0.1.
- Advancement to implementation authorized? **No.**

This document refines HLA-PACKAGE only. It does not authorize implementation, select a manifest syntax, or introduce a new architectural component.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-PACKAGE — Package Composition & Versioning.

**Component purpose:** HLA-PACKAGE is the engine's boundary for package-defined game, setting, scenario, rule, procedure, Oracle, schema, and asset semantics. It lets the engine remain ruleset-independent by keeping game-specific meaning in versioned Packages while providing the registry, composition, pinning, migration, and removal controls needed to use those Packages safely in Campaigns.

**Primary requirements:** FR-019, FR-020, FR-021, FR-022, FR-023, FR-024, FR-025, FR-026.

**Related requirements/NFRs:** FR-037, FR-038, FR-041, FR-042, NFR-005, NFR-006.

**In scope:**

- Package identity, version metadata, system/edition metadata, and dependency declarations.
- Structured package artifact model sufficient for Systems, Settings, Scenarios, Rules, Oracles, Procedures, Overrides, schemas, definitions, and assets.
- Package registration and coexistence of multiple versions.
- Explicit composition into an Effective Campaign Definition.
- Conflict detection and explicit override/extension behavior.
- Per-Campaign composition pins.
- Explicit package migration planning and atomic migration posture.
- Dependency-aware package removal behavior.
- Metadata HLA-CORE must retain with Events to keep history reconstructible after migration or package removal.
- Validation handoff to HLA-VALIDATE for package artifacts, Effective Campaign Definitions, and migration plans.

**Out of scope:**

- Concrete package manifest syntax or serialization format.
- Concrete schema language.
- Concrete rule, procedure, or Oracle execution language.
- Sandboxed executable package code.
- HLA-CORE Event append implementation.
- HLA-RESOLUTION Oracle Adapter invocation mechanics.
- HLA-PERSIST storage/export packaging format.
- HLA-QUERY Presentation Model generation.
- Runtime caller authorization, which is owned by HLA-CONTRACT.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE SHALL preserve the approved HLA constraints:

- It is the Microkernel boundary for package-defined game, setting, scenario, rule, procedure, Oracle, schema, and asset semantics.
- It owns Package registry data, version metadata, Effective Campaign Definitions, and per-Campaign composition pins.
- It does not depend on HLA-CORE or HLA-RESOLUTION for package definition.
- It invokes HLA-VALIDATE before registering package artifacts, admitting composed Effective Campaign Definitions, or applying migration plans.
- It does not allow Package content to access engine-core internals.
- It does not silently resolve collisions between Packages.
- It does not automatically migrate existing Campaigns when a Package is registered or upgraded.
- It keeps package version identity independent of RPG system/edition identity.

Any design that lets Package code mutate Campaign Reality directly, treats package upgrade as silent replacement, or resolves conflicts by choosing a winner without explicit override/extension violates the approved HLA.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE SHALL:

- Register validated Package artifacts by stable identity and version.
- Permit multiple versions of the same Package identity to coexist.
- Track system and edition identity as metadata independent of Package version.
- Resolve declared dependencies and required capabilities.
- Compose Packages, extensions, and overrides into an Effective Campaign Definition.
- Reject unresolved composition conflicts with diagnostics.
- Produce per-Campaign composition pins at Campaign instantiation or migration.
- Produce explicit migration plans when a Campaign changes package composition.
- Prevent removal of referenced Package versions unless forced with explicit acknowledgement.
- Provide package-defined contracts to HLA-RESOLUTION, HLA-VALIDATE, HLA-LIFECYCLE, and HLA-QUERY through internal interfaces.
- Define durable semantic references that HLA-CORE Events can retain for reconstructible history.

HLA-PACKAGE SHALL NOT:

- Own Campaign Reality or Observer Knowledge.
- Apply Resolution Commands or append HLA-CORE Events.
- Execute arbitrary package-supplied code.
- Validate package/import/oracle content except by invoking HLA-VALIDATE.
- Store binary asset payloads as package registry authority when HLA-PERSIST owns durable backing.
- Choose a storage engine, package file format, schema language, scripting language, or deployment model.

---

<a id="5-internal-decomposition"></a>
# 5. Internal Decomposition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE SHALL be decomposed internally:

| Internal Unit | Responsibility | Boundary |
|---|---|---|
| `PackageRegistry` | Stores registered Package identity, version, metadata, and validation status. | Does not compose Campaigns or own Campaign data. |
| `DependencyResolver` | Resolves package dependencies, capability requirements, and version references. | Does not silently choose conflict winners. |
| `CompositionBuilder` | Builds Effective Campaign Definitions from declared package inputs, extensions, and overrides. | Does not validate by itself; submits subjects to HLA-VALIDATE. |
| `ConflictDetector` | Detects collisions, missing overrides, incompatible dependencies, and ambiguous definitions. | Reports diagnostics; does not decide human policy. |
| `PinManager` | Produces and resolves per-Campaign composition pins. | Does not mutate Campaign Reality. |
| `MigrationPlanner` | Produces explicit package migration plans and rollback prerequisites. | Does not apply Campaign Reality Events. |
| `RemovalGuard` | Identifies Campaigns, archives, exports, or pins referencing a Package version before removal. | Does not delete referenced data without force acknowledgement. |
| `SemanticReferenceCatalog` | Defines stable references for package-defined types, schemas, rules, Oracles, Procedures, assets, and definitions. | Does not own HLA-CORE Events. |
| `PackageContractProvider` | Supplies declared contracts to HLA-VALIDATE, HLA-RESOLUTION, HLA-LIFECYCLE, and HLA-QUERY. | Does not execute arbitrary package code. |

These units may be implemented as modules, classes, services, or equivalent language-native constructs.

---

<a id="6-package-artifact-model"></a>
# 6. Package Artifact Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE SHALL use structured package data for v1. A separate declarative composition language is deferred unless structured data proves insufficient for an approved requirement. This resolves the carried-forward question from `engine-detailed-design.md` Section 6 for the current design.

Conceptual package artifact:

```text
PackageArtifact {
  packageIdentity
  packageVersion
  systemIdentity?
  editionIdentity?
  packageRole
  declaredDependencies[]
  declaredCapabilities[]
  definitions[]
  schemas[]
  ruleContracts[]
  oracleContracts[]
  procedureContracts[]
  scenarioContent?
  extensionDeclarations[]
  overrideDeclarations[]
  assetDeclarations[]
  validationDeclarations[]
  compatibilityDeclarations[]
  migrationDeclarations[]
  provenance
}
```

`packageRole` MAY include:

- `systemDefinition`
- `setting`
- `scenario`
- `focusedCapability`
- `userOverride`

The concrete artifact representation remains open. Package artifacts may eventually be represented as JSON, XML, YAML, database records, archives, or another format, but this Detailed Design commits only to required conceptual fields and relationships.

---

<a id="7-composition-model"></a>
# 7. Composition Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Composition SHALL be explicit. Collision is not composition.

Conceptual composition request:

```text
CompositionRequest {
  requestedPackages[]
  requestedScenario?
  requestedOverrides[]
  targetCampaignRef?
  compositionPurpose
}
```

`compositionPurpose` is one of:

- `campaignInstantiation`
- `migrationPlanning`
- `validationOnly`

Composition order SHALL be explicit and diagnosable:

1. Resolve requested Package identities and versions.
2. Resolve declared dependencies and required capabilities.
3. Load package-defined definitions, schemas, contracts, scenarios, extensions, overrides, and asset declarations.
4. Detect collisions.
5. Apply explicit extensions.
6. Apply explicit overrides.
7. Produce an Effective Campaign Definition candidate.
8. Submit the candidate to HLA-VALIDATE.
9. Admit or reject the Effective Campaign Definition.

Overrides and extensions are distinct:

- **Extension:** adds or specializes behavior without replacing the referenced definition.
- **Override:** deliberately replaces or supersedes a referenced definition and must identify the target being overridden.

HLA-PACKAGE SHALL NOT infer an override from name collision, load order, package priority, first match, last match, or package author identity.

---

<a id="8-effective-campaign-definition"></a>
# 8. Effective Campaign Definition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The Effective Campaign Definition is the composed package-defined semantic baseline used to instantiate or migrate a Campaign.

```text
EffectiveCampaignDefinition {
  effectiveDefinitionId
  compositionInputs[]
  resolvedPackageRefs[]
  systemIdentity?
  editionIdentity?
  composedDefinitions[]
  composedSchemas[]
  composedRuleContracts[]
  composedOracleContracts[]
  composedProcedureContracts[]
  composedScenarioContent?
  composedAssetRefs[]
  appliedExtensions[]
  appliedOverrides[]
  diagnostics[]
  validationDecision
}
```

The Effective Campaign Definition SHALL be:

- Derived from registered Package artifacts and explicit composition inputs.
- Validated before Campaign instantiation or migration.
- Persistently identifiable through a per-Campaign composition pin.
- Reconstructible from pinned Package versions and explicit composition inputs.
- Independent from any one concrete client or authoring tool.

The Effective Campaign Definition SHALL NOT:

- Become Campaign Reality by itself.
- Mutate a running Campaign without HLA-LIFECYCLE or migration flow.
- Replace HLA-CORE Event history.
- Hide unresolved composition conflicts.

---

<a id="9-composition-pins-and-core-metadata"></a>
# 9. Composition Pins and Core Metadata

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE resolves the HLA-CORE deferred metadata question as follows.

HLA-CORE Events SHALL retain stable semantic references to package-defined meaning, while HLA-PACKAGE owns the interpretation of those references.

Minimum semantic reference:

```text
PackageSemanticRef {
  packageIdentity
  packageVersion
  semanticId
  semanticKind
  effectiveDefinitionId?
}
```

`semanticKind` MAY include:

- `entityType`
- `locationType`
- `relationshipType`
- `propertyType`
- `stateType`
- `factType`
- `rule`
- `oracle`
- `procedure`
- `schema`
- `asset`
- `scenarioElement`

HLA-CORE SHALL retain `PackageSemanticRef` values in Events when package-defined types, schemas, or contracts give meaning to Event payloads. HLA-CORE SHALL NOT copy full Package definitions into every Event by default.

The composition pin SHALL preserve enough information to resolve `PackageSemanticRef` values later:

```text
CompositionPin {
  campaignRef
  effectiveDefinitionId
  pinnedPackageRefs[]
  compositionInputs[]
  appliedExtensions[]
  appliedOverrides[]
  createdBy
  createdAt
}
```

This design balances reconstructibility and duplication:

- Events carry stable semantic references.
- Composition pins identify the exact package set and composition context.
- HLA-PACKAGE resolves semantic references against the pinned Effective Campaign Definition.
- HLA-PERSIST preserves pins and required Package references for export/import.
- Forced Package removal may make semantic interpretation unavailable, but SHALL NOT corrupt HLA-CORE Event history.

If later HLA-PACKAGE migration design shows that some semantic definitions must be embedded or snapshotted for long-term interpretation, that SHALL be handled as an explicit migration/export rule rather than an implicit HLA-CORE behavior.

---

<a id="10-migration-and-removal"></a>
# 10. Migration and Removal

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Package migration is explicit and atomic.

```text
MigrationPlan {
  migrationId
  campaignRef
  fromCompositionPin
  toCompositionRequest
  proposedCompositionPin?
  compatibilityDiagnostics[]
  requiredTransformations[]
  rollbackPreconditions[]
  validationDecision
}
```

Migration SHALL:

- Start from the Campaign's current composition pin.
- Produce a proposed target composition.
- Identify definition, schema, contract, asset, and dependency changes.
- Identify package-defined semantic references that may become unresolved.
- Submit the migration plan to HLA-VALIDATE.
- Apply atomically or leave the previous composition pin intact.
- Preserve provenance sufficient for NFR-006 and FR-041.

HLA-PACKAGE owns migration planning. Applying Campaign Reality transformations caused by migration must coordinate with HLA-LIFECYCLE, HLA-STATE, HLA-RESOLUTION, and HLA-CORE as later designs require. HLA-PACKAGE SHALL NOT directly append Campaign Reality Events.

Package removal SHALL be dependency-aware:

- Referenced Package versions SHALL NOT be removed by default if current, inactive, archived, or exported Campaigns still reference them.
- Rejection diagnostics SHALL identify referencing Campaigns or archives where known.
- Forced removal SHALL require explicit acknowledgement through HLA-CONTRACT authorization and diagnostics.
- Forced removal may make semantic interpretation unavailable for affected Campaigns, but it SHALL NOT delete HLA-CORE Event history.

---

<a id="11-interface-contracts"></a>
# 11. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Public access occurs only through HLA-CONTRACT. HLA-PACKAGE provides internal component interfaces.

```text
registerPackage(packageArtifact) -> PackageRegistrationResult
compose(request: CompositionRequest) -> EffectiveCampaignDefinitionResult
pinComposition(campaignRef, effectiveDefinitionId) -> CompositionPin
getCompositionPin(campaignRef) -> CompositionPin
planMigration(campaignRef, request: CompositionRequest) -> MigrationPlan
applyMigrationPlan(migrationId) -> MigrationResult
removePackage(packageRef, force?) -> PackageRemovalResult
resolveSemanticRef(ref: PackageSemanticRef, pin: CompositionPin) -> PackageSemanticDefinition
getOracleContract(oracleRef, pin) -> OracleContract
getProcedureContract(procedureRef, pin) -> ProcedureContract
getRuleContract(ruleRef, pin) -> RuleContract
```

Mutation operations SHALL require HLA-CONTRACT capability checks before forwarding. HLA-PACKAGE SHALL assume that internal callers supply already-authorized requests, but HLA-PACKAGE SHALL still validate package/composition subjects through HLA-VALIDATE.

---

<a id="12-validation-and-diagnostics"></a>
# 12. Validation and Diagnostics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE SHALL invoke HLA-VALIDATE for:

- `packageArtifact` before registration.
- `effectiveCampaignDefinition` before Campaign instantiation or migration use.
- `migrationPlan` before package migration application.

Diagnostics SHALL identify:

- Missing dependencies.
- Unsatisfied required capabilities.
- Version conflicts.
- Composition collisions.
- Missing explicit overrides.
- Invalid extension targets.
- Invalid override targets.
- Incompatible system/edition metadata.
- Unresolvable semantic references.
- Non-portable or missing package-sourced assets.
- Migration rollback precondition failures.

Package-contributed validators remain declarative validation contracts or declarative rules interpreted by engine-owned validators. HLA-PACKAGE SHALL NOT execute package-supplied validator code in the current design.

---

<a id="13-asset-and-portability-boundary"></a>
# 13. Asset and Portability Boundary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE owns package-sourced asset declarations and semantic asset references. HLA-PERSIST owns durable payload storage, export/import representation, and portability verification.

Package artifacts MAY declare assets:

```text
PackageAssetDeclaration {
  assetId
  packageRef
  assetRole
  assetTypeHint?
  portabilityClass
  provenance
}
```

HLA-PACKAGE SHALL expose package-sourced asset declarations to HLA-PERSIST so Campaign export can verify that referenced package-sourced assets are independently portable or obtainable through composition pins.

HLA-PACKAGE SHALL NOT require HLA-CORE or HLA-OBSERVER to embed package asset payloads in authoritative state.

---

<a id="14-failure-semantics"></a>
# 14. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Failure Trigger | Required Behavior |
|---|---|
| Malformed Package artifact | Reject registration; return validation diagnostics. |
| Duplicate Package identity/version registration | Reject or report already-registered status without replacing existing version silently. |
| Missing dependency | Reject composition or registration context requiring that dependency. |
| Version conflict | Reject composition with explicit conflict diagnostics. |
| Collision without explicit override/extension | Reject composition. |
| Invalid override or extension target | Reject composition. |
| Effective Campaign Definition fails validation | Reject Campaign use or migration use. |
| Migration plan fails validation | Leave existing composition pin intact. |
| Migration application fails | Leave prior composition pin fully intact; no mixed composition state. |
| Package version referenced by Campaign or archive | Reject removal unless forced with acknowledgement. |
| Forced removal of referenced Package | Preserve Event history and pins; mark semantic interpretation availability degraded with diagnostics. |
| Package-sourced asset unavailable | Report diagnostics to HLA-PERSIST/export flow; Campaign export may fail per HLA-PERSIST rules. |

---

<a id="15-non-functional-derivation"></a>
# 15. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-PACKAGE Design Response |
|---|---|
| NFR-002 Security | Package artifacts, composed definitions, and migration plans pass HLA-VALIDATE before admission. |
| NFR-003 Reliability | Atomic migration posture leaves prior composition pins intact on failure. |
| NFR-005 Maintainability | Microkernel boundary keeps package-defined semantics separate from engine mechanics; structured package data avoids embedding RPG-system meaning in core code. |
| NFR-006 Auditability | Composition pins, applied overrides/extensions, migration plans, and removal diagnostics preserve provenance for package-related changes. |

No quantitative performance target is introduced here.

---

<a id="16-testing-alignment"></a>
# 16. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL include HLA-PACKAGE coverage for:

- Registering two versions of the same Package identity.
- Registering Packages with independent system/edition metadata.
- Composing an Effective Campaign Definition from explicit inputs.
- Rejecting collisions without explicit override/extension.
- Applying valid extensions.
- Applying valid overrides.
- Rejecting invalid override targets.
- Pinning Campaigns to exact Effective Campaign Definitions.
- Ensuring newly registered Package versions do not affect existing Campaign pins.
- Planning and validating explicit migrations.
- Failing migration atomically without changing the prior pin.
- Preventing removal of referenced Package versions by default.
- Forced removal diagnostics and degraded semantic interpretation behavior.
- Resolving `PackageSemanticRef` values against a composition pin.
- Preserving HLA-CORE Event reconstructibility through semantic references plus pins.
- Exposing package-sourced asset declarations to HLA-PERSIST.

---

<a id="17-traceability-summary"></a>
# 17. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Design Coverage |
|---|---|
| FR-019 | `PackageRegistry`, `PackageArtifact`, registration interface. |
| FR-020 | `CompositionBuilder`, `CompositionRequest`, `EffectiveCampaignDefinition`. |
| FR-021 | `ConflictDetector`, explicit override/extension rules, failure semantics. |
| FR-022 | `CompositionPin`, `PinManager`, per-Campaign pinning. |
| FR-023 | `MigrationPlan`, migration validation, atomic migration posture. |
| FR-024 | Multiple versions of same Package identity in `PackageRegistry`. |
| FR-025 | `RemovalGuard`, referenced-package removal diagnostics, force acknowledgement. |
| FR-026 | `systemIdentity` and `editionIdentity` metadata independent of `packageVersion`. |
| FR-037 | HLA-VALIDATE invocation for package artifacts, composed definitions, migration plans. |
| FR-038 | Structured diagnostics for registration, composition, migration, and removal. |
| FR-041 | Atomic migration posture. |
| FR-042 | Package-sourced asset declarations exposed for portability verification. |

---

<a id="18-open-questions"></a>
# 18. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

No HLA-PACKAGE-owned open questions remain at this draft level.

Deferred package-format decision set:

The following items are intentionally deferred together because resolving one in isolation would prematurely constrain the others:

- Concrete package artifact syntax.
- Concrete schema/contract language.
- Rule, Procedure, and Oracle declaration/execution form.
- Long-term semantic definition retention strategy.
- Migration transformation representation and application path.

Current design commitments:

- Package artifacts SHALL be structured.
- Package contracts SHALL be declarative by default.
- Package-supplied executable validators are out of scope.
- Package content SHALL NOT access engine internals.
- HLA-VALIDATE SHALL be able to validate Package artifacts, Effective Campaign Definitions, Oracle results, and migration plans without arbitrary package code execution.
- HLA-CORE SHALL store stable `PackageSemanticRef` values, not full package definitions per Event by default.
- Composition pins SHALL preserve the Effective Campaign Definition context.
- Any later package-format decision SHALL preserve portability, deterministic parsing, validation, diffability, migration support, semantic reconstructibility, and runtime-language independence.

Resolved during Detailed Design review:

- Composition language — HLA-PACKAGE SHALL use structured package data for v1. A separate declarative composition language is deferred unless structured data proves insufficient for an approved requirement.
- Concrete package artifact syntax — HLA-PACKAGE requires structured package artifacts, but the concrete syntax is deferred to package-format SRD or implementation planning. Candidate syntaxes SHALL be evaluated against portability, validation, authoring ergonomics, diffability, schema expressiveness, deterministic parsing, asset packaging, and import/export compatibility. No component SHALL depend on JSON, YAML, XML, archive layout, database representation, or another concrete syntax before that decision is made.
- HLA-CORE reconstructibility metadata — HLA-CORE Events retain stable `PackageSemanticRef` values while composition pins preserve the exact Effective Campaign Definition context. HLA-PACKAGE owns interpretation of those references; HLA-CORE does not duplicate full Package definitions into every Event by default.

---

<a id="19-phase-gate-declaration"></a>
# 19. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE Detailed Design is **approved at the component-design level**.

This approval does not authorize implementation by itself. It approves the HLA-PACKAGE Detailed Design baseline for continued Detailed Design work and later planning.

This design establishes package identity/versioning, structured package artifact concepts, explicit composition, composition pins, migration/removal posture, and the semantic reference model needed by HLA-CORE. No HLA-PACKAGE-owned open questions remain at this draft level; concrete package format, schema/contract language, rule/procedure/oracle declaration form, long-term semantic retention, and migration transformation representation remain intentionally deferred as a package-format decision set.

Implementation, testing, package-format SRD work, or later component designs may reveal a need to revisit this design. Any material change SHALL be handled through the project's normal lifecycle change-control process.
