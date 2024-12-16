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

#ifndef VCL_LOAD_SAVE_PLY_DETAIL_HEADER_H
#define VCL_LOAD_SAVE_PLY_DETAIL_HEADER_H

#include "ply.h"

#include <vclib/io/file_info.h>
#include <vclib/io/read.h>
#include <vclib/misc/string.h>
#include <vclib/misc/tokenizer.h>
#include <vclib/space/complex/mesh_info.h>

#include <clocale>
#include <string>
#include <vector>

namespace vcl::detail {

/**
 * @brief The PlyHeader class allows to manage the header of a ply file.
 *
 * Following the content of an header, a ply file can be loaded or saved.
 */
class PlyHeader
{
    bool mValid = false;

    ply::Format mFormat = ply::UNKNOWN;

    std::vector<PlyElement>  mElements;
    std::vector<std::string> mTextureFiles;

    // for each element, its position in the mElements vector
    uint mVertElemPos     = UINT_NULL;
    uint mFaceElemPos     = UINT_NULL;
    uint mEdgeElemPos     = UINT_NULL;
    uint mTriStripElemPos = UINT_NULL;

public:
    using iterator = std::vector<PlyElement>::const_iterator;

    PlyHeader() = default;

    PlyHeader(
        ply::Format              format,
        const MeshInfo&          info,
        std::vector<std::string> textureFiles = std::vector<std::string>()) :
            mValid(true), mFormat(format)
    {
        setInfo(info, textureFiles, format);
    }

    PlyHeader(std::istream& file, const std::string& filename = "")
    {
        clear();

        std::string line;
        std::getline(file, line);
        removeCarriageReturn(line);
        if (line.compare(0, 3, "ply") == 0) {
            bool        error        = false;
            bool        firstElement = true;
            std::string headerLine;
            PlyElement  element;
            do {
                Tokenizer spaceTokenizer =
                    readAndTokenizeNextNonEmptyLine(file);
                if (!error) {
                    Tokenizer::iterator token = spaceTokenizer.begin();
                    headerLine                     = *token;
                    if (headerLine == "format") {
                        token++;
                        if (*token == "ascii")
                            mFormat = ply::ASCII;
                        else if (
                            *token == "binary_little_endian" ||
                            *token == "binary")
                            mFormat = ply::BINARY_LITTLE_ENDIAN;
                        else if (*token == "binary_big_endian")
                            mFormat = ply::BINARY_BIG_ENDIAN;
                    }
                    // reading a comment, may be a texture file...
                    else if (headerLine == "comment") {
                        token++;
                        if (token != spaceTokenizer.end()) {
                            if (containsCaseInsensitive(
                                    *token, "texture")) {
                                ++token;
                                if (token != spaceTokenizer.end()) {
                                    std::string textName = *token;
                                    auto        it = findCaseInsensitive(
                                        textName, "<this>");
                                    if (it != textName.end()) {
                                        uint        pos = it - textName.begin();
                                        std::string fn  = FileInfo::
                                            fileNameWithoutExtension(filename);
                                        textName =
                                            textName.substr(0, pos) + fn +
                                            textName.substr(
                                                pos + 6, textName.size());
                                    }
                                    mTextureFiles.push_back(textName);
                                }
                            }
                        }
                    }
                    // I am reading a new element
                    else if (headerLine == "element") {
                        // if it is not the first element to read, it means that
                        // the previous one needs to be saved
                        if (!firstElement) {
                            // index of each element type in elements vector
                            if (element.type == ply::VERTEX)
                                mVertElemPos = mElements.size();
                            if (element.type == ply::FACE)
                                mFaceElemPos = mElements.size();
                            if (element.type == ply::EDGE)
                                mEdgeElemPos = mElements.size();
                            if (element.type == ply::TRISTRIP)
                                mTriStripElemPos = mElements.size();
                            mElements.push_back(element);
                            element = PlyElement();
                        }
                        element      = readElement(spaceTokenizer);
                        firstElement = false;
                    }
                    else if (headerLine == "property") {
                        PlyProperty p = readProperty(spaceTokenizer);
                        element.properties.push_back(p);
                    }
                    // save the last element
                    else if (headerLine == "end_header") {
                        if (element.type == ply::VERTEX)
                            mVertElemPos = mElements.size();
                        if (element.type == ply::FACE)
                            mFaceElemPos = mElements.size();
                        if (element.type == ply::EDGE)
                            mEdgeElemPos = mElements.size();
                        if (element.type == ply::TRISTRIP)
                            mTriStripElemPos = mElements.size();
                        mElements.push_back(element);
                    }
                }
            } while (!error && headerLine != "end_header");
            mValid = !error && hasVertices();
        }
    }

