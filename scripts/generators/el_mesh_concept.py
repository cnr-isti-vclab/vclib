from . import common

def generate_elem_mesh_concepts(element):
    target_file = "include/vclib/concepts/mesh/" + element.name + '_mesh_concept.h'

    with open('templates/element_mesh_concept.h', 'r') as file :
        element_mesh_concept = file.read()
    
    element_mesh_concept = common.replace_header(element_mesh_concept)

    element_mesh_concept = common.replace_element_strings(element_mesh_concept, element)

    with open("../" + target_file, 'w') as file:
        file.write(element_mesh_concept)
    
    print("Generated Element Header: " + target_file)
    return [target_file],[]