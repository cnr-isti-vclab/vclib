/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_BGFX_PROGRAMS_EMBEDDED_VF_PROGRAMS_H
#define VCL_BGFX_PROGRAMS_EMBEDDED_VF_PROGRAMS_H

#include "embedded_vf_programs/drawable_axis.h"
#include "embedded_vf_programs/drawable_directional_light.h"
#include "embedded_vf_programs/drawable_mesh_edges.h"
#include "embedded_vf_programs/drawable_mesh_edges_id.h"
#include "embedded_vf_programs/drawable_mesh_points.h"
#include "embedded_vf_programs/drawable_mesh_points_id.h"
// #include "embedded_vf_programs/drawable_mesh_surface.h"
#include "embedded_vf_programs/drawable_mesh_surface_id.h"

#include "embedded_vf_programs/drawable_mesh_wireframe_id.h"

#include "embedded_vf_programs/drawable_mesh_surface_flat_color_face.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_color_face_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_color_mesh.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_color_mesh_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_color_user.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_color_user_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_color_vertex.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_color_vertex_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_tex_vertex.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_tex_vertex_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_tex_wedge.h"
#include "embedded_vf_programs/drawable_mesh_surface_flat_tex_wedge_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_color_face.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_color_face_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_color_mesh.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_color_mesh_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_color_user.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_color_user_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_color_vertex.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_color_vertex_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_tex_vertex.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_tex_vertex_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_tex_wedge.h"
#include "embedded_vf_programs/drawable_mesh_surface_none_tex_wedge_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_color_face.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_color_face_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_color_mesh.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_color_mesh_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_color_user.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_color_user_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_color_vertex.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_color_vertex_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_tex_vertex.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_tex_vertex_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_tex_wedge.h"
#include "embedded_vf_programs/drawable_mesh_surface_smooth_tex_wedge_si.h"
#include "embedded_vf_programs/drawable_mesh_surface_uber.h"

#include "embedded_vf_programs/drawable_trackball.h"
#include "embedded_vf_programs/font_basic.h"
#include "embedded_vf_programs/font_distance_field.h"
#include "embedded_vf_programs/font_distance_field_drop_shadow.h"
#include "embedded_vf_programs/font_distance_field_drop_shadow_image.h"
#include "embedded_vf_programs/font_distance_field_outline.h"
#include "embedded_vf_programs/font_distance_field_outline_drop_shadow_image.h"
#include "embedded_vf_programs/font_distance_field_outline_image.h"
#include "embedded_vf_programs/font_distance_field_subpixel.h"
#include "embedded_vf_programs/lines.h"
#include "embedded_vf_programs/lines_instancing.h"
#include "embedded_vf_programs/lines_texture.h"
#include "embedded_vf_programs/polylines.h"
#include "embedded_vf_programs/polylines_indirect.h"
#include "embedded_vf_programs/polylines_indirect_joints.h"
#include "embedded_vf_programs/polylines_instancing.h"
#include "embedded_vf_programs/polylines_instancing_joints.h"
#include "embedded_vf_programs/polylines_texture.h"
#include "embedded_vf_programs/polylines_texture_joints.h"

#endif // VCL_BGFX_PROGRAMS_EMBEDDED_VF_PROGRAMS_H
