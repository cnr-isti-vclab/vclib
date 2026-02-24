#!/usr/bin/python3
import os;
import sys;
import subprocess;
import types;
import platform;

SMOOTH = 0;
FLAT = 1;
VARYING = 2;

UBER = 0;
SPLIT = 1;
UBER_IF = 2;

COL_VERT = 0;
COL_FACE = 1;
TEX_VERT = 2;
TEX_WEDGE = 3;

def shadingTypeOpt(enum):
    global SMOOTH, FLAT, VARYING
    if enum == FLAT:
        return "--flat"
    elif enum == VARYING:
        return "--on-the-fly"
    else:
        return None

def shadingSplittingOpt(enum):
    global UBER, SPLIT, UBER_IF
    if enum == SPLIT:
        return "--split"
    elif enum == UBER_IF:
        return "--uber-static-if"
    else:
        return None
    
def coloringOpt(enum):
    global COL_VERT, COL_FACE, TEX_VERT, TEX_WEDGE;
    if enum == COL_VERT:
        return "--force-col-vertex"
    elif enum == COL_FACE:
        return "--force-col-face"
    elif enum == TEX_VERT:
        return "--force-tex-vertex"
    elif enum == TEX_WEDGE:
        return "--force-tex-wedge"

class Args:
    def __init__(self, shadingType: int, shadingSplitting: int, coloring: int, resolution: tuple[int,int], mesh: str, repsLis: list[int]):
        self.shadingType = shadingType
        self.shadingSplitting = shadingSplitting
        self.coloring = coloring
        self.resolution = resolution
        self.mesh = mesh
        self.repsLis = repsLis

    @staticmethod
    def combinatory(shadTypeList: list[int], shadSplitList: list[int], coloringList: list[int], resList: list[tuple[int,int]], meshList: list[str], repsLis: list[int]):
        for mesh in meshList:
            for shadType in shadTypeList:
                for shadSplit in shadSplitList:
                    for coloring in coloringList:
                        for res in resList:
                            yield Args(shadType, shadSplit, coloring, res, mesh, repsLis)

    def asArgList(self):
        ret = list()
        shadOpt = shadingTypeOpt(self.shadingType)
        splitOpt = shadingSplittingOpt(self.shadingSplitting)
        colOpt = coloringOpt(self.coloring)
        if shadOpt is not None:
            ret.append(shadOpt)
        if splitOpt is not None:
            ret.append(splitOpt)
        if self.resolution is not None:
            ret.append("--res")
            ret.append(str(self.resolution[0]))
            ret.append(str(self.resolution[1]))
        if colOpt is not None:
            ret.append(colOpt)
        if self.repsLis is not None:
            temp = ["--svc", "--fvc", "--sfc", "--ffc", "--swt", "--fwt"]
            for i in range(6):
                ret.append(temp[i])
                ret.append(str(self.repsLis[i]))
        ret.append(self.mesh)
        return ret

class ArgsGeneratorGenerator:
    def __init__(self, shadTypeList: list[int], shadSplitList: list[int], coloringList: list[int], resList: list[tuple[int,int]], meshList: list[str], repsLis: list[int]):
        self.shadTypeList = shadTypeList
        self.shadSplitList = shadSplitList
        self.coloringList = coloringList
        self.resList = resList
        self.meshList = meshList
        self.repsLis = repsLis

    def getGenerator(self):
        return Args.combinatory(self.shadTypeList, self.shadSplitList, self.coloringList, self.resList, self.meshList, self.repsLis)

def run(executable_name: str, execution: Args, output_file: str = "./test_results.json"):
    if not os.path.exists(execution.mesh):
            print(f"{execution.mesh} model not found, skipping", file=sys.stderr);
            return;
    subprocess.run([executable_name, *execution.asArgList(), "-o", output_file, "-f", "250", "--scale", "+1", "--device-name", platform.node()]);

def main():
    global SMOOTH, FLAT, UBER, SPLIT, UBER_IF, COL_VERT, COL_FACE, TEX_VERT, TEX_WEDGE
    repeat = 1
    if len(sys.argv) > 1:
        try:
            repeat = int(sys.argv[1])
        except (TypeError, ValueError):
            repeat = 1
    reps = [1,1,1,1,1,1]
    if len(sys.argv) > 2:
        temp = eval(sys.argv[2])
        if isinstance(temp, list):
            if len(temp) != 6:
                print("Repetions (position 2) argument must be of length 6")
                exit(1)
            for el in temp:
                if not isinstance(el, int):
                    print("Repetitions (position 2) argument must be an integer list")
                    exit(1)
            reps = temp
        else:
            print("Repetitions (position 2) argument must be an integer list")
            exit(1)
    executable_name = None;
    if os.name == "nt":
        executable_name = "./vclib-render-example-980-benchmark.exe"
    else:
        executable_name = "./vclib-render-example-980-benchmark"
    shading = [VARYING]
    coloring = [SMOOTH]
    output_file = "./test_results_varying.json"
    argsList = [
        ArgsGeneratorGenerator(shading, [UBER, SPLIT, UBER_IF], coloring, [(960, 960), (1920, 1920)], [
                "./meshes/small/myram.ply",
                "./meshes/medium/gargoyle500K.ply",
                "./meshes/big/ESTE_PRINT.ply"
            ], 
            reps
        )
    ]
    for _ in range(repeat):
        for args in argsList:
            if isinstance(args, ArgsGeneratorGenerator):
                for generatedArgs in args.getGenerator():
                    run(executable_name, generatedArgs, output_file);
            else:
                run(executable_name, args, output_file);
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