    void clear()
    {
        mFormat = ply::UNKNOWN;
        mElements.clear();
        mTextureFiles.clear();
        mValid = false;

        mVertElemPos     = UINT_NULL;
        mFaceElemPos     = UINT_NULL;
        mEdgeElemPos     = UINT_NULL;
        mTriStripElemPos = UINT_NULL;
    }

    bool isValid() const { return mValid; }

    ply::Format format() const { return mFormat; }

    MeshInfo getInfo() const
    {
        MeshInfo mod;
        // x, y, z, nx, ny, nz, red, green, blue, alpha, vertex_indices

        if (mVertElemPos != UINT_NULL) {
            mod.setVertices();
            for (const PlyProperty& p : mElements[mVertElemPos].properties) {
                switch (p.name) {
                case ply::x:
                case ply::y:
                case ply::z: mod.setVertexCoords(); break;
                case ply::nx:
                case ply::ny:
                case ply::nz: mod.setVertexNormals(); break;
                case ply::red:
                case ply::green:
                case ply::blue:
                case ply::alpha: mod.setVertexColors(); break;
                case ply::quality: mod.setVertexQuality(); break;
                case ply::texture_u: mod.setVertexTexCoords(); break;
                case ply::unknown:
                    if (p.type <= ply::PropertyType::DOUBLE) {
                        mod.addVertexCustomComponent(
                            p.unknownPropertyName, (MeshInfo::DataType) p.type);
                    }
                default: break;
                }
            }
        }
        if (mFaceElemPos != UINT_NULL) {
            mod.setFaces();
            for (const PlyProperty& p : mElements[mFaceElemPos].properties) {
                switch (p.name) {
                case ply::vertex_indices: mod.setFaceVRefs(); break;
                case ply::nx:
                case ply::ny:
                case ply::nz: mod.setFaceNormals(); break;
                case ply::red:
                case ply::green:
                case ply::blue:
                case ply::alpha: mod.setFaceColors(); break;
                case ply::quality: mod.setFaceQuality(); break;
                case ply::texcoord: mod.setFaceWedgeTexCoords(); break;
                case ply::unknown:
                    if (p.type <= ply::PropertyType::DOUBLE) {
                        mod.addFaceCustomComponent(
                            p.unknownPropertyName, (MeshInfo::DataType) p.type);
                    }
                default: break;
                }
            }
        }
        if (mTriStripElemPos != UINT_NULL) {
            mod.setFaces();
            for (const PlyProperty& p :
                 mElements[mTriStripElemPos].properties) {
                switch (p.name) {
                case ply::vertex_indices: mod.setFaceVRefs(); break;
                case ply::nx:
                case ply::ny:
                case ply::nz: mod.setFaceNormals(); break;
                case ply::red:
                case ply::green:
                case ply::blue:
                case ply::alpha: mod.setFaceColors(); break;
                case ply::quality: mod.setFaceQuality(); break;
                case ply::texcoord: mod.setFaceWedgeTexCoords(); break;
                default: break;
                }
            }
        }
        if (mTextureFiles.size() > 0) {
            mod.setTextures(true);
        }
        return mod;
    }

