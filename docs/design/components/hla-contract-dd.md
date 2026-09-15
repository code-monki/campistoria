# HLA-CONTRACT Detailed Design

Project Name: Campistoria Engine
Component ID: HLA-CONTRACT
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
- [5. Interface Contract Model](#5-interface-contract-model)
- [6. Authorization Model](#6-authorization-model)
- [7. Public Operation Groups](#7-public-operation-groups)
- [8. Client Synchronization Contract](#8-client-synchronization-contract)
- [9. Data Design](#9-data-design)
- [10. Failure Semantics](#10-failure-semantics)
- [11. Non-Functional Derivation](#11-non-functional-derivation)
- [12. Testing Alignment](#12-testing-alignment)
- [13. Packaging and Orchestration Impact](#13-packaging-and-orchestration-impact)
- [14. Traceability Summary](#14-traceability-summary)
- [15. Open Questions](#15-open-questions)
- [16. Phase Gate Declaration](#16-phase-gate-declaration)

---

<a id="1-design-authority-declaration"></a>
# 1. Design Authority Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Architecture phase approved? **Yes** — `engine-hla.md` v1.0, approved 2026-09-12.
- Architectural Component ID stable? **Yes** — HLA-CONTRACT.
- Parent Detailed Design phase opened? **Yes** — `engine-detailed-design.md` v0.1.
- Advancement to implementation authorized? **No.**

This document refines HLA-CONTRACT only. It does not authorize implementation and does not introduce a new architectural component.

---

<a id="2-scope"></a>
# 2. Scope

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Component being refined:** HLA-CONTRACT — Public Contract Facade.

**Component purpose:** HLA-CONTRACT is the engine's public boundary. It gives first-party clients, third-party clients, authoring tools, and integrations one documented operation surface while keeping authorization, request/response conventions, diagnostics, and synchronization envelopes consistent across all callers.

**Primary requirement:** FR-039.

**Cross-cutting NFRs:** NFR-001 and NFR-005 directly; NFR-002 and NFR-006 indirectly because contract routing controls validation and provenance entry.

**In scope:**

- Public facade rules for first-party and third-party callers.
- Request, response, session, authorization, diagnostic, and synchronization envelopes.
- Capability-based authorization posture for mutating operations.
- Actor/session/authority vocabulary that works for current single-player use without preventing future multi-user refinement.
- Transport-neutral operation grouping across embedded, in-process service, and future network adapters.
- Reconnect behavior using a caller-supplied last-known revision or delta number.

**Out of scope:**

- Concrete programming language, API protocol, serialization format, schema language, authentication provider, persistence backend, or network transport.
- Complete internal contracts for HLA-LIFECYCLE, HLA-RESOLUTION, HLA-OBSERVER, HLA-PACKAGE, HLA-QUERY, HLA-PERSIST, HLA-STATE, or HLA-VALIDATE.
- Multi-user collaboration semantics beyond authorization and reconnect posture already needed to prevent privileged mutation.
- Hosted account management, player invitation flows, concurrent-player conflict resolution, multiplayer latency requirements, and persistent user-management storage.

---

<a id="3-architectural-conformance"></a>
# 3. Architectural Conformance

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CONTRACT SHALL preserve the approved HLA constraints:

- It is the only public entry surface for reference clients, third-party clients, authoring tools, and integrations.
- It exposes identical documented operations to first-party and third-party callers.
- It depends on all other HLA components; no other HLA component depends on it.
- It owns no Campaign Reality, Observer Knowledge, Package, Checkpoint, or persistence data.
- It performs authorization before forwarding mutating requests.
- It keeps deployment shape open: embedded library call, in-process service call, and future network API remain adapter choices.
- It does not let callers bypass HLA-VALIDATE for Package registration, Campaign import, or Oracle result acceptance.

Any design that requires a first-party shortcut, a transport-specific contract, or direct external access to an internal component is an Architecture rollback trigger.

---

<a id="4-responsibilities"></a>
# 4. Responsibilities

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CONTRACT SHALL:

- Publish the complete engine operation catalog available to all callers.
- Normalize caller input into component-facing requests.
- Establish caller/session identity at the contract boundary.
- Evaluate required capabilities before any mutating operation is forwarded.
- Route accepted requests to the owning HLA component.
- Return structured results, diagnostics, and synchronization metadata.
- Preserve a local, in-process invocation path for NFR-001.
- Prevent public callers from addressing HLA-CORE, HLA-RESOLUTION, or any other internal component directly.

HLA-CONTRACT SHALL NOT:

- Own authoritative campaign data.
- Persist Presentation Models as authoritative state.
- Perform Resolution, Validation, Package composition, Query projection, Persistence, Checkpointing, Undo, Recovery, or Retcon logic itself.
- Grant special capabilities to the reference client, first-party authoring tools, or first-party Packages.
- Bind the engine to a network protocol or deployment platform.

**Invariants:**

- Every public operation has one documented contract path.
- Every mutating operation declares at least one required capability.
- Permission checks resolve to explicit capabilities, not hardcoded role names.
- Denied operations have no side effect.
- A successful forwarded operation returns either a result or a structured diagnostic from the owning component.

---

<a id="5-interface-contract-model"></a>
# 5. Interface Contract Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

All public operations SHALL use the same conceptual envelope:

```text
ContractRequest {
  operationId
  callerContext
  targetContext
  synchronizationContext
  payload
}
```

```text
ContractResponse {
  outcome
  result
  diagnostics
  synchronizationState
  provenanceRef
}
```

**`operationId`** identifies one documented operation in the public operation catalog.

**`callerContext`** identifies the principal, actor, caller class, session, observer context, and declared role/capability grants available for the request. Caller class is descriptive only; it SHALL NOT grant privilege by itself.

**`targetContext`** identifies the Campaign, Package, Observer, Projection, import/export artifact, Checkpoint, or Resolution target addressed by the operation.

**`synchronizationContext`** carries the caller's last-known Campaign revision, Observer Knowledge revision, Presentation Model revision, or delta number where applicable.

**`payload`** carries operation-specific input. The payload is validated for envelope shape by HLA-CONTRACT, then forwarded to the owning component for domain validation.

**`outcome`** is one of:

- `accepted`
- `rejected`
- `unauthorized`
- `notFound`
- `conflict`
- `validationFailed`
- `componentFailure`

**`diagnostics`** SHALL be structured consistently with FR-038 where the failure can be attributed to an affected object and cause.

**Preconditions:**

- The operation is present in the public operation catalog.
- The request envelope is well-formed.
- The caller/session identity is established to the degree required by the operation.
- Required capabilities are available for mutating operations.

**Postconditions:**

- Rejected, unauthorized, malformed, or conflict responses have no side effect.
- Accepted mutating requests are forwarded exactly once to the owning component unless a component-level idempotency contract later specifies a safe retry behavior.
- Response metadata includes the latest relevant synchronization state when a Campaign, Observer, or Presentation Model revision is affected or queried.

---

<a id="6-authorization-model"></a>
# 6. Authorization Model

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CONTRACT uses **capability-based authorization as the enforcement primitive**.

Roles MAY exist as assignable bundles of capabilities, but authorization SHALL evaluate explicit capabilities. This preserves the project owner's RBAC concern without making hardcoded role names the enforcement mechanism.

Principal, role, actor, session, capability, RBAC, and CBAC terminology SHALL align with `docs/glossary.md`. HLA-CONTRACT defines the contract-level use of those terms for authorization checks, request routing, and future multi-user refinement.

Single-player operation SHALL use this same authority vocabulary with default principal, actor, and session references. HLA-CONTRACT SHALL NOT model single-player as "no actor model." This avoids a later contract retrofit if hosted or multi-user play is reopened, while preserving the current single-player scope.

HLA-CONTRACT defines the contract-level meaning of principal, actor, session, role bundle, and capability grant. In the current scope, authority assignments MAY be supplied by the embedding application or default solo-session setup. HLA-CONTRACT SHALL NOT require a hosted account store, multiplayer service, or persistent user-management subsystem.

Capability granularity SHALL follow a justified-split rule: each added capability ID must earn its place by representing a materially different authority level, information exposure risk, mutation risk, recovery risk, or test assertion. HLA-CONTRACT SHALL NOT decompose capabilities merely to create a large permission matrix.

This draft therefore deepens the taxonomy only around privileged knowledge, authoritative mutation, Package lifecycle, import, rollback, and Retcon operations.

**Initial capability taxonomy:**

| Capability ID | Meaning | Typical Operations |
|---|---|---|
| `campaign.create` | Create a Campaign from a Scenario | HLA-LIFECYCLE operations |
| `campaign.query.presentation` | Query caller-appropriate Presentation Models | Ordinary HLA-QUERY projection operations |
| `campaign.query.archive` | Query archived history outside the active working set | HLA-QUERY and HLA-PERSIST archival access |
| `tooling.inspect.campaign` | Query authority-filtered campaign inspection views for authoring, GM-facing, diagnostic, validation, or test tooling | Privileged HLA-QUERY inspection operations |
| `observer.record.self` | Record Observer Knowledge for the caller's own Observer context | Ordinary HLA-OBSERVER recording operations |
| `observer.record.other` | Record Observer Knowledge for another Observer context | GM/tool-mediated HLA-OBSERVER recording operations |
| `resolution.submit` | Submit an intended action or proposed Resolution input | Player/tool request into HLA-RESOLUTION |
| `resolution.apply` | Apply an accepted Resolution to authoritative state | Ordinary GM/system Resolution operations |
| `resolution.override` | Force or override normal Resolution flow under explicit authority | Exceptional HLA-RESOLUTION operations |
| `oracle.invoke` | Invoke a Package-defined Oracle through the approved adapter path | HLA-RESOLUTION Oracle operations |
| `provenance.query` | Query Resolution provenance and Oracle invocation diagnostic records | HLA-RESOLUTION provenance/diagnostic read operations |
| `package.validate` | Validate Package content without registration | HLA-VALIDATE and HLA-PACKAGE validation operations |
| `package.register` | Register and compose Packages | HLA-PACKAGE registration/composition operations |
| `package.remove` | Remove Package registrations where dependency rules allow | HLA-PACKAGE removal operations |
| `package.migrate` | Migrate a Campaign's pinned Package composition | HLA-PACKAGE and HLA-STATE operations |
| `persistence.export` | Export Campaign or Package artifacts | HLA-PERSIST export operations |
| `persistence.import.package` | Import Package artifacts | HLA-PERSIST import through HLA-VALIDATE |
| `persistence.import.campaign` | Import Campaign artifacts without replacing existing authoritative state | HLA-PERSIST Campaign import operations |
| `persistence.import.destructive` | Import or restore in a way that may replace existing authoritative state | High-risk HLA-PERSIST/HLA-STATE operations |
| `checkpoint.create` | Create Checkpoints | HLA-STATE checkpoint operations |
| `checkpoint.restore` | Restore from Checkpoints | HLA-STATE recovery operations |
| `resolution.undo` | Undo an unwanted Resolution | HLA-STATE and HLA-RESOLUTION operations |
| `reality.retcon` | Perform a human-authorized Retcon | HLA-STATE operations |
| `authority.manage` | Manage roles/capability grants where the deployment supports that concept | HLA-CONTRACT authority operations |

**Candidate role bundles:**

| Role Bundle | Default Capabilities | Notes |
|---|---|---|
| `campaignOwner` | All capabilities for the owned Campaign, including `authority.manage` | Administrative default; exact ownership semantics remain deployment-neutral. |
| `gameMaster` | `campaign.query.presentation`, `campaign.query.archive`, `tooling.inspect.campaign`, `observer.record.other`, `resolution.apply`, `resolution.override`, `oracle.invoke`, `provenance.query`, `checkpoint.create`, `resolution.undo`, `reality.retcon`, Package migration if granted | Models high-trust play authority without making GM a hardcoded authorization primitive. |
| `player` | `campaign.query.presentation`, `observer.record.self`, `resolution.submit` where granted | Supports player actions that mutate state only through ordinary authorized submission paths. |
| `authoringTool` | `package.validate`, `package.register`, `package.remove`, `tooling.inspect.campaign` where granted, import/export capabilities as granted | Same public contracts as any other tool. |
| `viewer` | `campaign.query.presentation` only | Read-only role bundle. |

The role bundle list is provisional. Component-level design may refine names and capability granularity, but SHALL NOT replace capability checks with role-name checks.

---

<a id="7-public-operation-groups"></a>
# 7. Public Operation Groups

<sup>[↩](#table-of-contents "Back to ToC")</sup>

The public catalog groups operations by owning HLA component. Exact method names may change during later component refinement, but every operation SHALL remain available through HLA-CONTRACT only.

| Operation Group | Owning Component | Example Conceptual Operations | Mutating? | Required Capability Posture |
|---|---|---|---|---|
| Campaign Lifecycle | HLA-LIFECYCLE | `createCampaign`, `getCampaignSeed` | Mixed | `campaign.create` for creation; query capability for reads |
| Resolution | HLA-RESOLUTION | `submitResolution`, `applyResolution`, `overrideResolution`, `invokeOracle` | Yes | `resolution.submit`, `resolution.apply`, `resolution.override`, `oracle.invoke` as applicable |
| Resolution Provenance | HLA-RESOLUTION | `getResolutionProvenance`, `getOracleInvocation`, `listOracleInvocations` | No authoritative mutation | `provenance.query` |
| Observer Knowledge | HLA-OBSERVER | `recordObservation`, `knowledgeOf` | Mixed | `observer.record.self` or `observer.record.other` for writes; query capability for reads |
| Package | HLA-PACKAGE | `validatePackage`, `registerPackage`, `compose`, `migrate`, `removePackage` | Mixed | `package.validate`, `package.register`, `package.remove`, `package.migrate` |
| Query | HLA-QUERY | `query`, `getPresentationModel`, `getDelta`, `queryInspectionView`, `queryArchived` | No authoritative mutation | `campaign.query.presentation`, `tooling.inspect.campaign`, or `campaign.query.archive` |
| Persistence | HLA-PERSIST | `export`, `importPackage`, `importCampaign`, `destructiveImport`, `resolveArchived` | Mixed | `persistence.export`, `persistence.import.package`, `persistence.import.campaign`, `persistence.import.destructive`, `campaign.query.archive` |
| State | HLA-STATE | `checkpoint`, `restore`, `undo`, `recover`, `retcon` | Yes | `checkpoint.create`, `checkpoint.restore`, `resolution.undo`, `reality.retcon` |
| Validation | HLA-VALIDATE | `validate` | No authoritative mutation | Depends on validation subject and caller context |
| Authority | HLA-CONTRACT | `describeOperations`, `describeCapabilities`, `assignCapabilityBundle` | Mixed | none for descriptions; `authority.manage` for assignment |

Read operations may still require a capability because Observer Knowledge and Presentation Models are subject to POV Resolution and caller context.

---

<a id="8-client-synchronization-contract"></a>
# 8. Client Synchronization Contract

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CONTRACT SHALL support the following synchronization posture:

- A caller with no usable prior state receives a full current representation appropriate to the requested operation.
- A caller with prior state MAY send a last-known revision or delta number.
- The engine decides whether to return missing deltas, a compacted delta, or a fresh full representation.
- The contract is transport-neutral and does not require a network connection, subscription channel, or server push model.
- Synchronization SHALL use separate revision streams for Campaign Reality, Observer Knowledge, and Presentation Models rather than one global Campaign delta.
- Principal, actor, and observer context SHALL travel with synchronization requests so future multi-user refinement can manage caller-specific deltas without changing the request shape.
- `deltaNumber`, where used, is a stream-scoped marker and SHALL NOT be interpreted as a single global Campaign counter.
- HLA-CONTRACT owns synchronization envelope semantics and response-mode vocabulary; it SHALL NOT materialize caller-visible full state, deltas, compacted deltas, or Presentation Models.
- HLA-QUERY owns caller-visible synchronization materialization as the responsible HLA component, but its Detailed Design SHALL decompose POV Resolution, Presentation Model materialization, revision mapping, archive bridging, and delta materialization into explicit internal modules or equivalent interfaces.

Conceptual request field:

```text
SynchronizationContext {
  principalRef?
  actorRef?
  observerRef?
  campaignRevision?
  observerRevision?
  presentationRevision?
  deltaNumber?
  acceptsFullState
  acceptsDelta
}
```

Conceptual response field:

```text
SynchronizationState {
  currentCampaignRevision?
  currentObserverRevision?
  currentPresentationRevision?
  returnedDeltaRange?
  stateTransferMode
}
```

`stateTransferMode` is one of:

- `full`
- `delta`
- `compactedDelta`
- `unchanged`
- `unavailable`

If the caller's revision is too old, unknown, belongs to a different Campaign/Observer/Presentation context, or cannot be satisfied from the active/archival window, HLA-CONTRACT SHALL return a diagnostic or full state depending on operation semantics and `acceptsFullState`.

This design captures reconnect behavior without deciding whether clients are local, intermittently networked, or future hosted clients.

---

<a id="9-data-design"></a>
# 9. Data Design

<sup>[↩](#table-of-contents "Back to ToC")</sup>

HLA-CONTRACT owns no authoritative domain data. It defines transient envelope structures and may rely on a deployment-provided session authority for caller identity and capability grants.

**Transient structures:**

- `ContractRequest`
- `ContractResponse`
- `CallerContext`
- `TargetContext`
- `SynchronizationContext`
- `SynchronizationState`
- `PrincipalRef`
- `ActorRef`
- `SessionRef`
- `CapabilityGrant`
- `DiagnosticEnvelope`

**Ownership boundaries:**

- Caller/session identity source is deployment-adapter owned, not engine-domain owned.
- Capability evaluation is HLA-CONTRACT behavior.
- Capability grants may be supplied by an embedding application or persisted later by an authority-management design, but this document does not select that storage.
- Campaign revisions are produced by HLA-CORE/HLA-OBSERVER/HLA-QUERY-facing operations, not invented by HLA-CONTRACT.

---

<a id="10-failure-semantics"></a>
# 10. Failure Semantics

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Failure Trigger | Outcome | Side Effect Posture |
|---|---|---|
| Unknown operation | `rejected` | No forwarding. |
| Malformed envelope | `rejected` | No forwarding. |
| Missing caller/session identity for an operation that requires it | `unauthorized` | No forwarding. |
| Missing required capability | `unauthorized` | No forwarding. |
| Stale synchronization state that cannot be satisfied as a delta | `conflict` or `accepted` with full state, depending on `acceptsFullState` | No mutation unless the underlying operation is otherwise accepted. |
| Owning component validation failure | `validationFailed` | Owning component guarantees no partial state change. |
| Owning component conflict | `conflict` | Owning component guarantees no partial state change. |
| Owning component technical failure | `componentFailure` | HLA-CONTRACT returns diagnostic; recovery semantics belong to owning component. |

HLA-CONTRACT does not retry mutating operations by default. Retry and idempotency behavior must be explicitly designed per operation before implementation.

---

<a id="11-non-functional-derivation"></a>
# 11. Non-Functional Derivation

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| NFR | HLA-CONTRACT Design Mechanism |
|---|---|
| NFR-001 Performance | The contract is an in-process facade by default. Transport adapters may wrap it later, but the component contract itself does not require network overhead. |
| NFR-002 Security | HLA-CONTRACT enforces authorization before forwarding and preserves mandatory HLA-VALIDATE routing for untrusted content. |
| NFR-005 Maintainability | Operation groups route to owning components; changes in Package semantics, persistence, projection, or validation do not require redefining unrelated public contracts. |
| NFR-006 Auditability | HLA-CONTRACT passes caller context and provenance-relevant request metadata to mutating components so consequential changes can record source. |

Performance targets remain those of NFR-001. No new numeric target is invented here.

---

<a id="12-testing-alignment"></a>
# 12. Testing Alignment

<sup>[↩](#table-of-contents "Back to ToC")</sup>

Future Test Planning SHALL include tests proving:

- A first-party caller and third-party caller use the same operation catalog for equivalent capabilities.
- No public operation exposes direct internal component access.
- Every mutating operation declares required capabilities.
- A missing capability prevents forwarding and produces no side effect.
- Role bundles grant capabilities, but enforcement checks capabilities rather than role names.
- Package registration, import, and Oracle result flows cannot bypass HLA-VALIDATE.
- A reconnecting caller with last-known revision/delta number receives delta, compacted delta, full state, unchanged, or diagnostic behavior as specified.
- The in-process facade path can be measured independently of any network adapter.

Test IDs are intentionally not assigned until the Test Planning phase.

---

<a id="13-packaging-and-orchestration-impact"></a>
# 13. Packaging and Orchestration Impact

<sup>[↩](#table-of-contents "Back to ToC")</sup>

No packaging or orchestration mechanism is selected.

HLA-CONTRACT imposes only these future packaging constraints:

- The public contract artifact must be distributable to first-party and third-party implementers on equal terms.
- Any generated client bindings, if later introduced, must be generated from the same public contract source.
- Embedded and hosted adapters must wrap the same conceptual operation catalog.

---

<a id="14-traceability-summary"></a>
# 14. Traceability Summary

<sup>[↩](#table-of-contents "Back to ToC")</sup>

| Requirement | Coverage in This Draft |
|---|---|
| FR-039 | Same public operation catalog, no first-party shortcut, public operation groups, identical capability surface. |
| NFR-001 | In-process facade path preserved; transport overhead excluded from component contract. |
| NFR-005 | Operation groups route to owning components without merging Package, persistence, projection, validation, and public contract responsibilities. |
| NFR-002 | Authorization gate and HLA-VALIDATE routing posture captured. |
| NFR-006 | Provenance metadata forwarding posture captured for mutating operations. |

This draft is sufficient to start review of HLA-CONTRACT design direction. It is not yet sufficient to mark FR-039 fully designed in the RTM.

---

<a id="15-open-questions"></a>
# 15. Open Questions

<sup>[↩](#table-of-contents "Back to ToC")</sup>

**Resolved during review:**

- Capability taxonomy depth — resolved as sufficient for the current HLA-CONTRACT draft. No additional split is currently justified; future component designs may add capabilities only under the justified-split rule.
- Role/capability assignment posture — HLA-CONTRACT defines principal, actor, session, role bundle, and capability vocabulary now. Current single-player operation may use default solo authority assignments supplied by the embedding application, but the contract shape SHALL NOT assume "no actor model." Hosted accounts, multiplayer infrastructure, and persistent user management remain out of current scope.
- Synchronization stream shape — resolved as separate revision streams for Campaign Reality, Observer Knowledge, and Presentation Models. `deltaNumber` may be retained only as a stream-scoped marker, not as a single global Campaign counter.
- Synchronization materialization ownership — HLA-CONTRACT owns envelope semantics and response-mode vocabulary only. HLA-QUERY owns caller-visible full-state and delta materialization as a component responsibility, and HLA-QUERY Detailed Design shall decompose POV Resolution, Presentation Model materialization, revision mapping, archive bridging, and delta materialization internally rather than treating HLA-QUERY as a monolith or God Component.

---

<a id="16-phase-gate-declaration"></a>
# 16. Phase Gate Declaration

<sup>[↩](#table-of-contents "Back to ToC")</sup>

- Component-level responsibilities defined? **Yes, draft.**
- Public interface model defined? **Yes, conceptual draft.**
- Authorization model defined? **Yes, draft capability-based model.**
- Data ownership defined? **Yes.**
- Failure semantics defined? **Yes, draft.**
- NFR derivation defined? **Yes, draft.**
- Test alignment defined? **Yes, draft without Test IDs.**
- Open questions remaining? **Yes.**
- Ready for implementation? **No.**

End of HLA-CONTRACT Detailed Design draft.
