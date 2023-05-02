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

#ifndef VCL_IO_FILE_MESH_INFO_H
#define VCL_IO_FILE_MESH_INFO_H

#include <array>
#include <list>
#include <string>
#include <typeindex>
#include <vector>
#include <bitset>

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief A simple class that allows to store which components has been loaded or are going
 * to be saved on a mesh file.
 *
 * When loading a Mesh from a file, an object of this type is used to know which Elements/Components
 * have been loaded from the file, using the getter functions:
 *
 * @code{.cpp}
 * FileMeshInfo info;
 * AMeshType m = vcl::load<AMeshType>("meshfile.ply", info);
 *
 * if (info.hasFaces()) { // the file had faces
 *     (info.hasFaceColors()) { // the file had face colors
 *         // ...
 *     }
 * }
 * @endcode
 *
 * When saving a Mesh to a file, an object of this type is used to tell which Elements/Components
 * save on the file and, when the file format supports it, to choose the type used to store a
 * specific component:
 *
 * @code{.cpp}
 * AMeshType m;
 * FileMeshInfo info(m); // compute the default FileMeshInfo object from the Mesh
 *
 * info.setVertexCoords(true, FileMeshInfo::FLOAT); // force to store vertex coords using floats
 * info.setVertexColors(false); // do not store
 *
 * vcl::save(m, "meshfile.ply", info);
 * @endcode
 *
 * @ingroup load_save
 */
class FileMeshInfo
{
public:
	/**
	 * @brief Enum used to describe the type of the Mesh - by default, a mesh is considered
	 * polygonal
	 */
	typedef enum { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH } MeshType;

	/**
	 * @brief Enum used to describe the type of Data
	 */
	typedef enum { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE, UNKNOWN } DataType;

	FileMeshInfo();

	template<MeshConcept Mesh>
	FileMeshInfo(const Mesh& m);

	/*
	 * Getter Elements/Components functions: they are used mostly after the loading of a Mesh from a
	 * file, to know if Elements/Components have been loaded.
	 */

	bool isTriangleMesh() const;
	bool isQuadMesh() const;
	bool isPolygonMesh() const;
	bool hasVertices() const;
	bool hasVertexCoords() const;
	bool hasVertexNormals() const;
	bool hasVertexColors() const;
	bool hasVertexScalars() const;
	bool hasVertexTexCoords() const;
	bool hasVertexCustomComponents() const;
	bool hasFaces() const;
	bool hasFaceVRefs() const;
	bool hasFaceNormals() const;
	bool hasFaceColors() const;
	bool hasFaceScalars() const;
	bool hasFaceWedgeTexCoords() const;
	bool hasFaceCustomComponents() const;
	bool hasEdges() const;
	bool hasEdgeVRefs() const;
	bool hasEdgeColors() const;
	bool hasTextures() const;

	/*
	 * Setter functions: they are used by the load functions to tell which Elements/Components are
	 * loaded from a file, and they can be used by the user that wants to save in a file only a
	 * specific set of Elements/Components of a Mesh.
	 */

	void setTriangleMesh();
	void setQuadMesh();
	void setPolygonMesh();
	void setMeshType(MeshType t);
	void setVertices(bool b = true);
	void setVertexCoords(bool b = true, DataType t = DOUBLE);
	void setVertexNormals(bool b = true, DataType t = FLOAT);
	void setVertexColors(bool b = true, DataType t = UCHAR);
	void setVertexScalars(bool b = true, DataType t = DOUBLE);
	void setVertexTexCoords(bool b = true, DataType t = FLOAT);
	void setVertexCustomComponents(bool b = true);
	void setFaces(bool b = true);
	void setFaceVRefs(bool b = true);
	void setFaceNormals(bool b = true, DataType t = FLOAT);
	void setFaceColors(bool b = true, DataType t = UCHAR);
	void setFaceScalars(bool b = true, DataType t = DOUBLE);
	void setFaceWedgeTexCoords(bool b = true, DataType t = FLOAT);
	void setFaceCustomComponents(bool b = true);
	void setEdges(bool b = true);
	void setEdgeVRefs(bool b = true);
	void setEdgeColors(bool b = true, DataType t = UCHAR);
	void setTextures(bool b = true);

	/*
	 * Getter Component type functions : they are used mostly by save functions to know the type
	 * that needs to use to save a given Component
	 */

	DataType vertexCoordsType() const;
	DataType vertexNormalsType() const;
	DataType vertexColorsType() const;
	DataType vertexScalarsType() const;
	DataType vertexTexCoordsType() const;
	DataType faceNormalsType() const;
	DataType faceColorsType() const;
	DataType faceScalarsType() const;
	DataType faceWedgeTexCoordsType() const;
	DataType edgeColorsType() const;

	FileMeshInfo intersect(const FileMeshInfo& i) const;

	void reset();

private:
	enum {
		VERTICES = 0,
		VERTEX_COORDS,
		VERTEX_NORMALS,
		VERTEX_COLORS,
		VERTEX_SCALAR,
		VERTEX_TEXCOORDS,
		VERTEX_CUSTOM_COMPONENTS,
		FACES,
		FACE_VREFS,
		FACE_NORMALS,
		FACE_COLORS,
		FACE_SCALAR,
		FACE_WEDGE_TEXCOORDS,
		FACE_CUSTOM_COMPONENTS,
		EDGES,
		EDGE_VREFS,
		EDGE_COLORS,
		TEXTURES,
		NUM_MODES
	};

	struct CustomComponent {
		std::string name;
		DataType type;
	};

	// Tells, for each mode, if it is enabled or not.
	std::bitset<NUM_MODES> mode = {false};

	// Tells, for each mode, the type of that mode. Does not apply for all the modes
	std::array<DataType, NUM_MODES> modeTypes = {UNKNOWN};

	// Mesh Type
	MeshType type = POLYGON_MESH;

	std::vector<CustomComponent> vertexCustomComponents;
	std::vector<CustomComponent> faceCustomComponents;

	template<typename T>
	static DataType getType();

	static DataType getType(std::type_index ti);
};

} // namespace vcl

#include "file_mesh_info.cpp"

#endif // VCL_IO_FILE_MESH_INFO_H
