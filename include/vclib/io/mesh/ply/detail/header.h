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

#ifndef VCL_PLY_HEADER_H
#define VCL_PLY_HEADER_H

#include <clocale>
#include <fstream>
#include <string>
#include <vector>

#include <vclib/io/file_info.h>
#include <vclib/io/read.h>
#include <vclib/mesh/utils/mesh_info.h>
#include <vclib/misc/string.h>
#include <vclib/misc/tokenizer.h>

#include "ply.h"

namespace vcl::detail {

/**
 * @brief The PlyHeader class allows to manage the header of a ply file.
 *
 * Following the content of an header, a ply file can be loaded or saved.
 */
class PlyHeader
{
    bool valid = false;

    ply::Format frmt = ply::UNKNOWN;

    std::vector<PlyElement>  elements;
    std::vector<std::string> textureFiles;

    // id in the elements vector for vertices, faces, edges and triStrips
    int vertElemPos = -1;
    int faceElemPos = -1;
    int edgeElemPos = -1;
    int trisElemPos = -1;

    int nextElementID = 0;

public:
    using iterator = std::vector<PlyElement>::const_iterator;

    PlyHeader() = default;

    PlyHeader(
        ply::Format              format,
        const MeshInfo&          info,
        std::vector<std::string> textureFiles = std::vector<std::string>()) :
            valid(true),
            frmt(format)
    {
        setInfo(info, textureFiles, format == ply::BINARY);
    }

