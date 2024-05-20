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
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/render/drawable/drawable_object_vector.h>

namespace vcl {

DrawableObjectVector::DrawableObjectVector(const DrawableObjectVector& oth)
{
    mDrawVector.resize(oth.size());
    for (uint i = 0; i < oth.size(); i++)
        mDrawVector[i] = oth[i].clone();
}

DrawableObjectVector::DrawableObjectVector(DrawableObjectVector&& oth)
{
    swap(oth);
}

DrawableObjectVector::~DrawableObjectVector()
{
    // delete all the DrawableObjects
    for (DrawableObjectI* obj : mDrawVector) {
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
uint DrawableObjectVector::pushBack(const DrawableObjectI& obj)
{
    mDrawVector.push_back(obj.clone());
    return mDrawVector.size();
}

/**
 * @brief Pushes a copy of the DrawableObject to the vector.
 * This function creates a **copy** of the given argument and inserts it
 * into the back of the vector. If the given argument is a nullptr, the
 * function returns UINT_NULL.
 * @param obj
 * @return
 */
uint DrawableObjectVector::pushBack(const DrawableObjectI* obj)
{
    if (obj == nullptr)
        return UINT_NULL;
    mDrawVector.push_back(obj->clone());
    return mDrawVector.size();
}

DrawableObjectI& DrawableObjectVector::at(uint i)
{
    assert(i < mDrawVector.size());
    return *mDrawVector.at(i);
}

const DrawableObjectI& DrawableObjectVector::at(uint i) const
{
    assert(i < mDrawVector.size());
    return *mDrawVector.at(i);
}

const DrawableObjectI& DrawableObjectVector::operator[](uint i) const
{
    return *mDrawVector[i];
}

std::size_t DrawableObjectVector::size() const
{
    return mDrawVector.size();
}

DrawableObjectI& DrawableObjectVector::front()
{
    return *mDrawVector.front();
}

const DrawableObjectI& DrawableObjectVector::front() const
{
    return *mDrawVector.front();
}

DrawableObjectI& DrawableObjectVector::back()
{
    return *mDrawVector.back();
}

const DrawableObjectI& DrawableObjectVector::back() const
{
    return *mDrawVector.back();
}

void DrawableObjectVector::clear()
{
    // delete all the DrawableObjects
    for (DrawableObjectI* obj : mDrawVector) {
        delete obj;
    }
    mDrawVector.clear();
}

Box3d DrawableObjectVector::boundingBox(bool onlyVisible) const
{
    Box3d bb;
    if (mDrawVector.size() > 0) {
        uint i = onlyVisible ? firstVisibleObject() : 0;

        if (i < mDrawVector.size()) {
            Point3d sc = mDrawVector.at(i)->center();
            bb.add(sc - mDrawVector.at(i)->radius());
            bb.add(sc + mDrawVector.at(i)->radius());

            for (i = i + 1; i < mDrawVector.size(); i++) { // rest of the list
                if (!onlyVisible || mDrawVector.at(i)->isVisible()) {
                    Point3d sc = mDrawVector.at(i)->center();
                    bb.add(sc - mDrawVector.at(i)->radius());
                    bb.add(sc + mDrawVector.at(i)->radius());
                }
            }
        }
    }
    return bb;
}

void DrawableObjectVector::swap(DrawableObjectVector& oth)
{
    using std::swap;
    swap(mDrawVector, oth.mDrawVector);
}

DrawableObjectVector& DrawableObjectVector::operator=(DrawableObjectVector oth)
{
    swap(oth);
    return *this;
}

DrawableObjectVector::iterator DrawableObjectVector::begin()
{
    return mDrawVector.begin();
}

DrawableObjectVector::iterator DrawableObjectVector::end()
{
    return mDrawVector.end();
}

DrawableObjectVector::const_iterator DrawableObjectVector::begin() const
{
    return mDrawVector.begin();
}

DrawableObjectVector::const_iterator DrawableObjectVector::end() const
{
    return mDrawVector.end();
}

uint DrawableObjectVector::firstVisibleObject() const
{
    for (uint i = 0; i < mDrawVector.size(); i++)
        if (mDrawVector[i]->isVisible())
            return i;
    return UINT_NULL;
}

DrawableObjectI& DrawableObjectVector::operator[](uint i)
{
    return *mDrawVector[i];
}

} // namespace vcl
