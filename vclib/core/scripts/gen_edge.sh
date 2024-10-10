#!/bin/bash

python3 generate_element.py edge \
    -c bit_flags custom_components vertex_pointers \
    -oc adjacent_edges adjacent_faces color mark quality \
    -ddc
