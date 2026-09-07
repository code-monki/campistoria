# Solo RPG Campaign Engine Primer

## Purpose

This primer is for agents working specifically on the Solo RPG Campaign Engine.

Read the project-wide instructions first:

`AGENTS.md`

The authoritative conceptual source for the engine is:

`docs/concept/engine-ideation.md`

This primer is a compact operating brief. It does not replace the concept document.

## Current Engine Status

Engine ideation is complete enough to close.

The next engine activity is requirements engineering. Do not prematurely choose technologies, design schemas, design file formats, select databases, design UI screens, implement code, or collapse conceptual boundaries unless the user explicitly redirects the work.

Conceptual decisions are settled unless new analysis exposes a contradiction. Implementation decisions remain open unless explicitly identified otherwise.

The next useful work is to turn the conceptual model into requirements and quality attributes without smuggling implementation choices into them.

## Ideation Gate Clarifications

The concept document now explicitly records project stakeholders, conceptual constraints, and preliminary non-functional signals. Treat those sections as Ideation-level framing for Requirements work, not as a RACI matrix, architecture, implementation plan, or measurable NFR specification.

Project-level decision and lifecycle advancement authority remains with the project owner during the current Ideation-to-Requirements process. Runtime Human Authority describes authority during play; it does not assign ownership over every project artifact, technology choice, or implementation decision.

Known conceptual constraints include human authority, deterministic/probabilistic separation, model independence, ruleset independence, inspectable authoritative state, portability, and lifecycle governance. These constraints limit acceptable future requirements and designs without selecting a language, storage technology, UI framework, deployment platform, schedule, budget, or quantitative performance target.

## Engine Problem

Existing solo-RPG tools are often disconnected. They help with notes, maps, rolls, tables, journals, or generators, but they usually do not maintain authoritative campaign reality, observer knowledge, unresolved state, provenance, temporal state, package composition, and durable campaign history as one coherent runtime.

The engine exists to preserve continuity and correctness in solo play while leaving game-system meaning to reusable packages and leaving creative authority with the human player.

## What The Engine Is

The engine is a reusable campaign-state runtime that maintains persistent, evolving, observer-aware campaign reality.

It must support:

- pre-authored scenario play;
- emergent play;
- hybrid play where some campaign reality is authored and some is unresolved until play;
- long-running campaigns;
- package-defined RPG systems, settings, scenarios, rules, oracles, procedures, and house rules;
- observer-specific knowledge and POV-derived presentation;
- durable user-owned campaign data.

The engine is not just a journal, map tool, oracle roller, encounter generator, or note-taking system.

## Core Invariants

Preserve these unless the user explicitly reopens them:

- The campaign maintains reality. What an observer knows about that reality may be much more limited or different.
- Campaign Reality and Observer Knowledge are separate.
- Resolution changes Campaign Reality. Observation changes Observer Knowledge.
- The map is a projection, not the world.
- Presentation is POV-derived.
- The client asks what may be presented; the engine decides what is valid to present.
- Campaign Reality describes what is true; rules describe what those truths mean mechanically.
- The engine knows the meta-model; packages know the game and world model.
- Game-specific, setting-specific, scenario-specific, genre-specific, and edition-specific concepts belong in packages.
- The scenario defines initial reality; the campaign owns evolving reality.
- A campaign pins the exact package composition from which it was instantiated unless the user explicitly changes it.
- Package upgrades are explicit migrations, never silent replacement.
- Package versions coexist; installing a newer version does not delete or supersede older versions.
- Composition is explicit; collision is not composition.
- Campaign scale must not imply working-set scale.
- Historical campaign information may be archived, segmented, compressed, or offloaded without changing semantic availability.
- Durable campaign data belongs to the user and must remain portable outside transient client storage.
- Human authority over play remains primary.
- AI is not an engine capability.

## Normalized Terms

Use this vocabulary consistently:

