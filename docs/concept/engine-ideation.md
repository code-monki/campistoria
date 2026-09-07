# Campistoria Engine Ideation

## Purpose

This document records the conceptual baseline for the Campistoria engine before formal requirements engineering begins. It consolidates the ideation work into a durable source for later requirements, architecture, SRD/specification, implementation, and agent handoff artifacts.

This is an ideation artifact, not a requirements specification or architecture decision record. It establishes vocabulary, problem framing, conceptual boundaries, success criteria, risks, and deliberate non-decisions.

## Problem Statement

Existing solo RPG support tools tend to solve isolated pieces of play: journals, maps, random tables, oracles, encounter generators, campaign notes, or virtual tabletop displays. Those tools are useful, but they often leave the player responsible for manually maintaining the deeper structure of campaign continuity: what is true, what is merely suspected, what has changed over time, what remains unresolved, which version of a rules/setting/scenario definition gave meaning to the campaign, and what each observer actually knows.

Campistoria addresses the need for an implementation-neutral campaign engine that maintains an evolving, persistent campaign reality for solo and related RPG play. The engine must support both pre-authored and emergent campaign material; preserve the distinction between established reality and observer knowledge; allow game-system, setting, scenario, oracle, and procedure semantics to come from reusable packages; and provide durable, user-owned campaign data that can outlive any particular client or engine implementation.

The engine is not intended to replace the human player, GM, author, or interpreter. It should help preserve and query campaign continuity without taking narrative authority away from the human.

## Stakeholders

This section identifies stakeholders in the project and concept. It is distinct from runtime Human Authority, which describes authority during play.

Primary stakeholders:

- Human player / GM: exercises final runtime authority over narrative interpretation, world-state changes, uncertainty, exceptions, retcons, and what becomes canonical during play.
- Solo RPG player: uses the engine to maintain continuity, inspect campaign state, preserve hidden or partial knowledge, and play long-running campaigns without manually reconciling disconnected tools.
- Project owner: holds project-level scope, lifecycle advancement, and acceptance authority during the current Ideation-to-Requirements process.
- Engine developer / maintainer: implements and evolves the campaign runtime while preserving the settled conceptual boundaries, especially the separation between Campaign Reality, Observer Knowledge, packages, clients, authoring tools, and humans.

Secondary stakeholders:

- Package authors: define reusable RPG systems, settings, scenarios, rules, oracles, procedures, house rules, schemas, and assets consumed by the engine through public contracts.
- Reference client developers: build interfaces that render and interact with engine-provided Presentation Models without owning campaign truth.
- Authoring tool developers: build tools for creating and validating packages and scenarios through public contracts rather than privileged internals.
- Third-party tool and integration developers: may import, export, inspect, validate, or extend campaign and package data through documented interfaces.

During the current lifecycle stage, project-level decision and advancement authority remains with the project owner. Runtime authority remains governed by the Human Authority section and does not make the human player / GM a RACI owner for every project artifact or implementation choice.

Stakeholder priorities may conflict. Convenience, automation, visual polish, model-assisted generation, package flexibility, client autonomy, and implementation speed must not override human authority, inspectable authoritative state, durable user-owned data, reproducibility where required, open interoperability, lifecycle governance, or the separation between deterministic campaign mechanics and probabilistic interpretation or generation.

## Conceptual Constraints

The following are conceptual constraints on the engine regardless of later implementation choices. They are constraints because they limit acceptable requirements, architectures, implementations, and integrations; they are not deferred technology selections.

- Human authority: the engine must preserve human authority over consequential narrative interpretation, world-state changes, uncertainty resolution, exceptions, retcons, and canon.
- Deterministic/probabilistic separation: deterministic campaign mechanics, authoritative state changes, and package-defined procedures must remain distinguishable from probabilistic oracle use, interpretation, generation, or model-assisted text.
- Model independence: the engine concept must not depend on a particular AI model, provider, proprietary model behavior, model availability, prompt format, or AI runtime. AI is not an engine capability.
- Ruleset independence: the engine must not embed one RPG system, setting, genre, edition, or rules vocabulary as intrinsic engine semantics. Game-specific meaning belongs in packages.
- Inspectable authoritative state: Campaign Reality, Observer Knowledge, material state transitions, package composition, and provenance-relevant information must be representable and inspectable independently of prose narration or client rendering.
- Portability: durable campaign and package data must remain user-owned and portable outside transient client storage, opaque application databases, or a single deployment environment.
- Lifecycle governance: requirements, architecture, implementation, and migration decisions that materially affect the approved concept must preserve the distinction between settled conceptual decisions and open implementation decisions unless the project owner explicitly reopens the concept.

