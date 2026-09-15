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

public:
    ShowNormalsEditorBGFX() = default;

    std::string name() const override { return "Show Normals"; }

    void onViewerSet() override
    {
        Base::viewerRegisterGlobalAction(
            "Toggle Show Normals",
            {Key::N, {KeyModifier::NO_MODIFIER}},
            [this]() {
                this->setActive(!this->isActive());
            });
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
        Base::viewerUpdate();
    }

    void refresh() override
    {
        mVertexNormalsLines.clear();
        mFaceNormalsLines.clear();

        for (const auto& drawable : *Base::drawList()) {
            const AbstractDrawableMesh* m =
                dynamic_cast<const AbstractDrawableMesh*>(drawable.get());
            if (m) {
                const auto& provider = m->meshProvider();

                // Vertex Normals
                mVertexNormalsLines.push_back(vertexNormalLines(provider));

                // Face Normals
                mFaceNormalsLines.push_back(faceNormalLines(provider));
            }
            else {
                mVertexNormalsLines.push_back(DrawableLines());
                mFaceNormalsLines.push_back(DrawableLines());
            }
        }

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
        using enum EditorSettings::EditMode;

        DrawObjectSettings settings;
        settings.viewId = viewId;

        if (mSettings.editMode == NONE)
            return;

        if (mSettings.editMode == CURRENT_OBJECT) {
            uint id = Base::drawList()->selectedObjectId();
            if (id < mVertexNormalsLines.size()) {
                if (mSettings.showVertexNormals)
                    mVertexNormalsLines[id].draw(settings);
                if (mSettings.showFaceNormals)
                    mFaceNormalsLines[id].draw(settings);
            }
        }
        else {
            for (uint i = 0; i < mVertexNormalsLines.size(); ++i) {
                bool show = (mSettings.editMode == VISIBLE_OBJECTS) ?
                                Base::drawList()->at(i)->isVisible() :
                                true;

                if (show) {
                    if (mSettings.showVertexNormals)
                        mVertexNormalsLines[i].draw(settings);
                    if (mSettings.showFaceNormals)
                        mFaceNormalsLines[i].draw(settings);
                }
            }
        }
    }

private:
    DrawableLines vertexNormalLines(const AbstractMeshProvider& provider)
    {
        DrawableLines vLines;
        if (provider.hasVertexNormals()) {
            std::vector<vcl::Point3d> pts;
            uint                      vCount = provider.vertexCount();
            vcl::Matrix44d            T      = provider.transformMatrix();

            // Compute normals and lines for each vertex
            for (uint i = 0; i < vCount; ++i) {
                vcl::Point3d pos = provider.vertexPosition(i);
                vcl::Point3d n   = provider.vertexNormal(i);
                if (n.squaredNorm() > 0) {
                    pos *= T;
                    n = vcl::multiplyNormalByMatrix(n, T);
                    n.normalize();
                    pts.push_back(pos);
                    pts.push_back(n); // Vector direction
                }
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
            std::vector<vcl::Point3d> pts;
            uint                      fCount = provider.faceCount();
            vcl::Matrix44d            T      = provider.transformMatrix();

            // Compute normals and lines for each face
            for (uint i = 0; i < fCount; ++i) {
                vcl::Point3d barycenter = provider.faceBarycenter(i);
                vcl::Point3d n          = provider.faceNormal(i);
                if (n.squaredNorm() > 0) {
                    barycenter *= T;
                    n = vcl::multiplyNormalByMatrix(n, T);
                    n.normalize();
                    pts.push_back(barycenter);
                    pts.push_back(n); // Vector direction
                }
            }
            fLines.setVertices(pts);
            fLines.setTopology(DrawableLines::Topology::VECTORS);
        }

        return fLines;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_SHOW_NORMALS_EDITOR_BGFX_H
