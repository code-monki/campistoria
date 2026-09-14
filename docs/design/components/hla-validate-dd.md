# HLA-VALIDATE Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-VALIDATE
Version: 0.1 (Component Draft)
Date (YYYY-MM-DD): 2026-09-13
Author(s): CodeMonki
Status: Draft
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
- [5. Validation Subject Model](#5-validation-subject-model)
- [6. Validator Chain Model](#6-validator-chain-model)
- [7. Interface Contracts](#7-interface-contracts)
- [8. Diagnostic Model](#8-diagnostic-model)
- [9. Trust Boundary Flows](#9-trust-boundary-flows)
- [10. Data Design](#10-data-design)
- [11. Failure Semantics](#11-failure-semantics)
- [12. Non-Functional Derivation](#12-non-functional-derivation)
- [13. Testing Alignment](#13-testing-alignment)
- [14. Traceability Summary](#14-traceability-summary)
- [15. Open Questions](#15-open-questions)
- [16. Phase Gate Declaration](#16-phase-gate-declaration)

---

<a id="1-design-authority-declaration"></a>
# 1. Design Authority Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Architecture phase approved? **Yes** — `engine-hla.md` v1.0, approved 2026-09-12.
- Architectural Component ID stable? **Yes** — HLA-VALIDATE.
- Parent Detailed Design phase opened? **Yes** — `engine-detailed-design.md` v0.1.
- Advancement to implementation authorized? **No.**

This document refines HLA-VALIDATE only. It does not authorize implementation and does not introduce a new architectural component.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-VALIDATE — Validation & Diagnostics.

**Primary requirements:** FR-037 and FR-038.

**Primary NFR:** NFR-002.

**Related deterministic-probabilistic constraint:** Package-declared Oracle and Procedure outputs must conform to the Package-declared contract before being accepted as Resolution input (`engine-srs.md` §6).

**Security posture:** Externally supplied artifacts are untrusted and may be hostile, not merely malformed. This includes Packages, Campaign imports, Oracle results, and artifacts generated or modified by LLMs or other external tools. AI is not an engine capability, but AI-generated artifacts may still arrive through public contracts and receive no special trust.

**In scope:**

- Validation subject taxonomy for Package content, Campaign import artifacts, Oracle results, and consistency checks.
- Validator chain ordering and stop/continue behavior.
- Structured diagnostic shape for validation failures.
- Stateless validation behavior at approved trust boundaries.
- Format-level, schema/contract-level, semantic, and cross-reference validation layers.
- Test-planning expectations proving bypass is not available.

**Out of scope:**

- Package composition algorithms owned by HLA-PACKAGE.
- Import/export persistence mechanics owned by HLA-PERSIST.
- Oracle invocation and Resolution semantics owned by HLA-RESOLUTION.
- Authorization decisions owned by HLA-CONTRACT.
- Persistence of diagnostics as authoritative history. HLA-VALIDATE produces diagnostics; owning components decide whether and how accepted operations record provenance.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-VALIDATE SHALL preserve the approved HLA constraints:

- It validates Package/Campaign consistency, imported data, and Oracle results at every approved trust boundary.
- It produces structured diagnostics.
- It uses Chain of Responsibility so engine-level and Package-contributed validators can compose without a fixed closed algorithm.
- It has no dependency on other components' internals beyond the supplied validation subject.
- It owns no persistent authoritative data.
- It does not mutate Campaign Reality, Observer Knowledge, Package registries, Checkpoints, imports, exports, or Resolution records.
- It is invoked by HLA-PACKAGE for Package registration/composition validation, HLA-PERSIST for Campaign import validation, and HLA-RESOLUTION for Oracle result validation.

Any path that allows Package content, imported Campaign data, or Oracle results to affect authoritative state without HLA-VALIDATE is a Detailed Design failure and an Architecture rollback candidate.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-VALIDATE SHALL:

- Accept an explicit validation subject and validation context.
- Select the applicable validator chain for the subject type.
- Execute validators in deterministic order.
- Collect structured diagnostics.
- Return a validation result that owning components can use to accept, reject, or continue with warnings.
- Distinguish format, schema/contract, semantic, dependency, consistency, and trust-boundary failures.
- Treat Oracle results as untrusted until validated against the declared Package contract.
- Fail closed for externally supplied artifacts at state-admitting trust boundaries.

HLA-VALIDATE SHALL NOT:

- Perform authorization.
- Compose Packages.
- Import Campaign data.
- Invoke Oracles.
- Resolve Unresolved State.
- Guess fallback values for invalid probabilistic output.
- Mutate authoritative state.
- Persist diagnostics as Campaign history.

**Invariants:**

- Validation is side-effect free.
- A fatal validation diagnostic prevents the invoking component from applying the subject.
- All diagnostics identify the subject, category, severity, and human-readable cause where determinable.
- Package-contributed validators can extend validation behavior only through declarative validation contracts or declarative rules interpreted by engine-owned validators; they do not receive engine-core access.

---

<a id="5-validation-subject-model"></a>
# 5. Validation Subject Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-VALIDATE SHALL recognize the following validation subject types:

| Subject Type | Owning Caller | Purpose | Required Validation Layers |
|---|---|---|---|
| `packageArtifact` | HLA-PACKAGE | Validate Package content before registration or composition. | Format, schema/contract, dependency, semantic, Package-declared validators. |
| `effectiveCampaignDefinition` | HLA-PACKAGE | Validate composed System/Setting/Scenario/Override result before Campaign use or migration. | Dependency, conflict, override, semantic consistency. |
| `campaignImportArtifact` | HLA-PERSIST | Validate imported Campaign data before it can affect Campaign Reality or Observer Knowledge. | Format, schema/contract, identity, version, consistency, dependency. |
| `oracleResult` | HLA-RESOLUTION | Validate probabilistic output before it can become Resolution input. | Envelope, Package-declared result contract, semantic fit to unresolved target. |
| `migrationPlan` | HLA-PACKAGE / HLA-STATE | Validate that a proposed Package migration can be applied atomically. | Dependency, version, compatibility, rollback precondition checks. |
| `exportArtifact` | HLA-PERSIST | Validate produced artifacts when a caller requests verification. | Format, completeness, manifest/reference consistency. |

This taxonomy is deliberately limited to subjects visible in the approved HLA/SRS. Additional subject types require component-level justification and traceability.

---

<a id="6-validator-chain-model"></a>
# 6. Validator Chain Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Validation SHALL execute as an ordered Chain of Responsibility. Validators may add diagnostics and may stop the chain only when later validators cannot safely reason about the subject.

**Default chain order:**

1. **Envelope validator** — confirms subject type, identity, version marker, and required top-level fields.
2. **Format validator** — confirms parseability and structural well-formedness of the artifact representation where applicable.
3. **Schema/contract validator** — confirms required fields, types, declared contracts, and Package-declared shapes.
4. **Dependency validator** — confirms declared dependencies, versions, and required capabilities are present.
5. **Semantic validator** — confirms the subject is meaningful in its target context.
6. **Cross-reference validator** — confirms referenced Campaign, Package, Observer, Entity, Location, Fact, or unresolved target references are identifiable where required.
7. **Policy validator** — confirms trust-boundary-specific constraints, such as destructive import preconditions or migration rollback prerequisites.
8. **Package-contributed validators** — execute only through declarative validation contracts or declarative rules interpreted by engine-owned validators, and only against subjects they are permitted to inspect.

**Stop/continue rules:**

- Unparseable or structurally malformed subjects stop the chain after format diagnostics.
- Missing required schema fields stop semantic and cross-reference validators for those fields.
- Dependency failures may continue into additional dependency checks, but SHALL prevent semantic acceptance.
- Warnings may continue and may allow acceptance if no fatal diagnostic exists.
- Fatal diagnostics SHALL result in `ValidationDecision.reject`.
- State-admitting trust boundaries require `accept`; `acceptWithWarnings` is advisory only and SHALL NOT admit externally supplied artifacts into authoritative state.

HLA-VALIDATE SHALL NOT rely on validator ordering to hide missing checks. Each validator declares its prerequisites and the subject types it handles.

---

<a id="7-interface-contracts"></a>
# 7. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Conceptual validation request:

```text
ValidationRequest {
  subjectType
  subjectRef?
  subjectPayload
  validationContext
  requestedDepth
  diagnosticDetail
}
```

Conceptual validation context:

```text
ValidationContext {
  campaignRef?
  packageCompositionRef?
  sourceComponent
  trustBoundary
  targetOperation
  declaredContractRef?
}
```

Conceptual validation response:

```text
ValidationResponse {
  decision
  diagnostics
  acceptedWarnings
  validatorTrace
}
```

`decision` is one of:

- `accept`
- `acceptWithWarnings`
- `reject`
- `indeterminate`

`requestedDepth` is one of:

- `formatOnly`
- `contract`
- `semantic`
- `full`

`diagnosticDetail` is one of:

- `standard`
- `trace`

**Preconditions:**

- The subject type is declared.
- The invoking component is one of the approved HLA callers or an approved public validation route through HLA-CONTRACT.
- The requested validation depth is valid for the subject type.

**Postconditions:**

- HLA-VALIDATE returns diagnostics and decision without mutating authoritative state.
- A `reject` decision prevents the invoking component from applying the subject.
- An `indeterminate` decision prevents application unless a later component-level design explicitly defines a safe degraded path. No such degraded path is defined in this draft.

---

<a id="8-diagnostic-model"></a>
# 8. Diagnostic Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Diagnostics SHALL be structured and caller-actionable.

```text
ValidationDiagnostic {
  diagnosticId
  severity
  category
  subjectType
  subjectRef?
  affectedRef?
  validatorId
  message
  machineCode?
  relatedRefs[]
}
```

`severity` is one of:

- `info`
- `warning`
- `error`
- `fatal`

`category` is one of:

- `format`
- `schema`
- `contract`
- `dependency`
- `semantic`
- `crossReference`
- `conflict`
- `trustBoundary`
- `internalValidatorFailure`

At minimum, every `error` or `fatal` diagnostic SHALL include an identifiable subject or affected reference, a category, and a human-readable message. If the affected object cannot be identified because the artifact is malformed, the diagnostic SHALL identify the smallest known container, such as the import artifact or Package artifact.

Diagnostics are not Campaign history. If an accepted operation later records provenance, that responsibility belongs to the owning component, not HLA-VALIDATE.

---

<a id="9-trust-boundary-flows"></a>
# 9. Trust Boundary Flows

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Package registration/composition flow:**

1. HLA-CONTRACT authorizes the Package operation.
2. HLA-PACKAGE submits Package content or composed Effective Campaign Definition to HLA-VALIDATE.
3. HLA-VALIDATE returns diagnostics and decision.
4. HLA-PACKAGE registers, composes, rejects, or reports warnings according to the decision.

**Campaign import flow:**

1. HLA-CONTRACT authorizes the import operation.
2. HLA-PERSIST parses only enough to identify the artifact envelope and submits the import subject to HLA-VALIDATE.
3. HLA-VALIDATE returns diagnostics and decision.
4. HLA-PERSIST rejects invalid imports before Campaign Reality or Observer Knowledge are affected.

**Oracle result flow:**

1. HLA-RESOLUTION receives an Oracle result through the approved Oracle Adapter path.
2. HLA-RESOLUTION submits the result and declared contract context to HLA-VALIDATE.
3. HLA-VALIDATE validates the result against the Package-declared Oracle contract and target unresolved state.
4. If validation rejects the result, HLA-RESOLUTION leaves the associated element Unresolved and returns diagnostics; it SHALL NOT synthesize a fallback value.

**Public validation flow:**

1. A caller may request validation through HLA-CONTRACT.
2. HLA-CONTRACT performs authorization if the validation subject requires it.
3. HLA-CONTRACT routes to HLA-VALIDATE and returns structured diagnostics.
4. Public validation remains read-only and does not register, import, migrate, or resolve anything by itself.

---

<a id="10-data-design"></a>
# 10. Data Design

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-VALIDATE owns no persistent domain data. It defines transient validation structures and validator metadata.

**Transient structures:**

- `ValidationRequest`
- `ValidationContext`
- `ValidationResponse`
- `ValidationDiagnostic`
- `ValidatorTrace`
- `ValidatorDescriptor`

**Validator descriptor:**

```text
ValidatorDescriptor {
  validatorId
  supportedSubjectTypes[]
  prerequisites[]
  producedCategories[]
  source
}
```

`source` distinguishes engine-owned validators from Package-declared validators. Package-declared validators are declarative validation contracts or declarative rules interpreted by engine-owned validators; they are not Package-supplied executable code.

---

<a id="11-failure-semantics"></a>
# 11. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Failure Trigger | Decision | Side Effect Posture |
|---|---|---|
| Unknown subject type | `reject` | No state change. |
| Malformed or unparseable subject | `reject` | No state change; format diagnostics returned. |
| Missing declared dependency | `reject` | No state change; dependency diagnostics returned. |
| Package-contributed validator cannot be resolved | `reject` for subjects that require it | No state change; trust-boundary diagnostic returned. |
| Package-contributed validator returns invalid output | `reject` or `indeterminate` | No state change; internal validator failure diagnostic returned. |
| Oracle result violates declared contract | `reject` | No Resolution application; target remains Unresolved. |
| Import artifact references unknown required object | `reject` | No partial import. |
| Non-fatal warning on advisory validation or non-admitting verification | `acceptWithWarnings` | No state admission; caller may use warnings for repair/debugging. |
| Non-fatal warning on Package registration, Effective Campaign Definition use, Campaign import, migration, or Oracle result acceptance | `reject` for admission purposes | No state admission; warnings must be corrected before admission. |

HLA-VALIDATE does not retry validators by default. Retry behavior for unavailable Package-declared validators remains open and cannot result in silent acceptance.

---

<a id="12-non-functional-derivation"></a>
# 12. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-VALIDATE Design Mechanism |
|---|---|
| NFR-002 Security | Mandatory validation gates for Package content, Campaign import artifacts, and Oracle results; failed validation prevents state influence. |
| NFR-005 Maintainability | Chain of Responsibility isolates validators by subject type and responsibility rather than centralizing all logic in one validator. |
| NFR-006 Auditability | Structured diagnostics can be attached by owning components to rejected operations or provenance records where appropriate. |

No quantitative performance target is introduced here. Validator cost and ordering should be profiled during implementation planning and Test Planning once concrete schemas and Package contracts exist.

---

<a id="13-testing-alignment"></a>
# 13. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL include tests proving:

- A Package with a missing declared dependency is rejected with structured diagnostics.
- A malformed Package artifact fails at format validation before semantic validators run.
- A malformed Campaign import artifact cannot partially affect Campaign Reality or Observer Knowledge.
- An Oracle result violating its declared Package contract is rejected and leaves the target Unresolved.
- Package registration, Campaign import, migration, and Oracle result acceptance do not proceed on `acceptWithWarnings`.
- Externally supplied artifacts, including LLM-generated or LLM-modified artifacts, are treated as untrusted and fail closed.
- A public validation request returns diagnostics without registering, importing, migrating, or resolving anything.
- Validator chain ordering is deterministic.
- Package-contributed validators are interpreted as declarative contracts/rules, cannot access engine-core internals, and cannot bypass engine-owned validators.
- All `error` and `fatal` diagnostics include subject or affected reference, category, severity, and human-readable message where determinable.
- HLA-PACKAGE, HLA-PERSIST, and HLA-RESOLUTION have no accepting path that bypasses HLA-VALIDATE at the approved trust boundaries.

Test IDs are intentionally not assigned until the Test Planning phase.

---

<a id="14-traceability-summary"></a>
# 14. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Coverage in This Draft |
|---|---|
| FR-037 | Validation subject taxonomy, dependency validation, consistency validation, trust-boundary flows. |
| FR-038 | Structured diagnostic model with subject/affected reference, category, severity, and message. |
| NFR-002 | Mandatory validation gates before untrusted Package/import/Oracle content can affect state. |
| NFR-005 | Chain of Responsibility and validator descriptors support separable validation logic. |
| NFR-006 | Diagnostics can support provenance and after-the-fact explanation when attached by owning components. |

This draft is sufficient to start review of HLA-VALIDATE design direction. It is not yet sufficient to mark FR-037 or FR-038 fully designed in the RTM.

---

<a id="15-open-questions"></a>
# 15. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Resolved during review:**

- Package-contributed validator form — resolved as declarative validation contracts or declarative rules interpreted by engine-owned validators for the current design. HLA-VALIDATE SHALL NOT execute Package-supplied validator code. Sandboxed executable validators are deferred unless and until declarative validation proves insufficient for a concrete approved requirement; introducing them would require Requirements and Architecture review covering sandboxing, determinism, portability, auditability, resource limits, failure behavior, and trust-boundary impact.
- Validator trace exposure — structured diagnostics are exposed by default. `validatorTrace` MAY be included only when the caller requests `diagnosticDetail = trace` and HLA-CONTRACT authorizes trace detail for that caller/context. Trace output is debug/support data rather than the stable user-facing diagnostic contract, and SHALL avoid exposing engine internals beyond validator IDs, execution status, skip/stop reasons, and diagnostic references needed for troubleshooting.
- Warning admission policy — state-admitting trust boundaries require strict `accept`. `acceptWithWarnings` is permitted only for advisory validation or non-admitting verification flows. Package registration, Effective Campaign Definition use, Campaign import, migration, and Oracle result acceptance SHALL NOT proceed on `acceptWithWarnings`.
- Hostile-input posture — externally supplied artifacts may be malicious, including artifacts generated or modified by LLMs or other external tools. HLA-VALIDATE treats them as untrusted and fails closed at admission boundaries.

---

<a id="16-phase-gate-declaration"></a>
# 16. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Component-level responsibilities defined? **Yes, draft.**
- Public interface model defined? **Yes, conceptual draft.**
- Validation subject taxonomy defined? **Yes, draft.**
- Validator chain model defined? **Yes, draft.**
- Diagnostic model defined? **Yes, draft.**
- Data ownership defined? **Yes.**
- Failure semantics defined? **Yes, draft.**
- NFR derivation defined? **Yes, draft.**
- Test alignment defined? **Yes, draft without Test IDs.**
- Open questions remaining? **Yes.**
- Ready for implementation? **No.**

End of HLA-VALIDATE Detailed Design draft.
