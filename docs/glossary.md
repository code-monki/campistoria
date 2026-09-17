# Campistoria Engine Glossary

Project Name: Campistoria Engine
Version: 0.2 (Working Glossary)
Date (YYYY-MM-DD): 2026-09-17
Author(s): CodeMonki
Status: Draft

---

## Purpose

This glossary consolidates Campistoria Engine terminology used across Ideation, Requirements, Architecture, and Detailed Design artifacts.

Definitions in approved phase artifacts remain authoritative for those approved artifacts. This glossary is the shared working reference for current and future artifacts, and SHALL NOT be used to retroactively change approved requirements or architecture without explicit change control.

---

## Terms

| Term | Meaning |
|---|---|
| Active Working Set | The recent portion of campaign data kept readily available for ordinary operation, distinct from archived history retrievable on demand. |
| Actor | The runtime participant or tool acting through a session, such as a player, GM-facing tool, authoring tool, or integration. |
| AssetRef | A semantic reference to an asset used by Campaign Reality or Observer Knowledge without embedding payload bytes or storage mechanics in authoritative state. |
| Authoritative State | Engine-owned state that determines what is true for its domain, such as Campaign Reality or Observer Knowledge, as distinct from derived views, caches, client renderings, or diagnostics. |
| Campaign | A persistent, mutable instance of play with durable identity, Campaign Reality, history, package composition, Observer Knowledge, Unresolved State, and user-owned data. |
| Campaign Authority Binding | The durable, Campaign-scoped association between a locally recognized Principal and the capability bundle or explicit capabilities that Principal may exercise for that Campaign. Imported foreign bindings may be retained as provenance but do not become active local grants. |
| Campaign Owner | The initial local Principal bound to a Campaign with the `campaignOwner` capability bundle. This is an authorization role bundle, not a claim of ownership over software or project governance. |
| Campaign Reality | The authoritative record of what is established as true in the campaign. |
| Campaign Time | The ordering and temporal frame in which campaign Events and State changes occur, independent of any package-specific calendar or time unit. |
| Capability | A named permission checked by HLA-CONTRACT before an operation is forwarded. Roles may grant capability bundles, but authorization resolves to explicit capabilities. |
| Capability Grant | An authorization assignment represented by an active Campaign Authority Binding. A capability named only in a caller-supplied request or Session does not grant authority. |
| Capability-Based Access Control (CBAC) | Authorization based on explicit capabilities required by an operation, rather than hardcoded role names. |
| Checkpoint | A deliberate, recoverable campaign state used for save, restore, recovery, or replay-bounding purposes. |
| Client | A reference UI, third-party UI, authoring tool, diagnostic tool, integration, or other external caller using HLA-CONTRACT. |
| Delta | A caller-visible change set after a known revision or delta number, used for synchronization without resending a full representation. |
| Delta Number | A stream-scoped synchronization marker supplied by a client or returned by the engine. It is not a single global Campaign counter. |
| Deterministic/Probabilistic Boundary | The boundary separating deterministic authoritative mechanics and state changes from probabilistic Oracle output, generated text, interpretation, or other uncertain inputs. |
| Effective Campaign Definition | The resolved package composition, extensions, overrides, and dependencies used to instantiate or migrate a Campaign. |
| Entity | A campaign object with persistent identity. Packages define domain-specific entity types. |
| Event | A recorded occurrence or change in Campaign Reality. Events support continuity, provenance, temporal query, reconstruction, and auditability. |
| Event Sourcing | A design pattern in which the ordered Event stream is the authoritative source of state, and current or historical state is derived by replaying or folding Events. |
| Fact | An established assertion about Campaign Reality, with provenance and temporal validity. |
| Fold | Deterministic reconstruction of state by applying an ordered sequence of committed Events to an initial or snapshot state until the requested Event or Campaign Time boundary is reached. |
| Full State Transfer | Synchronization mode in which the engine returns a full caller-appropriate representation rather than a delta. |
| Game Master / GM | The human runtime authority when play style includes a GM-like role. This is a runtime authority concept, not a project governance role by itself. |
| HLA | High-Level Architecture. The approved architectural decomposition and boundary model for the engine. |
| Human Authority | The principle that consequential narrative interpretation, world-state change, uncertainty resolution, exceptions, retcons, and canon remain under human authority. |
| Information Source | The provenance of a unit of Observer Knowledge, such as direct observation, map, rumor, NPC statement, document, sensor reading, Oracle result, or inference. |
| Location | An Entity capable of participating in spatial relationships and spatial projections. |
| Model Independence | The constraint that the engine concept does not depend on a specific AI model, model provider, prompt format, proprietary behavior, or AI runtime. |
| Observer | A viewpoint whose knowledge may differ from Campaign Reality. This is unrelated to the Gang-of-Four Observer design pattern. |
| Observer Knowledge | What an Observer knows, believes, suspects, remembers, has been told, has mapped, has inferred, or has misunderstood. |
| Oracle | A defined process that produces uncertain or randomized output. An Oracle result does not inherently change Campaign Reality. |
| Package | A versioned, identifiable unit contributing definitions, behavior, content, rules, procedures, or assets to a Campaign. |
| Package Artifact | The structured representation of one Package version, including identity, metadata, dependencies, definitions, contracts, assets, validation declarations, compatibility declarations, and migration declarations. |
| Package Composition Pin | A recorded reference to the exact effective package composition used by a Campaign. |
| Package Semantic Reference | A stable reference from Campaign Reality or related engine data to package-defined meaning, such as an entity type, relationship type, property type, rule, Oracle, Procedure, schema, asset, or scenario element. |
| Presentation Model | Engine-produced information a client may render for a given observer, context, time, and projection. It is derived, not authoritative. |
| Principal | The authenticated or otherwise locally identified security subject represented at the contract boundary. In single-player use this may be a default local Principal; authority over a Campaign still requires an active Campaign Authority Binding. |
| Procedure | A package-defined sequence used during play. Procedures may consult Rules and Oracles and may produce Events, State changes, or unresolved/resolved campaign information. |
| Projection | A purpose-specific representation of campaign information, such as a map, timeline, relationship view, journal index, current scene, or roster. |
| Property | A value or attribute associated with an Entity, Relationship, State, Event, or other campaign object. Packages define meaning and validation. |
| Provenance | Information identifying the source, mechanism, time, and context by which a piece of state, knowledge, or diagnostic record came to exist. |
| Recovery | Restoration of valid state after technical failure; not campaign history. |
| Relationship | A first-class connection between campaign objects. Packages define relationship categories and semantics. |
| Resolution | The process by which Unresolved State becomes established Campaign Reality through an attributable human decision, package-defined procedure, rule, or accepted Oracle result. |
| Resolution Command | An internal command describing an accepted attempt to resolve Unresolved State and apply an authoritative Campaign Reality change. |
| Resolution Record | A provenance-bearing record of a Resolution Command's outcome, including applied Event references or rejection diagnostics. |
| Retcon | Human-authorized revision of established Campaign Reality. A Retcon appends provenance-preserving correction history rather than rewriting prior Events. |
| Role-Based Access Control (RBAC) | Authorization based on role membership. In this project, roles may be used to assign capability bundles, but checks resolve through CBAC. |
| Rule | Package-defined semantics that interpret campaign information mechanically. |
| Ruleset Independence | The constraint that engine mechanics do not embed a specific RPG system, setting, genre, edition, or rules vocabulary as intrinsic semantics. |
| Scenario | An immutable package-defined starting configuration from which a Campaign may be instantiated. |
| Session | A bounded interaction context between a caller and HLA-CONTRACT carrying identity, actor, observer, requested-operation, and synchronization context as applicable. Session claims do not authorize an operation independently of the active Campaign Authority Binding. |
| Snapshot | A derived representation of component-owned state at a known Event boundary, used to bound replay cost. A snapshot is not more authoritative than committed Events. |
| State | The current or temporally bounded condition of campaign objects. |
| Transaction | A single call to a public engine operation, measured from call receipt to result return, exclusive of caller-side network or transport time. |
| Undo | Reversal of an accidental or unwanted operation. Undo does not rewrite campaign history and is distinct from Retcon. |
| Unresolved State | Explicitly represented campaign information that has not yet been resolved. |
| Validator Trace | Diagnostic validation detail that may be exposed only when authorized and requested at the appropriate diagnostic detail level. |
| Virtual Tabletop / VTT | A play-surface application category centered on maps, tokens, live table interaction, chat, media, visual scene composition, and client-side play ergonomics. A VTT may consume Campistoria engine contracts, but the Campistoria Engine is not itself a VTT. |
