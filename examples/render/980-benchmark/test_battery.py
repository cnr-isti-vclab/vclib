#!/usr/bin/python3
import os;
import sys;
import subprocess;
import types;
import platform;

SMOOTH = 0;
FLAT = 1;

UBER = 0;
SPLIT = 1;
UBER_IF = 2;

COL_VERT = 0;
COL_FACE = 1;
TEX_VERT = 2;
TEX_WEDGE = 3;

def shadingTypeOpt(enum):
    global SMOOTH, FLAT
    if enum == FLAT:
        return "--flat"
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
    def __init__(self, shadingType: int, shadingSplitting: int, coloring: int, resolution: tuple[int,int], mesh: str):
        self.shadingType = shadingType
        self.shadingSplitting = shadingSplitting
        self.coloring = coloring
        self.resolution = resolution
        self.mesh = mesh

    @staticmethod
    def combinatory(shadTypeList: list[int], shadSplitList: list[int], coloringList: list[int], resList: list[tuple[int,int]], meshList: list[str]):
        for mesh in meshList:
            for shadType in shadTypeList:
                for shadSplit in shadSplitList:
                    for coloring in coloringList:
                        for res in resList:
                            yield Args(shadType, shadSplit, coloring, res, mesh)

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
        ret.append(self.mesh)
        return ret

def run(executable_name: str, execution: Args):
    if not os.path.exists(execution.mesh):
            print(f"{execution.mesh} model not found, skipping", file=sys.stderr);
            return;
    subprocess.run([executable_name, *execution.asArgList(), "-o", "./test_results.json", "-f", "250", "--scale", "+1", "--device-name", platform.node()]);

def main():
    global SMOOTH, FLAT, UBER, SPLIT, UBER_IF, COL_VERT, COL_FACE, TEX_VERT, TEX_WEDGE
    repeat = 1
    if len(sys.argv) > 1:
        try:
            repeat = int(sys.argv[1])
        except (TypeError, ValueError):
            repeat = 1
    executable_name = None;
    if os.name == "nt":
        executable_name = "./vclib-render-example-980-benchmark.exe"
    else:
        executable_name = "./vclib-render-example-980-benchmark"
    argsList = [
        Args.combinatory([FLAT, SMOOTH], [UBER, SPLIT, UBER_IF], [COL_VERT, COL_FACE, TEX_WEDGE], [(960, 960), (1920, 1920)], [
                "./meshes/small/myram.ply",
                "./meshes/medium/gargoyle500K.ply",
                "./meshes/big/ESTE_PRINT.ply"
            ]
        )
    ]
    for i in range(repeat):
        for args in argsList:
            if isinstance(args, types.GeneratorType):
                for generatedArgs in args:
                    run(executable_name, generatedArgs);
            else:
                run(executable_name, args);
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