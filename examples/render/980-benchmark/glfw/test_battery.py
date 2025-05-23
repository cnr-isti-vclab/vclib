import os;
import sys;

def main():
    exec_path = None;
    if "980_BENCHMARK_PATH" in os.environ:
        exec_path = os.environ["980_BENCHMARK_PATH"];
    elif "980-benchmark" in os.listdir(os.getcwd()):
        exec_path = "./980-benchmark"
    else:
        raise Exception("Environment variable 980_BENCHMARK_PATH not defined and executable isn't present in the cwd");
    
    return;

if __name__ == "__main__":
    main();