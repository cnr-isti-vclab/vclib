from . import common

def generate_elem_header(element):
    target_file = "include/vclib/mesh/elements/" + element.name + '.h'
    template_file = "mesh/elements/element.h"

    return common.simple_string_replacement(target_file, template_file, element)
