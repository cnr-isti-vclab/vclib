# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-core-tests)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

set(SOURCES
    add_elements.cpp
    adjacencies.cpp
    append.cpp
    append_buffer.cpp
    clean_algorithms.cpp
    concepts.cpp
    const_correctness.cpp
    container_proxy.cpp
    create_algorithms.cpp
    cube_poly_32.cpp
    cube_poly_64.cpp
    cube_poly_on_trimesh.cpp
    custom_components.cpp
    delete_algorithms.cpp
    deletion_compaction.cpp
    edges.cpp
    elem_algorithms.cpp
    export_adjacencies.cpp
    export_colors_quality.cpp
    export_geometry.cpp
    export_normals_selection.cpp
    export_sentinels_bimap.cpp
    grid_query.cpp
    import_attributes.cpp
    import_colors.cpp
    import_errors_sentinels.cpp
    import_from.cpp
    import_geometry.cpp
    import_texcoords.cpp
    kd_tree.cpp
    lifetime.cpp
    load_save_mesh_obj.cpp
    load_save_mesh_off.cpp
    load_save_mesh_ply.cpp
    load_save_mesh_stl.cpp
    mesh_clean.cpp
    mesh_convex_hull.cpp
    mesh_copy_and_append.cpp
    mesh_crease_edges.cpp
    mesh_custom_components.cpp
    mesh_filter.cpp
    mesh_info.cpp
    mesh_misc_serialization.cpp
    mesh_pos.cpp
    mesh_provider.cpp
    mesh_update_normal.cpp
    navigation.cpp
    optional_components.cpp
    polygon.cpp
    polymesh_base.cpp
    polymesh_poly_topology.cpp
    polymesh_tri_topology.cpp
    presets.cpp
    random.cpp
    serialization.cpp
    shuffle.cpp
    space_complex.cpp
    space_core.cpp
    stat_quality.cpp
    static_asserts.cpp
    texture_double_32.cpp
    texture_double_64.cpp
    triedgemesh_topology.cpp
    trimesh_base.cpp
    trimesh_topology.cpp
    update_color.cpp
    update_normals.cpp
    update_quality.cpp
    update_selection.cpp
    vertex_references.cpp
    views.cpp
    wedge.cpp
    wedge_vertex_duplication.cpp
)

if(TARGET vclib-3rd-tinygltf)
    list(APPEND SOURCES load_mesh_gltf.cpp)
endif()

vclib_add_test(
    core
    SOURCES ${SOURCES}
    ${HEADER_ONLY_OPTION}
)

set_target_properties(
    vclib-core-test
    PROPERTIES UNITY_BUILD ON UNITY_BUILD_MODE BATCH UNITY_BUILD_BATCH_SIZE 8
)
