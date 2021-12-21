/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "material.h"

namespace vcl::io::obj {

inline Material::Material()
{
}

inline Material::Material(const Color& c) : hasColor(true)
{
	Ka.x() = c.redF();
	Ka.y() = c.greenF();
	Ka.z() = c.blueF();
}

inline Material::Material(const std::string& txtName) : map_Kd(txtName), hasTexture(true)
{
}

inline Material::Material(const Color& c, const std::string& txtName) :
		map_Kd(txtName), hasColor(true), hasTexture(true)
{
	Ka.x() = c.redF();
	Ka.y() = c.greenF();
	Ka.z() = c.blueF();
}

/**
 * @brief Operator that allows to sort materials
 * first we sort trough color
 * - if a material has no color, is < than one that has a color
 * - if both materials have color, order by color: if same, check texture
 * sort trough texture
 * - if a material has no texture, is < than one that has texture
 * - if both materials have texture, order by texture name
 */
inline bool Material::operator<(const Material& m)
{
	if (hasColor) {
		if (!m.hasColor) // color > no color
			return false;
		if (Ka != m.Ka)
			return Ka < m.Ka;
	}
	else if (m.hasColor) { // no color < color
		return true;
	}
	// will arrive here only if:
	// - this Material and m have both no color
	// - this Material has the same color of m
	if (hasTexture) {
		if (!m.hasTexture) // texture > no texture
			return false;
		return map_Kd < m.map_Kd;
	}
	else if (m.hasTexture) { // no texture < texture
		return true;
	}
	else { // no color and texture in both materials
		return false;
	}
}

} // namespace vcl::io::obj
