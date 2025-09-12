#! /usr/bin/env bash

# To use this script:
# 1. Create and activate a Python virtual environment
# 2. Install the required dependencies: pip setuptools wheel build
# 3. Run this script to build the wheel from the parent directory

# Set Qt6_DIR to the path where Qt6 is installed, if needed
# export Qt6_DIR=/path/to/qt6/

python -m build --wheel --outdir dist
pip install --force-reinstall dist/vclib*.whl
