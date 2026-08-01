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
        
        python3 -m build --wheel --outdir dist
        
        # Find the first wheel file in the dist directory
        WHEEL_FILE=$(ls dist/vclib-*.whl 2>/dev/null | head -n 1)
        
        if [ -n "$WHEEL_FILE" ]; then
            python3 -m pip install --force-reinstall "$WHEEL_FILE"
            
            echo -e "\nTo test any library example, remember to activate the virtual environment first:\n"
            echo -e "  \e[36msource $VENV_NAME/bin/activate\e[0m\n"
        else
            echo -e "\e[31mNo wheel found in dist folder!\e[0m" >&2
            exit 1
        fi
    )
else
    echo -e "\e[31mVirtual environment not found. Run scripts/wheel/0_create_and_activate_venv.sh first!\e[0m" >&2
    exit 1
fi
