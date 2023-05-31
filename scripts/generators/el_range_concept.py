from . import common

def generate_elem_range_concept(element):
    target_file = "include/vclib/concepts/ranges/mesh/" + element.name + '_range.h'
    template_file = "concepts/ranges/mesh/element_range.h"

    return common.simple_string_replacement(target_file, template_file, element)