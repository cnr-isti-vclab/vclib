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

#include "io_utils.h"

#include <algorithm>

#include <vclib/exceptions/io_exceptions.h>
#include <vclib/misc/file_info.h>
#include <vclib/misc/string.h>

namespace vcl::io::internal {

inline std::ofstream saveFileStream(const std::string& filename, const std::string& ext)
{
	setlocale(LC_ALL, "C");
	std::string actualfilename = FileInfo::addExtensionToFileName(filename, ext);

	std::ofstream fp;
	fp.imbue(std::locale().classic());

	fp.open(actualfilename, std::ofstream::binary); // need to set binary or windows will fail
	if (!fp) {
		throw vcl::CannotOpenFileException(actualfilename);
	}

	return fp;
}

inline std::ifstream loadFileStream(const std::string& filename)
{
	setlocale(LC_ALL, "C");
	std::ifstream fp(filename, std::ifstream::binary); // need to set binary or windows will fail
	fp.imbue(std::locale().classic());
	if (!fp.is_open()) {
		throw vcl::CannotOpenFileException(filename);
	}
	return fp;
}

template<MeshConcept MeshType>
void addPerVertexCustomComponent(MeshType& m, const FileMeshInfo::CustomComponent& cc)
{
	switch (cc.type) {
	case FileMeshInfo::CHAR: m.template addPerVertexCustomComponent<char>(cc.name); break;
	case FileMeshInfo::UCHAR: m.template addPerVertexCustomComponent<unsigned char>(cc.name); break;
	case FileMeshInfo::SHORT: m.template addPerVertexCustomComponent<short>(cc.name); break;
	case FileMeshInfo::USHORT: m.template addPerVertexCustomComponent<unsigned short>(cc.name); break;
	case FileMeshInfo::INT: m.template addPerVertexCustomComponent<int>(cc.name); break;
	case FileMeshInfo::UINT: m.template addPerVertexCustomComponent<uint>(cc.name); break;
	case FileMeshInfo::FLOAT: m.template addPerVertexCustomComponent<float>(cc.name); break;
	case FileMeshInfo::DOUBLE: m.template addPerVertexCustomComponent<double>(cc.name); break;
	default: assert(0);
	}
}

template<FaceMeshConcept MeshType>
void addPerFaceCustomComponent(MeshType& m, const FileMeshInfo::CustomComponent& cc)
{
	switch (cc.type) {
	case FileMeshInfo::CHAR: m.template addPerFaceCustomComponent<char>(cc.name); break;
	case FileMeshInfo::UCHAR: m.template addPerFaceCustomComponent<unsigned char>(cc.name); break;
	case FileMeshInfo::SHORT: m.template addPerFaceCustomComponent<short>(cc.name); break;
	case FileMeshInfo::USHORT: m.template addPerFaceCustomComponent<unsigned short>(cc.name); break;
	case FileMeshInfo::INT: m.template addPerFaceCustomComponent<int>(cc.name); break;
	case FileMeshInfo::UINT: m.template addPerFaceCustomComponent<uint>(cc.name); break;
	case FileMeshInfo::FLOAT: m.template addPerFaceCustomComponent<float>(cc.name); break;
	case FileMeshInfo::DOUBLE: m.template addPerFaceCustomComponent<double>(cc.name); break;
	default: assert(0);
	}
}

/**
 * @brief enableOptionalComponents enables all the components that are in the file mesh info and
 * that may be enabled in the mesh. If these components are not present in the mesh, the info file
 * will be modified in order to tell that a particular component cannot be saved into the mesh.
 *
 * This function is useful when you know what components are going to load from a file, and you want
 * that all the components that you are going to read in the file will be saved in the mesh, if
 * possible.
 *
 * @param info
 * @param m
 */
template<MeshConcept MeshType>
void enableOptionalComponents(FileMeshInfo& info, MeshType& m)
{
	if (info.hasVertices()) {
		if (info.hasVertexColors()) {
			if (!vcl::enableIfPerVertexColorOptional(m)) {
				info.setVertexColors(false);
			}
		}
		if (info.hasVertexNormals()) {
			if (!vcl::enableIfPerVertexNormalOptional(m)) {
				info.setVertexNormals(false);
			}
		}
		if (info.hasVertexScalars()) {
			if (!vcl::enableIfPerVertexScalarOptional(m)) {
				info.setVertexScalars(false);
			}
		}
		if (info.hasVertexTexCoords()) {
			if (!vcl::enableIfPerVertexTexCoordOptional(m)) {
				info.setVertexTexCoords(false);
			}
		}
		if (info.hasVertexCustomComponents()) {
			if constexpr (vcl::HasPerVertexCustomComponents<MeshType>) {
				for (const auto& cc : info.vertexCustomComponents()) {
					addPerVertexCustomComponent(m, cc);
				}
			}
			else {
				info.clearVertexCustomComponents();
			}
		}
	}
	else {
		info.setVertices(false);
	}

	if (info.hasFaces()) {
		if (info.hasFaceColors()) {
			if (!vcl::enableIfPerFaceColorOptional(m)) {
				info.setFaceColors(false);
			}
		}
		if (info.hasFaceNormals()) {
			if (!vcl::enableIfPerFaceNormalOptional(m)) {
				info.setFaceNormals(false);
			}
		}
		if (info.hasFaceScalars()) {
			if (!vcl::enableIfPerFaceScalarOptional(m)) {
				info.setFaceScalars(false);
			}
		}
		if (info.hasFaceWedgeTexCoords()) {
			if (!vcl::enableIfPerFaceWedgeTexCoordsOptional(m)) {
				info.setFaceWedgeTexCoords(false);
			}
		}
		if (info.hasFaceCustomComponents()) {
			if constexpr (vcl::HasPerFaceCustomComponents<MeshType>) {
				for (const auto& cc : info.faceCustomComponents()) {
					addPerFaceCustomComponent(m, cc);
				}
			}
			else {
				info.clearFaceCustomComponents();
			}
		}
	}
	else {
		info.setFaces(false);
	}
}

vcl::Tokenizer nextNonEmptyTokenizedLine(std::ifstream& file, char separator)
{
	std::string line;
	vcl::Tokenizer tokenizer;
	do {
		std::getline(file, line);
		if (!file) {
			throw vcl::MalformedFileException("Unexpected end of file.");
		}
		if (line.size() > 0) {
			str::removeWindowsNewLine(line);
			tokenizer = vcl::Tokenizer(line, separator);
		}
	} while (tokenizer.begin() == tokenizer.end());
	return tokenizer;
}

vcl::Tokenizer nextNonEmptyTokenizedLineNoThrow(std::ifstream& file, char separator)
{
	std::string line;
	vcl::Tokenizer tokenizer;
	do {
		std::getline(file, line);
		if (file && line.size() > 0) {
			str::removeWindowsNewLine(line);
			tokenizer = vcl::Tokenizer(line, separator);
		}
	} while (file && tokenizer.begin() == tokenizer.end());
	return tokenizer;
}

} // namespace vcl::io::internal
