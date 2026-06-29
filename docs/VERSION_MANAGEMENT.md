# VCLib Version Management

This document explains how version management works in VCLib.

## Overview

VCLib uses semantic versioning (major.minor.patch) with the VERSION file in the repository root as the single source of truth. The version is propagated to:

1. **VERSION file** (root directory) - Source of truth
2. **version.h.in** - Template for C++ header with version macros
3. **version.h** - Generated from version.h.in (do not edit directly!)
4. **CMakeLists.txt** - Reads VERSION for CMake project version
5. **setup.py** - Reads VERSION for Python package version

## Version Update Workflow

### Usage

To update the version, use the provided script:

```bash
python scripts/update_version.py <new_version>
# or
python3 scripts/update_version.py <new_version>
```

Example:
```bash
# Update to version 1.2.3
python3 scripts/update_version.py 1.2.3
```

This will automatically:
1. Update `VERSION` file in the root directory
2. Generate `vclib/core/include/vclib/version.h` from the `version.h.in` template

**Important**: The `version.h` file is automatically generated - do not edit it directly! 
Edit `version.h.in` template instead if you need to modify the structure.

### Dry Run

To see what would be changed without actually modifying files:

```bash
python scripts/update_version.py 1.2.3 --dry-run
```

## Using Version in C++ Code

The `version.h` header provides several macros:

```cpp
#include <vclib/version.h>

// Individual components
int major = VCLIB_VERSION_MAJOR;  // e.g., 1
int minor = VCLIB_VERSION_MINOR;  // e.g., 2
int patch = VCLIB_VERSION_PATCH;  // e.g., 3

// Full version string
const char* version = VCLIB_VERSION_STRING;  // e.g., "1.2.3"

// Numeric version for comparison (major * 10000 + minor * 100 + patch)
int versionNum = VCLIB_VERSION_NUMBER;  // e.g., 10203

// Version checking
#if VCLIB_VERSION_CHECK(1, 2, 0)
    // Code for version 1.2.0 or later
#endif
```
