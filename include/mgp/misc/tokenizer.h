/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_TOKENIZER_H
#define MGP_TOKENIZER_H

#include <string>
#include <vector>

namespace mgp {

/**
 * @brief The Tokenizer class
 */
class Tokenizer
{
public:
	Tokenizer();
	Tokenizer(const char* string, char separator);
	Tokenizer(const std::string& string, char separator);
	
	using iterator = std::vector<std::string>::const_iterator;
	
	iterator begin() const;
	iterator end() const;
	
	unsigned long int size() const ;
	const std::string& operator[](unsigned int i) const;
	
private:
	void split();
	const char* string;
	char separator;
	std::vector<std::string> splitted;
};

}

#include "tokenizer.cpp"

#endif // MGP_TOKENIZER_H
