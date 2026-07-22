// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_H
#define VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_H

#include <vclib/render/settings/draw_object_settings.h>

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
 * - draw(DrawObjectSettings settings);
 * - center();
 * - radius();
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
     * @param[in] settings: The settings to use to draw the object.
     */
    virtual void draw(const DrawObjectSettings& settings = {}) = 0;

    /**
     * @brief This member function should draw the object.
     * It will be called on request when the renderer needs to draw the ID of
     * the object.
     *
     * @param[in] settings: The settings to use to draw the object.
     */
    virtual void drawId(const DrawObjectSettings& settings) {};

    /**
     * @brief This member function is used to find a good camera position
     * to render object. It should return the the bounding box of the object.
     * Return a null bounding box if the object shouldn't influence the position
     * of the camera.
     * @return The bounding box of the object.
     */
    virtual vcl::Box3d boundingBox() const = 0;

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
    virtual const std::string& name() const { return mName; }

    /**
     * @brief Returns a reference of the name of the object, that allows to
     * modify it.
     * @param[in] name: The name of the object.
     */
    virtual std::string& name() { return mName; }

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

protected:
    /**
     * @brief Utility swap function that allows to swap the content of two
     * DrawableObject instances.
     *
     * It is meant to be used by the derived classes to implement the swap
     * member function (see the copy and swap idiom).
     *
     * @param[in] other: The other DrawableObject to swap with.
     */
    void swap(DrawableObject& other)
    {
        using std::swap;
        swap(mName, other.mName);
        swap(mInfo, other.mInfo);
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_DRAWABLE_OBJECT_H
