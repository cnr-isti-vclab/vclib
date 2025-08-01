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

#ifndef VCL_IO_READ_H
#define VCL_IO_READ_H

#include "file_info.h"

#include <vclib/base.h>
#include <vclib/mesh.h>

namespace vcl {

namespace detail {

/**
 * @brief Read the next non-empty line from an input stream.
 *
 * @throws MalformedFileException if the stream ends before a non-empty line is
 * found and THROW is true.
 *
 * @tparam THROW: if true, throw an exception if the stream ends before a
 * non-empty line is found. If false and if it reaches the end of file, returns
 * an empty string.
 *
 * @param[in] file: the stream to read from.
 * @return the next non-empty line read from the stream.
 */
template<bool THROW = true>
inline std::string readNextNonEmptyLine(std::istream& file)
{
    std::string line;
    do {
        std::getline(file, line);
        if constexpr (THROW) {
            if (!file) {
                throw MalformedFileException("Unexpected end of file.");
            }
        }
        if (file && line.size() > 0) {
            removeCarriageReturn(line);
        }
    } while (file && line.size() == 0);
    return line;
}

} // namespace detail

/**
 * @brief Open an input stream from a file.
 *
 * @throws CannotOpenFileException if the file cannot be opened.
 *
 * @param[in] filename: the name of the file to open.
 * @param[in] ext: the extension of the file to open. If empty, the filename is
 * used as is.
 * @return the input stream.
 */
inline std::ifstream openInputFileStream(
    const std::string& filename,
    const std::string& ext = "")
{
    setlocale(LC_ALL, "C");

    std::string actualfilename = filename;
    if (!ext.empty()) {
        actualfilename = FileInfo::addExtensionIfNeeded(filename, ext);
    }
    // need to set binary or windows will fail
    std::ifstream fp(actualfilename, std::ifstream::binary);
    fp.imbue(std::locale().classic());

    if (!fp.is_open()) {
        throw CannotOpenFileException(filename);
    }
    return fp;
}

/**
 * @brief Reads and returns the next non-empty line from a txt stream.
 *
 * @throws MalformedFileException if the stream ends before a non-empty line is
 * found.
 *
 * @param file: the stream to read from.
 * @return the next non-empty line read from the file.
 */
inline std::string readNextNonEmptyLine(std::istream& file)
{
    return detail::readNextNonEmptyLine<>(file);
}

/**
 * @brief Reads and returns the next non-empty line from a txt stream.
 *
 * @param[in] file: the stream to read from.
 * @return the next non-empty line read from the stream. If the stream ends
 * before a non-empty line is found, returns an empty string.
 */
inline std::string readNextNonEmptyLineNoThrow(std::istream& file)
{
    return detail::readNextNonEmptyLine<false>(file);
}

/**
 * @brief Reads and returns the next non-empty line from a txt stream, tokenized
 * with the given separator.
 *
 * @throws MalformedFileException if the stream ends before a non-empty line is
 * found.
 *
 * @param[in] file: the stream to read from.
 * @param[in] separators: the separators to use for tokenization.
 * @return the next non-empty line read from the stream, tokenized with the
 * given separator.
 */
inline Tokenizer readAndTokenizeNextNonEmptyLine(
    std::istream&     file,
    std::vector<char> separators = {' ', '\t'})
{
    std::string line;
    Tokenizer   tokenizer;

    do {
        line      = readNextNonEmptyLine(file);
        tokenizer = Tokenizer(line, separators);
    } while (tokenizer.begin() == tokenizer.end());

    return tokenizer;
}

/**
 * @brief Reads and returns the next non-empty line from a txt stream, tokenized
 * with the given separator.
 *
 * @param[in] file: the stream to read from.
 * @param[in] separators: the separators to use for tokenization.
 * @return the next non-empty line read from the stream, tokenized with the
 * given separator. If the stream ends before a non-empty line is found, returns
 * an empty tokenizer.
 */
inline Tokenizer readAndTokenizeNextNonEmptyLineNoThrow(
    std::istream&     file,
    std::vector<char> separators = {' ', '\t'})
{
    std::string line;
    Tokenizer   tokenizer;

    do {
        line      = readNextNonEmptyLineNoThrow(file);
        tokenizer = Tokenizer(line, separators);
    } while (file && tokenizer.begin() == tokenizer.end());

    return tokenizer;
}

namespace io {

// read/bin

// TODO: add isColor parameter also in integral read functions - and then
// divide by 255 if T is not integral

/**
 * @brief Reads a char (one byte) from a binary stream, and returns it as a
 * type T.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] end: the endianness of the stream.
 * @return A value of type T containing the read char.
 */
template<typename T>
T readChar(std::istream& file, std::endian end = std::endian::native)
{
    char c;
    deserialize(file, c, end);
    return static_cast<T>(c);
}

/**
 * @brief Reads an unsigned char (one byte) from a binary stream, and returns it
 * as a type T.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] end: the endianness of the stream.
 * @return A value of type T containing the read unsigned char.
 */
template<typename T>
T readUChar(std::istream& file, std::endian end = std::endian::native)
{
    unsigned char c;
    deserialize(file, c, end);
    return static_cast<T>(c);
}

/**
 * @brief Reads a short (two bytes) from a binary stream, and returns it as a
 * type T.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] end: the endianness of the stream.
 * @return A value of type T containing the read short.
 */
template<typename T>
T readShort(std::istream& file, std::endian end = std::endian::native)
{
    short c;
    deserialize(file, c, end);
    return static_cast<T>(c);
}

/**
 * @brief Reads an unsigned short (two bytes) from a binary stream, and returns
 * it as a type T.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] end: the endianness of the stream.
 * @return A value of type T containing the read unsigned short.
 */
template<typename T>
T readUShort(std::istream& file, std::endian end = std::endian::native)
{
    unsigned short c;
    deserialize(file, c, end);
    return static_cast<T>(c);
}

/**
 * @brief Reads an int (four bytes) from a binary stream, and returns it as a
 * type T.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] end: the endianness of the stream.
 * @return A value of type T containing the read int.
 */
template<typename T>
T readInt(std::istream& file, std::endian end = std::endian::native)
{
    int c;
    deserialize(file, c, end);
    return static_cast<T>(c);
}

/**
 * @brief Reads an unsigned int (four bytes) from a binary stream, and returns
 * it as a type T.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] end: the endianness of the stream.
 * @return A value of type T containing the read unsigned int.
 */
template<typename T>
T readUInt(std::istream& file, std::endian end = std::endian::native)
{
    uint c;
    deserialize(file, c, end);
    return static_cast<T>(c);
}

/**
 * @brief Reads a float (four bytes) from a binary stream, and returns it as a
 * type T.
 *
 * @note If the read primitive is a color, it is converted in a value that
 * makes sense depending on T. For example, if T is an integral type the value
 * is multiplied by 255. If T is a float, the value is not modified.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] end: the endianness of the stream.
 * @param[in] isColor: whether the primitive is a color (in which case it is
 * converted in a value that makes sense depending on T).
 * @return A value of type T containing the read float.
 */
template<typename T>
T readFloat(
    std::istream& file,
    std::endian   end     = std::endian::native,
    bool          isColor = false)
{
    float c;
    deserialize(file, c, end);
    if constexpr (std::integral<T>) {
        if (isColor)
            return static_cast<T>(c * 255);
    }
    return static_cast<T>(c);
}

/**
 * @brief Reads a double (eight bytes) from a binary stream, and returns it as
 * a type T.
 *
 * @note If the read primitive is a color, it is converted in a value that
 * makes sense depending on T. For example, if T is an integral type the value
 * is multiplied by 255. If T is a float, the value is not modified.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] end: the endianness of the stream.
 * @param[in] isColor: whether the primitive is a color (in which case it is
 * converted in a value that makes sense depending on T).
 * @return A value of type T containing the read double.
 */
template<typename T>
T readDouble(
    std::istream& file,
    std::endian   end     = std::endian::native,
    bool          isColor = false)
{
    double c;
    deserialize(file, c, end);
    if constexpr (std::integral<T>) {
        if (isColor)
            return static_cast<T>(c * 255);
    }
    return static_cast<T>(c);
}

/**
 * @brief Reads a primitive type from a binary stream, and returns it as a type
 * T.
 *
 * @note if you are reading from a txt stream, use first the function
 * readAndTokenizeNextNonEmptyLine to read and tokenize a line, and then use the
 * Tokernizer iterator to read the primitives.
 *
 * @note If the read primitive is a color, it is converted in a value that
 * makes sense depending on T and the primitive. For example, if T is an
 * integral type, and the primitive is a float, the value is multiplied by 255.
 *
 * @tparam T: the type to return.
 * @param[in] file: the stream to read from.
 * @param[in] type: the type of the primitive to read.
 * @param[in] end: the endianness of the stream.
 * @param[in] isColor: whether the primitive is a color (in which case it is
 * converted in a value that makes sense depending on T and type).
 * @return A value of type T containing the read primitive.
 */
template<typename T>
T readPrimitiveType(
    std::istream& file,
    PrimitiveType type,
    std::endian   end     = std::endian::native,
    bool          isColor = false)
{
    T p;
    switch (type) {
    case PrimitiveType::CHAR: p = readChar<T>(file, end); break;
    case PrimitiveType::UCHAR: p = readUChar<T>(file, end); break;
    case PrimitiveType::SHORT: p = readShort<T>(file, end); break;
    case PrimitiveType::USHORT: p = readUShort<T>(file, end); break;
    case PrimitiveType::INT: p = readInt<T>(file, end); break;
    case PrimitiveType::UINT: p = readUInt<T>(file, end); break;
    case PrimitiveType::FLOAT: p = readFloat<T>(file, end, isColor); break;
    case PrimitiveType::DOUBLE: p = readDouble<T>(file, end, isColor); break;
    default: assert(0); p = 0;
    }
    // if I read a color that must be returned as a float or double
    if (isColor && !std::is_integral<T>::value)
        p /= 255.0;
    return p;
}

template<ElementConcept El>
void readCustomComponent(
    std::istream&      file,
    El&                elem,
    const std::string& cName,
    PrimitiveType      type,
    std::endian        end = std::endian::native)
{
    std::type_index ti = elem.customComponentType(cName);
    if (ti == typeid(char))
        elem.template customComponent<char>(cName) =
            readPrimitiveType<char>(file, type, end);
    else if (ti == typeid(unsigned char))
        elem.template customComponent<unsigned char>(cName) =
            readPrimitiveType<unsigned char>(file, type, end);
    else if (ti == typeid(short))
        elem.template customComponent<short>(cName) =
            readPrimitiveType<short>(file, type, end);
    else if (ti == typeid(unsigned short))
        elem.template customComponent<unsigned short>(cName) =
            readPrimitiveType<unsigned short>(file, type, end);
    else if (ti == typeid(int))
        elem.template customComponent<int>(cName) =
            readPrimitiveType<int>(file, type, end);
    else if (ti == typeid(unsigned int))
        elem.template customComponent<uint>(cName) =
            readPrimitiveType<uint>(file, type, end);
    else if (ti == typeid(float))
        elem.template customComponent<float>(cName) =
            readPrimitiveType<float>(file, type, end);
    else if (ti == typeid(double))
        elem.template customComponent<double>(cName) =
            readPrimitiveType<double>(file, type, end);
    else
        assert(0);
}

// read/txt

template<typename T>
T readChar(Tokenizer::iterator& token, std::endian = std::endian::native)
{
    return std::stoi(*token++);
}

template<typename T>
T readUChar(Tokenizer::iterator& token, std::endian = std::endian::native)
{
    return std::stoi(*token++);
}

template<typename T>
T readShort(Tokenizer::iterator& token, std::endian = std::endian::native)
{
    return std::stoi(*token++);
}

template<typename T>
T readUShort(Tokenizer::iterator& token, std::endian = std::endian::native)
{
    return std::stoi(*token++);
}

template<typename T>
T readInt(Tokenizer::iterator& token, std::endian = std::endian::native)
{
    return std::stoi(*token++);
}

template<typename T>
T readUInt(Tokenizer::iterator& token, std::endian = std::endian::native)
{
    return std::stoi(*token++);
}

template<typename T>
T readFloat(
    Tokenizer::iterator& token,
    std::endian  = std::endian::native,
    bool isColor = false)
{
    if (isColor && std::is_integral<T>::value) {
        return std::stod(*token++) * 255;
    }
    else {
        return std::stod(*token++);
    }
}

template<typename T>
T readDouble(
    Tokenizer::iterator& token,
    std::endian  = std::endian::native,
    bool isColor = false)
{
    if (isColor && std::is_integral<T>::value) {
        return std::stod(*token++) * 255;
    }
    else {
        return std::stod(*token++);
    }
}

template<typename T>
T readPrimitiveType(
    Tokenizer::iterator& token,
    PrimitiveType        type,
    std::endian  = std::endian::native,
    bool isColor = false)
{
    T p;
    switch (type) {
    case PrimitiveType::CHAR:
    case PrimitiveType::UCHAR:
    case PrimitiveType::SHORT:
    case PrimitiveType::USHORT:
    case PrimitiveType::INT:
    case PrimitiveType::UINT: p = std::stoi(*token++); break;
    case PrimitiveType::FLOAT:
    case PrimitiveType::DOUBLE:
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
        p /= 255.0;
    return p;
}

template<ElementConcept El>
void readCustomComponent(
    Tokenizer::iterator& token,
    El&                  elem,
    const std::string&   cName,
    PrimitiveType        type,
    std::endian = std::endian::native)
{
    std::type_index ti = elem.customComponentType(cName);
    if (ti == typeid(char))
        elem.template customComponent<char>(cName) =
            readPrimitiveType<char>(token, type);
    else if (ti == typeid(unsigned char))
        elem.template customComponent<unsigned char>(cName) =
            readPrimitiveType<unsigned char>(token, type);
    else if (ti == typeid(short))
        elem.template customComponent<short>(cName) =
            readPrimitiveType<short>(token, type);
    else if (ti == typeid(unsigned short))
        elem.template customComponent<unsigned short>(cName) =
            readPrimitiveType<unsigned short>(token, type);
    else if (ti == typeid(int))
        elem.template customComponent<int>(cName) =
            readPrimitiveType<int>(token, type);
    else if (ti == typeid(unsigned int))
        elem.template customComponent<uint>(cName) =
            readPrimitiveType<uint>(token, type);
    else if (ti == typeid(float))
        elem.template customComponent<float>(cName) =
            readPrimitiveType<float>(token, type);
    else if (ti == typeid(double))
        elem.template customComponent<double>(cName) =
            readPrimitiveType<double>(token, type);
    else
        assert(0);
}

} // namespace io
} // namespace vcl

#endif // VCL_IO_READ_H