    PlyHeader(std::istream& file, const std::string& filename = "")
    {
        clear();

        std::string line;
        std::getline(file, line);
        str::removeWindowsNewLine(line);
        if (line.compare(0, 3, "ply") == 0) {
            bool        error        = false;
            bool        firstElement = true;
            std::string headerLine;
            PlyElement  element;
            do {
                vcl::Tokenizer spaceTokenizer =
                    readAndTokenizeNextNonEmptyLine(file);
                if (!error) {
                    vcl::Tokenizer::iterator token = spaceTokenizer.begin();
                    headerLine                     = *token;
                    if (headerLine == "format") {
                        token++;
                        if (*token == "ascii")
                            frmt = ply::ASCII;
                        else if (
                            *token == "binary_big_endian" ||
                            *token == "binary_little_endian" ||
                            *token == "binary")
                            frmt = ply::BINARY;
                    }
                    // reading a comment, may be a texture file...
                    else if (headerLine == "comment") {
                        token++;
                        if (token != spaceTokenizer.end()) {
                            if (vcl::str::containsCaseInsensitive(
                                    *token, "texture"))
                            {
                                ++token;
                                if (token != spaceTokenizer.end()) {
                                    std::string textName = *token;
                                    auto it = vcl::str::findCaseInsensitive(
                                        textName, "<this>");
                                    if (it != textName.end()) {
                                        uint        pos = it - textName.begin();
                                        std::string fn  = vcl::FileInfo::
                                            fileNameWithoutExtension(filename);
                                        textName =
                                            textName.substr(0, pos) + fn +
                                            textName.substr(
                                                pos + 6, textName.size());
                                    }
                                    textureFiles.push_back(textName);
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
                                vertElemPos = (long int) elements.size();
                            if (element.type == ply::FACE)
                                faceElemPos = (long int) elements.size();
                            if (element.type == ply::EDGE)
                                edgeElemPos = (long int) elements.size();
                            if (element.type == ply::TRISTRIP)
                                trisElemPos = (long int) elements.size();
                            elements.push_back(element);
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
                            vertElemPos = (long int) elements.size();
                        if (element.type == ply::FACE)
                            faceElemPos = (long int) elements.size();
                        if (element.type == ply::EDGE)
                            edgeElemPos = (long int) elements.size();
                        if (element.type == ply::TRISTRIP)
                            trisElemPos = (long int) elements.size();
                        elements.push_back(element);
                    }
                }
            } while (!error && headerLine != "end_header");
            valid = !error && hasVertices();
        }
    }

    void clear()
    {
        frmt = ply::UNKNOWN;
        elements.clear();
        textureFiles.clear();
        valid       = false;
        vertElemPos = -1;
        faceElemPos = -1;
        edgeElemPos = -1;
        trisElemPos = -1;
    }

    bool isValid() const { return valid; }

    ply::Format format() const { return frmt; }

    MeshInfo getInfo() const
    {
        MeshInfo mod;
        // x, y, z, nx, ny, nz, red, green, blue, alpha, vertex_indices

        if (vertElemPos >= 0) {
            mod.setVertices();
            for (const PlyProperty& p : elements[vertElemPos].properties) {
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
        if (faceElemPos >= 0) {
            mod.setFaces();
            for (const PlyProperty& p : elements[faceElemPos].properties) {
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
        if (trisElemPos >= 0) {
            mod.setFaces();
            for (const PlyProperty& p : elements[trisElemPos].properties) {
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
        if (textureFiles.size() > 0) {
            mod.setTextures(true);
        }
        return mod;
    }

    bool hasVertices() const { return vertElemPos >= 0; }

    bool hasFaces() const { return faceElemPos >= 0; }

    bool hasEdges() const { return edgeElemPos >= 0; }

    bool hasTriStrips() const { return trisElemPos >= 0; }

    bool hasTextureFileNames() const { return textureFiles.size() > 0; }

    uint numberVertices() const
    {
        assert(hasVertices());
        return elements[vertElemPos].numberElements;
    }

    uint numberFaces() const
    {
        assert(hasFaces());
        return elements[faceElemPos].numberElements;
    }

    uint numberEdges() const
    {
        assert(hasEdges());
        return elements[edgeElemPos].numberElements;
    }

    uint numberTriStrips() const
    {
        assert(hasTriStrips());
        return elements[trisElemPos].numberElements;
    }

    uint numberTextureFileNames() const { return textureFiles.size(); }

    const std::list<PlyProperty>& vertexProperties() const
    {
        assert(hasVertices());
        return elements[vertElemPos].properties;
    }

    const std::list<PlyProperty>& faceProperties() const
    {
        assert(hasFaces());
        return elements[faceElemPos].properties;
    }

    const std::list<PlyProperty>& edgeProperties() const
    {
        assert(hasEdges());
        return elements[edgeElemPos].properties;
    }

    const std::list<PlyProperty>& triStripsProperties() const
    {
        assert(hasTriStrips());
        return elements[trisElemPos].properties;
    }

    const std::vector<std::string>& textureFileNames() const
    {
        return textureFiles;
    }

    bool errorWhileLoading() const { return !valid; }

    void setNumberVertices(unsigned long int nV)
    {
        if (vertElemPos < 0)
            vertElemPos = nextElementID++;
        elements[vertElemPos].numberElements = nV;
    }

    void setNumberFaces(unsigned long int nF)
    {
        if (faceElemPos < 0)
            faceElemPos = nextElementID++;
        elements[faceElemPos].numberElements = nF;
    }

    void setNumberEdges(unsigned long int nE)
    {
        if (edgeElemPos < 0)
            edgeElemPos = nextElementID++;
        elements[edgeElemPos].numberElements = nE;
    }

    void pushTextureFileName(const std::string& tn)
    {
        textureFiles.push_back(tn);
    }

    void setInfo(
        const MeshInfo&          info,
        std::vector<std::string> textureFileNames = std::vector<std::string>(),
        bool                     binary           = true)
    {
        clear();
        frmt         = binary ? ply::BINARY : ply::ASCII;
        valid        = true;
        textureFiles = textureFileNames;
        if (info.hasVertices()) {
            vertElemPos = nextElementID++;
            PlyElement vElem;
            vElem.type = ply::VERTEX;
            if (info.hasVertexCoords()) {
                PlyProperty px, py, pz;
                px.name = ply::x;
                px.type = (ply::PropertyType) info.vertexCoordsType();
                py.name = ply::y;
                py.type = (ply::PropertyType) info.vertexCoordsType();
                pz.name = ply::z;
                pz.type = (ply::PropertyType) info.vertexCoordsType();
                vElem.properties.push_back(px);
                vElem.properties.push_back(py);
                vElem.properties.push_back(pz);
            }
            if (info.hasVertexNormals()) {
                PlyProperty vnx, vny, vnz;
                vnx.name = ply::nx;
                vnx.type = (ply::PropertyType) info.vertexNormalsType();
                vny.name = ply::ny;
                vny.type = (ply::PropertyType) info.vertexNormalsType();
                vnz.name = ply::nz;
                vnz.type = (ply::PropertyType) info.vertexNormalsType();
                vElem.properties.push_back(vnx);
                vElem.properties.push_back(vny);
                vElem.properties.push_back(vnz);
            }
            if (info.hasVertexColors()) {
                PlyProperty vcr, vcg, vcb, vca;
                vcr.name = ply::red;
                vcr.type = (ply::PropertyType) info.vertexColorsType();
                vcg.name = ply::green;
                vcg.type = (ply::PropertyType) info.vertexColorsType();
                vcb.name = ply::blue;
                vcb.type = (ply::PropertyType) info.vertexColorsType();
                vca.name = ply::alpha;
                vca.type = (ply::PropertyType) info.vertexColorsType();
                vElem.properties.push_back(vcr);
                vElem.properties.push_back(vcg);
                vElem.properties.push_back(vcb);
                vElem.properties.push_back(vca);
            }
            if (info.hasVertexQuality()) {
                PlyProperty vs;
                vs.name = ply::quality;
                vs.type = (ply::PropertyType) info.vertexQualityType();
                vElem.properties.push_back(vs);
            }
            if (info.hasVertexTexCoords()) {
                PlyProperty tcu, tcv, tcn;
                tcu.name = ply::texture_u;
                tcu.type = (ply::PropertyType) info.vertexTexCoordsType();
                tcv.name = ply::texture_v;
                tcv.type = (ply::PropertyType) info.vertexTexCoordsType();
                tcn.name = ply::texnumber;
                tcn.type = USHORT;
                vElem.properties.push_back(tcu);
                vElem.properties.push_back(tcv);
                vElem.properties.push_back(tcn);
            }
            if (info.hasVertexCustomComponents()) {
                for (const auto& cc : info.vertexCustomComponents()) {
                    if (cc.type <= DOUBLE) {
                        PlyProperty pp;
                        pp.name                = ply::unknown;
                        pp.unknownPropertyName = cc.name;
                        pp.type                = (ply::PropertyType) cc.type;
                        vElem.properties.push_back(pp);
                    }
                }
            }
            elements.push_back(vElem);
        }
        if (info.hasFaces()) {
            faceElemPos = nextElementID++;
            PlyElement fElem;
            fElem.type = ply::FACE;
            if (info.hasFaceVRefs()) {
                PlyProperty vids;
                vids.list         = true;
                vids.name         = ply::vertex_indices;
                vids.type         = UINT;
                vids.listSizeType = UCHAR;
                fElem.properties.push_back(vids);
            }
            if (info.hasFaceNormals()) {
                PlyProperty fnx, fny, fnz;
                fnx.name = ply::nx;
                fnx.type = (ply::PropertyType) info.faceNormalsType();
                fny.name = ply::ny;
                fny.type = (ply::PropertyType) info.faceNormalsType();
                fnz.name = ply::nz;
                fnz.type = (ply::PropertyType) info.faceNormalsType();
                fElem.properties.push_back(fnx);
                fElem.properties.push_back(fny);
                fElem.properties.push_back(fnz);
            }
            if (info.hasFaceColors()) {
                PlyProperty fcr, fcg, fcb, fca;
                fcr.name = ply::red;
                fcr.type = (ply::PropertyType) info.faceColorsType();
                fcg.name = ply::green;
                fcg.type = (ply::PropertyType) info.faceColorsType();
                fcb.name = ply::blue;
                fcb.type = (ply::PropertyType) info.faceColorsType();
                fca.name = ply::alpha;
                fca.type = (ply::PropertyType) info.faceColorsType();
                fElem.properties.push_back(fcr);
                fElem.properties.push_back(fcg);
                fElem.properties.push_back(fcb);
                fElem.properties.push_back(fca);
            }
            if (info.hasFaceQuality()) {
                PlyProperty fs;
                fs.name = ply::quality;
                fs.type = (ply::PropertyType) info.faceQualityType();
                fElem.properties.push_back(fs);
            }
            if (info.hasFaceWedgeTexCoords()) {
                PlyProperty tc, tn;
                tc.list         = true;
                tc.listSizeType = UCHAR;
                tc.name         = ply::texcoord;
                tc.type = (ply::PropertyType) info.faceWedgeTexCoordsType();
                tn.name = ply::texnumber;
                tn.type = USHORT;
                fElem.properties.push_back(tc);
                fElem.properties.push_back(tn);
            }
            if (info.hasFaceCustomComponents()) {
                for (const auto& cc : info.faceCustomComponents()) {
                    if (cc.type <= DOUBLE) {
                        PlyProperty pp;
                        pp.name                = ply::unknown;
                        pp.unknownPropertyName = cc.name;
                        pp.type                = (ply::PropertyType) cc.type;
                        fElem.properties.push_back(pp);
                    }
                }
            }
            elements.push_back(fElem);
        }
        if (info.hasEdges()) {
            edgeElemPos = nextElementID++;
            PlyElement eElem;
            eElem.type = ply::EDGE;
            if (info.hasEdgeVRefs()) {
                PlyProperty v1;
                v1.name = ply::vertex1;
                v1.type = UINT;
                eElem.properties.push_back(v1);
                PlyProperty v2;
                v2.name = ply::vertex2;
                v2.type = UINT;
                eElem.properties.push_back(v2);
            }
            elements.push_back(eElem);
        }
    }

    std::string toString() const
    {
        std::string s;

        s += "ply\nformat ";
        s +=
            (frmt == ply::ASCII ? "ascii 1.0\n" : "binary_little_endian 1.0\n");
        s += "comment Generated by vclib\n";
        for (const std::string& str : textureFiles) {
            s += "comment TextureFile " + str + "\n";
        }
        for (const PlyElement& e : elements) {
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

    void setFormat(ply::Format f) { frmt = f; }

    iterator begin() const { return elements.begin(); }

    iterator end() const { return elements.end(); }

private:
    PlyElement readElement(const vcl::Tokenizer& lineTokenizer) const
    {
        PlyElement               e;
        vcl::Tokenizer::iterator token = lineTokenizer.begin();
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

    PlyProperty readProperty(const vcl::Tokenizer& lineTokenizer) const
    {
        PlyProperty              p;
        vcl::Tokenizer::iterator token = lineTokenizer.begin();
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

    ply::PropertyType stringToType(const std::string& type) const
    {
        ply::PropertyType pt = UCHAR;
        if (type == "char")
            pt = CHAR;
        if (type == "uchar")
            pt = UCHAR;
        if (type == "short")
            pt = SHORT;
        if (type == "ushort")
            pt = USHORT;
        if (type == "int")
            pt = INT;
        if (type == "uint")
            pt = UINT;
        if (type == "float")
            pt = FLOAT;
        if (type == "double")
            pt = DOUBLE;
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

    std::string typeToString(ply::PropertyType t) const
    {
        switch (t) {
        case CHAR: return "char";
        case UCHAR: return "uchar";
        case SHORT: return "short";
        case USHORT: return "ushort";
        case INT: return "int";
        case UINT: return "uint";
        case FLOAT: return "float";
        case DOUBLE: return "double";
        case NONE: return "";
        }
        return "";
    }
};

} // namespace vcl::detail

#endif // VCL_PLY_HEADER_H
