import os
import glob
import re

def replace_header(file_path, header_string, old_start='/*', old_end='*/', line_comment_prefix='//'):
    with open(file_path, 'r') as file :
        file_string = file.read()

    # Handle shebang (must remain at the top)
    lines = file_string.split('\n')
    shebang = ""
    if lines and lines[0].startswith('#!'):
        shebang = lines[0] + '\n'
        file_string = '\n'.join(lines[1:])

    # We operate on the text after the shebang, ignoring leading empty lines
    stripped_file_string = file_string.lstrip()
    
    replaced = False
    keywords = ['Copyright', 'Mozilla']
    
    # 1. Check for old block header AT THE TOP
    if stripped_file_string.startswith(old_start):
        end_idx = stripped_file_string.find(old_end)
        if end_idx != -1:
            block = stripped_file_string[:end_idx + len(old_end)]
            if any(kw in block for kw in keywords):
                file_string = header_string + '\n\n' + stripped_file_string[end_idx + len(old_end):].lstrip()
                replaced = True

    # 2. Check for new line comment header AT THE TOP
    if not replaced:
        escaped_prefix = re.escape(line_comment_prefix)
        # Match a contiguous block of line comments
        pattern = r'^([ \t]*' + escaped_prefix + r'.*(?:\n|$))+'
        match = re.match(pattern, stripped_file_string)
        if match:
            matched_text = match.group(0)
            if any(kw in matched_text for kw in keywords):
                file_string = header_string + '\n\n' + stripped_file_string[match.end():].lstrip()
                replaced = True

    # 3. Check for old block header within the first 1000 characters as fallback
    if not replaced and old_start in file_string:
        start_idx = file_string.find(old_start)
        if 0 <= start_idx < 1000:
            end_idx = file_string.find(old_end, start_idx + len(old_start))
            if end_idx != -1:
                block = file_string[start_idx:end_idx + len(old_end)]
                if any(kw in block for kw in keywords):
                    file_string = file_string[:start_idx] + header_string + '\n\n' + file_string[end_idx + len(old_end):].lstrip()
                    replaced = True

    # 4. If no license header found, just insert the new header at the top
    if not replaced:
        file_string = header_string + '\n\n' + stripped_file_string
    
    # Prepend shebang back if it was there
    final_content = shebang + file_string

    with open(file_path, 'w') as file:
        file.write(final_content)

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
        elif file_path.endswith(('.h.in', '.h', '.hpp', '.mm', '.cpp.in','.cpp', '.sc', '.sh')):
            # if file_path does not contain 'varying.def.sc'
            if 'varying.def.sc' not in file_path:
                replace_header(file_path, header_string)

def replace_cmake_headers_in_dir(folder_path, recursive=True):
    # get the path where this script is located
    path = os.path.dirname(os.path.abspath(__file__))

    # Read in the file
    with open(path + '/templates/headings.txt', 'r') as file :
        header_string = file.read()
        # replace // with #
        header_string = re.sub(r'^//', '#', header_string, flags=re.MULTILINE)

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path) and recursive:
            replace_cmake_headers_in_dir(file_path)
        elif file_path.endswith(('CMakeLists.txt', '.cmake')):
            replace_header(file_path, header_string, old_start='#*', old_end='*/', line_comment_prefix='#')

def replace_python_headers_in_dir(folder_path, recursive=True):
    # get the path where this script is located
    path = os.path.dirname(os.path.abspath(__file__))

    # Read in the file
    with open(path + '/templates/headings.txt', 'r') as file :
        header_string = file.read()
        # replace // with #
        header_string = re.sub(r'^//', '#', header_string, flags=re.MULTILINE)

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path) and recursive:
            replace_python_headers_in_dir(file_path)
        elif file_path.endswith(('.py',)):
            replace_header(file_path, header_string, old_start='#*', old_end='*/', line_comment_prefix='#')

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
    replace_headers_in_dir('../3rdparty/cmake/templates/')
    replace_cmake_headers_in_dir('../', recursive=False)
    replace_cmake_headers_in_dir('../3rdparty/')
    replace_cmake_headers_in_dir('../cmake/')
    replace_cmake_headers_in_dir('../examples/')
    replace_cmake_headers_in_dir('../tests/')
    replace_python_headers_in_dir('../examples/')

    vcl_modules = ['bindings', 'core', 'external', 'render']

    # paths in which headings should not be replaced
    exclude_paths = ['bgfx/text/font', 'bgfx/font', 'bgfx_imgui']

    os.chdir('../vclib/')
    for module in vcl_modules:
        # Replace cmake headers in the module root directory (e.g., module/CMakeLists.txt)
        replace_cmake_headers_in_dir(module, recursive=False)

        if os.path.exists(module + '/include/vclib/'):
            replace_headers_in_dir(module + '/include/vclib/', exclude_paths)
            replace_cmake_headers_in_dir(module + '/include/vclib/', recursive=False)

        if os.path.exists(module + '/3rdparty'):
            replace_cmake_headers_in_dir(module + '/3rdparty', recursive=False)

        if os.path.exists(module + '/src/'):
            replace_headers_in_dir(module + '/src/', exclude_paths)

        if os.path.exists(module + '/shaders/'):
            replace_shader_headers_in_dir(module + '/shaders/', exclude_paths)
