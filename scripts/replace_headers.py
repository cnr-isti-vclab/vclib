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

def replace_headers_in_dir(folder_path):
    # get the path where this script is located
    path = os.path.dirname(os.path.abspath(__file__))

    # Read in the file
    with open(path + '/templates/header.txt', 'r') as file :
        header_string = file.read()

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path):
            replace_headers_in_dir(file_path)
        elif file_path.endswith(('.h', '.cpp')):
            replace_header(file_path, header_string)

def replace_cmake_headers_in_dir(folder_path, recursive=True):
    # get the path where this script is located
    path = os.path.dirname(os.path.abspath(__file__))

    # Read in the file
    with open(path + '/templates/header.txt', 'r') as file :
        header_string = file.read()
        # replace the first char of the line with a #
        header_string = re.sub(r'^.', '#', header_string, flags=re.MULTILINE)

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path) and recursive:
            replace_cmake_headers_in_dir(file_path)
        elif file_path.endswith(('CMakeLists.txt', '.cmake')):
            replace_header(file_path, header_string, start='#*')

def replace_shader_headers_in_dir(folder_path):
    # get the path where this script is located
    path = os.path.dirname(os.path.abspath(__file__))

    # Read in the file
    with open(path + '/templates/header.txt', 'r') as file :
        header_string = file.read()

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path):
            replace_shader_headers_in_dir(file_path)
        elif file_path.endswith(('.sc', '.sh')):
            replace_header(file_path, header_string)

if __name__ == "__main__":
    replace_headers_in_dir('../include/vclib/')
    replace_headers_in_dir('../test/')
    replace_cmake_headers_in_dir('../')
    replace_cmake_headers_in_dir('../include/vclib/')
    replace_cmake_headers_in_dir('../external/', recursive=False)
    replace_cmake_headers_in_dir('../cmake/')
    replace_cmake_headers_in_dir('../test/')

    # for vclib-render module, that has sources and shaders in different folders
    if os.path.exists('../src/'):
        replace_headers_in_dir('../src/')
    
    if os.path.exists('../shaders/'):
        replace_shader_headers_in_dir('../shaders/')
