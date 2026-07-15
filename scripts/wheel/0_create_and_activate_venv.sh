#!/usr/bin/env bash

# Default Qt6_DIR
QT6_DIR_DEFAULT="/opt/Qt/6.10.3/gcc_64"
QT6_DIR="${1:-$QT6_DIR_DEFAULT}"

# Project root is two levels up from scripts/wheel
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." >/dev/null 2>&1 && pwd)"
cd "$PROJECT_ROOT" || exit 1

VENV_NAME="vclib_build_venv"

echo -e "\e[36mCreating virtual environment '$VENV_NAME' in progress...\e[0m"
python3 -m venv "$VENV_NAME"

ACTIVATE_SCRIPT="$VENV_NAME/bin/activate"

if [ -f "$ACTIVATE_SCRIPT" ]; then
    echo -e "\e[36mInserting the Qt6_DIR variable into the activation scripts...\e[0m"
    
    # Modify activate script
    echo "" >> "$ACTIVATE_SCRIPT"
    echo "# Automatic setting of Qt6_DIR for the build" >> "$ACTIVATE_SCRIPT"
    echo "export Qt6_DIR=\"$QT6_DIR\"" >> "$ACTIVATE_SCRIPT"

    echo -e "\e[36mInstalling required packages (build, wheel, setuptools)...\e[0m"
    
    PYTHON_EXE="$VENV_NAME/bin/python"
    "$PYTHON_EXE" -m pip install --upgrade pip
    "$PYTHON_EXE" -m pip install build wheel setuptools
    
    echo -e "\n\e[32mDone! The virtual environment is ready and the packages are installed.\e[0m"
    echo -e "\e[33mNOTE: A bash script cannot activate a venv for you permanently unless sourced.\e[0m"
    echo -e "To activate it in your current terminal, run this command from the project root:\n"
    echo -e "  \e[36msource $VENV_NAME/bin/activate\e[0m\n"
else
    echo -e "\e[31mUnable to find the activation script in $ACTIVATE_SCRIPT\e[0m" >&2
    exit 1
fi
