/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_RENDER_INTERFACES_DRAWABLE_OBJECT_H
#define VCL_RENDER_INTERFACES_DRAWABLE_OBJECT_H

#include <vclib/space/core/box.h>
#include <vclib/space/core/point.h>

namespace vcl {

/**
 * @brief The DrawableObject class is the base class for all the objects that
 * can be drawn in a 3D viewer.
 *
 * The DrawableObject class stores only an attribute that represents the name
 * of the object, that will be used to identify it in the viewer.
 *
 * A class that inherits from DrawableObject must implement the following member
 * functions:
 * - draw(uint viewId);
 * - center();
 * - radius();
 * - clone();
 * - isVisibile();
 * - setVisibility(bool);
 *
 * There is also a member function that can be implemented, but it is not
 * mandatory:
 * - init();
 *
 * For more details about these member functions, check the documentation of
 * each one.
 */
class DrawableObject
{
    std::string mName; /**< @brief Name of the object */

    std::string mInfo; /**< @brief Info about the object */

public:
    /**< @brief Empty constructor */
    DrawableObject() = default;

    virtual ~DrawableObject() {}

    /**
     * @brief This member function is called after the initialization of the
     * Context. It must initialize and bind data to the GPU like buffers and
     * textures.
     */
    virtual void init() {};

    /**
     * @brief This member function must draw the object.
     * It will be called at every frame.
     *
     * @param viewId The ID of the view to draw. It may be used depending on the
     * rendering engine.
     */
    virtual void draw(uint viewId = 0) const = 0;

    /**
     * @brief This member function is used to find a good camera position
     * to render object. It should return the the bounding box of the object.
     * Return a null bounding box if the object shouldn't influence the position
     * of the camera.
     * @return The bounding box of the object.
     */
    virtual vcl::Box3d boundingBox() const = 0;

    /**
     * @brief This member function is used to create a new copy of the
     * DrawableObject. Each derived class must implement this member function,
     * that returns a shared pointer pointing to a copy of the current one. for
     * more details about this paradigm, check polimorphism clone in modern c++:
     * https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/
     *
     * @return A new dynamically allocated DrawableObject that is a copy of the
     * current one.
     */
    virtual std::shared_ptr<DrawableObject> clone() const = 0;

    /**
     * @brief This member function is used to check if the object is visible.
     * @return `true` if the object is visible;
     */
    virtual bool isVisible() const = 0;

    /**
     * @brief This member function is used to set the visibility of the object.
     * @param[in] vis: `true` if the object should be visible;
     */
    virtual void setVisibility(bool vis) = 0;

    /**
     * @brief Returns the name of the object.
     * @return The name of the object.
     */
    const std::string& name() const { return mName; }

    /**
     * @brief Returns a reference of the name of the object, that allows to
     * modify it.
     * @param[in] name: The name of the object.
     */
    std::string& name() { return mName; }

    /**
     * @brief Returns the info of the object.
     * @return The info of the object.
     */
    const std::string& info() const { return mInfo; }

    /**
     * @brief Returns a reference of the info of the object, that allows to
     * modify it.
     * @param[in] info: The info of the object.
     */
    std::string& info() { return mInfo; }
};

} // namespace vcl

#endif // VCL_RENDER_INTERFACES_DRAWABLE_OBJECT_H
