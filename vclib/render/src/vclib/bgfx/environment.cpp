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

namespace vcl {

Environment::Environment(const std::string& imagePath)
{
    using enum Environment::FileFormat;
    mSourceFormat = getFileFormat(imagePath);

    if(mSourceFormat == UNKNOWN) return;

    mImage = Environment::loadImage(imagePath);

    // if it's not a cubemap it's equirectangular
    mCubeSide = mImage->m_cubeMap? mImage->m_width : mImage->m_width / 4;
    mCubeMips = bimg::imageGetNumMips(
        bimg::TextureFormat::RGBA32F,
        mCubeSide,
        mCubeSide
    );

    mIrradianceCubeSide = mCubeSide / 4;

    mSpecularCubeSide = mCubeSide / 4;
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

void Environment::drawBackground(const DrawObjectSettings& settings)
{
    using enum Environment::FileFormat;
    if(mSourceFormat == UNKNOWN) return;

    if(!mBackgroundReady)
        prepareBackground();

    using enum VertFragProgram;
    ProgramManager& pm = Context::instance().programManager();

    mVertexBuffer.bindVertex(0);

    bgfx::setState(BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LEQUAL);

    bgfx::submit(
        settings.viewId, 
        pm.getProgram<DRAWABLE_BACKGROUND_PBR>()
    );
}

void Environment::prepareBackground()
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

    mBackgroundReady = true;
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

    if(!err.isOk())
        return nullptr;

	return output;
}

} // namespace vcl
