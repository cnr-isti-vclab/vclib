import string

class Component:
    def __init__(self, name, vertical = False, optional = False):
        self.name = name
        self.name_upper = name.upper()
        self.name_upper_camel = string.capwords(name, "_").replace("_", "")
        self.name_camel = self.name_upper_camel[0].lower() + self.name_upper_camel[1:]
        self.vertical = vertical
        self.optional = optional

class Element:
    def __init__(self, name, namespace = None, components = None, vertical_components = None, optional_components = None):
        self.name = name
        self.name_upper = name.upper()
        self.name_upper_camel = string.capwords(name, "_").replace("_", "")
        self.name_camel = self.name_upper_camel[0].lower() + self.name_upper_camel[1:]
        self.components = []
        if (namespace is not None):
            self.namespace = namespace
        else:
            self.namespace = self.name.replace("_", "")
            self.namespace = self.namespace[:4]
        
        if (components is not None):
            for value in components:
                if value is not None:
                    self.components.append(Component(value))
    
        if (vertical_components is not None):
            for value in vertical_components:
                if value is not None:
                    self.components.append(Component(value, True))
    
        if (optional_components is not None):
            for value in optional_components:
                if value is not None:
                    self.components.append(Component(value, True, True))

        self.components = sorted(self.components, key=lambda c: c.name)

def replace_element_strings(file_string, element):
    file_string = file_string.replace('%EL%', element.name)
    file_string = file_string.replace('%EL_U%', element.name_upper)
    file_string = file_string.replace('%EL_C%', element.name_camel)
    file_string = file_string.replace('%EL_UC%', element.name_upper_camel)
    file_string = file_string.replace('%EL_NS%', element.namespace)
    return file_string

def replace_header(file_string):
    # Read in the file
    with open('templates/header.txt', 'r') as file :
        header = file.read()

    file_string = file_string.replace('%HEADER%', header)
    return file_string

def replace_header_and_element_strings(file_string, element):
    file_string = replace_header(file_string)
    file_string = replace_element_strings(file_string, element)
    return file_string

def simple_string_replacement(target_file_name, template_file_name, element):
    # Read in the file
    with open('templates/' + template_file_name, 'r') as file :
        file_string = file.read()

    file_string = replace_header_and_element_strings(file_string, element)

    with open("../" + target_file_name, 'w') as file:
        file.write(file_string)

    print("Generated Element Header: " + target_file_name)
    return [target_file_name],[]