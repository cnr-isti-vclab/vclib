/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_FILE_MESH_INFO_H
#define MGP_IO_FILE_MESH_INFO_H

#include <list>
#include <string>
#include <vector>
#include <bitset>

namespace mgp::io {



/**
 * @brief A simple class that allows to store which properties has been loaded or are going
 * to be saved on a mesh file.
 */
class FileMeshInfo
{
public:
	typedef enum { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH } MeshType;

	FileMeshInfo();

	template<typename Mesh>
	FileMeshInfo(const Mesh& m);

	bool isTriangleMesh() const;
	bool isQuadMesh() const;
	bool isPolygonMesh() const;
	bool hasVertices() const;
	bool hasFaces() const;
	bool hasVertexCoords() const;
	bool hasVertexNormals() const;
	bool hasVertexColors() const;
	bool hasVertexScalar() const;
	bool hasFaceVRefs() const;
	bool hasFaceNormals() const;
	bool hasFaceColors() const;
	bool hasFaceScalar() const;
	bool hasEdges() const;
	bool hasEdgeColors() const;

	void setTriangleMesh();
	void setQuadMesh();
	void setPolygonMesh();
	void setMeshType(MeshType t);
	void setVertices();
	void setVertexCoords();
	void setVertexNormals();
	void setVertexColors();
	void setVertexScalar();
	void setFaces();
	void setFaceVRefs();
	void setFaceNormals();
	void setFaceColors();
	void setFaceScalar();
	void setEdges();
	void setEdgeColors();

	void reset();

private:
	typedef enum {
		VERTICES = 0,
		VERTEX_COORDS,
		VERTEX_NORMALS,
		VERTEX_COLORS,
		VERTEX_SCALAR,
		FACES,
		FACE_VREFS,
		FACE_NORMALS,
		FACE_COLORS,
		FACE_SCALAR,
		EDGES,
		EDGE_COLORS,
		NUM_MODES
	} FMM;
	std::bitset<NUM_MODES> mode;
	MeshType type;
};

} // namespace mgp::io

#include "file_mesh_info.cpp"

#endif // MGP_IO_FILE_MESH_INFO_H
