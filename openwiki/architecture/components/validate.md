---
type: architecture-component
title: "HLA-VALIDATE: Validation and Diagnostics"
description: "How HLA-VALIDATE gates every trust boundary (Packages, Campaign imports, Oracle results) through a Chain-of-Responsibility validator pipeline, treating externally supplied artifacts as potentially hostile."
tags: [architecture, hla, chain-of-responsibility, validation, trust-boundary, diagnostics]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-f26026492473cea3e9fda6ac
    resource: repo://docs/design/components/hla-validate-dd.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-VALIDATE: Validation and Diagnostics

HLA-VALIDATE is the engine's trust-boundary validation component. It evaluates Packages, Campaign imports, Oracle results, migration plans, and other admitted subjects **before they can influence authoritative state**, and returns structured diagnostics — without owning the state it protects.

**Security posture:** externally supplied artifacts are treated as untrusted and potentially *hostile*, not merely malformed. This explicitly includes Packages, Campaign imports, Oracle results, and artifacts generated or modified by LLMs or other external tools. AI is not an engine capability, but AI-generated artifacts arriving through public contracts receive no special trust.

## Why Chain of Responsibility

Validation rules are contributed by multiple sources — engine-level structural checks and Package-declared schema/contract checks — and must be composable without the engine needing a fixed, closed validation algorithm. **Chain of Responsibility** lets each validator handle what it knows about and pass the rest along, which is the right shape for a validation set that grows as Packages are added; a fixed algorithm (**Template Method**) would have to be rewritten every time a new kind of check was needed. **Visitor** was rejected for the same open-type-hierarchy reason as in [HLA-QUERY](query.md).

## Validation subjects

| Subject Type | Owning Caller | Required Validation Layers |
|---|---|---|
| `packageArtifact` | HLA-PACKAGE | Format, schema/contract, dependency, semantic, Package-declared |
| `effectiveCampaignDefinition` | HLA-PACKAGE | Dependency, conflict, override, semantic consistency |
| `campaignImportArtifact` | HLA-PERSIST | Format, schema/contract, identity, version, consistency, dependency |
| `oracleResult` | HLA-RESOLUTION | Envelope, Package-declared result contract, semantic fit to unresolved target |
| `migrationPlan` | HLA-PACKAGE / HLA-STATE | Dependency, version, compatibility, rollback precondition |
| `exportArtifact` | HLA-PERSIST | Format, completeness, manifest/reference consistency |

This taxonomy is deliberately limited to subjects visible in the approved HLA/SRS — adding a new subject type requires component-level justification and traceability.

## The validator chain

The default chain runs in a fixed order: **envelope → format → schema/contract → dependency → semantic → cross-reference → policy → Package-contributed validators.** Package-contributed validators execute only through declarative validation contracts or declarative rules interpreted by engine-owned validators — they never receive engine-core access and are never Package-supplied executable code.

Stop/continue rules matter as much as the order: unparseable subjects stop the chain after format diagnostics; missing required schema fields stop semantic and cross-reference validators for those fields; dependency failures may continue into further dependency checks but always prevent semantic acceptance; fatal diagnostics always produce `reject`. HLA-VALIDATE never relies on validator ordering to hide a missing check — each validator declares its own prerequisites and handled subject types explicitly.

## Strict admission: `acceptWithWarnings` never admits state

`decision` is one of `accept`, `acceptWithWarnings`, `reject`, or `indeterminate`. This is the component's central rule: **state-admitting trust boundaries require strict `accept`.** Package registration, Effective Campaign Definition use, Campaign import, migration, and Oracle result acceptance never proceed on `acceptWithWarnings` — that decision is permitted only for advisory validation or non-admitting verification flows (e.g., a caller checking a Package for problems without registering it). An `indeterminate` decision also blocks application unless a later design explicitly defines a safe degraded path — none is defined here.

## Diagnostic model

Every `ValidationDiagnostic` carries a `severity` (`info`/`warning`/`error`/`fatal`) and a `category` (`format`/`schema`/`contract`/`dependency`/`semantic`/`crossReference`/`conflict`/`trustBoundary`/`internalValidatorFailure`). At minimum, every `error` or `fatal` diagnostic identifies a subject or affected reference, a category, and a human-readable message — if the affected object itself can't be identified because the artifact is malformed, the diagnostic identifies the smallest known container instead (the import artifact or Package artifact as a whole). Diagnostics are explicitly **not Campaign history** — if an accepted operation later needs provenance, that's the owning component's responsibility, not HLA-VALIDATE's.

`validatorTrace` (full validator-by-validator execution detail) is exposed only when the caller requests `diagnosticDetail = trace` **and** HLA-CONTRACT authorizes trace detail for that caller/context — it's debug/support data, not the stable user-facing diagnostic contract.

## Trust boundary flows

Three flows share the same shape: the owning component authorizes/prepares, submits the subject to HLA-VALIDATE, and only proceeds on the returned decision. Notably in the **Oracle result flow**: if HLA-VALIDATE rejects the result, [HLA-RESOLUTION](resolution.md) leaves the associated element Unresolved and returns diagnostics — it never synthesizes a fallback value. A fourth **public validation flow** lets a caller request validation through HLA-CONTRACT directly; this remains strictly read-only and never registers, imports, migrates, or resolves anything by itself.

## Dependency relationships

- **Invoked by** [HLA-PACKAGE](package.md) (Package registration/composition), [HLA-PERSIST](persist.md) (Campaign import), and [HLA-RESOLUTION](resolution.md) (Oracle result validation) — HLA-VALIDATE has **no dependency on any other component's internals** beyond the subject being validated; other components depend on it, not the reverse.
- Owns **no persistent authoritative data** — validation is a stateless pass over supplied content.

## Failure semantics

The pattern across every failure trigger is the same: `reject` (or `indeterminate`) with **no state change** and a diagnostic identifying what failed. An unknown subject type, a malformed/unparseable subject, a missing declared dependency, an unresolvable Package-contributed validator, or an Oracle result violating its declared contract all reject outright rather than partially admitting the subject. HLA-VALIDATE does not retry validators by default, and retry behavior for unavailable Package-declared validators can never result in silent acceptance.

## Non-functional derivation

- **NFR-002 (Security):** mandatory validation gates for Package content, Campaign import artifacts, and Oracle results — failed validation always prevents state influence.
- **NFR-005 (Maintainability):** Chain of Responsibility isolates validators by subject type and responsibility instead of centralizing all logic in one validator.
- **NFR-006 (Auditability):** structured diagnostics can be attached by owning components to rejected operations or provenance records where appropriate.
