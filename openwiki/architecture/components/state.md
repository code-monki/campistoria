---
type: architecture-component
title: "HLA-STATE: Checkpoint, Undo, Recovery, and Retcon"
description: "How HLA-STATE acts as Memento caretaker for Checkpoints, reuses HLA-RESOLUTION's Command infrastructure for Undo, and treats Retcon as a conservative, human-authorized correction rather than a history rewrite."
tags: [architecture, hla, memento, checkpoint, undo, retcon, recovery]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-125ff0a16640cef3dc2fb75a
    resource: repo://docs/design/components/hla-state-dd.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-STATE: Checkpoint, Undo, Recovery, and Retcon

HLA-STATE owns state-management lifecycle operations that deliberately move a Campaign between valid states: Checkpoint creation/restoration, technical Recovery, Undo evaluation/application, and human-authorized Retcon orchestration. It coordinates HLA-CORE snapshots, HLA-RESOLUTION mutation application, HLA-PERSIST durable backing, and HLA-PACKAGE migration context **without ever becoming an alternate owner of live Campaign Reality.**

## Why Memento + reused Command

A Checkpoint is, definitionally, GoF Memento's stated purpose — "capture and externalize an object's internal state without violating its encapsulation, so it can be restored later." [HLA-CORE](core.md) (the Memento *originator*) produces the snapshot; HLA-STATE acts as the *caretaker*, holding Checkpoints without needing to understand Campaign Reality's internals — which matters because that structure is Package-extended and not fixed.

Undo does not need a new pattern: because Resolution is already modeled as Command ([HLA-RESOLUTION](resolution.md)), Undo is simply that Command's inverse operation, applied without becoming part of permanent history (FR-034's "not recorded as part of Campaign history"). The rejected alternative — ad hoc full-state copy without an originator/caretaker distinction — would have made Checkpoint restoration need to know Campaign Reality's internal structure directly. Modeling Retcon as silent direct mutation was also rejected: FR-036 requires provenance and direct-reference identification, so Retcon is instead a compensating Event appended to HLA-CORE's log, preserving the append-only guarantee Event Sourcing depends on — never a rewrite of prior events.

## Internal decomposition

| Internal Unit | Responsibility | Boundary |
|---|---|---|
| `CheckpointCoordinator` | Coordinates Checkpoint creation/restoration | Not component snapshot payload internals |
| `SnapshotCaretaker` | Records snapshot metadata, boundaries, payload references, integrity status | Durable backing remains HLA-PERSIST |
| `CadencePolicy` | Evaluates time/event/close triggers for Checkpoints | Never uses Campaign Time as wall-clock time |
| `RecoveryCoordinator` | Restores valid state after technical failure | Never performs semantic Retcon or Undo |
| `UndoEvaluator` | Evaluates UndoDescriptor eligibility and current-state compatibility | Never constructs HLA-CORE Events directly |
| `UndoStackManager` | Maintains the contiguous LIFO sequence of undo-eligible Resolutions | Never permits dependency-breaking non-contiguous Undo |
| `RetconCoordinator` | Coordinates Retcon intent, provenance, direct-reference identification | Never performs automatic consequence rewriting |
| `StateMutationRouter` | Routes accepted Undo/Retcon intent to HLA-RESOLUTION | Never bypasses HLA-RESOLUTION |
| `MigrationStateCoordinator` | Tracks rollback/checkpoint implications for migration plans | Never owns Package composition |
| `StateManagementAuditLog` | Records restore/recovery/undo/retcon/migration provenance outside Campaign history | Never creates HLA-CORE Campaign Reality Events |
| `StateDiagnosticBuilder` | Produces structured diagnostics | Never persists diagnostics as Campaign Reality itself |

## Checkpoints vs. Snapshots

HLA-STATE distinguishes a **Checkpoint** (a deliberate recoverable Campaign state exposed through HLA-STATE operations) from a **Snapshot** (a component-owned derived payload at a known boundary, used to bound replay cost). A Checkpoint may reference one or more component snapshots plus metadata to verify and restore the boundary. `checkpointReason` includes `manual`, `cadenceWallClock`, `cadenceEventCount`, `gracefulClose`, `preMigration`, `preRestore`, and `recovery`.

**Checkpoint cadence** (resolved 2026-09-09): a Checkpoint is triggered by whichever occurs first — 60 minutes of wall-clock time since the last Checkpoint, 50 Events appended since the last Checkpoint, or a graceful application close. Wall-clock time, not Campaign Time, governs the time-based trigger, because this is a reliability mechanism bounding crash/recovery data loss (FR-035/NFR-003), not a campaign-scale mechanism. Both numbers are provisional defaults, not derived from usage data, and are expected to be configurable.

