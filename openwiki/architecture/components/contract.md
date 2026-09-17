---
type: architecture-component
title: "HLA-CONTRACT: Public Contract Facade and Authorization"
description: "How HLA-CONTRACT provides the engine's single public operation surface and owns Campaign Authority Binding authorization before requests reach any internal component."
tags: [architecture, hla, facade, authorization, contract, capability-based-access-control]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-228f8989412a8e1e690be143
    resource: repo://docs/design/components/hla-contract-dd.md
  - id: openwiki-source-615487f2876b472d6b4fadc3
    resource: repo://docs/project/decision-log.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-CONTRACT: Public Contract Facade and Authorization

HLA-CONTRACT is the engine's public boundary: the single documented operation surface through which every external actor — reference client, third-party client, authoring tool, or integration — invokes engine capability. Per FR-039, first-party and third-party callers reach the engine through identical operations; there is no privileged first-party shortcut into internal components.

## Position in the architecture

HLA-CONTRACT depends on all nine other HLA components (HLA-LIFECYCLE, HLA-CORE, HLA-RESOLUTION, HLA-OBSERVER, HLA-PACKAGE, HLA-QUERY, HLA-PERSIST, HLA-STATE, HLA-VALIDATE) and aggregates their public operations into one surface. No other component depends on HLA-CONTRACT. This makes it the outermost layer of the architecture: every caller enters through HLA-CONTRACT, and no component may be addressed directly from outside the engine.

HLA-CONTRACT owns no Campaign Reality, Observer Knowledge, Package, or Checkpoint data. Its one piece of authoritative data ownership is **Campaign Authority Bindings** — the durable authorization metadata — whose actual storage is delegated to [HLA-PERSIST](persist.md).

## Why Facade

The architecture applies the Facade pattern because FR-039's requirement is structural rather than aspirational: there must be exactly *one* entry surface, not "the reference client's internal shortcut" plus "the public API for everyone else." A Facade enforces this by construction — every caller, first-party or third-party, calls the same operations with no alternate path into HLA-CORE, HLA-RESOLUTION, or any other internal component.

The rejected alternative was exposing each subsystem's interface directly to callers. That was specifically rejected because it would leave the door open for a first-party client to be built against internal component interfaces (a "privileged shortcut"), which is exactly what FR-039 prohibits.

## Authorization: capability-based, not role-based

HLA-CONTRACT is the authorization choke point evaluated before any Campaign-scoped operation reaches an internal component. It uses **capability-based authorization as the enforcement primitive**: authorization evaluates the tuple `(principalRef, campaignRef, requiredCapability)` against active local Campaign Authority Bindings. Role bundles (`campaignOwner`, `gameMaster`, `player`, `authoringTool`, `viewer`) may exist as assignable bundles of capabilities for convenience, but caller-supplied role names or capability claims never self-authorize — only capabilities resolved from an active local binding do.

The initial capability taxonomy follows a justified-split rule: a capability earns a distinct ID only when it represents a materially different authority level, exposure risk, mutation risk, recovery risk, or test assertion. The taxonomy concentrates this splitting around privileged knowledge, authoritative mutation, Package lifecycle, import, rollback, and Retcon operations (for example `resolution.apply` vs. `resolution.override`, or `persistence.import.campaign` vs. the higher-risk `persistence.import.destructive`), rather than producing an large undifferentiated permission matrix.

### Campaign Authority Binding

This authorization model was added by decision [DEC-001](../../reference/decision-log.md) ("Campaign-Scoped Principal Authority Binding"), which established that Campaign authority must be a durable, inspectable binding rather than implicit trust in caller-supplied roles or the local process/session. HLA-CONTRACT owns and evaluates the resulting `CampaignAuthorityBinding`:

- Campaign creation atomically establishes an initial active `campaignOwner` binding for the creating/default local Principal (owned by [HLA-LIFECYCLE](lifecycle.md) at the operation level, evaluated/recorded by HLA-CONTRACT).
- Campaign import requires an authorized local importer and creates a **new** active local binding for that importer; any foreign Principal or binding identifiers carried in the imported artifact are retained only as non-authoritative provenance and never become active local grants.
- HLA-PERSIST stores bindings and binding provenance durably but does not itself decide whether a request is authorized — that evaluation stays in HLA-CONTRACT.

