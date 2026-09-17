# Campistoria Engine Decision Log

Project Name: Campistoria Engine
Version: 0.1 (Working Decision Log)
Date (YYYY-MM-DD): 2026-09-17
Author(s): CodeMonki
Status: Draft

---

## Decisions

### DEC-001: Campaign-Scoped Principal Authority Binding

Date: 2026-09-17  
Status: Accepted — approved 2026-09-17

**Context:** The approved v1.0 baseline defined Principals, Sessions, role bundles, and capability-based access control, but did not require a durable association between a solo player Principal and a Campaign. Treating caller-supplied roles or capabilities as sufficient would make Campaign authority implicit and spoofable. Deferring the entire concern to multiplayer would also leave the single-player ownership boundary undefined.

**Decision:** Add a durable `CampaignAuthorityBinding` that associates a locally recognized Principal with Campaign-scoped capabilities. HLA-CONTRACT owns and evaluates the binding, HLA-LIFECYCLE establishes the initial `campaignOwner` binding atomically with Campaign creation, and HLA-PERSIST stores the binding durably. Campaign import creates a new active local binding for the authorized importing Principal; source Principal and binding identifiers may be retained only as provenance and never become active local grants.

The existing ten-component HLA remains intact. A separate authorization component is not justified for v1 because the added responsibility fits the established HLA-CONTRACT boundary. Hosted accounts, authentication providers, invitations, and multiplayer authority administration remain deferred.

**Consequences:** Requirements add FR-045 and NFR-007. Architecture, affected Detailed Designs, Test Planning, the Test Case Inventory, and the RTM require coordinated amendments. Previously approved v1.0 artifacts remain historical baselines; advancement resumes only after the amendment set is reviewed and approved.

**Alternatives rejected:**

- Trust caller-supplied role or capability claims: rejected because it permits self-authorization.
- Treat the local process or Session as implicit owner: rejected because authority would not survive persistence or remain inspectable.
- Add a new HLA authorization component: deferred because it would add an unjustified boundary before hosted or multiplayer identity management exists.

**Spiral impact:** Controlled rollback from Test Planning to Requirements is required. The change addresses a discovered requirement and architecture omission without treating future multiplayer discovery as a failure of the Spiral Development model.
