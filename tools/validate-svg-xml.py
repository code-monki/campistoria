#!/usr/bin/env python3
"""Validate exported SVG diagrams as strict XML."""

from __future__ import annotations

import sys
from pathlib import Path
from xml.etree import ElementTree


DEFAULT_GLOB = "docs/assets/diagrams/*.svg"


def main() -> int:
    root = Path.cwd()
    paths = [Path(arg) for arg in sys.argv[1:]]
    if not paths:
        paths = sorted(root.glob(DEFAULT_GLOB))

    if not paths:
        print(f"No SVG files found for {DEFAULT_GLOB}", file=sys.stderr)
        return 1

    failed = False
    for path in paths:
        try:
            ElementTree.parse(path)
        except ElementTree.ParseError as exc:
            print(f"FAIL {path}: {exc}", file=sys.stderr)
            failed = True
        except OSError as exc:
            print(f"FAIL {path}: {exc}", file=sys.stderr)
            failed = True
        else:
            print(f"OK   {path}")

    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