DEC-001 also confirms this responsibility fits within the existing ten-component HLA rather than justifying an eleventh, separate authorization component: hosted accounts, authentication providers, invitations, and multiplayer authority administration remain explicitly deferred.

## Request/response contract envelope

Every public operation uses the same conceptual envelope shape:

- `ContractRequest` — `operationId`, `callerContext` (principal/actor/session/caller-class/declared grants — descriptive only, not privilege-granting), `targetContext` (the Campaign/Package/Observer/Projection/artifact/Checkpoint/Resolution being addressed), `synchronizationContext`, and `payload`.
- `ContractResponse` — `outcome` (one of `accepted`, `rejected`, `unauthorized`, `notFound`, `conflict`, `validationFailed`, `componentFailure`), `result`, `diagnostics`, `synchronizationState`, and `provenanceRef`.

HLA-CONTRACT validates only envelope shape; payload domain validation is forwarded to the owning component. Rejected, unauthorized, malformed, or conflict responses are guaranteed to have no side effect, and accepted mutating requests are forwarded exactly once unless a component-level idempotency contract later specifies safe retry.

## Public operation groups

The catalog groups operations by owning component, so a change to one component's internal behavior does not require redefining unrelated public contracts (this is the NFR-005 maintainability mechanism):

| Operation Group | Owning Component | Mutating? |
|---|---|---|
| Campaign Lifecycle | [HLA-LIFECYCLE](lifecycle.md) | Mixed |
| Resolution / Resolution Provenance | [HLA-RESOLUTION](resolution.md) | Mixed / read-only |
| Observer Knowledge | [HLA-OBSERVER](observer.md) | Mixed |
| Package | [HLA-PACKAGE](package.md) | Mixed |
| Query | [HLA-QUERY](query.md) | Read-only (no authoritative mutation) |
| Persistence | [HLA-PERSIST](persist.md) | Mixed |
| State | [HLA-STATE](state.md) | Yes |
| Validation | [HLA-VALIDATE](validate.md) | Read-only |
| Authority | HLA-CONTRACT itself | Mixed |

Package registration, Campaign import, and Oracle result acceptance can never bypass HLA-VALIDATE regardless of which operation group routes to them.

## Client synchronization contract

HLA-CONTRACT owns synchronization *envelope semantics* only — `stateTransferMode` (`full`, `delta`, `compactedDelta`, `unchanged`, or `unavailable`) and the request/response synchronization vocabulary — but it does not itself materialize full state, deltas, or Presentation Models; that materialization is HLA-QUERY's responsibility. Synchronization uses **separate revision streams** for Campaign Reality, Observer Knowledge, and Presentation Models rather than one global Campaign counter, and `deltaNumber` is always stream-scoped. The contract is transport-neutral: it requires no network connection, subscription channel, or server push model, keeping the door open for embedded, in-process, and future network adapters without deciding among them.

## Failure semantics

Every failure category has a defined `outcome` and a defined side-effect posture — the recurring rule is that anything other than `accepted` (or `conflict`/`accepted` for stale synchronization state that falls back to full state) has **no side effect**. Notably: if Campaign creation cannot commit its initial `campaignOwner` binding, Campaign creation fails atomically and no authoritative Campaign remains; if import cannot create the authorized importer's local binding, import fails atomically and the foreign source binding stays non-authoritative provenance only. HLA-CONTRACT does not retry mutating operations by default — retry/idempotency behavior must be explicitly designed per operation before implementation.

## Non-functional derivation

- **NFR-001 (Performance):** the contract is an in-process facade by default; transport adapters may wrap it later, but the component contract itself carries no required network overhead.
- **NFR-002 (Security):** authorization is enforced before forwarding, and mandatory HLA-VALIDATE routing for untrusted content is preserved.
- **NFR-005 (Maintainability):** operation groups route to owning components, isolating unrelated contract changes.
- **NFR-006 (Auditability):** caller context and provenance-relevant metadata are forwarded to mutating components so consequential changes can record source.
- **NFR-007 (Authorization):** active Campaign Authority Bindings and explicit capability checks gate every Campaign-scoped operation before forwarding.