    bool hasVertices() const { return mVertElemPos != UINT_NULL; }

    bool hasFaces() const { return mFaceElemPos != UINT_NULL; }

    bool hasEdges() const { return mEdgeElemPos != UINT_NULL; }

    bool hasTriStrips() const { return mTriStripElemPos != UINT_NULL; }

    bool hasTextureFileNames() const { return mTextureFiles.size() > 0; }

    uint numberVertices() const
    {
        assert(hasVertices());
        return mElements[mVertElemPos].numberElements;
    }

    uint numberFaces() const
    {
        assert(hasFaces());
        return mElements[mFaceElemPos].numberElements;
    }

    uint numberEdges() const
    {
        assert(hasEdges());
        return mElements[mEdgeElemPos].numberElements;
    }

    uint numberTriStrips() const
    {
        assert(hasTriStrips());
        return mElements[mTriStripElemPos].numberElements;
    }

    uint numberTextureFileNames() const { return mTextureFiles.size(); }

    const std::list<PlyProperty>& vertexProperties() const
    {
        assert(hasVertices());
        return mElements[mVertElemPos].properties;
    }

    const std::list<PlyProperty>& faceProperties() const
    {
        assert(hasFaces());
        return mElements[mFaceElemPos].properties;
    }

    const std::list<PlyProperty>& edgeProperties() const
    {
        assert(hasEdges());
        return mElements[mEdgeElemPos].properties;
    }

    const std::list<PlyProperty>& triStripsProperties() const
    {
        assert(hasTriStrips());
        return mElements[mTriStripElemPos].properties;
    }

    const std::vector<std::string>& textureFileNames() const
    {
        return mTextureFiles;
    }

    bool errorWhileLoading() const { return !mValid; }

    void setNumberVertices(unsigned long int nV)
    {
        assert(hasVertices());
        mElements[mVertElemPos].numberElements = nV;
    }

    void setNumberFaces(unsigned long int nF)
    {
        assert(hasFaces());
        mElements[mFaceElemPos].numberElements = nF;
    }

    void setNumberEdges(unsigned long int nE)
    {
        assert(hasEdges());
        mElements[mEdgeElemPos].numberElements = nE;
    }

    void pushTextureFileName(const std::string& tn)
    {
        mTextureFiles.push_back(tn);
    }

