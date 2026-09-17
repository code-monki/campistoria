---
type: requirements-overview
title: "Requirements Traceability Matrix"
description: "How the RTM links every requirement to its owning HLA component, Detailed Design artifact, and Test Case ID, plus its deterministic-probabilistic boundary and NFR traceability sections and change-control lineage."
tags: [requirements, rtm, traceability, coverage]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-33243334d512b575358b7ab3
    resource: repo://docs/requirements/engine-rtm.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Requirements Traceability Matrix

`docs/requirements/engine-rtm.md` (currently v1.2, Approved) is the live instrument linking every requirement in the [SRS](srs-overview.md) to its owning [HLA component](../architecture/hla-overview.md), Detailed Design artifact, and Test Case ID. Its Authority Declaration section records that requirements, architecture, and detailed design are all approved and aligned, controlled Implementation is authorized, and Packaging/Release remain gated — Implementation, Packaging, and Orchestration columns stay unpopulated until those later phases produce actual artifacts.

## Core traceability matrix

The RTM's core table carries one row per Requirement ID (FR-001 through FR-045, NFR-001 through NFR-007) with columns for Requirement Type, the owning HLA Component ID, the Detailed Design artifact, Implementation Unit (currently `N/A` — no implementation phase reached), Test Case ID(s), Packaging/Orchestration references (currently `N/A`), Validation Status, and an Evidence Reference. Every row's Validation Status currently reads "Test planned; execution pending" — test cases exist and are mapped, but no test execution evidence exists yet because there is no implementation to execute against.

This table is where the [HLA's requirement-to-component mapping](../architecture/hla-overview.md) becomes durable and query-able rather than just narrative: for example FR-041 (Failed Migration Rollback) maps to *both* HLA-PACKAGE and HLA-STATE, reflecting that migration coordination genuinely spans both components' Detailed Designs, and FR-045 (Campaign Authority Binding) maps to HLA-CONTRACT, HLA-LIFECYCLE, *and* HLA-PERSIST — visibly reflecting the cross-component nature of the authority-binding amendment recorded in [DEC-001](../reference/decision-log.md).

## Deterministic–probabilistic boundary traceability

A dedicated section tracks the single deterministic–probabilistic boundary in the system as its own traceable unit: **DPB-001** (Oracle result vs. Resolution), tied to FR-014. Its row records the validation harness (HLA-VALIDATE, working with HLA-RESOLUTION), the containment logic (the Oracle Adapter plus HLA-RESOLUTION's Command boundary — no direct Campaign Reality writes), the fallback reference (FR-012, leave the element Unresolved), the observability reference (NFR-006 provenance via HLA-CORE/HLA-RESOLUTION event records and Oracle invocation provenance), and links to six Test Case IDs (TC-DPB-001 through TC-DPB-006). This section exists because the project's governance framework specifically requires deterministic–probabilistic boundaries to carry RTM containment mapping as their own traceability category, not just an ordinary FR row.

## Non-functional traceability

A separate table maps each NFR to its architectural enforcement mechanism, Design Artifact(s), and Test Case — for instance NFR-004 (Scalability) traces to the active-window/archival split through HLA-PERSIST with HLA-CORE/HLA-OBSERVER event logs, tested by TC-NFR-SCALE-001, and is flagged "calibration provisional" reflecting the open quantitative target noted in the [SRS](srs-overview.md#non-functional-requirements).

## Coverage validation

The RTM's coverage checklist is where the project asserts, row-by-row, that traceability is actually complete rather than merely intended: 100% FR-to-design coverage (FR-001 through FR-045 all have DD Artifact mappings), 100% NFR-to-design coverage, 100% requirement-to-test coverage (every FR and NFR maps to at least one approved Test Case ID), deterministic–probabilistic boundaries mapped through Test Planning, and no orphan artifacts (every HLA component traces to Requirement IDs and every Detailed Design artifact is referenced by the RTM or its parent). Implementation and Packaging/Orchestration traceability are marked `N/A` — not failures, but honest reflections of which lifecycle phases have actually produced artifacts yet.

## Change control and lineage

The RTM's change-control table is a dated log of every requirement-affecting change and whether it required a phase rollback. Most entries — adding FR-042/FR-043/FR-044, approving the SRS, approving the HLA, mapping Detailed Design artifacts — are marked "Phase Rollback Required: No," reflecting ordinary forward progression. The lone exception is the 2026-09-17 entry adding FR-045 and NFR-007 after a "missing durable solo Campaign-to-Principal authority association" was discovered during Test Case review — that entry is explicitly marked **"Yes — controlled rollback to Requirements,"** which is the RTM's record of the same event documented narratively in [DEC-001](../reference/decision-log.md) and structurally in [Lifecycle Governance Discipline](../overview/governance-lifecycle.md)'s Spiral Development discussion. The RTM is thus not just a snapshot but a lineage: it shows *when* rollback happened and *which* requirement IDs it touched, distinct from ordinary additive changes.
