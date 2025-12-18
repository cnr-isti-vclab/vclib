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

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/readerwriter.h>
#include <bx/math.h>

#include <bimg/bimg.h>
#include <bimg/decode.h>

#include <vclib/bgfx/environment.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

// needed for non power of two textures
#define CEIL_DIV(x, d) ((x) / (d) + ((x) % (d) != 0))

namespace vcl {

void Environment::drawBackground(const uint viewId)
{
    prepareBackground(viewId);

    if(!mCanDraw)
        return;

    using enum VertFragProgram;
    ProgramManager& pm = Context::instance().programManager();

    using enum TextureType;
    bindTexture(RAW_CUBE, VCL_MRB_CUBEMAP0);

    mVertexBuffer.bindVertex(0);

    bgfx::setState(BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LEQUAL);

    bgfx::submit(
        viewId, 
        pm.getProgram<DRAWABLE_BACKGROUND_PBR>()
    );
}

void Environment::bindTexture(TextureType type, uint stage, uint samplerFlags) const
{
    if(!mCanDraw)
        return;


    using enum TextureType;
    switch(type)
    {
        case RAW_CUBE:
        {
            mCubeMapTexture->bind(
                stage,
                mEnvCubeSamplerUniform.handle(),
                samplerFlags
            );
            break;
        }
        case IRRADIANCE:
        {
            mIrradianceTexture->bind(
                stage,
                mIrradianceCubeSamplerUniform.handle(),
                samplerFlags
            );
            break;
        }
        case SPECULAR:
        {
            mSpecularTexture->bind(
                stage,
                mSpecularCubeSamplerUniform.handle(),
                samplerFlags
            );
            break;
        }    
        case BRDF_LUT:
        {
            mBrdfLuTexture->bind(
                stage,
                mBrdfLutSamplerUniform.handle(),
                samplerFlags
            );
            break;
        }
    }
}

void Environment::bindDataUniform(const float d0, const float d1, const float d2, const float d3) const
{
    if(!mCanDraw)
        return;

    std::array<float, 4> data = {d0, d1, d2, d3};
    mDataUniform.bind(&data);
}

void Environment::prepareBackground(const uint viewId)
{
    if(mBackgroundReady) 
        return;

    mImage = loadImage(mImagePath);
    if(mImage == nullptr)
    {
        mBackgroundReady = true;
        mCanDraw = false;
        return;
    }

    setTextures();

    generateTextures(viewId);

    fullScreenTriangle();
    
    mBackgroundReady = true;
    mCanDraw = true;
}

Environment::FileFormat Environment::getFileFormat(const std::string& imagePath)
{
    using enum Environment::FileFormat;
    if(imagePath.find(".hdr", imagePath.length() - 4) != std::string::npos)
        return HDR;
    if(imagePath.find(".exr", imagePath.length() - 4) != std::string::npos)
        return EXR;
    if(imagePath.find(".ktx", imagePath.length() - 4) != std::string::npos)
        return KTX;
    return UNKNOWN;
}

bimg::ImageContainer* Environment::loadImage(std::string imagePath)
{
    /* Code from bimg texturec */

    using enum Environment::FileFormat;
    mSourceFormat = getFileFormat(mImagePath);

    if(mSourceFormat == UNKNOWN) 
        return nullptr;

	bx::Error err;
    bx::FileReader reader;

	// open the file

    if(!bx::open(&reader, imagePath.c_str(), &err))
        return nullptr;

	// read file size and allocate memory

    uint32_t inputSize = (uint32_t)bx::getSize(&reader);

    if(inputSize == 0)
		return nullptr;

	uint8_t* inputData = (uint8_t*)bx::alloc(&bxAlignedAllocator, inputSize);

	// read the file and put it raw in inputData

	bx::read(&reader, inputData, inputSize, &err);
	bx::close(&reader);

	// copy the data in the final container reading its characteristics

    using enum bimg::TextureFormat::Enum;

	bimg::ImageContainer* output  = bimg::imageParse(&bxAlignedAllocator, inputData, inputSize, Count, &err); 

	bx::free(&bxAlignedAllocator, inputData);

    if(
        !err.isOk() || 
        (
            !output->m_cubeMap   &&
            mSourceFormat != HDR && 
            mSourceFormat != EXR
        )
    ) // file is neither a cubemap nor an equirectangular map
    {
        bimg::imageFree(output);
        return nullptr;
    }

	return output;
}

void Environment::setTextures()
{
    // if it's not a cubemap it's equirectangular
    mCubeSide = mImage->m_cubeMap? mImage->m_width : CEIL_DIV(mImage->m_width, 4);
    mCubeMips = bimg::imageGetNumMips(
        bimg::TextureFormat::RGBA32F,
        mCubeSide,
        mCubeSide
    );

    mIrradianceCubeSide = CEIL_DIV(mCubeSide, 4);

    mSpecularCubeSide = CEIL_DIV(mCubeSide, 4);
    mSpecularMips = bimg::imageGetNumMips(
        bimg::TextureFormat::RGBA32F,
        mSpecularCubeSide,
        mSpecularCubeSide
    );

    if(!mImage->m_cubeMap) // equirect
    {
        auto hdrTexture = std::make_unique<Texture>();
        hdrTexture->set(
            mImage->m_data,
            Point2i(mImage->m_width, mImage->m_height),
            false,
            BGFX_TEXTURE_NONE,
            bgfx::TextureFormat::RGBA32F
        );
        mHdrTexture = std::move(hdrTexture);
    }

    const bool cubemapHasAlreadyMips = 
        mImage->m_cubeMap && 
        mImage->m_numMips > 1;
        
    const uint64_t cubemapTextureFlags = 
        cubemapHasAlreadyMips?
        // has already mips, no work to do
        BGFX_TEXTURE_NONE :
        // otherwise generate mips
        BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT;

    auto cubemapTexture = std::make_unique<Texture>();
    cubemapTexture->set(
        mImage->m_cubeMap? mImage->m_data : nullptr,
        Point2i(mCubeSide, mCubeSide),
        true,   // has mips
        cubemapTextureFlags,
        bgfx::TextureFormat::RGBA32F,
        true    // is cubemap
    );
    mCubeMapTexture = std::move(cubemapTexture);

    auto irradianceTexture = std::make_unique<Texture>();
    irradianceTexture->set(
        nullptr,
        Point2i(mIrradianceCubeSide, mIrradianceCubeSide),
        false, // has mips
        BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
        bgfx::TextureFormat::RGBA32F,
        true // is cubemap
    );
    mIrradianceTexture = std::move(irradianceTexture);

    auto specularTexture = std::make_unique<Texture>();
    specularTexture->set(
        nullptr,
        Point2i(mSpecularCubeSide, mSpecularCubeSide),
        true, // has mips
        BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
        bgfx::TextureFormat::RGBA32F,
        true // is cubemap
    );
    mSpecularTexture = std::move(specularTexture);

    auto brdfLuTexture = std::make_unique<Texture>();
    brdfLuTexture->set(
        nullptr,
        Point2i(mBrdfLutSize, mBrdfLutSize),
        false,
        BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
        bgfx::TextureFormat::RGBA32F
    );
    mBrdfLuTexture = std::move(brdfLuTexture);
}

void Environment::fullScreenTriangle()
{
    auto [vertices, releaseFn] =
        getAllocatedBufferAndReleaseFn<float>(mVertexNumber * 3);

    std::copy(mVertices, mVertices + mVertexNumber * 3, vertices);

    mVertexBuffer.create(
        vertices, 
        mVertexNumber,
        bgfx::Attrib::Enum::Position,
        3,           // attributes per vertex
        vcl::PrimitiveType::FLOAT,
        false,       // data is normalized
        releaseFn
    );
}

void Environment::generateTextures(const uint viewId)
{
    using enum ComputeProgram;
    ProgramManager& pm = Context::instance().programManager();

    if(!mImage->m_cubeMap)
    {
        // convert hdr equirectangular to cubemap

        mHdrTexture->bind(
            0,
            mHdrSamplerUniform.handle()
        );
        
        mCubeMapTexture->bindForCompute(
            1,
            0,
            bgfx::Access::Write,
            bgfx::TextureFormat::RGBA32F
        );

        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<HDR_EQUIRECT_TO_CUBEMAP>(),
            CEIL_DIV(mCubeSide, 8),
            CEIL_DIV(mCubeSide, 8),
            6
        );
    }

