# Engine Prototype Interoperability Experiment

Status: Experimental comparison, 2026-09-19. Evidence for [DEC-002](decision-log.md), not a product ABI, supported wrapper, or Packaging-phase validation.

## Experiment

The Rust and C++ storage slices each export the four functions in [the experimental header](../../prototypes/ffi/store_abi.h): create/free an opaque in-memory store, append a JSON event to a selected stream, and count events in that stream. The same [Python `ctypes` smoke test](../../prototypes/ffi/smoke.py) loads each shared library and checks successful appends, separate stream counts, rejection of blank event kind, invalid stream, and malformed JSON. The functions use only C-compatible pointers and integers; Rust and C++ object layouts are not exposed.

Run from the repository root on macOS after building both prototypes:

```sh
cargo build --manifest-path prototypes/rust-sqlite/Cargo.toml --release --locked
cmake -S prototypes/cpp-sqlite -B build/cpp-sqlite -DCMAKE_BUILD_TYPE=Release
cmake --build build/cpp-sqlite
python3 prototypes/ffi/smoke.py prototypes/rust-sqlite/target/release/libcampistoria_storage_prototype.dylib build/cpp-sqlite/libcampistoria_store_ffi.dylib
```

Both libraries passed on an arm64 Mac. Symbol inspection found the four named C exports in each. Linkage inspection found system `libsqlite3` and `libc++` dependencies in the default C++ shared library; the Rust library used bundled SQLite and did not list a separate SQLite library. That is a build/distribution observation, not a cross-platform support claim.

## Limits and Decision Impact

Both languages can expose a narrow, callable C boundary to a Python process. This removes a basic feasibility concern but does not distinguish their suitability for the full engine. The interface is intentionally minimal and uses undifferentiated `-1` errors. It lacks diagnostics, ABI versioning, thread-safety rules, durable-store handles, ownership rules for returned data, and any public HLA-CONTRACT behavior. A foreign caller must pass valid live handles and terminated strings; invalid pointers or double-free remain outside this smoke test. Native library loading does not prove Python, Ruby, Node, or Go packaging or compatibility across operating systems and architectures.

The next decision review should weigh these limits with maintainability, memory/lifetime safety, and intended distribution model. Stabilizing an ABI or implementing first-party wrappers remains later work under [DEC-003](decision-log.md) and normal lifecycle change control.
