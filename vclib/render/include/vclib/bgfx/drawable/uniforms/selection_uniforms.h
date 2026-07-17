// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_SELECTION_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_SELECTION_UNIFORMS_H

#include <vclib/render/selection/selection_mode.h>

#include <vclib/bgfx/uniform.h>
#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief The SelectionUniforms class is responsible for managing the shader
 * uniforms related to mesh selection.
 *
 * It provides a static interface to set the uniform data based on the
 * current selection data and to bind the uniforms to the shader programs.
 */
class SelectionUniforms
{
    static inline std::array<float, 4> sSelectionBox;
    static inline std::array<float, 4> sSelectionWorkgroupSizeAndCount;
    static inline std::array<float, 4> sMeshIDSelectionActionData;

    static inline Uniform sSelectionBoxUniform;
    static inline Uniform sSelectionWorkgroupSizeAndCountUniform;
    static inline Uniform sMeshIDDataUniform;

public:
    SelectionUniforms() = delete;

    static void setSelectionBox(const Box2d& box)
    {
        if (box.isNull()) {
            sSelectionBox = {0.0, 0.0, 0.0, 0.0};
        }
        else {
            sSelectionBox[0] = box.min().x();
            sSelectionBox[1] = box.min().y();
            sSelectionBox[2] = box.max().x();
            sSelectionBox[3] = box.max().y();
        }
    }

    static void setSelectionWorkgroupSize(const std::array<uint, 3>& size)
    {
        sSelectionWorkgroupSizeAndCount[0] =
            std::bit_cast<float>(size[0]); // workgroup size x
        sSelectionWorkgroupSizeAndCount[1] =
            std::bit_cast<float>(size[1]); // workgroup size y
        sSelectionWorkgroupSizeAndCount[2] =
            std::bit_cast<float>(size[2]); // workgroup size z
    }

    static void setNumPrimitivesForSelection(uint numPrimitives)
    {
        sSelectionWorkgroupSizeAndCount[3] =
            std::bit_cast<float>(numPrimitives);
    }

    static void setMeshIdForSelection(uint meshId)
    {
        sMeshIDSelectionActionData[0] = std::bit_cast<float>(meshId);
    }

    /**
     * @brief Set the selection operation type.
     *
     * @param[in] action
     */
    static void setSelectionAction(SelectionAction action)
    {
        float val = action == SelectionAction::SUBTRACT ? 1.0 : 0.0;
        sMeshIDSelectionActionData[1] = val;
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sSelectionBoxUniform.isValid())
            sSelectionBoxUniform =
                Uniform("u_selectionBox", bgfx::UniformType::Vec4);
        if (!sSelectionWorkgroupSizeAndCountUniform.isValid())
            sSelectionWorkgroupSizeAndCountUniform =
                Uniform("u_workgroupSizeAndCount", bgfx::UniformType::Vec4);
        if (!sMeshIDDataUniform.isValid())
            sMeshIDDataUniform =
                Uniform("u_meshIdSelectionActionData", bgfx::UniformType::Vec4);

        sSelectionBoxUniform.bind(&sSelectionBox);
        sSelectionWorkgroupSizeAndCountUniform.bind(
            &sSelectionWorkgroupSizeAndCount);
        sMeshIDDataUniform.bind(&sMeshIDSelectionActionData);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_SELECTION_UNIFORMS_H
