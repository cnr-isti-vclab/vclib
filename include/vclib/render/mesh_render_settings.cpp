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
	drawMode          = DRAW_SMOOTH | DRAW_FACECOLOR;
	wWidth    = 1;
	wColor[0] = (float)0.1;
	wColor[1] = (float)0.1;
	wColor[2] = (float)0.1;
	pWidth = 3;
}

inline bool MeshRenderSettings::isWireframeEnabled() const
{
	return drawMode & DRAW_WIREFRAME;
}

inline bool MeshRenderSettings::isPointShadingEnabled() const
{
	return drawMode & DRAW_POINTS;
}

inline bool MeshRenderSettings::isFlatShadingEnabled() const
{
	return drawMode & DRAW_FLAT;
}

inline bool MeshRenderSettings::isSmoothShadingEnabled() const
{
	return drawMode & DRAW_SMOOTH;
}

inline bool MeshRenderSettings::isBboxEnabled() const
{
	return drawMode & DRAW_BOUNDINGBOX;
}

inline bool MeshRenderSettings::isFaceColorEnabled() const
{
	return drawMode & DRAW_FACECOLOR;
}

inline bool MeshRenderSettings::isVertexColorEnabled() const
{
	return drawMode & DRAW_VERTEXCOLOR;
}

inline int MeshRenderSettings::pointWidth() const
{
	return pWidth;
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

inline void MeshRenderSettings::setWireframe(bool b)
{
	if (b) drawMode |=  DRAW_WIREFRAME;
	else   drawMode &= ~DRAW_WIREFRAME;
}

inline void MeshRenderSettings::setFlatShading()
{
	drawMode |=  DRAW_FLAT;
	drawMode &= ~DRAW_SMOOTH;
	drawMode &= ~DRAW_POINTS;
}

inline void MeshRenderSettings::setSmoothShading()
{
	drawMode |=  DRAW_SMOOTH;
	drawMode &= ~DRAW_FLAT;
	drawMode &= ~DRAW_POINTS;
}

inline void MeshRenderSettings::setPointsShading()
{
	drawMode |=  DRAW_POINTS;
	drawMode &= ~DRAW_FLAT;
	drawMode &= ~DRAW_SMOOTH;
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

inline void MeshRenderSettings::setWireframeWidth(int width)
{
	wWidth = width;
}

inline void MeshRenderSettings::setPointWidth(int width)
{
	pWidth = width;
}

inline void MeshRenderSettings::setEnableVertexColor()
{
	drawMode |=  DRAW_VERTEXCOLOR;
	drawMode &= ~DRAW_FACECOLOR;
}

inline void MeshRenderSettings::setEnableTriangleColor()
{
	drawMode |=  DRAW_FACECOLOR;
	drawMode &= ~DRAW_VERTEXCOLOR;
}

inline void MeshRenderSettings::setVisibleBoundingBox(bool b)
{
	if (b) drawMode |=  DRAW_BOUNDINGBOX;
	else   drawMode &= ~DRAW_BOUNDINGBOX;
}

} // namespace vcl
