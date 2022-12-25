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

#ifndef VCL_RENDER_MESH_RENDER_SETTINGS_H
#define VCL_RENDER_MESH_RENDER_SETTINGS_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/color.h>

namespace vcl {

/**
 * @brief The MeshRenderSettings class allows an easy management of render settings of a Mesh.
 * This class stores the rendering status of a Mesh and the rendering capability of a Mesh.
 *
 * Render capabilities store what can be actually rendered of a vcl::Mesh (e.g. it will be possible
 * to render the surface of the mesh if the Mesh has Faces, or it will be possible to render per
 * Face colors only if the Mesh has **enabled** per face colors).
 *
 * The render status of this class is **guaranteed to be consistent** to the render capabilities.
 *
 * An instance of this class must be initialized with a vcl::Mesh at first, in order to initialize
 * the Render capabilites. Render settings won't be possible to set if this class is not first
 * initialized using a vcl::Mesh, trough the constructor or the `setRenderCapabilityFrom()` member
 * function.
 *
 * Render capabilities can be get by calling the member functions `can*()` of this class.
 *
 * The memeber functions `is*()` allow to get the current render status.
 *
 * The render status can be modified using the `set*()` member functions, that return a boolean
 * indicating if the operation has been performed (if the capabilities allows it, the operation will
 * be always performed).
 */
class MeshRenderSettings
{
public:
	MeshRenderSettings();

	template <MeshConcept MeshType>
	MeshRenderSettings(const MeshType& m);

	//rendering options capability of the mesh
	bool canBeVisible() const;
	bool canPointCloudBeVisible() const;
	bool canPointCloudBeColoredPerVertex() const;
	bool canPointCloudBeColoredPerMesh() const;
	bool canSurfaceBeVisible() const;
	bool canSurfaceBeSmooth() const;
	bool canSurfaceBeColoredPerFace() const;
	bool canSurfaceBeColoredPerVertex() const;
	bool canSurfaceBeColoredPerMesh() const;
	bool canSurfaceBeColoredPerVertexTexcoords() const;
	bool canSurfaceBeColoredPerWedgeTexcoords() const;
	bool canWireframeBeColoredPerMesh() const;
	bool canBoundingBoxBeVisible() const;

	//rendering options getters
	bool isVisible() const;

	bool isPointCloudVisible() const;
	bool isPointCloudColorPerVertex() const;
	bool isPointCloudColorPerMesh() const;
	bool isPointCloudColorUserDefined() const;
	int pointWidth() const;
	vcl::Color pointCloudUserColor() const;
	const float* pointCloudUserColorData() const;

	bool isSurfaceVisible() const;
	bool isSurfaceShadingFlat() const;
	bool isSurfaceShadingSmooth() const;

	bool isSurfaceColorPerFace() const;
	bool isSurfaceColorPerVertex() const;
	bool isSurfaceColorPerMesh() const;
	bool isSurfaceColorUserDefined() const;
	bool isSurfaceColorPerVertexTexcoords() const;
	bool isSurfaceColorPerWedgeTexcoords() const;
	vcl::Color surfaceUserColor() const;
	const float* surfaceUserColorData() const;

	bool isWireframeVisible() const;
	bool isWireframeColorPerMesh() const;
	bool isWireframeColorUserDefined() const;
	int wireframeWidth() const;
	vcl::Color wireframeUserColor() const;
	const float* wireframeUserColorData() const;

	bool isBboxEnabled() const;

	// rendering options setters
	bool setVisibility(bool b);

	bool setPointCloudVisibility(bool b);
	bool setPointCloudColorPerVertex();
	bool setPointCloudColorPerMesh();
	bool setPointCloudColorUserDefined();

	bool setPointWidth(int width);
	bool setPointCloudUserColor(float r, float g, float b, float a = 1);
	bool setPointCloudUserColor(const vcl::Color& c);

	bool setSurfaceVisibility(bool b);
	bool setSurfaceShadingFlat();
	bool setSurfaceShadingSmooth();

	bool setSurfaceColorPerVertex();
	bool setSurfaceColorPerFace();
	bool setSurfaceColorPerMesh();
	bool setSurfaceColorUserDefined();
	bool setSurfaceColorPerVertexTexcoords();
	bool setSurfaceColorPerWedgeTexcoords();

	bool setSurfaceUserColor(float r, float g, float b, float a = 1);
	bool setSurfaceUserColor(const vcl::Color& c);

	bool setWireframeVisibility(bool b);
	bool setWireframeWidth(int width);
	bool setWireframeColorPerMesh();
	bool setWireframeColorUserDefined();
	bool setWireframeUserColor(float r, float g, float b, float a = 1);
	bool setWireframeUserColor(const vcl::Color& c);

	bool setBoundingBoxVisibility(bool b);

	template <MeshConcept MeshType>
	void setRenderCapabilityFrom(const MeshType& m);

	void setDefaultSettingsFromCapability();

private:
	enum {
		DRAW_MESH = 1 << 0, // mesh visibility
		// points
		DRAW_POINTS              = 1 << 1, // point visibility
		DRAW_POINTS_PIXEL        = 1 << 2, // draw points as pixels
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
		DRAW_SURF_TEX_VERTEX   = 1 << 17, // use per vertex texcoords
		DRAW_SURF_TEX_WEDGE    = 1 << 18, // use per wedge texcoords

		DRAW_WIREFRAME = 1 << 20, // draw wireframe
		DRAW_WIREFRAME_COLOR_USER = 1 << 21,
		DRAW_WIREFRAME_COLOR_MESH = 1 << 22,

		DRAW_BOUNDINGBOX = 1 << 25
	};

	// draw integers controlled using enum
	int   drawModeCapability = 0;
	int   drawMode           = 0;

	int   pWidth             = 3;
	float pUserColor[4]      = {1, 1, 0, 1};
	float sUserColor[4]      = {0.8, 0.8, 0.8, 1};
	int   wWidth             = 1;
	float wUserColor[4]      = {0, 0, 0, 1};

};

} // namespace vcl

#include "mesh_render_settings.cpp"

#endif // VCL_RENDER_MESH_RENDER_SETTINGS_H