- Campaign: persistent instance of play.
- Campaign Reality: authoritative representation of what has been established as true in the campaign.
- Entity: campaign object with persistent identity.
- Location: entity capable of participating in spatial relationships and projections.
- Property: intrinsic information on an entity or typed object.
- Relationship: first-class connection between identified campaign objects.
- Fact: explicit proposition about Campaign Reality.
- State: condition of campaign reality at a time.
- Event: meaningful occurrence or transition in campaign history.
- Campaign Time: ordering and temporal position within campaign reality.
- Unresolved State: deliberately unestablished part of campaign reality.
- Resolution: act of establishing previously unresolved campaign reality.
- Observer: viewpoint for knowledge.
- Observer Knowledge: what an observer knows, believes, suspects, has heard, or otherwise understands.
- Information Source: provenance for Observer Knowledge.
- Oracle: uncertain/randomized answer source.
- Procedure: package-defined play process.
- Rule: package-defined interpretation of campaign information.
- Environment: changing surrounding circumstance.
- Condition: relevant current state of something.
- Package: versioned unit of definition, behavior, content, assets, or initial knowledge.
- Scenario: immutable package-defined starting configuration.
- Projection: purpose-specific representation of campaign information.
- Presentation Model: engine-produced information a client may render.
- POV: observer/context basis for determining what may be presented.
- Checkpoint, Undo, Recovery, Retcon: separate concepts.

Prefer "Campaign Reality" over "World Truth" or "Campaign Truth."

Prefer "Observer Knowledge" over "epistemic state."

## Engine Responsibilities

The engine owns generic campaign-runtime machinery:

- campaign identity and lifecycle;
- Campaign Reality;
- permanent identity;
- typed entities and locations;
- properties, relationships, facts, state, events, and history;
- Campaign Time;
- Unresolved State and Resolution;
- observers and Observer Knowledge;
- information provenance;
- package composition;
- rule/oracle/procedure execution contracts;
- checkpoints and recovery primitives;
- query and POV resolution;
- Presentation Models;
- persistence, import, export, validation, and diagnostics contracts.

The engine must not hard-code Traveller, D&D, Fate, hexcrawls, sectors, hexes, rooms, starports, armor class, aspects, jump drives, or other game/system semantics.

## Package Responsibilities

Packages define game and world semantics:

- RPG system and edition;
- domain types and schemas;
- relationship and fact types;
- rules, oracles, and procedures;
- location vocabularies and coordinate models;
- environment and condition vocabularies;
- mechanics;
- setting and scenario content;
- initial Observer Knowledge;
- presentation hints;
- assets.

Packages should prefer data, schemas, declarative rules, declarative procedures, and assets over arbitrary executable code.

## Client Boundary

Clients own rendering and interaction, not campaign truth.

Clients render engine-provided Presentation Models. They must not independently derive hidden-information rules, POV rules, visibility, knowledge filtering, campaign truth, or temporal state.

The engine determines what may be presented. The client determines how to display it.

Do not make rendering technology part of the engine concept.

## Authoring Boundary

Authoring tools create packages. They are a separate project.

The engine provides public contracts to construct, validate, import, export, and use packages. It does not become a full authoring IDE.

Authoring may eventually include hierarchy/graph navigation, map editing, scenario editing, schema editing, oracle/procedure editors, validation UI, package building, Markdown/external editor integration, Git integration, asset workflows, and conversion tools.

## Human Authority

The human remains authoritative over play:

- establishes reality;
- resolves uncertainty;
- interprets oracle results;
- accepts or rejects generated content;
- overrides where permitted;
- corrects mistakes;
- retcons established reality;
- decides what becomes canonical.

The engine maintains consistency. Packages constrain semantics. Neither replaces the human player/GM.

See the concept document for project stakeholders and conceptual constraints. Do not treat this runtime authority section as project governance.

## Package and Composition Model

Packages are versioned units of definition, behavior, or content. A campaign may combine rules/system packages, setting packages, oracle packages, procedure packages, weather packages, scenario packages, and user override packages.

Composition rules:

