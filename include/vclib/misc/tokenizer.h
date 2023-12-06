/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
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

#ifndef VCL_MISC_TOKENIZER_H
#define VCL_MISC_TOKENIZER_H

#include <string>
#include <vector>

#include <vclib/types.h>

namespace vcl {

/**
 * @brief The Tokenizer class
 */
class Tokenizer
{
    char separator = '\0';

    std::vector<std::string> splitted;

public:
    using iterator = std::vector<std::string>::const_iterator;

    Tokenizer() = default;

    Tokenizer(const char* string, char separator, bool jumpEmptyTokens = true) :
            separator(separator)
    {
        split(string, jumpEmptyTokens);
    }

    Tokenizer(
        const std::string& string,
        char               separator,
        bool               jumpEmptyTokens = true) :
            separator(separator)
    {
        split(string.c_str(), jumpEmptyTokens);
    }

    iterator begin() const { return splitted.begin(); }

    iterator end() const { return splitted.end(); }

    unsigned long int size() const { return (unsigned long) splitted.size(); }

    const std::string& operator[](uint i) const { return splitted[i]; }

private:
    void split(const char* str, bool jumpEmptyTokens = true)
    {
        // https://stackoverflow.com/questions/53849/
        splitted.clear();
        if (*str != '\0') {
            do {
                const char* begin = str;
                while (*str != separator && *str)
                    str++;
                if (begin != str)
                    splitted.push_back(std::string(begin, str));
                else if (!jumpEmptyTokens) {
                    splitted.push_back(std::string());
                }
            } while ('\0' != *str++);
        }
    }
};

} // namespace vcl

#endif // VCL_MISC_TOKENIZER_H