No fixed implementation language, storage technology, database, file format, UI framework, rendering technology, deployment platform, quantitative performance target, delivery schedule, or monetary budget is imposed during Ideation unless this document explicitly says otherwise. Those matters remain deliberately open for Requirements, Architecture, project planning, or later trade study.

## Preliminary Non-Functional Signals

These signals identify concerns that Requirements should examine. They are not measurable non-functional requirements yet.

- Performance: the engine should support interactive solo play without making state inspection, resolution, observation, projection, or persistence feel disruptive to the play loop. Quantitative latency, throughput, and working-set targets are deferred to Requirements.
- Security: packages, imported campaign data, external assets, generated text, integrations, and possible future AI-assisted workflows create trust boundaries. Requirements should define validation, isolation, permissions, and unsafe-content handling without assuming a particular security architecture during Ideation.
- Reliability: authoritative campaign state should survive ordinary failures without silent corruption, uncontrolled reinterpretation, or loss of provenance needed to recover or explain consequential changes. Concrete durability, backup, recovery, and migration requirements are deferred.
- Scalability: campaign lifetime size may exceed active working-set size. Requirements should distinguish campaign scale, session scale, package scale, archival scale, and any future multi-user or cloud scale before setting targets.
- Maintainability: engine mechanics, package semantics, persistence, projection, validation, diagnostics, client rendering, authoring tools, and integrations should remain separable enough to evolve without collapsing conceptual boundaries.
- Auditability: consequential state changes, package composition, migrations, retcons, AI-mediated proposals if any, and authority decisions should carry enough provenance to support inspection and explanation. Exact logging, audit trail, and retention requirements are deferred.

## Current Lifecycle Status

Ideation has produced a stable conceptual baseline. The next project activity is requirements engineering unless a later approved artifact explicitly changes the lifecycle state.

The Ideation → Requirements phase gate (`ai-toolkit/02-governance/12-phase-gate-checklist.md`, Gate 1→2) was reviewed against all seven mandatory exit criteria — problem statement, stakeholders, constraints, risks, success criteria, alternative solution paths, and absence of unresolved ambiguity — and found satisfied. The project owner granted explicit approval to advance to the Requirements phase as of that review (2026-09-07). Requirements engineering is the current active lifecycle phase.

The following remain intentionally undecided until later lifecycle phases:

- Programming language.
- UI framework.
- Database or storage engine.
- Serialization format.
- Package manifest syntax.
- Specific client architecture.
- Rendering technology.
- Deployment or packaging model.

No implementation technology should be selected from this document alone.

## Conceptual Success Criteria

Campistoria succeeds conceptually if it can:

- Maintain authoritative campaign reality over long-running play.
- Represent unresolved campaign information without forcing answers to exist prematurely.
- Preserve the distinction between reality, observation, belief, rumor, and stale or incorrect knowledge.
- Record events, state changes, provenance, and temporal validity.
- Support both authored scenarios and emergent solo play.
- Allow reusable packages to define game, setting, scenario, oracle, procedure, and capability semantics.
- Keep first-party and third-party packages on the same public footing.
- Let clients render maps, timelines, rosters, journals, graphs, and other views without those views owning campaign reality.
- Preserve user ownership and long-term interpretability of campaign data.
- Avoid embedding AI, cloud services, multiplayer, or a specific UI model as engine assumptions.

## Core Principle

The engine maintains campaign reality; packages provide game and world semantics; clients present and interact with the campaign; authoring tools create packages; humans retain authority over play. AI is not an engine capability or dependency.

## Normalized Conceptual Model

At a high level:

```text
Campaign
|
+-- Campaign Reality
|   +-- Entities
|   |   +-- Locations
|   +-- Properties
|   +-- Relationships
|   +-- Facts
|   +-- State
|   +-- Environment / Conditions
|   +-- Unresolved State
|   +-- Events / History
|
+-- Campaign Time
|
+-- Observers
|   +-- Observer Knowledge
|       +-- Information Sources
|
+-- Resolution
|   +-- Human Decisions
|   +-- Oracles
|   +-- Procedures
|   +-- Rules
|
+-- Package Composition
|
+-- Projections / API
    +-- Clients
```