- package version and RPG system edition are distinct;
- packages declare dependencies and capabilities;
- packages depend on capabilities where possible and specific packages where necessary;
- extension and override are distinct;
- house rules are packages, not hidden mutations;
- first-party templates use the same public mechanisms as third-party packages;
- composition produces an Effective Campaign Definition;
- campaigns pin effective package composition;
- package upgrades are explicit migrations;
- package versions coexist;
- removal is dependency-aware across current, inactive, and archived campaigns.

Do not assume automatic package updates. Do not assume "latest version wins."

## Scenario and Campaign Lifecycle

Scenario content is immutable authored starting definition.

Starting play instantiates a mutable Campaign.

The same scenario may produce many divergent campaigns. Running play mutates the campaign, not the scenario.

The lifecycle is:

```text
Packages
    -> validate composition
    -> resolve dependencies and capabilities
    -> apply explicit extensions and overrides
    -> Effective Campaign Definition
    -> instantiate Campaign
    -> play, resolve, observe, change
    -> checkpoint/save/export
    -> resume/diagnose/migrate only through explicit operations
```

Scenario/package upgrades for an existing campaign are potentially unsafe. Treat them as explicit migrations requiring validation and checkpoint/backup semantics.

## Presentation Model

POV includes more than line of sight. It may include perception, prior exploration, maps, documents, sensors, communications, rumors, knowledge, beliefs, and stale or incorrect information.

Presentation should be context-local and scope-local. A campaign may be huge, but a view should request only the relevant observer/context/time/projection.

## Storage and Scale

Campaign lifetime size may exceed active working-set size.

The engine must support large campaigns conceptually: hundreds of thousands of spatial cells, many systems/locations/entities, years of events, and large historical archives.

Do not assume all campaign history fits in IndexedDB, memory, one database table, one file, or one active index.

Open storage ideas include segmentation, archiving, compression, offloading, snapshots, event compaction, indexes, and portable artifacts. These are implementation decisions.

Archived does not mean discarded. Historical information must remain semantically available for reconstruction and search.

## Engine Interoperability Requirement

Open interoperability is foundational:

- public package specification;
- public campaign representation;
- public engine contracts;
- platform SRD;
- no privileged first-party package capabilities;
- no requirement to use first-party authoring tools;
- durable user-owned campaign data;
- implementation-neutral import/export.

Authoring tools and third-party tools must use public contracts rather than privileged internals.

## Deferred / Out Of Engine Scope

Do not pull these into immediate engine requirements unless the user explicitly reopens them:

- full authoring IDE;
- AI engine capability or AI-specific campaign semantics;
- multiplayer implementation;
- cloud accounts/sync/hosting/telemetry/subscriptions;
- collaboration protocols;
- branching campaigns / alternate timelines;
- automatic retcon consequence rewriting;
- generic rules inference or expert-system reasoning;
- general-purpose worldbuilding platform;
- rendering technology;
- rich media creation/editing;
- package marketplace or commercial ecosystem;
- automatic package upgrades.

## Open Implementation Decisions

These remain open:

- language;
- browser-only vs Electron vs Qt/native vs native-core/browser-client;
- database/storage technology;
- archive/compression/snapshot strategy;
- event/history implementation;
- indexing/search implementation;
- serialization and package manifest syntax;
- API transport;
- UI framework and rendering technology;
- authoring-tool technology;
- deployment/signing strategy.

Do not choose these during requirements work unless the user asks for a technology trade study.

The concept document also states that Ideation imposes no fixed implementation language, storage technology, UI framework, deployment platform, quantitative performance target, schedule, or monetary budget unless explicitly established elsewhere. Do not infer one from the open implementation list.

## Immediate Next Work

Start requirements engineering.

Recommended first requirements pass:

1. Derive functional requirements from the concept document.
2. Derive non-functional requirements and quality attributes.
3. Separate MVP requirements from later capabilities.
4. Record open questions without resolving them prematurely.
5. Preserve every conceptual invariant listed above.
