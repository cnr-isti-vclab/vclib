/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "ply_header.h"
#include <mgp/misc/tokenizer.h>
#include <clocale>

namespace mgp {
namespace ply {

inline PlyHeader::PlyHeader() :
	_format(ply::UNKNOWN),
	isValid(false),
	v(-1),
	f(-1),
	e(-1)
{
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
}

inline PlyHeader::PlyHeader(ply::Format f, const ply::Element &vElement, const ply::Element fElement) :
	_format(f),
	isValid(true),
	v(0),
	f(1),
	e(-1)
{
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	elements.push_back(vElement);
	elements.push_back(fElement);
}

inline PlyHeader::PlyHeader(ply::Format f, const ply::Element &vElement, const ply::Element fElement, const ply::Element eElement) :
	_format(f),
	isValid(true),
	v(0),
	f(1),
	e(2)
{
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	elements.push_back(vElement);
	elements.push_back(fElement);
	elements.push_back(eElement);
}

inline PlyHeader::PlyHeader(std::ifstream &file) :
	_format(ply::UNKNOWN),
	isValid(false)
{
	clear();
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	if (file.is_open()){
		std::string line;
		std::getline(file,line);
		if (line.compare(0, 3, "ply") == 0){
			bool error, first = true;
			std::string headerLine;
			ply::Element element;
			do {
				error = !(std::getline(file,line));
				if (!error){
					mgp::Tokenizer spaceTokenizer(line, ' ');
					if (spaceTokenizer.begin() == spaceTokenizer.end()) continue;
					mgp::Tokenizer::iterator token = spaceTokenizer.begin();
					headerLine = *token;
					if (headerLine == "format") {
						token++;
						if (*token == "ascii")
							_format = ply::ASCII;
						else if (*token == "binary_big_endian" || *token == "binary_little_endian" || *token == "binary")
							_format = ply::BINARY;
					}
					else if (headerLine == "element") { //new type of element read
						if (!first){ //last element finished, save it
							if (element.type == ply::VERTEX) v = (long int)elements.size();
							if (element.type == ply::FACE) f = (long int)elements.size();
							if (element.type == ply::EDGE) e = (long int)elements.size();
							elements.push_back(element);
							element = ply::Element();
						}
						element = readElement(spaceTokenizer);
						first = false;
					}
					else if (headerLine == "property"){
						ply::Property p = readProperty(spaceTokenizer);
						element.properties.push_back(p);
					}
					else if (headerLine == "end_header") { //save the last element
						if (element.type == ply::VERTEX) v = (long int)elements.size();
						if (element.type == ply::FACE) f = (long int)elements.size();
						if (element.type == ply::EDGE) e = (long int)elements.size();
						elements.push_back(element);
					}
				}
			} while (!error && headerLine != "end_header");
			isValid = !error && hasVertexAndFaceElements();
		}
	}
}

inline void PlyHeader::clear()
{
	_format = ply::UNKNOWN;
	elements.clear();
	isValid = false;
	v = -1;
	f = -1;
	e = -1;
}

inline bool PlyHeader::errorWhileLoading() const
{
	return !isValid;
}

inline bool PlyHeader::hasVertexAndFaceElements() const
{
	if (v < 0 || f < 0) return false;
	bool vertex = false, face = false;

	bool xb = false, yb = false, zb = false;
	for (const ply::Property& p : elements[v].properties) {
		if (p.name == ply::x)
			xb = true;
		if (p.name == ply::y)
			yb = true;
		if (p.name == ply::z)
			zb = true;
	}
	if (xb && yb && zb)
		vertex = true;
	for (const ply::Property& p : elements[f].properties) {
		if (p.name == ply::vertex_indices)
			face = true;
	}
	return vertex && face;
}

inline bool PlyHeader::hasEdges() const
{
	return e >= 0;
}

inline ply::Format PlyHeader::format() const
{
	return _format;
}

inline const std::list<ply::Property> &PlyHeader::vertexProperties() const
{
	return elements[v].properties;
}

inline const std::list<ply::Property> &PlyHeader::faceProperties() const
{
	return elements[f].properties;
}

inline const std::list<Property>& PlyHeader::edgeProperties() const
{
	return elements[e].properties;
}

inline uint PlyHeader::numberVertices() const
{
	return elements[v].numberElements;
}

inline uint PlyHeader::numberFaces() const
{
	return elements[f].numberElements;
}

inline uint PlyHeader::numberEdges() const
{
	return elements[e].numberElements;
}

inline void PlyHeader::setNumberVertices(unsigned long int nV)
{
	if (v >= 0)
		elements[v].numberElements = nV;
}

inline void PlyHeader::setNumberFaces(unsigned long int nF)
{
	if (f >= 0)
		elements[f].numberElements = nF;
}

inline void PlyHeader::setNumberEdges(unsigned long nE)
{
	if (e >= 0)
		elements[e].numberElements = nE;
}

inline io::FileMeshInfo PlyHeader::getInfo() const
{
	io::FileMeshInfo mod;
	//x, y, z, nx, ny, nz, red, green, blue, alpha, vertex_indices

	for (Property p : elements[v].properties){
		switch (p.name) {
			case ply::nx :
			case ply::ny :
			case ply::nz :
				mod.setVertexNormals(); break;
			case ply::red :
			case ply::green :
			case ply::blue :
			case ply::alpha :
				mod.setVertexColors(); break;
			default : break;
		}
	}
	for (Property p : elements[f].properties){
		switch (p.name) {
			case ply::nx :
			case ply::ny :
			case ply::nz :
				mod.setFaceNormals(); break;
			case ply::red :
			case ply::green :
			case ply::blue :
			case ply::alpha :
				mod.setFaceColors(); break;
			default : break;
		}
	}
	return mod;
}

inline void PlyHeader::setInfo(const io::FileMeshInfo &mode, bool binary)
{
	clear();
	this->v = 0; this->f = 1; isValid = true;
	_format = binary ? BINARY : ASCII;
	ply::Element v;
	v.type = ply::VERTEX;
	ply::Property px, py, pz;
	px.list = false; px.name = x; px.type = DOUBLE;
	py.list = false; py.name = y; py.type = DOUBLE;
	pz.list = false; pz.name = z; pz.type = DOUBLE;
	v.properties.push_back(px);
	v.properties.push_back(py);
	v.properties.push_back(pz);
	if(mode.hasVertexNormals()){
		ply::Property vnx, vny, vnz;
		vnx.list = false; vnx.name = nx; vnx.type = FLOAT;
		vny.list = false; vny.name = ny; vny.type = FLOAT;
		vnz.list = false; vnz.name = nz; vnz.type = FLOAT;
		v.properties.push_back(vnx);
		v.properties.push_back(vny);
		v.properties.push_back(vnz);
	}
	if(mode.hasVertexColors()){
		ply::Property vcr, vcg, vcb, vca;
		vcr.list = false; vcr.name = red; vcr.type = UCHAR;
		vcg.list = false; vcg.name = green; vcg.type = UCHAR;
		vcb.list = false; vcb.name = blue; vcb.type = UCHAR;
		vca.list = false; vca.name = alpha; vca.type = UCHAR;
		v.properties.push_back(vcr);
		v.properties.push_back(vcg);
		v.properties.push_back(vcb);
		v.properties.push_back(vca);
	}
	ply::Element f;
	f.type = ply::FACE;
	ply::Property vids;
	vids.list = true; vids.name = vertex_indices; vids.type = UINT; vids.listSizeType = UCHAR;
	f.properties.push_back(vids);
	if(mode.hasFaceNormals()){
		ply::Property fnx, fny, fnz;
		fnx.list = false; fnx.name = nx; fnx.type = FLOAT;
		fny.list = false; fny.name = ny; fny.type = FLOAT;
		fnz.list = false; fnz.name = nz; fnz.type = FLOAT;
		f.properties.push_back(fnx);
		f.properties.push_back(fny);
		f.properties.push_back(fnz);
	}
	if(mode.hasFaceColors()){
		ply::Property fcr, fcg, fcb, fca;
		fcr.list = false; fcr.name = red; fcr.type = UCHAR;
		fcg.list = false; fcg.name = green; fcg.type = UCHAR;
		fcb.list = false; fcb.name = blue; fcb.type = UCHAR;
		fca.list = false; fca.name = alpha; fca.type = UCHAR;
		f.properties.push_back(fcr);
		f.properties.push_back(fcg);
		f.properties.push_back(fcb);
		f.properties.push_back(fca);
	}


	elements.push_back(v);
	elements.push_back(f);
}

inline std::string PlyHeader::toString() const
{
	std::string s;

	s += "ply\nformat ";
	s+= (_format == ASCII ? "ascii 1.0\n" : "binary_little_endian 1.0\n");
	s+= "comment Generated by mgplib (https://github.com/mgphci/mgplib)\n";
	for (Element e : elements) {
		s += "element ";
		switch (e.type) {
			case VERTEX:
				s+= "vertex " + std::to_string(e.numberElements) +"\n";
				break;
			case FACE:
				s+= "face " + std::to_string(e.numberElements) +"\n";
				break;
			case EDGE:
				s+= "edge " + std::to_string(e.numberElements) +"\n";
				break;
			case MATERIAL:
				s+= "material " + std::to_string(e.numberElements) +"\n";
				break;
			case OTHER:
				break;
		}
		for (Property p : e.properties){
			s+= "property ";
			if (p.list){
				s+= "list ";
				s+= typeToString(p.listSizeType) + " ";
			}
			s+= typeToString(p.type) + " ";
			if (p.name == unknown)
				s+= p.unknownPropwertyName + "\n";
			else
				s+= nameToString(p.name) + "\n";
		}
	}
	s += "end_header\n";

	return s;
}

inline void PlyHeader::addElement(const ply::Element &e)
{
        if (e.type == ply::VERTEX) v = (long int)elements.size();
        if (e.type == ply::FACE) f = (long int)elements.size();
	elements.push_back(e);
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

inline ply::Element PlyHeader::readElement(const Tokenizer &lineTokenizer) const
{
	ply::Element e;
	mgp::Tokenizer::iterator token = lineTokenizer.begin();
	std::string s = *(++token);
	if (s == "vertex"){
		e.type = ply::VERTEX;
		e.numberElements = std::stoi(*(++token));
	}
	else if (s == "face"){
		e.type = ply::FACE;
		e.numberElements = std::stoi(*(++token));
	}
	else if (s == "edge") {
		e.type = ply::EDGE;
		e.numberElements = std::stoi(*(++token));
	}
	else
		e.type = ply::OTHER;
	return e;
}

inline ply::Property PlyHeader::readProperty(const mgp::Tokenizer& lineTokenizer) const
{
	ply::Property p;
	mgp::Tokenizer::iterator token = lineTokenizer.begin();
	std::string type = *(++token);
	if (type == "list"){
		p.list = true;
		std::string typeSize = *(++token);
		std::string typeData = *(++token);
		std::string name = *(++token);
		p.listSizeType = stringToType(typeSize);
		p.type = stringToType(typeData);
		p.name = stringToListName(name);
	}
	else {
		p.list = false;
		std::string name =*(++token);
		p.type = stringToType(type);
		p.name = stringToName(name);
		if (p.name == unknown)
			p.unknownPropwertyName = name;
	}

	return p;
}

inline ply::PropertyName PlyHeader::stringToName(const std::string& name) const
{
	ply::PropertyName pn = ply::unknown;
	if (name == "x") pn = ply::x;
	if (name == "y") pn = ply::y;
	if (name == "z") pn = ply::z;
	if (name == "nx") pn = ply::nx;
	if (name == "ny") pn = ply::ny;
	if (name == "nz") pn = ply::nz;
	if (name == "red") pn = ply::red;
	if (name == "green") pn = ply::green;
	if (name == "blue") pn = ply::blue;
	if (name == "alpha") pn = ply::alpha;
	if (name == "vertex1") pn = ply::vertex1;
	if (name == "vertex2") pn = ply::vertex2;
	return pn;
}

inline ply::PropertyName PlyHeader::stringToListName(const std::string &name) const
{
	ply::PropertyName pn = ply::unknown;
	if (name == "vertex_indices") pn = ply::vertex_indices;
	return pn;
}

inline ply::PropertyType PlyHeader::stringToType(const std::string &type) const
{
	ply::PropertyType pt = ply::UCHAR;
	if (type == "char") pt = ply::CHAR;
	if (type == "uchar") pt = ply::UCHAR;
	if (type == "short") pt = ply::SHORT;
	if (type == "ushort") pt = ply::USHORT;
	if (type == "int") pt = ply::INT;
	if (type == "uint") pt = ply::UINT;
	if (type == "float") pt = ply::FLOAT;
	if (type == "double") pt = ply::DOUBLE;
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
		case ply::vertex_indices: return  "vertex_indices";
		case ply::vertex1 : return "vertex1";
		case ply::vertex2 : return "vertex2";
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
	}
	return "";
}

} //namespace mgp::ply
} //namespace mgp
