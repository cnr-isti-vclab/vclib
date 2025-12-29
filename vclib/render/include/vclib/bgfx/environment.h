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
#include <bx/allocator.h>
#include <vclib/base.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/bgfx/texture.h>

namespace vcl {

class Environment
{

    std::string mImagePath;

    vcl::VertexBuffer mVertexBuffer;
    static const uint mVertexNumber = 3;
    inline static const float mVertices[mVertexNumber * 3] {
        -3, -1,  1,
         1, -1,  1,
         1,  3,  1
    };

    bool 
        mBackgroundReady = false,
        mCanDraw         = false;

    uint32_t 
        mCubeSide           = 0,
        mIrradianceCubeSide = 0,
        mSpecularCubeSide   = 0,
        mBrdfLutSize        = 1024;

    uint8_t 
        mCubeMips     = 0,
        mSpecularMips = 0;

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

    enum class FileFormat
    {
        UNKNOWN,
        HDR,
        EXR,
        KTX,
        DDS
    };

    FileFormat mSourceFormat = FileFormat::UNKNOWN;

    class AlignedAllocator : public bx::AllocatorI
    {
    public:
    	AlignedAllocator(bx::AllocatorI* _allocator, size_t _minAlignment)
    		: m_allocator(_allocator)
    		, m_minAlignment(_minAlignment)
    	{
    	}

    	virtual void* realloc(
    			void* _ptr
    		, size_t _size
    		, size_t _align
    		, const char* _file
    		, uint32_t _line
    		)
    	{
    		return m_allocator->realloc(_ptr, _size, bx::max(_align, m_minAlignment), _file, _line);
    	}

    	bx::AllocatorI* m_allocator;
    	size_t m_minAlignment;
    };

    // Allocator references are stored in image containers
    // so they have to remain visible somehow.
    // TODO: find a better way to do so.
    inline static bx::DefaultAllocator bxDefaultAllocator;
    inline static AlignedAllocator bxAlignedAllocator = AlignedAllocator(&bxDefaultAllocator, 16);

    bimg::ImageContainer* mImage = nullptr;

    public:

    Environment() = default;

    Environment(const std::string& imagePath):
        mImagePath(imagePath)
    {}

    Environment(const Environment& other) = delete;

    Environment(Environment&& other) { swap(other); }

    ~Environment() = default;

    Environment& operator=(Environment&& other)
    {
        swap(other);
        return *this;
    }

    void swap(Environment& other)
    {
        using std::swap;
        swap(mBackgroundReady, other.mBackgroundReady);
        swap(mCanDraw, other.mCanDraw);
        swap(mCubeSide, other.mCubeSide);
        swap(mIrradianceCubeSide, other.mIrradianceCubeSide);
        swap(mSpecularCubeSide, other.mSpecularCubeSide);
        swap(mBrdfLutSize, other.mBrdfLutSize);
        swap(mCubeMips, other.mCubeMips);
        swap(mSpecularMips, other.mSpecularMips);
        swap(mSourceFormat, other.mSourceFormat);
        swap(mImage, other.mImage);
        swap(mHdrTexture, other.mHdrTexture);
        swap(mCubeMapTexture, other.mCubeMapTexture);
        swap(mIrradianceTexture, other.mIrradianceTexture);
        swap(mSpecularTexture, other.mSpecularTexture);
        swap(mBrdfLuTexture, other.mBrdfLuTexture);
        mImagePath.swap(other.mImagePath);
        mVertexBuffer.swap(other.mVertexBuffer);
    }

    friend void swap(Environment& first, Environment& second)
    {
        first.swap(second);
    }

    enum class TextureType
    {
        RAW_CUBE,
        IRRADIANCE,
        SPECULAR,
        BRDF_LUT
    };

    void drawBackground(const uint viewId);

    void bindTexture(TextureType type, uint stage, uint samplerFlags = BGFX_SAMPLER_UVW_CLAMP) const;

    void bindDataUniform(const float d0 = 0.0f, const float d1 = 0.0f, const float d2 = 0.0f, const float d3 = 0.0f) const;

    bool canDraw() const { return mCanDraw; }

    uint8_t specularMips() const { return mSpecularMips; }

    private:

    void prepareBackground(const uint viewId);

    FileFormat getFileFormat(const std::string& imagePath);

    bimg::ImageContainer* loadImage(std::string imagePath);

    void setTextures();

    void fullScreenTriangle();

    void generateTextures(const uint viewId);

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
