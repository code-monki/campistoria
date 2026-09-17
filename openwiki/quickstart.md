---
type: quickstart
title: "Quickstart"
description: "Task-routing map for the Campistoria wiki: what the project is, its current documentation-only status, and where to find concept, requirements, architecture, component design, testing, and reference material."
tags: [quickstart, navigation, overview]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-23775c3de52f3ab95a13cb8b
    resource: repo://README.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Quickstart

Campistoria is an open, system-neutral engine for maintaining the persistent state of tabletop RPG campaigns — see [Campistoria Concept and Principles](overview/project-concept.md) for the full picture.

## Project status: documentation phase, no implementation yet

Campistoria is currently in the requirements/architecture/detailed-design phase of a strict, phase-gated engineering discipline (see [Lifecycle Governance Discipline](overview/governance-lifecycle.md)). **There is no implemented engine, reference client, package, or test yet.** The repository's `engine/`, `clients/reference/`, `specifications/campaign/`, `specifications/package/`, `packages/samples/`, and `tests/` directories exist but are currently empty placeholders reserved for later lifecycle phases — see [Repository Structure and Tooling](reference/repository-structure.md) for the full layout. Everything else in this wiki documents approved *specification* and *design* artifacts, not running behavior.

## Where to go

**Understand the project:**
- [Campistoria Concept and Principles](overview/project-concept.md) — what Campistoria is, its core conceptual invariants (Campaign Reality vs. Observer Knowledge, unresolved state, the engine/package split, human authority), and what it explicitly is not (not a VTT, not an AI-dependent system).
- [Lifecycle Governance Discipline](overview/governance-lifecycle.md) — the phase-gated engineering discipline every artifact in this repository follows, including the governance configuration, phase-gate conventions, and the local `ai-toolkit/` enforcement layer.

**Understand what's required:**
- [Software Requirements Specification](requirements/srs-overview.md) — the functional (FR-001–FR-045) and non-functional (NFR-001–NFR-007) requirements, the deterministic-probabilistic requirements bounding Oracle behavior, and project constraints/assumptions.
- [Requirements Traceability Matrix](requirements/traceability.md) — how every requirement traces to an architecture component, design artifact, and test case, plus the project's change-control lineage.

**Understand the architecture:**
- [High-Level Architecture Overview](architecture/hla-overview.md) — the Hexagonal Architecture style, the ten major components and their dependency graph, and the deterministic-probabilistic boundary.
- Ten component pages, one per HLA component: [HLA-LIFECYCLE](architecture/components/lifecycle.md), [HLA-CORE](architecture/components/core.md), [HLA-RESOLUTION](architecture/components/resolution.md), [HLA-OBSERVER](architecture/components/observer.md), [HLA-PACKAGE](architecture/components/package.md), [HLA-QUERY](architecture/components/query.md), [HLA-PERSIST](architecture/components/persist.md), [HLA-STATE](architecture/components/state.md), [HLA-VALIDATE](architecture/components/validate.md), and [HLA-CONTRACT](architecture/components/contract.md).

**Understand how it'll be tested:**
- [Test Strategy and Case Inventory](testing/test-strategy.md) — the test plan's scope and strategy per testing level, deterministic-probabilistic validation approach, and the Test Case ID conventions linking back to requirements.

**Reference material:**
- [Glossary](reference/glossary.md) — project terminology grouped by theme.
- [Project Decision Log](reference/decision-log.md) — recorded project decisions with context, rationale, and rejected alternatives (e.g., the Campaign Authority Binding decision that added FR-045/NFR-007).
- [Repository Structure and Tooling](reference/repository-structure.md) — the tracked documentation layout versus reserved-but-empty implementation directories, plus diagram validation tooling.
