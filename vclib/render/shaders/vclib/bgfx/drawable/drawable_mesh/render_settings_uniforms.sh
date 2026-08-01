// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_MESH_RENDER_SETTINGS_UNIFORMS_SH
#define VCL_BGFX_DRAWABLE_DRAWABLE_MESH_RENDER_SETTINGS_UNIFORMS_SH

#include <vclib/bgfx/shaders_common.sh>

uniform vec4 u_mrsDrawPack;
uniform vec4 u_mrsWidthPack;
uniform vec4 u_mrsColorPack[2];

#define u_pointsMode floatBitsToUint(u_mrsDrawPack.x)
#define u_surfaceMode (floatBitsToUint(u_mrsDrawPack.x) >> 16)
#define u_wireframeMode floatBitsToUint(u_mrsDrawPack.y)
#define u_edgesMode (floatBitsToUint(u_mrsDrawPack.y) >> 16)

#define isPerVertexColorAvailable() ((floatBitsToUint(u_mrsDrawPack.z) & 1) != 0)
#define isPerVertexTangentAvailable() ((floatBitsToUint(u_mrsDrawPack.z) & 2) != 0)

// Points
#define isPointsVisible() (bool(u_pointsMode & posToBitFlag(VCL_MRS_DRAW_POINTS)))
#define isPointsShapePixel() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_PIXEL)))
#define isPointsShapeCircle() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_CIRCLE)))
#define isPointsShapeSphere() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_SPHERE)))
#define isPointsShadingNone() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_SHADING_NONE)))
#define isPointsShadingVert() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_SHADING_VERT)))
#define isPointsColorVertex() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_VERTEX)))
#define isPointsColorMesh() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_MESH)))
#define isPointsColorUser() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_USER)))
#define isPointsDrawSelection() (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_DRAW_SELECTION)))

// Surface
#define isSurfaceVisible() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_DRAW_SURF)))
#define isSurfaceShadingNone() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_SHADING_NONE)))
#define isSurfaceShadingFlat() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_SHADING_FLAT)))
#define isSurfaceShadingSmooth() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_SHADING_SMOOTH)))
#define isSurfaceShadingNormalMap() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_SHADING_NORMAL_MAP)))
#define isSurfaceColorVertex() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_COLOR_VERTEX)))
#define isSurfaceColorFace() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_COLOR_FACE)))
#define isSurfaceTexVertex() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_TEX_VERTEX)))
#define isSurfaceTexWedge() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_TEX_WEDGE)))
#define isSurfaceColorMesh() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_COLOR_MESH)))
#define isSurfaceColorUser() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_COLOR_USER)))
#define isSurfaceDrawSelection() (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_DRAW_SELECTION)))

// Wireframe
#define isWireframeVisible() (bool(u_wireframeMode & posToBitFlag(VCL_MRS_DRAW_WIREFRAME)))
#define isWireframeShadingNone() (bool(u_wireframeMode & posToBitFlag(VCL_MRS_WIREFRAME_SHADING_NONE)))
#define isWireframeShadingVert() (bool(u_wireframeMode & posToBitFlag(VCL_MRS_WIREFRAME_SHADING_VERT)))
#define isWireframeColorVert() (bool(u_wireframeMode & posToBitFlag(VCL_MRS_WIREFRAME_COLOR_VERT)))
#define isWireframeColorMesh() (bool(u_wireframeMode & posToBitFlag(VCL_MRS_WIREFRAME_COLOR_MESH)))
#define isWireframeColorUser() (bool(u_wireframeMode & posToBitFlag(VCL_MRS_WIREFRAME_COLOR_USER)))

// Edges
#define isEdgesVisible() (bool(u_edgesMode & posToBitFlag(VCL_MRS_DRAW_EDGES)))
#define isEdgesShadingNone() (bool(u_edgesMode & posToBitFlag(VCL_MRS_EDGES_SHADING_NONE)))
#define isEdgesShadingFlat() (bool(u_edgesMode & posToBitFlag(VCL_MRS_EDGES_SHADING_FLAT)))
#define isEdgesShadingSmooth() (bool(u_edgesMode & posToBitFlag(VCL_MRS_EDGES_SHADING_SMOOTH)))
#define isEdgesColorVertex() (bool(u_edgesMode & posToBitFlag(VCL_MRS_EDGES_COLOR_VERTEX)))
#define isEdgesColorEdge() (bool(u_edgesMode & posToBitFlag(VCL_MRS_EDGES_COLOR_EDGE)))
#define isEdgesColorMesh() (bool(u_edgesMode & posToBitFlag(VCL_MRS_EDGES_COLOR_MESH)))
#define isEdgesColorUser() (bool(u_edgesMode & posToBitFlag(VCL_MRS_EDGES_COLOR_USER)))

#define u_pointWidth u_mrsWidthPack.x
#define u_wireframeWidth u_mrsWidthPack.y
#define u_edgesWidth u_mrsWidthPack.z

#define u_userPointColorFloat u_mrsColorPack[0].x
#define u_userSurfaceColorFloat u_mrsColorPack[0].y
#define u_userWireframeColorFloat u_mrsColorPack[0].z
#define u_userEdgesColorFloat u_mrsColorPack[0].w
#define u_selectionPointColorFloat u_mrsColorPack[1].x
#define u_selectionSurfaceColorFloat u_mrsColorPack[1].y

#define u_selectionPointColor \
    uintABGRToVec4Color(floatBitsToUint(u_selectionPointColorFloat))

#define u_selectionSurfaceColor \
    uintABGRToVec4Color(floatBitsToUint(u_selectionSurfaceColorFloat))

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_MESH_RENDER_SETTINGS_UNIFORMS_SH
