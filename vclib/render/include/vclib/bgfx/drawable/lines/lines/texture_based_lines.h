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

#ifndef VCL_BGFX_DRAWABLE_LINES_LINES_TEXTURE_BASED_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_LINES_TEXTURE_BASED_LINES_H

#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {

class TextureBasedLines : public DrawableLines
{
    bgfx::ProgramHandle mComputeTexturePH =
        Context::instance().programManager().getProgram(
            VclProgram::LINES_TEXTURE_BASED_CS);
    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::LINES_TEXTURE_BASED_VSFS);

    static inline const std::vector<float> mVertices =
        {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    static inline const std::vector<uint32_t> mIndexes = {0, 1, 2, 1, 3, 2};

    uint32_t                 mMaxTextureSize;
    std::vector<LinesVertex> mPoints;

    bgfx::TextureHandle             mTextureBH = BGFX_INVALID_HANDLE;
    bgfx::DynamicVertexBufferHandle mPointsBH  = BGFX_INVALID_HANDLE;

    bgfx::IndirectBufferHandle mIndirectBH     = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle        mIndirectDataUH = BGFX_INVALID_HANDLE;

    bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  mIndexesBH  = BGFX_INVALID_HANDLE;

public:
    TextureBasedLines() = default;

    TextureBasedLines(
        const std::vector<LinesVertex>& points,
        const uint32_t                  maxTextureSize);

    TextureBasedLines(const TextureBasedLines& other);

    TextureBasedLines(TextureBasedLines&& other);

    ~TextureBasedLines();

    TextureBasedLines& operator=(TextureBasedLines other);

    void swap(TextureBasedLines& other);

    std::shared_ptr<vcl::DrawableObject> clone() const override;

    void draw(uint viewId) const override;

    void update(const std::vector<LinesVertex>& points) override;

private:
    void generateTextureBuffer();

    void allocateTextureBuffer();

    void allocatePointsBuffer();

    void allocateVerticesBuffer();

    void allocateIndexesBuffer();
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_LINES_TEXTURE_BASED_LINES_H
