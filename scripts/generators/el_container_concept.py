from . import common

def generate_elem_container_concept(element):
    target_file = "include/vclib/concepts/mesh/containers/" + element.name + '_container.h'

    # Read in the file
    with open('templates/element_container_concept.h', 'r') as file :
        element_container_concept = file.read()

    element_container_concept = common.replace_header(element_container_concept)

    element_container_concept = common.replace_element_strings(element_container_concept, element)

    with open("../" + target_file, 'w') as file:
        file.write(element_container_concept)

    print("Generated Element Header: " + target_file)
    return [target_file],[]