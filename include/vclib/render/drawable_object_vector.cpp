/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#include "drawable_object_vector.h"

namespace vcl {

inline DrawableObjectVector::DrawableObjectVector()
{
}

inline DrawableObjectVector::DrawableObjectVector(const DrawableObjectVector& oth)
{
	drawVector.resize(oth.size());
	for (uint i = 0; i < oth.size(); i++)
		drawVector[i] = oth[i].clone();
}

inline DrawableObjectVector::DrawableObjectVector(DrawableObjectVector&& oth)
{
	swap(oth);
}

inline DrawableObjectVector::~DrawableObjectVector()
{
	// delete all the DrawableObjects
	for (DrawableObject* obj : drawVector) {
		delete obj;
	}
}

/**
 * @brief Pushes the DrawableObject to the vector. This function creates a **copy** of the given
 * argument and inserts it into the vector.
 * @param obj
 * @return
 */
inline uint DrawableObjectVector::pushBack(const DrawableObject& obj)
{
	drawVector.push_back(obj.clone());
	return drawVector.size();
}

inline uint DrawableObjectVector::pushBack(const DrawableObject* obj)
{
	if (obj == nullptr) return -1;
	drawVector.push_back(obj->clone());
	return drawVector.size();
}

inline DrawableObject& DrawableObjectVector::at(uint i)
{
	assert(i < drawVector.size());
	return *drawVector[i];
}

inline const DrawableObject& DrawableObjectVector::at(uint i) const
{
	assert(i < drawVector.size());
	return *drawVector[i];
}

inline DrawableObject &DrawableObjectVector::operator[](uint i)
{
	return at(i);
}

inline const DrawableObject &DrawableObjectVector::operator[](uint i) const
{
	return at(i);
}

inline std::size_t DrawableObjectVector::size() const
{
	return drawVector.size();
}

inline void DrawableObjectVector::swap(DrawableObjectVector &oth)
{
	vcl::swap(*this, oth);
}

inline DrawableObjectVector::iterator DrawableObjectVector::begin()
{
	return drawVector.begin();
}

inline DrawableObjectVector::iterator DrawableObjectVector::end()
{
	return drawVector.end();
}

inline DrawableObjectVector::const_iterator DrawableObjectVector::begin() const
{
	return drawVector.begin();
}

inline DrawableObjectVector::const_iterator DrawableObjectVector::end() const
{
	return drawVector.end();
}

inline void swap(DrawableObjectVector& v1, DrawableObjectVector& v2)
{
	using std::swap;
	std::swap(v1.drawVector, v2.drawVector);
}

} // namespace vcl
