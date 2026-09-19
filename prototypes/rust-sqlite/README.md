# Rust and SQLite storage prototype

This is an implementation-planning experiment for [DEC-002](../../docs/project/decision-log.md), not a conforming Campistoria Engine. It exercises a small part of HLA-PERSIST's local-storage criteria using a Rust library and bundled SQLite.

Run from this directory:

```sh
cargo test
```

The tests cover separate ordered Campaign Reality and Observer Knowledge event streams, rejection without an appended event, transaction rollback on a partially valid import, JSON export/import without copying a SQLite database file, file-backed persistence across reopen, and import of the [shared experimental snapshot](../fixtures/snapshot-v1.json).

This prototype does not implement Campaign Authority Bindings, Resolution, provenance rules, Package validation, archival retrieval, portable assets, crash recovery, or the approved public HLA-CONTRACT surface. Its JSON snapshot is experimental and does not establish a Campaign interchange format. Rust and SQLite remain proposed choices until the project owner approves a decision after reviewing the prototype evidence and remaining gaps.

The `benchmark` binary and its limits are documented in the [comparison report](../../docs/project/engine-prototype-benchmark.md).

The experimental C ABI and Python smoke test are documented in the [interop report](../../docs/project/engine-prototype-interop.md). They are not a stable ABI or distributable wrapper.
