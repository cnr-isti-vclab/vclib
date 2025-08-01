from . import common
import re

def replace(element_file_string, new_element, start, end):

    start_index = element_file_string.index(start)
    end_index = element_file_string.index(end, start_index + len(start))

    element_list = element_file_string[start_index + len(start):(end_index - 1)].split('\n')

    # if element_list contains ELEMENTS_NUMBER
    if '        ELEMENTS_NUMBER,' in element_list:
        element_list.remove('        ELEMENTS_NUMBER,')
        element_list.append('        ' + new_element + ',')
        element_list.append('        ELEMENTS_NUMBER,')
    else:
        element_list.append('    ' + new_element + ',')

    elememnts_string ='\n'.join(element_list) + '\n'

    element_file_string = element_file_string[:start_index + len(start)] + elememnts_string + element_file_string[end_index:]
    
    return element_file_string

def increment_number_in_string(string):
    regex = r'\d+'

    # Find the first number in the string, using a regular expression
    number = re.findall(regex, string)[0]

    # Increment the number by one
    number = str(int(number) + 1)

    # Replace the first occurance of the number in the string with the new number
    string = re.sub(regex, number, string, count=1)

    return string

def update_element_list(element):
    target_file = "../include/vclib/mesh/elements/base/base.h"

    # Read in the file
    with open(target_file, 'r') as file :
        element_file = file.read()

    start = '    enum Enum : uint {'
    end = '    };'
    element_file = replace(element_file, element.name_upper, start, end)

    start = 'ELEMENT_ENUM_STRINGS[ElemId::ELEMENTS_NUMBER] = {'
    end = '};'
    element_file = replace(element_file, '"' + element.name_upper_camel + '"', start, end)

    with open(target_file, 'w') as file:
        file.write(element_file)
