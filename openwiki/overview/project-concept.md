---
type: concept-overview
title: "Campistoria Concept and Principles"
description: "What Campistoria is — a system-neutral campaign-state engine for tabletop RPGs — and the conceptual invariants (Campaign Reality vs. Observer Knowledge, unresolved state, engine/package split, human authority) established during ideation."
tags: [concept, principles, campaign-reality, observer-knowledge, ideation]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-506524e6f504c69f27bb5627
    resource: repo://docs/agent/engine-primer.md
  - id: openwiki-source-23775c3de52f3ab95a13cb8b
    resource: repo://README.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Campistoria Concept and Principles

Campistoria is an open, system-neutral engine for maintaining the persistent state of tabletop role-playing game campaigns. Rather than treating maps, journals, characters, locations, events, oracles, and other campaign information as independent tools, it treats them as views and operations over one shared campaign model that maintains what has been established as true, how that reality changes over time, what remains unresolved, and what different observers know or believe about it. It is designed to support solo play, traditional GM-led campaigns, and other play styles without embedding the assumptions of a particular RPG system, setting, genre, or user interface.

These are conceptual and requirements-level commitments established during Ideation — the project has no implemented engine or application yet (see [Repository Structure and Tooling](../reference/repository-structure.md)).

## The engine problem

Existing solo-RPG tools tend to be disconnected: they help with notes, maps, rolls, tables, journals, or generators, but usually don't maintain authoritative campaign reality, observer knowledge, unresolved state, provenance, temporal state, package composition, and durable campaign history as one coherent runtime. Campistoria exists to preserve continuity and correctness across all of that while leaving game-system meaning to reusable packages and creative authority with the human player.

## Core conceptual invariants

These principles are treated as settled unless new analysis exposes a contradiction:

**Campaign reality is persistent and evolves through play.** A campaign has an authoritative representation of what has been established as true. That reality may evolve as locations are discovered, characters act, relationships change, or previously unresolved questions are answered — the campaign, not the scenario it originated from, owns this evolving reality.

**Reality and knowledge are different.** What is true and what a character, party, faction, or other observer *knows* are separate concerns. Observer knowledge may be incomplete, approximate, stale, ambiguous, or incorrect — Resolution changes Campaign Reality, Observation changes Observer Knowledge, and discovering something changes observer knowledge without necessarily creating the thing being discovered. (This distinction is enforced structurally, not just conceptually — see [HLA-OBSERVER](../architecture/components/observer.md).)

**Unresolved does not mean nonexistent.** A scenario may deliberately leave part of reality unresolved until play requires an answer; resolution establishes that answer through human decision, an oracle, or a procedure defined by the campaign's package composition. (See [HLA-RESOLUTION](../architecture/components/resolution.md) for how this is structurally contained.)

**Maps are projections, not the world.** Spatial information is part of campaign state, but a map is only one possible presentation of it. The underlying model supports persistent identity, containment, topology, coordinate spaces, and scale as generic spatial concepts without requiring every campaign — or location — to use a map. The same campaign information may be projected as maps, timelines, relationship views, journals, rosters, or other purpose-specific representations, and presentation is always POV-derived: the client asks what may be presented, and the engine decides what is valid to present.

**The engine knows the meta-model; packages know the game.** The engine owns generic campaign-runtime machinery — identity, entities, locations, properties, relationships, facts, state, events, Campaign Time, Unresolved State/Resolution, observers, provenance, package composition, checkpoints/recovery, query/POV resolution, and persistence/import/export/validation contracts. It must not hard-code any specific RPG system, edition, genre, or setting concept (no "Traveller," "D&D," "hexcrawls," "armor class," or similar baked in). Packages instead define game- and world-specific concepts: rules, types, properties, relationships, facts, spatial vocabularies, oracles, procedures, environments, conditions, and scenario content — and should prefer data, schemas, declarative rules, and assets over arbitrary executable code.

