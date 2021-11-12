/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "tokenizer.h"

namespace mgp {

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

inline const std::string& Tokenizer::operator[](unsigned int i) const
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

} // namespace mgp
