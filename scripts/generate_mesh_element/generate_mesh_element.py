#!/usr/bin/env python3
"""
Generates the boilerplate code required to add a new element to the VCLib mesh
(see README.md in this directory).
"""

import argparse

from generators import common
from generators.components import COMPONENTS
from generators.el_container import generate_container
from generators.el_element import generate_element, generate_element_components
from generators.el_registry import update_element_enum, update_traits
from generators.el_requirements import generate_requirements
from generators.el_view import generate_view


def main():
    parser = argparse.ArgumentParser(
        description='Generate a Mesh Element for the VCLib library.',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        'elem_name', help='Element name, written using snake case.')
    parser.add_argument(
        '-n', '--namespace',
        help='Namespace of the element components (vcl::<namespace>). '
        'Default: first four characters of the element name.')
    parser.add_argument(
        '-p', '--plural',
        help='Plural of the element name, written using snake case. '
        'Default: the element name followed by an "s".')
    parser.add_argument(
        '-r', '--required', nargs='*', default=[],
        help='Components that every element must have (bit_flags is always '
        'required).')
    parser.add_argument(
        '-c', '--components', nargs='*', default=[],
        help='Other components that the element can use. The components that '
        'can be optional get both the plain and the Optional aliases. '
        'Supported components: ' + ', '.join(sorted(COMPONENTS)) + '.')
    parser.add_argument(
        '-vn', '--vertex_count', type=int,
        help='Adds the (required) vertex_references component. Number of '
        'vertices of the element (-1 for a dynamic number).')
    parser.add_argument(
        '--mesh_dir', default=str(common.DEFAULT_MESH_DIR),
        help='The vclib/mesh directory of the core module in which the '
        'element is generated and registered.')

    args = parser.parse_args()

    element = common.Element(
        args.elem_name, args.mesh_dir, args.namespace, args.plural,
        args.required, args.components, args.vertex_count)

    headers = [
        generate_element(element),
        generate_element_components(element),
        generate_container(element),
        generate_requirements(element),
        generate_view(element),
    ]
    update_element_enum(element)
    update_traits(element)

    print('Generated headers (relative to ' + str(element.mesh_dir) + '):')
    for h in headers:
        print('\t' + h)
    print('Updated: elements/base/base.h, containers/base/element_container.h, '
          'containers.h, elements.h, views/elements.h, traits.h, '
          'requirements/mesh_concepts.h, requirements/mesh_concepts/base.h, '
          'requirements/contracts.h')


if __name__ == '__main__':
    main()
