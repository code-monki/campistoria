---
type: requirements-overview
title: "Software Requirements Specification"
description: "Summary of the Campistoria engine SRS: functional requirement groups (FR-001 through FR-045), non-functional requirements (NFR-001 through NFR-007), the deterministic-probabilistic requirements section, constraints, and assumptions."
tags: [requirements, srs, functional-requirements, non-functional-requirements]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-d233c2abd023a0d2cf2bcab7
    resource: repo://docs/requirements/engine-srs.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Software Requirements Specification

`docs/requirements/engine-srs.md` is the normative Software Requirements Specification (SRS) for the Campistoria engine — the first lifecycle artifact translating the [ideation-stage concept](../overview/project-concept.md) into individually identified, testable requirements. Per its own Requirements Authority Declaration, it exists to define *intent*, not structure: architectural commitments and implementation bias are explicitly out of scope for this document (see [Lifecycle Governance Discipline](../overview/governance-lifecycle.md)).

## Functional requirement groups

Requirements are grouped by capability area for readability; grouping does not imply architectural structure (the [HLA](../architecture/hla-overview.md) later assigns each group to owning components):

| Group | Requirement IDs | Covers |
|---|---|---|
| Campaign Lifecycle and Identity | FR-001, FR-002, FR-003, FR-040, FR-044, FR-045 | Campaign creation from Scenario, permanent identity, independent divergence, Scenario immutability, seed assignment, [Campaign Authority Binding](../reference/decision-log.md) |
| Campaign Reality | FR-004–FR-011 | Entities, locations, properties, relationships, facts, state, Campaign Time, history |
| Unresolved State and Resolution | FR-012, FR-013, FR-014 | Explicit unresolved state, Resolution provenance, Oracle boundary |
| Observers and Observer Knowledge | FR-015–FR-018 | Observer identity, separation from Campaign Reality, Information Source |
| Package Composition and Versioning | FR-019–FR-026 | Registration, composition, conflict handling, pinning, migration, removal, coexisting versions |
| Query, POV Resolution, and Presentation | FR-027–FR-030 | Query interface, POV authority, Presentation Model generation, Projection types |
| Persistence and Portability | FR-031, FR-032, FR-042, FR-043 | Durable storage, implementation-neutral import/export, asset portability, archival retrieval |
| Checkpoint, Undo, Recovery, and Retcon | FR-033–FR-036 | State-management lifecycle operations |
| Validation, Diagnostics, and Public Contracts | FR-037–FR-039 | Trust-boundary validation, structured diagnostics, symmetric public contracts |

Representative examples (full text lives in the SRS itself):

- **FR-001 (Campaign Creation From Scenario):** instantiating a Campaign from a Scenario produces a permanent, unique identity distinct from the Scenario's, and the Scenario's own content is unchanged afterward.
- **FR-003 (Independent Campaign Divergence):** multiple Campaigns instantiated from the same Scenario evolve independently with no automatic state propagation between them.
- **FR-044 (Campaign Seed Assignment):** the engine assigns and persists a seed value per Campaign for package-defined Oracles that choose to use it — the engine guarantees only that the seed is assigned, persisted, and exposed, not that any given Oracle's output is reproducible from it, and cross-implementation PRNG-sharing is explicitly left open for a future platform SRD.
- **FR-045 (Campaign Authority Binding):** added through a Spiral Development rollback on 2026-09-17 after Test Planning exposed that the actor-aware contract lacked a durable Campaign-to-Principal authority association — see [DEC-001](../reference/decision-log.md) for the full decision record.

## Non-functional requirements

