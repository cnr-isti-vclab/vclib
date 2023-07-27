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

#include <vector>
#include <string>
#include <fstream>

#include <vclib/misc/mesh_info.h>

#include "ply.h"

namespace vcl {

class Tokenizer;

namespace io::ply {

/**
 * @brief The PlyHeader class allows to manage the header of a ply file.
 *
 * Following the content of an header, a ply file can be loaded or saved.
 */
class PlyHeader
{
public:
	PlyHeader();
	PlyHeader(
		ply::Format              f,
		const MeshInfo&      info,
		std::vector<std::string> textureFiles = std::vector<std::string>());
	PlyHeader(const std::string& filename, std::ifstream& file);

	void clear();
	bool isValid() const;

	ply::Format format() const;

	MeshInfo getInfo() const;

	bool hasVertices() const;
	bool hasFaces() const;
	bool hasEdges() const;
	bool hasTriStrips() const;
	bool hasTextureFileNames() const;

	uint numberVertices() const;
	uint numberFaces() const;
	uint numberEdges() const;
	uint numberTriStrips() const;
	uint numberTextureFileNames() const;

	const std::list<ply::Property>& vertexProperties() const;
	const std::list<ply::Property>& faceProperties() const;
	const std::list<ply::Property>& edgeProperties() const;
	const std::list<ply::Property>& triStripsProperties() const;
	const std::vector<std::string>& textureFileNames() const;

	bool errorWhileLoading() const;

	void setNumberVertices(unsigned long int nV);
	void setNumberFaces(unsigned long int nF);
	void setNumberEdges(unsigned long int nE);
	void pushTextureFileName(const std::string& tn);

	void setInfo(
		const MeshInfo&  info,
		std::vector<std::string> textureFiles = std::vector<std::string>(),
		bool                     binary       = true);

	std::string toString() const;

	void setFormat(ply::Format f);

	typedef std::vector<ply::Element>::const_iterator iterator;

	iterator begin() const;
	iterator end() const;


private:
	ply::Element readElement(const vcl::Tokenizer& lineTokenizer) const;
	ply::Property readProperty(const vcl::Tokenizer& lineTokenizer) const;
	ply::PropertyName stringToName(const std::string &name) const;
	ply::PropertyType stringToType(const std::string &type) const;
	std::string nameToString(ply::PropertyName n) const;
	std::string typeToString(ply::PropertyType t) const;

	ply::Format frmt;
	std::vector<ply::Element> elements;
	std::vector<std::string> textureFiles;

	bool valid;

	// say which is the id in the elements vector for vertices, faces, edges and triStrips
	int vertElemPos, faceElemPos, edgeElemPos, trisElemPos;

	int nextElementID = 0;
};

} //namespace vcl::ply
} //namespace vcl

#include "ply_header.cpp"

#endif // VCL_PLY_HEADER_H
