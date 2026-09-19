# C++ and SQLite storage prototype

This is a like-for-like implementation-planning experiment alongside [the Rust prototype](../rust-sqlite/README.md) for [DEC-002](../../docs/project/decision-log.md). It is not a conforming Campistoria Engine.

Build and test from the repository root:

```sh
cmake -S prototypes/cpp-sqlite -B build/cpp-sqlite
cmake --build build/cpp-sqlite
ctest --test-dir build/cpp-sqlite --output-on-failure
```

The build requires a C++20 compiler, CMake, SQLite development files, and network access on the first configure to fetch a checksum-pinned `nlohmann/json` release. It uses the system SQLite library rather than bundling SQLite, unlike the Rust prototype; this is a distribution difference to evaluate, not a semantic distinction. The test executable checks stream separation and JSON round trip, partial-import rollback, rejected append, file-backed reopen, and import of the [shared experimental snapshot](../fixtures/snapshot-v1.json).

This prototype does not implement Campaign Authority Bindings, Resolution, provenance rules, Package validation, archival retrieval, portable assets, crash recovery, or the approved public HLA-CONTRACT surface. The JSON snapshot is experimental and does not establish a Campaign interchange format.

An Apple Clang build with AddressSanitizer and UndefinedBehaviorSanitizer passed the same test executable. The result checks this narrow path only and does not establish memory safety for a full engine.

The optional `storage_benchmark` executable and matched-SQLite configuration are documented in the [comparison report](../../docs/project/engine-prototype-benchmark.md). The default system-SQLite build is not comparable to Rust's bundled-SQLite timing.

The optional shared-library interop experiment is documented in the [interop report](../../docs/project/engine-prototype-interop.md). It is not a stable ABI or distributable wrapper.