Packages define most vocabulary inside these generic structures. The engine knows the meta-model; packages define game and world semantics.

## Vocabulary

**Campaign**  
A persistent, mutable instance of play with durable identity, campaign reality, history, package composition, observer knowledge, unresolved state, and user-owned data.

**Campaign Reality**  
The authoritative record of what is established as true in the campaign, including entities, locations, facts, relationships, state, conditions, unresolved state, and history.

**Entity**  
A campaign object with identity. Packages define entity types such as character, faction, ship, world, monster, settlement, clue, or organization.

**Location**  
An entity capable of participating in spatial relationships and spatial projections. Location deserves special conceptual treatment because containment, topology, scale, coordinates, spatial queries, and map projections are common campaign operations.

**Property**  
A value or attribute associated with an entity, relationship, state, event, or other campaign object. Packages define meaning and validation.

**Relationship**  
A first-class connection between campaign objects. Relationships may be spatial, social, political, causal, ownership-based, hierarchical, procedural, or package-defined.

**Fact**  
An established assertion about campaign reality. Facts may have provenance, temporal validity, confidence or certainty semantics supplied by packages, and relationships to events or information sources.

**State**  
Current or temporally bounded condition of campaign objects. State may change as the campaign evolves.

**Campaign Time**  
The ordering and temporal frame in which campaign events and state changes occur. Packages may impose game-specific time units or calendars, but the engine needs a generic temporal concept.

**Event / History**  
An occurrence or recorded change in campaign reality. Events support continuity, provenance, query, reconstruction, and long-term campaign memory.

**Unresolved State**  
Explicitly represented campaign information that has not yet been resolved. An unresolved question may be constrained without requiring the answer to exist in advance.

**Resolution**  
The process by which unresolved state becomes established campaign reality. Resolution may involve human decisions, package-defined procedures, rules, or oracle results.

**Oracle**  
A defined process that produces uncertain or randomized output. An oracle result does not inherently change campaign reality. Resolution determines what, if anything, the result establishes.

**Procedure**  
A defined sequence used during play, such as travel, encounter generation, downtime, trade, weather transition, mystery progress, or NPC generation. Procedures may consult rules and oracles and may produce events, state changes, or unresolved/resolved campaign information.

**Rule**  
Package-defined semantics that interpret campaign information mechanically. Campaign reality describes what is true; rules describe what those truths mean mechanically.

**Observer**  
A character, party, faction, player-facing perspective, sensor platform, or other viewpoint whose knowledge may differ from campaign reality.

**Observer Knowledge**  
What an observer knows, believes, suspects, remembers, has been told, has mapped, has inferred, or has misunderstood. Knowledge may be correct, incorrect, incomplete, approximate, ambiguous, or stale.

**Information Source**  
The origin of observer knowledge, such as direct observation, map, rumor, NPC statement, document, sensor reading, oracle result, or inference. Source provenance prevents observer knowledge from contaminating campaign reality.

**Package**  
A versioned, identifiable unit that contributes definitions, behavior, or content to a campaign.

**Scenario**  
An immutable package-defined starting configuration from which a campaign may be instantiated. The scenario defines initial reality; the campaign owns evolving reality.

**Projection**  
A purpose-specific representation of campaign information, such as a map, timeline, NPC roster, relationship view, journal index, current scene, subsector display, or dungeon view. A projection does not own campaign reality.

**Presentation Model**  
Engine-produced information a client may render for a particular observer, context, time, and projection.

**Point of View**  
The observer/context basis for deciding what information is appropriate to present. POV includes more than line of sight; it includes knowledge, maps, documents, rumors, sensor information, memory, and belief.

**Checkpoint**  
A known recoverable campaign state.

**Undo**  
Reversal of an accidental or unwanted operation. Undo is not campaign history.

**Recovery**  
Restoration of valid campaign state after technical failure. Recovery is not campaign history.

**Retcon**  
An intentional revision of established campaign reality under human authority. A retcon is not ordinary undo.

## Human Authority

The human may establish reality, resolve uncertainty, interpret oracle results, reject generated results, override where permitted, correct mistakes, and retcon established reality.

Packages constrain game semantics. The engine maintains consistency and provenance. Neither the engine nor a package usurps the human as campaign authority.

## Engine Boundary

The engine owns:

- Campaign identity and lifecycle.
- Campaign reality.
- Campaign time.
- Entities, locations, facts, relationships, state, unresolved state, and events at the meta-model level.
- Observer knowledge and information-source separation.
- Resolution mechanics as public engine concepts.
- Package composition, dependency awareness, capability resolution, version pinning, and validation.
- Public contracts for queries, POV resolution, projections, and presentation models.
- Durable persistence concepts.
- Checkpoints, recovery, undo, and retcon as distinct concepts.
- Import/export and implementation-neutral durable representation.
- Structured validation and diagnostic information.

The engine does not own:

- Human interpretation of play.
- Game-system-specific rules, ontologies, or vocabulary.
- Setting-specific meaning.
- Scenario authorship.
- Rendering.
- UI layout or client presentation.
- Rich authoring environments.
- Arbitrary executable plugin behavior.
- AI interpretation or generation.
- Cloud services.
- Multiplayer infrastructure.
- Commercial package ecosystem concerns.

## Package Model

A package is a versioned, identifiable unit that contributes definitions, behavior, or content to a campaign.

Common package roles include:

- System Definition Package: reusable game-system concepts and mechanics.
- Setting Package: world-specific definitions and content.
- Scenario Package: immutable starting state for play.
- Focused Capability Package: reusable capabilities such as weather, travel, trade, oracle, encounter, or downtime procedures.
- User Override Package: house rules and deliberate user customizations.

System Definition Packages are intended to be reusable shared foundations. A scenario or setting should reference an existing system definition rather than duplicate it unless it deliberately defines a new or incompatible system variant.

Package composition conceptually produces an Effective Campaign Definition:

```text
Engine
  +
System Definition Package
  +
Setting Package
  +
Scenario Package
  +
Optional Rule / Oracle / Procedure Packages
  +
User Overrides
  =
Effective Campaign Definition
```

Package principles:

- Packages are versioned units of definition, behavior, or content.
- Package version and RPG system edition are independent metadata.
- Settings are not inherently bound to a single rules edition.
- Packages depend on capabilities where possible and specific packages where necessary.
- Composition is explicit; collision is not composition.
- Extension and override are distinct operations.
- House rules and user customizations are packages, not hidden mutations.
- System templates and first-party content use the same public package mechanisms as third-party content.
- Campaigns pin their effective package composition.
- Package upgrades are explicit migrations, never silent replacement.
- The engine knows the meta-model; packages know the game and world model.

## Scenario And Campaign Relationship

A scenario is immutable starting material. A campaign instantiated from a scenario receives its own durable identity and mutable reality.

Two campaigns may instantiate from the same scenario and diverge permanently. Later scenario updates have no automatic authority over existing campaigns. Reconciliation between an updated scenario and a running campaign is an explicit migration or human-directed operation.

## Resolution And Emergence

The engine must support emergent play in which facts are not all known or defined at campaign start.

An unresolved wilderness hex, unanswered mystery, unknown NPC motive, or undefined political relationship may be represented explicitly. A package-defined oracle or procedure may produce a result. The human or a procedure then resolves what that result establishes.

Core distinction:

```text
Oracle result
    produces input

Resolution
    establishes campaign reality

Observation
    changes observer knowledge
```

This distinction supports solo play without turning the engine into a storyteller.

## Observer Knowledge

Campaign reality and observer knowledge are separate. The campaign may know that a bridge has collapsed while the player character still believes the bridge is intact. A map may be outdated. An NPC statement may be false. A rumor may be partially correct. A sensor reading may be approximate.

The engine must preserve these differences rather than flattening them into one global truth visible to every observer.

Observer knowledge is not only for multiplayer or GM secrecy. It matters even in a single-player solo campaign because the human player may maintain a distinction between what has become true in the campaign and what the character has learned, believes, or suspects.

## Projections And Presentation

The map is a projection, not the world.

Maps, timelines, rosters, relationship graphs, journals, current-scene views, subsector views, dungeon views, and dashboards are ways of presenting campaign information. They do not own campaign reality.

Conceptually:

```text
Campaign Reality
    +
Observer Knowledge
    +
Context
    +
Time
    |
    v
POV Resolution
    |
    v
Presentation Model
    |
    v
Client Renderer
```

This keeps rendering and UI technology outside the engine contract.

## Persistence And Portability

Durable campaign persistence is fundamental. A campaign must survive normal close/reopen behavior, technical failure, long-running play, migration between machines, package upgrades, and eventual application replacement.

