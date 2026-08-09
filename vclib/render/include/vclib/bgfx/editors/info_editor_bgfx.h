// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_INFO_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_INFO_EDITOR_BGFX_H

#include <vclib/bgfx/primitives/lines.h>
#include <vclib/bgfx/primitives/points.h>
#include <vclib/bgfx/text/text_view.h>

#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/editors/editor.h>
#include <vclib/render/settings/info_editor_settings.h>

#include <vclib/space/complex.h>

#include <optional>

namespace vcl {

/**
 * @brief The InfoEditorBGFX class provides an editor for displaying information
 * about clicked mesh elements (vertices, edges, or faces).
 *
 * It uses a GPU readback to identify the element under the cursor, retrieves
 * its data from the mesh provider, and overlays text and highlights on the
 * screen to provide visual feedback and data values (e.g., coordinates, IDs).
 */
template<typename ViewerDrawer>
class InfoEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    ushort mLastObjectId    = USHORT_NULL;
    ushort mLastElementType = USHORT_NULL;
    uint   mLastElementId   = 0;

    vcl::Lines  mOutlineLines;
    vcl::Points mOutlinePoints;

    vcl::TextView mTextView;
    bool          mTextViewInitialized = false;

    std::vector<vcl::Point3d> mLastElementPositions;
    std::vector<uint>         mLastElementVertexIds;

    vcl::Color mTextColor = vcl::Color::Black;
    int        mTextSize  = 20;

    InfoEditorSettings mSettings;

public:
    InfoEditorBGFX()
    {
        mOutlineLines.setGeneralColor(vcl::Color::Red);
        mOutlineLines.setWidth(5.0f);
        mOutlineLines.setTopology(vcl::Lines::Topology::LINE_STRIP);
        mOutlineLines.setDepthOffset(0.00012f);

        mOutlinePoints.setGeneralColor(vcl::Color::Red);
        mOutlinePoints.setWidth(5.0f);
        mOutlinePoints.setShape(vcl::Points::Shape::CIRCLE);
        mOutlinePoints.setDepthOffset(0.00012f);
    }

    InfoEditorSettings& settings() override { return mSettings; }

    const InfoEditorSettings& settings() const override { return mSettings; }

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
        mOutlineLines.setGeneralColor(this->settings().color);
        mOutlinePoints.setGeneralColor(this->settings().color);

        mOutlineLines.setWidth(this->settings().thickness);
        mOutlinePoints.setWidth(this->settings().thickness);

        mTextColor = this->settings().textColor;
        mTextSize  = this->settings().textSize;

        if (mTextViewInitialized) {
            auto dpi = Base::viewerDpiScale();
            mTextView.setTextFont(
                vcl::VclFont::DROID_SANS, mTextSize * dpi.x());
        }

