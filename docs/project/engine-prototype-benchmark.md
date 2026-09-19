# Engine Storage Prototype Benchmark

Status: Experimental comparison, 2026-09-19. This is evidence for [DEC-002](decision-log.md), not an NFR-001 acceptance test.

## Method

Both release-optimized prototypes used the same SQLite 3.53.2 amalgamation and matching SQLite compile definitions. Rust used `rusqlite` 0.40.2 and `serde_json`; C++ used the SQLite C API and `nlohmann/json` 3.12.0. Measurements ran sequentially on an arm64 Mac with macOS 26.6.2 and Apple Clang 21.0.0.21000334. Five measured runs followed one warm-up run. The harness reports monotonic elapsed nanoseconds and operation counts as CSV. Values below are means of the five per-run per-operation times; ranges show the minimum and maximum run means. The in-memory workloads used 300 events, each with a Campaign ID, kind, integer index, and short text payload.

| Workload | Rust mean (range) | C++ mean (range) | Unit |
|---|---:|---:|---|
| Append, in-memory/autocommit | 2.20 (1.99–2.61) | 2.28 (2.10–2.73) | µs/event |
| Read and decode 300 events | 93.50 (84.55–112.89) | 142.34 (134.93–153.60) | µs/read |
| Export 300 events as JSON | 134.93 (124.05–151.64) | 384.80 (381.34–390.48) | µs/export |
| Import 300 events into a fresh store and verify count | 700.72 (671.84–796.57) | 874.19 (848.35–887.66) | µs/import |
| Append, file-backed/autocommit | 231.39 (195.78–312.71) | 224.04 (203.30–274.56) | µs/event |

The file-backed workload times 100 individual inserts on a newly created local database per run. It does not measure crash recovery or establish that data reached durable media. The import workload includes opening a fresh in-memory store and checking the imported event count. The read and export workloads operate on one populated in-memory store per run. Storage setup is outside the append timer. These are small-sample, single-machine observations, not confidence intervals.

## Reproduction

From the repository root, locate the `sqlite3` directory in the Cargo-cached `libsqlite3-sys-0.38.2` source, then run:

```sh
cargo test --manifest-path prototypes/rust-sqlite/Cargo.toml --locked
cargo run --manifest-path prototypes/rust-sqlite/Cargo.toml --release --bin benchmark
cmake -S prototypes/cpp-sqlite -B build/cpp-sqlite-comparison -DCMAKE_BUILD_TYPE=Release -DCAMPISTORIA_SQLITE_AMALGAMATION=/path/to/libsqlite3-sys-0.38.2/sqlite3
cmake --build build/cpp-sqlite-comparison
ctest --test-dir build/cpp-sqlite-comparison --output-on-failure
build/cpp-sqlite-comparison/storage_benchmark
```

Each benchmark writes CSV to standard output and the SQLite version to standard error. Run the executables sequentially on an otherwise idle machine. A normal C++ build without `CAMPISTORIA_SQLITE_AMALGAMATION` still uses system SQLite and is not an equal-SQLite comparison.

## Interpretation

The nearly equal append results suggest SQLite transaction cost dominates this narrow path. The read and JSON differences also reflect the different adapter and serialization implementations, so they are not isolated language effects. File-system caching, compiler settings, thermal state, run order, and small sample size remain uncontrolled. No public engine Transaction, Resolution, authorization, archive, asset, or migration workload was measured. In particular, these timings cannot validate the provisional NFR-001 p95 target of 500 ms. Use them to inform the stack review, not to approve a production performance claim.
