import argparse

from generators.common import Element
from generators.el_components import generate_elem_components
from generators.el_container import generate_elem_container
from generators.el_header import generate_elem_header
from generators.el_requirements import generate_elem_requirements
from generators.el_view import generate_elem_view
from generators.update_element_type_list import update_element_list

def main():
    parser = argparse.ArgumentParser(description="Generate a Mesh Element for the VCLib library",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument("elem_name", help="Element Name, should be written using snake case.")
    parser.add_argument('-n', '--namespace', help='Element Namespace.')
    parser.add_argument('-c', '--components', nargs='*', help='Horizontal Components of the element.')
    parser.add_argument('-vc', '--vertical_components', nargs='*', help='Vertical Components of the element.')
    parser.add_argument('-oc', '--optional_components', nargs='*', help='Optional Components of the element.')
    parser.add_argument('-ddc', '--dont_define_components', help='Don''t Define Components of the element in its namespace, by default this is done.', action='store_true')

    args = parser.parse_args()

    element = Element(args.elem_name, args.namespace, args.components, args.vertical_components, args.optional_components)

    headers_list = []

    gen_function_list = [
        generate_elem_header,
        generate_elem_components,
        generate_elem_container,
        generate_elem_requirements,
        generate_elem_view
    ]

    update_function_list = [
        update_element_list
    ]

    for f in gen_function_list:
        if f == generate_elem_components:
            if not args.dont_define_components:
                h = f(element)
                headers_list += [h]
        else:
            h = f(element)
            headers_list += [h]

    for f in update_function_list:
        f(element)

    print("Generated Headers:")
    for h in headers_list:
        print('\t' + h)
    

if __name__ == "__main__":
    main()
