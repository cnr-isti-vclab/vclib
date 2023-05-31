from . import common

def get_comp_string(element, functions, type):
    with open('templates/mesh/requirements/' + functions + '_comp_' + type + '.txt', 'r') as file :
        per_comp_string = file.read()

    comp_string = ""
    for c in element.components:
        if c.name == "custom_components":
            continue
        if c.optional:
            string = per_comp_string.replace('%CNUC%', c.name_upper_camel)
            comp_string += string.replace('%CNU%', c.name_upper)
    
    return comp_string

def generate_elem_requirements(element):
    target_file_h = "include/vclib/mesh/requirements/" + element.name + '_requirements.h'
    template_file_h = "mesh/requirements/element_requirements.h"

    is_enable_comp_string = get_comp_string(element, "is_enable", "header")
    require_comp_string = get_comp_string(element, "require", "header")

    # Read in the file
    with open('templates/' + template_file_h, 'r') as file :
        element_requirements = file.read()

    element_requirements = element_requirements.replace('%IS_ENABLE_COMPONENTS_HEADER%', is_enable_comp_string)
    element_requirements = element_requirements.replace('%REQUIRE_COMPONENTS_HEADER%', require_comp_string)

    element_requirements = common.replace_header_and_element_strings(element_requirements, element)

    with open("../" + target_file_h, 'w') as file:
        file.write(element_requirements)

    print("Generated Element Container Header: " + target_file_h)

    target_file_cpp = "include/vclib/mesh/requirements/" + element.name + '_requirements.cpp'
    template_file_cpp = "mesh/requirements/element_requirements.cpp"

    is_enable_comp_string = get_comp_string(element, "is_enable", "source")
    require_comp_string = get_comp_string(element, "require", "source")

    # Read in the file
    with open('templates/' + template_file_cpp, 'r') as file :
        element_requirements = file.read()

    element_requirements = element_requirements.replace('%IS_ENABLE_COMPONENTS_SOURCE%', is_enable_comp_string)
    element_requirements = element_requirements.replace('%REQUIRE_COMPONENTS_SOURCE%', require_comp_string)

    element_requirements = common.replace_header_and_element_strings(element_requirements, element)

    with open("../" + target_file_cpp, 'w') as file:
        file.write(element_requirements)

    print("Generated Element Container Source: " + target_file_cpp)

    return [target_file_h],[target_file_cpp]