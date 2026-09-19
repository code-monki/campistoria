"""Cross-language smoke test of the two experimental storage ABIs."""

import ctypes
import json
import sys


def check(path):
    library = ctypes.CDLL(path)
    library.campistoria_store_new.restype = ctypes.c_void_p
    library.campistoria_store_free.argtypes = [ctypes.c_void_p]
    library.campistoria_store_append.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
    library.campistoria_store_append.restype = ctypes.c_int
    library.campistoria_store_count.argtypes = [ctypes.c_void_p, ctypes.c_int]
    library.campistoria_store_count.restype = ctypes.c_int

    handle = library.campistoria_store_new()
    assert handle, path
    try:
        valid = json.dumps({
            "campaign_id": "campaign-1", "kind": "created", "payload": {"source": "smoke"}
        }).encode()
        invalid = json.dumps({
            "campaign_id": "campaign-1", "kind": " ", "payload": {}
        }).encode()
        assert library.campistoria_store_append(handle, 0, valid) == 0
        assert library.campistoria_store_append(handle, 1, valid) == 0
        assert library.campistoria_store_append(handle, 0, invalid) == -1
        assert library.campistoria_store_append(handle, 2, valid) == -1
        assert library.campistoria_store_append(handle, 0, b"not json") == -1
        assert library.campistoria_store_count(handle, 0) == 1
        assert library.campistoria_store_count(handle, 1) == 1
        assert library.campistoria_store_count(handle, 2) == -1
    finally:
        library.campistoria_store_free(handle)
    print(f"passed: {path}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        raise SystemExit("usage: smoke.py RUST_LIBRARY CXX_LIBRARY")
    for library_path in sys.argv[1:]:
        check(library_path)