    void setInfo(
        const MeshInfo&          info,
        std::vector<std::string> textureFileNames = std::vector<std::string>(),
        ply::Format              format           = ply::BINARY_LITTLE_ENDIAN)
    {
        clear();
        mFormat       = format;
        mValid        = true;
        mTextureFiles = textureFileNames;
        if (info.hasVertices()) {
            mVertElemPos = mElements.size();
            PlyElement vElem;
            vElem.type = ply::VERTEX;
            if (info.hasVertexCoords()) {
                PlyProperty px, py, pz;
                px.name = ply::x;
                px.type = info.vertexCoordsType();
                py.name = ply::y;
                py.type = info.vertexCoordsType();
                pz.name = ply::z;
                pz.type = info.vertexCoordsType();
                vElem.properties.push_back(px);
                vElem.properties.push_back(py);
                vElem.properties.push_back(pz);
            }
            if (info.hasVertexNormals()) {
                PlyProperty vnx, vny, vnz;
                vnx.name = ply::nx;
                vnx.type = info.vertexNormalsType();
                vny.name = ply::ny;
                vny.type = info.vertexNormalsType();
                vnz.name = ply::nz;
                vnz.type = info.vertexNormalsType();
                vElem.properties.push_back(vnx);
                vElem.properties.push_back(vny);
                vElem.properties.push_back(vnz);
            }
            if (info.hasVertexColors()) {
                PlyProperty vcr, vcg, vcb, vca;
                vcr.name = ply::red;
                vcr.type = info.vertexColorsType();
                vcg.name = ply::green;
                vcg.type = info.vertexColorsType();
                vcb.name = ply::blue;
                vcb.type = info.vertexColorsType();
                vca.name = ply::alpha;
                vca.type = info.vertexColorsType();
                vElem.properties.push_back(vcr);
                vElem.properties.push_back(vcg);
                vElem.properties.push_back(vcb);
                vElem.properties.push_back(vca);
            }
            if (info.hasVertexQuality()) {
                PlyProperty vs;
                vs.name = ply::quality;
                vs.type = info.vertexQualityType();
                vElem.properties.push_back(vs);
            }
            if (info.hasVertexTexCoords()) {
                PlyProperty tcu, tcv, tcn;
                tcu.name = ply::texture_u;
                tcu.type = info.vertexTexCoordsType();
                tcv.name = ply::texture_v;
                tcv.type = info.vertexTexCoordsType();
                tcn.name = ply::texnumber;
                tcn.type = PrimitiveType::USHORT;
                vElem.properties.push_back(tcu);
                vElem.properties.push_back(tcv);
                vElem.properties.push_back(tcn);
            }
            if (info.hasVertexCustomComponents()) {
                for (const auto& cc : info.vertexCustomComponents()) {
                    if (cc.type <= PrimitiveType::DOUBLE) {
                        PlyProperty pp;
                        pp.name                = ply::unknown;
                        pp.unknownPropertyName = cc.name;
                        pp.type                = cc.type;
                        vElem.properties.push_back(pp);
                    }
                }
            }
            mElements.push_back(vElem);
        }
        if (info.hasFaces()) {
            mFaceElemPos = mElements.size();
            PlyElement fElem;
            fElem.type = ply::FACE;
            if (info.hasFaceVRefs()) {
                PlyProperty vids;
                vids.list         = true;
                vids.name         = ply::vertex_indices;
                vids.type         = PrimitiveType::UINT;
                vids.listSizeType = PrimitiveType::UCHAR;
                fElem.properties.push_back(vids);
            }
            if (info.hasFaceNormals()) {
                PlyProperty fnx, fny, fnz;
                fnx.name = ply::nx;
                fnx.type = info.faceNormalsType();
                fny.name = ply::ny;
                fny.type = info.faceNormalsType();
                fnz.name = ply::nz;
                fnz.type = info.faceNormalsType();
                fElem.properties.push_back(fnx);
                fElem.properties.push_back(fny);
                fElem.properties.push_back(fnz);
            }
            if (info.hasFaceColors()) {
                PlyProperty fcr, fcg, fcb, fca;
                fcr.name = ply::red;
                fcr.type = info.faceColorsType();
                fcg.name = ply::green;
                fcg.type = info.faceColorsType();
                fcb.name = ply::blue;
                fcb.type = info.faceColorsType();
                fca.name = ply::alpha;
                fca.type = info.faceColorsType();
                fElem.properties.push_back(fcr);
                fElem.properties.push_back(fcg);
                fElem.properties.push_back(fcb);
                fElem.properties.push_back(fca);
            }
            if (info.hasFaceQuality()) {
                PlyProperty fs;
                fs.name = ply::quality;
                fs.type = (ply::PropertyType::Enum) info.faceQualityType();
                fElem.properties.push_back(fs);
            }
            if (info.hasFaceWedgeTexCoords()) {
                PlyProperty tc, tn;
                tc.list         = true;
                tc.listSizeType = PrimitiveType::UCHAR;
                tc.name         = ply::texcoord;
                tc.type =
                    (ply::PropertyType::Enum) info.faceWedgeTexCoordsType();
                tn.name = ply::texnumber;
                tn.type = PrimitiveType::USHORT;
                fElem.properties.push_back(tc);
                fElem.properties.push_back(tn);
            }
            if (info.hasFaceCustomComponents()) {
                for (const auto& cc : info.faceCustomComponents()) {
                    if (cc.type <= PrimitiveType::DOUBLE) {
                        PlyProperty pp;
                        pp.name                = ply::unknown;
                        pp.unknownPropertyName = cc.name;
                        pp.type = (ply::PropertyType::Enum) cc.type;
                        fElem.properties.push_back(pp);
                    }
                }
            }
            mElements.push_back(fElem);
        }
        if (info.hasEdges()) {
            mEdgeElemPos = mElements.size();
            PlyElement eElem;
            eElem.type = ply::EDGE;
            if (info.hasEdgeVRefs()) {
                PlyProperty v1;
                v1.name = ply::vertex1;
                v1.type = PrimitiveType::UINT;
                eElem.properties.push_back(v1);
                PlyProperty v2;
                v2.name = ply::vertex2;
                v2.type = PrimitiveType::UINT;
                eElem.properties.push_back(v2);
            }
            mElements.push_back(eElem);
        }
    }

