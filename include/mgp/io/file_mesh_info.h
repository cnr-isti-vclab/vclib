/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_FILE_MESH_INFO_H
#define MGP_IO_FILE_MESH_INFO_H

#include <list>
#include <string>
#include <vector>

namespace mgp::io {



/**
 * @brief A simple class that allows to store which properties has been loaded or are going
 * to be saved on a mesh file.
 */
class FileMeshInfo
{
public:
	typedef enum { RGB, RGBA } ColorMode;

	typedef enum { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH } MeshType;

	FileMeshInfo();
	FileMeshInfo(
		MeshType ft,
		bool         vn,
		bool         vc,
		bool         fn,
		bool         fc,
		bool         e  = false,
		bool         ec = false);
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
	bool hasFaceVRefs() const;
	bool hasFaceNormals() const;
	bool hasFaceColors() const;
	bool hasEdges() const;
	bool hasEdgeColors() const;

	void setTriangleMesh();
	void setQuadMesh();
	void setPolygonMesh();
	void setMeshType(MeshType t);
	void setVertexCoords();
	void setVertexNormals();
	void setVertexColors();
	void setFaceVRefs();
	void setFaceNormals();
	void setFaceColors();
	void setEdges();
	void setEdgeColors();

	void reset();

private:
	typedef enum {
		VERTEX_COORDS  = 1 << 0,
		VERTEX_NORMALS = 1 << 1,
		VERTEX_COLORS  = 1 << 2,
		FACE_VREFS     = 1 << 3,
		FACE_COLORS    = 1 << 4,
		FACE_NORMALS   = 1 << 5,
		EDGES          = 1 << 6,
		EDGE_COLORS    = 1 << 7
	} FMM;
	int          mode;
	MeshType type;
};

} // namespace mgp::io

#include "file_mesh_info.cpp"

#endif // MGP_IO_FILE_MESH_INFO_H
