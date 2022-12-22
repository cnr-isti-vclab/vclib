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

inline MeshRenderSettings::MeshRenderSettings()
{
}

template<MeshConcept MeshType>
MeshRenderSettings::MeshRenderSettings(const MeshType &m)
{
	setRenderCapabilityFrom(m);
}

inline bool MeshRenderSettings::canBeVisible() const
{
	return drawModeCapability & DRAW_MESH;
}

inline bool MeshRenderSettings::canPointCloudBeVisible() const
{
	return drawModeCapability & DRAW_POINTS;
}

inline bool MeshRenderSettings::canPointCloudBeColoredPerVertex() const
{
	return drawModeCapability & DRAW_POINTS_COLOR_VERTEX;
}

inline bool MeshRenderSettings::canPointCloudBeColoredPerMesh() const
{
	return drawModeCapability & DRAW_POINTS_COLOR_MESH;
}

inline bool MeshRenderSettings::canSurfaceBeVisible() const
{
	return drawModeCapability & DRAW_SURF;
}

inline bool MeshRenderSettings::canSurfaceBeSmooth() const
{
	return drawModeCapability & DRAW_SURF_SMOOTH;
}

inline bool MeshRenderSettings::canSurfaceBeColoredPerFace() const
{
	return drawModeCapability & DRAW_SURF_COLOR_FACE;
}

inline bool MeshRenderSettings::canSurfaceBeColoredPerVertex() const
{
	return drawModeCapability & DRAW_SURF_COLOR_VERTEX;
}

inline bool MeshRenderSettings::canSurfaceBeColoredPerMesh() const
{
	return drawModeCapability & DRAW_SURF_COLOR_MESH;
}

inline bool MeshRenderSettings::canSurfaceBeColoredPerVertexTexcoords() const
{
	return drawModeCapability & DRAW_SURF_TEX_VERTEX;
}

inline bool MeshRenderSettings::canSurfaceBeColoredPerWedgeTexcoords() const
{
	return drawModeCapability & DRAW_SURF_TEX_WEDGE;
}

inline bool MeshRenderSettings::canWireframeBeColoredPerMesh() const
{
	return drawModeCapability & DRAW_WIREFRAME_COLOR_MESH;
}

inline bool MeshRenderSettings::canBoundingBoxBeVisible() const
{
	return drawModeCapability & DRAW_BOUNDINGBOX;
}

inline bool MeshRenderSettings::isVisible() const
{
	return drawMode & DRAW_MESH;
}

inline bool MeshRenderSettings::isPointCloudVisible() const
{
	return drawMode & DRAW_POINTS;
}

inline bool MeshRenderSettings::isPointCloudColorPerVertex() const
{
	return drawMode & DRAW_POINTS_COLOR_VERTEX;
}

inline bool MeshRenderSettings::isPointCloudColorPerMesh() const
{
	return drawMode & DRAW_POINTS_COLOR_MESH;
}

