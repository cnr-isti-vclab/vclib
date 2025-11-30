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

// Allocator references are stored in image containers
// so they have to remain visible somehow.
// TODO: find a better way to do so.
static bx::DefaultAllocator bxDefaultAllocator;
static AlignedAllocator bxAlignedAllocator(&bxDefaultAllocator, 16);

namespace vcl {

bimg::ImageContainer* loadHdr(std::string fileName)
{
	// check the format

	if(fileName.find(".hdr", fileName.length() - 4) == std::string::npos)
        throw std::runtime_error("File format is not hdr");

    /* Code from bimg texturec */

	bx::Error err;
    bx::FileReader reader;

	// open the file

    if(!bx::open(&reader, fileName.c_str(), &err))
        throw std::runtime_error("Failed to open input file");

	// read file size and allocate memory

    uint32_t inputSize = (uint32_t)bx::getSize(&reader);

    if(inputSize == 0)
		throw std::runtime_error("Failed to open input file");

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

bimg::ImageContainer* hdrToCubemap(bimg::ImageContainer* hdr)
{
	bx::Error err;
	bimg::ImageContainer* output = nullptr;

	if(hdr != nullptr)
    {

        bimg::TextureFormat::Enum inputFormat  = hdr->m_format;
		bimg::TextureFormat::Enum outputFormat = hdr->m_format;

		// check if the input is an equirectangular projection

        const bimg::ImageBlockInfo&  inputBlockInfo  = bimg::getBlockInfo(inputFormat);
		const bimg::ImageBlockInfo&  outputBlockInfo = bimg::getBlockInfo(outputFormat);
		const uint32_t blockWidth  = outputBlockInfo.blockWidth;
		const uint32_t blockHeight = outputBlockInfo.blockHeight;
		const uint32_t minBlockX   = outputBlockInfo.minBlockX;
		const uint32_t minBlockY   = outputBlockInfo.minBlockY;
		uint32_t outputWidth  = bx::max(blockWidth  * minBlockX, ( (hdr->m_width  + blockWidth  - 1) / blockWidth )*blockWidth);
		uint32_t outputHeight = bx::max(blockHeight * minBlockY, ( (hdr->m_height + blockHeight - 1) / blockHeight)*blockHeight);
		uint32_t outputDepth  = hdr->m_depth;

        if (outputDepth != 1 || outputWidth/2 != outputHeight)
			throw std::runtime_error("Input image format is not equirectangular projection (expected aspect ratio is 2:1).");

		// convert it to cubemap

		output = bimg::imageCubemapFromLatLongRgba32F(&bxAlignedAllocator, *hdr, true, &err);

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

    if(output == nullptr) {
		bimg::imageFree(hdr);
		throw std::runtime_error("Failed to create output.");
	}  

    return output;
}

bimg::ImageContainer* loadCubemapFromHdr(std::string fileName)
{
	bimg::ImageContainer* hdr = loadHdr(fileName);
	bimg::ImageContainer* cubemap = hdrToCubemap(hdr);
	bimg::imageFree(hdr);
	return cubemap;
}

} // namespace vcl

#endif // VCL_IO_IMAGE_HDR_LOAD_H
