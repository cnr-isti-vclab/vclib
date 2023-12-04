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

#ifndef VCL_IO_WRITE_H
#define VCL_IO_WRITE_H

#include <fstream>
#include <typeindex>

#include <vclib/concepts/mesh/elements/element.h>
#include <vclib/types.h>

#include "file_info.h"

namespace vcl {

inline std::ofstream openOutputFileStream(
    const std::string& filename,
    const std::string& ext = "")
{
    setlocale(LC_ALL, "C");
    std::string actualfilename = filename;
    if (!ext.empty()) {
        actualfilename = FileInfo::addExtensionIfNeeded(filename, ext);
    }

    std::ofstream fp;
    fp.imbue(std::locale().classic());

    // need to set binary or windows will fail
    fp.open(actualfilename, std::ofstream::binary);
    if (!fp) {
        throw vcl::CannotOpenFileException(actualfilename);
    }

    return fp;
}

namespace io {

template<typename T>
void writeChar(std::ostream& file, T p, bool bin = true, bool isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    char tmp = p;
    if (bin)
        file.write((const char*) &tmp, 1);
    else
        file << (int) p << " "; // cast necessary to not print the ascii char
}

template<typename T>
void writeUChar(std::ostream& file, T p, bool bin = true, bool isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    unsigned char tmp = p;
    if (bin)
        file.write((const char*) &tmp, 1);
    else
        file << (uint) p << " "; // cast necessary to not print the ascii char
}

template<typename T>
void writeShort(std::ostream& file, T p, bool bin = true, bool isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    short tmp = p;
    if (bin)
        file.write((const char*) &tmp, 2);
    else
        file << tmp << " ";
}

template<typename T>
void writeUShort(std::ostream& file, T p, bool bin = true, bool isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    unsigned short tmp = p;
    if (bin)
        file.write((const char*) &tmp, 2);
    else
        file << tmp << " ";
}

template<typename T>
void writeInt(std::ostream& file, T p, bool bin = true, bool isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    int tmp = p;
    if (bin)
        file.write((const char*) &tmp, 4);
    else
        file << tmp << " ";
}

template<typename T>
void writeUInt(std::ostream& file, T p, bool bin = true, bool isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    uint tmp = p;
    if (bin)
        file.write((const char*) &tmp, 4);
    else
        file << tmp << " ";
}

template<typename T>
void writeFloat(
    std::ostream& file,
    const T&      p,
    bool          bin     = true,
    bool          isColor = false)
{
    float tmp = p;
    if (isColor && std::is_integral<T>::value)
        tmp /= 255;
    if (bin)
        file.write((const char*) &tmp, 4);
    else
        file << tmp << " ";
}

template<typename T>
void writeDouble(
    std::ostream& file,
    const T&      p,
    bool          bin     = true,
    bool          isColor = false)
{
    double tmp = p;
    if (isColor && std::is_integral<T>::value)
        tmp /= 255;
    if (bin)
        file.write((const char*) &tmp, 8);
    else
        file << tmp << " ";
}

// TODO - rename to writePrimitiveType
template<typename T>
void writeProperty(
    std::ostream& file,
    const T&      p,
    PrimitiveType type,
    bool          bin     = true,
    bool          isColor = false)
{
    switch (type) {
    case CHAR: writeChar(file, p, bin, isColor); break;
    case UCHAR: writeUChar(file, p, bin, isColor); break;
    case SHORT: writeShort(file, p, bin, isColor); break;
    case USHORT: writeUShort(file, p, bin, isColor); break;
    case INT: writeInt(file, p, bin, isColor); break;
    case UINT: writeUInt(file, p, bin, isColor); break;
    case FLOAT: writeFloat(file, p, bin, isColor); break;
    case DOUBLE: writeDouble(file, p, bin, isColor); break;
    default: assert(0);
    }
}

// TODO - move this to some specific mesh file
template<ElementConcept El>
void writeCustomComponent(
    std::ostream&      file,
    const El&          elem,
    const std::string& cName,
    PrimitiveType      type,
    bool               bin = true)
{
    std::type_index ti = elem.customComponentType(cName);
    if (ti == typeid(char))
        writeProperty(
            file, elem.template customComponent<char>(cName), type, bin);
    else if (ti == typeid(unsigned char))
        writeProperty(
            file,
            elem.template customComponent<unsigned char>(cName),
            type,
            bin);
    else if (ti == typeid(short))
        writeProperty(
            file, elem.template customComponent<short>(cName), type, bin);
    else if (ti == typeid(unsigned short))
        writeProperty(
            file,
            elem.template customComponent<unsigned short>(cName),
            type,
            bin);
    else if (ti == typeid(int))
        writeProperty(
            file, elem.template customComponent<int>(cName), type, bin);
    else if (ti == typeid(unsigned int))
        writeProperty(
            file, elem.template customComponent<uint>(cName), type, bin);
    else if (ti == typeid(float))
        writeProperty(
            file, elem.template customComponent<float>(cName), type, bin);
    else if (ti == typeid(double))
        writeProperty(
            file, elem.template customComponent<double>(cName), type, bin);
    else
        assert(0);
}

} // namespace io
} // namespace vcl

#endif // VCL_IO_WRITE_H
