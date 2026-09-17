---
type: architecture-component
title: "HLA-RESOLUTION: Unresolved State and the Oracle Boundary"
description: "How HLA-RESOLUTION turns unresolved campaign information into authoritative state through provenance-bearing Resolution Commands, owning the deterministic-probabilistic boundary via a generic Oracle Adapter."
tags: [architecture, hla, command, adapter, oracle, deterministic-probabilistic-boundary]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-5ed2fe0189eb180d8681fca0
    resource: repo://docs/architecture/engine-hla.md
  - id: openwiki-source-59a0b8f4c90f25ff616cbb5b
    resource: repo://docs/design/components/hla-resolution-dd.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# HLA-RESOLUTION: Unresolved State and the Oracle Boundary

HLA-RESOLUTION turns unresolved or uncertain campaign information into authoritative Campaign Reality changes through controlled, provenance-bearing Resolution Commands. It is the **deterministic/probabilistic boundary** where human decisions, Procedures, Rules, and validated Oracle results can become state changes — without letting probabilistic output mutate reality directly.

## Why Command + Adapter

A Resolution is a request to change Campaign Reality that must be attributable to a source, auditable (NFR-006), and — per FR-034 — undoable. That is precisely GoF **Command**'s intent: encapsulate a request as an object so it can be logged, queued, and reversed. **Strategy** (interchangeable resolution algorithms) was the closest competing idea but doesn't naturally carry the "this specific invocation, with this source, happened at this time and can be undone" semantics that Command provides as a first-class property.

For Oracle normalization, the architecture uses **Adapter**: a single, engine-owned generic contract rather than per-Package bespoke Oracle Adapters. That alternative — letting each Package define its own Adapter shape — was considered and rejected on 2026-09-09: a single engine-owned contract lets HLA-VALIDATE check every Oracle result the same way instead of needing bespoke validation per Package, keeps NFR-006 provenance-recording logic written once instead of duplicated, and matches "the engine knows the meta-model, Packages know the game" more cleanly. The accepted trade-off is reduced per-Oracle flexibility, mitigated by keeping the generic contract's payload/result envelope deliberately minimal and opaque.

## Internal decomposition

| Internal Module | Responsibility | Persistent authority |
|---|---|---|
| `UnresolvedStateRegistry` | Tracks unresolved markers and target references | Unresolved marker metadata only |
| `ResolutionCommandFactory` | Converts accepted inputs into Resolution Command objects | None — transient structures |
| `ResolutionSourceNormalizer` | Normalizes human/Procedure/Rule/Oracle sources into provenance metadata | None |
| `OracleAdapter` | Invokes Package-defined Oracles through the generic adapter contract | None — cannot write Campaign Reality |
| `OracleResultAcceptor` | Sends Oracle results to HLA-VALIDATE, accepts/rejects as Resolution input | None — controls acceptance only |
| `ResolutionApplier` | Applies accepted Resolution Commands atomically to HLA-CORE | None — HLA-CORE owns the Event log |
| `UndoDescriptorBuilder` | Captures command metadata for HLA-STATE's Undo evaluation | None — HLA-STATE owns Undo execution |

## The exclusive write path

**HLA-RESOLUTION is the only component that writes resolved Campaign Reality changes to [HLA-CORE](core.md) after Campaign instantiation.** No Package code, Oracle output, Procedure output, Rule, or client ever mutates Campaign Reality directly — any design that lets probabilistic output bypass `Adapter → Validate → Resolution Command → HLA-CORE` is treated as a deterministic/probabilistic boundary failure.

## Unresolved State, Resolution Commands, and Undo descriptors

An `UnresolvedStateRef` marks a target as unresolved with `allowedResolutionSources[]`. A `ResolutionCommand` (source, payload, preconditions, expected effects, provenance) is built from an authorized input and applied atomically; the resulting `ResolutionRecord` carries an `outcome` of `applied`, `rejected`, `leftUnresolved`, `conflict`, or `componentFailure`. For every *applied* command, HLA-RESOLUTION also produces a reference-based `UndoDescriptor` (`inversePolicy`: `directInverse` / `compensatingResolution` / `nonUndoable`; `undoEligibility`: `eligible` / `blocked` / `unknown`) — HLA-RESOLUTION builds this descriptor but never executes Undo itself; that belongs to [HLA-STATE](state.md).

`declareUnresolved` is deliberately **internal only** — invoked by lifecycle setup, validated Package procedures/rules, or import/migration handling, but never exposed as a general public HLA-CONTRACT operation. A caller-facing unresolved-state proposal operation for GM tools would need its own separate, explicitly authorized design.

## The Oracle Adapter contract

The Adapter normalizes Oracle output into an `OracleResult`, preserves invocation metadata for provenance, and passes Package-defined opaque payloads through without interpreting game semantics beyond the declared contract. It provides an **invocation-specific random context** derived from the Campaign seed plus invocation-specific data — never the raw Campaign seed reused as the sole PRNG seed for every invocation.

Critically, **validation is eligibility only — it does not imply acceptance into Campaign Reality.** The Adapter never writes Campaign Reality, never treats Oracle output as authoritative, never synthesizes fallback values, and never records rejected Oracle results as HLA-CORE Events (they remain non-authoritative provenance/diagnostic records inside HLA-RESOLUTION).

## Resolution flow

**Human/Rule/Procedure flow:** HLA-CONTRACT authorizes → HLA-RESOLUTION confirms the target is still unresolved → source is normalized → a Command is built with preconditions/expected effects → `ResolutionApplier` applies it atomically to HLA-CORE → a Resolution Record is returned.

**Oracle-assisted flow:** HLA-CONTRACT authorizes → `OracleAdapter` invokes the Package-defined Oracle → `OracleResultAcceptor` submits the result to [HLA-VALIDATE](validate.md) → if rejected, HLA-RESOLUTION retains a non-authoritative invocation record, **leaves the target unresolved**, and returns diagnostics — it never guesses or defaults an answer; if accepted, the result becomes *eligible* Resolution input only — **a human/GM authority must explicitly accept the Oracle-derived input** before any Resolution Command can reference it, and a Resolution Command still has to be built and applied before Campaign Reality actually changes.

If a target is no longer unresolved, preconditions fail, or HLA-CORE rejects the write, the outcome is `conflict` or `rejected` with diagnostics and **no partial effect is ever applied.**

## Failure semantics

The recurring rule: any failure category leaves the target either unresolved or in conflict, **never partially resolved.** Oracle invocation failure or Oracle result validation failure both produce `leftUnresolved` with no fallback value — HLA-RESOLUTION never guesses, synthesizes, or defaults an answer when probabilistic input is invalid or unavailable.

## Dependency relationships

- **Writes exclusively to** [HLA-CORE](core.md) — no other component may write to Campaign Reality.
- **Depends on** [HLA-PACKAGE](package.md) for Oracle/Procedure/Rule definitions and [HLA-VALIDATE](validate.md) for Oracle result validation.
- **Cooperates with** [HLA-STATE](state.md), which reuses this component's Command infrastructure for Undo rather than requiring a separate write contract into HLA-CORE.

## Non-functional derivation

- **NFR-002 (Security):** Oracle results are validated by HLA-VALIDATE before use as Resolution input.
- **NFR-003 (Reliability):** Resolution Commands apply atomically; failed commands have no partial state effect.
- **NFR-005 (Maintainability):** internal modules separate Oracle adaptation, command construction, source normalization, application, and Undo descriptor creation.
- **NFR-006 (Auditability):** every successful Resolution records source/provenance and an Event reference sufficient for after-the-fact inspection.
