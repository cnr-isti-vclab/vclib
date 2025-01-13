import os
import glob
import re

def replace_first_between(s, start, end, replacement):
    try:
        start_index = s.index(start)
        end_index = s.index(end, start_index + len(start))
        return s[:start_index] + replacement + s[end_index + len(end):]
    except ValueError:
        return s

def replace_header(file_path, header_string, start='/*', end='*/'):
    # Read in the file
    with open(file_path, 'r') as file :
        file_string = file.read()

    file_string = replace_first_between(file_string, start, end, header_string)

    with open(file_path, 'w') as file:
        file.write(file_string)

def replace_headers_in_dir(folder_path, exclude_paths = []):
    # get the path where this script is located
    path = os.path.dirname(os.path.abspath(__file__))

    # Read in the file
    with open(path + '/templates/headings.txt', 'r') as file :
        header_string = file.read()

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path):
            # for each string in exclude_paths, if the file_path contains the string, skip it
            if any(exclude in file_path for exclude in exclude_paths):
                continue
            replace_headers_in_dir(file_path, exclude_paths)
        elif file_path.endswith(('.h', '.cpp', '.sc', '.sh')):
            # if file_path does not contain 'varying.def.sc'
            if 'varying.def.sc' not in file_path:
                replace_header(file_path, header_string)

def replace_cmake_headers_in_dir(folder_path, recursive=True):
    # get the path where this script is located
    path = os.path.dirname(os.path.abspath(__file__))

    # Read in the file
    with open(path + '/templates/headings.txt', 'r') as file :
        header_string = file.read()
        # replace the first char of the line with a #
        header_string = re.sub(r'^.', '#', header_string, flags=re.MULTILINE)

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path) and recursive:
            replace_cmake_headers_in_dir(file_path)
        elif file_path.endswith(('CMakeLists.txt', '.cmake')):
            replace_header(file_path, header_string, start='#*')

def replace_shader_headers_in_dir(folder_path, exclude_paths = []):
    # get the path where this script is located
    path = os.path.dirname(os.path.abspath(__file__))

    # Read in the file
    with open(path + '/templates/headings.txt', 'r') as file :
        header_string = file.read()

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path):
            # for each string in exclude_paths, if the file_path contains the string, skip it
            if any(exclude in file_path for exclude in exclude_paths):
                continue
            replace_shader_headers_in_dir(file_path, exclude_paths)
        elif file_path.endswith(('.sc', '.sh')):
            replace_header(file_path, header_string)

if __name__ == "__main__":
    replace_headers_in_dir('../examples/')
    replace_headers_in_dir('../tests/')
    replace_cmake_headers_in_dir('../', recursive=False)
    replace_cmake_headers_in_dir('../cmake/')
    replace_cmake_headers_in_dir('../examples/')
    replace_cmake_headers_in_dir('../tests/')

    vcl_modules = ['core', 'external', 'processing', 'render']

    # paths in which headings should not be replaced
    exclude_paths = ['bgfx/text/font', 'vclib/bgfx_imgui']

    os.chdir('../vclib/')
    for module in vcl_modules:
        replace_headers_in_dir(module + '/include/vclib/', exclude_paths)
        replace_cmake_headers_in_dir(module + '/include/vclib/', recursive=False)

        if os.path.exists(module + '/3rdparty'):
            replace_cmake_headers_in_dir(module + '/3rdparty', recursive=False)

        if os.path.exists(module + '/src/'):
            replace_headers_in_dir(module + '/src/', exclude_paths)
        
        if os.path.exists(module + '/shaders/'):
            replace_shader_headers_in_dir(module + '/shaders/', exclude_paths)
