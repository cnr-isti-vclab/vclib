#!/bin/bash

python3 generate_element.py face \
    -c bit_flags custom_components polygon_bit_flags triangle_bit_flags vertex_pointers \
    -oc adjacent_edges adjacent_faces color mark normal principal_curvature quality wedge_colors wedge_tex_coords \
    -ddc
