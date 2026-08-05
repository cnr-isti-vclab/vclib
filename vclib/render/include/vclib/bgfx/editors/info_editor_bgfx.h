// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_INFO_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_INFO_EDITOR_BGFX_H

#include <vclib/bgfx/primitives/lines.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/editors/editor.h>
#include <vclib/space/complex/mesh_info.h>

#include <iostream>

namespace vcl {

template<typename ViewerDrawer>
class InfoEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    ushort mLastObjectId    = USHORT_NULL;
    ushort mLastElementType = USHORT_NULL;
    uint   mLastElementId   = 0;

    vcl::Lines mOutlineLines;

public:
    InfoEditorBGFX()
    {
        Base::settings().customSettings["color"] = vcl::Color(vcl::Color::Red);
        Base::settings().customSettings["thickness"] = 5.0f;

        mOutlineLines.setGeneralColor(vcl::Color::Red);
        mOutlineLines.setWidth(5.0f);
        mOutlineLines.setTopology(vcl::Lines::Topology::LINE_STRIP);
        mOutlineLines.setDepthOffset(0.00012f);
    }

    // Editor implementation

    void setActive(bool active) override
    {
        Base::setActive(active);
        Base::viewerUpdate();
    }

    void refreshSettings() override
    {
        if (Base::settings().customSettings.count("color")) {
            mOutlineLines.setGeneralColor(
                std::any_cast<vcl::Color>(
                    Base::settings().customSettings.at("color")));
        }
        if (Base::settings().customSettings.count("thickness")) {
            mOutlineLines.setWidth(
                std::any_cast<float>(
                    Base::settings().customSettings.at("thickness")));
        }
        Base::viewerUpdate();
    }

    void draw(uint viewId) override
    {
        if (mLastObjectId != USHORT_NULL &&
            mLastElementType == vcl::MeshInfo::FACE) {
            if (mOutlineLines.hasPositions()) {
                mOutlineLines.draw(viewId);
            }
        }
    }

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        bool block = Base::onMousePress(button, x, y, modifiers);

        if (!block && button == vcl::MouseButton::LEFT) {
            block = true; // consume the event to prevent further propagation
            auto callback =
                [this](ushort objectId, ushort elementType, uint elementId) {
                    if (objectId == 0xFFFF) {
                        mLastObjectId = 0xFFFF;
                        Base::viewerUpdate();
                        return;
                    }

                    std::cerr << "InfoEditor:\n"
                              << "  Object ID:    " << objectId << "\n"
                              << "  Element Type: " << elementType << "\n"
                              << "  Element ID:   " << elementId << "\n";

                    mLastObjectId    = objectId;
                    mLastElementType = elementType;
                    mLastElementId   = elementId;

                    if (elementType == vcl::MeshInfo::FACE) {
                        auto list = Base::drawList();
                        if (list && objectId < list->size()) {
                            auto obj     = list->at(objectId);
                            auto meshObj = std::dynamic_pointer_cast<
                                vcl::AbstractDrawableMesh>(obj);
                            if (meshObj) {
                                auto positions =
                                    meshObj->meshProvider().facePositions(
                                        elementId);
                                if (positions.size() > 0) {
                                    positions.push_back(positions[0]);
                                }
                                auto T =
                                    meshObj->meshProvider().transformMatrix();
                                multiplyPointsByMatrix(positions, T);
                                mOutlineLines.setVertices(positions);
                            }
                        }
                    }
                    else {
                        mOutlineLines.setVertices(std::vector<Point3d>());
                    }

                    Base::viewerUpdate();
                };

            Base::viewerReadElementIdRequest(x, y, callback);
        }
        return block;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_INFO_EDITOR_BGFX_H
