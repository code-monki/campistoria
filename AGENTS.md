# Campistoria Agent Instructions

## Governing Toolkit

Campistoria development is governed by the local `ai-toolkit/` hierarchy at the repository root. Agents MUST consult and follow the relevant toolkit governance documents before performing work.

Use these toolkit areas as applicable:

- `ai-toolkit/01-foundations/`: operating principles and background.
- `ai-toolkit/02-governance/guardrails-index.md`: entry point for lifecycle and phase-specific guardrails.
- `ai-toolkit/02-governance/`: phase guardrails for ideation, requirements, architecture, detailed design, implementation, code documentation, system documentation, test, traceability, packaging, orchestration, and phase-gate review.
- `ai-toolkit/04-templates/`: required or expected project, system, and documentation artifact templates.
- `ai-toolkit/05-platform-config/`: platform-specific supporting guidance where relevant.

`ai-toolkit/` is local governance infrastructure. It is not part of the Campistoria product and MUST NOT be added to Git or copied into tracked project artifacts unless the user explicitly requests that specific action.

## Authority And Lifecycle

Approved normative Campistoria artifacts govern product semantics, including requirements, architecture, SRD/specifications, and similar approved project documents. The toolkit governs the process used to create and change those artifacts. Agents MUST NOT silently override either product artifacts or toolkit governance; surface conflicts or gaps to the user.

The next project activity is requirements engineering unless a later approved artifact explicitly changes the lifecycle state. Do not prematurely select implementation technology or introduce design decisions that belong to later lifecycle phases.

## Project Orientation

- `docs/concept/`: conceptual and ideation material.
- `docs/requirements/`: requirements artifacts.
- `docs/architecture/`: architecture artifacts.
- `docs/srd/`: system reference or specification material.
- `specifications/`: formal specifications where applicable.
- `engine/`: core engine code when implementation begins.
- `clients/reference/`: reference client code when implementation begins.
- `packages/samples/`: sample packages and examples.
- `tests/`: test assets.
- `tools/`: project tooling.

## AI Artifact Integrity

AI agents and tools are implementation aids and are NOT authors, co-authors, collaborators, contributors, publishers, or owners.

Agents MUST NOT add, insert, append, embed, or generate visible or hidden attribution, authorship/co-authorship claims, collaboration notices, provenance watermarks, AI-generated-content notices, signatures, branding, metadata, comments, markers, identifiers, or similar material identifying the agent, model, provider, tool, or service unless the user explicitly requests that specific material.

This rule applies to document text, source comments, headers, footers, metadata, generated-file properties, manifests, commit-message text, embedded identifiers, and machine-readable fields.

Agents MUST NOT alter existing authorship, copyright, licensing, ownership, attribution, or provenance unless explicitly instructed. AI assistance does not authorize modification of project attribution or ownership.
