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

#ifndef IMGUI_TEXTURE_GETTER_979
#define IMGUI_TEXTURE_GETTER_979

#include <imgui.h>

#include <vclib/bgfx/drawable/drawable_mesh_979.h>
#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/selection/selection_mode.h>

#include <algorithm>
#include <vector>

#include <bgfx/bgfx.h>

template<typename MeshType> 
class ImguiTextureGetter979Wrapper {
public:
    template<typename DerivedDrawer>
    class ImGuiTextureGetter979 : public vcl::PlainDrawer<DerivedDrawer>
    {
        std::shared_ptr<const vcl::DrawableMeshBGFX979<MeshType>> msh;
        vcl::SelectionMode mode = vcl::SelectionMode::VERTEX_REGULAR;



    public:
        using vcl::PlainDrawer<DerivedDrawer>::PlainDrawer;
        using Base = vcl::PlainDrawer<DerivedDrawer>;

        void setMesh(const vcl::DrawableObjectVector& dov, size_t index) {
            this->msh = std::dynamic_pointer_cast<const vcl::DrawableMeshBGFX979<MeshType>>(dov.at(index));
        }

        void onDraw(vcl::uint viewId)
        {
            Base::onDraw(viewId);

            ImGuiIO& io = ImGui::GetIO();
            ImGui::Begin("Calculate selection button", nullptr);
            ImGui::RadioButton("Regular", [this]() -> bool {return this->mode == vcl::SelectionMode::VERTEX_REGULAR;}, [this](bool b){this->mode = vcl::SelectionMode::VERTEX_REGULAR;});
            ImGui::SameLine();
            ImGui::RadioButton("Add", [this]() -> bool {return this->mode == vcl::SelectionMode::VERTEX_ADD;}, [this](bool b){this->mode = vcl::SelectionMode::VERTEX_ADD;});
            ImGui::SameLine();
            ImGui::RadioButton("Subtract", [this]() -> bool {return this->mode == vcl::SelectionMode::VERTEX_SUBTRACT;}, [this](bool b){this->mode = vcl::SelectionMode::VERTEX_SUBTRACT;});
            ImGui::SameLine();
            ImGui::RadioButton("Invert", [this]() -> bool {return this->mode == vcl::SelectionMode::VERTEX_INVERT;}, [this](bool b){this->mode = vcl::SelectionMode::VERTEX_INVERT;});
            ImGui::SameLine();
            ImGui::RadioButton("All", [this]() -> bool {return this->mode == vcl::SelectionMode::VERTEX_ALL;}, [this](bool b){this->mode = vcl::SelectionMode::VERTEX_ALL;});
            ImGui::SameLine();
            ImGui::RadioButton("None", [this]() -> bool {return this->mode == vcl::SelectionMode::VERTEX_NONE;}, [this](bool b){this->mode = vcl::SelectionMode::VERTEX_NONE;});
            if (ImGui::Button("Calculate selection")) {
                msh->setTransform();
                msh->getMRB().calculateSelection(viewId, mode);
            }
            ImGui::End();
        }
    };
};

#endif
