// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/panorama.h>

#include <vclib/io.h>

#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/math.h>
#include <bx/readerwriter.h>
#include <bx/allocator.h>

namespace vcl {

static bx::DefaultAllocator bxAllocator;

Panorama::Panorama(const std::string& imagePath) : mImagePath(imagePath)
{
    mFormat = getFileFormat(imagePath);
    mImageContainer = loadImage(imagePath, mFormat);
}

Panorama::~Panorama()
{
    if (mImageContainer) {
        bimg::imageFree(mImageContainer);
    }
}

Panorama::Panorama(Panorama&& other)
{
    swap(other);
}

Panorama& Panorama::operator=(Panorama&& other)
{
    swap(other);
    return *this;
}

void Panorama::swap(Panorama& other)
{
    using std::swap;
    swap(mImagePath, other.mImagePath);
    swap(mImageContainer, other.mImageContainer);
    swap(mFormat, other.mFormat);
}

std::string Panorama::imageFileName() const
{
    return FileInfo::fileNameWithExtension(mImagePath);
}

Panorama::FileFormat Panorama::getFileFormat(const std::string& imagePath)
{
    using enum Panorama::FileFormat;
    std::string fmt = vcl::toLower(FileInfo::extension(imagePath));

    if (fmt == ".hdr")
        return HDR;
    if (fmt == ".exr")
        return EXR;
    if (fmt == ".ktx")
        return KTX;
    if (fmt == ".dds")
        return DDS;
    return UNKNOWN;
}

bimg::ImageContainer* Panorama::loadImage(const std::string& imagePath, FileFormat format)
{
    using enum Panorama::FileFormat;

    if (format == UNKNOWN)
        return nullptr;

    bx::Error      err;
    bx::FileReader reader;

    // open the file
    if (!bx::open(&reader, imagePath.c_str(), &err))
        return nullptr;

    // read file size and allocate memory
    uint32_t inputSize = (uint32_t) bx::getSize(&reader);

    if (inputSize == 0)
        return nullptr;

    uint8_t* inputData = (uint8_t*) bx::alloc(&bxAllocator, inputSize);

    // read the file and put it raw in inputData
    uint rd = bx::read(&reader, inputData, inputSize, &err);
    bx::close(&reader);

    if (!err.isOk() || rd != inputSize) {
        bx::free(&bxAllocator, inputData);
        return nullptr;
    }

    // copy the data in the final container reading its characteristics
    using enum bimg::TextureFormat::Enum;

    bimg::ImageContainer* output =
        bimg::imageParse(&bxAllocator, inputData, inputSize, RGBA32F, &err);

    bx::free(&bxAllocator, inputData);

    if (!output) {
        return nullptr;
    }

    if (!err.isOk() ||
        (!output->m_cubeMap && format != HDR && format != EXR)) {
        // file is neither a cubemap nor an equirectangular map
        bimg::imageFree(output);
        return nullptr;
    }

    return output;
}

} // namespace vcl
