from . import common

def generate_elem_concept(element):
    # Read in the file
    with open('templates/element_concept.h', 'r') as file :
        element_concept = file.read()

    element_concept = common.replace_header(element_concept)

    element_concept = common.replace_element_strings(element_concept, element)
    
    comp_string = ""
    for c in element.components:
        comp_string += "template<typename T>\n"
        comp_string += "concept Has" + c.name_upper_camel + " = comp::Has" + c.name_upper_camel + "<T>;\n"
        if c.optional:
            comp_string += "template<typename T>\n"
            comp_string += "concept HasOptional" + c.name_upper_camel + " = comp::HasOptional" + c.name_upper_camel + "<T>;\n"

    element_concept = element_concept.replace('%COMPONENTS_CONCEPTS%', comp_string)

    with open("../include/vclib/concepts/mesh/elements/" + element.name + '.h', 'w') as file:
        file.write(element_concept)

    print("Generated Element Concepts: include/vclib/concepts/mesh/elements/" + element.name + '.h')