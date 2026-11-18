// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_SHOW_NORMALS_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_SHOW_NORMALS_EDITOR_BGFX_H

#include <vclib/bgfx/drawable/drawable_lines.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/editors/editor.h>
#include <vclib/render/settings/show_normals_editor_settings.h>

#include <vclib/algorithms/core.h>

#include <nlohmann/json.hpp>

namespace vcl {

/**
 * @brief Editor that draws the normals of the visible objects using BGFX.
 *
 * @tparam ViewerDrawer: the type of the viewer drawer.
 */
template<typename ViewerDrawer>
class ShowNormalsEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    std::vector<DrawableLines> mVertexNormalsLines;
    std::vector<DrawableLines> mFaceNormalsLines;

    ShowNormalsEditorSettings mSettings;

    bool mDirty = true; // Indicates whether normal lines need updating

public:
    ShowNormalsEditorBGFX() = default;

    std::string name() const override { return "Show Normals"; }

    void onViewerSet() override
    {
        Base::viewerRegisterGlobalAction(
            "Toggle Show Normals",
            {Key::N, {KeyModifier::NO_MODIFIER}},
            [this]() { this->setActive(!this->isActive()); });
    }

    ShowNormalsEditorSettings& settings() override { return mSettings; }

    const ShowNormalsEditorSettings& settings() const override
    {
        return mSettings;
    }

    void loadSettings(const nlohmann::json& j) override
    {
        mSettings.loadSettings(j);
    }

    void saveSettings(nlohmann::json& j) const override
    {
        mSettings.saveSettings(j);
    }

    void setActive(bool active) override
    {
        Base::setActive(active);
        // Lazily allocate/synchronize normal lines only when activated
        if (active && mDirty) {
            updateNormalLines();
        }
        Base::viewerUpdate();
    }

    void refresh() override
    {
        // When inactive, release GPU buffers immediately and defer updates
        if (!Base::isActive()) {
            mVertexNormalsLines.clear();
            mFaceNormalsLines.clear();
            mDirty = true;
            return;
        }

        updateNormalLines();
    }

    void updateNormalLines()
    {
        const uint currentSize = mVertexNormalsLines.size();
        const uint targetSize  = Base::drawList()->size();

        // 1. Shrink if objects were removed
        if (currentSize > targetSize) {
            mVertexNormalsLines.resize(targetSize);
            mFaceNormalsLines.resize(targetSize);
        }

        // 2. Incremental update for existing objects: compare linesCount()
        // against expected vertex/face count. If matched, existing GPU buffers
        // are preserved without churn.
        const uint commonCount = std::min(currentSize, targetSize);
        for (uint i = 0; i < commonCount; ++i) {
            const auto&                 drawable = Base::drawList()->at(i);
            const AbstractDrawableMesh* m =
                dynamic_cast<const AbstractDrawableMesh*>(drawable.get());

            if (m) {
                const auto& provider = m->meshProvider();
                uint        expectedVLines =
                    provider.hasVertexNormals() ? provider.vertexCount() : 0;
                uint expectedFLines =
                    provider.hasFaceNormals() ? provider.faceCount() : 0;

                if (mVertexNormalsLines[i].linesCount() != expectedVLines) {
                    mVertexNormalsLines[i] = vertexNormalLines(provider);
                }
                if (mFaceNormalsLines[i].linesCount() != expectedFLines) {
                    mFaceNormalsLines[i] = faceNormalLines(provider);
                }
            }
            else {
                if (mVertexNormalsLines[i].linesCount() > 0) {
                    mVertexNormalsLines[i] = DrawableLines();
                }
                if (mFaceNormalsLines[i].linesCount() > 0) {
                    mFaceNormalsLines[i] = DrawableLines();
                }
            }
        }

        // 3. Append normal lines for newly added objects
        mVertexNormalsLines.reserve(targetSize);
        mFaceNormalsLines.reserve(targetSize);

        for (uint i = commonCount; i < targetSize; ++i) {
            const auto&                 drawable = Base::drawList()->at(i);
            const AbstractDrawableMesh* m =
                dynamic_cast<const AbstractDrawableMesh*>(drawable.get());
            if (m) {
                const auto& provider = m->meshProvider();
                mVertexNormalsLines.push_back(vertexNormalLines(provider));
                mFaceNormalsLines.push_back(faceNormalLines(provider));
            }
            else {
                mVertexNormalsLines.emplace_back();
                mFaceNormalsLines.emplace_back();
            }
        }

        mDirty = false;
        refreshSettings();
    }