    const bool generateCubeMips = 
        !mImage->m_cubeMap ||
        (mImage->m_cubeMap && mImage->m_numMips > 1);

    if(generateCubeMips)
    {
        // generate mipmaps for cubemap

        for(uint8_t mip = 1; mip < mCubeMips; mip++)
        {
            const uint32_t mipSize = CEIL_DIV(mCubeSide, 1 << mip);

            // ensure at least 1 threadgroup is dispatched for small mips
            // assuming the compute shader uses 8x8 threads per group
            // and checks for out-of-bounds internally
            const uint32_t threadGroups = (mipSize < 8) ? 1 : CEIL_DIV(mipSize, 8);

            mCubeMapTexture->bindForCompute(
                0,
                mip - 1,
                bgfx::Access::ReadWrite,
                bgfx::TextureFormat::RGBA32F
            );

            mCubeMapTexture->bindForCompute(
                1,
                mip,
                bgfx::Access::ReadWrite,
                bgfx::TextureFormat::RGBA32F
            );

            bgfx::dispatch(
                viewId,
                pm.getComputeProgram<CUBEMAP_MIPMAP_GEN>(),
                threadGroups,
                threadGroups,
                6
            );
        }
    }

    // create irradiance map from cubemap

    mCubeMapTexture->bind(
        0,
        mEnvCubeSamplerUniform.handle(),
        BGFX_SAMPLER_UVW_CLAMP
    );

