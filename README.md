# Cronus Test Workspace

A small C++ project with intentional bugs and missing features, used for testing
the Cronus agent's ability to fix bugs, add features, and perform refactors.

This repo is included as a **git submodule** of the main Cronus project, so it
functions as a real git repo — exactly what Cronus expects when managing agent
branches, commits, and PRs.

## Project: Inventory Manager

A simple inventory management library with:
- Item tracking (name, category, quantity, price)
- Case-insensitive name lookups
- Category-based filtering
- Aggregate queries (total count, total value)
- A minimal CLI interface

## Setup

```bash
# If cloning cronus for the first time:
git clone --recurse-submodules https://github.com/caseymcc/cronus.git

# If the submodule wasn't initialized:
cd cronus
git submodule update --init test-workspace
```

## Building

```bash
cd test-workspace
mkdir -p build && cd build
cmake ..
cmake --build .
```

## Running Tests

```bash
cd build
ctest --output-on-failure
# Or run directly:
./inventory_tests
```

## Usage with Cronus

### Quick start

```bash
# Full dev environment using test-workspace (auto-rebuild + web dev server)
./dev.sh --test

# Server only, pointed at test-workspace
./dev.sh --test --server
```

`dev.sh --test` automatically calls `setup.sh` before starting the server,
which resets the workspace to the buggy baseline and creates a fresh agent branch.

### Manual session management

```bash
cd test-workspace

# Reset to clean baseline and create a working branch
./setup.sh                          # auto-named: agent/session-<timestamp>
./setup.sh --branch fix/remove-bug  # explicit branch name
./setup.sh --clean                  # full reset: delete all branches, back to baseline
```

## Structure

```
test-workspace/
├── CMakeLists.txt          # Build configuration
├── setup.sh                # Session manager (reset, create branches)
├── tasks.json              # Known test tasks with expected fixes
├── include/
│   ├── item.h              # Item struct
│   └── inventory.h         # Inventory class
├── src/
│   ├── item.cpp            # Item implementation
│   ├── inventory.cpp       # Inventory implementation (has bugs)
│   └── main.cpp            # CLI interface
└── tests/
    ├── testFramework.h     # Minimal test macros (no external deps)
    ├── testMain.cpp        # Test runner entry point
    ├── itemTest.cpp        # Item tests (all pass)
    └── inventoryTest.cpp   # Inventory tests (some fail due to bugs)
```

## Test Tasks

The project has **5 known tasks** defined in `tasks.json` that exercise different
agent capabilities. These can be loaded as Cronus tasks to test agent performance.

### Bug Fixes

| ID | Title | Difficulty | Failing Tests |
|----|-------|-----------|---------------|
| `bug-remove-total-count` | `removeItem` does not update total item count | Easy | `RemoveItemUpdatesTotalCount`, `RemoveItemTotalValueUpdated` |
| `bug-category-case-sensitive` | `findItemsByCategory` is case-sensitive | Easy | `FindByCategoryCaseInsensitive` |

### New Features

| ID | Title | Difficulty | Tests |
|----|-------|-----------|-------|
| `feature-low-stock` | Add `getLowStockItems(threshold)` method | Easy | `GetLowStockItems`, `GetLowStockItemsNone`, `GetLowStockItemsEmpty` |
| `feature-json-serialization` | Add JSON serialization (`toJson`/`fromJson`) | Medium | `ToJson`, `FromJson`, `FromJsonEmpty`, `FromJsonInvalid` |

### Refactors

| ID | Title | Difficulty |
|----|-------|-----------|
| `refactor-extract-string-utils` | Extract `toLower`/`trim` into separate `stringUtils` module | Medium |

### Current Test Status (baseline)

Running `./inventory_tests` on the baseline code shows:
- **Item tests**: 5 pass
- **Inventory tests**: 10 pass, 3 fail
  - `RemoveItemUpdatesTotalCount` — FAIL (bug)
  - `RemoveItemTotalValueUpdated` — FAIL (bug)
  - `FindByCategoryCaseInsensitive` — FAIL (bug)

Feature tests are commented out and will fail at compile time until uncommented.

## Multi-Agent Workflow

Since this is a real git repo, Cronus can run its multi-agent workflow:

1. `main` branch always contains the buggy baseline
2. Each agent session works on its own branch (e.g., `agent/session-*`)
3. Agent makes commits as it fixes bugs
4. Branches can be compared with `git diff main..agent/session-*`
5. Reset between sessions with `./setup.sh --clean`
