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

#include <vclib/mesh/requirements.h>
#include <vclib/space/color.h>

namespace vcl {

class MeshRenderSettings
{
public:
	MeshRenderSettings();

	//rendering options getters
	bool isVisible() const;

	bool isPointCloudVisible() const;
	int pointWidth() const;

	bool isSurfaceVisible() const;
	bool isSurfaceShadingFlat() const;
	bool isSurfaceShadingSmooth() const;
	bool isSurfaceColorPerFace() const;
	bool isSurfaceColorPerVertex() const;

	bool isWireframeVisible() const;
	int wireframeWidth() const;
	vcl::Color wireframeColor() const;
	const float* wireframeColorData() const;

	bool isBboxEnabled() const;

	// rendering options setters
	void setVisibility(bool b);

	void setPointCloudVisibility(bool b);
	void setPointWidth(int width);

	void setSurfaceShadingFlat();
	void setSurfaceShadingSmooth();
	void setSurfaceColorPerVertex();
	void setSurfaceColorPerFace();

	void setWireframeVisibility(bool b);
	void setWireframeWidth(int width);
	void setWireframeColor(float r, float g, float b);
	void setWireframeColor(const vcl::Color& c);

	void setBoundingBoxVisibility(bool b);

	template <MeshConcept MeshType>
	void setRenderCapabilityFrom(const MeshType m);

private:
	enum {
		DRAW_MESH = 1 << 0, // mesh visibility
		// points
		DRAW_POINTS              = 1 << 1, // point visibility
		DRAW_POINTS_SQUARE       = 1 << 2, // draw points as squares
		DRAW_POINTS_CIRCLE       = 1 << 3, // draw points as circles
		DRAW_POINTS_COLOR_VERTEX = 1 << 4, // use vertex colors for points
		DRAW_POINTS_COLOR_MESH   = 1 << 5, // use mesh color for points
		DRAW_POINTS_COLOR_USER   = 1 << 6, // use user color for points

		DRAW_SURF              = 1 << 10, // surface visibility
		DRAW_SURF_FLAT         = 1 << 11, // flat shading
		DRAW_SURF_SMOOTH       = 1 << 12, // smooth shading
		DRAW_SURF_COLOR_FACE   = 1 << 13, // use face color for surface
		DRAW_SURF_COLOR_VERTEX = 1 << 14, // use vertex color for surface
		DRAW_SURF_COLOR_MESH   = 1 << 15, // use mesh color for surface
		DRAW_SURF_COLOR_USER   = 1 << 16, // use user color for surface

		DRAW_WIREFRAME = 1 << 20, // draw wireframe
		DRAW_WIREFRAME_COLOR_USER = 1 << 21,
		DRAW_WIREFRAME_COLOR_MESH = 1 << 22,

		DRAW_BOUNDINGBOX = 1 << 25
	};

	// draw integers controlled using enum
	int   drawModeCapability = ~0;
	int   drawMode           = DRAW_MESH | DRAW_SURF_SMOOTH | DRAW_SURF_COLOR_VERTEX;

	int   pWidth             = 3;
	float pUserColor[4]      = {1, 1, 0, 1};
	float sUserColor[4]      = {0.8, 0.8, 0.8, 1};
	int   wWidth             = 1;
	float wColor[4]          = {0, 0, 0, 1};

};

} // namespace vcl

#include "mesh_render_settings.cpp"

#endif // VCLIB_RENDER_MESH_RENDER_SETTINGS_H
