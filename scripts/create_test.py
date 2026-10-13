#!/usr/bin/env python3
import os
import sys
import argparse
import shutil

def main():
    parser = argparse.ArgumentParser(description="Create a new test for vclib")
    parser.add_argument("test_name", help="The name of the test (e.g., 025-my-test)")
    # accept any unknown arguments as a potential module
    
    # Let's do custom parsing to easily support --<module>
    args = sys.argv[1:]
    
    module = "core"
    test_name = None
    
    for arg in args:
        if arg.startswith("--"):
            module = arg[2:]
        else:
            test_name = arg
            
    if not test_name:
        print("Usage: python create_test.py [--<module>] <test_name>")
        print("Example: python create_test.py --render 025-my-test")
        sys.exit(1)
        
    script_dir = os.path.dirname(os.path.abspath(__file__))
    repo_root = os.path.dirname(script_dir)
    
    templates_dir = os.path.join(script_dir, "templates", "test")
    dest_dir = os.path.join(repo_root, "tests", module, test_name)
    
    if os.path.exists(dest_dir):
        print(f"Error: directory '{dest_dir}' already exists.")
        sys.exit(1)
        
    os.makedirs(dest_dir, exist_ok=True)
    
    # Files to process
    files = ["main.cpp", "CMakeLists.txt"]
    
    for f in files:
        src = os.path.join(templates_dir, f)
        dst = os.path.join(dest_dir, f)
        
        if not os.path.exists(src):
            print(f"Warning: template '{f}' not found in '{templates_dir}'.")
            continue
            
        with open(src, "r") as src_file:
            content = src_file.read()
            
        content = content.replace("{{MODULE}}", module)
            
        with open(dst, "w") as dst_file:
            dst_file.write(content)
            
    print(f"Created test '{test_name}' in tests/{module}/{test_name}")

if __name__ == "__main__":
    main()
