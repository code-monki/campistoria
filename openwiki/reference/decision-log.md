---
type: reference
title: "Project Decision Log"
description: "Record of significant Campistoria project decisions, their context, rationale, rejected alternatives, and downstream consequences for approved requirements/architecture/design artifacts."
tags: [reference, decisions, governance, authority-binding]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-615487f2876b472d6b4fadc3
    resource: repo://docs/project/decision-log.md
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Project Decision Log

`docs/project/decision-log.md` is the working decision log for the Campistoria Engine — a record of significant project decisions with their context, rationale, rejected alternatives, and downstream consequences. It exists to preserve *why* a decision was made, not just *what* changed, and to link decisions across the phase-gated lifecycle documented in [Lifecycle Governance Discipline](../overview/governance-lifecycle.md).

## DEC-001: Campaign-Scoped Principal Authority Binding

**Status:** Accepted, approved 2026-09-17.

**Context:** The approved v1.0 baseline defined Principals, Sessions, role bundles, and capability-based access control, but did not require a durable association between a solo player Principal and a Campaign. Treating caller-supplied roles or capabilities as sufficient would make Campaign authority implicit and spoofable; deferring the entire concern to a future multiplayer phase would also leave the single-player ownership boundary undefined.

**Decision:** Add a durable `CampaignAuthorityBinding` that associates a locally recognized Principal with Campaign-scoped capabilities. [HLA-CONTRACT](../architecture/components/contract.md) owns and evaluates the binding; [HLA-LIFECYCLE](../architecture/components/lifecycle.md) establishes the initial `campaignOwner` binding atomically with Campaign creation; [HLA-PERSIST](../architecture/components/persist.md) stores the binding durably. Campaign import creates a new active local binding for the authorized importing Principal — the source Principal and binding identifiers from an import artifact may be retained only as provenance and never become active local grants. The existing ten-component HLA was judged to remain intact: a separate, eleventh authorization component was not justified because the added responsibility fits the established HLA-CONTRACT boundary.

**Consequences:** This decision added FR-045 and NFR-007 to the requirements, and required coordinated amendments to the architecture, the affected Detailed Designs (HLA-CONTRACT, HLA-LIFECYCLE, HLA-PERSIST), Test Planning, the Test Case Inventory, and the RTM. Previously approved v1.0 artifacts remained historical baselines pending review and approval of the amendment set.

**Alternatives rejected:**

- Trusting caller-supplied role or capability claims — rejected because it permits self-authorization.
- Treating the local process or Session as implicit owner — rejected because authority would not survive persistence or remain inspectable.
- Adding a new, dedicated HLA authorization component — deferred because it would add an unjustified architectural boundary before hosted or multiplayer identity management exists.

**Spiral impact:** This decision required a controlled rollback from Test Planning back to Requirements — an explicit application of the project's Spiral Development model, addressing a discovered requirement/architecture omission rather than being treated as a failure of that model. See [Lifecycle Governance Discipline](../overview/governance-lifecycle.md) for how this rollback mechanism fits the broader phase-gated discipline.
