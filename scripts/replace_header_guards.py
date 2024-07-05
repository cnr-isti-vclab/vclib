import os
import glob

def replace_header_guard(file_string, file_path):
    # remove extension from file_path
    file_path = os.path.splitext(file_path)[0]

    # replace '/' with '_' in the file path
    header_guard = file_path.replace('/', '_').upper()

    # add 'VCL_' prefix and '_H' suffix
    header_guard = 'VCL_' + header_guard + '_H'

    # look for the first line that starts with '#ifndef', and replace that line
    # and the following one with the new header guard
    lines = file_string.split('\n')
    for i, line in enumerate(lines):
        if line.startswith('#ifndef'):
            lines[i] = '#ifndef ' + header_guard
            lines[i + 1] = '#define ' + header_guard
            break

    # now replace the latest line that starts with '#endif' and replace it with
    # a new line in the form of '#endif // ' + header_guard
    for i, line in enumerate(reversed(lines)):
        if line.startswith('#endif'):
            lines[-i - 1] = '#endif // ' + header_guard
            break
    
    return '\n'.join(lines)

def replace_header_guards_in_dir(folder_path):
    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path):
            replace_header_guards_in_dir(file_path)
        elif file_path.endswith(('.h', '.hpp')):
            # Read in the file
            with open(file_path, 'r') as file:
                file_string = file.read()

            file_string = replace_header_guard(file_string, file_path)

            with open(file_path, 'w') as file:
                file.write(file_string)

if __name__ == "__main__":
    # move to the root directory '../include/vclib/'
    os.chdir('../include/vclib/')

    replace_header_guards_in_dir('')