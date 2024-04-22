#!/bin/bash

python3 generate_element.py vertex -dc \
    -c bit_flags custom_components vertex_pointers \
    -vc coordinate \
    -oc adjacent_edges adjacent_faces adjacent_vertices color mark normal principal_curvature quality tex_coord \
    -ddc
