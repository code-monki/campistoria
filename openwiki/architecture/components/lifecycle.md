---
type: architecture-component
title: "HLA-LIFECYCLE: Campaign Lifecycle and Scenario Instantiation"
description: "How HLA-LIFECYCLE creates a mutable Campaign from an immutable Scenario via Factory Method, assigns permanent identity/seed, and atomically establishes the initial Campaign Authority Binding."
tags: [architecture, hla, factory-method, campaign-lifecycle, scenario-instantiation]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-fe76efa140cc0bb8ac40b6c1
    resource: repo://docs/design/components/hla-lifecycle-dd.md
  - id: openwiki-source-615487f2876b472d6b4fadc3
    resource: repo://docs/project/decision-log.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-LIFECYCLE: Campaign Lifecycle and Scenario Instantiation

HLA-LIFECYCLE owns the controlled creation of a mutable Campaign from an immutable Scenario. It assigns and preserves Campaign identity and seed metadata, records the Scenario and Effective Campaign Definition (ECD) basis for instantiation, and guarantees that Campaigns created from the same Scenario diverge independently without ever mutating the Scenario itself.

## Why Factory Method

Campaign creation is the canonical Factory Method scenario: a creation operation (`createCampaign(scenarioRef) -> CampaignId`) that produces a new object (Campaign) from a template (Scenario) without exposing construction internals to the caller, and without the Scenario itself being mutated or consumed in the process (FR-040).

Two alternatives were considered and rejected. **Prototype** was rejected because Prototype clones an existing instance of the *same* type to make a new one, but a Scenario is not a Campaign instance and is not "cloned into" a Campaign — it is a distinct, immutable template type, which breaks Prototype's core assumption. **Builder** was rejected as the *primary* pattern here (it is used instead in [HLA-PACKAGE](package.md) for composing the Effective Campaign Definition), because Campaign instantiation from an already-resolved Scenario is a single creation step, not a multi-step assembly.

## Internal decomposition

| Module | Responsibility | Not responsible for |
|---|---|---|
| `CampaignFactory` | Orchestrates instantiation from validated Scenario/ECD input | Post-creation play mutation |
| `ScenarioReferenceResolver` | Resolves Scenario references through HLA-PACKAGE, captures immutable Scenario basis | Package artifact ownership or mutation |
| `InstantiationValidator` | Ensures the ECD and Scenario basis are accepted by HLA-VALIDATE before creation | Owning validator implementations |
| `CampaignIdentityAssigner` | Produces and verifies permanent Campaign identity | Selecting a concrete ID library |
| `CampaignSeedAssigner` | Produces, records, and exposes stable Campaign seed metadata | Guaranteeing package Oracle reproducibility |
| `InitialRealityInitializer` | Translates validated scenario starting state into HLA-CORE initialization requests | Owning Campaign Reality after initialization |
| `LifecycleMetadataRegistry` | Maintains identity, seed, Scenario/ECD references, and creation provenance | Durable storage mechanics |
| `InstantiationTransactionCoordinator` | Ensures creation commits all required metadata atomically or has no effect | HLA-STATE recovery algorithms or HLA-PERSIST storage representation |

## Campaign creation flow

1. HLA-CONTRACT authorizes `campaign.create`.
2. HLA-LIFECYCLE resolves the requested Scenario reference through [HLA-PACKAGE](package.md).
3. HLA-PACKAGE composes an Effective Campaign Definition for `campaignInstantiation`.
4. [HLA-VALIDATE](validate.md) accepts the ECD and Scenario basis.
5. HLA-LIFECYCLE assigns a new, permanent Campaign identity.
6. HLA-LIFECYCLE assigns a Campaign seed — from caller-supplied creation seed metadata if provided, otherwise generated.
7. HLA-LIFECYCLE records immutable lifecycle metadata: identity, seed, Scenario reference, ECD reference, package composition pin, creation source, and creation time.
8. HLA-CONTRACT prepares the initial active **Campaign Authority Binding** between the new Campaign and the creating/default local Principal with the `campaignOwner` bundle.
9. HLA-LIFECYCLE asks [HLA-CORE](core.md) to initialize Campaign Reality from the validated scenario starting state.
10. HLA-PERSIST records the Campaign, lifecycle metadata, and authority binding durably.
11. The creation transaction commits the Campaign and initial authority binding together; HLA-LIFECYCLE returns the result through HLA-CONTRACT.

