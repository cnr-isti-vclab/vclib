/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_SPACE_CORE_TEXTURE_DESCRIPTOR_H
#define VCL_SPACE_CORE_TEXTURE_DESCRIPTOR_H

#include <vclib/base.h>

namespace vcl {

/**
 * @class TextureDescriptor
 * @brief Describes the properties of a texture, such as its source path and
 * rendering parameters.
 *
 * This class encapsulates metadata for a texture, including its file path,
 * minification and magnification filters, and texture coordinate wrapping
 * modes. The enum values for filters and wrap modes follow the glTF 2.0
 * specification.
 * @ingroup space_core
 */
class TextureDescriptor
{
public:
    /**
     * @enum MinificationFilter
     * @brief Defines the texture minification filter modes, following the
     * glTF 2.0 specification. The minification filter is used when the pixel
     * being textured maps to an area greater than one texel.
     */
    enum class MinificationFilter {
        NONE    = -1,   ///< No filter specified.
        NEAREST = 9728, ///< Nearest neighbor filtering.
        LINEAR,         ///< Linear filtering.
        NEAREST_MIPMAP_NEAREST =
            9984, ///< Nearest neighbor filtering with the nearest mipmap level.
        LINEAR_MIPMAP_NEAREST, ///< Linear filtering with the nearest mipmap
                               ///< level.
        NEAREST_MIPMAP_LINEAR, ///< Nearest neighbor filtering with linear
                               ///< interpolation between mipmap levels.
        LINEAR_MIPMAP_LINEAR   ///< Linear filtering with linear interpolation
                               ///< between mipmap levels (trilinear filtering).
    };

    /**
     * @enum MagnificationFilter
     * @brief Defines the texture magnification filter modes, following the
     * glTF 2.0 specification. The magnification filter is used when the pixel
     * being textured maps to an area less than or equal to one texel.
     */
    enum class MagnificationFilter {
        NONE    = -1,   ///< No filter specified.
        NEAREST = 9728, ///< Nearest neighbor filtering.
        LINEAR          ///< Linear filtering.
    };

    /**
     * @enum WrapMode
     * @brief Defines the texture wrapping modes for S (U) and T (V)
     * coordinates, following the glTF 2.0 specification.
     */
    enum class WrapMode {
        REPEAT        = 10497,  ///< The texture repeats.
        CLAMP_TO_EDGE = 33071,  ///< The texture coordinates are clamped to the
                                ///< [0.0, 1.0] range.
        MIRRORED_REPEAT = 33648 ///< The texture repeats in a mirrored fashion.
    };

private:
    std::string        mPath; ///< The file path to the texture source.
    MinificationFilter mMinFilter =
        MinificationFilter::NONE; ///< The minification filter mode.
    MagnificationFilter mMagFilter =
        MagnificationFilter::NONE; ///< The magnification filter mode.
    WrapMode mWrapU =
        WrapMode::REPEAT; ///< The wrap mode for the U (S) texture coordinate.
    WrapMode mWrapV =
        WrapMode::REPEAT; ///< The wrap mode for the V (T) texture coordinate.

public:
    /**
     * @brief Default constructor. Initializes with an empty path and default
     * filter/wrap modes.
     */
    TextureDescriptor() {}

    /**
     * @brief Constructs a TextureDescriptor from a file path.
     *
     * Filter/wrap modes are initialized to default values.
     *
     * @param[in] path: The file path of the texture.
     */
    TextureDescriptor(const std::string& path) : mPath(path) {}

    /**
     * @brief Checks whether the texture descriptor is null (i.e., has an empty
     * path).
     * @return true if the path is empty, false otherwise.
     */
    bool isNull() const { return mPath.empty(); }

    /**
     * @brief Gets the file path of the texture.
     * @return A const reference to the texture's file path.
     */
    const std::string& path() const { return mPath; }

    /**
     * @brief Gets a mutable reference to the file path of the texture.
     * @return A reference to the texture's file path.
     */
    std::string& path() { return mPath; }

    /**
     * @brief Gets the minification filter of the texture.
     * @return The minification filter mode.
     */
    MinificationFilter minFilter() const { return mMinFilter; }

    /**
     * @brief Gets a mutable reference to the minification filter of the
     * texture.
     * @return A reference to the minification filter mode.
     */
    MinificationFilter& minFilter() { return mMinFilter; }

    /**
     * @brief Gets the magnification filter of the texture.
     * @return The magnification filter mode.
     */
    MagnificationFilter magFilter() const { return mMagFilter; }

    /**
     * @brief Gets a mutable reference to the magnification filter of the
     * texture.
     * @return A reference to the magnification filter mode.
     */
    MagnificationFilter& magFilter() { return mMagFilter; }

    /**
     * @brief Gets the wrap mode for the U (S) texture coordinate.
     * @return The U wrap mode.
     */
    WrapMode wrapU() const { return mWrapU; }

    /**
     * @brief Gets a mutable reference to the wrap mode for the U (S) texture
     * coordinate.
     * @return A reference to the U wrap mode.
     */
    WrapMode& wrapU() { return mWrapU; }

    /**
     * @brief Gets the wrap mode for the V (T) texture coordinate.
     * @return The V wrap mode.
     */
    WrapMode wrapV() const { return mWrapV; }

    /**
     * @brief Gets a mutable reference to the wrap mode for the V (T) texture
     * coordinate.
     * @return A reference to the V wrap mode.
     */
    WrapMode& wrapV() { return mWrapV; }

    /**
     * @brief Serializes the TextureDescriptor to an output stream.
     * @param[in/out] os: The output stream.
     */
    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mPath);
        vcl::serialize(os, mMinFilter, mMagFilter, mWrapU, mWrapV);
    }

    /**
     * @brief Deserializes the TextureDescriptor from an input stream.
     * @param[in/out] is: The input stream.
     */
    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mPath);
        vcl::deserialize(is, mMinFilter, mMagFilter, mWrapU, mWrapV);
    }

    /**
     * @brief Compares this TextureDescriptor with another for equality.
     * @param[in] other: The other TextureDescriptor to compare against.
     * @return true if all members are equal, false otherwise.
     */
    bool operator==(const TextureDescriptor& other) const = default;
};

/* Concepts */

/**
 * @brief A concept representing a TextureDescriptor.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a TextureDescriptor class.
 *
 * @tparam T: The type to be tested for conformity to the TextureConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept TextureDescriptorConcept =
    std::derived_from<std::remove_cvref_t<T>, TextureDescriptor>;

} // namespace vcl

#endif // VCL_SPACE_CORE_TEXTURE_DESCRIPTOR_H