    std::string toString() const
    {
        std::string s;

        s += "ply\nformat ";

        switch (mFormat) {
        case ply::ASCII: s += "ascii 1.0\n"; break;
        case ply::BINARY_BIG_ENDIAN: s += "binary_big_endian 1.0\n"; break;
        default: s += "binary_little_endian 1.0\n"; break;
        }

        s += "comment Generated by vclib\n";
        for (const std::string& str : mTextureFiles) {
            s += "comment TextureFile " + str + "\n";
        }
        for (const PlyElement& e : mElements) {
            s += "element ";
            switch (e.type) {
            case ply::VERTEX:
                s += "vertex " + std::to_string(e.numberElements) + "\n";
                break;
            case ply::FACE:
                s += "face " + std::to_string(e.numberElements) + "\n";
                break;
            case ply::EDGE:
                s += "edge " + std::to_string(e.numberElements) + "\n";
                break;
            case ply::TRISTRIP:
                s += "tristrips " + std::to_string(e.numberElements) + "\n";
                break;
            case ply::MATERIAL:
                s += "material " + std::to_string(e.numberElements) + "\n";
                break;
            case ply::OTHER:
                s += e.unknownElementType + " " +
                     std::to_string(e.numberElements) + "\n";
                break;
            }
            for (const PlyProperty& p : e.properties) {
                s += "property ";
                if (p.list) {
                    s += "list ";
                    s += typeToString(p.listSizeType) + " ";
                }
                s += typeToString(p.type) + " ";
                if (p.name == ply::unknown)
                    s += p.unknownPropertyName + "\n";
                else
                    s += nameToString(p.name) + "\n";
            }
        }
        s += "end_header\n";

        return s;
    }

    void setFormat(ply::Format f) { mFormat = f; }

    iterator begin() const { return mElements.begin(); }

    iterator end() const { return mElements.end(); }

private:
    PlyElement readElement(const Tokenizer& lineTokenizer) const
    {
        PlyElement               e;
        Tokenizer::iterator token = lineTokenizer.begin();
        std::string              s     = *(++token);
        if (s == "vertex") {
            e.type           = ply::VERTEX;
            e.numberElements = std::stoi(*(++token));
        }
        else if (s == "face") {
            e.type           = ply::FACE;
            e.numberElements = std::stoi(*(++token));
        }
        else if (s == "edge") {
            e.type           = ply::EDGE;
            e.numberElements = std::stoi(*(++token));
        }
        else if (s == "tristrips") {
            e.type           = ply::TRISTRIP;
            e.numberElements = std::stoi(*(++token));
        }
        else {
            e.type               = ply::OTHER;
            e.numberElements     = std::stoi(*(++token));
            e.unknownElementType = s;
        }
        return e;
    }

