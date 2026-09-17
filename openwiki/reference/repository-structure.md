---
type: reference
title: "Repository Structure and Tooling"
description: "How the Campistoria repository is laid out — tracked documentation subtrees versus empty placeholder directories reserved for future lifecycle phases — plus its diagram validation tooling and MCP configuration."
tags: [reference, repository-structure, tooling, documentation]
verified:
  - by: openwiki/0.5.2
    at: 2026-09-17T22:44:59.369Z
sources:
  - id: openwiki-source-f5a489e5822d87c0b8fc66ef
    resource: repo://.mcp.json
  - id: openwiki-source-8037e2358a2c4f9b2c722a11
    resource: repo://AGENTS.md
  - id: openwiki-source-23775c3de52f3ab95a13cb8b
    resource: repo://README.md
  - id: openwiki-source-b48e59f2cb2b8baa9d8ed7d1
    resource: repo://tools/validate-svg-xml.py
generated: { by: "claude-code", at: "2026-09-17T22:44:59.369Z" }
---

# Repository Structure and Tooling

Campistoria is currently in the **requirements/architecture/detailed-design documentation phase** (see [Lifecycle Governance Discipline](../overview/governance-lifecycle.md)). Its repository layout reflects that: almost everything currently tracked in Git is documentation, and the directories reserved for actual engine/client/test code are empty placeholders awaiting later lifecycle phases.

## Tracked documentation subtrees

| Path | Contents |
|---|---|
| `README.md`, `LICENSE` | Project overview and Apache 2.0 license |
| `AGENTS.md` | Project-wide agent orientation and governance pointers |
| `claude.md` / `CLAUDE.md` | The lifecycle governance configuration (see [Lifecycle Governance Discipline](../overview/governance-lifecycle.md)) |
| `docs/concept/` | Ideation material (`engine-ideation.md`) |
| `docs/agent/` | Agent orientation material (`engine-primer.md`) |
| `docs/requirements/` | The SRS (`engine-srs.md`) and RTM (`engine-rtm.md`) |
| `docs/architecture/` | The approved HLA (`engine-hla.md`) |
| `docs/design/` | The engine-wide Detailed Design overview plus ten per-component Detailed Designs under `docs/design/components/` |
| `docs/test/` | The test plan and test case inventory |
| `docs/project/` | The [project decision log](decision-log.md) |
| `docs/glossary.md` | The project [glossary](glossary.md) |
| `docs/assets/diagrams/` | Exported HTML/SVG architecture and lifecycle diagrams (system context, conceptual model, campaign lifecycle, package composition, resolution/presentation flow) |
| `docs/index.html`, `docs/.nojekyll` | GitHub Pages publishing plumbing for the documentation site — not content in their own right |
| `tools/validate-svg-xml.py` | SVG XML validation tool (see below) |
| `.mcp.json` | Configures the `openwiki` MCP server (`openwiki mcp --host claude`) used to generate this wiki |

## Reserved, currently empty directories

The following directories exist locally but are **empty** and not tracked by Git (which does not track empty directories) — they are reserved locations per the README's repository-structure section for content that will exist once the project advances past documentation into implementation:

- `engine/` — the campaign engine implementation, once implementation is authorized.
- `clients/reference/` — the reference Campistoria client.
- `specifications/campaign/` and `specifications/package/` — normative machine- and implementation-facing campaign/package format specifications.
- `packages/samples/` — reference and demonstration packages.
- `tests/` — project verification assets.

None of these currently contain source code, specifications, or tests — do not infer implementation state from their presence in the directory tree.

## Diagram export validation

`tools/validate-svg-xml.py` validates every SVG file under `docs/assets/diagrams/*.svg` (or explicit paths passed as arguments) as strict, parseable XML using Python's `xml.etree.ElementTree`. Per `AGENTS.md`, this check is required whenever an exported SVG diagram is added or updated, before staging or committing — Markdown previewers and documentation hosts can silently show a broken-image icon when an SVG contains XML errors (duplicate attributes, unescaped ampersands) even though the source HTML diagram it was exported from renders correctly. The script prints `OK <path>` or `FAIL <path>: <error>` per file and exits non-zero if any file fails.

## MCP configuration

`.mcp.json` configures a single MCP server, `openwiki`, launched as `openwiki mcp --host claude`. This is the server behind the OpenWiki documentation generation workflow that produced this page.
