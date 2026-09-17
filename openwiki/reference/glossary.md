---
type: reference
title: "Glossary"
description: "Campistoria terminology consolidated from ideation, requirements, architecture, and detailed design artifacts, grouped by theme."
tags: [reference, glossary, terminology]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-b95f8916e05b902dc80950a3
    resource: repo://docs/glossary.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Glossary

`docs/glossary.md` is the shared working reference for Campistoria terminology across Ideation, Requirements, Architecture, and Detailed Design artifacts. Definitions in approved phase artifacts remain authoritative for those artifacts — this glossary is a working reference, not a mechanism for retroactively changing approved requirements or architecture.

## Campaign reality and history

- **Campaign** — a persistent, mutable instance of play with durable identity, Campaign Reality, history, package composition, Observer Knowledge, Unresolved State, and user-owned data.
- **Campaign Reality** — the authoritative record of what is established as true in the campaign.
- **Campaign Time** — the ordering and temporal frame in which campaign Events and State changes occur, independent of any package-specific calendar or time unit.
- **Entity** — a campaign object with persistent identity; Packages define domain-specific entity types.
- **Location** — an Entity capable of participating in spatial relationships and spatial projections.
- **Relationship** — a first-class connection between campaign objects; Packages define relationship categories and semantics.
- **Property** — a value or attribute associated with an Entity, Relationship, State, Event, or other campaign object.
- **Fact** — an established assertion about Campaign Reality, with provenance and temporal validity.
- **State** — the current or temporally bounded condition of campaign objects.
- **Event** — a recorded occurrence or change in Campaign Reality, supporting continuity, provenance, temporal query, reconstruction, and auditability.
- **Event Sourcing** — the design pattern in which the ordered Event stream is the authoritative source of state, and current/historical state is derived by replaying or folding Events. See [HLA-CORE](../architecture/components/core.md).
- **Fold** — deterministic reconstruction of state by applying an ordered sequence of committed Events to an initial or snapshot state until the requested boundary is reached.
- **Authoritative State** — engine-owned state that determines what is true for its domain (Campaign Reality, Observer Knowledge), distinct from derived views, caches, renderings, or diagnostics.

## Unresolved state and resolution

- **Unresolved State** — explicitly represented campaign information that has not yet been resolved.
- **Resolution** — the process by which Unresolved State becomes established Campaign Reality through an attributable human decision, package-defined procedure, rule, or accepted Oracle result.
- **Resolution Command** / **Resolution Record** — the internal command describing an accepted resolution attempt, and the provenance-bearing record of its outcome. See [HLA-RESOLUTION](../architecture/components/resolution.md).
- **Oracle** — a defined process that produces uncertain or randomized output; an Oracle result does not inherently change Campaign Reality.
- **Procedure** — a package-defined sequence used during play, which may consult Rules and Oracles and produce Events or (un)resolved campaign information.
- **Rule** — package-defined semantics that interpret campaign information mechanically.
- **Deterministic/Probabilistic Boundary** — the boundary separating deterministic authoritative mechanics and state changes from probabilistic Oracle output or other uncertain input.

## Observer knowledge

- **Observer** — a viewpoint whose knowledge may differ from Campaign Reality (unrelated to the Gang-of-Four Observer design pattern). See [HLA-OBSERVER](../architecture/components/observer.md).
- **Observer Knowledge** — what an Observer knows, believes, suspects, remembers, has been told, has mapped, has inferred, or has misunderstood.
- **Information Source** — the provenance of a unit of Observer Knowledge (direct observation, map, rumor, NPC statement, document, sensor reading, Oracle result, inference).

## Packages and composition

- **Package** — a versioned, identifiable unit contributing definitions, behavior, content, rules, procedures, or assets to a Campaign.
- **Package Artifact** — the structured representation of one Package version, including identity, metadata, dependencies, definitions, contracts, and declarations.
- **Effective Campaign Definition** — the resolved package composition, extensions, overrides, and dependencies used to instantiate or migrate a Campaign.
- **Package Composition Pin** — a recorded reference to the exact effective package composition used by a Campaign.
- **Package Semantic Reference** — a stable reference from Campaign Reality to package-defined meaning (an entity type, relationship type, rule, Oracle, schema, asset, etc.). See [HLA-PACKAGE](../architecture/components/package.md).
- **Scenario** — an immutable package-defined starting configuration from which a Campaign may be instantiated.

