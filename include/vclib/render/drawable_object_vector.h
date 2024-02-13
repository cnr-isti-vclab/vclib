/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_RENDER_DRAWABLE_OBJECT_VECTOR_H
#define VCL_RENDER_DRAWABLE_OBJECT_VECTOR_H

#include "drawable_object.h"

#include <vclib/space/box.h>

namespace vcl {

class DrawableObjectVector
{
    std::vector<DrawableObjectI*> drawVector;

public:
    using iterator       = std::vector<DrawableObjectI*>::iterator;
    using const_iterator = std::vector<DrawableObjectI*>::const_iterator;

    DrawableObjectVector() = default;

    DrawableObjectVector(const DrawableObjectVector& oth);

    DrawableObjectVector(DrawableObjectVector&& oth);

    ~DrawableObjectVector();

    uint pushBack(const DrawableObjectI& obj);

    uint pushBack(const DrawableObjectI* obj);

    DrawableObjectI& at(uint i);

    const DrawableObjectI& at(uint i) const;

    DrawableObjectI& operator[](uint i);

    const DrawableObjectI& operator[](uint i) const;

    std::size_t size() const;

    void clear();

    vcl::Box3d boundingBox(bool onlyVisible = true) const;

    void swap(DrawableObjectVector& oth);

    DrawableObjectVector& operator=(DrawableObjectVector oth);

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

private:
    uint firstVisibleObject() const;
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_OBJECT_VECTOR_H
