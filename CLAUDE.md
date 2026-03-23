# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Medusa is a modular, interactive binary disassembler written in C++17. It supports multiple architectures (x86, ARM) and binary formats (PE, ELF) through a plugin-based architecture. Frontends include a Qt5 GUI (qMedusa), a text CLI, and Python bindings (pydusa via nanobind).

## Build Commands

```bash
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=<vcpkg_root>/scripts/buildsystems/vcpkg.cmake ..
cmake --build .
```

Key CMake options:
- `MEDUSA_BUILD_TOOLS` (default TRUE) — build executables (pydusa, qMedusa)
- `MEDUSA_BUILD_TESTS` (default TRUE) — build test suite
- `MEDUSA_MODULES_TO_BUILD` (default "all") — selective module compilation

Currently disabled modules: `arch_avr8`, `arch_st62`, `arch_z80`, `db_soci`, `db_sqlite3`, `ldr_gb`, `ldr_mach-o`, `ldr_st62`, `os_windows`.

## Dependencies

Managed via vcpkg (see `vcpkg.json`). Core: Boost (≥1.83), fmt, PEGTL, Catch2, nanobind. Optional: Qt5 (GUI), SQLite/sqlite-orm, LLVM, OGDF (graph layout).

## Running Tests

Tests use Catch2 v3. Run via ctest from the build directory:

```bash
ctest --timeout 30             # all tests
ctest -R test_core             # single test suite
```

Test suites: `test_core`, `test_arch`, `test_bind`, `test_compil`, `test_db`, `test_emul`, `test_expr`, `test_ldr`, `test_os`.

## Architecture

### Module/Plugin System

All extensibility is through dynamic shared libraries. Each module type has a trait defined in `inc/medusa/module.hpp` with a prefix convention and exported factory function:

| Type | Prefix | Directory | Factory |
|------|--------|-----------|---------|
| Architecture | `arch_` | `src/arch/` | `GetArchitecture` |
| Loader | `ldr_` | `src/ldr/` | `GetLoader` |
| Database | `db_` | `src/db/` | `GetDatabase` |
| Emulator | `emul_` | `src/emul/` | `GetEmulator` |
| Compiler | `compil_` | `src/compil/` | `GetCompiler` |
| Binding | `bind_` | `src/bind/` | `GetBinding` |
| OS | `os_` | `src/os/` | `GetOperatingSystem` |

New modules are added with `medusa_module()` macro from `cmake/medusa.cmake`.

### Core Components

- **Medusa** (`inc/medusa/medusa.hpp`) — main orchestrator: module loading, document management, analysis coordination
- **Document** (`inc/medusa/document.hpp`) — central data model holding cells, memory areas, xrefs, labels; uses Boost.Signals2 for observer pattern
- **Architecture** (`inc/medusa/architecture.hpp`) — abstract base for ISA disassembly and semantic analysis
- **Loader** (`inc/medusa/loader.hpp`) — binary format parsing and memory mapping
- **Analyzer** (`inc/medusa/analyzer.hpp`) — automatic analysis: function discovery, string detection, xref building
- **Database** (`inc/medusa/database.hpp`) — abstract storage backend; `db_memory` is the primary in-memory implementation

### Expression System

Semantic analysis uses an expression AST (`inc/medusa/expression.hpp`) with visitor pattern for traversal, simplification, and PEGTL-based text parsing (`src/core/expression_parser.cpp`).

### Python Bindings

- **pydusa** (`src/tools/pydusa/`) — nanobind-based Python extension module exposing the full medusa API
- **bind_python** (`src/bind/python/`) — embeds Python inside medusa for scripting; uses CPython C API (`PyRun_SimpleString`)

## CI

- AppVeyor for Windows builds (vcpkg)
- Travis CI for Linux builds (deprecated)
