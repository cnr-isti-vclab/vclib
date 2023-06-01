from . import common

def generate_elem_container_concept(element):    
    include_file = "containers/" + element.name + '_container.h'
    target_file = "include/vclib/concepts/mesh/" + include_file
    template_file = "concepts/mesh/containers/element_container.h"

    common.insert_include_in_file("include/vclib/concepts/mesh/containers.h", include_file)

    return common.simple_string_replacement(target_file, template_file, element)