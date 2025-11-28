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

#ifndef VCL_IO_IMAGE_HDR_LOAD_H
#define VCL_IO_IMAGE_HDR_LOAD_H

#include <bgfx/bgfx.h>

#include <bx/allocator.h>
#include <bx/readerwriter.h>
#include <bx/endian.h>
#include <bx/math.h>

#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bimg/encode.h>

#include <bx/bx.h>
#include <bx/file.h>

#include <string>
#include <exception>

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

namespace vcl {

bimg::ImageContainer* loadCubemapFromHdr(std::string fileName)
{

    if(fileName.find(".hdr", fileName.length() - 4) == std::string::npos)
        throw std::runtime_error("File format is not hdr");

    // Code from bimg texturec

    // load the file
    bx::Error err;
    bx::FileReader reader;

    if(!bx::open(&reader, fileName.c_str(), &err))
        throw std::runtime_error("Failed to open input file");

    uint32_t inputSize = (uint32_t)bx::getSize(&reader);

    if(inputSize == 0)
		throw std::runtime_error("Failed to open input file");

    bx::DefaultAllocator defaultAllocator;
	AlignedAllocator allocator(&defaultAllocator, 16);

	uint8_t* inputData = (uint8_t*)bx::alloc(&allocator, inputSize);

	bx::read(&reader, inputData, inputSize, &err);
	bx::close(&reader);

    // convert it

    using enum bimg::TextureFormat::Enum;

    bimg::ImageContainer* output = nullptr;
	bimg::ImageContainer* input  = bimg::imageParse(&allocator, inputData, inputSize, Count, &err); 

    if(!err.isOk())
        return nullptr;
    
    if(input != nullptr)
    {

        bimg::TextureFormat::Enum inputFormat  = input->m_format;
		bimg::TextureFormat::Enum outputFormat = input->m_format;

        const bimg::ImageBlockInfo&  inputBlockInfo  = bimg::getBlockInfo(inputFormat);
		const bimg::ImageBlockInfo&  outputBlockInfo = bimg::getBlockInfo(outputFormat);
		const uint32_t blockWidth  = outputBlockInfo.blockWidth;
		const uint32_t blockHeight = outputBlockInfo.blockHeight;
		const uint32_t minBlockX   = outputBlockInfo.minBlockX;
		const uint32_t minBlockY   = outputBlockInfo.minBlockY;
		uint32_t outputWidth  = bx::max(blockWidth  * minBlockX, ( (input->m_width  + blockWidth  - 1) / blockWidth )*blockWidth);
		uint32_t outputHeight = bx::max(blockHeight * minBlockY, ( (input->m_height + blockHeight - 1) / blockHeight)*blockHeight);
		uint32_t outputDepth  = input->m_depth;

        if (outputDepth != 1 || outputWidth/2 != outputHeight)
		{
			bimg::imageFree(input);
			throw std::runtime_error("Input image format is not equirectangular projection (expected aspect ratio is 2:1).");
		}

		output = bimg::imageCubemapFromLatLongRgba32F(&allocator, *input, true, &err);
		bimg::imageFree(input);

        if(!err.isOk())
            return nullptr;

		// const uint8_t  numMips  = bimg get num mips;
		// const uint16_t numSides = 6;

		// for (uint16_t side = 0; side < numSides && err.isOk(); ++side)
		// {
		//   TODO: readapt the for loop to make just mipmaps
		// }
    }

    if(!err.isOk() && output != nullptr) {
        bimg::imageFree(output);
		output = nullptr;
    }

    bx::free(&allocator, inputData);

    if(output == nullptr)
        throw std::runtime_error("Failed to create output.");

    return output;
}

} // namespace vcl

#endif // VCL_IO_IMAGE_HDR_LOAD_H
