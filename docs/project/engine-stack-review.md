# Initial Engine Stack Review

Date: 2026-09-19
Status: Reviewed; project owner approved the recommendation in [DEC-002](decision-log.md) on 2026-09-19 with the stated caveats.

## Decision Scope

Choose an initial implementation language and local storage adapter for the first embedded engine. This does not approve a public ABI, a client platform, first-party wrappers, a Campaign interchange format, or Packaging/Release. The approved architecture's HLA boundaries and public contract remain authoritative. The comparison uses the [storage benchmark](engine-prototype-benchmark.md), [foreign-call experiment](engine-prototype-interop.md), and the two experimental implementations under `prototypes/`.

## Evidence Review

| Criterion | Rust prototype | C++ prototype | Interpretation |
|---|---|---|---|
| Storage correctness | Five focused tests passed; Clippy passed with warnings denied. | Five equivalent checks passed; a narrow sanitizer run passed before the interop adapter was added. | Both meet the limited experiment, not the engine requirements. |
| SQLite timing | Append close to C++; faster read/JSON paths in this implementation. | Append close to Rust; slower read/JSON paths in this implementation. | No compelling SQLite performance basis for selection; serializer and adapter choices differ. |
| Foreign caller | Python loaded and exercised four C exports from a Rust `cdylib`. | Python loaded and exercised the same exports from a C++ shared library. | A narrow C boundary is feasible with either; neither has a stable ABI. |
| Static safety posture | Ownership and borrowing checks constrain safe Rust; `unsafe` remains at the FFI boundary and SQLite uses native code. | Native resource management relies on RAII and review; the C ABI adapter catches exceptions. Sanitizers cover only executed paths. | Rust better matches the stated preference for compile-time guarantees, without eliminating validation or native-code risk. |
| Local build inputs | Cargo lockfile; bundled SQLite in the tested build. | CMake; checksum-pinned JSON source; default system SQLite and C++ runtime linkage. A comparison-only build can use the same SQLite amalgamation. | Both are viable locally. Their current dependency and distribution models differ; no supported-platform matrix was tested. |
| Maintenance | Typed error enum and automatic connection/transaction lifetime management; explicit `unsafe` at foreign calls. | Direct SQLite API, explicit statement RAII, exception and transaction rollback handling. | Both codebases are readable at this scale. The prototypes are too small to predict whole-engine maintenance effort or human/agent productivity. |

## Recommendation

Select **Rust for the first engine core, with SQLite as a replaceable local HLA-PERSIST adapter**. The project owner approved this recommendation with the caveats in DEC-002. The decisive consideration is the requested static safety posture for a stateful engine handling imported data, coupled with a workable native-library route to future language wrappers. The benchmark does not establish a meaningful SQLite speed advantage for either language. C++ remains a legitimate alternative if a concrete client, toolchain, or integration constraint makes Rust's build or binding model a poor fit.

The storage format must remain internal; portable Campaign export and assets are separate contracts. Define the actual foreign-language boundary only after the public engine contract is stable. First-party Python, Ruby, Node, and Go wrapper ownership is governed separately by DEC-003 and must not silently enlarge this initial implementation slice.

## Limits and Revisit Triggers

This is a one-machine, small-workload comparison. It does not validate NFR-001, cross-platform compilation, crash durability, authorization, archive behavior, migration safety, or a supported package. Rust's `unsafe` FFI functions still require valid caller pointers and lifetime discipline. Neither prototype has complete error diagnostics or thread-safety rules at the foreign boundary.

Revisit the core language or storage adapter if implementation evidence shows an approved public contract cannot be supported cleanly, the target client has a binding constraint, supported platforms cannot build reproducibly, or representative full-engine tests expose unacceptable performance or maintainability costs. Such changes require the normal design/RTM impact review. Full-engine feature gaps are implementation and test obligations, not prerequisites to making this technology choice.

## Owner Decision

Rust plus replaceable SQLite was approved for the first engine core on 2026-09-19. The approval resolves DEC-002's technology selection only; it does not advance Packaging or Release, approve the experimental C ABI, or waive any approved requirement or test gate. A single queued writer remains only a possible response to observed contention, not part of the selected architecture. Reopening multi-user access should trigger a fresh concurrency and latency review; single-user use does not currently justify adding the queue.