    void refreshSettings() override
    {
        uint i = 0;
        for (const auto& drawable : *Base::drawList()) {
            const AbstractDrawableMesh* m =
                dynamic_cast<const AbstractDrawableMesh*>(drawable.get());
            if (m && i < mVertexNormalsLines.size()) {
                double diag       = m->boundingBox().diagonal();
                double lineLength = diag * mSettings.normalLengthRatio;

                mVertexNormalsLines[i].setGeneralColor(
                    mSettings.vertexNormalColor);
                mVertexNormalsLines[i].setWidth(mSettings.thickness);
                mVertexNormalsLines[i].setVectorLength(lineLength);

                mFaceNormalsLines[i].setGeneralColor(mSettings.faceNormalColor);
                mFaceNormalsLines[i].setWidth(mSettings.thickness);
                mFaceNormalsLines[i].setVectorLength(lineLength);
            }
            i++;
        }
        Base::viewerUpdate();
    }

    void drawContent(uint viewId) override
    {
        // Ensure normal lines are synchronized before drawing
        if (mDirty) {
            updateNormalLines();
        }

        using enum EditorSettings::EditMode;

        DrawObjectSettings settings;
        settings.viewId = viewId;

        if (mSettings.editMode == NONE)
            return;

        if (mSettings.editMode == CURRENT_OBJECT) {
            uint id = Base::drawList()->selectedObjectId();
            if (id < mVertexNormalsLines.size()) {
                auto m = dynamic_cast<AbstractDrawableMesh*>(
                    Base::drawList()->at(id).get());
                if (m) {
                    vcl::Matrix44f model =
                        m->meshProvider().transformMatrix().cast<float>();
                    if (mSettings.showVertexNormals) {
                        bgfx::setTransform(model.data());
                        mVertexNormalsLines[id].draw(settings);
                    }
                    if (mSettings.showFaceNormals) {
                        bgfx::setTransform(model.data());
                        mFaceNormalsLines[id].draw(settings);
                    }
                }
            }
        }
        else {
            for (uint i = 0; i < mVertexNormalsLines.size(); ++i) {
                bool show = (mSettings.editMode == VISIBLE_OBJECTS) ?
                                Base::drawList()->at(i)->isVisible() :
                                true;

                if (show) {
                    auto m = dynamic_cast<AbstractDrawableMesh*>(
                        Base::drawList()->at(i).get());
                    if (m) {
                        vcl::Matrix44f model =
                            m->meshProvider().transformMatrix().cast<float>();
                        if (mSettings.showVertexNormals) {
                            bgfx::setTransform(model.data());
                            mVertexNormalsLines[i].draw(settings);
                        }
                        if (mSettings.showFaceNormals) {
                            bgfx::setTransform(model.data());
                            mFaceNormalsLines[i].draw(settings);
                        }
                    }
                }
            }
        }
    }

private:
    DrawableLines vertexNormalLines(const AbstractMeshProvider& provider)
    {
        DrawableLines vLines;
        if (provider.hasVertexNormals()) {
            uint                      vCount = provider.vertexCount();
            std::vector<vcl::Point3d> pts;
            pts.reserve(vCount * 2);

            // Compute normals and lines for each vertex.
            // Using a zero vector for degenerate normals guarantees
            // linesCount() == vCount for reliable synchronization without
            // rendering artifacts.
            for (uint i = 0; i < vCount; ++i) {
                vcl::Point3d pos = provider.vertexPosition(i);
                vcl::Point3d n   = provider.vertexNormal(i);
                if (n.squaredNorm() > 0) {
                    n.normalize();
                }
                else {
                    n = vcl::Point3d(0, 0, 0);
                }
                pts.push_back(pos);
                pts.push_back(n); // Vector direction
            }
            vLines.setVertices(pts);
            vLines.setTopology(DrawableLines::Topology::VECTORS);
        }

        return vLines;
    }

    DrawableLines faceNormalLines(const AbstractMeshProvider& provider)
    {
        DrawableLines fLines;
        if (provider.hasFaceNormals()) {
            uint                      fCount = provider.faceCount();
            std::vector<vcl::Point3d> pts;
            pts.reserve(fCount * 2);

            // Compute normals and lines for each face.
            // Using a zero vector for degenerate normals guarantees
            // linesCount() == fCount for reliable synchronization without
            // rendering artifacts.
            for (uint i = 0; i < fCount; ++i) {
                vcl::Point3d barycenter = provider.faceBarycenter(i);
                vcl::Point3d n          = provider.faceNormal(i);
                if (n.squaredNorm() > 0) {
                    n.normalize();
                }
                else {
                    n = vcl::Point3d(0, 0, 0);
                }
                pts.push_back(barycenter);
                pts.push_back(n); // Vector direction
            }
            fLines.setVertices(pts);
            fLines.setTopology(DrawableLines::Topology::VECTORS);
        }

        return fLines;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_SHOW_NORMALS_EDITOR_BGFX_H
