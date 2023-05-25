from . import common

def generate_elem_container_concept(element):
    target_file = "include/vclib/concepts/mesh/containers/" + element.name + '_container.h'
    template_file = "element_container_concept.h"

    return common.simple_string_replacement(target_file, template_file, element)