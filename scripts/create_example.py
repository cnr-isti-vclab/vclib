#!/usr/bin/env python3
import os
import sys
import glob
import argparse
import shutil

def main():
    args = sys.argv[1:]
    
    module = "core"
    from_module = None
    example_arg = None
    
    for arg in args:
        if arg.startswith("--from-") and arg.endswith("-example"):
            # e.g., --from-core-example
            from_module = arg[7:-8]
        elif arg.startswith("--"):
            module = arg[2:]
        else:
            example_arg = arg
            
    if not example_arg:
        print("Usage: python create_example.py [--<module>] [--from-<module>-example] <example_name_or_number>")
        print("Example 1: python create_example.py --render 025-my-example")
        print("Example 2: python create_example.py --render --from-core-example 025")
        sys.exit(1)
        
    script_dir = os.path.dirname(os.path.abspath(__file__))
    repo_root = os.path.dirname(script_dir)
    
    example_name = example_arg
    if from_module:
        # Search for the example name in examples/from_module/
        search_dir = os.path.join(repo_root, "examples", from_module)
        matches = glob.glob(os.path.join(search_dir, f"{example_arg}-*"))
        if not matches:
            # Maybe it perfectly matches the whole directory?
            full_match = os.path.join(search_dir, example_arg)
            if os.path.exists(full_match) and os.path.isdir(full_match):
                matches = [full_match]
                
        if not matches:
            print(f"Error: could not find any example starting with '{example_arg}' in '{search_dir}'.")
            sys.exit(1)
        
        if len(matches) > 1:
            print(f"Error: multiple examples match prefix '{example_arg}' in '{search_dir}':")
            for m in matches:
                print("  ", os.path.basename(m))
            sys.exit(1)
            
        example_name = os.path.basename(matches[0])
        dest_dir = os.path.join(repo_root, "examples", module, from_module, example_name)
    else:
        dest_dir = os.path.join(repo_root, "examples", module, example_name)
    
    templates_dir = os.path.join(script_dir, "templates", "example")
    
    if os.path.exists(dest_dir):
        print(f"Error: directory '{dest_dir}' already exists.")
        sys.exit(1)
        
    os.makedirs(dest_dir, exist_ok=True)
    
    # Files to process
    files = ["main.cpp", "CMakeLists.txt", "requirements.cmake"]
    
    for f in files:
        src = os.path.join(templates_dir, f)
        dst = os.path.join(dest_dir, f)
        
        if not os.path.exists(src):
            print(f"Warning: template '{f}' not found in '{templates_dir}'.")
            # Create an empty file just in case it's requirements.cmake and missing
            if f == "requirements.cmake":
                open(dst, "w").close()
            continue
            
        with open(src, "r") as src_file:
            content = src_file.read()
            
        content = content.replace("{{MODULE}}", module)
        
        # If it's CMakeLists.txt and we are using --from-module-example
        if f == "CMakeLists.txt" and from_module:
            content = content.replace(
                f"project(vclib-{module}-example-${{EXAMPLE_NAME}})", 
                f"project(vclib-{module}-example-{from_module}-${{EXAMPLE_NAME}})"
            )
            content = content.replace(
                f"MODULE {module}", 
                f"MODULE {module}\n    {from_module.upper()}_EXAMPLE ${{EXAMPLE_NAME}}"
            )
            
        with open(dst, "w") as dst_file:
            dst_file.write(content)
            
    if from_module:
        print(f"Created example '{example_name}' in examples/{module}/{from_module}/{example_name} (from {from_module})")
    else:
        print(f"Created example '{example_name}' in examples/{module}/{example_name}")

if __name__ == "__main__":
    main()
