/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_FILE_MESH_MODE_H
#define MGP_IO_FILE_MESH_MODE_H

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

	FileMeshInfo() : mode(0), type(TRIANGLE_MESH) {}
	FileMeshInfo(
		MeshType ft,
		bool         vn,
		bool         vc,
		bool         fn,
		bool         fc,
		bool         e  = false,
		bool         ec = false) :
			mode(0), type(ft)
	{
		if (vn)
			setVertexNormals();
		if (vc)
			setVertexColors();
		if (fn)
			setFaceNormals();
		if (fc)
			setFaceColors();
		if (e)
			setEdges();
		if (ec)
			setEdgeColors();
	}
	bool isTriangleMesh() const { return type == TRIANGLE_MESH; }
	bool isQuadMesh() const { return type == QUAD_MESH; }
	bool isPolygonMesh() const { return type == POLYGON_MESH; }
	bool hasVertexNormals() const { return mode & VERTEX_NORMALS; }
	bool hasVertexColors() const { return mode & VERTEX_COLORS; }
	bool hasFaceNormals() const { return mode & FACE_NORMALS; }
	bool hasFaceColors() const { return mode & FACE_COLORS; }
	bool hasEdges() const { return mode & EDGES; }
	bool hasEdgeColors() const { return mode & EDGE_COLORS; }

	void setTriangleMesh() { type = TRIANGLE_MESH; }
	void setQuadMesh() { type = QUAD_MESH; }
	void setPolygonMesh() { type = POLYGON_MESH; }
	void setMeshType(MeshType t) { type = t; }
	void setVertexNormals() { mode |= VERTEX_NORMALS; }
	void setVertexColors() { mode |= VERTEX_COLORS; }
	void setFaceNormals() { mode |= FACE_NORMALS; }
	void setFaceColors() { mode |= FACE_COLORS; }
	void setEdges() { mode |= EDGES; }
	void setEdgeColors() { mode |= EDGE_COLORS; }

	void reset()
	{
		mode = 0;
		type = TRIANGLE_MESH;
	}

private:
	typedef enum {
		VERTEX_NORMALS = 1 << 0,
		VERTEX_COLORS  = 1 << 1,
		FACE_COLORS    = 1 << 2,
		FACE_NORMALS   = 1 << 3,
		EDGES          = 1 << 4,
		EDGE_COLORS    = 1 << 5
	} FMM;
	int          mode;
	MeshType type;
};

} // namespace mgp::io

#endif // MGP_IO_FILE_MESH_MODE_H
