#!/bin/bash

python3 generate_element.py vertex -dc \
    -c bit_flags position custom_components vertex_pointers \
    -oc adjacent_edges adjacent_faces adjacent_vertices color mark normal principal_curvature quality tex_coord \
    -ddc