| NFR | Category | Summary |
|---|---|---|
| NFR-001 | Performance | Local, single-process Transactions complete quickly enough not to disrupt the interactive solo play loop — provisionally, 95% within 500ms of engine-local processing time (marked *(OPEN)*, pending empirical validation; excludes network/transport time) |
| NFR-002 | Security | Packages, imported Campaign data, and external assets are validated at defined trust boundaries before incorporation, never assumed trustworthy |
| NFR-003 | Reliability | Authoritative Campaign state survives ordinary failures (interrupted write, crash) without silent corruption or unrecoverable provenance loss |
| NFR-004 | Scalability | The active working set is bounded to a recent-activity window (provisionally ~30 in-game days, *(OPEN)* pending calibration); aged-out data moves to archival storage while remaining retrievable on demand |
| NFR-005 | Maintainability | Engine mechanics, Package semantics, persistence, projection, validation, diagnostics, and public contracts remain separable |
| NFR-006 | Auditability | Consequential state changes, Package composition changes, migrations, and Retcon operations carry sufficient provenance for after-the-fact inspection |
| NFR-007 | Authorization | Every Campaign-scoped operation is authorized against active capabilities bound to the requesting Principal; caller-supplied role names or grants never self-authorize |

Several NFRs carry a quantitative target explicitly marked *(OPEN)* — the concept document deliberately deferred numeric targets, and per the project's Hallucination Guardrails, no numeric target is invented in the SRS itself; each open item must be resolved with stakeholder input before Architecture relies on it. NFR-007 was added alongside FR-045 through the same Spiral Development rollback (see [DEC-001](../reference/decision-log.md)).

## Deterministic–probabilistic requirements

A dedicated section bounds the engine's only permitted source of probabilistic behavior — Oracle-generated results — at the requirement level:

- Oracle-generated results must not directly mutate Campaign Reality (FR-014); the engine bounds *where in the pipeline* probabilistic output can influence state (only through Resolution, FR-013/FR-014), not the content of an Oracle's randomness itself, which is package-defined.
- Package-declared Oracle and Procedure outputs must conform to the Package's declared contract before being accepted as Resolution input (FR-037).
- No probabilistic subsystem — Oracle, Procedure, or any future AI-assisted tool — may write directly to Campaign Reality; all writes pass through Resolution, which requires an identifiable source (FR-013).
- If an Oracle or Procedure fails to produce a valid result, the engine leaves the associated element Unresolved rather than defaulting or guessing silently (FR-012).
- Every Resolution action records which Oracle/Procedure/rule/human source produced it (FR-013, NFR-006).
- The per-Campaign seed (FR-044) may be used by conformant Oracles for reproducibility within a single implementation, but this does not extend to physically-sourced randomness and does not assume cross-implementation PRNG sharing.

**Unbounded probabilistic integration — an Oracle or Procedure writing to Campaign Reality without passing through Resolution — is explicitly prohibited by this specification.** (See [HLA-RESOLUTION](../architecture/components/resolution.md) for how the architecture structurally enforces this.)

## Constraints and assumptions

The SRS restates and elaborates the ideation-stage conceptual constraints as formal requirements-level constraints: human authority over consequential decisions, deterministic/probabilistic separation, model independence (no dependency on a specific AI model/provider/runtime), ruleset independence, inspectable authoritative state, portability, lifecycle governance discipline, and no mandated technology stack.

Key assumptions, several explicitly flagged as uncertain: a single-campaign, single-user focus at this phase (multiplayer deferred); no regulated data categories currently in scope (flagged as uncertain, to be revisited if scope changes); no fixed budget or timeline (hobby-project context); trusted-enough Package authorship at this phase (structural/schema validation is the primary trust concern, not adversarial security review, since no networked/cloud package distribution is in scope yet); a durable storage mechanism will exist (technology undetermined, an Architecture-phase decision); and some caller exists to invoke engine operations through public contracts (no specific transport assumed).

## Error handling and edge conditions

The SRS explicitly specifies fail-safe behavior for common edge cases: a Package composition collision without an explicit override/extension is rejected, not silently resolved (FR-021); removing a Package version still referenced by an existing Campaign is rejected unless explicitly forced (FR-025); querying an Unresolved element returns its unresolved status without forcing generation of an answer (FR-012); an invalid import file is rejected with structured diagnostics rather than partially imported (FR-037/FR-038); an interrupted write recovers to the last valid state without manual repair (FR-035/NFR-003); a Package migration that fails partway leaves the Campaign in its prior valid, pinned composition (FR-041); and Campaign import never activates an exported foreign Principal identifier as a local authority grant (FR-045).
