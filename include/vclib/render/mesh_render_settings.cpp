/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "mesh_render_settings.h"

namespace vcl {

MeshRenderSettings::MeshRenderSettings()
{
}

inline bool MeshRenderSettings::isVisible() const
{
	return drawMode & DRAW_MESH;
}

inline bool MeshRenderSettings::isPointCloudVisible() const
{
	return drawMode & DRAW_POINTS;
}

inline int MeshRenderSettings::pointWidth() const
{
	return pWidth;
}

inline bool MeshRenderSettings::isSurfaceVisible() const
{
	return drawMode & DRAW_SURF;
}

inline bool MeshRenderSettings::isSurfaceShadingFlat() const
{
	return drawMode & DRAW_SURF_FLAT;
}

inline bool MeshRenderSettings::isSurfaceShadingSmooth() const
{
	return drawMode & DRAW_SURF_SMOOTH;
}

inline bool MeshRenderSettings::isSurfaceColorPerFace() const
{
	return drawMode & DRAW_SURF_COLOR_FACE;
}

inline bool MeshRenderSettings::isSurfaceColorPerVertex() const
{
	return drawMode & DRAW_SURF_COLOR_VERTEX;
}

inline bool MeshRenderSettings::isSurfaceColorPerMesh() const
{
	return drawMode & DRAW_SURF_COLOR_MESH;
}

inline bool MeshRenderSettings::isSurfaceColorUserDefined() const
{
	return drawMode & DRAW_SURF_COLOR_USER;
}

inline bool MeshRenderSettings::isSurfaceColorPerVertexTexcoords() const
{
	return drawMode & DRAW_SURF_TEX_VERTEX;
}

inline bool MeshRenderSettings::isSurfaceColorPerWedgeTexcoords() const
{
	return drawMode & DRAW_SURF_TEX_WEDGE;
}

inline bool MeshRenderSettings::isWireframeVisible() const
{
	return drawMode & DRAW_WIREFRAME;
}

inline int MeshRenderSettings::wireframeWidth() const
{
	return wWidth;
}

inline Color MeshRenderSettings::wireframeColor() const
{
	vcl::Color c;
	c.setRedF(wColor[0]);
	c.setGreenF(wColor[1]);
	c.setBlueF(wColor[2]);
	c.setAlphaF(wColor[3]);
	return c;
}

inline const float *MeshRenderSettings::wireframeColorData() const
{
	return wColor;
}

inline bool MeshRenderSettings::isBboxEnabled() const
{
	return drawMode & DRAW_BOUNDINGBOX;
}

inline void MeshRenderSettings::setVisibility(bool b)
{
	if (b) drawMode |=  DRAW_MESH;
	else   drawMode &= ~DRAW_MESH;
}

inline void MeshRenderSettings::setPointCloudVisibility(bool b)
{
	if (b) drawMode |=  DRAW_POINTS;
	else   drawMode &= ~DRAW_POINTS;
}

inline void MeshRenderSettings::setPointWidth(int width)
{
	pWidth = width;
}

inline void MeshRenderSettings::setSurfaceVisibility(bool b)
{
	if (b) drawMode |=  DRAW_SURF;
	else   drawMode &= ~DRAW_SURF;
}

/**
 * @brief Sets the visibility of the surface flat (using triangle normals).
 * Unsets automatically the smooth shading.
 */
inline void MeshRenderSettings::setSurfaceShadingFlat()
{
	drawMode |=  DRAW_SURF_FLAT;
	drawMode &= ~DRAW_SURF_SMOOTH;
}

/**
 * @brief Sets the visibility of the surface smooth (using vertex normals).
 * Unsets automatically the flat shading.
 */
inline void MeshRenderSettings::setSurfaceShadingSmooth()
{
	drawMode |=  DRAW_SURF_SMOOTH;
	drawMode &= ~DRAW_SURF_FLAT;
}

/**
 * @brief Set the surface coloring per vertex (using the vertex colors).
 * Unsets automatically the other possible surface colorizations:
 * - color per face
 * - color per mesh
 * - color user defined
 * - per vertex texture
 * - per wedge texture
 */
inline void MeshRenderSettings::setSurfaceColorPerVertex()
{
	drawMode |=  DRAW_SURF_COLOR_VERTEX;
	drawMode &= ~DRAW_SURF_COLOR_FACE;
	drawMode &= ~DRAW_SURF_COLOR_MESH;
	drawMode &= ~DRAW_SURF_COLOR_USER;
	drawMode &= ~DRAW_SURF_TEX_VERTEX;
	drawMode &= ~DRAW_SURF_TEX_WEDGE;
}

/**
 * @brief Set the surface coloring per face (using the face colors).
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per mesh
 * - color user defined
 * - per vertex texture
 * - per wedge texture
 */
inline void MeshRenderSettings::setSurfaceColorPerFace()
{
	drawMode &= ~DRAW_SURF_COLOR_VERTEX;
	drawMode |=  DRAW_SURF_COLOR_FACE;
	drawMode &= ~DRAW_SURF_COLOR_MESH;
	drawMode &= ~DRAW_SURF_COLOR_USER;
	drawMode &= ~DRAW_SURF_TEX_VERTEX;
	drawMode &= ~DRAW_SURF_TEX_WEDGE;
}

/**
 * @brief Set the surface coloring per mesh (using the mesh color).
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per face
 * - color user defined
 * - per vertex texture
 * - per wedge texture
 */
inline void MeshRenderSettings::setSurfaceColorPerMesh()
{
	drawMode &= ~DRAW_SURF_COLOR_VERTEX;
	drawMode &= ~DRAW_SURF_COLOR_FACE;
	drawMode |=  DRAW_SURF_COLOR_MESH;
	drawMode &= ~DRAW_SURF_COLOR_USER;
	drawMode &= ~DRAW_SURF_TEX_VERTEX;
	drawMode &= ~DRAW_SURF_TEX_WEDGE;
}

/**
 * @brief Set the surface coloring by the user defined color.
 * To set the user defined color, you can use the setSurfaceColorUserDefined() function.
 *
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per face
 * - color per mesh
 * - per vertex texture
 * - per wedge texture
 */
inline void MeshRenderSettings::setSurfaceColorUserDefined()
{
	drawMode &= ~DRAW_SURF_COLOR_VERTEX;
	drawMode &= ~DRAW_SURF_COLOR_FACE;
	drawMode &= ~DRAW_SURF_COLOR_MESH;
	drawMode |=  DRAW_SURF_COLOR_USER;
	drawMode &= ~DRAW_SURF_TEX_VERTEX;
	drawMode &= ~DRAW_SURF_TEX_WEDGE;
}

/**
 * @brief Set the surface texture per vertex texcoords.
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per face
 * - color per mesh
 * - color user defined
 * - per wedge texture
 */
inline void MeshRenderSettings::setSurfaceColorPerVertexTexcoords()
{
	drawMode &= ~DRAW_SURF_COLOR_VERTEX;
	drawMode &= ~DRAW_SURF_COLOR_FACE;
	drawMode &= ~DRAW_SURF_COLOR_MESH;
	drawMode &= ~DRAW_SURF_COLOR_USER;
	drawMode |=  DRAW_SURF_TEX_VERTEX;
	drawMode &= ~DRAW_SURF_TEX_WEDGE;
}

/**
 * @brief Set the surface texture per wedge texcoords.
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per face
 * - color per mesh
 * - color user defined
 * - per vertex texture
 */
inline void MeshRenderSettings::setSurfaceColorPerWedgeTexcoords()
{
	drawMode &= ~DRAW_SURF_COLOR_VERTEX;
	drawMode &= ~DRAW_SURF_COLOR_FACE;
	drawMode &= ~DRAW_SURF_COLOR_MESH;
	drawMode &= ~DRAW_SURF_COLOR_USER;
	drawMode &= ~DRAW_SURF_TEX_VERTEX;
	drawMode |=  DRAW_SURF_TEX_WEDGE;
}

inline void MeshRenderSettings::setSurfaceUserDefinedColor(float r, float g, float b, float a)
{
	sUserColor[0] = r;
	sUserColor[1] = g;
	sUserColor[2] = b;
	sUserColor[3] = a;
}

inline void MeshRenderSettings::setSurfaceUserDefinedColor(const Color &c)
{
	sUserColor[0] = c.redF();
	sUserColor[1] = c.greenF();
	sUserColor[2] = c.blueF();
	sUserColor[3] = c.alphaF();
}

inline void MeshRenderSettings::setWireframeVisibility(bool b)
{
	if (b) drawMode |=  DRAW_WIREFRAME;
	else   drawMode &= ~DRAW_WIREFRAME;
}

inline void MeshRenderSettings::setWireframeWidth(int width)
{
	wWidth = width;
}

inline void MeshRenderSettings::setWireframeColor(float r, float g, float b, float a)
{
	wColor[0] = r;
	wColor[1] = g;
	wColor[2] = b;
	wColor[3] = a;
}

inline void MeshRenderSettings::setWireframeColor(const Color &c)
{
	wColor[0] = c.redF();
	wColor[1] = c.greenF();
	wColor[2] = c.blueF();
	wColor[3] = c.alphaF();
}

inline void MeshRenderSettings::setBoundingBoxVisibility(bool b)
{
	if (b) drawMode |=  DRAW_BOUNDINGBOX;
	else   drawMode &= ~DRAW_BOUNDINGBOX;
}

template<MeshConcept MeshType>
void MeshRenderSettings::setRenderCapabilityFrom(const MeshType& m)
{
	if constexpr (vcl::HasFaces<MeshType>) {

	}
	else {
		drawModeCapability &= ~DRAW_SURF;
		drawModeCapability &= ~DRAW_SURF_FLAT;
		drawModeCapability &= ~DRAW_SURF_SMOOTH;
		drawModeCapability &= ~DRAW_SURF_COLOR_FACE;
		drawModeCapability &= ~DRAW_SURF_COLOR_VERTEX;
		drawModeCapability &= ~DRAW_SURF_COLOR_MESH;
		drawModeCapability &= ~DRAW_SURF_COLOR_USER;
	}
}

} // namespace vcl
