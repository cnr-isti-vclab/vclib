#!/usr/bin/python3
import os;
import sys;
import subprocess;
import types;

SMOOTH = 0;
FLAT = 1;

UBER = 0;
SPLIT = 1;
UBER_IF = 2;

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
    

class Args:
    def __init__(self, shadingType: int, shadingSplitting: int, resolution: tuple[int,int], mesh: str):
        self.shadingType = shadingType
        self.shadingSplitting = shadingSplitting
        self.resolution = resolution
        self.mesh = mesh

    @staticmethod
    def combinatory(shadTypeList: list[int], shadSplitList: list[int], resList: list[tuple[int,int]], meshList: list[str]):
        for mesh in meshList:
            for shadType in shadTypeList:
                for shadSplit in shadSplitList:
                    for res in resList:
                        yield Args(shadType, shadSplit, res, mesh)

    def asArgList(self):
        ret = list()
        shadOpt = shadingTypeOpt(self.shadingType)
        splitOpt = shadingSplittingOpt(self.shadingType)
        if shadOpt is not None:
            ret.append(shadOpt)
        if splitOpt is not None:
            ret.append(splitOpt)
        if self.resolution is not None:
            ret.append("--res")
            ret.append(str(self.resolution[0]))
            ret.append(str(self.resolution[1]))
        ret.append(self.mesh)
        return ret

def run(executable_name: str, execution: Args):
    if not os.path.exists(execution.mesh):
            print(f"{execution.mesh} model not found, skipping", file=sys.stderr);
            return;
    resultPath = f"./results/{execution.mesh.split('/').pop()}";
    create_all_in_path(resultPath);
    subprocess.run([executable_name, *execution.asArgList()]);

def main():
    global SMOOTH, FLAT, UBER, SPLIT, UBER_IF
    executable_name = None;
    if os.name == "nt":
        executable_name = "./980-benchmark.exe"
    else:
        executable_name = "./980-benchmark"
    argsList = [
        # Example
        Args(SMOOTH, SPLIT, (1024, 768), "./usage_example/example.ply"),
        # Combinatory example
        Args.combinatory([FLAT, SMOOTH], [UBER, SPLIT, UBER_IF], [(800, 600)], ["./usage_example/example.ply"])
    ]
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