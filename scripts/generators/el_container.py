from . import common

def get_comp_string(element, type):
    with open('templates/mesh/containers/oc_' + type + '.txt', 'r') as file :
        per_comp_string = file.read()

    comp_string = ""
    for c in element.components:
        if c.name == "custom_components":
            continue
        if c.optional:
            comp_string += per_comp_string.replace('%CNUC%', c.name_upper_camel)

    if element.has_custom_components:
        with open('templates/mesh/containers/cc_' + type +'.txt', 'r') as file :
            cc_string = file.read()
        comp_string += cc_string
    
    return comp_string

def generate_elem_container(element):
    include_file = "containers/" + element.name + '_container.h'
    target_file_h = "include/vclib/mesh/" + include_file
    template_file_h = "mesh/containers/element_container.h"

    comp_string = get_comp_string(element, "header")

    # Read in the file
    with open('templates/' + template_file_h, 'r') as file :
        element_container = file.read()

    element_container = element_container.replace('%OPTIONAL_COMPONENTS%', comp_string)

    element_container = common.replace_header_and_element_strings(element_container, element)

    with open("../" + target_file_h, 'w') as file:
        file.write(element_container)

    common.insert_include_in_file("include/vclib/mesh/mesh_containers.h", include_file)

    target_file_cpp = "include/vclib/mesh/containers/" + element.name + '_container.cpp'
    template_file_cpp = "mesh/containers/element_container.cpp"

    comp_string = get_comp_string(element, "source")

    # Read in the file
    with open('templates/' + template_file_cpp, 'r') as file :
        element_container = file.read()

    element_container = element_container.replace('%OPTIONAL_COMPONENTS%', comp_string)

    element_container = common.replace_header_and_element_strings(element_container, element)

    with open("../" + target_file_cpp, 'w') as file:
        file.write(element_container)

    return [target_file_h],[target_file_cpp]