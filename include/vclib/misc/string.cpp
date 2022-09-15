/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "string.h"

#include <algorithm>
#include <cctype>

namespace vcl::str {

/**
 * @brief Returns `true` if the `input` string contains `substr` as a substring, without taking
 * into account case sensitiveness.
 * @param input: input string.
 * @param substr: substring to look into `input`.
 * @return `true` if `substr` was found in `input`
 */
bool containsCaseInsensitive(const std::string& input, const std::string& substr)
{
	return findCaseInsensitive(input, substr) != input.end();
}

/**
 * @brief Looks into `input` if there is a substring equal to `substr`, without taking into account
 * case sensitiveness. Returns an iterator pointing to the beginning position in `input` of the
 * substring, return an iterator `end()` if the substring was not found.
 * @param input: input string.
 * @param substr: substring to look into `input`.
 * @return an iterator pointing to the substring, or an iterator pointing to `end()`.
 */
std::string::const_iterator findCaseInsensitive(const std::string& input, const std::string& substr)
{
	// link: https://stackoverflow.com/a/19839371/5851101

	std::string::const_iterator it = std::search(
		input.begin(), input.end(), substr.begin(), substr.end(), [](char ch1, char ch2) {
			return std::toupper(ch1) == std::toupper(ch2);
		});
	return it;
}

inline std::string toLower(const std::string& s)
{
	std::string ret(s);
	std::transform(
		s.begin(), s.end(), ret.begin(), [](unsigned char c) { return std::tolower(c); });
	return ret;
}

inline std::string toUpper(const std::string& s)
{
	std::string ret(s);
	std::transform(
		s.begin(), s.end(), ret.begin(), [](unsigned char c) { return std::toupper(c); });
	return ret;
}

inline void removeWindowsNewLine(std::string& s)
{
	if (s.size() > 0 && s[s.size()-1] == '\r')
		s = s.substr(0, s.size()-1);
}

} // namespace vcl::str
