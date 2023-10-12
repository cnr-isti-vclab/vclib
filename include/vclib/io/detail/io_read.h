/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_IO_INTERNAL_IO_READ_H
#define VCL_IO_INTERNAL_IO_READ_H

#include "io_utils.h"

namespace vcl::io::detail {

// read/bin

template<typename T>
T readChar(std::ifstream& file)
{
    char c;
    file.read(&c, 1);
    return (T) c;
}

template<typename T>
T readUChar(std::ifstream& file)
{
    unsigned char c;
    file.read((char*) &c, 1);
    return (T) c;
}

template<typename T>
T readShort(std::ifstream& file)
{
    short c;
    file.read((char*) &c, 2);
    return (T) c;
}

template<typename T>
T readUShort(std::ifstream& file)
{
    unsigned short c;
    file.read((char*) &c, 2);
    return (T) c;
}

template<typename T>
T readInt(std::ifstream& file)
{
    int c;
    file.read((char*) &c, 4);
    return (T) c;
}

template<typename T>
T readUInt(std::ifstream& file)
{
    uint c;
    file.read((char*) &c, 4);
    return (T) c;
}

template<typename T>
T readFloat(std::ifstream& file, bool isColor = false)
{
    float c;
    file.read((char*) &c, 4);
    if (isColor)
        return (T) (c * 255);
    return (T) c;
}

template<typename T>
T readDouble(std::ifstream& file, bool isColor = false)
{
    double c;
    file.read((char*) &c, 8);
    if (isColor)
        return (T) (c * 255);
    return (T) c;
}

template<typename T>
T readProperty(std::ifstream& file, PropertyType type, bool isColor = false)
{
    T p;
    switch (type) {
    case CHAR: p = readChar<T>(file); break;
    case UCHAR: p = readUChar<T>(file); break;
    case SHORT: p = readShort<T>(file); break;
    case USHORT: p = readUShort<T>(file); break;
    case INT: p = readInt<T>(file); break;
    case UINT: p = readUInt<T>(file); break;
    case FLOAT: p = readFloat<T>(file, isColor); break;
    case DOUBLE: p = readDouble<T>(file, isColor); break;
    default: assert(0); p = 0;
    }
    // if I read a color that must be returned as a float or double
    if (isColor && !std::is_integral<T>::value)
        p = (float) p / 255.0;
    return p;
}

template<ElementConcept El>
void readCustomComponent(
    std::ifstream&     file,
    El&                elem,
    const std::string& cName,
    PropertyType       type)
{
    std::type_index ti = elem.customComponentType(cName);
    if (ti == typeid(char))
        elem.template customComponent<char>(cName) =
            readProperty<char>(file, type);
    else if (ti == typeid(unsigned char))
        elem.template customComponent<unsigned char>(cName) =
            readProperty<unsigned char>(file, type);
    else if (ti == typeid(short))
        elem.template customComponent<short>(cName) =
            readProperty<short>(file, type);
    else if (ti == typeid(unsigned short))
        elem.template customComponent<unsigned short>(cName) =
            readProperty<unsigned short>(file, type);
    else if (ti == typeid(int))
        elem.template customComponent<int>(cName) =
            readProperty<int>(file, type);
    else if (ti == typeid(unsigned int))
        elem.template customComponent<uint>(cName) =
            readProperty<uint>(file, type);
    else if (ti == typeid(float))
        elem.template customComponent<float>(cName) =
            readProperty<float>(file, type);
    else if (ti == typeid(double))
        elem.template customComponent<double>(cName) =
            readProperty<double>(file, type);
    else
        assert(0);
}

// read/txt

template<typename T>
T readChar(vcl::Tokenizer::iterator& token)
{
    return std::stoi(*token++);
}

template<typename T>
T readUChar(vcl::Tokenizer::iterator& token)
{
    return std::stoi(*token++);
}

template<typename T>
T readShort(vcl::Tokenizer::iterator& token)
{
    return std::stoi(*token++);
}

template<typename T>
T readUShort(vcl::Tokenizer::iterator& token)
{
    return std::stoi(*token++);
}

template<typename T>
T readInt(vcl::Tokenizer::iterator& token)
{
    return std::stoi(*token++);
}

template<typename T>
T readUInt(vcl::Tokenizer::iterator& token)
{
    return std::stoi(*token++);
}

template<typename T>
T readFloat(vcl::Tokenizer::iterator& token, bool isColor = false)
{
    if (isColor && std::is_integral<T>::value) {
        return std::stod(*token++) * 255;
    }
    else {
        return std::stod(*token++);
    }
}

template<typename T>
T readDouble(vcl::Tokenizer::iterator& token, bool isColor = false)
{
    if (isColor && std::is_integral<T>::value) {
        return std::stod(*token++) * 255;
    }
    else {
        return std::stod(*token++);
    }
}

template<typename T>
T readProperty(
    vcl::Tokenizer::iterator& token,
    PropertyType              type,
    bool                      isColor = false)
{
    T p;
    switch (type) {
    case CHAR:
    case UCHAR:
    case SHORT:
    case USHORT:
    case INT:
    case UINT: p = std::stoi(*token++); break;
    case FLOAT:
    case DOUBLE:
        if (isColor) {
            p = std::stod(*token++) * 255;
        }
        else {
            p = std::stod(*token++);
        }
        break;
    default: assert(0); p = 0;
    }
    // if I read a color that must be returned as a float or double
    if (isColor && !std::is_integral<T>::value)
        p = (float) p / 255.0;
    return p;
}

template<ElementConcept El>
void readCustomComponent(
    vcl::Tokenizer::iterator& token,
    El&                       elem,
    const std::string&        cName,
    PropertyType              type)
{
    std::type_index ti = elem.customComponentType(cName);
    if (ti == typeid(char))
        elem.template customComponent<char>(cName) =
            readProperty<char>(token, type);
    else if (ti == typeid(unsigned char))
        elem.template customComponent<unsigned char>(cName) =
            readProperty<unsigned char>(token, type);
    else if (ti == typeid(short))
        elem.template customComponent<short>(cName) =
            readProperty<short>(token, type);
    else if (ti == typeid(unsigned short))
        elem.template customComponent<unsigned short>(cName) =
            readProperty<unsigned short>(token, type);
    else if (ti == typeid(int))
        elem.template customComponent<int>(cName) =
            readProperty<int>(token, type);
    else if (ti == typeid(unsigned int))
        elem.template customComponent<uint>(cName) =
            readProperty<uint>(token, type);
    else if (ti == typeid(float))
        elem.template customComponent<float>(cName) =
            readProperty<float>(token, type);
    else if (ti == typeid(double))
        elem.template customComponent<double>(cName) =
            readProperty<double>(token, type);
    else
        assert(0);
}

} // vcl::io::detail

#endif // VCL_IO_INTERNAL_IO_READ_H