        Base::viewerUpdate();
    }

    void drawContent(uint viewId) override
    {
        if (mLastObjectId != USHORT_NULL) {
            if (mLastElementType == vcl::MeshInfo::FACE &&
                mOutlineLines.hasPositions()) {
                mOutlineLines.draw(viewId);
            }
            else if (
                mLastElementType == vcl::MeshInfo::VERTEX &&
                mOutlinePoints.hasPositions()) {
                mOutlinePoints.draw(viewId);
            }
            else if (
                mLastElementType == vcl::MeshInfo::EDGE &&
                mOutlineLines.hasPositions()) {
                mOutlineLines.draw(viewId);
            }
        }

        auto size = Base::viewerCanvasSize();
        if (!mTextViewInitialized) {
            mTextView.init(size.x(), size.y());
            auto dpi = Base::viewerDpiScale();
            mTextView.setTextFont(
                vcl::VclFont::DROID_SANS, mTextSize * dpi.x());
            mTextView.enableText(Base::isActive());
            mTextViewInitialized = true;
        }
        else {
            mTextView.resize(size.x(), size.y());
        }

        mTextView.clearText();

        if (mLastObjectId != USHORT_NULL) {
            if (mLastElementType == vcl::MeshInfo::FACE &&
                mOutlineLines.hasPositions()) {
                drawFaceInfo(viewId, size);
            }
            else if (
                mLastElementType == vcl::MeshInfo::VERTEX &&
                mOutlinePoints.hasPositions()) {
                drawVertexInfo(viewId, size);
            }
            else if (
                mLastElementType == vcl::MeshInfo::EDGE &&
                mOutlineLines.hasPositions()) {
                drawEdgeInfo(viewId, size);
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

            // The callback receives the exact Object ID, Element Type, and
            // Element ID from the GPU readback, avoiding the need for expensive
            // CPU ray-tracing.
            auto callback = [this](
                                ushort objectId,
                                ushort elementType,
                                uint   elementId) {
                if (objectId == 0xFFFF) {
                    mLastObjectId = 0xFFFF;
                    Base::viewerUpdate();
                    return;
                }

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
                            auto vIds =
                                meshObj->meshProvider().faceVertices(elementId);

                            if (positions.size() > 0) {
                                positions.push_back(positions[0]);
                            }
                            auto T = meshObj->meshProvider().transformMatrix();
                            multiplyPointsByMatrix(positions, T);
                            mOutlineLines.setVertices(positions);
                            mLastElementPositions = positions;
                            mLastElementVertexIds = vIds;
                        }
                    }
                }
                else if (elementType == vcl::MeshInfo::VERTEX) {
                    auto list = Base::drawList();
                    if (list && objectId < list->size()) {
                        auto obj     = list->at(objectId);
                        auto meshObj = std::dynamic_pointer_cast<
                            vcl::AbstractDrawableMesh>(obj);
                        if (meshObj) {
                            auto pos = meshObj->meshProvider().vertexPosition(
                                elementId);
                            auto T = meshObj->meshProvider().transformMatrix();
                            std::vector<vcl::Point3d> positions = {pos};
                            multiplyPointsByMatrix(positions, T);
                            mOutlinePoints.setVertices(positions);
                            mLastElementPositions = positions;
                            mLastElementVertexIds = {elementId};
                        }
                    }
                }
                else if (elementType == vcl::MeshInfo::EDGE) {
                    auto list = Base::drawList();
                    if (list && objectId < list->size()) {
                        auto obj     = list->at(objectId);
                        auto meshObj = std::dynamic_pointer_cast<
                            vcl::AbstractDrawableMesh>(obj);
                        if (meshObj) {
                            auto edgeVIds =
                                meshObj->meshProvider().edgeVertices(elementId);
                            auto posPair =
                                meshObj->meshProvider().edgePositions(
                                    elementId);

                            std::vector<vcl::Point3d> positions = {
                                posPair.first, posPair.second};
                            auto T = meshObj->meshProvider().transformMatrix();
                            multiplyPointsByMatrix(positions, T);
                            mOutlineLines.setVertices(positions);
                            mLastElementPositions = positions;
                            mLastElementVertexIds = {
                                edgeVIds.first, edgeVIds.second};
                        }
                    }
                }
                else {
                    mOutlineLines.setVertices(std::vector<Point3d>());
                    mOutlinePoints.setVertices(std::vector<Point3d>());
                    mLastElementPositions.clear();
                    mLastElementVertexIds.clear();
                }

                Base::viewerUpdate();
            };

            Base::viewerReadElementIdRequest(x, y, callback);
        }
        return block;
    }

