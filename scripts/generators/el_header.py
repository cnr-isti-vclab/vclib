from . import common

def generate_elem_header(element):
    # Read in the file
    with open('templates/element.h', 'r') as file :
        element_header = file.read()

    element_header = common.replace_header(element_header)

    element_header = common.replace_element_strings(element_header, element)

    with open("../include/vclib/mesh/elements/" + element.name + '.h', 'w') as file:
        file.write(element_header)

    print("Generated Element Header: include/vclib/mesh/elements/" + element.name + '.h')
