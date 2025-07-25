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

#include "mesh_render_buffers_979.h"
#include <imgui.h>
#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/render/drawers/plain_drawer.h>

#include <algorithm>
#include <vector>

#include <bgfx/bgfx.h>

template<typename DerivedDrawer>
class ImGuiTextureGetter979 : public vcl::PlainDrawer<DerivedDrawer>
{
    bool mViewIdInit = false;
    bgfx::ViewId mOffScreenId;

    vcl::uint mVertNum;

    uint64_t mCurrentFrame = 0;

    uint8_t* read_to = NULL;
    uint32_t read_to_size;

    bool     mAwaitingRead = false;
    bool     mAvailable    = false;
    bool     mStringValid  = false;
    uint32_t mAvailabilityWait;

    vcl::Box3d bbox;

    vcl::MeshRenderBuffers979<vcl::TriMesh>* mMRB;

    std::string mString;

public:
    using vcl::PlainDrawer<DerivedDrawer>::PlainDrawer;
    using Base = vcl::PlainDrawer<DerivedDrawer>;

    void setMRB(const vcl::MeshRenderBuffers979<vcl::TriMesh>* mrb) { 
        mMRB = const_cast<vcl::MeshRenderBuffers979<vcl::TriMesh>*>(mrb);
        mVertNum = mMRB->vertexNumber();
    }

    void setBbox(const vcl::Box3d& box) { bbox = box; }

    void onDraw(vcl::uint viewId)
    {
        Base::onDraw(viewId);

        if(!mViewIdInit) {
            auto &ctx = vcl::Context::instance();
            //mOffScreenId = ctx.requestViewId();
            mViewIdInit = true;
        }

        if(read_to == NULL) {
            std::array<uint16_t, 2> texSize = mMRB->getSelectionTexSize();
            bgfx::TextureInfo texInfo;
            bgfx::calcTextureSize(
                texInfo,
                texSize[0],
                texSize[1],
                1,
                false,
                false,
                1,
                bgfx::TextureFormat::RGBA8
            );
            read_to_size = texInfo.storageSize;
            read_to = (uint8_t*)malloc(read_to_size);
        }

        mCurrentFrame++;
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Read texture button", nullptr);
        if (ImGui::Button("Calculate selection and read texture")) {
            mAwaitingRead = true;
            mAvailable    = false;
            mStringValid  = false;
            mMRB->calculateSelection(viewId, viewId, bbox);
            mAvailabilityWait =
                bgfx::readTexture(mMRB->getReadBackTexture(), (void*) read_to);
            mString = "";
        }
        if (mAwaitingRead) {
            if(mAvailabilityWait == 0 || mCurrentFrame >= mAvailabilityWait) {
                mAwaitingRead = false;
                mAvailable    = true;
            }
        }
        if (mAvailable) {
            if (!mStringValid) {
                for (vcl::uint index = 0; index < read_to_size / 4; index++) {
                    if (index >= mVertNum) {
                        break;
                    }
                    mString += std::to_string(index) + std::string(": ") +
                               std::to_string(read_to[4 * index]) +
                               std::string(", ") +
                               std::to_string(read_to[4 * index + 1]) +
                               std::string(", ") +
                               std::to_string(read_to[4 * index + 2]) +
                               std::string(", ") +
                               std::to_string(read_to[4 * index + 3]) +
                               std::string("\n");
                }
                mStringValid = true;
            }
            ImGui::TextUnformatted(mString.c_str());
        }
        ImGui::End();
    }

    ~ImGuiTextureGetter979() {
        free(read_to);
    }
};

#endif
