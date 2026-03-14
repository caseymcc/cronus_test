# Cronus Test Workspace

A purpose-built git repository for debugging and testing the Cronus server during
development. It contains multi-language, multi-file projects with known bugs and
test suites that exercise the agent's coding capabilities.

This repo is included as a **git submodule** of the main Cronus project, so it
functions as a real git repo — exactly what Cronus expects when managing agent
branches, commits, and PRs.

## Setup

```bash
# If cloning cronus for the first time:
git clone --recurse-submodules https://github.com/caseymcc/cronus.git

# If the submodule wasn't initialized:
cd cronus
git submodule update --init test-workspace
```

## Usage

### Quick start with dev.sh

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
./setup.sh                        # auto-named: agent/session-20260314-153000
./setup.sh --branch fix/calculator  # explicit branch name
./setup.sh --clean                # full reset: delete all branches, back to baseline

# Start server manually
../build/linux_x64_debug/server/cronus/cronus --web --port 9000
```

### Multi-agent workflow

Since this is a real git repo, Cronus can run its multi-agent workflow:

1. `main` branch always contains the buggy baseline
2. Each agent session works on its own branch (e.g., `agent/session-*`)
3. Agent makes commits as it fixes bugs
4. Branches can be compared with `git diff main..agent/session-*`
5. Reset between sessions with `./setup.sh --clean`

## Structure

```
test-workspace/
├── setup.sh              # Session manager (reset, create branches)
├── python/
│   ├── calculator/       # Multi-file Python project with known bugs
│   │   ├── calculator.py
│   │   ├── operations.py
│   │   └── test_calculator.py
│   └── text_processor/   # String processing with edge case bugs
│       ├── processor.py
│       └── test_processor.py
├── cpp/
│   ├── linked_list/      # Data structure with memory/logic bugs
│   │   ├── linked_list.h
│   │   ├── linked_list.cpp
│   │   └── test_linked_list.cpp
│   └── string_utils/     # String utilities with off-by-one errors
│       ├── string_utils.h
│       ├── string_utils.cpp
│       └── test_string_utils.cpp
├── javascript/
│   ├── task_manager/     # Async task manager with logic bugs
│   │   ├── taskManager.js
│   │   └── taskManager.test.js
│   └── data_transform/   # Data pipeline with transformation bugs
│       ├── transform.js
│       └── transform.test.js
└── prompts/              # Pre-written prompts for each bug scenario
    └── scenarios.json
```

## Test Scenarios

Each project has intentional bugs. `prompts/scenarios.json` lists 16 scenarios
with the prompt to send, expected files, test commands, and difficulty.

Run tests to see the bugs:

```bash
# Python (3 failures)
python3 -m unittest python/calculator/test_calculator.py

# Python (8 failures)
python3 -m unittest python/text_processor/test_processor.py

# C++ (4 failures)
cd cpp/linked_list && g++ -std=c++17 -o test test_linked_list.cpp linked_list.cpp && ./test

# C++ (5 failures)
cd cpp/string_utils && g++ -std=c++17 -o test test_string_utils.cpp string_utils.cpp && ./test

# JavaScript (1 failure)
node javascript/task_manager/taskManager.test.js

# JavaScript (2 failures)
node javascript/data_transform/transform.test.js
```

A correct fix makes all tests pass. The agent can run these commands to
discover what's broken and verify its work.