All state-management operations (Checkpoint restore, Recovery, Undo, Retcon, migration rollback) are recorded as `StateManagementRecord` provenance entries — these are audit/provenance records for engine state-management operations, explicitly **not** HLA-CORE Campaign Reality Events, and are never presented as in-world Campaign history.

## Restore vs. Recovery — deliberately distinct operations

**Checkpoint restore** is a deliberate, user-initiated operation moving the Campaign to a selected Checkpoint. **Recovery** is a technical operation restoring valid state after failure, typically to the last valid persisted Checkpoint/Event boundary. HLA-STATE treats these as **distinct operations with distinct diagnostics** — Recovery never fabricates missing Events, never treats a failed semantic operation as accepted, and never performs Retcon or Undo as a side effect. If Recovery cannot restore a valid state, it returns a specific, diagnosable failure rather than hiding unrecoverable corruption behind a nominal success.

## Undo: stack-oriented, never a history rewrite

HLA-STATE evaluates Undo using HLA-RESOLUTION's `UndoDescriptor` records. Undo is **stack-oriented**: ordinary Undo reverses only the latest eligible Resolution or a contiguous LIFO range of recent eligible Resolutions. Non-contiguous reversal, dependency-breaking reversal, or reversing an older operation while preserving later dependent operations is **rejected as Undo** and redirected to Retcon or another explicit correction workflow instead.

The flow: HLA-CONTRACT authorizes with `resolution.undo` → HLA-STATE retrieves the relevant UndoDescriptor(s) → `UndoStackManager` verifies contiguity → `UndoEvaluator` checks current-state compatibility and declared eligibility → if the operation requires Campaign Reality mutation, HLA-STATE sends an accepted Undo intent to **HLA-RESOLUTION** (never directly to HLA-CORE) → HLA-RESOLUTION normalizes the intent into an authoritative mutation and coordinates the HLA-CORE append → HLA-STATE records non-Campaign-history provenance. Undo never rewrites prior HLA-CORE Events and never creates a normal in-world historical occurrence describing itself.

## Retcon: conservative, human-authorized, no automatic consequence rewriting

Retcon is a human-authorized revision of established Campaign Reality, routed through HLA-RESOLUTION the same way Undo is, and appended as **correction history rather than a rewrite of prior Events.** The flow includes an explicit step where HLA-STATE asks HLA-CORE/HLA-QUERY-owned read interfaces for directly referencing campaign objects where determinable, and constructs the Retcon intent with prior value, proposed value, and that direct-reference set before handing off to HLA-RESOLUTION.

Retcon handling is intentionally conservative because a change to established past Campaign Reality can create future-facing inconsistencies analogous to time-travel paradoxes — later Events, Facts, Relationships, State, Observer Knowledge, or Presentation Models may have been created under assumptions the Retcon changes. HLA-STATE therefore treats **automatic consequence rewriting as explicitly out of scope**: Retcon identifies direct references and reports diagnostics on what it could and couldn't determine, but leaves any follow-up Retcons or Resolutions to explicit human/GM decision. Retcon never silently reinterprets Observer Knowledge and never hides the prior value when it's determinable.

## Migration coordination

HLA-PACKAGE owns migration *planning*; HLA-STATE owns state-management coordination around migration *safety* — creating pre-migration Checkpoints, verifying rollback preconditions supplied by HLA-PACKAGE, preserving prior composition-pin state, and coordinating Recovery if a technical failure interrupts migration. HLA-STATE never decides Package compatibility, never transforms package-defined semantic references itself, and never applies migration-driven Campaign Reality changes directly to HLA-CORE.

## Dependency relationships

- **Depends on** [HLA-CORE](core.md) as the Memento originator for Campaign Reality snapshots, and on [HLA-RESOLUTION](resolution.md) for reused Command infrastructure — **HLA-STATE never writes Undo or Retcon mutations directly to HLA-CORE.**
- **Depended upon only by** HLA-CONTRACT.
- **Coordinates with** HLA-PERSIST for durable Checkpoint/snapshot backing and HLA-PACKAGE for migration rollback context.

## Non-functional derivation

- **NFR-003 (Reliability):** Checkpoints, snapshots, boundary verification, Recovery planning, and replay from the last valid boundary provide a diagnosable path after ordinary failures.
- **NFR-005 (Maintainability):** state-management policy is separated from HLA-CORE Event mechanics, HLA-RESOLUTION mutation application, HLA-PERSIST durable backing, and HLA-PACKAGE migration planning.
- **NFR-006 (Auditability):** Retcon, Recovery, migration state coordination, and Undo state-management records preserve provenance without ever rewriting Campaign history.
