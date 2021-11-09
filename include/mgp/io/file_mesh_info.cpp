#include "file_mesh_info.h"
#include <mgp/mesh/requirements.h>

namespace mgp::io {

inline FileMeshInfo::FileMeshInfo()
{
}

template<typename Mesh>
inline FileMeshInfo::FileMeshInfo(const Mesh& m)
{
	if (mgp::hasVertices(m)){
		setVertices();
		setVertexCoords(getPropType<typename Mesh::VertexType::CoordinateType::ScalarType>());
		if constexpr (mgp::hasPerVertexNormal(m))
			if (mgp::isPerVertexNormalEnabled(m))
				setVertexNormals(getPropType<typename Mesh::VertexType::NormalType::ScalarType>());
		if constexpr (mgp::hasPerVertexColor(m))
			if (mgp::isPerVertexColorEnabled(m))
				setVertexColors(CHAR);
		if constexpr (mgp::hasPerVertexScalar(m))
			if (mgp::isPerVertexScalarEnabled(m))
				setVertexScalars(getPropType<typename Mesh::VertexType::ScalarType>());
	}

	if (mgp::hasFaces(m)){
		setFaces();
		setFaceVRefs();
		if (mgp::hasTriangles(m))
			setTriangleMesh();
		if (mgp::hasQuads(m))
			setQuadMesh();
		if constexpr (mgp::hasPerFaceNormal(m))
			if (mgp::isPerFaceNormalEnabled(m))
				setFaceNormals(getPropType<typename Mesh::FaceType::NormalType::ScalarType>());
		if constexpr (mgp::hasPerFaceColor(m))
			if (mgp::isPerFaceColorEnabled(m))
				setFaceColors(CHAR);
		if constexpr (mgp::hasPerFaceScalar(m))
			if (mgp::isPerFaceScalarEnabled(m))
				setVertexScalars(getPropType<typename Mesh::FaceType::ScalarType>());
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

inline bool FileMeshInfo::hasVertexScalars() const
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

inline bool FileMeshInfo::hasFaceScalars() const
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

inline void FileMeshInfo::setVertexCoords(PropType t)
{
	mode[VERTEX_COORDS] = true;
	modeTypes[VERTEX_COORDS] = t;
}

inline void FileMeshInfo::setVertexNormals(PropType t)
{
	mode[VERTEX_NORMALS] = true;
	modeTypes[VERTEX_NORMALS] = t;
}

inline void FileMeshInfo::setVertexColors(PropType t)
{
	mode[VERTEX_COLORS] = true;
	modeTypes[VERTEX_COLORS] = t;
}

inline void FileMeshInfo::setVertexScalars(PropType t)
{
	mode[VERTEX_SCALAR] = true;
	modeTypes[VERTEX_SCALAR] = t;
}

inline void FileMeshInfo::setFaces()
{
	mode[FACES] = true;
}

inline void FileMeshInfo::setFaceVRefs()
{
	mode[FACE_VREFS] = true;
}

inline void FileMeshInfo::setFaceNormals(PropType t)
{
	mode[FACE_NORMALS] = true;
	modeTypes[FACE_NORMALS] = t;
}

inline void FileMeshInfo::setFaceColors(PropType t)
{
	mode[FACE_COLORS] = true;
	modeTypes[FACE_COLORS] = t;
}

inline void FileMeshInfo::setFaceScalars(PropType t)
{
	mode[FACE_SCALAR] = true;
	modeTypes[FACE_SCALAR] = t;
}

inline void FileMeshInfo::setEdges()
{
	mode[EDGES] = true;
}

inline void FileMeshInfo::setEdgeColors(PropType t)
{
	mode[EDGE_COLORS] = true;
	modeTypes[EDGE_COLORS] = t;
}

inline FileMeshInfo::PropType FileMeshInfo::vertexCoordsType() const
{
	return modeTypes[VERTEX_COORDS];
}

inline FileMeshInfo::PropType FileMeshInfo::vertexNormalsType() const
{
	return modeTypes[VERTEX_NORMALS];
}

inline FileMeshInfo::PropType FileMeshInfo::vertexColorsType() const
{
	return modeTypes[VERTEX_COLORS];
}

inline FileMeshInfo::PropType FileMeshInfo::vertexScalarsType() const
{
	return modeTypes[VERTEX_SCALAR];
}

inline FileMeshInfo::PropType FileMeshInfo::faceNormalsType() const
{
	return modeTypes[FACE_NORMALS];
}

inline FileMeshInfo::PropType FileMeshInfo::faceColorsType() const
{
	return modeTypes[FACE_COLORS];
}

inline FileMeshInfo::PropType FileMeshInfo::faceScalarsType() const
{
	return modeTypes[FACE_SCALAR];
}

inline FileMeshInfo::PropType FileMeshInfo::edgeColorsType() const
{
	return modeTypes[EDGE_COLORS];
}

inline void FileMeshInfo::reset()
{
	mode.reset();
	modeTypes = {UNKNOWN};
	type = TRIANGLE_MESH;
}

template<typename T>
FileMeshInfo::PropType FileMeshInfo::getPropType()
{
	if constexpr (std::is_same_v<T, char>) return CHAR;
	if constexpr (std::is_same_v<T, unsigned char>) return UCHAR;
	if constexpr (std::is_same_v<T, short>) return SHORT;
	if constexpr (std::is_same_v<T, unsigned short>) return USHORT;
	if constexpr (std::is_same_v<T, int>) return INT;
	if constexpr (std::is_same_v<T, unsigned int>) return UINT;
	if constexpr (std::is_integral_v<T>) return INT; // fallback to int
	if constexpr (std::is_same_v<T, float>) return FLOAT;
	if constexpr (std::is_same_v<T, double>) return DOUBLE;
	if constexpr (std::is_floating_point_v<T>) return FLOAT; // fallback to float
	return UNKNOWN;
}

} // namespace mgp::io
