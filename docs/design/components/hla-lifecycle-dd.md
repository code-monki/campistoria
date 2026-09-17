# HLA-LIFECYCLE Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-LIFECYCLE
Version: 1.0 (Component Baseline)
Date (YYYY-MM-DD): 2026-09-15
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
- [6. Campaign Creation Model](#6-campaign-creation-model)
- [7. Campaign Identity Model](#7-campaign-identity-model)
- [8. Campaign Seed Model](#8-campaign-seed-model)
- [9. Scenario Immutability Boundary](#9-scenario-immutability-boundary)
- [10. Independent Divergence Model](#10-independent-divergence-model)
- [11. Package and Migration Boundary](#11-package-and-migration-boundary)
- [12. Creation Provenance](#12-creation-provenance)
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
- Architectural Component ID stable? **Yes** — HLA-LIFECYCLE.
- Parent Detailed Design phase opened? **Yes** — [Engine Detailed Design](../engine-detailed-design.md) v1.0 candidate.
- Advancement to implementation authorized? **No.**

This document refines HLA-LIFECYCLE only. It does not authorize implementation, introduce a new architectural component, select an identity-generation algorithm, or select a random-number generation algorithm.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-LIFECYCLE — Campaign Lifecycle & Scenario Instantiation.

**Component purpose:** HLA-LIFECYCLE owns the controlled creation of a mutable Campaign from an immutable Scenario. It assigns and preserves Campaign identity and seed metadata, records the Scenario and Effective Campaign Definition basis for instantiation, and ensures that Campaigns created from the same Scenario diverge independently without mutating the Scenario.

**Primary requirements:** FR-001, FR-002, FR-003, FR-040, FR-044.

**Related requirements/NFRs:** FR-019 through FR-024, FR-031, FR-032, FR-041, NFR-002, NFR-003, NFR-005, NFR-006.

**In scope:**

- Campaign instantiation from a validated Scenario.
- Campaign identity assignment and lifecycle invariants.
- Campaign seed assignment, persistence boundary, and public retrieval posture.
- Immutable Scenario reference capture.
- Initial Campaign creation metadata.
- Coordination with HLA-PACKAGE for Effective Campaign Definition resolution.
- Coordination with HLA-VALIDATE for admission of the Effective Campaign Definition before instantiation.
- Coordination with HLA-CORE to initialize Campaign Reality from scenario-derived starting state.
- Independent divergence guarantees for Campaigns instantiated from the same Scenario.
- Failure behavior for invalid scenarios, identity collision, seed generation failure, validation failure, and partial instantiation.

**Out of scope:**

- Owning Package artifacts, Scenario authored content, or Effective Campaign Definition composition, which belong to HLA-PACKAGE.
- Owning Campaign Reality after initial creation, which belongs to HLA-CORE.
- Owning Observer Knowledge, which belongs to HLA-OBSERVER.
- Public authorization envelope semantics and capability checks, which belong to HLA-CONTRACT.
- Durable storage, export/import representation, and migration rollback mechanics, which belong to HLA-PERSIST and HLA-STATE.
- Package migration planning, which belongs to HLA-PACKAGE.
- Package-defined Oracle reproducibility beyond exposing the stable Campaign seed.
- Branching campaigns or alternate timelines beyond the independent-Campaign divergence semantics required by FR-003.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-LIFECYCLE SHALL preserve the approved HLA constraints:

- It uses a Factory Method style creation boundary: `createCampaign(scenarioRef) -> CampaignId`.
- It depends on HLA-PACKAGE for Scenario and Effective Campaign Definition inputs.
- It depends on HLA-VALIDATE before admitting a Scenario/ECD for Campaign instantiation.
- It depends on HLA-CORE to initialize Campaign Reality.
- It is invoked externally only through HLA-CONTRACT.
- It owns Campaign identity and seed metadata.
- It does not own Campaign Reality after creation.
- It does not mutate Scenario authored content.
- It does not implement Scenario upgrade or Package migration as ordinary instantiation.
- It does not select a concrete identity-generation library, PRNG algorithm, storage technology, or serialization format.

Any design that mutates Scenario content during Campaign creation, derives Campaign identity from Scenario identity, lets multiple Campaigns share mutable Campaign Reality, or treats the Campaign seed as a guarantee of cross-implementation Oracle reproducibility violates approved Requirements and Architecture.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-LIFECYCLE SHALL:

- Accept authorized Campaign creation requests from HLA-CONTRACT.
- Request or consume a validated Effective Campaign Definition for the requested Scenario.
- Create a permanent Campaign identity distinct from the Scenario identity.
- Assign a stable Campaign seed at creation.
- Record immutable Scenario and package composition basis for the Campaign.
- Coordinate initial Campaign Reality creation with HLA-CORE.
- Ensure instantiation either succeeds completely or has no authoritative effect.
- Preserve identity and seed invariants across save/reload, export/import, and package migration by requiring HLA-PERSIST/HLA-PACKAGE/HLA-STATE flows to retain them.
- Reject attempts to modify Scenario authored content through Campaign-facing lifecycle operations.
- Support retrieval of Campaign identity, Scenario basis, and Campaign seed through authorized HLA-CONTRACT operations.

HLA-LIFECYCLE SHALL NOT:

- Modify Scenario authored content.
- Treat a Scenario as a Campaign clone or mutable prototype.
- Reuse Scenario identity as Campaign identity.
- Share mutable Campaign Reality between Campaigns instantiated from the same Scenario.
- Append ordinary play Events after instantiation; those flow through HLA-RESOLUTION and HLA-CORE.
- Own Package registration, composition, or migration planning.
- Own durable persistence, import/export representation, or rollback mechanics.
- Guarantee Oracle output reproducibility beyond stable seed assignment and exposure.

---

<a id="5-internal-decomposition"></a>
# 5. Internal Decomposition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Conceptual internal modules:

| Module | Responsibility | Not Responsible For |
|---|---|---|
| `CampaignFactory` | Orchestrates Campaign instantiation from validated Scenario/ECD input. | Post-creation play mutation. |
| `ScenarioReferenceResolver` | Resolves requested Scenario references through HLA-PACKAGE and captures immutable Scenario basis. | Package artifact ownership or mutation. |
| `InstantiationValidator` | Ensures the Effective Campaign Definition and Scenario basis are accepted by HLA-VALIDATE before creation. | Owning validator implementations. |
| `CampaignIdentityAssigner` | Produces and verifies permanent Campaign identity. | Selecting implementation-specific ID library in this phase. |
| `CampaignSeedAssigner` | Produces, records, and exposes stable Campaign seed metadata. | Guaranteeing package Oracle reproducibility. |
| `InitialRealityInitializer` | Translates validated scenario starting state into HLA-CORE initialization requests. | Owning Campaign Reality after initialization. |
| `LifecycleMetadataRegistry` | Maintains Campaign identity, seed, Scenario reference, ECD reference, and creation provenance metadata. | Durable storage mechanics. |
| `InstantiationTransactionCoordinator` | Ensures creation either commits all required lifecycle/Core metadata or has no authoritative effect. | HLA-STATE recovery algorithms or HLA-PERSIST storage representation. |

---

<a id="6-campaign-creation-model"></a>
# 6. Campaign Creation Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Conceptual creation request:

```text
CreateCampaignRequest {
  requestedScenarioRef
  requestedPackageRefs[]
  requestedOverrides[]
  creationLabel?
  creationSeedMetadata?
  callerContext
}
```

Conceptual creation result:

```text
CreateCampaignResult {
  campaignRef
  scenarioRef
  effectiveDefinitionRef
  packageCompositionPin
  campaignSeed
  initialCoreBoundary
  diagnostics[]
}
```

Instantiation flow:

1. HLA-CONTRACT authorizes `campaign.create`.
2. HLA-LIFECYCLE resolves `requestedScenarioRef` through HLA-PACKAGE.
3. HLA-PACKAGE composes an Effective Campaign Definition for `campaignInstantiation`.
4. HLA-VALIDATE accepts the Effective Campaign Definition and Scenario basis.
5. HLA-LIFECYCLE assigns a new Campaign identity.
6. HLA-LIFECYCLE assigns a Campaign seed from creation seed metadata when supplied, or generates one when absent.
7. HLA-LIFECYCLE records immutable lifecycle metadata: Campaign identity, seed, Scenario reference, ECD reference, package composition pin, creation source, and creation time.
8. HLA-LIFECYCLE asks HLA-CORE to initialize Campaign Reality from the validated scenario starting state.
9. HLA-PERSIST durable backing records the created Campaign through its normal persistence boundary.
10. HLA-LIFECYCLE returns the creation result through HLA-CONTRACT.

The creation operation SHALL be atomic at the component-contract level: either a Campaign exists with identity, seed, lifecycle metadata, package composition basis, and initial Campaign Reality boundary, or no authoritative Campaign exists.

---

<a id="7-campaign-identity-model"></a>
# 7. Campaign Identity Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Campaign identity is permanent and unique for the Campaign lifecycle.

Conceptual identity metadata:

```text
CampaignIdentity {
  campaignRef
  createdAt
  createdBy?
  scenarioRef
  effectiveDefinitionRef
  packageCompositionPin
  lifecycleVersion
}
```

Identity invariants:

- `campaignRef` SHALL be assigned during Campaign creation.
- `campaignRef` SHALL NOT equal the Scenario identity.
- `campaignRef` SHALL NOT be derived solely from Scenario identity.
- `campaignRef` SHALL remain stable across persistence, save/reload, export/import, and package migration.
- HLA-PERSIST import SHALL preserve Campaign identity for an imported Campaign representation unless a future explicit clone/fork operation is approved.
- Package migration SHALL NOT change Campaign identity.
- Identity collision detection SHALL fail closed before any authoritative Campaign state is admitted.

The concrete identity format is deferred. It may later be UUID-like, content-addressed with collision safeguards, composite, or another stable identifier form, provided the invariants above are preserved.

---

<a id="8-campaign-seed-model"></a>
# 8. Campaign Seed Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The Campaign seed is stable Campaign metadata assigned at creation and exposed through public contracts for package-defined Oracles that choose to consume it.

Conceptual seed metadata:

```text
CampaignSeed {
  campaignRef
  seedValue
  seedVersion
  seedSource
  sourceMetadataRef?
  assignedAt
}
```

Seed invariants:

- Every newly created Campaign SHALL have a seed value.
- HLA-LIFECYCLE SHALL accept caller-supplied Campaign seed metadata during Campaign creation when the validated campaign/scenario creation input supplies it.
- If creation seed metadata is absent, HLA-LIFECYCLE SHALL generate the seed.
- The seed value SHALL remain stable across save/reload and export/import.
- The seed value SHALL be immutable after Campaign creation.
- HLA-LIFECYCLE SHALL expose the seed through an authorized public contract operation as ordinary Campaign metadata.
- HLA-LIFECYCLE SHALL NOT require any package-defined Oracle to consume the seed.
- HLA-LIFECYCLE SHALL NOT treat caller-supplied seed metadata as a direct control path into Oracle execution.
- HLA-LIFECYCLE SHALL NOT provide the raw Campaign seed as the sole repeated PRNG seed for every Oracle invocation.
- HLA-LIFECYCLE SHALL NOT guarantee reproducible output for Oracles whose output originates from physical or real-world randomness.
- HLA-LIFECYCLE SHALL NOT define a cross-implementation PRNG algorithm as part of this Detailed Design.

Caller-supplied seed metadata is a Campaign creation input, not a post-creation mutation. It may originate from a campaign/scenario creation file or equivalent creation request metadata. HLA-LIFECYCLE SHALL record `seedSource`, and where available a source metadata reference or caller context, so later diagnostics can distinguish engine-generated seeds from caller-supplied seeds. The requirement is stable assignment and exposure, not a specific seed-generation algorithm.

Random variation for Oracle calls SHALL come from an invocation-specific random context derived from the Campaign seed and invocation-specific data such as Oracle identity, invocation identity or sequence, Campaign context, and optional approved entropy. HLA-LIFECYCLE owns the Campaign seed as the stable root metadata; HLA-RESOLUTION and the Oracle Adapter own Oracle invocation provenance and random-context handoff. Exact PRNG algorithms and cross-implementation sequence compatibility remain deferred to the future platform SRD or package-format SRD.

---

<a id="9-scenario-immutability-boundary"></a>
# 9. Scenario Immutability Boundary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Scenario content is immutable authored starting definition owned through HLA-PACKAGE.

HLA-LIFECYCLE SHALL:

- Treat Scenario content as input to Campaign creation, not mutable Campaign state.
- Record a stable Scenario reference and Effective Campaign Definition reference for the created Campaign.
- Reject Campaign-facing attempts to modify Scenario authored content.
- Ensure Campaign creation does not consume, alter, replace, or remove the Scenario.

Running play mutates the Campaign, not the Scenario.

Changes to package-sourced Scenario definitions after Campaign creation SHALL NOT automatically alter existing Campaign Reality. Scenario/package upgrades for an existing Campaign are potentially unsafe and SHALL be treated as explicit migration flows coordinated by HLA-PACKAGE, HLA-STATE, HLA-VALIDATE, HLA-PERSIST, HLA-RESOLUTION, and HLA-CORE as applicable.

---

<a id="10-independent-divergence-model"></a>
# 10. Independent Divergence Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Multiple Campaigns may be instantiated from the same Scenario. They SHALL diverge independently.

Independence invariants:

- Each Campaign has its own Campaign identity.
- Each Campaign has its own Campaign seed.
- Each Campaign has its own HLA-CORE Campaign Reality Event stream.
- Each Campaign has its own HLA-OBSERVER Observer Knowledge streams.
- Each Campaign has its own package composition pin or Effective Campaign Definition reference as recorded at creation or migration.
- Mutations to one Campaign SHALL NOT automatically propagate to another Campaign even when they share the same Scenario.
- Scenario-authored content remains shared only as immutable package-defined input, not as mutable Campaign state.

This design does not introduce branching campaigns, alternate timelines, or a version-control system for Campaigns. Ordinary Scenario instantiation is not branching. If a future approved clone/fork/branch operation is introduced, it SHALL be modeled as an explicit lifecycle operation and define identity, seed, provenance, source Campaign reference, Observer Knowledge, checkpoint, synchronization, persistence, and migration implications rather than reusing ordinary Scenario instantiation semantics.

Retcon remains a separate in-Campaign correction mechanism owned through HLA-STATE/HLA-RESOLUTION flow. Retcon SHALL NOT be treated as Campaign branching.

---

<a id="11-package-and-migration-boundary"></a>
# 11. Package and Migration Boundary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-LIFECYCLE consumes HLA-PACKAGE outputs; it does not compose packages itself.

For Campaign creation:

- HLA-PACKAGE owns Package registration, dependency resolution, Scenario resolution, Effective Campaign Definition composition, package composition pins, and migration planning.
- HLA-VALIDATE owns validation of the Effective Campaign Definition before use.
- HLA-LIFECYCLE owns the decision to create a Campaign only after the Scenario/ECD basis is accepted.
- HLA-CORE owns authoritative Campaign Reality after initialization.

For existing Campaign changes:

- Scenario/package upgrades are not ordinary lifecycle creation.
- Package migration SHALL preserve Campaign identity and seed.
- Failed migration rollback is governed by HLA-PACKAGE, HLA-STATE, HLA-PERSIST, HLA-VALIDATE, and owning state components.
- HLA-LIFECYCLE may expose Campaign lifecycle metadata needed by migration planning, but it SHALL NOT apply package migration by itself.

---

<a id="12-creation-provenance"></a>
# 12. Creation Provenance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Lifecycle metadata is sufficient creation provenance for v1.

HLA-LIFECYCLE SHALL record Campaign identity, Scenario reference, Effective Campaign Definition reference, package composition pin, seed value, seed source, source metadata reference where available, creation time, and caller context where available.

HLA-CORE initial Events remain the provenance boundary for initial Campaign Reality. HLA-LIFECYCLE SHALL NOT create a separate lifecycle provenance record for v1 unless later Test Planning, audit requirements, or implementation evidence demonstrate that lifecycle metadata is insufficient.

---

<a id="13-interface-contracts"></a>
# 13. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Public callers do not invoke HLA-LIFECYCLE directly. HLA-CONTRACT forwards authorized requests.

Conceptual internal operations:

```text
createCampaign(request: CreateCampaignRequest) -> CreateCampaignResult
getCampaignIdentity(campaignRef) -> CampaignIdentity
getCampaignSeed(campaignRef) -> CampaignSeed
getCampaignLifecycleMetadata(campaignRef) -> CampaignLifecycleMetadata
```

Conceptual lifecycle metadata:

```text
CampaignLifecycleMetadata {
  campaignRef
  scenarioRef
  effectiveDefinitionRef
  packageCompositionPin
  campaignSeedRef
  createdAt
  createdBy?
  lifecycleVersion
  diagnostics[]
}
```

`createCampaign` requires `campaign.create`.

`getCampaignIdentity`, `getCampaignSeed`, and `getCampaignLifecycleMetadata` are read operations and require query or inspection capabilities as defined by HLA-CONTRACT. Seed exposure SHALL NOT bypass authorization, but it is not treated as Campaign Reality or Observer Knowledge.

---

<a id="14-failure-semantics"></a>
# 14. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-LIFECYCLE SHALL fail closed before admitting partial Campaign state.

| Failure Trigger | Required Behavior |
|---|---|
| Caller lacks `campaign.create` | Reject through HLA-CONTRACT before HLA-LIFECYCLE execution where possible. |
| Scenario reference cannot be resolved | Reject Campaign creation with diagnostics; no Campaign identity is admitted. |
| Scenario/ECD validation fails | Reject Campaign creation; no Campaign identity or Campaign Reality is admitted. |
| Scenario content mutation requested through Campaign-facing lifecycle API | Reject or no-op with diagnostics; Scenario content remains unchanged. |
| Campaign identity collision detected | Reject creation before authoritative state is admitted. |
| Seed assignment fails | Reject creation before authoritative state is admitted. |
| Initial HLA-CORE initialization fails | Roll back lifecycle metadata admission or mark the attempt non-authoritative; no partial Campaign may be returned as created. |
| Durable backing fails during creation | Return diagnosable failure and preserve no partial authoritative Campaign, or recover to the last valid state under HLA-PERSIST/HLA-STATE semantics. |
| Package composition changes after Campaign creation | No automatic propagation to existing Campaign; require explicit migration flow. |

Diagnostics SHALL distinguish invalid Scenario, validation failure, authorization failure, identity collision, seed failure, initialization failure, persistence failure, and attempted Scenario mutation.

---

<a id="15-non-functional-derivation"></a>
# 15. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-LIFECYCLE Design Response |
|---|---|
| NFR-002 Security | Campaign creation uses HLA-CONTRACT authorization and HLA-VALIDATE admission before untrusted package/scenario content affects state. |
| NFR-003 Reliability | Instantiation is atomic at the component-contract level; identity, seed, lifecycle metadata, and initial HLA-CORE boundary must be created together or not at all. |
| NFR-005 Maintainability | Lifecycle creation is separated from package composition, validation, persistence, migration, and Campaign Reality ownership. |
| NFR-006 Auditability | Lifecycle metadata records Scenario reference, ECD reference, package composition pin, seed metadata, creation source, and creation time; HLA-CORE initial Events cover initial Campaign Reality provenance. |

HLA-LIFECYCLE SHALL NOT invent new quantitative performance targets during Detailed Design. Performance remains governed by HLA-CONTRACT/NFR-001 and later Test Planning.

---

<a id="16-testing-alignment"></a>
# 16. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL cover at least:

- Creating a Campaign from a valid Scenario produces a Campaign identity distinct from the Scenario identity.
- Campaign creation does not mutate Scenario authored content.
- Campaign identity remains stable across save/reload.
- Campaign identity remains stable across export/import.
- Campaign identity remains stable across package migration.
- Multiple Campaigns from the same Scenario have distinct identities, distinct seeds, and independent Campaign Reality streams.
- A mutation to one Campaign does not affect another Campaign instantiated from the same Scenario.
- Every newly created Campaign has a retrievable seed.
- Campaign seed remains stable across save/reload and export/import.
- Caller-supplied seed behavior, if supported, records seed source and requires authorization.
- Lifecycle metadata records creation provenance without requiring a separate lifecycle provenance record.
- HLA-CORE initial Events remain the provenance boundary for initial Campaign Reality.
- Invalid Scenario references fail before Campaign state is admitted.
- Validation failure prevents Campaign creation.
- Identity collision fails closed.
- Seed assignment failure fails closed.
- Initial HLA-CORE initialization failure leaves no partial authoritative Campaign.
- Package changes after Campaign creation do not automatically alter existing Campaign Reality.

---

<a id="17-traceability-summary"></a>
# 17. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Design Coverage |
|---|---|
| FR-001 | Campaign Creation Model, Scenario resolution, validated ECD input, HLA-CORE initialization. |
| FR-002 | Campaign Identity Model and persistence/import/migration invariants. |
| FR-003 | Independent Divergence Model and separate Campaign Reality/Observer Knowledge streams. |
| FR-040 | Scenario Immutability Boundary and failure semantics for Scenario mutation attempts. |
| FR-044 | Campaign Seed Model and public retrieval posture. |
| FR-019 through FR-024 | HLA-PACKAGE boundary for Scenario/ECD inputs and composition pins. |
| FR-031, FR-032 | Identity and seed persistence/export-import invariants. |
| FR-041 | Migration boundary preserving identity/seed and deferring rollback to owning components. |
| NFR-002 | Authorization and validation admission before instantiation. |
| NFR-003 | Atomic creation and no partial Campaign admission. |
| NFR-005 | Separation of lifecycle from package, validation, persistence, and core state ownership. |
| NFR-006 | Lifecycle metadata as creation provenance, with HLA-CORE initial Events covering initial Campaign Reality provenance. |

---

<a id="18-open-questions"></a>
# 18. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

No HLA-LIFECYCLE-owned open questions remain at this design level.

Resolved during Detailed Design review:

1. Caller-supplied Campaign seeds — resolved as allowed for v1 when supplied as Campaign creation metadata, such as metadata carried by a campaign/scenario creation file or equivalent creation request. The seed is fixed during Campaign creation, source provenance is recorded, generated seeds remain the default when no seed metadata is supplied, and seed metadata SHALL NOT act as a backdoor into Oracle execution.
2. Campaign seed access — resolved as ordinary authorized Campaign metadata access for v1. `getCampaignSeed` does not require a distinct capability beyond the relevant campaign query/read capability; the meaningful control point is Oracle invocation and invocation-specific random-context derivation, not hiding the Campaign seed as a secret.
3. Lifecycle creation provenance — resolved as lifecycle metadata for v1. HLA-LIFECYCLE SHALL NOT create a separate lifecycle provenance record unless later Test Planning, audit requirements, or implementation evidence demonstrate that lifecycle metadata is insufficient. HLA-CORE initial Events remain the provenance boundary for initial Campaign Reality.
4. Clone/fork/branch terminology — resolved as deferred feature terminology only. HLA-LIFECYCLE v1 does not introduce Campaign branching, alternate timelines, or a version-control system. Ordinary Scenario instantiation is not branching, and Retcon remains a separate in-Campaign correction mechanism rather than a branch mechanism. Future clone/fork/branch support would require explicit lifecycle operations and separate identity, seed, provenance, Observer Knowledge, checkpoint, synchronization, persistence, and migration semantics.

---

<a id="19-phase-gate-declaration"></a>
# 19. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-LIFECYCLE Detailed Design is **approved at the component-design level**.

This approval does not authorize implementation by itself. It approves the HLA-LIFECYCLE Detailed Design baseline for continued Detailed Design work and later planning.

This design establishes the HLA-LIFECYCLE component boundary, Campaign creation model, Campaign identity model, Campaign seed model, Scenario immutability boundary, independent divergence model, package/migration boundary, creation provenance posture, interface contracts, and failure semantics. No HLA-LIFECYCLE-owned open questions remain at this design level.

Implementation, testing, future clone/fork/branch rescoping, package-format SRD work, or later component designs may reveal a need to revisit this design. Any material change SHALL be handled through the project's normal lifecycle change-control process.
