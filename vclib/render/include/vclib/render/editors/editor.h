/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_RENDER_EDITORS_EDITOR_H
#define VCL_RENDER_EDITORS_EDITOR_H

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/input.h>
#include <vclib/render/settings/editor_settings.h>

#include <vclib/base.h>

namespace vcl {

/**
 * @brief The Editor abstract class is a base class for all the editors of the
 * VCLib Render module.
 *
 * An editor is an object contained in an AbstractViewerDrawer that can "edit"
 * the behavior of the viewer itself. For example, it can be a gizmo that allows
 * to edit the position of a drawable object, or it can be a widget that allows
 * draw additional elements on top of the viewer (e.g. a grid, or a bounding
 * box).
 */
template<typename ViewerDrawer>
class Editor
{
    friend ViewerDrawer;

    ViewerDrawer* mViewer = nullptr;

    std::shared_ptr<DrawableObjectVector> mDrawList;

    EditorSettings mSettings;

    bool mIsActive = false;

public:
    using ViewerDrawerType = ViewerDrawer;

    Editor() = default;

    bool isActive() const { return mIsActive; }

    virtual void setActive(bool active) { mIsActive = active; }

    EditorSettings& settings() { return mSettings; }

    const EditorSettings& settings() const { return mSettings; }

    virtual void refresh() {}

    virtual void draw(uint viewId) const = 0;

    virtual bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onKeyRelease(Key::Enum key, const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMouseMove(double x, double y, const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMouseScroll(
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

protected:
    std::shared_ptr<DrawableObjectVector> drawList() const { return mDrawList; }

    void viewerReadIdRequest(
        double                    x,
        double                    y,
        std::function<void(uint)> idCallback)
    {
        assert(mViewer);
        mViewer->readIdRequest(x, y, std::move(idCallback));
    }

    void viewerUpdate() const
    {
        assert(mViewer);
        mViewer->requestUpdate();
    }

private:
    // functions called by the (friend) viewer to set up the Editor
    void setViewer(ViewerDrawer* viewer) { mViewer = viewer; }

    void setDrawableObjectVector(const std::shared_ptr<DrawableObjectVector>& v)
    {
        mDrawList = v;
        refresh();
    }
};

} // namespace vcl

#endif // VCL_RENDER_EDITORS_EDITOR_H
