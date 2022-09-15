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

#ifndef VCL_IO_OBJ_MATERIAL_H
#define VCL_IO_OBJ_MATERIAL_H

#include <vclib/space/color.h>
#include <vclib/space/point.h>

namespace vcl::io::obj {

struct Material
{
	Material();
	Material(const vcl::Color& c);
	Material(const std::string& txtName);
	Material(const vcl::Color& c, const std::string& txtName);

	bool isEmpty() const;

	vcl::Color         color() const;
	const std::string& texture() const;
	uint textureId() const;

	bool operator<(const Material& m) const;
	bool operator==(const Material& m) const;
	bool operator!=(const Material& m) const;

	Point3f Ka = Point3f(0.2f, 0.2f, 0.2f); // ambient
	Point3f Kd = Point3f(1.0f, 1.0f, 1.0f); // diffuse
	Point3f Ks = Point3f(1.0f, 1.0f, 1.0f); // specular

	float d = 1.0f;  // alpha

	int   illum = 2; // specular illumination
	float Ns    = 0.f;

	std::string map_Kd; // filename texture
	uint        mapId;  // id of the texture in the mesh, used when loading materials

	bool hasColor   = false;
	bool hasTexture = false;
};

std::ostream& operator<<(std::ostream& out, const Material& m);

} // namespace vcl::io::obj

#include "material.cpp"

#endif // VCL_IO_OBJ_MATERIAL_H