Conceptual persistence principles:

- Campaign data belongs to the user.
- Durable campaign representation must be implementation-neutral.
- Portable campaigns must preserve or reliably resolve the exact definitions required to interpret their state.
- Long-term campaign portability requires public documentation of the campaign format, package model, and engine contracts.
- Browser or client-local storage, if used, is working storage and must not be treated as the only long-term representation.
- Campaign scale must not imply working-set scale.

The persistence architecture should permit historical campaign information to be segmented, archived, compressed, offloaded, or otherwise managed outside active working storage without changing its semantic availability.

Compression, archiving, compaction, and deletion are distinct concepts:

- Compression preserves the same information in a smaller representation.
- Archiving moves infrequently used information out of working storage.
- Compaction replaces verbose operational representation with an equivalent canonical representation.
- Deletion intentionally removes information.

## Checkpoint, Undo, Recovery, And Retcon

These concepts must remain separate:

- Checkpoint: a deliberate recoverable campaign state.
- Undo: reversal of an accidental or unwanted operation.
- Recovery: restoration after technical failure.
- Retcon: human-authorized revision of established campaign reality.

A retcon changes canonical campaign reality under human authority. The engine should preserve provenance and identify potentially affected dependent state rather than silently rewriting consequences.

## SRD And Public Contracts

Campistoria should eventually include a platform SRD describing the engine public meta-model, package model, semantics, campaign representation, and interoperability contracts sufficiently for independent implementations of packages, clients, and authoring tools.

This SRD is not an RPG-system SRD. It is the Campistoria platform reference.

It should eventually define, at minimum, the normative meanings of Campaign, Campaign Reality, Entity, Location, Relationship, Fact, State, Event, Observer Knowledge, Package, Resolution, Oracle, Procedure, Projection, package composition, package compatibility, persistence representation, and public engine contracts.

First-party tools and packages must not have privileged private capabilities unavailable to third-party implementations.

## Scope Baseline

Core engine scope includes:

- Campaign lifecycle and permanent identity.
- Campaign Reality.
- Package-defined types and schemas.
- Entities and specialized Locations.
- Properties.
- First-class Relationships.
- Facts.
- State and temporal validity.
- Campaign Time.
- Events/history.
- Unresolved State and Resolution.
- Observers and Observer Knowledge/Belief.
- Information sources and provenance.
- Generic spatial primitives: containment, topology, coordinate spaces, and scale.
- Environment and Conditions.
- Rules, Oracles, and Procedures as package-supplied semantics.
- Package composition, dependencies, capabilities, versions, and explicit overrides.
- System/edition identification independent of package version.
- Immutable Scenario to mutable Campaign instantiation.
- Package-version pinning.
- Package registry with simultaneous versions and dependency-aware removal.
- Queries and POV resolution.
- Presentation Models/Projections through the public engine API.
- Durable persistence.
- Checkpoints, recovery, undo, and retcon as distinct concepts.
- Import/export and implementation-neutral durable representation.
- Validation of campaign/package consistency.
- Structured diagnostics.

## Deferred Or Out Of Scope

Deferred or excluded capabilities should be documented so they do not re-enter by accident.

**Authoring Environment**  
The engine provides contracts necessary to construct and validate packages. A separate authoring project may eventually provide visual editors, schema tools, map editors, procedure editors, oracle editors, Git integration, Markdown editing, asset editing, or a package IDE.

**AI**  
AI is not an engine capability. The engine has no LLM dependency, embedding requirement, RAG requirement, AI-generated-description capability, AI oracle interpretation requirement, or AI GM function. External AI tools may use public APIs like any other client or authoring tool.

**Multiplayer**  
Multiplayer is deferred. The engine/client boundary should not assume the caller and engine always execute in the same process or execution context, but networking, synchronization, authentication, signaling, conflict resolution, and host migration are not first-phase concerns.

**Cloud Services**  
Accounts, hosted campaigns, cloud persistence, cloud sync, telemetry, analytics, subscriptions, and remote backup are not engine requirements.

**Collaboration**  
Shared editing, co-GMs, distributed authoring, and simultaneous assistants are deferred.

**Branching Campaigns / Alternate Timelines**  
Explicit alternate-timeline support is deferred. Checkpoint and restore are sufficient early concepts.

**Sophisticated Retcon Propagation**  
Automatic consequence rewriting is deferred. The engine should identify directly affected references where possible and leave consequence resolution to the human.