    PlyProperty readProperty(const Tokenizer& lineTokenizer) const
    {
        PlyProperty              p;
        Tokenizer::iterator token = lineTokenizer.begin();
        std::string              type  = *(++token);
        if (type == "list") {
            p.list               = true;
            std::string typeSize = *(++token);
            std::string typeData = *(++token);
            std::string name     = *(++token);
            p.listSizeType       = stringToType(typeSize);
            p.type               = stringToType(typeData);
            p.name               = stringToName(name);
            if (p.name == ply::unknown)
                p.unknownPropertyName = name;
        }
        else {
            p.list           = false;
            std::string name = *(++token);
            p.type           = stringToType(type);
            p.name           = stringToName(name);
            if (p.name == ply::unknown)
                p.unknownPropertyName = name;
        }

        return p;
    }

    ply::PropertyName stringToName(const std::string& name) const
    {
        ply::PropertyName pn = ply::unknown;
        if (name == "x")
            pn = ply::x;
        if (name == "y")
            pn = ply::y;
        if (name == "z")
            pn = ply::z;
        if (name == "nx")
            pn = ply::nx;
        if (name == "ny")
            pn = ply::ny;
        if (name == "nz")
            pn = ply::nz;
        if (name == "red")
            pn = ply::red;
        if (name == "green")
            pn = ply::green;
        if (name == "blue")
            pn = ply::blue;
        if (name == "alpha")
            pn = ply::alpha;
        if (name == "quality" || name == "scalar")
            pn = ply::quality;
        if (name == "texture_u")
            pn = ply::texture_u;
        if (name == "texture_v")
            pn = ply::texture_v;
        if (name == "texnumber")
            pn = ply::texnumber;
        if (name == "vertex_indices")
            pn = ply::vertex_indices;
        if (name == "texcoord")
            pn = ply::texcoord;
        if (name == "vertex1")
            pn = ply::vertex1;
        if (name == "vertex2")
            pn = ply::vertex2;

        return pn;
    }

    ply::PropertyType::Enum stringToType(const std::string& type) const
    {
        ply::PropertyType::Enum pt = ply::PropertyType::UCHAR;
        if (type == "char")
            pt = ply::PropertyType::CHAR;
        if (type == "uchar")
            pt = ply::PropertyType::UCHAR;
        if (type == "short")
            pt = ply::PropertyType::SHORT;
        if (type == "ushort")
            pt = ply::PropertyType::USHORT;
        if (type == "int")
            pt = ply::PropertyType::INT;
        if (type == "uint")
            pt = ply::PropertyType::UINT;
        if (type == "float")
            pt = ply::PropertyType::FLOAT;
        if (type == "double")
            pt = ply::PropertyType::DOUBLE;
        return pt;
    }

    std::string nameToString(ply::PropertyName n) const
    {
        switch (n) {
        case ply::x: return "x";
        case ply::y: return "y";
        case ply::z: return "z";
        case ply::nx: return "nx";
        case ply::ny: return "ny";
        case ply::nz: return "nz";
        case ply::red: return "red";
        case ply::green: return "green";
        case ply::blue: return "blue";
        case ply::alpha: return "alpha";
        case ply::quality: return "quality";
        case ply::texture_u: return "texture_u";
        case ply::texture_v: return "texture_v";
        case ply::texnumber: return "texnumber";
        case ply::vertex_indices: return "vertex_indices";
        case ply::texcoord: return "texcoord";
        case ply::vertex1: return "vertex1";
        case ply::vertex2: return "vertex2";
        default: return "unknown";
        }
    }

    std::string typeToString(ply::PropertyType::Enum t) const
    {
        switch (t) {
        case ply::PropertyType::CHAR: return "char";
        case ply::PropertyType::UCHAR: return "uchar";
        case ply::PropertyType::SHORT: return "short";
        case ply::PropertyType::USHORT: return "ushort";
        case ply::PropertyType::INT: return "int";
        case ply::PropertyType::UINT: return "uint";
        case ply::PropertyType::FLOAT: return "float";
        case ply::PropertyType::DOUBLE: return "double";
        case ply::PropertyType::NONE: return "";
        }
        return "";
    }
};

} // namespace vcl::detail

#endif // VCL_LOAD_SAVE_PLY_DETAIL_HEADER_H
