# HLA-RESOLUTION Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-RESOLUTION
Version: 1.0 (Component Baseline)
Date (YYYY-MM-DD): 2026-09-13
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
- [6. State and Command Model](#6-state-and-command-model)
- [7. Interface Contracts](#7-interface-contracts)
- [8. Oracle Adapter Contract](#8-oracle-adapter-contract)
- [9. Resolution Flow](#9-resolution-flow)
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
- Architectural Component ID stable? **Yes** — HLA-RESOLUTION.
- Parent Detailed Design phase opened? **Yes** — [Engine Detailed Design](../engine-detailed-design.md) v1.0 candidate.
- Advancement to implementation authorized? **Yes** — controlled Implementation was authorized by the approved Test Planning gate on 2026-09-17; this component baseline alone did not grant that authorization.

This document refines HLA-RESOLUTION only. It does not authorize implementation and does not introduce a new architectural component.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-RESOLUTION — Unresolved State, Resolution, and the Oracle Boundary.

**Component purpose:** HLA-RESOLUTION turns unresolved or uncertain campaign information into authoritative Campaign Reality changes through controlled, provenance-bearing Resolution Commands. It is the deterministic/probabilistic boundary where human decisions, Procedures, Rules, and validated Oracle results can become state changes without letting probabilistic output mutate reality directly.

**Primary requirements:** FR-012, FR-013, FR-014.

**Related requirements/NFRs:** FR-034, FR-037, NFR-006.

**In scope:**

- Explicit representation of Unresolved State.
- Resolution Command construction and application posture.
- Source/provenance requirements for human decisions, Package-defined procedures, rules, and Oracle results.
- Generic Oracle Adapter contract.
- Validation handoff to HLA-VALIDATE before Oracle results can become Resolution input.
- Exclusive write-path coordination to HLA-CORE after Campaign instantiation.
- Failure behavior that leaves unresolved targets unresolved when probabilistic input fails.

**Out of scope:**

- HLA-CORE Event schema and append implementation.
- HLA-STATE Undo execution and Checkpoint/Recovery mechanics.
- HLA-PACKAGE Oracle/rule/procedure definition language.
- HLA-QUERY Presentation Model behavior.
- Authorization decisions, which are owned by HLA-CONTRACT.
- Concrete PRNG algorithm, storage mechanism, runtime language, or API transport.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-RESOLUTION SHALL preserve the approved HLA constraints:

- It is the only component that writes resolved Campaign Reality changes to HLA-CORE after Campaign instantiation.
- It represents Unresolved State explicitly.
- It treats Oracle results as non-authoritative inputs until a Resolution Command applies them.
- It depends on HLA-PACKAGE for Oracle/Procedure/Rule definitions.
- It depends on HLA-VALIDATE for Oracle result validation.
- It records Resolution source/provenance sufficient for auditability.
- It does not let Package code, Oracle output, Procedure output, Rules, or clients mutate Campaign Reality directly.

Any design that allows probabilistic output to bypass Adapter -> Validate -> Resolution Command -> HLA-CORE is a deterministic/probabilistic boundary failure.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-RESOLUTION SHALL:

- Create and maintain references to Unresolved State markers.
- Build Resolution Commands from authorized inputs.
- Validate Oracle results before accepting them as Resolution input.
- Apply accepted Resolution Commands atomically to HLA-CORE.
- Attach source/provenance metadata to each Resolution.
- Preserve enough command metadata for HLA-STATE to reason about Undo.
- Leave targets unresolved when probabilistic input fails validation or cannot be produced.

HLA-RESOLUTION SHALL NOT:

- Authorize callers.
- Execute Package-supplied arbitrary code.
- Validate Package or import artifacts except by invoking HLA-VALIDATE for Oracle results.
- Own Campaign Reality storage.
- Own Observer Knowledge.
- Persist Checkpoints or perform Recovery.
- Guess, synthesize, or default an answer when an Oracle/Procedure result is invalid.
- Rewrite historical Events.

**Invariants:**

- Every Resolution has an identifiable source.
- Every Resolution that changes Campaign Reality is represented as a Command.
- Oracle results are inputs to Resolution, not state changes.
- A failed Resolution has no partial Campaign Reality effect.
- An unresolved target remains queryably unresolved until a Resolution Command succeeds.

---

<a id="5-internal-decomposition"></a>
# 5. Internal Decomposition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-RESOLUTION SHALL be decomposed internally to avoid becoming a God Component:

| Internal Module | Responsibility | Persistent Authority |
|---|---|---|
| `UnresolvedStateRegistry` | Tracks unresolved markers and their target references. | Owns unresolved marker metadata, not resolved Campaign Reality. |
| `ResolutionCommandFactory` | Converts accepted inputs into Resolution Command objects. | None; creates transient command structures. |
| `ResolutionSourceNormalizer` | Normalizes human, Procedure, Rule, and Oracle sources into provenance metadata. | None; produces provenance metadata. |
| `OracleAdapter` | Invokes Package-defined Oracle definitions through the generic engine-owned adapter contract. | None; cannot write Campaign Reality. |
| `OracleResultAcceptor` | Sends Oracle results to HLA-VALIDATE and accepts/rejects them as Resolution input. | None; controls acceptance only. |
| `ResolutionApplier` | Applies accepted Resolution Commands to HLA-CORE atomically. | None; HLA-CORE owns Event log and Campaign Reality. |
| `UndoDescriptorBuilder` | Captures command metadata needed by HLA-STATE to evaluate Undo. | None; HLA-STATE owns Undo execution. |

These are Detailed Design subcomponents, not new HLA components. They exist to separate mechanisms while preserving the approved HLA boundary.

---

<a id="6-state-and-command-model"></a>
# 6. State and Command Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Unresolved State marker:**

```text
UnresolvedStateRef {
  unresolvedId
  campaignRef
  targetRef
  declaredAtCampaignTime?
  unresolvedType
  allowedResolutionSources[]
}
```

**Resolution Command:**

```text
ResolutionCommand {
  commandId
  campaignRef
  unresolvedRef
  source
  payload
  preconditions[]
  expectedEffects[]
  provenance
}
```

**Resolution Record:**

```text
ResolutionRecord {
  resolutionId
  commandId
  campaignRef
  unresolvedRef
  source
  appliedEventRef?
  rejectedDiagnostics[]
  outcome
}
```

**Undo Descriptor:**

```text
UndoDescriptor {
  commandId
  resolutionId
  campaignRef
  unresolvedRef?
  appliedEventRef
  source
  affectedRefs[]
  preStateRefs[]
  postStateRefs[]
  inversePolicy
  undoEligibility
  createdAt
}
```

`inversePolicy` is one of:

- `directInverse`
- `compensatingResolution`
- `nonUndoable`

`undoEligibility` is one of:

- `eligible`
- `blocked`
- `unknown`

`outcome` is one of:

- `applied`
- `rejected`
- `leftUnresolved`
- `conflict`
- `componentFailure`

HLA-RESOLUTION owns Unresolved State marker metadata, Resolution provenance records, and Undo descriptors for applied Resolution Commands. HLA-CORE owns the Event log and authoritative Campaign Reality. HLA-STATE owns Undo evaluation, Undo application, Recovery, Checkpoints, and Retcon execution.

---

<a id="7-interface-contracts"></a>
# 7. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Conceptual request to declare unresolved state:

```text
declareUnresolved(targetRef, unresolvedType, allowedResolutionSources[]) -> UnresolvedStateRef
```

`declareUnresolved` is an internal HLA-RESOLUTION operation. It may be invoked by lifecycle setup, validated Package procedures/rules, import/migration handling, or other approved engine-internal paths. It SHALL NOT be exposed as a general public HLA-CONTRACT operation. If caller-facing unresolved-state creation is needed for GM-facing tools, authoring tools, or debugging workflows, it SHALL be modeled later as a separate authorized request/proposal operation rather than direct declaration.

Conceptual request to resolve:

```text
resolve(unresolvedRef, source, payload) -> ResolutionRecord
```

Conceptual request to invoke an Oracle:

```text
invokeOracle(oracleRef, params, unresolvedRef?) -> OracleInvocationRecord
```

Conceptual provenance read operations exposed through HLA-CONTRACT authorization:

```text
getResolutionProvenance(resolutionId) -> ResolutionProvenance
getOracleInvocation(invocationId) -> OracleInvocationRecord
listOracleInvocations(filter) -> OracleInvocationSummary[]
```

**Preconditions for `resolve`:**

- HLA-CONTRACT has authorized the caller for the relevant Resolution capability.
- `unresolvedRef` identifies an unresolved target.
- `source` is one of the allowed source categories or is explicitly accepted by component-level policy.
- Oracle-derived payloads have passed HLA-VALIDATE.
- Oracle-derived payloads have explicit human/GM acceptance before use in a Resolution Command.
- Command preconditions are satisfied.

**Postconditions for `resolve`:**

- On success, exactly one accepted Resolution Command writes its expected Campaign Reality effect through HLA-CORE.
- On rejection, Campaign Reality is unchanged and the target remains unresolved unless another prior successful Resolution already resolved it.
- Resolution provenance is available through the Resolution Record.

---

<a id="8-oracle-adapter-contract"></a>
# 8. Oracle Adapter Contract

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The Oracle Adapter is a single engine-owned generic adapter. Packages define Oracle contracts and semantics; they do not define bespoke adapter shapes.

Conceptual Oracle invocation envelope:

```text
OracleInvocation {
  oracleRef
  campaignRef
  unresolvedRef?
  params
  campaignSeedRef?
  invocationContext
}
```

Conceptual Oracle result envelope:

```text
OracleResult {
  oracleRef
  invocationId
  resultPayload
  declaredContractRef
  generationMetadata
}
```

Conceptual Oracle invocation record:

```text
OracleInvocationRecord {
  invocationId
  oracleRef
  campaignRef
  unresolvedRef?
  paramsRef?
  campaignSeedRef?
  resultRef?
  validationDecision?
  diagnostics[]
  authoritativeStatus
  replaySupport
}
```

`authoritativeStatus` is one of:

- `notApplied`
- `eligibleInput`
- `acceptedForResolution`
- `appliedByResolution`
- `rejected`

`replaySupport` is one of:

- `replayable`
- `notReplayable`
- `unknown`

The Adapter SHALL:

- Normalize Oracle output into `OracleResult`.
- Preserve invocation metadata for provenance.
- Pass any Package-defined opaque payload through without interpreting game semantics beyond the declared contract.
- Provide an invocation-specific random context where the Oracle definition elects to use deterministic seeded behavior. The context is derived from the Campaign seed and invocation-specific data rather than by reusing the raw Campaign seed as the sole PRNG seed for every invocation.
- Submit the result to HLA-VALIDATE before it may be used as Resolution input.
- Treat validation as eligibility only; validation does not imply acceptance into Campaign Reality.
- Retain Oracle invocation records, including rejected results, as non-authoritative HLA-RESOLUTION provenance/diagnostic records.

The Adapter SHALL NOT:

- Write Campaign Reality.
- Treat Oracle output as authoritative.
- Synthesize fallback values.
- Select a PRNG algorithm for Packages.
- Execute AI model integration as an engine capability.
- Record rejected Oracle results as HLA-CORE Events.

---

<a id="9-resolution-flow"></a>
# 9. Resolution Flow

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Human/Rule/Procedure Resolution flow:**

1. HLA-CONTRACT authorizes the mutating operation.
2. HLA-RESOLUTION confirms the unresolved target is still unresolved.
3. `ResolutionSourceNormalizer` records the source category and provenance inputs.
4. `ResolutionCommandFactory` builds a command with expected effects and preconditions.
5. `ResolutionApplier` applies the command atomically to HLA-CORE.
6. HLA-RESOLUTION returns a Resolution Record with source/provenance and Event reference.

**Oracle-assisted Resolution flow:**

1. HLA-CONTRACT authorizes Oracle invocation or Resolution operation.
2. `OracleAdapter` invokes the Package-defined Oracle through the generic adapter contract.
3. `OracleResultAcceptor` submits the Oracle result to HLA-VALIDATE.
4. If HLA-VALIDATE rejects the result, HLA-RESOLUTION retains a non-authoritative Oracle invocation record, leaves the target unresolved, and returns diagnostics.
5. If HLA-VALIDATE accepts the result, the result becomes eligible Resolution input.
6. A human/GM authority must explicitly accept the Oracle-derived input before a Resolution Command can reference it.
7. A Resolution Command still must be built and applied before Campaign Reality changes.

**Conflict flow:**

If a target is no longer unresolved, if command preconditions fail, or if HLA-CORE rejects the write, HLA-RESOLUTION returns `conflict` or `rejected` with diagnostics and SHALL NOT partially apply effects.

---

<a id="10-data-design"></a>
# 10. Data Design

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**HLA-RESOLUTION-owned data:**

- Unresolved State marker metadata.
- Resolution provenance records.
- Oracle invocation/result references needed to explain a Resolution source.
- Undo descriptors used by HLA-STATE to reason about inverse operations.

**Not owned by HLA-RESOLUTION:**

- Campaign Reality Events and current state — HLA-CORE.
- Package Oracle/Procedure/Rule definitions — HLA-PACKAGE.
- Validation diagnostics model — HLA-VALIDATE.
- Checkpoints, Recovery, Retcon, and Undo execution — HLA-STATE.
- Caller authority — HLA-CONTRACT.

Resolution payload structure remains Package/ruleset-independent. HLA-RESOLUTION tracks meta-model references and provenance; it does not embed game-specific rules vocabulary.

Rejected Oracle invocation records are not Campaign Reality and are not HLA-CORE Events. HLA-CORE Events may reference Oracle invocation records only when an accepted Oracle-derived Resolution Command changes Campaign Reality.

---

<a id="11-failure-semantics"></a>
# 11. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Failure Trigger | Outcome | Side Effect Posture |
|---|---|---|
| Unknown unresolved target | `rejected` | No state change. |
| Target already resolved | `conflict` | No state change. |
| Unauthorized caller reaches component unexpectedly | `rejected` | No state change; HLA-CONTRACT should have blocked this earlier. |
| Oracle invocation fails | `leftUnresolved` | Target remains unresolved; no fallback value. |
| Oracle result fails HLA-VALIDATE | `leftUnresolved` | Target remains unresolved; diagnostics returned. |
| Resolution Command precondition fails | `conflict` | No state change. |
| HLA-CORE append/apply fails | `componentFailure` | No partial Resolution; HLA-CORE/HLA-STATE failure semantics apply. |
| Provenance metadata missing | `rejected` | No state change; source must be identifiable. |

HLA-RESOLUTION does not retry Oracle invocation or HLA-CORE writes by default. Retry/idempotency behavior must be explicitly designed after HLA-CORE and HLA-STATE contracts are refined.

---

<a id="12-non-functional-derivation"></a>
# 12. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-RESOLUTION Design Mechanism |
|---|---|
| NFR-002 Security | Oracle results are validated by HLA-VALIDATE before use as Resolution input. |
| NFR-003 Reliability | Resolution Commands apply atomically; failed commands have no partial state effect. |
| NFR-005 Maintainability | Internal modules separate Oracle adaptation, command construction, source normalization, application, and Undo descriptor creation. |
| NFR-006 Auditability | Every successful Resolution records source/provenance and Event reference sufficient for after-the-fact inspection. |

No new quantitative performance target is introduced here.

---

<a id="13-testing-alignment"></a>
# 13. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL include tests proving:

- Unresolved State can be queried without forcing a generated answer.
- Generating an Oracle result does not change Campaign Reality.
- A valid Oracle result still does not change Campaign Reality until referenced by a Resolution Command.
- An invalid Oracle result leaves the target unresolved and returns HLA-VALIDATE diagnostics.
- Human, Procedure, Rule, and Oracle-sourced Resolutions record source/provenance.
- Failed command preconditions produce no partial Event or state change.
- HLA-RESOLUTION is the only post-instantiation write path to HLA-CORE.
- HLA-STATE can identify Undo-relevant command metadata without HLA-RESOLUTION performing Undo itself.
- Package-defined Oracle behavior cannot bypass the generic Oracle Adapter contract.
- Rejected Oracle invocation records are retained as non-authoritative HLA-RESOLUTION provenance/diagnostic records and are not HLA-CORE Events.
- HLA-CONTRACT capability checks control access to Oracle invocation/provenance records through `provenance.query`.

Test IDs are intentionally not assigned until the Test Planning phase.

---

<a id="14-traceability-summary"></a>
# 14. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Coverage in This Design |
|---|---|
| FR-012 | Unresolved State marker model and `declareUnresolved` contract. |
| FR-013 | Resolution Command model, source/provenance handling, successful application flow. |
| FR-014 | Oracle Adapter boundary and rule that Oracle results do not mutate state without Resolution. |
| FR-037 | Oracle result validation handoff to HLA-VALIDATE before use as Resolution input. |
| NFR-006 | Resolution provenance records and Event reference posture. |

This design is sufficient to approve the HLA-RESOLUTION component-design baseline for continued Detailed Design work and later planning.

---

<a id="15-open-questions"></a>
# 15. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

None.

**Resolved during review:**

- Unresolved declaration exposure — `declareUnresolved` is internal to HLA-RESOLUTION and approved engine-internal callers. It is not exposed as a general public HLA-CONTRACT operation. A future caller-facing unresolved-state request/proposal operation may be designed for GM-facing tools, authoring tools, or debugging workflows if justified.
- Oracle result acceptance — a validated Oracle result is eligible Resolution input only. In the current design, Oracle-derived Resolution requires explicit human/GM acceptance before a Resolution Command may change Campaign Reality. Future Package-defined automation may be considered only if it preserves human authority, records the automation source, and is explicitly authorized by package/ruleset semantics and HLA-CONTRACT capability checks.
- Undo descriptor shape — HLA-RESOLUTION SHALL produce a reference-based `UndoDescriptor` for every applied Resolution Command, identifying the command, Resolution, Campaign, applied Event, source, affected references, pre/post state references, inverse policy, eligibility, and creation time. HLA-RESOLUTION SHALL NOT execute Undo; HLA-STATE owns Undo evaluation and application.
- Rejected Oracle invocation retention — Oracle invocation records, including rejected results, are retained as non-authoritative HLA-RESOLUTION provenance/diagnostic records. They are not HLA-CORE Events. HLA-CORE Events may reference Oracle invocation records only when an accepted Oracle-derived Resolution Command changes Campaign Reality. Authoring/debugging tools may access retained records through HLA-CONTRACT-authorized provenance queries, subject to `provenance.query`.

---

<a id="16-phase-gate-declaration"></a>
# 16. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-RESOLUTION Detailed Design is **approved at the component-design level**.

This approval does not authorize implementation by itself. It approves the HLA-RESOLUTION Detailed Design baseline for continued Detailed Design work and later planning.

This design establishes Unresolved State handling, Resolution Command shape, Oracle Adapter boundary, explicit human/GM acceptance posture for Oracle-derived Resolution, reference-based Undo descriptor production, rejected Oracle invocation retention, provenance records, and the deterministic/probabilistic mutation boundary. No HLA-RESOLUTION-owned open questions remain at this design level.

Implementation, testing, package-format SRD work, future automation policy, or later component designs may reveal a need to revisit this design. Any material change SHALL be handled through the project's normal lifecycle change-control process.
