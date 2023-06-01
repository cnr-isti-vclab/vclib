from . import common

def generate_elem_view(element):    
    include_file = "elements/" + element.name + '.h'
    target_file = "include/vclib/views/mesh/" + include_file
    template_file = "views/mesh/element.h"

    common.insert_include_in_file("include/vclib/views/mesh/elements.h", include_file)

    return common.simple_string_replacement(target_file, template_file, element)