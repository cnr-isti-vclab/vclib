/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
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

#include "material.h"

namespace vcl::io::obj {

inline Material::Material()
{
}

inline Material::Material(const Color& c) : hasColor(true)
{
	Kd.x() = c.redF();
	Kd.y() = c.greenF();
	Kd.z() = c.blueF();
	d = c.alphaF();
}

inline Material::Material(const std::string& txtName) : map_Kd(txtName), hasTexture(true)
{
}

inline Material::Material(const Color& c, const std::string& txtName) :
		map_Kd(txtName), hasColor(true), hasTexture(true)
{
	Kd.x() = c.redF();
	Kd.y() = c.greenF();
	Kd.z() = c.blueF();
	d = c.alphaF();
}

inline bool Material::isEmpty() const
{
	return !hasColor && !hasTexture;
}

inline Color Material::color() const
{
	return vcl::Color(Kd.x()*255, Kd.y()*255, Kd.z()* 255, d*255);
}

inline const std::string& Material::texture() const
{
	return map_Kd;
}

inline uint Material::textureId() const
{
	return mapId;
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
inline bool Material::operator<(const Material& m) const
{
	if (hasColor) {
		if (!m.hasColor) // color > no color
			return false;
		if (Kd != m.Kd)
			return Kd < m.Kd;
		if (d != m.d)
			return d < m.d;
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

bool Material::operator==(const Material& m) const
{
	return !(*this < m) && !(m < *this);
}

bool Material::operator!=(const Material& m) const
{
	return !(*this == m);
}

std::ostream& operator<<(std::ostream& out, const Material& m)
{
	if (m.hasColor) {
		out << "Kd " << m.Kd.x() << " " << m.Kd.y() << " " << m.Kd.z() << std::endl;
		out << "d " << m.d << std::endl;
	}
	if (m.hasTexture) {
		out << "map_Kd " << m.map_Kd << std::endl;
	}
	return out;
}

} // namespace vcl::io::obj