**The creation operation is atomic at the component-contract level:** either a Campaign exists with identity, seed, lifecycle metadata, package composition basis, an initial Campaign Reality boundary, *and* an active local Campaign Authority Binding — or no authoritative Campaign exists at all. This atomicity requirement was added by decision [DEC-001](../../reference/decision-log.md) ("Campaign-Scoped Principal Authority Binding"), which requires the initial `campaignOwner` binding to be established atomically with Campaign creation rather than as a separate, potentially-skippable step — a caller-supplied role or an implicit local-process assumption is not sufficient authority.

## Campaign identity

Identity is **permanent and unique** for the Campaign's lifecycle: `campaignRef` is assigned at creation, never equals or is derived solely from the Scenario identity, and remains stable across persistence, save/reload, export/import, and package migration. Identity collision detection fails closed before any authoritative Campaign state is admitted. The concrete identity format (UUID-like, content-addressed, composite, or otherwise) is deliberately deferred — only the stability invariants are fixed at this design level.

## Campaign seed

Every newly created Campaign has a stable `CampaignSeed`, either caller-supplied (recorded with `seedSource` for diagnostics) or engine-generated, immutable after creation, and exposed through an authorized public contract operation as ordinary Campaign metadata. Critically, the seed is **not** a guarantee of Oracle reproducibility: HLA-LIFECYCLE explicitly does not provide the raw Campaign seed as a repeated PRNG seed for every Oracle invocation, does not guarantee reproducible output for Oracles drawing on physical/real-world randomness, and does not define a cross-implementation PRNG algorithm. Random variation for Oracle calls instead comes from an invocation-specific random context *derived from* the Campaign seed plus invocation-specific data (Oracle identity, invocation sequence, Campaign context) — HLA-LIFECYCLE owns only the stable root seed; [HLA-RESOLUTION](resolution.md) and the Oracle Adapter own invocation provenance and random-context handoff.

## Scenario immutability and independent divergence

Scenario content is immutable authored input owned through HLA-PACKAGE. HLA-LIFECYCLE treats it strictly as Campaign-creation input, never as mutable Campaign state, and rejects any Campaign-facing attempt to modify it. Running play mutates the Campaign, never the Scenario.

Multiple Campaigns instantiated from the same Scenario diverge **independently**: each has its own Campaign identity, its own Campaign seed, its own HLA-CORE Event stream, its own HLA-OBSERVER Observer Knowledge streams, and its own package composition pin. A mutation to one never propagates to another, even though the Scenario itself remains shared, immutable input. This is not a branching or version-control mechanism — a future clone/fork/branch operation, if approved, would need its own explicit lifecycle design covering identity, seed, provenance, and synchronization implications rather than reusing ordinary instantiation semantics.

## Failure semantics

HLA-LIFECYCLE fails closed before admitting any partial Campaign state. Notably: if the initial Campaign Authority Binding cannot be established or persisted, creation is rejected atomically and no authoritative Campaign is admitted; if initial HLA-CORE initialization fails, lifecycle metadata admission is rolled back or marked non-authoritative rather than returning a partially-created Campaign. Package composition changes after Campaign creation never automatically propagate to an existing Campaign — that requires an explicit migration flow coordinated with HLA-PACKAGE.

## Dependency relationships

- **Depends on** [HLA-PACKAGE](package.md) for Scenario and Effective Campaign Definition inputs, and on HLA-VALIDATE to accept that ECD/Scenario basis before instantiation.
- **Depends on** [HLA-CORE](core.md) to initialize Campaign Reality.
- **Invoked externally only through** HLA-CONTRACT — no public caller reaches HLA-LIFECYCLE directly.
- Owns Campaign identity and seed metadata; does **not** own Campaign Reality once created (HLA-CORE does).

## Non-functional derivation

- **NFR-002 (Security):** Campaign creation is gated by HLA-CONTRACT authorization and HLA-VALIDATE admission before untrusted package/scenario content affects state.
- **NFR-003 (Reliability):** instantiation is atomic — identity, seed, lifecycle metadata, and the initial HLA-CORE boundary are created together or not at all.
- **NFR-005 (Maintainability):** lifecycle creation stays separated from package composition, validation, persistence, migration, and Campaign Reality ownership.
- **NFR-006 (Auditability):** lifecycle metadata records the Scenario reference, ECD reference, composition pin, seed metadata, and creation source/time as creation provenance.
