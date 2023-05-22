import argparse
import string


elem_name = ""
elem_name_upper = ""
elem_name_camel = ""
elem_name_upper_camel = ""
elem_namespace = ""
components = []


class Component:
    def __init__(self, name, vertical = False, optional = False):
        self.name = name
        self.name_upper = name.upper()
        self.name_upper_camel = string.capwords(name, "_").replace("_", "")
        self.name_camel = self.name_upper_camel[0].lower() + self.name_upper_camel[1:]
        self.vertical = vertical
        self.optional = optional


def main():
    global elem_name
    global elem_name_upper
    global elem_name_camel
    global elem_name_upper_camel
    global elem_namespace
    global components

    parser = argparse.ArgumentParser(description="Generate a Mesh Element for the VCLib library",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument("elem_name", help="Element Name, should be written using snake case.")
    parser.add_argument('-n', '--namespace', help='Element Namespace.')
    parser.add_argument('-c', '--components', nargs='*', help='Horizontal Components of the element.')
    parser.add_argument('-vc', '--vertical_components', nargs='*', help='Vertical Components of the element.')
    parser.add_argument('-oc', '--optional_components', nargs='*', help='Optional Components of the element.')

    args = parser.parse_args()

    elem_name = args.elem_name
    elem_name_upper = elem_name.upper()
    elem_name_upper_camel = string.capwords(elem_name, "_").replace("_", "")
    elem_name_camel = elem_name_upper_camel[0].lower() + elem_name_upper_camel[1:]

    print("Snake Case: " + elem_name)
    print("Upper Case: " + elem_name_upper)
    print("Camel Case: " + elem_name_camel)
    print("Upper Camel Case: " + elem_name_upper_camel)

    if (args.namespace is not None):
         elem_namespace = args.namespace
    else:
        elem_namespace = elem_name.replace("_", "")
        elem_namespace = elem_namespace[:4]

    print("Namespace: " + elem_namespace)

    if (args.components is not None):
        for value in args.components:
            if value is not None:
                components.append(Component(value))
    
    if (args.vertical_components is not None):
        for value in args.vertical_components:
            if value is not None:
                components.append(Component(value, True))
    
    if (args.optional_components is not None):
        for value in args.optional_components:
            if value is not None:
                components.append(Component(value, True, True))

    components = sorted(components, key=lambda c: c.name)

    generate_elem_header()
    generate_elem_concept()
    generate_elem_components()


def replace_strings(file_string):
    file_string = file_string.replace('%EL%', elem_name)
    file_string = file_string.replace('%EL_U%', elem_name_upper)
    file_string = file_string.replace('%EL_C%', elem_name_camel)
    file_string = file_string.replace('%EL_UC%', elem_name_upper_camel)
    file_string = file_string.replace('%EL_NS%', elem_namespace)
    return file_string


def generate_elem_header():
    # Read in the file
    with open('templates/element.h', 'r') as file :
        element_header = file.read()

    element_header = replace_strings(element_header)

    with open("../../include/vclib/mesh/elements/" + elem_name + '.h', 'w') as file:
        file.write(element_header)

    print("Generated Element Header: include/vclib/mesh/elements/" + elem_name + '.h')


def generate_elem_concept():
    # Read in the file
    with open('templates/element_concept.h', 'r') as file :
        element_concept = file.read()

    element_concept = replace_strings(element_concept)
    
    comp_string = ""
    for c in components:
        comp_string += "template<typename T>\n"
        comp_string += "concept Has" + c.name_upper_camel + " = comp::Has" + c.name_upper_camel + "<T>;\n"
        if c.optional:
            comp_string += "template<typename T>\n"
            comp_string += "concept HasOptional" + c.name_upper_camel + " = comp::HasOptional" + c.name_upper_camel + "<T>;\n"

    element_concept = element_concept.replace('%COMPONENTS_CONCEPTS%', comp_string)

    with open("../../include/vclib/concepts/mesh/elements/" + elem_name + '.h', 'w') as file:
        file.write(element_concept)

    print("Generated Element Concepts: include/vclib/concepts/mesh/elements/" + elem_name + '.h')


def generate_elem_components():
    # Read in the file
    with open('templates/element_components.h', 'r') as file :
        element_components = file.read()

    element_components = replace_strings(element_components)

    include_comp_string = ""
    comp_string = ""

    for c in components:
        include_comp_string += '#include "../components/' + c.name + '.h"\n'
        comp_string += "/* Port " + c.name_upper_camel + " class into edge namespace */\n"
        comp_string += "using " + c.name_upper_camel + " = comp::" + c.name_upper_camel + "<TODO_CHECK_THIS>;\n\n"
        if c.vertical:
            comp_string += "template<typename " + elem_name_upper_camel + "Type>\n"
            comp_string += "using Vertical" + c.name_upper_camel + " = comp::" + c.name_upper_camel + "<TODO_CHECK_THIS, " + elem_name_upper_camel + "Type>;\n\n"
        if c.optional:
            comp_string += "template<typename " + elem_name_upper_camel + "Type>\n"
            comp_string += "using Optional" + c.name_upper_camel + " = comp::Optional" + c.name_upper_camel + "<TODO_CHECK_THIS, " + elem_name_upper_camel + "Type, true>;\n\n"

    element_components = element_components.replace('%INCLUDE_COMPONENTS%', include_comp_string)

    element_components = element_components.replace('%ELEMENT_COMPONENTS%', comp_string)

    with open("../../include/vclib/mesh/elements/" + elem_name + '_components.h', 'w') as file:
        file.write(element_components)

    print("Generated Element Components: include/vclib/mesh/elements/" + elem_name + '_components.h')

if __name__ == "__main__":
    main()