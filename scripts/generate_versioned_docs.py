#!/usr/bin/env python3
"""
Script to generate versioned documentation for each Git tag of the VCLib library.

This script:
1. Cleans existing documentation
2. Gets all Git tags from the repository
3. For each tag, checks out the corresponding code
4. Generates documentation using Doxygen with the version number set
5. Saves documentation in docs/_doxygen/html/${tag}/
6. Returns to the original branch

Usage: 
  python3 generate_versioned_docs.py                # Generate docs for all versions
  python3 generate_versioned_docs.py --verbose      # Show Doxygen output
  python3 generate_versioned_docs.py --devel        # Generate only devel version
  python3 generate_versioned_docs.py --verbose --devel  # Combine options
"""

import re
import shutil
import subprocess
import sys
import json
import argparse
from pathlib import Path
from typing import List


class VersionedDocsGenerator:
    def __init__(self, repo_root: Path, verbose: bool = False):
        self.repo_root = repo_root
        self.docs_dir = repo_root / "docs"
        self.doxyfile_path = self.docs_dir / "Doxyfile"
        self.output_base_dir = self.docs_dir / "_doxygen" / "html"
        self.original_branch = None
        self.verbose = verbose
        
        # Verify we are in a Git repository
        if not (repo_root / ".git").exists():
            raise RuntimeError(f"Directory {repo_root} is not a Git repository")
        
        # Verify that Doxyfile exists
        if not self.doxyfile_path.exists():
            raise RuntimeError(f"Doxyfile not found: {self.doxyfile_path}")

    def run_command(self, cmd: List[str], cwd: Path = None, capture_output: bool = True) -> subprocess.CompletedProcess:
        """Executes a command and handles errors."""
        if cwd is None:
            cwd = self.repo_root
        
        try:
            result = subprocess.run(
                cmd, 
                cwd=cwd, 
                capture_output=capture_output, 
                text=True, 
                check=True
            )
            return result
        except subprocess.CalledProcessError as e:
            print(f"Error executing command {' '.join(cmd)}: {e}")
            if e.stdout:
                print(f"STDOUT: {e.stdout}")
            if e.stderr:
                print(f"STDERR: {e.stderr}")
            raise

    def get_current_branch(self) -> str:
        """Gets the current branch name."""
        result = self.run_command(["git", "rev-parse", "--abbrev-ref", "HEAD"])
        return result.stdout.strip()

    def get_git_tags(self) -> List[str]:
        """Gets all Git tags from the repository."""
        result = self.run_command(["git", "tag", "--list"])
        tags = [tag.strip() for tag in result.stdout.split('\n') if tag.strip()]
        
        # Sort tags in semantic order (versions)
        def sort_key(tag):
            # Try to extract version numbers for semantic ordering
            version_match = re.search(r'(\d+)\.(\d+)\.(\d+)', tag)
            if version_match:
                return tuple(int(x) for x in version_match.groups())
            # If it's not a semantic version, use alphabetical ordering
            return (999, 999, 999, tag)
        
        tags.sort(key=sort_key)
        return tags

    def checkout_tag(self, tag: str) -> None:
        """Checks out a specific tag."""
        print(f"Checking out tag: {tag}")
        self.run_command(["git", "checkout", tag])

    def checkout_original_branch(self) -> None:
        """Returns to the original branch."""
        if self.original_branch:
            print(f"Returning to original branch: {self.original_branch}")
            self.run_command(["git", "checkout", self.original_branch])

    def create_doxyfile_for_version(self, version: str, output_dir: Path) -> Path:
        """Creates a temporary Doxyfile with the specified version number and output directory."""
        with open(self.doxyfile_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Update PROJECT_NUMBER with the version
        content = re.sub(
            r'^PROJECT_NUMBER\s*=.*$', 
            f'PROJECT_NUMBER         = "{version}"', 
            content, 
            flags=re.MULTILINE
        )
        
        # Update OUTPUT_DIRECTORY to point to the version-specific directory
        # Use a relative path from the repository root
        relative_output_dir = output_dir.relative_to(self.repo_root)
        content = re.sub(
            r'^OUTPUT_DIRECTORY\s*=.*$', 
            f'OUTPUT_DIRECTORY       = {relative_output_dir}', 
            content, 
            flags=re.MULTILINE
        )
        
        # Set HTML_OUTPUT to current directory to avoid creating html subdirectory
        content = re.sub(
            r'^HTML_OUTPUT\s*=.*$', 
            'HTML_OUTPUT            = .', 
            content, 
            flags=re.MULTILINE
        )
        
        # Create temporary file
        temp_doxyfile = self.repo_root / f"Doxyfile.{version}.tmp"
        with open(temp_doxyfile, 'w', encoding='utf-8') as f:
            f.write(content)
        
        return temp_doxyfile

    def generate_docs_for_version(self, version: str) -> bool:
        """Generates documentation for a specific version."""
        version_output_dir = self.output_base_dir / version
        
        # Create output directory
        version_output_dir.mkdir(parents=True, exist_ok=True)
        
        # Create temporary Doxyfile
        temp_doxyfile = self.create_doxyfile_for_version(version, version_output_dir)
        
        try:
            print(f"Generating documentation for version {version}...")
            
            # Run Doxygen with or without captured output based on verbose mode
            if self.verbose:
                result = self.run_command(["doxygen", str(temp_doxyfile)], capture_output=False)
            else:
                result = self.run_command(["doxygen", str(temp_doxyfile)], capture_output=True)
            
            print(f"Documentation generated successfully for version {version}")
            return True
            
        except subprocess.CalledProcessError as e:
            print(f"Error generating documentation for version {version}: {e}")
            # Show Doxygen output only on error (or if verbose is enabled)
            if e.stdout:
                print(f"Doxygen STDOUT:\n{e.stdout}")
            if e.stderr:
                print(f"Doxygen STDERR:\n{e.stderr}")
            return False
        finally:
            # Remove temporary file
            if temp_doxyfile.exists():
                temp_doxyfile.unlink()

    def clean_output_directory(self) -> None:
        """Cleans the output directory."""
        if self.output_base_dir.exists():
            print(f"Cleaning directory {self.output_base_dir}")
            shutil.rmtree(self.output_base_dir)
        self.output_base_dir.mkdir(parents=True, exist_ok=True)

    def copy_version_selector_script(self) -> None:
        """Copies the version selector JavaScript to the output directory."""
        script_source = self.repo_root / "scripts" / "templates" / "version-selector.js"
        script_destination = self.output_base_dir / "version-selector.js"
        
        if script_source.exists():
            print(f"Copying version selector script to {script_destination}")
            shutil.copy2(script_source, script_destination)
        else:
            print(f"Warning: version selector script not found at {script_source}")

    def generate_versions_list(self, versions: List[str]) -> None:
        """Generates a JSON file with the list of available versions."""
        versions_file = self.output_base_dir / "versions.json"
        
        # Sort versions with 'devel' first, then semantic versions in descending order
        sorted_versions = sorted(versions, key=lambda v: (
            0 if v == 'devel' else 1,  # 'devel' first
            *[-int(x) for x in v.replace('v', '').split('.') if x.isdigit()]  # semantic versions desc
        ))
        
        versions_data = {
            "versions": sorted_versions,
            "generated": subprocess.run(
                ["date", "-Iseconds"], 
                capture_output=True, 
                text=True
            ).stdout.strip()
        }
        
        print(f"Generating versions list file: {versions_file}")
        with open(versions_file, 'w', encoding='utf-8') as f:
            json.dump(versions_data, f, indent=2)

    def create_redirect_index(self, versions: List[str]) -> None:
        """Creates an index.html file that redirects to the latest version."""
        index_file = self.output_base_dir / "index.html"
        template_file = self.repo_root / "scripts" / "templates" / "index_redirect.html"
        
        # Determine the latest version
        if not versions:
            latest_version = "devel"
        else:
            # Sort to get the latest version (excluding devel)
            non_devel_versions = [v for v in versions if v != 'devel']
            if non_devel_versions:
                # Sort semantic versions to get the latest
                def version_key(v):
                    try:
                        parts = v.replace('v', '').split('.')
                        return tuple(int(x) for x in parts if x.isdigit())
                    except:
                        return (0, 0, 0)
                
                sorted_versions = sorted(non_devel_versions, key=version_key, reverse=True)
                latest_version = sorted_versions[0] if sorted_versions else "devel"
            else:
                latest_version = "devel"
        
        # Read template and replace placeholders
        if template_file.exists():
            with open(template_file, 'r', encoding='utf-8') as f:
                html_content = f.read()
            
            # Replace placeholder with actual version
            html_content = html_content.replace('{{LATEST_VERSION}}', latest_version)
            
            print(f"Creating redirect index.html to version: {latest_version}")
            with open(index_file, 'w', encoding='utf-8') as f:
                f.write(html_content)
        else:
            print(f"Warning: Template file not found at {template_file}")
            # Fallback to simple redirect
            html_content = f'''<!DOCTYPE html>
<html><head><meta http-equiv="refresh" content="0; url=./{latest_version}/"></head>
<body><p>Redirecting to <a href="./{latest_version}/">{latest_version}</a>...</p></body></html>'''
            
            print(f"Creating fallback redirect index.html to version: {latest_version}")
            with open(index_file, 'w', encoding='utf-8') as f:
                f.write(html_content)

    def generate_all_versions(self, devel_only: bool = False) -> None:
        """Generate documentation for all versions and current development branch."""
        # Save current branch
        self.original_branch = self.get_current_branch()
        print(f"Current branch: {self.original_branch}")
        
        # Get all tags (only if not devel_only mode)
        if devel_only:
            tags = []
            print("Running in devel-only mode - skipping tags")
        else:
            tags = self.get_git_tags()
            
            if not tags:
                print("No tags found in repository.")
            else:
                print(f"Found {len(tags)} tags: {', '.join(tags)}")
        
        # Always clean directory
        self.clean_output_directory()
        
        # Copy version selector script
        self.copy_version_selector_script()
        
        successful_versions = []
        failed_versions = []
        
        try:
            # First, generate documentation for the current development branch
            print("Generating documentation for current development branch (devel)...")
            try:
                if self.generate_docs_for_version("devel"):
                    successful_versions.append("devel")
                    print("Documentation for development branch generated successfully")
                else:
                    failed_versions.append("devel")
                    print("Failed to generate documentation for development branch")
            except Exception as e:
                print(f"Error processing development branch: {e}")
                failed_versions.append("devel")
            
            # Generate documentation for each tag (only if not devel_only mode)
            if not devel_only:
                for tag in tags:
                    try:
                        # Checkout tag
                        self.checkout_tag(tag)
                        
                        # Generate documentation
                        if self.generate_docs_for_version(tag):
                            successful_versions.append(tag)
                        else:
                            failed_versions.append(tag)
                            
                    except Exception as e:
                        print(f"Error processing tag {tag}: {e}")
                        failed_versions.append(tag)
        
        finally:
            # Always return to original branch
            try:
                self.checkout_original_branch()
            except Exception as e:
                print(f"Error returning to original branch: {e}")
        
        # Summary
        print("\n" + "="*50)
        print("DOCUMENTATION GENERATION SUMMARY")
        print("="*50)
        
        if successful_versions:
            print(f"Versions processed successfully ({len(successful_versions)}): {', '.join(successful_versions)}")
        else:
            print("No versions were processed successfully")
            
        if failed_versions:
            print(f"Failed versions ({len(failed_versions)}): {', '.join(failed_versions)}")
        else:
            print("No failures occurred")
            
        print(f"\nDocumentation saved in: {self.output_base_dir}")
        
        # Generate versions list file for JavaScript
        if successful_versions:
            self.generate_versions_list(successful_versions)
            # Create redirect index.html
            self.create_redirect_index(successful_versions)


def main():
    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description="Generate versioned documentation for VCLib using Doxygen"
    )
    parser.add_argument(
        '--verbose', '-v',
        action='store_true',
        help='Show Doxygen output during generation'
    )
    parser.add_argument(
        '--devel',
        action='store_true',
        help='Generate documentation only for the current branch (devel), skip all tags'
    )
    
    args = parser.parse_args()
    
    # Use the parent directory of this script as the repository root
    repo_root = Path(__file__).parent.parent
    
    # Verify that the root directory exists
    if not repo_root.exists():
        print(f"Error: Directory {repo_root} does not exist")
        sys.exit(1)
    
    try:
        generator = VersionedDocsGenerator(repo_root=repo_root, verbose=args.verbose)
        generator.generate_all_versions(devel_only=args.devel)
        
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
