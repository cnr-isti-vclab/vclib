from . import common

def generate_elem_view(element):    
    include_file = "elements/" + element.name + '.h'
    target_file = "include/vclib/mesh/views/" + include_file
    template_file = "mesh/views/element.h"

    common.insert_include_in_file("include/vclib/mesh/views/elements.h", include_file)

    return common.simple_string_replacement(target_file, template_file, element)
