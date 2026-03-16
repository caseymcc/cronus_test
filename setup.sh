#!/bin/bash
# test-workspace/setup.sh
#
# Prepares the test workspace for an agent debugging session.
#
# What this does:
#   1. Ensures the workspace is a git repo (initializes if needed)
#   2. Resets to the baseline commit (the intentional bugs)
#   3. Creates a fresh working branch for the agent to commit to
#
# Usage:
#   ./setup.sh                    # Reset and create branch "agent/session-<timestamp>"
#   ./setup.sh --branch fix/calc  # Reset and create branch "fix/calc"
#   ./setup.sh --clean            # Full reset: discard everything, back to baseline
#
# This enables the multi-agent PR workflow:
#   - main branch always has the buggy baseline
#   - Each agent session works on its own branch
#   - Agent commits are preserved for review
#   - Reset between sessions to get a clean slate

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

BRANCH_NAME=""  
CLEAN=0

# Parse arguments
for arg in "$@"; do
    case $arg in
        --branch=*)
            BRANCH_NAME="${arg#*=}"
            ;;
        --clean)
            CLEAN=1
            ;;
        --help|-h)
            echo "Usage: $0 [--branch=NAME] [--clean]"
            echo ""
            echo "  --branch=NAME  Create and switch to the given branch (default: agent/session-<timestamp>)"
            echo "  --clean        Full reset: discard all changes and branches, back to baseline"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $arg${NC}"
            exit 1
            ;;
    esac
done

# Ensure this is a git repo
if [ ! -d ".git" ]; then
    echo -e "${RED}Error: test-workspace is not a git repo.${NC}"
    echo "If test-workspace was added as a submodule, run:"
    echo "  git submodule update --init test-workspace"
    echo ""
    echo "Otherwise, initialize it manually — see test-workspace/README.md"
    exit 1
fi

# Mark this directory as safe for git (needed when running inside Docker
# as root, where the repo files are owned by the host user)
git config --global --get-all safe.directory 2>/dev/null | grep -qxF "$SCRIPT_DIR" \
    || git config --global --add safe.directory "$SCRIPT_DIR"

# Get the baseline commit (first commit on main)
BASELINE_COMMIT=$(git rev-list --max-parents=0 HEAD 2>/dev/null || echo "")

if [ -z "$BASELINE_COMMIT" ]; then
    echo -e "${RED}Error: No commits found. The test-workspace repo may be corrupt.${NC}"
    exit 1
fi

echo -e "${BLUE}Test workspace baseline: ${BASELINE_COMMIT:0:8}${NC}"

# Helper: reset the workspace to baseline while preserving this script.
# The baseline commit (first commit) doesn't contain setup.sh, README, or
# .cronus/, so we must save them before reset and restore after.
reset_to_baseline()
{
    # Save files that live outside the baseline commit
    local tmpdir
    tmpdir=$(mktemp -d)
    cp "$SCRIPT_DIR/setup.sh" "$tmpdir/setup.sh"
    [ -f "$SCRIPT_DIR/README.md" ] && cp "$SCRIPT_DIR/README.md" "$tmpdir/README.md"

    # Clean up any Cronus runtime state (stale locks, etc.)
    rm -rf "$SCRIPT_DIR/.cronus"

    # Discard uncommitted changes and untracked files
    git checkout -- . 2>/dev/null || true
    git clean -fd 2>/dev/null || true

    # Switch to main and hard-reset to baseline
    git checkout main 2>/dev/null || git checkout -b main
    git reset --hard "$BASELINE_COMMIT"

    # Restore preserved files
    cp "$tmpdir/setup.sh" "$SCRIPT_DIR/setup.sh"
    chmod +x "$SCRIPT_DIR/setup.sh"
    [ -f "$tmpdir/README.md" ] && cp "$tmpdir/README.md" "$SCRIPT_DIR/README.md"
    rm -rf "$tmpdir"
}

# Full clean mode: discard everything
if [ $CLEAN -eq 1 ]; then
    echo -e "${YELLOW}Full reset: discarding all changes and non-main branches...${NC}"

    reset_to_baseline

    # Delete all other branches
    for branch in $(git branch --format='%(refname:short)' | grep -v '^main$'); do
        git branch -D "$branch" 2>/dev/null || true
    done

    echo -e "${GREEN}Workspace fully reset to baseline.${NC}"
    exit 0
fi

# Generate branch name if not specified
if [ -z "$BRANCH_NAME" ]; then
    TIMESTAMP=$(date +%Y%m%d-%H%M%S)
    BRANCH_NAME="agent/session-$TIMESTAMP"
fi

reset_to_baseline

# Create and switch to the working branch
if git rev-parse --verify "$BRANCH_NAME" >/dev/null 2>&1; then
    echo -e "${YELLOW}Branch '$BRANCH_NAME' already exists, switching to it...${NC}"
    git checkout "$BRANCH_NAME"
else
    git checkout -b "$BRANCH_NAME"
    echo -e "${GREEN}Created branch: $BRANCH_NAME${NC}"
fi

echo ""
echo -e "${GREEN}Test workspace ready.${NC}"
echo -e "  Branch:   ${BLUE}$BRANCH_NAME${NC}"
echo -e "  Baseline: ${BLUE}${BASELINE_COMMIT:0:8}${NC} (main)"
echo ""
echo "The agent can now make changes and commit to this branch."
echo "Build and run tests:"
echo "  mkdir -p build && cd build && cmake .. && cmake --build . && ctest --output-on-failure"
