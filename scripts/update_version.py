#!/usr/bin/env python3
"""
Script to update the version of VCLib in all relevant files.

Usage:
    python update_version.py <new_version>
    
Example:
    python update_version.py 1.2.3
"""

import argparse
import os
import re
import sys
from pathlib import Path


def parse_version(version_string):
    """Parse a semantic version string into major, minor, patch components."""
    match = re.match(r'^(\d+)\.(\d+)\.(\d+)$', version_string)
    if not match:
        raise ValueError(
            f"Invalid version format: '{version_string}'. "
            "Expected format: major.minor.patch (e.g., 1.2.3)"
        )
    return tuple(map(int, match.groups()))


def update_version_file(repo_root, version_string):
    """Update the VERSION file in the repository root."""
    version_file = repo_root / "VERSION"
    print(f"Updating {version_file}")
    with open(version_file, 'w') as f:
        f.write(version_string + '\n')
    print(f"  ✓ VERSION file updated to {version_string}")


def update_version_header(repo_root, version_string, major, minor, patch):
    """Generate version.h header file from version.h.in template."""
    version_header_in = repo_root / "vclib" / "core" / "include" / "vclib" / "version.h.in"
    version_header = repo_root / "vclib" / "core" / "include" / "vclib" / "version.h"
    
    if not version_header_in.exists():
        print(f"Error: Template file {version_header_in} does not exist!")
        sys.exit(1)
    
    print(f"Generating {version_header} from template")
    
    # Read template
    with open(version_header_in, 'r') as f:
        content = f.read()
    
    # Replace CMake-style placeholders
    content = content.replace('@VCLIB_VERSION_MAJOR@', str(major))
    content = content.replace('@VCLIB_VERSION_MINOR@', str(minor))
    content = content.replace('@VCLIB_VERSION_PATCH@', str(patch))
    content = content.replace('@VCLIB_VERSION_STRING@', version_string)
    
    # Write output
    with open(version_header, 'w') as f:
        f.write(content)
    
    print(f"  ✓ version.h generated from template:")
    print(f"    - VCLIB_VERSION_MAJOR: {major}")
    print(f"    - VCLIB_VERSION_MINOR: {minor}")
    print(f"    - VCLIB_VERSION_PATCH: {patch}")
    print(f"    - VCLIB_VERSION_STRING: \"{version_string}\"")


def main():
    parser = argparse.ArgumentParser(
        description='Update VCLib version in all relevant files',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s 1.0.0          Update to version 1.0.0
  %(prog)s 2.3.4          Update to version 2.3.4
        """
    )
    parser.add_argument(
        'version',
        help='New version number in semantic versioning format (major.minor.patch)'
    )
    parser.add_argument(
        '--dry-run',
        action='store_true',
        help='Show what would be updated without actually changing files'
    )
    
    args = parser.parse_args()
    
    # Parse and validate version
    try:
        major, minor, patch = parse_version(args.version)
    except ValueError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
    
    # Find repository root (where this script's parent is located)
    script_dir = Path(__file__).resolve().parent
    repo_root = script_dir.parent
    
    # Verify we're in the right place
    version_file = repo_root / "VERSION"
    if not version_file.exists():
        print(f"Error: VERSION file not found at {version_file}", file=sys.stderr)
        print("Make sure you're running this script from the repository.", file=sys.stderr)
        sys.exit(1)
    
    print(f"Repository root: {repo_root}")
    print(f"New version: {args.version}")
    print()
    
    if args.dry_run:
        print("DRY RUN MODE - No files will be modified")
        print()
    
    # Update files
    if not args.dry_run:
        update_version_file(repo_root, args.version)
        update_version_header(repo_root, args.version, major, minor, patch)
        print()
        print("✓ All version files updated successfully!")
        print()
        print("Next steps:")
        print("  1. Review the changes: git diff")
        print(f"  2. Commit the changes: git commit -am 'Bump version to {args.version}'")
        print(f"  3. Tag the release: git tag -a v{args.version} -m 'Version {args.version}'")
        print("  4. Push changes: git push && git push --tags")
    else:
        print(f"Would update VERSION to: {args.version}")
        print(f"Would generate version.h from version.h.in template with:")
        print(f"  - major={major}, minor={minor}, patch={patch}")


if __name__ == '__main__':
    main()
