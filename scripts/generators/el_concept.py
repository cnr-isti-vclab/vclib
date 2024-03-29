from . import common

def generate_elem_concept(element):
    target_file = "include/vclib/concepts/mesh/elements/" + element.name + '.h'

    # Read in the file
    with open('templates/concepts/mesh/elements/element.h', 'r') as file :
        element_concept = file.read()

    element_concept = common.replace_header_and_element_strings(element_concept, element)
    
    comp_string = ""
    for c in element.components:
        comp_string += "template<typename T>\n"
        comp_string += "concept Has" + c.name_upper_camel + " = comp::Has" + c.name_upper_camel + "<T>;\n"
        if c.optional:
            comp_string += "template<typename T>\n"
            comp_string += "concept HasOptional" + c.name_upper_camel + " = comp::HasOptional" + c.name_upper_camel + "<T>;\n"

    element_concept = element_concept.replace('%COMPONENTS_CONCEPTS%', comp_string)

    with open("../" + target_file, 'w') as file:
        file.write(element_concept)

    return target_file