/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "tokenizer.h"

namespace vcl {

inline Tokenizer::Tokenizer() : string(""), separator('\0')
{
}

inline Tokenizer::Tokenizer(const char* string, char separator) :
		string(string), separator(separator)
{
	split();
}

inline Tokenizer::Tokenizer(const std::string& string, char separator) :
		string(string.c_str()), separator(separator)
{
	split();
}

inline Tokenizer::iterator Tokenizer::begin() const
{
	return splitted.begin();
}

inline Tokenizer::iterator Tokenizer::end() const
{
	return splitted.end();
}

inline unsigned long Tokenizer::size() const
{
	return (unsigned long) splitted.size();
}

inline const std::string& Tokenizer::operator[](uint i) const
{
	return splitted[i];
}

/**
 * @brief Tokenizer::split
 * https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
 */
inline void Tokenizer::split()
{
	const char* str = string;
	splitted.clear();
	if (*str != '\0') {
		do {
			const char* begin = str;
			while (*str != separator && *str)
				str++;
			if (begin != str)
				splitted.push_back(std::string(begin, str));
		} while ('\0' != *str++);
	}
}

} // namespace vcl
