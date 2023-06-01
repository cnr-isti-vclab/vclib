from . import common

def generate_per_elem_concepts(element):
    include_file = "mesh/per_" + element.name + '.h'
    target_file = "include/vclib/concepts/" + include_file
    template_file = "concepts/mesh/per_element.h"

    with open('templates/concepts/mesh/per_comp.txt', 'r') as file :
        per_comp_string = file.read()
    
    comp_string = ""
    for c in element.components:
        comp_string += per_comp_string.replace('%CNUC%', c.name_upper_camel)

    with open('templates/' + template_file, 'r') as file :
        per_element_concepts = file.read()

    per_element_concepts = per_element_concepts.replace('%PER_ELEM_CONCEPTS%', comp_string)

    per_element_concepts = common.replace_header_and_element_strings(per_element_concepts, element)

    with open("../" + target_file, 'w') as file:
        file.write(per_element_concepts)

    return [target_file],[]
