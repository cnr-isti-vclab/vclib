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

#ifndef VCL_BGFX_ENVIRONMENT_H
#define VCL_BGFX_ENVIRONMENT_H

#include <bgfx/bgfx.h>
#include <vclib/base.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/bgfx/texture.h>
#include <vclib/render/drawable/draw_object_settings.h>

namespace vcl {

class Environment
{
    vcl::VertexBuffer mVertexBuffer;
    static const uint mVertexNumber = 3;
    inline static const float mVertices[mVertexNumber * 3] {
        -3, -1,  1,
         1, -1,  1,
         1,  3,  1
    };

    bool mBackgroundReady = false;

    std::array<float, 4> mUniformData = {0.0f, 0.0f, 0.0f, 0.0f};
    uint8_t mSpecularMips = 0;

    vcl::Uniform 
        mHdrSamplerUniform            = Uniform("s_hdr", bgfx::UniformType::Sampler),
        mEnvCubeSamplerUniform        = Uniform("s_env0", bgfx::UniformType::Sampler),
        mIrradianceCubeSamplerUniform = Uniform("s_irradiance", bgfx::UniformType::Sampler),
        mSpecularCubeSamplerUniform   = Uniform("s_specular", bgfx::UniformType::Sampler),
        mBrdfLutSamplerUniform        = Uniform("s_brdf_lut", bgfx::UniformType::Sampler),
        mDataUniform                  = Uniform("u_dataPack", bgfx::UniformType::Vec4);

    std::unique_ptr<Texture> 
        mHdrTexture, 
        mCubeMapTexture, 
        mIrradianceTexture,
        mSpecularTexture,
        mBrdfLuTexture;

    public:

    Environment() = default;

    Environment(const Environment& other) = default;

    Environment(Environment&& other) { swap(other); }

    ~Environment() = default;

    Environment& operator=(Environment other)
    {
        swap(other);
        return *this;
    }

    void swap(Environment& other)
    {
        using std::swap;
        Environment::swap(other);

        mVertexBuffer.swap(other.mVertexBuffer);
    }

    friend void swap(Environment& first, Environment& second)
    {
        first.swap(second);
    }

    void drawBackground(const DrawObjectSettings& settings);

    private:

    void prepareBackground();

    template<typename T>
    std::pair<T*, bgfx::ReleaseFn> getAllocatedBufferAndReleaseFn(uint size)
    {
        T* buffer = new T[size];

        return std::make_pair(buffer, [](void* ptr, void*) {
            delete[] static_cast<T*>(ptr);
        });
    }
};

} // namespace vcl

#endif // VCL_BGFX_ENVIRONMENT_H