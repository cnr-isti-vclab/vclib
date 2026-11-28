#!/usr/bin/env bash

# Project root is two levels up from scripts/wheel
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." >/dev/null 2>&1 && pwd)"
cd "$PROJECT_ROOT" || exit 1

VENV_NAME="vclib_build_venv"
VENV_ACTIVATE="$VENV_NAME/bin/activate"

if [ -f "$VENV_ACTIVATE" ]; then
    echo -e "\e[36mActivating virtual environment...\e[0m"
    # Run in a subshell to keep the environment clean
    (
        source "$VENV_ACTIVATE"

        FAILED=()
        for f in examples/bindings/core/*.py; do
            [ -e "$f" ] || continue
            printf "=== Running %s ===\n" "$f"
            if ! python "$f"; then
                FAILED+=("$f")
            fi
            printf "\n"
        done

        if [ ${#FAILED[@]} -gt 0 ]; then
            echo -e "\e[31mFailed examples:\e[0m" >&2
            printf "  %s\n" "${FAILED[@]}" >&2
            exit 1
        fi
    )
else
    echo -e "\e[31mVirtual environment not found. Run scripts/wheel/0_create_and_activate_venv.sh first!\e[0m" >&2
    exit 1
fi
