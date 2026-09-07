# Campistoria

**Campistoria — The Campaign Engine**

Campistoria is an open, system-neutral engine for maintaining the persistent state of tabletop role-playing game campaigns.

Rather than treating maps, journals, characters, locations, events, oracles, and other campaign information as independent tools, Campistoria treats them as views and operations over a shared campaign model. The engine maintains what has been established as true in the campaign, how that reality changes over time, what remains unresolved, and what different observers know or believe about it.

Campistoria is intended to support solo play, traditional GM-led campaigns, and other styles of play without embedding the assumptions of a particular RPG system, setting, genre, or user interface.

## Project Status

Campistoria is currently in the **requirements engineering** phase.

The conceptual model and initial project boundaries have been established and validated through ideation. Requirements, architecture, implementation technology, persistence technology, client framework, and other implementation decisions should not be inferred from exploratory discussions or prematurely selected.

There is currently no usable Campistoria engine or application.

## Core Ideas

Several principles define the current direction of the project.

### Campaign reality is persistent

A campaign has an authoritative representation of what has been established as true.

That reality may evolve through play as locations are discovered, characters act, relationships change, events occur, environments change, or previously unresolved questions are answered.

The campaign—not the scenario from which it originated—owns this evolving reality.

### Reality and knowledge are different

What is true and what a character, party, faction, or other observer knows are separate concerns.

Observer knowledge may be incomplete, approximate, stale, ambiguous, or incorrect. Discovering something changes observer knowledge; it does not necessarily create the thing being discovered.

### Unresolved does not mean nonexistent

Campaign information does not always need to be predetermined.

A scenario may deliberately leave some part of reality unresolved until play requires an answer. Resolution establishes that answer as campaign reality through human decision, an oracle, a procedure, or another mechanism defined by the campaign's package composition.

### Maps are projections, not the world

Spatial information is part of campaign state, but a map is only one possible presentation of that information.

The underlying model supports persistent identity, containment, topology, coordinate spaces, scale, and other generic spatial concepts without requiring every campaign—or every location—to use a map.

The same campaign information may be projected as maps, timelines, relationship views, journals, rosters, scene information, or other purpose-specific representations.

### The engine knows the meta-model; packages know the game

Campistoria provides generic campaign concepts and runtime semantics.

Packages define game- and world-specific concepts such as rules, types, properties, relationships, facts, spatial vocabularies, coordinate models, oracles, procedures, environments, conditions, settings, and scenarios.

The engine should not need built-in knowledge of a particular RPG, edition, genre, setting, or campaign.

### Human authority is fundamental

Campistoria assists with maintaining and operating a campaign; it does not claim authorship of it.

Human participants retain authority to interpret uncertain results, establish campaign reality, override permitted behavior, correct mistakes, and intentionally revise campaign canon.

Oracles and procedures may inform decisions but do not inherently replace human judgment.

### AI is not an engine dependency

Campistoria does not require artificial intelligence and defines no AI-specific campaign semantics.

AI-assisted clients, authoring tools, or other external systems may eventually interact with Campistoria through its public interfaces, but they have no special authority over campaign reality.

### Campaign data belongs to the user

Campaigns are intended to remain portable and interpretable independently of a particular Campistoria implementation.

Public specifications and a System Reference Document (SRD) are expected to define the relevant campaign, package, and interoperability contracts.

A long-running campaign should not become captive to a particular application, storage technology, service, or vendor.

## Packages

Campistoria uses composable, versioned packages to supply game- and campaign-specific definitions and content.

Expected package roles include:

- system definitions;
- settings;
- scenarios;
- rules;
- oracles;
- procedures;
- reusable capabilities;
- assets; and
- user or campaign-specific overrides.

Packages may provide and require capabilities, allowing compositions to depend on required behavior without unnecessarily depending on a specific provider.

Running campaigns are expected to remain pinned to the package composition from which they were instantiated unless an explicit migration is performed. Installing a newer package version does not silently alter an existing campaign.

First-party packages use the same public package mechanisms available to everyone else.

## Open Platform

Campistoria is intended to be an open platform rather than a closed campaign application.

The project is expected to provide:

- the campaign engine;
- public package specifications;
- public campaign persistence/interchange specifications;
- a platform SRD;
- a reference client;
- sample packages and scenarios;
- validation and interoperability facilities; and
- documentation sufficient to support independent tools and implementations.

Specialized authoring tools are expected to be developed separately from the engine. Creating valid Campistoria content must not require privileged or proprietary first-party tooling.

## Repository Structure

```text
.
├── clients/
│   └── reference/       # Reference Campistoria client
├── docs/
│   ├── agent/           # Agent/project orientation material
│   ├── architecture/    # Approved architecture artifacts
│   ├── assets/          # Documentation assets
│   ├── concept/         # Ideation and conceptual foundation
│   ├── requirements/    # Requirements artifacts
│   └── srd/             # System Reference Document
├── engine/              # Campaign engine implementation
├── packages/
│   └── samples/         # Reference and demonstration packages
├── specifications/
│   ├── campaign/        # Campaign representation specifications
│   └── package/         # Package format and contract specifications
├── tests/               # Project verification assets
└── tools/               # Project-supporting tools
```

Some local development environments may also contain development-governance material that is intentionally excluded from the repository.

## Documentation

The principal project documentation is organized by lifecycle and purpose.

- [`docs/concept/engine-ideation.md`](docs/concept/engine-ideation.md) — conceptual foundation, problem definition, scope, terminology, and decisions established during ideation.
- [`docs/agent/engine-primer.md`](docs/agent/engine-primer.md) — orientation and context for agents and other tools working with the project.
- [`docs/requirements/`](docs/requirements/) — normative system requirements as they are developed and approved.
- [`docs/architecture/`](docs/architecture/) — approved architectural decisions and system architecture.
- [`docs/srd/`](docs/srd/) — normative public Campistoria meta-model and interoperability semantics.
- [`specifications/`](specifications/) — normative machine- and implementation-facing package and campaign specifications.

As the project progresses, approved requirements, architecture, SRD material, and specifications supersede earlier exploratory material where they intentionally refine or replace it.

## Development Approach

Campistoria is being developed using an explicit lifecycle and governance discipline.

Conceptual exploration, requirements, architecture, detailed design, implementation, verification, traceability, packaging, and documentation are treated as distinct but connected engineering activities. Decisions are expected to be made at the appropriate lifecycle stage rather than introduced implicitly during implementation.

The project is not tied to a particular programming language, application framework, persistence mechanism, database, UI toolkit, or deployment model at this stage.

Technology selection follows requirements and quality attributes rather than preceding them.

## Contributing

Campistoria is still establishing its requirements and public contracts. Contribution procedures have not yet been defined.

Until those procedures and the relevant specifications stabilize, prospective contributors should treat the repository as work in progress rather than assume that current internal structures constitute stable extension points or APIs.

## License

Licensed under the [Apache License, Version 2.0](LICENSE).

Unless explicitly stated there, the presence of source code, specifications, documentation, or other material in this repository should not be interpreted as granting rights beyond those established by the applicable license.