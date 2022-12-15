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

#ifndef VCLIB_RENDER_MESH_RENDER_SETTINGS_H
#define VCLIB_RENDER_MESH_RENDER_SETTINGS_H

#include <vclib/space/color.h>

namespace vcl {

class MeshRenderSettings
{
public:
	MeshRenderSettings();

	//rendering options getters
	bool isWireframeEnabled() const;
	bool isPointShadingEnabled() const;
	bool isFlatShadingEnabled() const;
	bool isSmoothShadingEnabled() const;
	bool isBboxEnabled() const;
	bool isFaceColorEnabled() const;
	bool isVertexColorEnabled() const;
	int pointWidth() const;
	int wireframeWidth() const;
	vcl::Color wireframeColor() const;
	const float* wireframeColorData() const;

	// rendering options setters
	void setWireframe(bool b);
	void setFlatShading();
	void setSmoothShading();
	void setPointsShading();
	void setWireframeColor(float r, float g, float b);
	void setWireframeColor(const vcl::Color& c);
	void setWireframeWidth(int width);
	void setPointWidth(int width);
	void setEnableVertexColor();
	void setEnableTriangleColor();
	void setVisibleBoundingBox(bool b);

private:
	enum {
		DRAW_MESH        = 1 << 0,
		DRAW_POINTS      = 1 << 1,
		DRAW_FLAT        = 1 << 2,
		DRAW_SMOOTH      = 1 << 3,
		DRAW_WIREFRAME   = 1 << 4,
		DRAW_FACECOLOR   = 1 << 5,
		DRAW_VERTEXCOLOR = 1 << 6,
		DRAW_BOUNDINGBOX = 1 << 7
	};

	int   drawMode  = DRAW_SMOOTH | DRAW_VERTEXCOLOR;
	int   wWidth    = 1;
	float wColor[4] = {0, 0, 0, 1};
	int   pWidth    = 3;
};

} // namespace vcl

#include "mesh_render_settings.cpp"

#endif // VCLIB_RENDER_MESH_RENDER_SETTINGS_H
