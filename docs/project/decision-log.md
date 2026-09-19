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

### DEC-002: Initial Engine Language and Local Persistence

Date: 2026-09-19
Status: Accepted — approved by project owner 2026-09-19, subject to stated caveats

**Context:** Controlled Implementation is authorized by the approved Test Planning gate. Before this decision, no implementation language, storage technology, client platform, or deployment model had been selected. The first engine is expected to run locally inside apps or tools. No particular future client platform is assumed. This choice must preserve the approved HLA component boundaries and the technology-neutral public contract rather than silently selecting a UI or transport.

**Decision drivers:** NFR-002 requires validation at untrusted-input boundaries; NFR-004 and FR-043 require an active working set with archival retrieval; NFR-005 requires separable engine concerns; NFR-006 requires durable provenance; FR-032 requires implementation-neutral import/export; FR-041 requires atomic migration failure behavior; FR-042 requires portable asset references; FR-045 and NFR-007 require durable Campaign-scoped authorization. HLA-PERSIST Detailed Design section 6 further requires separate append-only streams, atomic import admission, local durability, integrity checks, and replaceable storage adapters.

**Alternatives considered:**

| Option | Strength for the first local engine | Cost or limitation |
|---|---|---|
| TypeScript on Node.js | Straightforward integration with future web-technology desktop or tool clients; static checks during development. | A different host language would need an interprocess adapter or binding; TypeScript types alone do not validate untrusted runtime data. |
| Python | Quick iteration, mature local tooling, and a standard-library SQLite interface. | Embedding in a non-Python client would need an interprocess adapter or binding; runtime validation and distribution need deliberate design. |
| Rust | Strong compile-time modeling of domain invariants and a native-library route to multiple hosts. | Greater implementation and binding complexity, especially before a client platform is known. |
| Modern C++ | Mature native-library ecosystem, direct SQLite C API access, and portable compiled output. | Resource and lifetime safety depend on disciplined implementation and testing; distribution and cross-toolchain ABI need deliberate boundaries. |
| C | Broadly usable library ABI and minimal language runtime assumptions. | Modeling the engine's stateful domain and ownership rules would require more manual discipline; retain as a candidate interface boundary, not a full-engine prototype unless evidence calls for one. |

**Decision:** Use Rust for the first engine core and SQLite as a replaceable local HLA-PERSIST adapter. The project owner prioritizes strong static guarantees, native portability, and minimal runtime dependencies over the shortest first implementation. The database is not the portable Campaign export format. This decision does not select a client runtime, public ABI, or wrapper implementation.

