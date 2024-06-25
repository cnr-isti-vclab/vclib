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

#ifndef VCL_MISC_STRING_H
#define VCL_MISC_STRING_H

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>

#include <vclib/io/serialization.h>

namespace vcl {

/**
 * @brief Looks into `input` if there is a substring equal to `substr`, without
 * taking into account case sensitiveness. Returns an iterator pointing to the
 * beginning position in `input` of the substring, return an iterator `end()` if
 * the substring was not found.
 * @param input: input string.
 * @param substr: substring to look into `input`.
 * @return an iterator pointing to the substring, or an iterator pointing to
 * `end()`.
 */
inline std::string::const_iterator findCaseInsensitive(
    const std::string& input,
    const std::string& substr)
{
    // link: https://stackoverflow.com/a/19839371/5851101

    std::string::const_iterator it = std::search(
        input.begin(),
        input.end(),
        substr.begin(),
        substr.end(),
        [](char ch1, char ch2) {
            return std::toupper(ch1) == std::toupper(ch2);
        });
    return it;
}

/**
 * @brief Converts a value of type `T` to a string.
 *
 * With respect to `std::to_string`, this function also works with pointers.
 *
 * @param val: value to convert.
 * @return string representation of `val`.
 */
template<typename T>
std::string toString(T val)
{
    // if T is a pointer
    if constexpr (std::is_pointer_v<T>) {
        const void*       address = static_cast<const void*>(val);
        std::stringstream ss;
        ss << address;
        return ss.str();
    }
    else {
        return std::to_string(val);
    }
}

/**
 * @brief Returns `true` if the `input` string contains `substr` as a substring,
 * without taking into account case sensitiveness.
 * @param input: input string.
 * @param substr: substring to look into `input`.
 * @return `true` if `substr` was found in `input`
 */
inline bool containsCaseInsensitive(
    const std::string& input,
    const std::string& substr)
{
    return findCaseInsensitive(input, substr) != input.end();
}

inline std::string toLower(const std::string& s)
{
    std::string ret(s);
    std::transform(s.begin(), s.end(), ret.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return ret;
}

inline std::string toUpper(const std::string& s)
{
    std::string ret(s);
    std::transform(s.begin(), s.end(), ret.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return ret;
}

/**
 * @brief Removes the carriage return character ('\r') from the end of the
 * string.
 *
 * This is useful when reading text files in Windows, where the end of line is
 * represented by the sequence "\r\n".
 *
 * @param[in/out] s: input string.
 */
inline void removeCarriageReturn(std::string& s)
{
    if (s.size() > 0 && s[s.size() - 1] == '\r')
        s = s.substr(0, s.size() - 1);
}

} // namespace vcl

#endif // VCL_MISC_STRING_H
