from . import common

def generate_elem_header(element):
    target_file = "include/vclib/mesh/elements/" + element.name + '.h'

    # Read in the file
    with open('templates/element.h', 'r') as file :
        element_header = file.read()

    element_header = common.replace_header(element_header)

    element_header = common.replace_element_strings(element_header, element)

    with open("../" + target_file, 'w') as file:
        file.write(element_header)

    print("Generated Element Header: " + target_file)
    return [target_file],[]