**Rules Inference / Expert System**  
The engine evaluates explicitly supplied rules and procedures. It does not infer game mechanics from campaign facts or act as a generic semantic reasoner.

**General-Purpose Worldbuilding**  
The engine is not a general-purpose worldbuilding system. It maintains enough authoritative, temporal, observer-aware campaign state to execute an RPG campaign.

**Rendering**  
The engine produces presentation models. Clients render them. The engine does not require SVG, Canvas, WebGL, Qt painting, HTML, CSS, 3D, or printer-driver semantics.

**Rich Media Generation / Editing**  
Images, maps, sound, video, music, and 3D models may be referenced by packages and campaigns. Creating or editing those assets is not the engine's job.

**Internet Package Ecosystem**  
A package registry concept belongs in the platform, but an Internet repository, marketplace, ratings/reviews, publisher accounts, payments, licensing service, or automatic update ecosystem is deferred.

## Conceptual Solution Directions Considered

The ideation process considered several conceptual directions:

**Map-first solo tool**  
Useful for hexcrawls and exploration, but too narrow. Campaign reality is more fundamental than any one map projection.

**Journal/oracle assistant**  
Useful for solo play, but insufficient for long-term continuity, observer knowledge, package semantics, and structured campaign state.

**Worldbuilding database**  
Powerful, but too broad. Campistoria should not become a general-purpose universe simulator.

**Campaign reality engine**  
The selected conceptual direction: maintain evolving campaign reality, unresolved state, history, observer knowledge, package semantics, persistence, and projections while leaving narrative authority with the human.

## Stress-Test Findings

The model was exercised against several conceptual scenarios:

**Traveller-style play**  
Validated scale, hierarchy, package composition, system/edition identity, large location graphs, trade/travel procedures, and locality of working state.

**Hexcrawl play**  
Validated unresolved geography, emergent discovery, environment, conditions, topology, spatial uncertainty, and map-as-projection.

**Fate-style play**  
Validated relationships, non-spatial truth, aspects-like facts, temporary state, and observer knowledge/belief.

**Systemless solo play**  
Validated that the engine still has a coherent role without maps, elaborate settings, combat rules, classes, skills, multiplayer, or a GM. What remains is identity, entities, relationships, facts, state, time, history, observer knowledge, resolution, packages, persistence, queries, and projections.

The tests converged on the same core idea: the engine maintains an evolving campaign reality and exposes observer-appropriate projections of that reality through public contracts.

## Risks And Open Questions

Risks to carry into requirements:

- The package model could become too powerful and drift into arbitrary executable plugins.
- The engine could accidentally absorb authoring-tool responsibilities.
- Observer knowledge could be oversimplified and collapsed into global truth.
- Retcon semantics could become dangerously automatic.
- Persistence could become tied to a specific implementation and weaken long-term user ownership.
- Package migration could silently alter campaign semantics.
- The project could prematurely choose technology before quality attributes are known.
- The system could expand into general worldbuilding rather than campaign execution.
- AI features could re-enter as implicit engine behavior rather than optional external tooling.
- Public contracts could be underspecified, making third-party packages or clients second-class.

Open questions for later phases:

- What are the formal requirements identifiers and acceptance criteria?
- What minimum package semantics are required for first release?
- What is the minimum viable campaign representation?
- How much diagnostic detail must the engine expose?
- What must be normative in the SRD versus implementation guidance?
- What storage, indexing, and archival strategies best satisfy the requirements?
- What client architecture best satisfies the eventual user workflows?
- What technology stack best satisfies the requirements and quality attributes?

## Requirements Engineering Entry Notes

Requirements engineering should begin from the following stable conceptual statements:

- Campistoria maintains evolving campaign reality.
- Campaign reality and observer knowledge are separate.
- Unresolved campaign state may exist explicitly.
- Oracles produce inputs; resolution establishes reality.
- Packages define game and world semantics.
- Package composition is explicit and version-pinned.
- Campaigns own their evolving reality after scenario instantiation.
- Clients consume projections and presentation models; they do not own reality.
- Durable, implementation-neutral, user-owned campaign data is foundational.
- AI, cloud services, multiplayer, rich authoring, and implementation technology are not engine assumptions.

Requirements should not smuggle in architecture or implementation decisions. Where technology questions arise, record them as later trade-study inputs unless the user explicitly approves a lifecycle transition or constraint.
