from . import common

def generate_elem_mesh_concepts(element):
    target_file = "include/vclib/concepts/mesh/" + element.name + '_mesh_concept.h'
    template_file = "element_mesh_concept.h"

    return common.simple_string_replacement(target_file, template_file, element)