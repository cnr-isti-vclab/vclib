/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_FILE_MESH_INFO_H
#define MGP_IO_FILE_MESH_INFO_H

#include <array>
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

	// Properties types, useful to set which type in binary files should be used to save a property
	typedef enum { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE, UNKNOWN } PropType;

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
	bool hasVertexScalars() const;
	bool hasFaceVRefs() const;
	bool hasFaceNormals() const;
	bool hasFaceColors() const;
	bool hasFaceScalars() const;
	bool hasEdges() const;
	bool hasEdgeColors() const;

	void setTriangleMesh();
	void setQuadMesh();
	void setPolygonMesh();
	void setMeshType(MeshType t);
	void setVertices();
	void setVertexCoords(PropType t = DOUBLE);
	void setVertexNormals(PropType t = FLOAT);
	void setVertexColors(PropType t = CHAR);
	void setVertexScalars(PropType t = DOUBLE);
	void setFaces();
	void setFaceVRefs();
	void setFaceNormals(PropType t = FLOAT);
	void setFaceColors(PropType t = CHAR);
	void setFaceScalars(PropType t = DOUBLE);
	void setEdges();
	void setEdgeColors(PropType t = CHAR);

	PropType vertexCoordsType() const;
	PropType vertexNormalsType() const;
	PropType vertexColorsType() const;
	PropType vertexScalarsType() const;
	PropType faceNormalsType() const;
	PropType faceColorsType() const;
	PropType faceScalarsType() const;
	PropType edgeColorsType() const;

	void reset();

private:
	enum {
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
	};
	std::bitset<NUM_MODES> mode = {false};
	std::array<PropType, NUM_MODES> modeTypes = {UNKNOWN};
	MeshType type = POLYGON_MESH;

	template<typename T>
	static PropType getPropType();
};

} // namespace mgp::io

#include "file_mesh_info.cpp"

#endif // MGP_IO_FILE_MESH_INFO_H
