from . import common
import bisect

def replace(cmake_file_string, list):
    start = 'set(HEADERS\n'
    end = ')'

    start_index = cmake_file_string.index(start)
    end_index = cmake_file_string.index(end, start_index + len(start))

    files = cmake_file_string[start_index + len(start):(end_index - 1)].split('\n')

    if list is not None:
        for new_header in list:
            item = '\t' + new_header
            pos = bisect.bisect_left(files, item)
            if pos == len(files) or files[pos] != item:
                bisect.insort(files, item)

        files_string ='\n'.join(files) + '\n'

        cmake_file_string = cmake_file_string[:start_index + len(start)] + files_string + cmake_file_string[end_index:]
    
    return cmake_file_string

def update_cmake_file(headers_list = None):
    target_file = "../CMakeLists.txt"

    # Read in the file
    with open(target_file, 'r') as file :
        cmake_file = file.read()

    cmake_file = replace(cmake_file, headers_list)

    with open(target_file, 'w') as file:
        file.write(cmake_file)

