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

#ifndef VCL_TOKENIZER_H
#define VCL_TOKENIZER_H

#include <string>
#include <vector>

#include "types.h"

namespace vcl {

/**
 * @brief The Tokenizer class
 */
class Tokenizer
{
public:
	Tokenizer();
	Tokenizer(const char* string, char separator, bool jumpEmptyTokens = true);
	Tokenizer(const std::string& string, char separator, bool jumpEmptyTokens = true);
	
	using iterator = std::vector<std::string>::const_iterator;
	
	iterator begin() const;
	iterator end() const;
	
	unsigned long int size() const ;
	const std::string& operator[](uint i) const;
	
private:
	void split(const char* str, bool jumpEmptyTokens = true);
	char separator;
	std::vector<std::string> splitted;
};

}

#include "tokenizer.cpp"

#endif // VCL_TOKENIZER_H