private:
    std::optional<vcl::Point2f> projectPoint(
        const vcl::Point3d&   point3d,
        const vcl::Matrix44f& viewMatrix,
        const vcl::Matrix44f& projMatrix,
        const Point2<uint>&   size) const
    {
        vcl::Point4f p(point3d.x(), point3d.y(), point3d.z(), 1.0f);
        p = projMatrix * (viewMatrix * p);

        if (p.w() != 0.0f) {
            p /= p.w();
            return vcl::Point2f(
                (p.x() + 1.0f) * 0.5f * size.x(),
                (1.0f - p.y()) * 0.5f * size.y());
        }
        return std::nullopt;
    }

    void drawFaceInfo(uint viewId, const Point2<uint>& size)
    {
        const auto& pts = mLastElementPositions;
        if (pts.size() > 1) {
            vcl::Point3d barycenter(0.0, 0.0, 0.0);
            for (size_t i = 0; i < pts.size() - 1; ++i) {
                barycenter += pts[i];
            }
            barycenter /= double(pts.size() - 1);

            vcl::Matrix44f mv   = Base::viewerViewMatrix();
            vcl::Matrix44f proj = Base::viewerProjectionMatrix();

            // Draw face info at barycenter
            if (auto pos2D = projectPoint(barycenter, mv, proj, size)) {
                std::stringstream faceSS;
                faceSS << "f#" << mLastElementId << " - v#(";
                for (size_t i = 0; i < mLastElementVertexIds.size(); ++i) {
                    faceSS << mLastElementVertexIds[i];
                    if (i < mLastElementVertexIds.size() - 1)
                        faceSS << ", ";
                }
                faceSS << ")";

                mTextView.appendTransientText(*pos2D, faceSS.str(), mTextColor);
            }

            // Draw vertex info at each vertex position
            for (size_t i = 0; i < mLastElementVertexIds.size(); ++i) {
                if (auto pos2D = projectPoint(pts[i], mv, proj, size)) {
                    std::stringstream vertSS;
                    vertSS << "fv[" << i << "] : v#" << mLastElementVertexIds[i]
                           << " - pos " << pts[i];

                    mTextView.appendTransientText(
                        *pos2D, vertSS.str(), mTextColor);
                }
            }
        }
    }

    void drawVertexInfo(uint viewId, const Point2<uint>& size)
    {
        const auto& pts = mLastElementPositions;
        if (!pts.empty() && !mLastElementVertexIds.empty()) {
            vcl::Matrix44f mv   = Base::viewerViewMatrix();
            vcl::Matrix44f proj = Base::viewerProjectionMatrix();

            if (auto pos2D = projectPoint(pts[0], mv, proj, size)) {
                std::stringstream vertSS;
                vertSS << "v#" << mLastElementVertexIds[0] << " - pos "
                       << pts[0];
                mTextView.appendTransientText(*pos2D, vertSS.str(), mTextColor);
            }
        }
    }

    void drawEdgeInfo(uint viewId, const Point2<uint>& size)
    {
        const auto& pts = mLastElementPositions;
        if (pts.size() == 2 && mLastElementVertexIds.size() == 2) {
            vcl::Matrix44f mv   = Base::viewerViewMatrix();
            vcl::Matrix44f proj = Base::viewerProjectionMatrix();

            // Draw edge info at edge center
            vcl::Point3d center = (pts[0] + pts[1]) * 0.5;
            if (auto pos2D = projectPoint(center, mv, proj, size)) {
                std::stringstream edgeSS;
                edgeSS << "e#" << mLastElementId << " - v#("
                       << mLastElementVertexIds[0] << ", "
                       << mLastElementVertexIds[1] << ")";
                mTextView.appendTransientText(*pos2D, edgeSS.str(), mTextColor);
            }

            // Draw vertex info at each vertex position
            for (size_t i = 0; i < 2; ++i) {
                if (auto pos2D = projectPoint(pts[i], mv, proj, size)) {
                    std::stringstream vertSS;
                    vertSS << "ev[" << i << "] : v#" << mLastElementVertexIds[i]
                           << " - pos " << pts[i];
                    mTextView.appendTransientText(
                        *pos2D, vertSS.str(), mTextColor);
                }
            }
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_INFO_EDITOR_BGFX_H
