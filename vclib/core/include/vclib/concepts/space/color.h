/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_CONCEPTS_SPACE_COLOR_H
#define VCL_CONCEPTS_SPACE_COLOR_H

#include <vclib/concepts/const_correctness.h>

#include <concepts>
#include <cstdint>

namespace vcl {

/**
 * @brief ColorConcept is satisfied only if a class provides the member
 * functions specified in this concept. These member functions allows to access
 * to the color components of a given color.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept ColorConcept = requires (
    T&&                                 obj,
    typename RemoveRef<T>::ColorABGR    cABGR,
    uint32_t                            u32,
    uint16_t                            u16,
    uint8_t                             u8,
    float                               f,
    typename RemoveRef<T>::Format::Enum fr) {
    typename RemoveRef<T>::Format;
    typename RemoveRef<T>::ColorABGR;
    typename RemoveRef<T>::ColorMap;

    RemoveRef<T>();
    RemoveRef<T>(cABGR);
    RemoveRef<T>(u32, fr);
    RemoveRef<T>(u8, u8, u8);
    RemoveRef<T>(u8, u8, u8, u8);
    RemoveRef<T>(obj);

    { obj.red() } -> std::convertible_to<uint8_t>;
    { obj.green() } -> std::convertible_to<uint8_t>;
    { obj.blue() } -> std::convertible_to<uint8_t>;
    { obj.alpha() } -> std::convertible_to<uint8_t>;

    { obj.redF() } -> std::same_as<float>;
    { obj.greenF() } -> std::same_as<float>;
    { obj.blueF() } -> std::same_as<float>;
    { obj.alphaF() } -> std::same_as<float>;

    { obj.hsvHue() } -> std::same_as<uint8_t>;
    { obj.hsvSaturation() } -> std::same_as<uint8_t>;

    { obj.hsvHueF() } -> std::same_as<float>;
    { obj.hsvSaturationF() } -> std::same_as<float>;

    { obj.rgba() } -> std::same_as<uint32_t>;
    { obj.bgra() } -> std::same_as<uint32_t>;
    { obj.bgr5() } -> std::same_as<unsigned short>;
    { obj.rgb5() } -> std::same_as<unsigned short>;

    { obj == obj } -> std::same_as<bool>;
    { obj != obj } -> std::same_as<bool>;
    { obj < obj } -> std::same_as<bool>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.red() } -> std::same_as<uint8_t&>;
        { obj.green() } -> std::same_as<uint8_t&>;
        { obj.blue() } -> std::same_as<uint8_t&>;
        { obj.alpha() } -> std::same_as<uint8_t&>;

        { obj.setAlpha(u8) } -> std::same_as<void>;
        { obj.setRed(u8) } -> std::same_as<void>;
        { obj.setGreen(u8) } -> std::same_as<void>;
        { obj.setBlue(u8) } -> std::same_as<void>;

        { obj.setRgb(u8, u8, u8) } -> std::same_as<void>;
        { obj.setRgb(u8, u8, u8, u8) } -> std::same_as<void>;
        { obj.set(u32, fr) } -> std::same_as<void>;
        { obj.setAbgr(u32) } -> std::same_as<void>;
        { obj.setArgb(u32) } -> std::same_as<void>;
        { obj.setRgba(u32) } -> std::same_as<void>;
        { obj.setBgra(u32) } -> std::same_as<void>;
        { obj.setBgr5(u16) } -> std::same_as<void>;
        { obj.setRgb5(u16) } -> std::same_as<void>;
        { obj.setHsv(u8, u8, u8) } -> std::same_as<void>;
        { obj.setHsv(u8, u8, u8, u8) } -> std::same_as<void>;

        { obj.setAlphaF(f) } -> std::same_as<void>;
        { obj.setRedF(f) } -> std::same_as<void>;
        { obj.setGreenF(f) } -> std::same_as<void>;
        { obj.setBlueF(f) } -> std::same_as<void>;
        { obj.setRgbF(f, f, f) } -> std::same_as<void>;
        { obj.setRgbF(f, f, f, f) } -> std::same_as<void>;
        { obj.setHsvF(f, f, f) } -> std::same_as<void>;
        { obj.setHsvF(f, f, f, f) } -> std::same_as<void>;
    };
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_COLOR_H