**Human authority is fundamental.** Campistoria assists with maintaining and operating a campaign; it does not claim authorship of it. The human establishes reality, resolves uncertainty, interprets oracle results, accepts or rejects generated content, corrects mistakes, and decides what becomes canonical. Oracles and procedures may inform decisions but do not replace human judgment.

**AI is not an engine dependency.** Campistoria does not require artificial intelligence and defines no AI-specific campaign semantics. AI-assisted clients or tools may eventually interact with Campistoria through its public interfaces, but they have no special authority over campaign reality.

**Campaign data belongs to the user.** Campaigns are intended to remain portable and interpretable independently of any particular Campistoria implementation. A long-running campaign should not become captive to a particular application, storage technology, service, or vendor — public specifications and a System Reference Document (SRD) are expected to define the relevant campaign, package, and interoperability contracts.

## The package and composition model

Packages are versioned units of definition, behavior, or content — system definitions, settings, scenarios, rules, oracles, procedures, reusable capabilities, assets, and user/campaign-specific overrides. Package version and RPG system/edition identity are treated as distinct; packages declare dependencies and capabilities; extension and override are distinct operations; house rules are themselves packages rather than hidden mutations; and first-party templates use the same public mechanisms as third-party packages. Composition produces an Effective Campaign Definition, and a running campaign pins the exact package composition it was instantiated from — installing a newer package version never silently alters an existing campaign, package upgrades are explicit migrations, and multiple package versions coexist rather than the newest silently winning.

## Scenario and campaign lifecycle

Scenario content is an immutable, authored starting definition. Starting play instantiates a mutable Campaign from that Scenario — the same Scenario may produce many divergent Campaigns, and running play mutates the Campaign, never the Scenario. (See [HLA-LIFECYCLE](../architecture/components/lifecycle.md) for the structural instantiation model.)

## Storage and scale

Campaign lifetime size may exceed the active working-set size — the engine is meant to support large campaigns conceptually (hundreds of thousands of spatial cells, many systems/locations/entities, years of events, large historical archives) without assuming all history fits in one database table, file, or in-memory index. Archived does not mean discarded: historical information must remain semantically available for reconstruction and search even when segmented, compressed, or offloaded.

## Open platform and interoperability

Campistoria is intended to be an open platform rather than a closed application. The project is expected to eventually provide the campaign engine itself, public package specifications, public campaign persistence/interchange specifications, a platform SRD, a reference client, sample packages/scenarios, validation/interoperability facilities, and documentation sufficient for independent implementations. Authoring tools are expected to be developed separately from the engine, and creating valid Campistoria content must not require privileged or proprietary first-party tooling — there is no requirement to use first-party authoring tools, and durable user-owned campaign data must support implementation-neutral import/export.

Every Campaign-scoped operation requires an active local **Campaign Authority Binding** between the requesting Principal and the target Campaign — session, role, or capability claims supplied by a caller never self-authorize. This minimal single-player authorization boundary is v1 engine scope and does not introduce hosted identity, accounts, invitations, or multiplayer authority administration. (See [HLA-CONTRACT](../architecture/components/contract.md) for the structural authorization model.)

## What Campistoria explicitly is not

The engine is not a journal, map tool, oracle roller, encounter generator, or note-taking system by itself, and it is explicitly **not a Virtual Tabletop (VTT)** — a VTT-like application may be built as a client of the engine, but token manipulation UX, live table presence, chat, audio/video, and other table-surface concerns belong outside the engine. Clients own rendering and interaction, never campaign truth: they render engine-provided Presentation Models and must not independently derive hidden-information rules, POV rules, visibility, or campaign truth themselves. Authoring tools that create packages are likewise a separate project — the engine provides public contracts to construct, validate, import, export, and use packages, but does not become a full authoring IDE.

Deferred out of current engine scope: a full authoring IDE, AI as an engine capability, multiplayer implementation, cloud accounts/identity/sync/hosting, branching campaigns or alternate timelines, automatic Retcon consequence rewriting, generic rules inference, a general-purpose worldbuilding platform, rendering technology, and a package marketplace.