inline bool MeshRenderSettings::isPointCloudColorUserDefined() const
{
	return drawMode & DRAW_POINTS_COLOR_USER;
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

inline bool MeshRenderSettings::setVisibility(bool b)
{
	if (canBeVisible()) {
		if (b) drawMode |=  DRAW_MESH;
		else   drawMode &= ~DRAW_MESH;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setPointCloudVisibility(bool b)
{
	if (canPointCloudBeVisible()) {
		if (b) drawMode |=  DRAW_POINTS;
		else   drawMode &= ~DRAW_POINTS;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setPointCloudColorPerVertex()
{
	if (canSurfaceBeColoredPerVertex()) {
		drawMode |=  DRAW_POINTS_COLOR_VERTEX;
		drawMode &= ~DRAW_POINTS_COLOR_MESH;
		drawMode &= ~DRAW_POINTS_COLOR_USER;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setPointCloudColorPerMesh()
{
	if (canSurfaceBeColoredPerMesh()) {
		drawMode &= ~DRAW_POINTS_COLOR_VERTEX;
		drawMode |=  DRAW_POINTS_COLOR_MESH;
		drawMode &= ~DRAW_POINTS_COLOR_USER;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setPointCloudColorUserDefined()
{
	if (canPointCloudBeVisible()) {
		drawMode &= ~DRAW_POINTS_COLOR_VERTEX;
		drawMode &= ~DRAW_POINTS_COLOR_MESH;
		drawMode |=  DRAW_POINTS_COLOR_USER;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setPointWidth(int width)
{
	if (canPointCloudBeVisible()) {
		pWidth = width;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setPointCloudUserDefinedColor(float r, float g, float b, float a)
{
	if (canPointCloudBeVisible()) {
		pUserColor[0] = r;
		pUserColor[1] = g;
		pUserColor[2] = b;
		pUserColor[3] = a;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setPointCloudUserDefinedColor(const Color &c)
{
	if (canPointCloudBeVisible()) {
		pUserColor[0] = c.redF();
		pUserColor[1] = c.greenF();
		pUserColor[2] = c.blueF();
		pUserColor[3] = c.alphaF();
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setSurfaceVisibility(bool b)
{
	if (canSurfaceBeVisible()) {
		if (b) drawMode |=  DRAW_SURF;
		else   drawMode &= ~DRAW_SURF;
		return true;
	}
	else {
		return false;
	}
}

/**
 * @brief Sets the visibility of the surface flat (using triangle normals).
 * Unsets automatically the smooth shading.
 */
inline bool MeshRenderSettings::setSurfaceShadingFlat()
{
	if (canSurfaceBeVisible()) {
		drawMode |=  DRAW_SURF_FLAT;
		drawMode &= ~DRAW_SURF_SMOOTH;
		return true;
	}
	else {
		return false;
	}
}

/**
 * @brief Sets the visibility of the surface smooth (using vertex normals).
 * Unsets automatically the flat shading.
 */
inline bool MeshRenderSettings::setSurfaceShadingSmooth()
{
	if (canSurfaceBeSmooth()) {
		drawMode |=  DRAW_SURF_SMOOTH;
		drawMode &= ~DRAW_SURF_FLAT;
		return true;
	}
	else {
		return false;
	}
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
inline bool MeshRenderSettings::setSurfaceColorPerVertex()
{
	if (canSurfaceBeColoredPerVertex()) {
		drawMode |=  DRAW_SURF_COLOR_VERTEX;
		drawMode &= ~DRAW_SURF_COLOR_FACE;
		drawMode &= ~DRAW_SURF_COLOR_MESH;
		drawMode &= ~DRAW_SURF_COLOR_USER;
		drawMode &= ~DRAW_SURF_TEX_VERTEX;
		drawMode &= ~DRAW_SURF_TEX_WEDGE;
		return true;
	}
	else {
		return false;
	}
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
inline bool MeshRenderSettings::setSurfaceColorPerFace()
{
	if (canSurfaceBeColoredPerFace()) {
		drawMode &= ~DRAW_SURF_COLOR_VERTEX;
		drawMode |=  DRAW_SURF_COLOR_FACE;
		drawMode &= ~DRAW_SURF_COLOR_MESH;
		drawMode &= ~DRAW_SURF_COLOR_USER;
		drawMode &= ~DRAW_SURF_TEX_VERTEX;
		drawMode &= ~DRAW_SURF_TEX_WEDGE;
		return true;
	}
	else {
		return false;
	}
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
inline bool MeshRenderSettings::setSurfaceColorPerMesh()
{
	if (canSurfaceBeColoredPerMesh()) {
		drawMode &= ~DRAW_SURF_COLOR_VERTEX;
		drawMode &= ~DRAW_SURF_COLOR_FACE;
		drawMode |=  DRAW_SURF_COLOR_MESH;
		drawMode &= ~DRAW_SURF_COLOR_USER;
		drawMode &= ~DRAW_SURF_TEX_VERTEX;
		drawMode &= ~DRAW_SURF_TEX_WEDGE;
		return true;
	}
	else {
		return false;
	}
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
inline bool MeshRenderSettings::setSurfaceColorUserDefined()
{
	if (canSurfaceBeVisible()) {
		drawMode &= ~DRAW_SURF_COLOR_VERTEX;
		drawMode &= ~DRAW_SURF_COLOR_FACE;
		drawMode &= ~DRAW_SURF_COLOR_MESH;
		drawMode |=  DRAW_SURF_COLOR_USER;
		drawMode &= ~DRAW_SURF_TEX_VERTEX;
		drawMode &= ~DRAW_SURF_TEX_WEDGE;
		return true;
	}
	else {
		return false;
	}
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
inline bool MeshRenderSettings::setSurfaceColorPerVertexTexcoords()
{
	if (canSurfaceBeColoredPerVertexTexcoords()) {
		drawMode &= ~DRAW_SURF_COLOR_VERTEX;
		drawMode &= ~DRAW_SURF_COLOR_FACE;
		drawMode &= ~DRAW_SURF_COLOR_MESH;
		drawMode &= ~DRAW_SURF_COLOR_USER;
		drawMode |=  DRAW_SURF_TEX_VERTEX;
		drawMode &= ~DRAW_SURF_TEX_WEDGE;
		return true;
	}
	else {
		return false;
	}
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
inline bool MeshRenderSettings::setSurfaceColorPerWedgeTexcoords()
{
	if (canSurfaceBeColoredPerWedgeTexcoords()) {
		drawMode &= ~DRAW_SURF_COLOR_VERTEX;
		drawMode &= ~DRAW_SURF_COLOR_FACE;
		drawMode &= ~DRAW_SURF_COLOR_MESH;
		drawMode &= ~DRAW_SURF_COLOR_USER;
		drawMode &= ~DRAW_SURF_TEX_VERTEX;
		drawMode |=  DRAW_SURF_TEX_WEDGE;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setSurfaceUserDefinedColor(float r, float g, float b, float a)
{
	if (canSurfaceBeVisible()) {
		sUserColor[0] = r;
		sUserColor[1] = g;
		sUserColor[2] = b;
		sUserColor[3] = a;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setSurfaceUserDefinedColor(const Color &c)
{
	if (canSurfaceBeVisible()) {
		sUserColor[0] = c.redF();
		sUserColor[1] = c.greenF();
		sUserColor[2] = c.blueF();
		sUserColor[3] = c.alphaF();
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setWireframeVisibility(bool b)
{
	if (canSurfaceBeVisible()) {
		if (b) drawMode |=  DRAW_WIREFRAME;
		else   drawMode &= ~DRAW_WIREFRAME;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setWireframeWidth(int width)
{
	if (canSurfaceBeVisible()) {
		wWidth = width;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setWireframeColor(float r, float g, float b, float a)
{
	if (canSurfaceBeVisible()) {
		wColor[0] = r;
		wColor[1] = g;
		wColor[2] = b;
		wColor[3] = a;
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setWireframeColor(const Color &c)
{
	if (canSurfaceBeVisible()) {
		wColor[0] = c.redF();
		wColor[1] = c.greenF();
		wColor[2] = c.blueF();
		wColor[3] = c.alphaF();
		return true;
	}
	else {
		return false;
	}
}

inline bool MeshRenderSettings::setBoundingBoxVisibility(bool b)
{
	if (canBoundingBoxBeVisible()) {
		if (b) drawMode |=  DRAW_BOUNDINGBOX;
		else   drawMode &= ~DRAW_BOUNDINGBOX;
		return true;
	}
	else {
		return false;
	}
}

template<MeshConcept MeshType>
void MeshRenderSettings::setRenderCapabilityFrom(const MeshType& m)
{
	drawModeCapability = 0;

	if (m.vertexNumber() > 0) {
		drawModeCapability |= DRAW_MESH;

		// -- Points --
		drawModeCapability |= DRAW_POINTS;
		drawModeCapability |= DRAW_POINTS_SQUARE;
		drawModeCapability |= DRAW_POINTS_CIRCLE;
		drawModeCapability |= DRAW_POINTS_COLOR_USER;

		if constexpr (vcl::HasPerVertexColor<MeshType>) {
			if (vcl::isPerVertexColorEnabled(m)) {
				drawModeCapability |= DRAW_POINTS_COLOR_VERTEX;
			}
		}

		if constexpr (vcl::HasColor<MeshType>) {
			drawModeCapability |= DRAW_POINTS_COLOR_MESH;
		}

		// -- Surface and Wireframe --
		if constexpr (vcl::HasFaces<MeshType>) {
			if (m.faceNumber() > 0) {
				drawModeCapability |= DRAW_SURF;
				drawModeCapability |= DRAW_SURF_FLAT;
				drawModeCapability |= DRAW_SURF_COLOR_USER;
				drawModeCapability |= DRAW_WIREFRAME;
				drawModeCapability |= DRAW_WIREFRAME_COLOR_USER;

				if constexpr (vcl::HasColor<MeshType>) {
					drawModeCapability |= DRAW_SURF_COLOR_MESH;
					drawModeCapability |= DRAW_WIREFRAME_COLOR_MESH;
				}

				if constexpr (vcl::HasPerVertexNormal<MeshType>) {
					if (vcl::isPerVertexNormalEnabled(m))
						drawModeCapability |= DRAW_SURF_SMOOTH;
				}

				if constexpr (vcl::HasPerFaceColor<MeshType>) {
					if (vcl::isPerFaceColorEnabled(m))
						drawModeCapability |= DRAW_SURF_COLOR_FACE;
				}

				if constexpr (vcl::HasPerVertexColor<MeshType>) {
					if (vcl::isPerVertexColorEnabled(m)) {
						drawModeCapability |= DRAW_SURF_COLOR_VERTEX;
					}
				}

				if constexpr (vcl::HasTexturePaths<MeshType>) {
					if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
						if (vcl::isPerVertexTexCoordEnabled(m))
							drawModeCapability |= DRAW_SURF_TEX_VERTEX;
					}

					if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
						if (vcl::isPerFaceWedgeTexCoordsEnabled(m))
							drawModeCapability |= DRAW_SURF_TEX_WEDGE;
					}
				}
			}
		}

		// -- Bounding Box --
		if constexpr (vcl::HasBoundingBox<MeshType>) {
			drawModeCapability |= DRAW_BOUNDINGBOX;
		}
	}

	// make sure that the previous draw mode satisfies the new capabilites
	drawMode &= drawModeCapability;
}

} // namespace vcl
