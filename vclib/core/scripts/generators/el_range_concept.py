from . import common

def generate_elem_range_concept(element):
    include_file = "ranges/" + element.name + '_range.h'
    target_file = "include/vclib/mesh/concepts/ranges/" + element.name + '_range.h'
    template_file = "mesh/concepts/ranges/element_range.h"

    common.insert_include_in_file("include/vclib/mesh/concepts/ranges.h", include_file)

    return common.simple_string_replacement(target_file, template_file, element)