    mIrradianceTexture->bindForCompute(
        1,
        0,
        bgfx::Access::Write,
        bgfx::TextureFormat::RGBA32F
    );

    bindDataUniform(float(mCubeSide));

    bgfx::dispatch(
        viewId,
        pm.getComputeProgram<CUBEMAP_TO_IRRADIANCE>(),
        CEIL_DIV(mIrradianceCubeSide, 8),
        CEIL_DIV(mIrradianceCubeSide, 8),
        6
    );

    // create specular map from cubemap

    for(uint8_t mip = 0; mip < mSpecularMips; ++mip) 
    {
        const uint32_t mipSize = CEIL_DIV(mSpecularCubeSide, 1 << mip);
        const float roughness = static_cast<float>(mip) / static_cast<float>(mSpecularMips - 1);

        // ensure at least 1 threadgroup is dispatched for small mips
        // assuming the compute shader uses 8x8 threads per group
        // and checks for out-of-bounds internally
        const uint32_t threadGroups = (mipSize < 8) ? 1 : CEIL_DIV(mipSize, 8);

        mCubeMapTexture->bind(
            0,
            mEnvCubeSamplerUniform.handle(),
            BGFX_SAMPLER_UVW_CLAMP
        );

        mSpecularTexture->bindForCompute(
            1,
            mip,
            bgfx::Access::Write,
            bgfx::TextureFormat::RGBA32F
        );

        bindDataUniform(roughness, float(mCubeSide));

        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<CUBEMAP_TO_SPECULAR>(),
            threadGroups,
            threadGroups,
            6
        );

    }

    // generate BRDF lookup texture

    mBrdfLuTexture->bindForCompute(
        0,
        0,
        bgfx::Access::Write,
        bgfx::TextureFormat::RGBA32F
    );

    bgfx::dispatch(
        viewId,
        pm.getComputeProgram<IBL_LOOKUP_TEXTURE_GEN>(),
        CEIL_DIV(mBrdfLutSize, 8),
        CEIL_DIV(mBrdfLutSize, 8)
    );
}

} // namespace vcl
