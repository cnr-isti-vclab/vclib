#!/usr/bin/python3
import os;
import sys;
import subprocess;

def main():
    executable_name = None;
    if os.name == "nt":
        executable_name = "./980-benchmark.exe"
    else:
        executable_name = "./980-benchmark"
    shadingTypesOptions = [
        {"option": "", "name": "smooth"},
        {"option": "--flat", "name": "flat"}
    ];
    shadingSplittingOptions  = [
        {"option": "", "name": "uber"}, 
        {"option": "--split", "name": "split"}, 
        {"option": "--uber-static-if", "name": "uber_static_if"}
    ];
    resolutions = [
        ["1920", "1080"], 
        ["1366", "768"], 
        ["1536", "864"], 
        ["1280", "720"]
    ];
    #TODO: add the meshes to be used
    meshes = [];
    for mesh in meshes:
        if not os.path.exists(mesh):
            print(f"{mesh} not found, skipping model...", file=sys.stderr);
        for shadingTypeOption in shadingTypesOptions:
            for shadingSplittingOption in shadingSplittingOptions:
                for resolution in resolutions:
                    resultPath = f"./results/{mesh.split('/').pop()}"
                    create_all_in_path(resultPath);
                    toRun = [
                        executable_name, 
                        shadingSplittingOption["option"], 
                        shadingTypeOption["option"], 
                        "--res", resolution[0], resolution[1], 
                        "-o", f"{resultPath}/{shadingSplittingOption['name']}_{shadingTypeOption['name']}_{resolution[0]}_{resolution[1]}.csv",
                        mesh
                    ];
                    subprocess.run([x for x in toRun if x!=""]);
    return;

def create_all_in_path(path: str):
    split_path = [x for x in path.split("/") if x!= ""];
    total_path = "";
    for partial in split_path:
        total_path+=partial
        if(not os.path.exists(total_path)):
            os.mkdir(total_path);
        total_path+="/";

if __name__ == "__main__":
    main();