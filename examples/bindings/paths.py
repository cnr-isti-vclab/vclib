import os

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = os.path.normpath(os.path.join(current_file_path, "../../assets/example_meshes"))
VCLIB_PYTHON_RESULTS_PATH = os.path.normpath(os.path.join(current_file_path, "../../assets/results/python"))
