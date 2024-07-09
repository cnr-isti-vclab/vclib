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

#ifndef VCL_IO_WRITE_H
#define VCL_IO_WRITE_H

#include <filesystem>
#include <fstream>
#include <typeindex>

#include <vclib/concepts/mesh/elements/element.h>
#include <vclib/types.h>

#include "file_info.h"
#include "file_type.h"
#include "serialization/serialize.h"

namespace vcl {

inline std::ofstream openOutputFileStream(
    const std::string& filename,
    const std::string& ext = "")
{
    setlocale(LC_ALL, "C");
    std::string actualfilename = filename;
    std::string path           = FileInfo::pathWithoutFileName(filename);

    if (!std::filesystem::exists(path)) {
        bool res = std::filesystem::create_directory(path);
        if (!res) {
            throw std::runtime_error("Cannot create directory: " + path);
        }
    }

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
void writeChar(
    std::ostream& file,
    T             p,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    char tmp = p;
    if (format.isBinary)
        serialize(file, tmp, format.endian);
    else
        file << (int) p << " "; // cast necessary to not print the ascii char
}

template<typename T>
void writeUChar(
    std::ostream& file,
    T             p,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    unsigned char tmp = p;
    if (format.isBinary)
        serialize(file, tmp, format.endian);
    else
        file << (uint) p << " "; // cast necessary to not print the ascii char
}

template<typename T>
void writeShort(
    std::ostream& file,
    T             p,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    short tmp = p;
    if (format.isBinary)
        serialize(file, tmp, format.endian);
    else
        file << tmp << " ";
}

template<typename T>
void writeUShort(
    std::ostream& file,
    T             p,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    unsigned short tmp = p;
    if (format.isBinary)
        serialize(file, tmp, format.endian);
    else
        file << tmp << " ";
}

template<typename T>
void writeInt(
    std::ostream& file,
    T             p,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    int tmp = p;
    if (format.isBinary)
        serialize(file, tmp, format.endian);
    else
        file << tmp << " ";
}

template<typename T>
void writeUInt(
    std::ostream& file,
    T             p,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    if (isColor && !std::is_integral<T>::value)
        p *= 255;
    uint tmp = p;
    if (format.isBinary)
        serialize(file, tmp, format.endian);
    else
        file << tmp << " ";
}

template<typename T>
void writeFloat(
    std::ostream& file,
    const T&      p,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    float tmp = p;
    if (isColor && std::is_integral<T>::value)
        tmp /= 255;
    if (format.isBinary)
        serialize(file, tmp, format.endian);
    else
        file << tmp << " ";
}

template<typename T>
void writeDouble(
    std::ostream& file,
    const T&      p,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    double tmp = p;
    if (isColor && std::is_integral<T>::value)
        tmp /= 255;
    if (format.isBinary)
        serialize(file, tmp, format.endian);
    else
        file << tmp << " ";
}

// TODO - rename to writePrimitiveType
template<typename T>
void writeProperty(
    std::ostream& file,
    const T&      p,
    PrimitiveType type,
    FileType    format  = FileType(),
    bool          isColor = false)
{
    switch (type) {
    case CHAR: writeChar(file, p, format, isColor); break;
    case UCHAR: writeUChar(file, p, format, isColor); break;
    case SHORT: writeShort(file, p, format, isColor); break;
    case USHORT: writeUShort(file, p, format, isColor); break;
    case INT: writeInt(file, p, format, isColor); break;
    case UINT: writeUInt(file, p, format, isColor); break;
    case FLOAT: writeFloat(file, p, format, isColor); break;
    case DOUBLE: writeDouble(file, p, format, isColor); break;
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
    FileType         format = FileType())
{
    std::type_index ti = elem.customComponentType(cName);
    if (ti == typeid(char))
        writeProperty(
            file, elem.template customComponent<char>(cName), type, format);
    else if (ti == typeid(unsigned char))
        writeProperty(
            file,
            elem.template customComponent<unsigned char>(cName),
            type,
            format);
    else if (ti == typeid(short))
        writeProperty(
            file, elem.template customComponent<short>(cName), type, format);
    else if (ti == typeid(unsigned short))
        writeProperty(
            file,
            elem.template customComponent<unsigned short>(cName),
            type,
            format);
    else if (ti == typeid(int))
        writeProperty(
            file, elem.template customComponent<int>(cName), type, format);
    else if (ti == typeid(unsigned int))
        writeProperty(
            file, elem.template customComponent<uint>(cName), type, format);
    else if (ti == typeid(float))
        writeProperty(
            file, elem.template customComponent<float>(cName), type, format);
    else if (ti == typeid(double))
        writeProperty(
            file, elem.template customComponent<double>(cName), type, format);
    else
        assert(0);
}

} // namespace io
} // namespace vcl

#endif // VCL_IO_WRITE_H
