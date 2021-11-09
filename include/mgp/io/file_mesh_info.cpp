#include "file_mesh_info.h"
#include <mgp/mesh/requirements.h>

namespace mgp::io {

inline FileMeshInfo::FileMeshInfo() : mode(0), type(TRIANGLE_MESH)
{
}

template<typename Mesh>
FileMeshInfo::FileMeshInfo(const Mesh& m) :
		mode(0), type(POLYGON_MESH)
{
	if (mgp::hasVertices(m)){
		setVertices();
		setVertexCoords();
		if (mgp::isPerVertexNormalEnabled(m))
			setVertexNormals();
		if (mgp::isPerVertexColorEnabled(m))
			setVertexColors();
	}

	if (mgp::hasFaces(m)){
		setFaces();
		setFaceVRefs();
		if (mgp::hasTriangles(m))
			setTriangleMesh();
		if (mgp::hasQuads(m))
			setQuadMesh();
		if (mgp::isPerFaceNormalEnabled(m))
			setFaceNormals();
		if (mgp::isPerFaceColorEnabled(m))
			setFaceNormals();
	}
}

inline bool FileMeshInfo::isTriangleMesh() const
{
	return type == TRIANGLE_MESH;
}

inline bool FileMeshInfo::isQuadMesh() const
{
	return type == QUAD_MESH;
}

inline bool FileMeshInfo::isPolygonMesh() const
{
	return type == POLYGON_MESH;
}

inline bool FileMeshInfo::hasVertices() const
{
	return mode[VERTICES];
}

inline bool FileMeshInfo::hasFaces() const
{
	return mode[FACES];
}

inline bool FileMeshInfo::hasVertexCoords() const
{
	return mode[VERTEX_COORDS];
}

inline bool FileMeshInfo::hasVertexNormals() const
{
	return mode[VERTEX_NORMALS];
}

inline bool FileMeshInfo::hasVertexColors() const
{
	return mode[VERTEX_COLORS];
}

bool FileMeshInfo::hasVertexScalar() const
{
	return mode[VERTEX_SCALAR];
}

inline bool FileMeshInfo::hasFaceVRefs() const
{
	return mode[FACE_VREFS];
}

inline bool FileMeshInfo::hasFaceNormals() const
{
	return mode[FACE_NORMALS];
}

inline bool FileMeshInfo::hasFaceColors() const
{
	return mode[FACE_COLORS];
}

bool FileMeshInfo::hasFaceScalar() const
{
	return mode[FACE_SCALAR];
}

inline bool FileMeshInfo::hasEdges() const
{
	return mode[EDGES];
}

inline bool FileMeshInfo::hasEdgeColors() const
{
	return mode[EDGE_COLORS];
}

inline void FileMeshInfo::setTriangleMesh()
{
	type = TRIANGLE_MESH;
}

inline void FileMeshInfo::setQuadMesh()
{
	type = QUAD_MESH;
}

inline void FileMeshInfo::setPolygonMesh()
{
	type = POLYGON_MESH;
}

inline void FileMeshInfo::setMeshType(MeshType t)
{
	type = t;
}

inline void FileMeshInfo::setVertices()
{
	mode[VERTICES] = true;
}

inline void FileMeshInfo::setVertexCoords()
{
	mode[VERTEX_COORDS] = true;
}

inline void FileMeshInfo::setVertexNormals()
{
	mode[VERTEX_NORMALS] = true;
}

inline void FileMeshInfo::setVertexColors()
{
	mode[VERTEX_COLORS] = true;
}

inline void FileMeshInfo::setVertexScalar()
{
	mode[VERTEX_SCALAR] = true;
}

void FileMeshInfo::setFaces()
{
	mode[FACES] = true;
}

inline void FileMeshInfo::setFaceVRefs()
{
	mode[FACE_VREFS] = true;
}

inline void FileMeshInfo::setFaceNormals()
{
	mode[FACE_NORMALS] = true;
}

inline void FileMeshInfo::setFaceColors()
{
	mode[FACE_COLORS] = true;
}

void FileMeshInfo::setFaceScalar()
{
	mode[FACE_SCALAR] = true;
}

inline void FileMeshInfo::setEdges()
{
	mode[EDGES] = true;
}

inline void FileMeshInfo::setEdgeColors()
{
	mode[EDGE_COLORS] = true;
}

inline void FileMeshInfo::reset()
{
	mode.reset();
	type = TRIANGLE_MESH;
}

} // namespace mgp::io