## Presentation and synchronization

- **Presentation Model** — engine-produced information a client may render for a given observer/context/time/projection; derived, never authoritative.
- **Projection** — a purpose-specific representation of campaign information (map, timeline, relationship view, journal index, roster). See [HLA-QUERY](../architecture/components/query.md).
- **Delta** / **Delta Number** — a caller-visible change set after a known revision, and the stream-scoped (never global) marker identifying that revision.
- **Full State Transfer** — the synchronization mode returning a full caller-appropriate representation rather than a delta.

## Authorization

- **Principal** — the authenticated or otherwise locally identified security subject represented at the contract boundary.
- **Actor** — the runtime participant or tool acting through a session (player, GM-facing tool, authoring tool, integration).
- **Session** — a bounded interaction context between a caller and HLA-CONTRACT; Session claims never authorize an operation independently of the active Campaign Authority Binding.
- **Campaign Authority Binding** — the durable, Campaign-scoped association between a locally recognized Principal and the capability bundle/explicit capabilities that Principal may exercise for that Campaign. Imported foreign bindings are provenance only, never active local grants. See [HLA-CONTRACT](../architecture/components/contract.md).
- **Campaign Owner** — the initial local Principal bound to a Campaign with the `campaignOwner` capability bundle (an authorization role bundle, not a claim of software/project ownership).
- **Capability** — a named permission checked by HLA-CONTRACT before forwarding an operation; roles may grant capability bundles, but authorization always resolves to explicit capabilities.
- **Capability-Based Access Control (CBAC)** — authorization based on explicit required capabilities rather than hardcoded role names.
- **Role-Based Access Control (RBAC)** — in this project, roles assign capability bundles, but the actual checks resolve through CBAC.
- **Game Master / GM** — the human runtime authority when play style includes a GM-like role — a runtime authority concept, not a project governance role.

## State management

- **Checkpoint** — a deliberate, recoverable campaign state used for save, restore, recovery, or replay-bounding purposes.
- **Snapshot** — a derived representation of component-owned state at a known Event boundary, used to bound replay cost; never more authoritative than committed Events.
- **Recovery** — restoration of valid state after technical failure; explicitly not campaign history.
- **Undo** — reversal of an accidental or unwanted operation; does not rewrite campaign history and is distinct from Retcon.
- **Retcon** — human-authorized revision of established Campaign Reality, appending provenance-preserving correction history rather than rewriting prior Events. See [HLA-STATE](../architecture/components/state.md).

## Persistence and scale

- **Active Working Set** — the recent portion of campaign data kept readily available for ordinary operation, distinct from archived history retrievable on demand.
- **AssetRef** — a semantic reference to an asset used by Campaign Reality or Observer Knowledge, without embedding payload bytes or storage mechanics in authoritative state. See [HLA-PERSIST](../architecture/components/persist.md).
- **Provenance** — information identifying the source, mechanism, time, and context by which a piece of state, knowledge, or diagnostic came to exist.
- **Transaction** — a single call to a public engine operation, measured from call receipt to result return, exclusive of caller-side network/transport time.

## Project-level constraints

- **HLA** — High-Level Architecture, the approved architectural decomposition and boundary model for the engine.
- **Human Authority** — the principle that consequential narrative interpretation, world-state change, uncertainty resolution, exceptions, retcons, and canon remain under human authority.
- **Model Independence** — the constraint that the engine concept does not depend on a specific AI model, provider, prompt format, or AI runtime.
- **Ruleset Independence** — the constraint that engine mechanics do not embed a specific RPG system, setting, genre, edition, or rules vocabulary as intrinsic semantics.
- **Virtual Tabletop / VTT** — a play-surface application category (maps, tokens, live table interaction, chat, media); a VTT may consume Campistoria engine contracts, but the engine itself is not a VTT.
