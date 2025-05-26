
#!/usr/bin/python3
def main():
    import os;
    import sys;
    import subprocess;
    executable_name = None;
    if os.name == "nt":
        executable_name = "./980-benchmark.exe"
    else:
        executable_name = "./980-benchmark"
    shadingTypesOptions = ["", "--flat"];
    shadingSplittingOptions  = ["", "--split", "--uber-static-if"];
    resolutions = [["1920", "1080"], ["1366", "768"], ["1536", "864"], ["1280", "720"]];
    #TODO: add the meshes to be used
    meshes = [];
    for mesh in meshes:
        for shadingTypeOption in shadingTypesOptions:
            for shadingSplittingOption in shadingSplittingOptions:
                for resolution in resolutions:
                    subprocess.run([executable_name, shadingSplittingOption, shadingTypeOption, "--res", resolution[0], resolution[1], mesh]);
    return;

if __name__ == "__main__":
    main();