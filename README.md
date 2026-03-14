# Cronus Test Workspace

This is a small, purpose-built repository for debugging and testing the Cronus server
during development. It contains multi-language, multi-file projects with known bugs
and test suites that exercise the agent's coding capabilities.

## Purpose

- **Dev debugging**: Run `./dev.sh` and point Cronus at this directory to test features
- **Feature testing**: Exercises edit format parsing, multi-file context, test generation
- **Quick iteration**: Small enough to scan instantly, big enough to be realistic

## Structure

```
test-workspace/
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

## Usage

```bash
# Start Cronus server pointed at this workspace
cd test-workspace
../build/linux_x64_debug/server/cronus/cronus --web --port 9000

# Or via dev.sh (auto-rebuild on server changes)
cd test-workspace && ../dev.sh --server
```

## Test Scenarios

Each project has intentional bugs. The `prompts/scenarios.json` file lists each
scenario with its description, expected fix, and which files are affected.
Tests are already written — a correct fix makes all tests pass.
