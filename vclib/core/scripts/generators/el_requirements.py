from . import common

def get_per_comp_string(element):
    with open('templates/mesh/requirements/per_comp.txt', 'r') as file :
        per_comp_string = file.read()

    comp_string = ""
    for c in element.components:
        comp_string += per_comp_string.replace('%CNUC%', c.name_upper_camel)

    return comp_string

def get_comp_string(element, functions):
    with open('templates/mesh/requirements/' + functions + '_comp.txt', 'r') as file :
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
    include_file = 'requirements/' + element.name + '_requirements.h'
    target_file_h = "include/vclib/mesh/" + include_file
    template_file_h = "mesh/requirements/element_requirements.h"

    per_comp_string = get_per_comp_string(element)
    is_enable_comp_string = get_comp_string(element, "is_enable")
    require_comp_string = get_comp_string(element, "require")

    # Read in the file
    with open('templates/' + template_file_h, 'r') as file :
        element_requirements = file.read()

    element_requirements = element_requirements.replace('%PER_ELEM_CONCEPTS%', per_comp_string)
    element_requirements = element_requirements.replace('%IS_ENABLE_COMPONENTS%', is_enable_comp_string)
    element_requirements = element_requirements.replace('%REQUIRE_COMPONENTS%', require_comp_string)

    element_requirements = common.replace_header_and_element_strings(element_requirements, element)

    with open("../" + target_file_h, 'w') as file:
        file.write(element_requirements)

    common.insert_include_in_file("include/vclib/mesh/requirements.h", include_file)

    return target_file_h
