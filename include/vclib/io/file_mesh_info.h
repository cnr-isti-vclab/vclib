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

#ifndef VCL_IO_FILE_MESH_INFO_H
#define VCL_IO_FILE_MESH_INFO_H

#include <array>
#include <list>
#include <string>
#include <vector>
#include <bitset>

namespace vcl::io {



/**
 * @brief A simple class that allows to store which components has been loaded or are going
 * to be saved on a mesh file.
 */
class FileMeshInfo
{
public:
	typedef enum { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH } MeshType;

	// Types, useful to set which type in binary files should be used to save a component
	typedef enum { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE, UNKNOWN } PropType;

	FileMeshInfo();

	template<typename Mesh>
	FileMeshInfo(const Mesh& m);

	bool isTriangleMesh() const;
	bool isQuadMesh() const;
	bool isPolygonMesh() const;
	bool hasVertices() const;
	bool hasVertexCoords() const;
	bool hasVertexNormals() const;
	bool hasVertexColors() const;
	bool hasVertexScalars() const;
	bool hasVertexTexCoords() const;
	bool hasFaces() const;
	bool hasFaceVRefs() const;
	bool hasFaceNormals() const;
	bool hasFaceColors() const;
	bool hasFaceScalars() const;
	bool hasFaceWedgeTexCoords() const;
	bool hasEdges() const;
	bool hasEdgeColors() const;
	bool hasTextures() const;

	void setTriangleMesh();
	void setQuadMesh();
	void setPolygonMesh();
	void setMeshType(MeshType t);
	void setVertices(bool b = true);
	void setVertexCoords(bool b = true, PropType t = DOUBLE);
	void setVertexNormals(bool b = true, PropType t = FLOAT);
	void setVertexColors(bool b = true, PropType t = UCHAR);
	void setVertexScalars(bool b = true, PropType t = DOUBLE);
	void setVertexTexCoords(bool b = true, PropType t = FLOAT);
	void setFaces(bool b = true);
	void setFaceVRefs(bool b = true);
	void setFaceNormals(bool b = true, PropType t = FLOAT);
	void setFaceColors(bool b = true, PropType t = UCHAR);
	void setFaceScalars(bool b = true, PropType t = DOUBLE);
	void setFaceWedgeTexCoords(bool b = true, PropType t = FLOAT);
	void setEdges(bool b = true);
	void setEdgeColors(bool b = true, PropType t = UCHAR);
	void setTextures(bool b = true);

	PropType vertexCoordsType() const;
	PropType vertexNormalsType() const;
	PropType vertexColorsType() const;
	PropType vertexScalarsType() const;
	PropType vertexTexCoordsType() const;
	PropType faceNormalsType() const;
	PropType faceColorsType() const;
	PropType faceScalarsType() const;
	PropType faceWedgeTexCoordsType() const;
	PropType edgeColorsType() const;

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
		FACES,
		FACE_VREFS,
		FACE_NORMALS,
		FACE_COLORS,
		FACE_SCALAR,
		FACE_WEDGE_TEXCOORDS,
		EDGES,
		EDGE_COLORS,
		TEXTURES,
		NUM_MODES
	};
	std::bitset<NUM_MODES> mode = {false};
	std::array<PropType, NUM_MODES> modeTypes = {UNKNOWN};
	MeshType type = POLYGON_MESH;

	template<typename T>
	static PropType getPropType();
};

} // namespace vcl::io

#include "file_mesh_info.cpp"

#endif // VCL_IO_FILE_MESH_INFO_H
