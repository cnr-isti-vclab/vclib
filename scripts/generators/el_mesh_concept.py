from . import common

def generate_elem_mesh_concepts(element):
    include_file = "mesh/" + element.name + '_mesh_concept.h'
    target_file = "include/vclib/concepts/" + include_file
    template_file = "concepts/mesh/element_mesh_concept.h"

    common.insert_include_in_file("include/vclib/concepts/mesh.h", include_file)

    return common.simple_string_replacement(target_file, template_file, element)