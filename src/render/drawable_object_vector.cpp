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

#include <vclib/render/drawable_object_vector.h>

namespace vcl {

DrawableObjectVector::DrawableObjectVector(const DrawableObjectVector& oth)
{
    drawVector.resize(oth.size());
    for (uint i = 0; i < oth.size(); i++)
        drawVector[i] = oth[i].clone();
}

DrawableObjectVector::DrawableObjectVector(DrawableObjectVector&& oth)
{
    swap(oth);
}

DrawableObjectVector::~DrawableObjectVector()
{
    // delete all the DrawableObjects
    for (DrawableObject* obj : drawVector) {
        delete obj;
    }
}

/**
 * @brief Pushes a copy of the DrawableObject to the vector.
 * This function creates a **copy** of the given argument and inserts it
 * into the back of the vector.
 * @param obj
 * @return
 */
uint DrawableObjectVector::pushBack(const DrawableObject& obj)
{
    drawVector.push_back(obj.clone());
    return drawVector.size();
}

/**
 * @brief Pushes a copy of the DrawableObject to the vector.
 * This function creates a **copy** of the given argument and inserts it
 * into the back of the vector. If the given argument is a nullptr, the
 * function returns UINT_NULL.
 * @param obj
 * @return
 */
uint DrawableObjectVector::pushBack(const DrawableObject* obj)
{
    if (obj == nullptr)
        return UINT_NULL;
    drawVector.push_back(obj->clone());
    return drawVector.size();
}

DrawableObject& DrawableObjectVector::at(uint i)
{
    assert(i < drawVector.size());
    return *drawVector.at(i);
}

const DrawableObject& DrawableObjectVector::at(uint i) const
{
    assert(i < drawVector.size());
    return *drawVector.at(i);
}

const DrawableObject& DrawableObjectVector::operator[](uint i) const
{
    return *drawVector[i];
}

std::size_t DrawableObjectVector::size() const
{
    return drawVector.size();
}

void DrawableObjectVector::clear()
{
    // delete all the DrawableObjects
    for (DrawableObject* obj : drawVector) {
        delete obj;
    }
    drawVector.clear();
}


Box3d DrawableObjectVector::boundingBox(bool onlyVisible) const
{
    Box3d bb;
    if (drawVector.size() > 0) {
        uint i = onlyVisible ? firstVisibleObject() : 0;

        if (i < drawVector.size()) {
            Point3d sc = drawVector.at(i)->center();
            bb.add(sc - drawVector.at(i)->radius());
            bb.add(sc + drawVector.at(i)->radius());

            for (i = i + 1; i < drawVector.size(); i++) { // rest of the list
                if (!onlyVisible || drawVector.at(i)->isVisible()) {
                    Point3d sc = drawVector.at(i)->center();
                    bb.add(sc - drawVector.at(i)->radius());
                    bb.add(sc + drawVector.at(i)->radius());
                }
            }
        }
    }
    return bb;
}

void DrawableObjectVector::swap(DrawableObjectVector& oth)
{
    using std::swap;
    swap(drawVector, oth.drawVector);
}

DrawableObjectVector& DrawableObjectVector::operator=(DrawableObjectVector oth)
{
    swap(oth);
    return *this;
}

DrawableObjectVector::iterator DrawableObjectVector::begin()
{
    return drawVector.begin();
}

DrawableObjectVector::iterator DrawableObjectVector::end()
{
    return drawVector.end();
}

DrawableObjectVector::const_iterator DrawableObjectVector::begin() const
{
    return drawVector.begin();
}

DrawableObjectVector::const_iterator DrawableObjectVector::end() const
{
    return drawVector.end();
}

uint DrawableObjectVector::firstVisibleObject() const
{
    for (uint i = 0; i < drawVector.size(); i++)
        if (drawVector[i]->isVisible())
            return i;
    return UINT_NULL;
}

DrawableObject& DrawableObjectVector::operator[](uint i)
{
    return *drawVector[i];
}

} // namespace vcl
