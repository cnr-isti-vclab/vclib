/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "ply_header.h"
#include <clocale>
#include <mgp/misc/tokenizer.h>

namespace mgp {
namespace ply {

inline PlyHeader::PlyHeader() : _format(ply::UNKNOWN), valid(false), v(-1), f(-1), e(-1)
{
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
}

inline PlyHeader::PlyHeader(
	ply::Format         f,
	const ply::Element& vElement,
	const ply::Element  fElement) :
		_format(f), valid(true), v(0), f(1), e(-1)
{
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	elements.push_back(vElement);
	elements.push_back(fElement);
}

inline PlyHeader::PlyHeader(
	ply::Format         f,
	const ply::Element& vElement,
	const ply::Element  fElement,
	const ply::Element  eElement) :
		_format(f), valid(true), v(0), f(1), e(2)
{
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	elements.push_back(vElement);
	elements.push_back(fElement);
	elements.push_back(eElement);
}

PlyHeader::PlyHeader(Format format, const io::FileMeshInfo& info) :
		_format(format), valid(true), v(-1), f(-1), e(-1)
{
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	setInfo(info, format == BINARY);
}

inline PlyHeader::PlyHeader(std::ifstream& file) : _format(ply::UNKNOWN), valid(false)
{
	clear();
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	if (file.is_open()) {
		std::string line;
		std::getline(file, line);
		if (line.compare(0, 3, "ply") == 0) {
			bool         error, first = true;
			std::string  headerLine;
			ply::Element element;
			do {
				error = !(std::getline(file, line));
				if (!error) {
					mgp::Tokenizer spaceTokenizer(line, ' ');
					if (spaceTokenizer.begin() == spaceTokenizer.end())
						continue;
					mgp::Tokenizer::iterator token = spaceTokenizer.begin();
					headerLine                     = *token;
					if (headerLine == "format") {
						token++;
						if (*token == "ascii")
							_format = ply::ASCII;
						else if (
							*token == "binary_big_endian" || *token == "binary_little_endian" ||
							*token == "binary")
							_format = ply::BINARY;
					}
					else if (headerLine == "element") { // new type of element read
						if (!first) {                   // last element finished, save it
							if (element.type == ply::VERTEX)
								v = (long int) elements.size();
							if (element.type == ply::FACE)
								f = (long int) elements.size();
							if (element.type == ply::EDGE)
								e = (long int) elements.size();
							elements.push_back(element);
							element = ply::Element();
						}
						element = readElement(spaceTokenizer);
						first   = false;
					}
					else if (headerLine == "property") {
						ply::Property p = readProperty(spaceTokenizer);
						element.properties.push_back(p);
					}
					else if (headerLine == "end_header") { // save the last element
						if (element.type == ply::VERTEX)
							v = (long int) elements.size();
						if (element.type == ply::FACE)
							f = (long int) elements.size();
						if (element.type == ply::EDGE)
							e = (long int) elements.size();
						elements.push_back(element);
					}
				}
			} while (!error && headerLine != "end_header");
			valid = !error && hasVertices() && hasFaces();
		}
	}
}

inline void PlyHeader::clear()
{
	_format = ply::UNKNOWN;
	elements.clear();
	valid = false;
	v       = -1;
	f       = -1;
	e       = -1;
}

bool PlyHeader::isValid() const
{
	return valid;
}

inline ply::Format PlyHeader::format() const
{
	return _format;
}

inline io::FileMeshInfo PlyHeader::getInfo() const
{
	io::FileMeshInfo mod;
	// x, y, z, nx, ny, nz, red, green, blue, alpha, vertex_indices

	if (v >= 0) {
		mod.setVertices();
		for (Property p : elements[v].properties) {
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
			case ply::scalar: mod.setVertexScalars(); break;
			default: break;
			}
		}
	}
	if (f >= 0) {
		mod.setFaces();
		for (Property p : elements[f].properties) {
			switch (p.name) {
			case ply::vertex_indices: mod.setFaceVRefs(); break;
			case ply::nx:
			case ply::ny:
			case ply::nz: mod.setFaceNormals(); break;
			case ply::red:
			case ply::green:
			case ply::blue:
			case ply::alpha: mod.setFaceColors(); break;
			case ply::scalar: mod.setFaceScalars(); break;
			default: break;
			}
		}
	}
	return mod;
}

inline bool PlyHeader::hasVertices() const
{
	return v >= 0;
}

inline bool PlyHeader::hasFaces() const
{
	return f >= 0;
}

inline bool PlyHeader::hasEdges() const
{
	return e >= 0;
}

inline uint PlyHeader::numberVertices() const
{
	assert(hasVertices());
	return elements[v].numberElements;
}

inline uint PlyHeader::numberFaces() const
{
	assert(hasFaces());
	return elements[f].numberElements;
}

inline uint PlyHeader::numberEdges() const
{
	assert(hasEdges());
	return elements[e].numberElements;
}

inline const std::list<ply::Property>& PlyHeader::vertexProperties() const
{
	assert(hasVertices());
	return elements[v].properties;
}

inline const std::list<ply::Property>& PlyHeader::faceProperties() const
{
	assert(hasFaces());
	return elements[f].properties;
}

inline const std::list<Property>& PlyHeader::edgeProperties() const
{
	assert(hasEdges());
	return elements[e].properties;
}

inline bool PlyHeader::errorWhileLoading() const
{
	return !valid;
}

inline void PlyHeader::setNumberVertices(unsigned long int nV)
{
	if (v < 0)
		v = nextElementID++;
	elements[v].numberElements = nV;
}

inline void PlyHeader::setNumberFaces(unsigned long int nF)
{
	if (f < 0)
		f = nextElementID++;
	elements[f].numberElements = nF;
}

inline void PlyHeader::setNumberEdges(unsigned long nE)
{
	if (e < 0)
		e =nextElementID++;
	elements[e].numberElements = nE;
}

inline void PlyHeader::setInfo(const io::FileMeshInfo& info, bool binary)
{
	clear();
	_format             = binary ? BINARY : ASCII;
	valid             = true;
	if (info.hasVertices()) {
		v = nextElementID++;
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
		if (info.hasVertexScalars()) {
			ply::Property vs;
			vs.name = scalar;
			vs.type = (PropertyType)info.vertexScalarsType();
			vElem.properties.push_back(vs);
		}
		elements.push_back(vElem);
	}
	if (info.hasFaces()) {
		f = nextElementID++;
		ply::Element fElem;
		fElem.type = ply::FACE;
		if (info.hasFaceVRefs()) {
			ply::Property vids;
			vids.list         = true;
			vids.name         = vertex_indices;
			vids.type         = UINT;
			vids.listSizeType = UCHAR;
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
		if (info.hasFaceScalars()) {
			ply::Property fs;
			fs.name = scalar;
			fs.type = (PropertyType)info.faceScalarsType();
			fElem.properties.push_back(fs);
		}
		elements.push_back(fElem);
	}
	if (info.hasEdges()) {
		e = nextElementID++;
		ply::Element eElem;
		eElem.type = ply::EDGE;
		// ToDo: populate eElem
		elements.push_back(eElem);
	}
}

inline std::string PlyHeader::toString() const
{
	std::string s;

	s += "ply\nformat ";
	s += (_format == ASCII ? "ascii 1.0\n" : "binary_little_endian 1.0\n");
	s += "comment Generated by mgplib (https://github.com/alemuntoni/mgplib)\n";
	for (Element e : elements) {
		s += "element ";
		switch (e.type) {
		case VERTEX: s += "vertex " + std::to_string(e.numberElements) + "\n"; break;
		case FACE: s += "face " + std::to_string(e.numberElements) + "\n"; break;
		case EDGE: s += "edge " + std::to_string(e.numberElements) + "\n"; break;
		case MATERIAL: s += "material " + std::to_string(e.numberElements) + "\n"; break;
		case OTHER: s += e.unknownElementType + " " + std::to_string(e.numberElements) + "\n"; break;
		}
		for (Property p : e.properties) {
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

inline void PlyHeader::addElement(const ply::Element& elem)
{
	if (elem.type == ply::VERTEX) {
		if (v >= 0) {
			elements[v] = elem;
		}
		else {
			elements.push_back(elem);
			v = nextElementID++;
		}
	}
	else if (elem.type == ply::FACE) {
		if (f >= 0) {
			elements[f] = elem;
		}
		else {
			elements.push_back(elem);
			f = nextElementID++;
		}
	}
	else if (elem.type == ply::EDGE) {
		if (e >= 0) {
			elements[e] = elem;
		}
		else {
			elements.push_back(elem);
			e = nextElementID++;
		}
	}
	else {
		elements.push_back(elem);
	}
}

inline void PlyHeader::setFormat(ply::Format f)
{
	_format = f;
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
	mgp::Tokenizer::iterator token = lineTokenizer.begin();
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
	else {
		e.type = ply::OTHER;
		e.unknownElementType = s;
	}
	return e;
}

inline ply::Property PlyHeader::readProperty(const mgp::Tokenizer& lineTokenizer) const
{
	ply::Property            p;
	mgp::Tokenizer::iterator token = lineTokenizer.begin();
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
		pn = ply::scalar;
	if (name == "vertex1")
		pn = ply::vertex1;
	if (name == "vertex2")
		pn = ply::vertex2;
	if (name == "vertex_indices")
		pn = ply::vertex_indices;
	return pn;
}

inline ply::PropertyType PlyHeader::stringToType(const std::string& type) const
{
	ply::PropertyType pt = ply::UCHAR;
	if (type == "char")
		pt = ply::CHAR;
	if (type == "uchar")
		pt = ply::UCHAR;
	if (type == "short")
		pt = ply::SHORT;
	if (type == "ushort")
		pt = ply::USHORT;
	if (type == "int")
		pt = ply::INT;
	if (type == "uint")
		pt = ply::UINT;
	if (type == "float")
		pt = ply::FLOAT;
	if (type == "double")
		pt = ply::DOUBLE;
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
	case ply::scalar: return "scalar";
	case ply::vertex_indices: return "vertex_indices";
	case ply::vertex1: return "vertex1";
	case ply::vertex2: return "vertex2";
	default: return "unknown";
	}
}

inline std::string PlyHeader::typeToString(PropertyType t) const
{
	switch (t) {
	case ply::CHAR: return "char";
	case ply::UCHAR: return "uchar";
	case ply::SHORT: return "short";
	case ply::USHORT: return "ushort";
	case ply::INT: return "int";
	case ply::UINT: return "uint";
	case ply::FLOAT: return "float";
	case ply::DOUBLE: return "double";
	case ply::NONE: return "";
	}
}

} // namespace ply
} // namespace mgp
