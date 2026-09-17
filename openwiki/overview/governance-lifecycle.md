---
type: process-overview
title: "Lifecycle Governance Discipline"
description: "The phase-gated engineering discipline governing Campistoria's development: explicit lifecycle phases, mandatory gate approvals, traceability, and the local ai-toolkit/ enforcement layer."
tags: [governance, lifecycle, process, phase-gate, traceability]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-ea70eb6c045047448e446296
    resource: repo://.gitignore
  - id: openwiki-source-8037e2358a2c4f9b2c722a11
    resource: repo://AGENTS.md
  - id: openwiki-source-0591ae76bc956d7b47725358
    resource: repo://claude.md
  - id: openwiki-source-615487f2876b472d6b4fadc3
    resource: repo://docs/project/decision-log.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Lifecycle Governance Discipline

Campistoria is developed under an explicit, phase-gated engineering discipline rather than ad hoc iteration. Ideation, requirements, architecture, detailed design, implementation, testing, traceability, and packaging are treated as distinct but connected activities, and decisions are expected to be made at the appropriate lifecycle stage rather than introduced implicitly during a later one.

## The governance configuration

The repository root's `claude.md` (also read as `CLAUDE.md`) is the "Claude Enterprise Governance Configuration" — it configures any AI assistant working in the repository to act as a **lifecycle enforcement collaborator**, not just a code generator. It states explicit prohibitions: an assistant must not skip lifecycle phases, collapse ideation into requirements, introduce architectural decisions inside the SRS, modify architecture without a phase rollback, produce detailed design before HLA approval, generate implementation artifacts prematurely, or provide code when lifecycle prerequisites are incomplete.

The configuration defines discipline per phase:

- **Ideation:** clarify problem boundaries, explore alternatives, surface risks — explicitly avoiding SRS-level requirements, architectural modeling, or implementation detail. Premature convergence is prohibited.
- **Requirements:** separate FRs and NFRs, assign unique Requirement IDs, ensure measurable acceptance criteria, avoid architectural commitments. "Requirements define intent — not structure."
- **Architecture and Detailed Design:** every structural component and interaction must map to a recognized architectural or design pattern with an explicitly justified selection (e.g., "Using Strategy pattern to satisfy R-102 modularity requirements"), deterministic–probabilistic boundaries must be declared explicitly, and any proposed architecture requiring new scope mandates an SRS update and RTM revision *before* proceeding — architectural drift is prohibited.
- **Implementation, Testing, Traceability, Packaging:** code must reference Requirement IDs and conform to approved Detailed Design; testing requires explicit Test Case IDs and requirement-to-test mapping; traceability must preserve Requirement IDs, Architectural Component IDs, design references, and Test Case IDs with no orphan artifacts; release requires verified packaging automation and reproducibility.

A **Refusal Protocol** obligates the assistant to refuse advancing a lifecycle phase without gate satisfaction, generating implementation before design approval, ignoring RTM or containment-boundary gaps, or approving non-reproducible packaging.

## Phase gates in practice

<!-- openwiki: broken internal link [hla-overview.md] file "hla-overview.md" does not exist. Fix the href or restore the target, then delete this comment. -->
This discipline is directly visible in every approved `docs/` artifact. Each substantial document — the [SRS](../requirements/srs-overview.md), the [RTM](../requirements/traceability.md), the [HLA](hla-overview.md), each component Detailed Design, and the [test plan](../testing/test-strategy.md) — opens with a **Design/Requirements/Architectural Authority Declaration** section verifying that its prerequisite phase was approved and stating whether advancement is authorized, and closes with a **Phase Gate Declaration** and **Approval** section recording the document's own approval status and version. A document version like "v1.1 (Approved)" with an explicit prior-baseline note (e.g., "the prior v1.0 baseline remains historically approved") reflects this gate-and-amend discipline directly.

## The `ai-toolkit/` layer

`ai-toolkit/` at the repository root is the local governance infrastructure implementing this discipline in detail — `02-governance/00-lifecycle-bootstrap.md` plus guardrails per phase (ideation, requirements, architecture, detailed design, implementation, code documentation, system documentation, test, traceability, packaging, orchestration, and a phase-gate checklist) and `04-templates/` for required project/system/documentation artifact templates. Per `AGENTS.md`, `ai-toolkit/` is explicitly **local governance infrastructure, not part of the Campistoria product** — it is excluded from Git via `.gitignore` (`/ai-toolkit/`) and must not be added to version control or copied into tracked project artifacts unless a user explicitly requests that specific action. `AGENTS.md` directs agents to consult it before performing work, distinguishing it from the approved normative Campistoria artifacts (requirements, architecture, SRD/specifications) that govern product *semantics* — the toolkit governs the *process* used to create and change those artifacts.

## Spiral rollback, not linear-only progression

The lifecycle is not strictly one-directional. The [project decision log](../reference/decision-log.md) documents a "Spiral Development" model: decision DEC-001 (adding a Campaign Authority Binding) required a "controlled rollback from Test Planning to Requirements" because it addressed a discovered requirement/architecture omission — the log frames this explicitly as normal spiral behavior, "without treating future multiplayer discovery as a failure of the Spiral Development model." This is the mechanism by which the project reconciles strict phase-gating with the reality that later-phase work sometimes surfaces gaps in earlier-phase artifacts: the correction flows backward through the same gated discipline (SRS amendment, RTM revision, re-approval) rather than being patched in place.
