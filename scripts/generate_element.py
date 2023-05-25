import argparse

from generators.common import Element
from generators.el_components import generate_elem_components
from generators.el_concept import generate_elem_concept
from generators.el_container_concept import generate_elem_container_concept
from generators.el_header import generate_elem_header
from generators.el_mesh_concept import generate_elem_mesh_concepts
from generators.per_el_concepts import generate_per_elem_concepts
from generators.update_cmake import update_cmake_file
from generators.update_element_type_list import update_element_list

def main():
    parser = argparse.ArgumentParser(description="Generate a Mesh Element for the VCLib library",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument("elem_name", help="Element Name, should be written using snake case.")
    parser.add_argument('-n', '--namespace', help='Element Namespace.')
    parser.add_argument('-c', '--components', nargs='*', help='Horizontal Components of the element.')
    parser.add_argument('-vc', '--vertical_components', nargs='*', help='Vertical Components of the element.')
    parser.add_argument('-oc', '--optional_components', nargs='*', help='Optional Components of the element.')
    parser.add_argument('-dc', '--define_components', help='Define Components of the element in its namespace, by default this is not done.', action='store_true')

    args = parser.parse_args()

    element = Element(args.elem_name, args.namespace, args.components, args.vertical_components, args.optional_components)

    headers_list = []
    sources_list = []

    h, s = generate_per_elem_concepts(element)
    headers_list += h
    sources_list += s
    h, s = generate_elem_mesh_concepts(element)
    headers_list += h
    sources_list += s
    h, s = generate_elem_container_concept(element)
    headers_list += h
    sources_list += s
    h, s = generate_elem_concept(element)
    headers_list += h
    sources_list += s
    h, s = generate_elem_header(element)
    headers_list += h
    sources_list += s
    if args.define_components:
        h, s = generate_elem_components(element)
        headers_list += h
        sources_list += s
        
    update_cmake_file(headers_list, sources_list)
    update_element_list(element)
    

if __name__ == "__main__":
    main()