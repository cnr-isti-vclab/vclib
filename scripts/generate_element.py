import argparse

from generators.common import Element
from generators.el_components import generate_elem_components
from generators.el_concept import generate_elem_concept
from generators.el_container_concept import generate_elem_container_concept
from generators.el_header import generate_elem_header
from generators.per_el_concepts import generate_per_elem_concepts

def main():
    parser = argparse.ArgumentParser(description="Generate a Mesh Element for the VCLib library",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument("elem_name", help="Element Name, should be written using snake case.")
    parser.add_argument('-n', '--namespace', help='Element Namespace.')
    parser.add_argument('-c', '--components', nargs='*', help='Horizontal Components of the element.')
    parser.add_argument('-vc', '--vertical_components', nargs='*', help='Vertical Components of the element.')
    parser.add_argument('-oc', '--optional_components', nargs='*', help='Optional Components of the element.')

    args = parser.parse_args()

    element = Element(args.elem_name, args.namespace, args.components, args.vertical_components, args.optional_components)

    generate_per_elem_concepts(element)
    generate_elem_container_concept(element)
    generate_elem_concept(element)
    generate_elem_header(element)
    generate_elem_components(element)
    

if __name__ == "__main__":
    main()