// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PANORAMA_H
#define VCL_BGFX_PANORAMA_H

#include <string>

namespace bimg {
struct ImageContainer;
} // namespace bimg

namespace vcl {

/**
 * @brief A class representing a panorama image in RAM.
 *
 * It manages the loading and the lifetime of a bimg::ImageContainer from disk.
 * Supports HDR, EXR, KTX, DDS files (equirectangular or cubemap).
 */
class Panorama
{
public:
    enum class FileFormat { UNKNOWN, HDR, EXR, KTX, DDS };

private:
    std::string           mImagePath;
    bimg::ImageContainer* mImageContainer = nullptr;
    FileFormat            mFormat         = FileFormat::UNKNOWN;

public:
    Panorama() = default;

    /**
     * @brief Constructor that loads an image from a given file path.
     * @param[in] imagePath: The path to the image file.
     */
    Panorama(const std::string& imagePath);

    ~Panorama();

    Panorama(const Panorama&)            = delete;
    Panorama& operator=(const Panorama&) = delete;

    Panorama(Panorama&& other);
    Panorama& operator=(Panorama&& other);

    void swap(Panorama& other);

    friend void swap(Panorama& first, Panorama& second) { first.swap(second); }

    /**
     * @brief Checks if the panorama image was loaded successfully.
     * @return true if valid, false otherwise.
     */
    bool isValid() const { return mImageContainer != nullptr; }

    const bimg::ImageContainer* image() const { return mImageContainer; }

    const std::string& imagePath() const { return mImagePath; }

    std::string imageFileName() const;

    FileFormat format() const { return mFormat; }

private:
    static FileFormat            getFileFormat(const std::string& imagePath);
    static bimg::ImageContainer* loadImage(
        const std::string& imagePath,
        FileFormat         format);
};

} // namespace vcl

#endif // VCL_BGFX_PANORAMA_H
