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

#ifndef VCLIB_RENDER_GENERIC_DRAWABLE_MESH_H
#define VCLIB_RENDER_GENERIC_DRAWABLE_MESH_H

#include "drawable_object.h"
#include "mesh_render_settings.h"

namespace vcl {

class GenericDrawableMesh : public vcl::DrawableObject
{
public:
	GenericDrawableMesh();

	template<MeshConcept MeshType>
	GenericDrawableMesh(const MeshType& m);

	const MeshRenderSettings& renderSettings() const;
	void setRenderSettings(const MeshRenderSettings& rs);

	// DrawableObject implementation
	void draw() const = 0;
	vcl::Point3d sceneCenter() const = 0;
	double sceneRadius() const = 0;
	DrawableObject* clone() const = 0;

protected:
	MeshRenderSettings mrs;
};

} // namespace vcl

#include "generic_drawable_mesh.cpp"

#endif // VCLIB_RENDER_GENERIC_DRAWABLE_MESH_H
