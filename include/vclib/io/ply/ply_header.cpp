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

#include "ply_header.h"
#include <clocale>

#include <vclib/misc/file_info.h>
#include <vclib/misc/string.h>
#include <vclib/misc/tokenizer.h>

#include "../internal/io_utils.h"

namespace vcl::io::ply {

inline PlyHeader::PlyHeader() :
		frmt(ply::UNKNOWN),
		valid(false),
		vertElemPos(-1),
		faceElemPos(-1),
		edgeElemPos(-1),
		trisElemPos(-1)
{
}

inline PlyHeader::PlyHeader(
	Format                   format,
	const FileMeshInfo&      info,
	std::vector<std::string> textureFiles) :
		frmt(format),
		valid(true),
		vertElemPos(-1),
		faceElemPos(-1),
		edgeElemPos(-1),
		trisElemPos(-1)
{
	setInfo(info, textureFiles, format == BINARY);
}

inline PlyHeader::PlyHeader(const std::string& filename, std::ifstream& file) :
		frmt(ply::UNKNOWN), valid(false)
{
	clear();
	if (file.is_open()) {
		std::string line;
		std::getline(file, line);
		str::removeWindowsNewLine(line);
		if (line.compare(0, 3, "ply") == 0) {
			bool error = false;
			bool firstElement = true;
			std::string  headerLine;
			ply::Element element;
			do {
				vcl::Tokenizer spaceTokenizer = vcl::io::internal::nextNonEmptyTokenizedLine(file);
				if (!error) {
					str::removeWindowsNewLine(line);
					vcl::Tokenizer::iterator token = spaceTokenizer.begin();
					headerLine                     = *token;
					if (headerLine == "format") {
						token++;
						if (*token == "ascii")
							frmt = ply::ASCII;
						else if (
							*token == "binary_big_endian" || *token == "binary_little_endian" ||
							*token == "binary")
							frmt = ply::BINARY;
					}
					// reading a comment, may be a texture file...
					else if (headerLine == "comment") {
						token++;
						if (token != spaceTokenizer.end()){
							if (vcl::str::containsCaseInsensitive(*token, "texture")) {
								++token;
								if (token != spaceTokenizer.end()){
									std::string textName = *token;
									auto it = vcl::str::findCaseInsensitive(textName, "<this>");
									if (it != textName.end()) {
										uint pos = it-textName.begin();
										std::string fn =
											vcl::FileInfo::filenameWithoutExtension(filename);
										textName =
											textName.substr(0, pos) +
											fn + textName.substr(pos+6, textName.size());
									}
									textureFiles.push_back(textName);
								}
							}
						}
					}
					// I am reading a new element
					else if (headerLine == "element") { // new type of element read
						// if it is not the first element to read, it means that the previous one
						// needs to be saved
						if (!firstElement) {
							// index of each element type in the elements vector
							if (element.type == ply::VERTEX)
								vertElemPos = (long int) elements.size();
							if (element.type == ply::FACE)
								faceElemPos = (long int) elements.size();
							if (element.type == ply::EDGE)
								edgeElemPos = (long int) elements.size();
							if (element.type == ply::TRISTRIP)
								trisElemPos = (long int) elements.size();
							elements.push_back(element);
							element = ply::Element();
						}
						element = readElement(spaceTokenizer);
						firstElement   = false;
					}
					else if (headerLine == "property") {
						ply::Property p = readProperty(spaceTokenizer);
						element.properties.push_back(p);
					}
					else if (headerLine == "end_header") { // save the last element
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
}

inline void PlyHeader::clear()
{
	frmt = ply::UNKNOWN;
	elements.clear();
	textureFiles.clear();
	valid = false;
	vertElemPos       = -1;
	faceElemPos       = -1;
	edgeElemPos       = -1;
	trisElemPos       = -1;
}

bool PlyHeader::isValid() const
{
	return valid;
}

inline ply::Format PlyHeader::format() const
{
	return frmt;
}

inline FileMeshInfo PlyHeader::getInfo() const
{
	FileMeshInfo mod;
	// x, y, z, nx, ny, nz, red, green, blue, alpha, vertex_indices

	if (vertElemPos >= 0) {
		mod.setVertices();
		for (const Property& p : elements[vertElemPos].properties) {
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
				if (p.type <= PropertyType::DOUBLE) {
					mod.addVertexCustomComponent(p.unknownPropertyName, (FileMeshInfo::DataType)p.type);
				}
			default: break;
			}
		}
	}
	if (faceElemPos >= 0) {
		mod.setFaces();
		for (const Property& p : elements[faceElemPos].properties) {
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
				if (p.type <= PropertyType::DOUBLE) {
					mod.addFaceCustomComponent(p.unknownPropertyName, (FileMeshInfo::DataType)p.type);
				}
			default: break;
			}
		}
	}
	if (trisElemPos >= 0) {
		mod.setFaces();
		for (const Property& p : elements[trisElemPos].properties) {
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

inline bool PlyHeader::hasVertices() const
{
	return vertElemPos >= 0;
}

inline bool PlyHeader::hasFaces() const
{
	return faceElemPos >= 0;
}

inline bool PlyHeader::hasEdges() const
{
	return edgeElemPos >= 0;
}

inline bool PlyHeader::hasTriStrips() const
{
	return trisElemPos >= 0;
}

inline bool PlyHeader::hasTextureFileNames() const
{
	return textureFiles.size() > 0;
}

inline uint PlyHeader::numberVertices() const
{
	assert(hasVertices());
	return elements[vertElemPos].numberElements;
}

inline uint PlyHeader::numberFaces() const
{
	assert(hasFaces());
	return elements[faceElemPos].numberElements;
}

inline uint PlyHeader::numberEdges() const
{
	assert(hasEdges());
	return elements[edgeElemPos].numberElements;
}

inline uint PlyHeader::numberTriStrips() const
{
	assert(hasTriStrips());
	return elements[trisElemPos].numberElements;
}

inline uint PlyHeader::numberTextureFileNames() const
{
	return textureFiles.size();
}

inline const std::list<ply::Property>& PlyHeader::vertexProperties() const
{
	assert(hasVertices());
	return elements[vertElemPos].properties;
}

inline const std::list<ply::Property>& PlyHeader::faceProperties() const
{
	assert(hasFaces());
	return elements[faceElemPos].properties;
}

inline const std::list<Property>& PlyHeader::edgeProperties() const
{
	assert(hasEdges());
	return elements[edgeElemPos].properties;
}

inline const std::list<Property>& PlyHeader::triStripsProperties() const
{
	assert(hasTriStrips());
	return elements[trisElemPos].properties;
}

inline const std::vector<std::string>& PlyHeader::textureFileNames() const
{
	return textureFiles;
}

inline bool PlyHeader::errorWhileLoading() const
{
	return !valid;
}

inline void PlyHeader::setNumberVertices(unsigned long int nV)
{
	if (vertElemPos < 0)
		vertElemPos = nextElementID++;
	elements[vertElemPos].numberElements = nV;
}

inline void PlyHeader::setNumberFaces(unsigned long int nF)
{
	if (faceElemPos < 0)
		faceElemPos = nextElementID++;
	elements[faceElemPos].numberElements = nF;
}

inline void PlyHeader::setNumberEdges(unsigned long nE)
{
	if (edgeElemPos < 0)
		edgeElemPos =nextElementID++;
	elements[edgeElemPos].numberElements = nE;
}

inline void PlyHeader::pushTextureFileName(const std::string& tn)
{
	textureFiles.push_back(tn);
}

inline void PlyHeader::setInfo(
	const FileMeshInfo& info,
	std::vector<std::string> textureFileNames,
	bool binary)
{
	clear();
	frmt             = binary ? BINARY : ASCII;
	valid             = true;
	textureFiles = textureFileNames;
	if (info.hasVertices()) {
		vertElemPos = nextElementID++;
		ply::Element vElem;
		vElem.type = ply::VERTEX;
		if (info.hasVertexCoords()) {
			ply::Property px, py, pz;
			px.name = x;
			px.type = (PropertyType)info.vertexCoordsType();
			py.name = y;
			py.type = (PropertyType)info.vertexCoordsType();
			pz.name = z;
			pz.type = (PropertyType)info.vertexCoordsType();
			vElem.properties.push_back(px);
			vElem.properties.push_back(py);
			vElem.properties.push_back(pz);
		}
		if (info.hasVertexNormals()) {
			ply::Property vnx, vny, vnz;
			vnx.name = nx;
			vnx.type = (PropertyType)info.vertexNormalsType();
			vny.name = ny;
			vny.type = (PropertyType)info.vertexNormalsType();
			vnz.name = nz;
			vnz.type = (PropertyType)info.vertexNormalsType();
			vElem.properties.push_back(vnx);
			vElem.properties.push_back(vny);
			vElem.properties.push_back(vnz);
		}
		if (info.hasVertexColors()) {
			ply::Property vcr, vcg, vcb, vca;
			vcr.name = red;
			vcr.type = (PropertyType)info.vertexColorsType();
			vcg.name = green;
			vcg.type = (PropertyType)info.vertexColorsType();
			vcb.name = blue;
			vcb.type = (PropertyType)info.vertexColorsType();
			vca.name = alpha;
			vca.type = (PropertyType)info.vertexColorsType();
			vElem.properties.push_back(vcr);
			vElem.properties.push_back(vcg);
			vElem.properties.push_back(vcb);
			vElem.properties.push_back(vca);
		}
		if (info.hasVertexQuality()) {
			ply::Property vs;
			vs.name = quality;
			vs.type = (PropertyType)info.vertexQualityType();
			vElem.properties.push_back(vs);
		}
		if (info.hasVertexTexCoords()) {
			ply::Property tcu, tcv, tcn;
			tcu.name = texture_u;
			tcu.type = (PropertyType)info.vertexTexCoordsType();
			tcv.name = texture_v;
			tcv.type = (PropertyType)info.vertexTexCoordsType();
			tcn.name = texnumber;
			tcn.type = io::internal::USHORT;
			vElem.properties.push_back(tcu);
			vElem.properties.push_back(tcv);
			vElem.properties.push_back(tcn);
		}
		if (info.hasVertexCustomComponents()) {
			for (const auto& cc : info.vertexCustomComponents()) {
				if (cc.type <= FileMeshInfo::DOUBLE) {
					ply::Property pp;
					pp.name = unknown;
					pp.unknownPropertyName = cc.name;
					pp.type = (PropertyType)cc.type;
					vElem.properties.push_back(pp);
				}
			}
		}
		elements.push_back(vElem);
	}
	if (info.hasFaces()) {
		faceElemPos = nextElementID++;
		ply::Element fElem;
		fElem.type = ply::FACE;
		if (info.hasFaceVRefs()) {
			ply::Property vids;
			vids.list         = true;
			vids.name         = vertex_indices;
			vids.type         = io::internal::UINT;
			vids.listSizeType = io::internal::UCHAR;
			fElem.properties.push_back(vids);
		}
		if (info.hasFaceNormals()) {
			ply::Property fnx, fny, fnz;
			fnx.name = nx;
			fnx.type = (PropertyType)info.faceNormalsType();
			fny.name = ny;
			fny.type = (PropertyType)info.faceNormalsType();
			fnz.name = nz;
			fnz.type = (PropertyType)info.faceNormalsType();
			fElem.properties.push_back(fnx);
			fElem.properties.push_back(fny);
			fElem.properties.push_back(fnz);
		}
		if (info.hasFaceColors()) {
			ply::Property fcr, fcg, fcb, fca;
			fcr.name = red;
			fcr.type = (PropertyType)info.faceColorsType();
			fcg.name = green;
			fcg.type = (PropertyType)info.faceColorsType();
			fcb.name = blue;
			fcb.type = (PropertyType)info.faceColorsType();
			fca.name = alpha;
			fca.type = (PropertyType)info.faceColorsType();
			fElem.properties.push_back(fcr);
			fElem.properties.push_back(fcg);
			fElem.properties.push_back(fcb);
			fElem.properties.push_back(fca);
		}
		if (info.hasFaceQuality()) {
			ply::Property fs;
			fs.name = quality;
			fs.type = (PropertyType)info.faceQualityType();
			fElem.properties.push_back(fs);
		}
		if (info.hasFaceWedgeTexCoords()) {
			ply::Property tc, tn;
			tc.list         = true;
			tc.listSizeType = io::internal::UCHAR;
			tc.name = texcoord;
			tc.type = (PropertyType)info.faceWedgeTexCoordsType();
			tn.name = texnumber;
			tn.type = io::internal::USHORT;
			fElem.properties.push_back(tc);
			fElem.properties.push_back(tn);
		}
		if (info.hasFaceCustomComponents()) {
			for (const auto& cc : info.faceCustomComponents()) {
				if (cc.type <= FileMeshInfo::DOUBLE) {
					ply::Property pp;
					pp.name = unknown;
					pp.unknownPropertyName = cc.name;
					pp.type = (PropertyType)cc.type;
					fElem.properties.push_back(pp);
				}
			}
		}
		elements.push_back(fElem);
	}
	if (info.hasEdges()) {
		edgeElemPos = nextElementID++;
		ply::Element eElem;
		eElem.type = ply::EDGE;
		if (info.hasEdgeVRefs()) {
			ply::Property v1;
			v1.name = vertex1;
			v1.type = io::internal::UINT;
			eElem.properties.push_back(v1);
			ply::Property v2;
			v2.name = vertex2;
			v2.type = io::internal::UINT;
			eElem.properties.push_back(v2);
		}
		elements.push_back(eElem);
	}
}

inline std::string PlyHeader::toString() const
{
	std::string s;

	s += "ply\nformat ";
	s += (frmt == ASCII ? "ascii 1.0\n" : "binary_little_endian 1.0\n");
	s += "comment Generated by vclib\n";
	for (const std::string& str : textureFiles) {
		s += "comment TextureFile " + str + "\n";
	}
	for (const Element& e : elements) {
		s += "element ";
		switch (e.type) {
		case VERTEX: s += "vertex " + std::to_string(e.numberElements) + "\n"; break;
		case FACE: s += "face " + std::to_string(e.numberElements) + "\n"; break;
		case EDGE: s += "edge " + std::to_string(e.numberElements) + "\n"; break;
		case TRISTRIP: s += "tristrips " + std::to_string(e.numberElements) + "\n"; break;
		case MATERIAL: s += "material " + std::to_string(e.numberElements) + "\n"; break;
		case OTHER: s += e.unknownElementType + " " + std::to_string(e.numberElements) + "\n"; break;
		}
		for (const Property& p : e.properties) {
			s += "property ";
			if (p.list) {
				s += "list ";
				s += typeToString(p.listSizeType) + " ";
			}
			s += typeToString(p.type) + " ";
			if (p.name == unknown)
				s += p.unknownPropertyName + "\n";
			else
				s += nameToString(p.name) + "\n";
		}
	}
	s += "end_header\n";

	return s;
}

inline void PlyHeader::setFormat(ply::Format f)
{
	frmt = f;
}

inline PlyHeader::iterator PlyHeader::begin() const
{
	return elements.begin();
}

inline PlyHeader::iterator PlyHeader::end() const
{
	return elements.end();
}

inline ply::Element PlyHeader::readElement(const Tokenizer& lineTokenizer) const
{
	ply::Element             e;
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
		e.type =ply::TRISTRIP;
		e.numberElements = std::stoi(*(++token));
	}
	else {
		e.type = ply::OTHER;
		e.numberElements = std::stoi(*(++token));
		e.unknownElementType = s;
	}
	return e;
}

inline ply::Property PlyHeader::readProperty(const vcl::Tokenizer& lineTokenizer) const
{
	ply::Property            p;
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
		if (p.name == unknown)
			p.unknownPropertyName = name;
	}
	else {
		p.list           = false;
		std::string name = *(++token);
		p.type           = stringToType(type);
		p.name           = stringToName(name);
		if (p.name == unknown)
			p.unknownPropertyName = name;
	}

	return p;
}

inline ply::PropertyName PlyHeader::stringToName(const std::string& name) const
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

inline ply::PropertyType PlyHeader::stringToType(const std::string& type) const
{
	ply::PropertyType pt = io::internal::UCHAR;
	if (type == "char")
		pt = io::internal::CHAR;
	if (type == "uchar")
		pt = io::internal::UCHAR;
	if (type == "short")
		pt = io::internal::SHORT;
	if (type == "ushort")
		pt = io::internal::USHORT;
	if (type == "int")
		pt = io::internal::INT;
	if (type == "uint")
		pt = io::internal::UINT;
	if (type == "float")
		pt = io::internal::FLOAT;
	if (type == "double")
		pt = io::internal::DOUBLE;
	return pt;
}

inline std::string PlyHeader::nameToString(PropertyName n) const
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

inline std::string PlyHeader::typeToString(PropertyType t) const
{
	switch (t) {
	case io::internal::CHAR: return "char";
	case io::internal::UCHAR: return "uchar";
	case io::internal::SHORT: return "short";
	case io::internal::USHORT: return "ushort";
	case io::internal::INT: return "int";
	case io::internal::UINT: return "uint";
	case io::internal::FLOAT: return "float";
	case io::internal::DOUBLE: return "double";
	case io::internal::NONE: return "";
	}
	return "";
}

} // namespace vcl::io::ply
