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

inline void MeshRenderSettings::setSurfaceShadingFlat()
{
	drawMode |=  DRAW_SURF_FLAT;
	drawMode &= ~DRAW_SURF_SMOOTH;
}

inline void MeshRenderSettings::setSurfaceShadingSmooth()
{
	drawMode |=  DRAW_SURF_SMOOTH;
	drawMode &= ~DRAW_SURF_FLAT;
}

inline void MeshRenderSettings::setSurfaceColorPerVertex()
{
	drawMode |=  DRAW_SURF_COLOR_VERTEX;
	drawMode &= ~DRAW_SURF_COLOR_FACE;
}

inline void MeshRenderSettings::setSurfaceColorPerFace()
{
	drawMode |=  DRAW_SURF_COLOR_FACE;
	drawMode &= ~DRAW_SURF_COLOR_VERTEX;
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

inline void MeshRenderSettings::setWireframeColor(float r, float g, float b)
{
	wColor[0] = r;
	wColor[1] = g;
	wColor[2] = b;
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
void MeshRenderSettings::setRenderCapabilityFrom(const MeshType m)
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
