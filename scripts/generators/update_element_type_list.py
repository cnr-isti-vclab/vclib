from . import common

def replace(element_file_string, new_element, start):
    end = '};'

    start_index = element_file_string.index(start)
    end_index = element_file_string.index(end, start_index + len(start))

    element_list = element_file_string[start_index + len(start):(end_index - 1)].split('\n')

    element_list.append('\t' + new_element + ',')

    elememnts_string ='\n'.join(element_list) + '\n'

    element_file_string = element_file_string[:start_index + len(start)] + elememnts_string + element_file_string[end_index:]
    
    return element_file_string

def update_element_list(element):
    target_file = "../include/vclib/types/mesh_elements.h"

    # Read in the file
    with open(target_file, 'r') as file :
        element_file = file.read()

    start = 'enum ElementEnumType {'
    element_file = replace(element_file, element.name_upper, start)

    start = 'ELEMENT_ENUM_STRINGS[ELEMENTS_NUMBER] = {'
    element_file = replace(element_file, '"' + element.name_upper_camel + '"', start)
    
    with open(target_file, 'w') as file:
        file.write(element_file)