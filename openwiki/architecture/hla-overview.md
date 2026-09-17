---
type: architecture-overview
title: "High-Level Architecture Overview"
description: "The Campistoria engine's Hexagonal Architecture: HLA-CORE at the center with no outward dependencies, ten major components as ports, the deterministic-probabilistic boundary, and cross-cutting NFR enforcement."
tags: [architecture, hla, hexagonal-architecture, system-design]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# High-Level Architecture Overview

The High-Level Architecture (HLA) translates the [Campistoria concept and SRS requirements](../requirements/srs-overview.md) into structural form: system boundaries, ten major components, their governing design patterns, and how non-functional requirements are enforced structurally rather than aspirationally. It commits to no implementation language, storage technology, or deployment framework — those remain open per the SRS unless a structural pattern chosen here narrows that space without fully deciding it.

## System context

There are no external *systems* in architectural scope — cloud services, multiplayer infrastructure, and AI integration are all deferred. The only external interfaces are client, authoring-tool, and third-party-integration actors, all interacting exclusively through [HLA-CONTRACT](components/contract.md). Two trust boundaries apply: Package content and imported Campaign data are untrusted until they pass [HLA-VALIDATE](components/validate.md) (NFR-002), and client/authoring-tool requests are always subject to POV Resolution before any content is returned — the engine, not the caller, decides what is valid to present.

## Architectural style: Hexagonal Architecture

The engine's defining structural requirement is **symmetry** — FR-039 requires first-party and third-party clients, authoring tools, and Packages to all interact through identical public contracts, with no privileged internal shortcut. **Hexagonal Architecture (Ports and Adapters, Cockburn)** expresses this directly: the engine core exposes a fixed set of ports (Query, Resolution, Package, Persistence), and every external actor — reference client or third-party tool alike — is just another adapter plugged into the same port. No actor is structurally closer to the core than any other.

Two alternatives were considered and rejected as the *overall* style:

- **Layered Architecture** — rejected because it implies a single directional stack with one primary caller "on top," which conflicts with FR-039's requirement that multiple peer caller types have equally direct access.
- **Event-Driven Architecture** — rejected at the system level because the dominant interaction shape in the SRS is synchronous request/response, not asynchronous pub/sub between independently-deployed services; adopting it as the overall style would imply a distributed execution model no FR requires and would foreclose the embedded-library deployment option the SRS deliberately leaves open. (Event Sourcing — a related but distinct pattern concerned with *how state is persisted*, not *how services communicate* — is still used internally in HLA-CORE and HLA-OBSERVER.)

**Microkernel** was considered seriously (Package composition is genuinely a plugin problem) but not adopted for the whole engine — it's scoped specifically to [HLA-PACKAGE](components/package.md), because most other components are fixed engine responsibilities, not plugin-shaped.

**Dependency direction rule:** ports point inward; adapters depend on ports, never the reverse. [HLA-CORE](components/core.md) sits at the center with no outward dependencies. Resolution, Observer Knowledge, and Package Composition depend on Core but not on each other, except where explicitly noted (Resolution depends on Package for Oracle/Procedure/Rule references). Query, Persistence, Validation, and the Public Contract Facade depend on the components below them but are never depended upon by them.

## The ten components

| Component | Responsibility | Governing pattern(s) |
|---|---|---|
| [HLA-LIFECYCLE](components/lifecycle.md) | Campaign instantiation from immutable Scenario | Factory Method |
| [HLA-CORE](components/core.md) | Campaign Reality & history — the center of the hexagon | Event Sourcing |
| [HLA-RESOLUTION](components/resolution.md) | Unresolved State, Resolution, the Oracle boundary | Command + Adapter |
| [HLA-OBSERVER](components/observer.md) | Observer Knowledge, structurally separate from Campaign Reality | Event Sourcing (independent stream) |
| [HLA-PACKAGE](components/package.md) | Package composition & versioning | Microkernel + Builder |
| [HLA-QUERY](components/query.md) | Query, POV Resolution & Presentation | Facade + Strategy |
| [HLA-PERSIST](components/persist.md) | Persistence & portability | Adapter |
| [HLA-STATE](components/state.md) | Checkpoint, Undo, Recovery, Retcon | Memento + reused Command |
| [HLA-VALIDATE](components/validate.md) | Validation & diagnostics at every trust boundary | Chain of Responsibility |
| [HLA-CONTRACT](components/contract.md) | Public contract facade & authorization | Facade |

Every component traces to at least one Requirement ID, and every FR group in the SRS maps to at least one component — there are no orphan requirements or untraceable structural elements.

## The deterministic–probabilistic boundary

The only probabilistic component in the system is a package-defined **Oracle** (FR-014). Oracle invocation is structurally isolated behind a single, engine-owned, generic **Oracle Adapter** inside HLA-RESOLUTION — every Package-defined Oracle conforms to the same interface rather than supplying its own bespoke Adapter. The Adapter's job is narrow and fixed: normalize output into the engine's uniform Resolution-input contract. It has **no authority to write Campaign Reality.**

