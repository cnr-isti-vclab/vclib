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
    std::vector<DrawableObject*> drawVector;

public:
    using iterator       = std::vector<DrawableObject*>::iterator;
    using const_iterator = std::vector<DrawableObject*>::const_iterator;

    DrawableObjectVector() = default;

    DrawableObjectVector(const DrawableObjectVector& oth)
    {
        drawVector.resize(oth.size());
        for (uint i = 0; i < oth.size(); i++)
            drawVector[i] = oth[i].clone();
    }

    DrawableObjectVector(DrawableObjectVector&& oth) { swap(oth); }

    ~DrawableObjectVector()
    {
        // delete all the DrawableObjects
        for (DrawableObject* obj : drawVector) {
            delete obj;
        }
    }

    /**
     * @brief Pushes a copy of the DrawableObject to the vector.
     *
     * This function creates a **copy** of the given argument and inserts it
     * into the back of the vector.
     *
     * @param obj
     * @return
     */
    uint pushBack(const DrawableObject& obj)
    {
        drawVector.push_back(obj.clone());
        return drawVector.size();
    }

    /**
     * @brief Pushes a copy of the DrawableObject to the vector.
     *
     * This function creates a **copy** of the given argument and inserts it
     * into the back of the vector. If the given argument is a nullptr, the
     * function returns UINT_NULL.
     *
     * @param obj
     * @return
     */
    uint pushBack(const DrawableObject* obj)
    {
        if (obj == nullptr)
            return UINT_NULL;
        drawVector.push_back(obj->clone());
        return drawVector.size();
    }

    DrawableObject& at(uint i)
    {
        assert(i < drawVector.size());
        return *drawVector.at(i);
    }

    const DrawableObject& at(uint i) const
    {
        assert(i < drawVector.size());
        return *drawVector.at(i);
    }

    DrawableObject& operator[](uint i) { return *drawVector[i]; }

    const DrawableObject& operator[](uint i) const { return *drawVector[i]; }

    std::size_t size() const { return drawVector.size(); }

    void clear()
    {
        // delete all the DrawableObjects
        for (DrawableObject* obj : drawVector) {
            delete obj;
        }
        drawVector.clear();
    }

    vcl::Box3d boundingBox(bool onlyVisible = true) const
    {
        Box3d bb;
        if (drawVector.size() > 0) {
            uint i = onlyVisible ? firstVisibleObject() : 0;

            if (i < drawVector.size()) {
                Point3d sc = drawVector.at(i)->center();
                bb.add(sc - drawVector.at(i)->radius());
                bb.add(sc + drawVector.at(i)->radius());

                for (i = i + 1; i < drawVector.size(); i++)
                { // rest of the list
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

    void swap(DrawableObjectVector& oth)
    {
        using std::swap;
        swap(drawVector, oth.drawVector);
    }

    DrawableObjectVector& operator=(DrawableObjectVector oth)
    {
        swap(oth);
        return *this;
    }

    iterator begin() { return drawVector.begin(); }

    iterator end() { return drawVector.end(); }

    const_iterator begin() const { return drawVector.begin(); }

    const_iterator end() const { return drawVector.end(); }

private:
    uint firstVisibleObject() const
    {
        for (uint i = 0; i < drawVector.size(); i++)
            if (drawVector[i]->isVisible())
                return i;
        return UINT_NULL;
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_OBJECT_VECTOR_H
