# Cronus Test Workspace# Cronus Test Workspace



A small C++ project with intentional bugs and missing features, used for testingA purpose-built git repository for debugging and testing the Cronus server during

the Cronus agent's ability to fix bugs, add features, and perform refactors.development. It contains multi-language, multi-file projects with known bugs and

test suites that exercise the agent's coding capabilities.

This repo is included as a **git submodule** of the main Cronus project, so it

functions as a real git repo — exactly what Cronus expects when managing agentThis repo is included as a **git submodule** of the main Cronus project, so it

branches, commits, and PRs.functions as a real git repo — exactly what Cronus expects when managing agent

branches, commits, and PRs.

## Project: Inventory Manager

## Setup

A simple inventory management library with:

- Item tracking (name, category, quantity, price)```bash

- Case-insensitive name lookups# If cloning cronus for the first time:

- Category-based filteringgit clone --recurse-submodules https://github.com/caseymcc/cronus.git

- Aggregate queries (total count, total value)

- A minimal CLI interface# If the submodule wasn't initialized:

cd cronus

## Setupgit submodule update --init test-workspace

```

```bash

# If cloning cronus for the first time:## Usage

git clone --recurse-submodules https://github.com/caseymcc/cronus.git

### Quick start with dev.sh

# If the submodule wasn't initialized:

cd cronus```bash

git submodule update --init test-workspace# Full dev environment using test-workspace (auto-rebuild + web dev server)

```./dev.sh --test



## Building# Server only, pointed at test-workspace

./dev.sh --test --server

```bash```

cd test-workspace

mkdir -p build && cd build`dev.sh --test` automatically calls `setup.sh` before starting the server,

cmake ..which resets the workspace to the buggy baseline and creates a fresh agent branch.

cmake --build .

```### Manual session management



## Running Tests```bash

cd test-workspace

```bash

cd build# Reset to clean baseline and create a working branch

ctest --output-on-failure./setup.sh                        # auto-named: agent/session-20260314-153000

# Or run directly:./setup.sh --branch fix/calculator  # explicit branch name

./inventory_tests./setup.sh --clean                # full reset: delete all branches, back to baseline

```

# Start server manually

## Usage with Cronus../build/linux_x64_debug/server/cronus/cronus --web --port 9000

```

### Quick start

### Multi-agent workflow

```bash

# Full dev environment using test-workspace (auto-rebuild + web dev server)Since this is a real git repo, Cronus can run its multi-agent workflow:

./dev.sh --test

1. `main` branch always contains the buggy baseline

# Server only, pointed at test-workspace2. Each agent session works on its own branch (e.g., `agent/session-*`)

./dev.sh --test --server3. Agent makes commits as it fixes bugs

```4. Branches can be compared with `git diff main..agent/session-*`

5. Reset between sessions with `./setup.sh --clean`

`dev.sh --test` automatically calls `setup.sh` before starting the server,

which resets the workspace to the buggy baseline and creates a fresh agent branch.## Structure



### Manual session management```

test-workspace/

```bash├── setup.sh              # Session manager (reset, create branches)

cd test-workspace├── python/

│   ├── calculator/       # Multi-file Python project with known bugs

# Reset to clean baseline and create a working branch│   │   ├── calculator.py

./setup.sh                          # auto-named: agent/session-<timestamp>│   │   ├── operations.py

./setup.sh --branch fix/remove-bug  # explicit branch name│   │   └── test_calculator.py

./setup.sh --clean                  # full reset: delete all branches, back to baseline│   └── text_processor/   # String processing with edge case bugs

```│       ├── processor.py

│       └── test_processor.py

## Structure├── cpp/

│   ├── linked_list/      # Data structure with memory/logic bugs

```│   │   ├── linked_list.h

test-workspace/│   │   ├── linked_list.cpp

├── CMakeLists.txt          # Build configuration│   │   └── test_linked_list.cpp

├── setup.sh                # Session manager (reset, create branches)│   └── string_utils/     # String utilities with off-by-one errors

├── tasks.json              # Known test tasks with expected fixes│       ├── string_utils.h

├── include/│       ├── string_utils.cpp

│   ├── item.h              # Item struct│       └── test_string_utils.cpp

│   └── inventory.h         # Inventory class├── javascript/

├── src/│   ├── task_manager/     # Async task manager with logic bugs

│   ├── item.cpp            # Item implementation│   │   ├── taskManager.js

│   ├── inventory.cpp       # Inventory implementation (has bugs)│   │   └── taskManager.test.js

│   └── main.cpp            # CLI interface│   └── data_transform/   # Data pipeline with transformation bugs

└── tests/│       ├── transform.js

    ├── testFramework.h     # Minimal test macros (no external deps)│       └── transform.test.js

    ├── itemTest.cpp        # Item tests (all pass)└── prompts/              # Pre-written prompts for each bug scenario

    └── inventoryTest.cpp   # Inventory tests (some fail due to bugs)    └── scenarios.json

``````



## Test Tasks## Test Scenarios



The project has **5 known tasks** defined in `tasks.json` that exercise differentEach project has intentional bugs. `prompts/scenarios.json` lists 16 scenarios

agent capabilities. These can be loaded as Cronus tasks to test agent performance.with the prompt to send, expected files, test commands, and difficulty.



### Bug FixesRun tests to see the bugs:



| ID | Title | Difficulty | Failing Tests |```bash

|----|-------|-----------|---------------|# Python (3 failures)

| `bug-remove-total-count` | `removeItem` does not update total item count | Easy | `RemoveItemUpdatesTotalCount`, `RemoveItemTotalValueUpdated` |python3 -m unittest python/calculator/test_calculator.py

| `bug-category-case-sensitive` | `findItemsByCategory` is case-sensitive | Easy | `FindByCategoryCaseInsensitive` |

# Python (8 failures)

### New Featurespython3 -m unittest python/text_processor/test_processor.py



| ID | Title | Difficulty | Tests |# C++ (4 failures)

|----|-------|-----------|-------|cd cpp/linked_list && g++ -std=c++17 -o test test_linked_list.cpp linked_list.cpp && ./test

| `feature-low-stock` | Add `getLowStockItems(threshold)` method | Easy | `GetLowStockItems`, `GetLowStockItemsNone`, `GetLowStockItemsEmpty` |

| `feature-json-serialization` | Add JSON serialization (`toJson`/`fromJson`) | Medium | `ToJson`, `FromJson`, `FromJsonEmpty`, `FromJsonInvalid` |# C++ (5 failures)

cd cpp/string_utils && g++ -std=c++17 -o test test_string_utils.cpp string_utils.cpp && ./test

### Refactors

# JavaScript (1 failure)

| ID | Title | Difficulty |node javascript/task_manager/taskManager.test.js

|----|-------|-----------|

| `refactor-extract-string-utils` | Extract `toLower`/`trim` into separate `stringUtils` module | Medium |# JavaScript (2 failures)

node javascript/data_transform/transform.test.js

### Current Test Status (baseline)```



Running `./inventory_tests` on the baseline code shows:A correct fix makes all tests pass. The agent can run these commands to

- **Item tests**: 5 passdiscover what's broken and verify its work.

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
