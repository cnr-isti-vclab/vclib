/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "load_ply.h"

#include <vclib/mesh/requirements.h>

namespace vcl::io {

namespace internal {

/**
 * @brief enableOptionalComponents enables all the components that are in the ply header and that
 * may be enabled in the mesh. If these components are not present in the mesh, the info file will
 * be modified telling that a particular property cannot be saved into the mesh.
 *
 * @param info
 * @param m
 */
template<typename MeshType>
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
	}
	else {
		info.setFaces(false);
	}
}

} // namespace internal

template<typename MeshType>
MeshType loadPly(const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	return loadPly<MeshType>(filename, loadedInfo, enableOptionalComponents);
}

template<typename MeshType>
MeshType loadPly(
	const std::string&     filename,
	vcl::io::FileMeshInfo& loadedInfo,
	bool                   enableOptionalComponents)
{
	MeshType m;
	loadPly(m, filename, loadedInfo, enableOptionalComponents);
	return m;
}

/**
 * @brief loadPly loads the given ply file and puts the content into the mesh m.
 *
 * The function will fill all the components read into the file that can be filled into the mesh.
 * If the enableOprionalComponents argument is enabled, some eventual optional components of the
 * mesh that were not enabled and that can be loaded from the file, will be enabled before loading
 * the file.
 *
 * If you need to know what elements and components have been loaded after the loading of the file,
 * please see the overload of the function with the additional 'loadedInfo' argument.
 *
 * @param m
 * @param filename
 * @param enableOptionalComponents
 */
template<typename MeshType>
void loadPly(MeshType& m, const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	loadPly(m, filename, loadedInfo, enableOptionalComponents);
}

/**
 * @brief loadPly loads the given ply file and puts the content into the mesh m.
 *
 * The function will fill all the components read into the file that can be filled into the mesh.
 * If the enableOprionalComponents argument is enabled, some eventual optional components of the
 * mesh that were not enabled and that can be loaded from the file, will be enabled before loading
 * the file.
 *
 * The info about what elements and components have been loaded from the file will be stored into
 * the loadedInfo argument.
 *
 * @param m
 * @param filename
 * @param loadedInfo
 * @param enableOptionalComponents
 */
template<typename MeshType>
void loadPly(
	MeshType&              m,
	const std::string&     filename,
	vcl::io::FileMeshInfo& loadedInfo,
	bool                   enableOptionalComponents)
{
	std::ifstream file(filename, std::ifstream::binary); // need to set binary of windows will fail
	if (!file.is_open()) {
		throw vcl::CannotOpenFileException(filename);
	}
	vcl::ply::PlyHeader header(filename, file);
	if (header.errorWhileLoading())
		throw MalformedFileException("Header not valid: " + filename);

	loadedInfo = header.getInfo();

	if (enableOptionalComponents)
		internal::enableOptionalComponents(loadedInfo, m);

	m.clear();
	try {
		for (ply::Element el : header) {
			switch (el.type) {
			case ply::VERTEX: ply::loadVertices(file, header, m); break;
			case ply::FACE: ply::loadFaces(file, header, m); break;
			case ply::TRISTRIP: ply::loadTriStrips(file, header, m); break;
			default: break;
			}
		}
		ply::loadTextures(header, m);
	}
	catch(const std::runtime_error& err) {
		m.clear();
		file.close();
		throw err;
	}

	file.close();
}

} // namespace vcl::io