Every Oracle result passes through HLA-VALIDATE's Chain of Responsibility against the Package's declared output contract before HLA-RESOLUTION's Command layer will accept it as Resolution input. **Containment logic:** only a Resolution Command may write to Campaign Reality — Oracle Adapters, Procedures, and Rules are all upstream of that single write path, so there is exactly one gate through which probabilistic input can become deterministic state. If the Adapter cannot produce a valid result, the associated element remains Unresolved — no default or synthesized value is ever substituted. Every Resolution Command records its source (human decision, Procedure, Rule, or Oracle Adapter invocation) as part of the Event it appends, satisfying auditability. There is no silent modality blending: nothing outside HLA-RESOLUTION's Command layer can mutate Campaign Reality.

## Data architecture

**Persistence model:** an append-only Event log per Campaign (HLA-CORE) and per Observer (HLA-OBSERVER), with periodic Memento-pattern snapshots (HLA-STATE) bounding replay cost. The active working set is the most recent segment of that log plus its nearest snapshot; older segments move to archival storage accessible on demand without being loaded for ordinary queries.

**Ownership boundaries:** Campaign Reality and Observer Knowledge are separate logs, never merged. Package definitions are independent of any specific Campaign's data. Campaign Authority Bindings are Campaign-scoped authorization metadata owned and evaluated by HLA-CONTRACT (durable representation delegated to HLA-PERSIST) — they are not Campaign Reality or Campaign history. Presentation Models are never persisted as authoritative data; they're always recomputed from the logs above.

**Consistency guarantees:** each Resolution Command is atomic with respect to Campaign Reality — it either fully appends its Event or has no effect. Package migration is likewise atomic: a failed migration leaves the prior pinned composition fully intact, never a mixture of old and new. Package content and imported Campaign data both flow into HLA-CORE/HLA-RESOLUTION or HLA-CORE/HLA-OBSERVER only after HLA-VALIDATE clears them — no component reads another's owned data without going through that owner's interface, and there is no fourth path into Campaign Reality that bypasses Package registration, Campaign import, or Oracle result acceptance as validation checkpoints.

## Non-functional architecture

| NFR | Structural enforcement |
|---|---|
| NFR-001 (Performance) | HLA-CONTRACT is an in-process Facade call, not a network boundary by default |
| NFR-002 (Security) | HLA-VALIDATE's Chain of Responsibility is the mandatory gate before Package/import content reaches HLA-CORE/HLA-PACKAGE — no component-level bypass exists |
| NFR-003 (Reliability) | Event Sourcing plus Memento-based Checkpoint/Recovery means recovery is "replay from last valid snapshot," not ad hoc repair |
| NFR-004 (Scalability) | The active-window/archival split is the direct structural implementation of the bounded-working-set requirement |
| NFR-005 (Maintainability) | Hexagonal ports isolate each component behind a fixed interface; HLA-PACKAGE's Microkernel boundary isolates Package semantics from engine mechanics |
| NFR-006 (Auditability) | Event Sourcing's append-only log is inherently an audit trail; Resolution's Command pattern additionally records source on every write |
| NFR-007 (Authorization) | HLA-CONTRACT evaluates active Campaign Authority Bindings and explicit capabilities before forwarding Campaign-scoped operations |

## Failure posture

Deterministic failures (Package/import validation failure, migration failure) leave prior state fully intact with structured diagnostics — never a partial state change. Probabilistic failures (Oracle Adapter failure or contract-violating result) leave the element Unresolved with no synthesized fallback. If archival storage is unreachable, the active working set remains fully queryable — only explicit archival retrieval calls fail, with a diagnostic rather than a silent empty result. Recovery is Event log replay from the last valid Checkpoint. Undo reverses a single Resolution Command without becoming history; Retcon appends a compensating Event rather than rewriting the log, preserving Event Sourcing's append-only guarantee even under correction.

**Risk concentration:** HLA-CORE's Event log and HLA-VALIDATE's gate are the two components whose failure has the widest blast radius, since every other component depends on one or both — both are flagged High priority in the architecture's risk assessment, alongside risks that the HLA-CONTRACT Facade could accumulate a first-party-only "back door" during implementation, or that mutating client operations could be validated for shape but not authorized by actor capability.

## Deployment and orchestration

No target runtime, containerization approach, networking model, build tooling, or CI/CD platform is selected at this phase — Hexagonal Architecture is specifically what preserves that openness structurally. The engine core (HLA-CORE through HLA-VALIDATE) is architecturally a single deployable unit with no internal network dependency; HLA-CONTRACT is the only component whose deployment shape (embedded vs. hosted) is deliberately left open by design.
