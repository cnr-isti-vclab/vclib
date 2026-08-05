// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_INFO_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_INFO_EDITOR_BGFX_H

#include <vclib/bgfx/primitives/lines.h>
#include <vclib/bgfx/text/text_view.h>
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

    vcl::TextView mTextView;
    bool          mTextViewInitialized = false;

    std::vector<vcl::Point3d> mLastFacePositions;

    vcl::Color mTextColor = vcl::Color::Black;
    int        mTextSize  = 20;

public:
    InfoEditorBGFX()
    {
        Base::settings().customSettings["color"] = vcl::Color(vcl::Color::Red);
        Base::settings().customSettings["thickness"] = 5.0f;
        Base::settings().customSettings["text_color"] = vcl::Color(vcl::Color::Black);
        Base::settings().customSettings["text_size"] = 20;

        mOutlineLines.setGeneralColor(vcl::Color::Red);
        mOutlineLines.setWidth(5.0f);
        mOutlineLines.setTopology(vcl::Lines::Topology::LINE_STRIP);
        mOutlineLines.setDepthOffset(0.00012f);
    }

    // Editor implementation

    void setActive(bool active) override
    {
        Base::setActive(active);
        if (mTextViewInitialized) {
            mTextView.enableText(active);
        }
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
        if (Base::settings().customSettings.count("text_color")) {
            mTextColor = std::any_cast<vcl::Color>(
                Base::settings().customSettings.at("text_color"));
        }
        if (Base::settings().customSettings.count("text_size")) {
            mTextSize = std::any_cast<int>(
                Base::settings().customSettings.at("text_size"));
            if (mTextViewInitialized) {
                auto dpi = Base::viewerDpiScale();
                mTextView.setTextFont(vcl::VclFont::DROID_SANS, mTextSize * dpi.x());
            }
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

        auto size = Base::viewerCanvasSize();
        if (!mTextViewInitialized) {
            mTextView.init(size.x(), size.y());
            auto dpi = Base::viewerDpiScale();
            mTextView.setTextFont(vcl::VclFont::DROID_SANS, mTextSize * dpi.x());
            mTextView.enableText(Base::isActive());
            mTextViewInitialized = true;
        } else {
            mTextView.resize(size.x(), size.y());
        }

        mTextView.clearText();

        if (mLastObjectId != USHORT_NULL && mLastElementType == vcl::MeshInfo::FACE && mOutlineLines.hasPositions()) {
            const auto& pts = mLastFacePositions;
            if (pts.size() > 1) {
                vcl::Point3d barycenter(0.0, 0.0, 0.0);
                for (size_t i = 0; i < pts.size() - 1; ++i) {
                    barycenter += pts[i];
                }
                barycenter /= double(pts.size() - 1);

                vcl::Matrix44f mv = Base::viewerViewMatrix();
                vcl::Matrix44f proj = Base::viewerProjectionMatrix();
                vcl::Point4f p(barycenter.x(), barycenter.y(), barycenter.z(), 1.0f);
                p = proj * (mv * p);
                
                if (p.w() != 0.0f) {
                    p /= p.w();
                    
                    vcl::Point2f pos2D(
                        (p.x() + 1.0f) * 0.5f * size.x(),
                        (1.0f - p.y()) * 0.5f * size.y()
                    );
                    
                    mTextView.appendTransientText(pos2D, "f # " + std::to_string(mLastElementId), mTextColor);
                }
            }
        }

        mTextView.frame(Base::viewerCanvasFrameBuffer());
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
                                mLastFacePositions = positions;
                            }
                        }
                    }
                    else {
                        mOutlineLines.setVertices(std::vector<Point3d>());
                        mLastFacePositions.clear();
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
