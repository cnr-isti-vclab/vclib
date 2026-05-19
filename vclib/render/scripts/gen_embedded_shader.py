# usage (e.g. to generate the embedded shader files for drawable axis:
# python gen_embedded_shader.py -e DRAWABLE_AXIS -p vclib/shaders/drawable/drawable_axis -v vs_drawable_axis.sc -f fs_drawable_axis.sc
# or, for compute shaders:
# python gen_embedded_shader.py -e DRAWABLE_AXIS -p vclib/shaders/drawable/drawable_axis -c compute_drawable_axis.sc

import argparse

def replace_strings(input_string, enum_name, shader_path=None, vertex_shader_name=None, fragment_shader_name=None, compute_shader_name=None):
    enum_name_lc = enum_name.lower()
    enum_name_uc = enum_name.upper()

    input_string = input_string.replace('%PR_NAME_UC%', enum_name_uc)
    input_string = input_string.replace('%PR_NAME_LC%', enum_name_lc)

    if shader_path is not None:
        input_string = input_string.replace('%PR_PATH%', shader_path)

    if vertex_shader_name is not None:
        input_string = input_string.replace('%PR_VS_NAME%', vertex_shader_name)

    if fragment_shader_name is not None:
        input_string = input_string.replace('%PR_FS_NAME%', fragment_shader_name)

    if compute_shader_name is not None:
        input_string = input_string.replace('%PR_CS_NAME%', compute_shader_name)

    return input_string

def create_c_file(f_input, f_output, enum_name, shader_path, compute_shader_name):
    with open(f_input, 'r') as f:
        input_string = f.read()

    output_string = replace_strings(input_string, enum_name, shader_path=shader_path, compute_shader_name=compute_shader_name)

    with open(f_output, 'w') as f:
        f.write(output_string)
    print(f"File {f_output} created successfully.")

def create_vf_files(f_input, f_output, enum_name, shader_path, vertex_shader_name, fragment_shader_name):
    with open(f_input, 'r') as f:
        input_string = f.read()

    output_string = replace_strings(input_string, enum_name, shader_path=shader_path, vertex_shader_name=vertex_shader_name, fragment_shader_name=fragment_shader_name)

    with open(f_output, 'w') as f:
        f.write(output_string)
    print(f"File {f_output} created successfully.")

def main():
    parser = argparse.ArgumentParser(description="Generate Embedded Shader header and source files.",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument('-e', '--enum', help='Enum name used for the shader, it should be written using upper case.', required=True)
    parser.add_argument('-p', '--path', help='Path to the shader directory, starting from the vclib/bgfx root dir', required=True)
    parser.add_argument('-v', '--vertex', help='Vertex shader filename (e.g. vs_drawable_axis.sc)', required=False)
    parser.add_argument('-f', '--fragment', help='Fragment shader filename (e.g. fs_drawable_axis.sc)', required=False)
    parser.add_argument('-c', '--compute', help='Compute shader filename (e.g. compute_drawable_axis.sc)', required=False)

    args = parser.parse_args()

    enum_name = args.enum
    enum_name_lc = enum_name.lower()
    shader_path = args.path

    # the user should input both vertex and fragment shaders, or just compute shader
    if args.vertex is None and args.fragment is None and args.compute is None:
        print("Please provide either vertex and fragment shaders or a compute shader.")
        return

    if args.compute is None:
        vertex_shader_name = args.vertex.split('/')[-1].split('.')[0]
        fragment_shader_name = args.fragment.split('/')[-1].split('.')[0]

        create_vf_files('templates/embedded_vf_programs/header.h', '../include/vclib/bgfx/programs/embedded_vf_programs/' + enum_name_lc + '.h', enum_name, shader_path, vertex_shader_name, fragment_shader_name)
        create_vf_files('templates/embedded_vf_programs/source.cpp', '../src/vclib/bgfx/programs/embedded_vf_programs/' + enum_name_lc + '.cpp', enum_name, shader_path, vertex_shader_name, fragment_shader_name)

        # Update the embedded_vf_programs.h file to include the new header
        with open('../include/vclib/bgfx/programs/embedded_vf_programs.h', 'r') as f:
            lines = f.readlines()
        for i, line in enumerate(lines):
            if '#include' in line:
                break
        lines.insert(i + 1, '#include "embedded_vf_programs/' + enum_name_lc + '.h"\n')
        with open('../include/vclib/bgfx/programs/embedded_vf_programs.h', 'w') as f:
            f.writelines(lines)
    else:
        compute_shader_name = args.compute.split('/')[-1].split('.')[0]

        create_c_file('templates/embedded_c_programs/header.h', '../include/vclib/bgfx/programs/embedded_c_programs/' + enum_name_lc + '.h', enum_name, shader_path, compute_shader_name)
        create_c_file('templates/embedded_c_programs/source.cpp', '../src/vclib/bgfx/programs/embedded_c_programs/' + enum_name_lc + '.cpp', enum_name, shader_path, compute_shader_name)


        # Update the embedded_c_programs.h file to include the new header
        with open('../include/vclib/bgfx/programs/embedded_c_programs.h', 'r') as f:
            lines = f.readlines()
        for i, line in enumerate(lines):
            if '#include' in line:
                break
        lines.insert(i + 1, '#include "embedded_c_programs/' + enum_name_lc + '.h"\n')
        with open('../include/vclib/bgfx/programs/embedded_c_programs.h', 'w') as f:
            f.writelines(lines)


if __name__ == "__main__":
    main()
