from . import common

def generate_elem_components(element):
    target_file = "include/vclib/mesh/elements/" + element.name + '_components.h'

    # Read in the file
    with open('templates/mesh/elements/element_components.h', 'r') as file :
        element_components = file.read()

    element_components = common.replace_header_and_element_strings(element_components, element)

    include_comp_string = ""
    comp_string = ""

    for c in element.components:
        include_comp_string += '#include "../components/' + c.name + '.h"\n'
        comp_string += "/* Port " + c.name_upper_camel + " class into edge namespace */\n"
        comp_string += "using " + c.name_upper_camel + " = comp::" + c.name_upper_camel + "<TODO_CHECK_THIS>;\n\n"
        if c.vertical:
            comp_string += "template<typename " + element.name_upper_camel + "Type>\n"
            comp_string += "using Vertical" + c.name_upper_camel + " = comp::" + c.name_upper_camel + "<TODO_CHECK_THIS, " + element.name_upper_camel + "Type>;\n\n"
        if c.optional:
            comp_string += "template<typename " + element.name_upper_camel + "Type>\n"
            comp_string += "using Optional" + c.name_upper_camel + " = comp::Optional" + c.name_upper_camel + "<TODO_CHECK_THIS, " + element.name_upper_camel + "Type, true>;\n\n"

    element_components = element_components.replace('%INCLUDE_COMPONENTS%', include_comp_string)

    element_components = element_components.replace('%ELEMENT_COMPONENTS%', comp_string)

    with open("../" + target_file, 'w') as file:
        file.write(element_components)

    return [target_file],[]