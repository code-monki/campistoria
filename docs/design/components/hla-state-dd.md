# HLA-STATE Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-STATE
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
- [6. Checkpoint and Snapshot Model](#6-checkpoint-and-snapshot-model)
- [7. Checkpoint Cadence](#7-checkpoint-cadence)
- [8. Restore and Recovery Model](#8-restore-and-recovery-model)
- [9. Undo Model](#9-undo-model)
- [10. Retcon Model](#10-retcon-model)
- [11. Migration Coordination](#11-migration-coordination)
- [12. Interface Contracts](#12-interface-contracts)
- [13. Validation and Diagnostics](#13-validation-and-diagnostics)
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
- Architectural Component ID stable? **Yes** — HLA-STATE.
- Parent Detailed Design phase opened? **Yes** — [Engine Detailed Design](../engine-detailed-design.md) v1.0 candidate.
- Advancement to implementation authorized? **No.**

This document refines HLA-STATE only. It does not authorize implementation and does not introduce a new architectural component.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-STATE — Checkpoint, Undo, Recovery, and Retcon.

**Component purpose:** HLA-STATE owns state-management lifecycle operations that deliberately move a Campaign between valid states: Checkpoint creation/restoration, technical Recovery, Undo evaluation/application, and human-authorized Retcon orchestration. It coordinates HLA-CORE snapshots, HLA-RESOLUTION mutation application, HLA-PERSIST durable backing, and HLA-PACKAGE migration context without becoming an alternate owner of live Campaign Reality.

**Primary requirements:** FR-033, FR-034, FR-035, FR-036.

**Related requirements/NFRs:** FR-011, FR-031, FR-041, NFR-003, NFR-006.

**In scope:**

- Checkpoint creation and restoration.
- Snapshot lifecycle and ownership at the HLA-STATE level.
- Checkpoint cadence configuration using the approved provisional cadence.
- Recovery after technical failure.
- Undo eligibility evaluation and Undo application orchestration.
- Retcon authorization context, direct-reference impact identification, and provenance orchestration.
- Coordination with HLA-RESOLUTION for Undo/Retcon Campaign Reality mutations.
- Coordination with HLA-CORE for snapshot payloads, append/fold/history boundaries, and direct-reference discovery inputs.
- Coordination with HLA-PERSIST for durable checkpoint/snapshot storage.
- Coordination with HLA-PACKAGE for migration rollback/precondition context.

**Out of scope:**

- HLA-CORE Event schema, append implementation, and fold implementation.
- HLA-RESOLUTION command construction internals beyond the state-management intent contract.
- HLA-PERSIST durable storage representation, export format, or snapshot persistence adapter.
- HLA-PACKAGE concrete package format or migration transformation language.
- HLA-OBSERVER snapshot/recovery behavior, which requires HLA-OBSERVER Detailed Design.
- Caller authorization, which is owned by HLA-CONTRACT.
- Concrete storage engine, runtime language, scheduler, or wall-clock implementation.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-STATE SHALL preserve the approved HLA constraints:

- It acts as Memento caretaker for Checkpoints and Recovery; HLA-CORE remains the originator for Campaign Reality snapshot payloads.
- It does not own live Campaign Reality.
- It does not write Undo or Retcon mutations directly to HLA-CORE.
- Undo and Retcon mutations SHALL flow through HLA-CONTRACT authorization, HLA-STATE eligibility/policy evaluation, HLA-RESOLUTION mutation normalization and command application, and then HLA-CORE append.
- Recovery restores valid engine state after technical failure and is distinct from Undo and Retcon.
- Retcon appends provenance-preserving correction history rather than rewriting prior Events.
- Checkpoint restoration, Recovery, Undo, and Retcon SHALL remain distinct operations with distinct diagnostics.
- HLA-PERSIST provides durable backing for Checkpoints/snapshots without owning their meaning.

Any design that lets HLA-STATE silently mutate HLA-CORE history, bypass HLA-RESOLUTION for semantic Campaign Reality changes, or treat Recovery as a user-facing Retcon violates the approved HLA.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-STATE SHALL:

- Create Checkpoints that represent known recoverable Campaign states.
- Restore a Campaign to a valid Checkpoint.
- Manage snapshot metadata and checkpoint lifecycle policy.
- Request component-owned snapshot payloads from HLA-CORE and other snapshot-capable components once those designs exist.
- Persist and retrieve Checkpoint/snapshot artifacts through HLA-PERSIST.
- Recover to the last valid persisted state after technical failure.
- Evaluate Undo eligibility using HLA-RESOLUTION `UndoDescriptor` records and current Campaign state.
- Orchestrate Undo through HLA-RESOLUTION when an Undo requires Campaign Reality mutation.
- Orchestrate human-authorized Retcon through HLA-RESOLUTION and preserve Retcon provenance.
- Identify directly referencing campaign objects where determinable for Retcon diagnostics and provenance.
- Coordinate with HLA-PACKAGE migration plans for rollback preconditions and package-composition state.

HLA-STATE SHALL NOT:

- Authorize callers.
- Own HLA-CORE Event history.
- Rewrite committed HLA-CORE Events.
- Apply semantic Campaign Reality changes directly to HLA-CORE.
- Decide Package composition or Package migration semantics.
- Persist checkpoint artifacts except through HLA-PERSIST.
- Automatically rewrite indirect consequences of Retcons.
- Treat Checkpoint restore, Undo, Recovery, and Retcon as interchangeable operations.

---

<a id="5-internal-decomposition"></a>
# 5. Internal Decomposition

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-STATE SHALL be decomposed internally:

| Internal Unit | Responsibility | Boundary |
|---|---|---|
| `CheckpointCoordinator` | Coordinates Checkpoint creation and restoration. | Does not own component snapshot payload internals. |
| `SnapshotCaretaker` | Records snapshot metadata, boundaries, component payload references, and integrity status. | Durable backing remains HLA-PERSIST. |
| `CadencePolicy` | Evaluates time/event/close triggers and configurability for Checkpoints. | Does not use Campaign Time as wall-clock time. |
| `RecoveryCoordinator` | Restores valid state after technical failure using persisted Events and Checkpoints. | Does not perform semantic Retcon or Undo. |
| `UndoEvaluator` | Evaluates `UndoDescriptor` eligibility, current-state compatibility, and inverse policy. | Does not construct HLA-CORE Events directly. |
| `UndoStackManager` | Maintains or derives the contiguous LIFO sequence of undo-eligible Resolution operations. | Does not permit dependency-breaking non-contiguous Undo. |
| `RetconCoordinator` | Coordinates Retcon intent, provenance, direct-reference identification, and HLA-RESOLUTION handoff. | Does not perform automatic consequence rewriting. |
| `StateMutationRouter` | Routes accepted Undo/Retcon state-management intent to HLA-RESOLUTION. | Does not bypass HLA-RESOLUTION. |
| `MigrationStateCoordinator` | Tracks rollback and checkpoint implications for Package migration plans. | Does not own Package composition. |
| `StateManagementAuditLog` | Records Checkpoint restore, Recovery, Undo metadata, Retcon orchestration, and migration rollback provenance outside Campaign history. | Does not create HLA-CORE Campaign Reality Events. |
| `StateDiagnosticBuilder` | Produces structured diagnostics for checkpoint, restore, recovery, undo, and retcon failures. | Does not persist diagnostics as Campaign Reality unless routed through owning components. |

---

<a id="6-checkpoint-and-snapshot-model"></a>
# 6. Checkpoint and Snapshot Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-STATE distinguishes Checkpoints from Snapshots:

- **Checkpoint:** a deliberate recoverable Campaign state exposed through HLA-STATE operations.
- **Snapshot:** a component-owned derived payload at a known boundary used to bound replay cost.

A Checkpoint may contain or reference one or more component snapshots plus metadata needed to verify and restore the boundary.

Conceptual Checkpoint:

```text
Checkpoint {
  checkpointId
  campaignRef
  checkpointReason
  checkpointBoundary
  componentSnapshots[]
  compositionPinRef?
  createdBy
  createdAt
  durabilityStatus
  provenance
}
```

`checkpointReason` is one of:

- `manual`
- `cadenceWallClock`
- `cadenceEventCount`
- `gracefulClose`
- `preMigration`
- `preRestore`
- `recovery`

Conceptual snapshot boundary:

```text
SnapshotBoundary {
  campaignRef
  componentId
  committedEventBoundary
  campaignTimeBoundary
  componentSchemaVersion
}
```

Conceptual component snapshot reference:

```text
ComponentSnapshotRef {
  componentId
  snapshotBoundary
  payloadRef
  integrityRef?
  restoreEligibility
}
```

Conceptual state-management record:

```text
StateManagementRecord {
  recordId
  campaignRef
  operationType
  requestedBy?
  checkpointRef?
  resolutionRef?
  retconRef?
  migrationRef?
  restoredFromBoundary?
  restoredToBoundary?
  reason?
  occurredAt
  diagnostics[]
}
```

`StateManagementRecord` entries are audit/provenance records for engine state-management operations. They are not HLA-CORE Campaign Reality Events and SHALL NOT be presented as in-world Campaign history.

HLA-STATE SHALL preserve HLA-CORE's boundary invariants:

- A Core snapshot boundary identifies Campaign, committed Event boundary, Campaign Time boundary, and Core schema/model version.
- Snapshot payloads are derived from committed Events.
- HLA-CORE can resume folding from a valid snapshot boundary plus subsequent Events.
- HLA-CORE rejects restoration when the snapshot boundary does not match the Event stream it claims to summarize.

HLA-STATE owns Checkpoint lifecycle metadata and restore policy. Component-owned snapshot payloads remain opaque to HLA-STATE except for boundary, integrity, and eligibility metadata.

---

<a id="7-checkpoint-cadence"></a>
# 7. Checkpoint Cadence

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The approved provisional Checkpoint cadence is:

- 60 minutes of wall-clock time since the last Checkpoint.
- 50 Events appended since the last Checkpoint.
- Graceful application close.

Whichever trigger occurs first creates or requests a Checkpoint.

Cadence rules:

- Wall-clock time SHALL NOT be interpreted as Campaign Time.
- The cadence values are provisional defaults pending real usage data.
- Implementations SHOULD make cadence values configurable rather than hardcoded.
- Manual Checkpoints MAY be requested independently of cadence triggers.
- Pre-migration and pre-restore Checkpoints MAY be created as safety boundaries when required by policy.

HLA-STATE SHALL NOT invent new quantitative cadence targets beyond the approved provisional values without Requirements or Architecture change control.

---

<a id="8-restore-and-recovery-model"></a>
# 8. Restore and Recovery Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Restore and Recovery are distinct.

**Checkpoint restore** is a deliberate operation that moves the Campaign to a selected Checkpoint. User-initiated restore SHALL create state-management provenance outside Campaign history.

**Recovery** is a technical operation that restores valid state after failure, typically to the last valid persisted Checkpoint/Event boundary. Recovery SHALL create state-management provenance where durable recording is possible.

Conceptual Recovery state:

```text
RecoveryPlan {
  recoveryId
  campaignRef
  failureContext
  selectedCheckpointRef?
  selectedEventBoundary?
  replayRange?
  validationDiagnostics[]
  recoveryDecision
}
```

Recovery flow:

1. Identify the last valid persisted Checkpoint and Event boundary.
2. Verify Checkpoint/snapshot integrity through HLA-PERSIST metadata and component restore checks.
3. Restore component snapshot payloads through their owning components.
4. Replay committed Events after the selected boundary.
5. Reject or diagnose partial, corrupted, or indeterminate writes.
6. Report Recovery outcome with diagnostics.

Recovery SHALL NOT:

- Fabricate missing Events.
- Treat a failed semantic operation as accepted.
- Perform Retcon or Undo as a side effect.
- Hide unrecoverable corruption behind a nominal success result.
- Record restore/recovery provenance as HLA-CORE Campaign Reality Events.

If Recovery cannot restore a valid state, it SHALL return a specific, diagnosable failure as required by NFR-003.

---

<a id="9-undo-model"></a>
# 9. Undo Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Undo reverses an accidental or unwanted operation without becoming Campaign history.

HLA-STATE evaluates Undo using HLA-RESOLUTION `UndoDescriptor` records.

Undo is stack-oriented. HLA-STATE SHALL treat ordinary Undo as reversal of the latest eligible Resolution or a contiguous last-in-first-out range of recent eligible Resolutions. Non-contiguous reversal, dependency-breaking reversal, or reversal of an older operation while preserving later dependent operations SHALL be rejected as Undo and redirected to Retcon or another explicit correction workflow.

Conceptual Undo request:

```text
UndoRequest {
  campaignRef
  resolutionRef
  requestedBy
  reason?
}
```

Conceptual Undo evaluation:

```text
UndoEvaluation {
  undoRequestId
  campaignRef
  resolutionRef
  undoDescriptorRef
  eligibility
  inversePolicy
  blockingRefs[]
  diagnostics[]
}
```

Undo flow:

1. HLA-CONTRACT authorizes the caller with `resolution.undo`.
2. HLA-STATE retrieves the relevant `UndoDescriptor` or contiguous LIFO descriptor range.
3. `UndoStackManager` verifies that the requested Undo target is the latest eligible Resolution or a contiguous LIFO range.
4. `UndoEvaluator` verifies Campaign, Resolution, Event, affected references, pre/post state references, current-state compatibility, and declared eligibility.
5. If the operation is not undoable, HLA-STATE rejects with diagnostics.
6. If Undo requires Campaign Reality mutation, HLA-STATE sends an accepted Undo intent to HLA-RESOLUTION.
7. HLA-RESOLUTION normalizes the intent into an authoritative mutation and coordinates HLA-CORE append.
8. HLA-STATE records non-Campaign-history state-management provenance for the Undo operation.

Undo SHALL NOT:

- Rewrite prior HLA-CORE Events.
- Create a normal Campaign historical occurrence describing the Undo as in-world history.
- Bypass HLA-RESOLUTION when Campaign Reality mutation is required.
- Override package-defined or descriptor-defined non-undoable policy without explicit future design.
- Reverse non-contiguous operations while preserving later dependent operations.

Undo metadata SHALL be represented in HLA-STATE state-management/provenance records. HLA-CORE does not require a special non-history cancellation marker in the current design; if later implementation proves such a marker is required for consistency, the design must be reopened through change control.

---

<a id="10-retcon-model"></a>
# 10. Retcon Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Retcon is a human-authorized revision of established Campaign Reality.

Conceptual Retcon request:

```text
RetconRequest {
  campaignRef
  targetRef
  priorValueRef?
  proposedValue
  authorizedBy
  reason
}
```

Conceptual Retcon record:

```text
RetconRecord {
  retconId
  campaignRef
  targetRef
  priorValueRef?
  appliedEventRef?
  directlyReferencingRefs[]
  authorizedBy
  reason
  diagnostics[]
}
```

Retcon flow:

1. HLA-CONTRACT authorizes the caller with `reality.retcon`.
2. HLA-STATE verifies human/GM authorization context and Retcon target eligibility.
3. HLA-STATE asks HLA-CORE/HLA-QUERY-owned read interfaces for directly referencing campaign objects where determinable.
4. HLA-STATE constructs a Retcon intent with prior value, proposed value, direct-reference set, reason, and provenance.
5. HLA-STATE sends accepted Retcon intent to HLA-RESOLUTION.
6. HLA-RESOLUTION normalizes the intent into an authoritative correction command and coordinates HLA-CORE append.
7. HLA-STATE records Retcon provenance and diagnostics.

Retcon SHALL:

- Preserve provenance of the change.
- Identify directly referencing campaign objects where determinable.
- Append correction history rather than rewrite prior Events.
- Remain human-authorized.

Retcon SHALL NOT:

- Automatically rewrite indirect consequences.
- Silently reinterpret Observer Knowledge.
- Bypass HLA-RESOLUTION for Campaign Reality mutation.
- Hide the prior value when it is determinable.

Retcon handling is intentionally conservative because a change to established past Campaign Reality can create future-facing inconsistencies analogous to time-travel paradoxes: later Events, Facts, Relationships, State, Observer Knowledge, or Presentation Models may have been created under assumptions the Retcon changes. HLA-STATE SHALL therefore treat automatic consequence rewriting as out of scope. Retcon diagnostics SHOULD identify the target, prior value, new value, known direct references, unresolved potential consequences, and limits of analysis so the human/GM can decide any follow-up Retcons, Resolutions, or annotations explicitly.

---

<a id="11-migration-coordination"></a>
# 11. Migration Coordination

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-PACKAGE owns migration planning. HLA-STATE owns state-management coordination around migration safety.

HLA-STATE SHALL support migration by:

- Creating pre-migration Checkpoints when required by migration policy.
- Verifying migration rollback preconditions supplied by HLA-PACKAGE.
- Preserving prior composition-pin state needed to leave a failed migration intact.
- Coordinating Recovery if a technical failure interrupts migration application.
- Ensuring migration failure does not leave a mixed package composition state.

HLA-STATE SHALL NOT:

- Decide Package compatibility.
- Transform package-defined semantic references by itself.
- Apply migration-driven Campaign Reality changes directly to HLA-CORE.

Migration transformation application remains a deferred package-format/design-set concern in HLA-PACKAGE. If migration transformations become authoritative Campaign Reality changes, their application SHALL preserve HLA-CONTRACT authorization, HLA-VALIDATE validation, HLA-STATE rollback/recovery policy, HLA-RESOLUTION mutation routing, and HLA-CORE append/fold invariants.

---

<a id="12-interface-contracts"></a>
# 12. Interface Contracts

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Public access occurs only through HLA-CONTRACT. HLA-STATE provides internal component interfaces.

```text
checkpoint(campaignRef, reason?) -> CheckpointResult
restore(checkpointRef) -> RestoreResult
recover(campaignRef, failureContext?) -> RecoveryResult
evaluateUndo(resolutionRef) -> UndoEvaluation
undo(resolutionRef, reason?) -> UndoResult
retcon(request: RetconRequest) -> RetconResult
prepareForMigration(migrationPlanRef) -> MigrationStatePlan
completeMigration(migrationPlanRef, outcome) -> MigrationStateResult
```

Required capabilities at HLA-CONTRACT before forwarding include:

- `checkpoint.create`
- `checkpoint.restore`
- `resolution.undo`
- `reality.retcon`

Recovery may be caller-requested or implementation-triggered after technical failure. HLA-CONTRACT and later implementation design must define whether caller-triggered Recovery requires a distinct public capability or is available only through controlled administration flows.

Current solo-scope posture: Recovery SHALL be available as both an implementation-triggered technical operation and an authorized administrative operation through HLA-CONTRACT. The authorized administrative operation SHALL require a distinct recovery capability. In the current single-user scope, the user is expected to be the administrator and SHALL receive structured notification of Recovery outcome, selected boundary, replay range, and diagnostics.

Deferred multiplayer posture: if multiplayer is reopened, Recovery and Checkpoint restore SHALL also define client synchronization behavior so each connected or reconnecting player refreshes from the correct Campaign Reality, Observer Knowledge, and Presentation Model revision after state restoration. That behavior is deferred to future HLA-CONTRACT, HLA-QUERY, HLA-OBSERVER, and multiplayer requirements work.

---

<a id="13-validation-and-diagnostics"></a>
# 13. Validation and Diagnostics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-STATE SHALL produce structured diagnostics for:

- Checkpoint creation failure.
- Missing, corrupted, or incompatible Checkpoint artifacts.
- Snapshot boundary mismatch.
- Restore ineligibility.
- Recovery indeterminate state.
- Undo descriptor missing or stale.
- Undo current-state incompatibility.
- Non-undoable inverse policy.
- Retcon target not found.
- Retcon direct-reference discovery partial or unavailable.
- Migration rollback precondition failure.

HLA-STATE MAY invoke HLA-VALIDATE for migration plans or imported/restored checkpoint artifacts when those subjects cross a trust boundary. HLA-STATE SHALL NOT treat unvalidated imported checkpoint artifacts as safe to restore.

---

<a id="14-failure-semantics"></a>
# 14. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Failure Trigger | Required Behavior |
|---|---|
| Checkpoint payload cannot be produced | Checkpoint creation fails; prior valid state remains authoritative. |
| Checkpoint cannot be durably persisted | Checkpoint creation fails or remains non-durable with diagnostics; it is not advertised as recoverable unless policy permits transient checkpoints. |
| Snapshot boundary mismatch | Reject restore/recovery from that snapshot; attempt older valid boundary or return diagnosable failure. |
| Corrupted Checkpoint artifact | Reject restore; return diagnostics; do not partially restore. |
| Interrupted write | Recovery restores last valid persisted state or reports diagnosable failure. |
| Undo descriptor missing | Reject Undo with diagnostics. |
| Undo target no longer compatible with current state | Reject Undo or require Retcon path; do not force inverse mutation silently. |
| `nonUndoable` inverse policy | Reject Undo unless future requirements define an explicit override path. |
| Retcon lacks required human authorization context | Reject Retcon before HLA-RESOLUTION handoff. |
| Retcon direct-reference discovery incomplete | Proceed only with diagnostics identifying incomplete coverage; do not invent indirect consequence rewrites. |
| Migration interrupted | Recover to prior valid composition state or report diagnosable failure; no mixed composition state. |

---

<a id="15-non-functional-derivation"></a>
# 15. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-STATE Design Response |
|---|---|
| NFR-003 Reliability | Checkpoints, snapshots, boundary verification, Recovery planning, and replay from last valid boundary provide a diagnosable path after ordinary failures. |
| NFR-005 Maintainability | State-management policy is separated from HLA-CORE Event mechanics, HLA-RESOLUTION mutation application, HLA-PERSIST durable backing, and HLA-PACKAGE migration planning. |
| NFR-006 Auditability | Retcon, Recovery, migration state coordination, and Undo state-management records preserve provenance without rewriting Campaign history. |

HLA-STATE SHALL NOT introduce new quantitative performance or cadence targets beyond approved Requirements/Architecture values.

---

<a id="16-testing-alignment"></a>
# 16. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL include HLA-STATE coverage for:

- Manual Checkpoint creation.
- Cadence-triggered Checkpoint creation by wall-clock interval, event count, and graceful close.
- Checkpoint restore reproducing the Checkpoint state.
- Rejection of corrupted or mismatched snapshots.
- Recovery after interrupted write to last valid persisted state.
- Recovery diagnostics when no valid state can be restored.
- Undo eligibility evaluation for `directInverse`, `compensatingResolution`, and `nonUndoable`.
- Undo rejection when current state no longer matches descriptor expectations.
- Undo handoff to HLA-RESOLUTION rather than direct HLA-CORE mutation.
- Retcon authorization enforcement.
- Retcon direct-reference identification where determinable.
- Retcon correction routed through HLA-RESOLUTION and appended as correction history.
- Migration pre-checkpoint and failed migration rollback behavior.

---

<a id="17-traceability-summary"></a>
# 17. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Design Coverage |
|---|---|
| FR-033 | Checkpoint model, snapshot references, restore interface, cadence policy. |
| FR-034 | Undo model, `UndoEvaluator`, HLA-RESOLUTION handoff, no direct history rewrite. |
| FR-035 | Recovery model, last-valid-boundary restoration, diagnosable failure behavior. |
| FR-036 | Retcon model, authorization context, prior value, direct-reference identification, provenance. |
| FR-041 | Migration coordination, pre-migration checkpoint, rollback/recovery posture. |
| NFR-003 | Checkpoint/Recovery boundary verification and replay from last valid state. |
| NFR-006 | Retcon, Recovery, migration, and state-management provenance. |

---

<a id="18-open-questions"></a>
# 18. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

No HLA-STATE-owned open questions remain at this design level. Observer Knowledge snapshot payload mechanics remain deferred to HLA-OBSERVER Detailed Design.

Resolved during Detailed Design review:

- Undo/Retcon write path — semantic Undo and Retcon mutations route through HLA-RESOLUTION; HLA-STATE does not receive an independent ordinary write contract into HLA-CORE.
- Undo representation — HLA-STATE SHALL use state-management/provenance records plus an Undo stack derived from `UndoDescriptor` records. Ordinary Undo applies only to the latest eligible Resolution or a contiguous LIFO range of eligible Resolutions. Non-contiguous or dependency-breaking reversal is rejected as Undo and redirected to Retcon or another explicit correction workflow. HLA-CORE does not require a special non-history cancellation marker in the current design.
- Checkpoint restore provenance — user-initiated Checkpoint restore SHALL create auditable state-management provenance outside Campaign history. Technical Recovery SHALL also create state-management provenance where durable recording is possible. Restore/Recovery provenance SHALL NOT be modeled as HLA-CORE Campaign Reality Events. HLA-STATE owns these records; HLA-PERSIST provides durable backing; HLA-CONTRACT may expose them through diagnostic, administrative, or provenance operations subject to capability checks.
- Recovery exposure — Recovery SHALL be available both as an implementation-triggered technical operation and as an authorized administrative operation through HLA-CONTRACT, guarded by a distinct recovery capability. In current solo scope, the user/admin SHALL be notified of Recovery outcome and diagnostics. Future multiplayer scope must define how connected and reconnecting players refresh their views after Recovery or Checkpoint restore.
- Observer Knowledge checkpoint participation — Checkpoints SHALL be campaign-level consistency boundaries and SHALL include Observer Knowledge once HLA-OBSERVER defines its snapshot payload and boundary mechanics. HLA-STATE coordinates the checkpoint boundary, but HLA-OBSERVER owns Observer Knowledge snapshot content. Restoring a Checkpoint SHALL NOT leave Campaign Reality and Observer Knowledge at different logical checkpoint boundaries. Observer isolation remains mandatory: checkpoint, restore, Recovery, and future multiplayer synchronization must not expose one observer's knowledge to another observer or player except through authorized HLA-CONTRACT/HLA-QUERY behavior.
- Snapshot boundary ownership — HLA-CORE defines Core snapshot boundary invariants; HLA-STATE owns Checkpoint lifecycle, restore policy, Recovery policy, and snapshot orchestration.
- Checkpoint cadence — the current design uses the approved provisional cadence of 60 wall-clock minutes, 50 Events, or graceful close, whichever occurs first, with configurability expected during implementation.

---

<a id="19-phase-gate-declaration"></a>
# 19. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-STATE Detailed Design is **approved at the component-design level**.

This approval does not authorize implementation by itself. It approves the HLA-STATE Detailed Design baseline for continued Detailed Design work and later planning.

This design establishes Checkpoint, Snapshot, Restore, Recovery, Undo, Retcon, and migration state coordination boundaries. No HLA-STATE-owned open questions remain at this design level; Observer Knowledge snapshot payload mechanics are deferred to HLA-OBSERVER Detailed Design.

Implementation, testing, HLA-OBSERVER Detailed Design, multiplayer rescoping, or later component designs may reveal a need to revisit this design. Any material change SHALL be handled through the project's normal lifecycle change-control process.
