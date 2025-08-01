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

#ifndef VCL_MISC_STRING_H
#define VCL_MISC_STRING_H

#include <vclib/base.h>

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

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

inline constexpr std::string toLower(const std::string& s)
{
    std::string ret(s);
    std::transform(s.begin(), s.end(), ret.begin(), ::tolower);
    return ret;
}

inline constexpr std::string toUpper(const std::string& s)
{
    std::string ret(s);
    std::transform(s.begin(), s.end(), ret.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return ret;
}

/**
 * @brief Converts a string from camel case to snake case.
 *
 * For example, "CamelCase" is converted to "camel_case".
 *
 * @param[in] s: input string.
 * @return the input string converted to snake case.
 */
inline constexpr std::string camelCaseToSnakeCase(const std::string& s)
{
    std::string ret;
    for (size_t i = 0; i < s.size(); i++) {
        if (i > 0 && std::isupper(s[i]))
            ret += '_';
        ret += std::tolower(s[i]);
    }
    return ret;
}

/**
 * @brief Removes the carriage return character (`\r`) from the end of the
 * string.
 *
 * This is useful when reading text files in Windows, where the end of line is
 * represented by the sequence `\r\n`.
 *
 * @param[in,out] s: input string.
 */
inline void removeCarriageReturn(std::string& s)
{
    if (s.size() > 0 && s[s.size() - 1] == '\r')
        s = s.substr(0, s.size() - 1);
}

/**
 * @brief Computes the Levenshtein distance between two strings.
 *
 * The [Levenshtein
 * distance](https://en.wikipedia.org/wiki/Levenshtein_distance) is the minimum
 * number of single-character edits (insertions, deletions, or substitutions)
 * required to change one word into another.
 *
 * @param[in] str1: first string.
 * @param[in] str2: second string.
 * @return the Levenshtein distance between `str2` and `str2`.
 */
inline uint levenshteinDist(const std::string& str1, const std::string& str2)
{
    std::vector<std::vector<uint>> d(
        str1.size() + 1, std::vector<uint>(str2.size() + 1));

    if (str1.size() == 0)
        return str2.size();
    if (str2.size() == 0)
        return str1.size();

    for (uint i = 0; i <= str1.size(); i++)
        d[i][0] = i;
    for (uint j = 0; j <= str2.size(); j++)
        d[0][j] = j;

    for (uint i = 1; i <= str1.size(); i++) {
        for (uint j = 1; j <= str2.size(); j++) {
            uint cost = (str2[j - 1] == str1[i - 1]) ? 0 : 1;

            uint deletion     = d[i - 1][j] + 1;
            uint insertion    = d[i][j - 1] + 1;
            uint substitution = d[i - 1][j - 1] + cost;

            d[i][j] = vcl::min(deletion, insertion, substitution);
        }
    }

    return d[str1.size()][str2.size()];
}

/**
 * @brief Computes the distance between two strings.
 *
 * This function is an alias for `levenshteinDist`.
 *
 * @param[in] str1: first string.
 * @param[in] str2: second string.
 * @return the Levenshtein distance between `str1` and `str2`.
 */
inline uint distance(const std::string& str1, const std::string& str2)
{
    return levenshteinDist(str1, str2);
}

} // namespace vcl

#endif // VCL_MISC_STRING_H
