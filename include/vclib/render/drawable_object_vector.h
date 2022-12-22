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

#ifndef VCLIB_RENDER_DRAWABLE_OBJECT_VECTOR_H
#define VCLIB_RENDER_DRAWABLE_OBJECT_VECTOR_H

#include "drawable_object.h"

namespace vcl {

class DrawableObjectVector
{
public:
	using iterator = std::vector<DrawableObject*>::iterator;
	using const_iterator = std::vector<DrawableObject*>::const_iterator;

	DrawableObjectVector();
	DrawableObjectVector(const DrawableObjectVector& oth);
	DrawableObjectVector(DrawableObjectVector&& oth);

	~DrawableObjectVector();

	uint pushBack(const DrawableObject& obj);
	uint pushBack(const DrawableObject* obj);

	DrawableObject& at(uint i);
	const DrawableObject& at(uint i) const;

	DrawableObject& operator[](uint i);
	const DrawableObject& operator[](uint i) const;

	std::size_t size() const;

	/// @private
	friend void swap(DrawableObjectVector& v1, DrawableObjectVector& v2);
	void swap(DrawableObjectVector& oth);

	DrawableObjectVector& operator=(DrawableObjectVector oth);

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

private:
	std::vector<DrawableObject*> drawVector;
};

inline void swap(DrawableObjectVector& v1, DrawableObjectVector& v2);

} // namespace vcl

#include "drawable_object_vector.cpp"

#endif // VCLIB_RENDER_DRAWABLE_OBJECT_VECTOR_H