**Costs and risks:** Rust raises the development and binding effort compared with Python or TypeScript. SQLite permits only one writer at a time for a database; its database file alone does not satisfy portable assets or implementation-neutral export. The first release's single-user focus is not expected to need explicit write queuing. If multi-user or concurrent access is later brought out of the Waiting Room, revisit write contention and latency as part of that scope change. A single in-process writer with a request queue (potentially implemented as a singleton-like service) is one candidate for managing contention, not an approved design; it schedules writes rather than enabling simultaneous SQLite writers. Evaluate queue latency, backpressure, transaction ordering, shutdown/recovery, and multi-process access before choosing it; an in-process queue does not coordinate independent processes. If later validation fails, revisit the storage adapter without changing the HLA-PERSIST boundary. SQLite's [appropriate-use guidance](https://www.sqlite.org/whentouse.html) and [transaction documentation](https://www.sqlite.org/atomiccommit.html) support using it for the first local single-user engine, not assuming it satisfies all HLA-PERSIST requirements.

**Prototype evidence:** `prototypes/rust-sqlite/` builds as a Rust library with bundled SQLite. On 2026-09-19, `cargo test --locked` passed five focused tests covering separate ordered streams, rejected append, partial-import rollback, JSON transfer, file-backed reopen, and import of a shared experimental fixture; Clippy passed with warnings denied. `prototypes/cpp-sqlite/` builds as a C++20 static library using system SQLite and a checksum-pinned `nlohmann/json` build dependency. Its five equivalent checks passed through CTest, including the same shared fixture. The C++ test executable also passed an Apple Clang AddressSanitizer/UndefinedBehaviorSanitizer build before the interop adapter was added; this is a narrow test result, not proof of general memory safety. The default C++ library links to system SQLite, whereas the Rust slice bundles SQLite. These slices do not establish full engine conformance, a stable Campaign interchange format, supported native caller packages, or comparative production safety. The Rust slice was completed before active-effort tracking began; C++ effort is recorded in the [work effort log](work-effort-log.md).

**Performance experiment:** A [controlled storage-slice comparison](engine-prototype-benchmark.md) used the same SQLite 3.53.2 amalgamation and compile definitions for both release builds. Five measured runs after warm-up found in-memory append means of 2.20 µs/event (Rust) and 2.28 µs/event (C++), and file-backed append means of 231.39 and 224.04 µs/event respectively. Rust's read and JSON paths were faster in these implementations, but serializer and adapter choices are confounded with language. The experiment is not representative public-Transaction NFR-001 validation and does not settle DEC-002.

**Interoperability experiment:** The [shared C ABI smoke test](engine-prototype-interop.md) passed through Python `ctypes` against both native libraries on arm64 macOS. This shows a narrow foreign-language call boundary is feasible for either candidate, not that an ABI or language wrapper is ready. The default C++ library links system SQLite and `libc++`; the Rust test library bundles SQLite. Other operating systems, runtime packages, diagnostics, lifetime rules, and complete engine behavior remain untested.

**Review rationale:** The [stack review](engine-stack-review.md) recommended Rust plus replaceable SQLite, based chiefly on the requested static safety posture and demonstrated narrow native interop. C++ remains viable if a concrete future constraint warrants reconsideration. The prototypes are too small to establish whole-engine maintainability or comparative safety, and the unmeasured Rust session is not a numeric effort baseline. The owner approved the selection with these caveats.

**Implementation evidence still needed after selection:** Archival retrieval, asset portability, migration rollback, integrity/corruption handling, authorization, complete HLA-CONTRACT validation, representative NFR-001 measurement, and supported-platform build verification remain governed by the approved requirements, design, test plan, and RTM. The minimal foreign-caller test does not cover the complete engine contract. Selection does not approve an ABI, wrapper packages, Packaging, or Release.

**Alternatives disposition and scope impact:** Modern C++ was not selected for the first core because it offers no demonstrated SQLite or interoperability advantage that outweighs the owner's preference for Rust's compile-time guarantees; it remains a credible fallback. Python and TypeScript remain future wrapper/client ecosystems rather than the native core. C remains a possible interface language, not the full engine implementation. The implementation choice does not itself change approved requirements, HLA component ownership, Detailed Design behavior, or RTM mappings. Any later structural change requires normal impact review. Packaging and Release remain gated.

### DEC-003: First-Party Language Wrapper Ownership

Date: 2026-09-19
Status: Approved direction — implementation scope and release timing pending

**Context:** Platform portability includes access from client ecosystems, not just compiling the core on multiple operating systems. Independently maintained bindings could drift from the engine contract or lag releases. This concern is separate from the core-language and local-storage selection in DEC-002.

**Decision:** The project intends to own and maintain Python, Ruby, Node, and Go wrappers as first-party packages when a stable public engine interface and concrete integration scope are defined. Wrappers shall expose the same engine behavior rather than reimplement domain logic. A shared contract-test suite and fixtures shall run through each supported wrapper; a wrapper failure shall block a release that claims support for that wrapper. Version compatibility, supported runtime/OS/architecture matrix, packaging, and release synchronization must be specified before those packages are shipped.

**Timing and boundaries:** This approves ownership direction, not immediate implementation of four packages or a particular foreign-function interface. The ABI or other binding boundary is to be selected after the public contract and core technology are settled. Packaging and Release remain subject to their existing gates. Any new requirement, architecture, design, test-plan, or RTM scope must pass normal change control before implementation.

**Alternatives considered:** Relying entirely on community wrappers would reduce project maintenance but leave compatibility and release cadence outside project control. Building all wrappers alongside the storage prototypes would establish packaging before the public contract stabilizes and multiply churn. Neither is adopted for the initial engine slice.

**Consequences:** First-party ownership adds runtime-version, platform, packaging, CI, and support obligations even when implementation work is automated. Shared conformance tests provide evidence of synchronized behavior; code generation or AI assistance alone does not. The first wrapper can validate the binding boundary, with the others following under an explicit supported-platform plan.
