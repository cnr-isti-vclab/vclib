/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "load_ply.h"

#include <mgp/mesh/requirements.h>

namespace mgp::io {

namespace internal {

/**
 * @brief enableOptionalComponents enables all the properties that are in the ply header and that
 * may be enabled in the mesh. If these properties are not present in the mesh, the info file will
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
			if (!mgp::enableIfPerVertexColorOptional(m)) {
				info.setVertexColors(false);
			}
		}
		if (info.hasVertexNormals()) {
			if (!mgp::enableIfPerVertexNormalOptional(m)) {
				info.setVertexNormals(false);
			}
		}
		if (info.hasVertexScalars()) {
			if (!mgp::enableIfPerVertexScalarOptional(m)) {
				info.setVertexScalars(false);
			}
		}
	}
	else {
		info.setVertices(false);
	}

	if (info.hasFaces()) {
		if (info.hasFaceColors()) {
			if (!mgp::enableIfPerFaceColorOptional(m)) {
				info.setFaceColors(false);
			}
		}
		if (info.hasFaceNormals()) {
			if (!mgp::enableIfPerFaceNormalOptional(m)) {
				info.setFaceNormals(false);
			}
		}
		if (info.hasFaceScalars()) {
			if (!mgp::enableIfPerFaceScalarOptional(m)) {
				info.setFaceScalars(false);
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
	mgp::io::FileMeshInfo& loadedInfo,
	bool                   enableOptionalComponents)
{
	MeshType m;
	loadPly(m, filename, loadedInfo, enableOptionalComponents);
	return m;
}

/**
 * @brief loadPly loads the given ply file and puts the content into the mesh m.
 *
 * The function will fill all the properties read into the file that can be filled into the mesh.
 * If the enableOprionalComponents argument is enabled, some eventual optional properties of the
 * mesh that were not enabled and that can be loaded from the file, will be enabled before loading
 * the file.
 *
 * If you need to know what elements and properties have been loaded after the loading of the file,
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
 * The function will fill all the properties read into the file that can be filled into the mesh.
 * If the enableOprionalComponents argument is enabled, some eventual optional properties of the
 * mesh that were not enabled and that can be loaded from the file, will be enabled before loading
 * the file.
 *
 * The info about what elements and properties have been loaded from the file will be stored into
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
	mgp::io::FileMeshInfo& loadedInfo,
	bool                   enableOptionalComponents)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw mgp::CannotOpenFileException(filename);
	}
	mgp::ply::PlyHeader header(file);
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
			default: break;
			}
		}
	}
	catch(const std::runtime_error& err) {
		m.clear();
		file.close();
		throw err;
	}

	file.close();
}

} // namespace mgp::io
