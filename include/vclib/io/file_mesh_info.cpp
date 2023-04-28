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

#include "file_mesh_info.h"
#include "vclib/concepts/mesh/per_vertex.h"
#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Default constructor.
 *
 * All the Elements/Components are disabled, their type is set to CompType::UNKNOWN and the Mesh
 * Type is set to MeshType::POLYGON_MESH.
 */
inline FileMeshInfo::FileMeshInfo()
{
}

/**
 * @brief Sets the current status of the FileMeshInfo object from the input mesh.
 *
 * @tparam Mesh: The type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] m: the mesh from which construct the FileMeshInfo object
 */
template<MeshConcept Mesh>
FileMeshInfo::FileMeshInfo(const Mesh& m)
{
	setVertices();
	setVertexCoords(true, getPropType<typename Mesh::VertexType::CoordType::ScalarType>());
	if constexpr (vcl::HasPerVertexNormal<Mesh>)
		if (vcl::isPerVertexNormalEnabled(m))
			setVertexNormals(true, getPropType<typename Mesh::VertexType::NormalType::ScalarType>());
	if constexpr (vcl::HasPerVertexColor<Mesh>)
		if (vcl::isPerVertexColorEnabled(m))
			setVertexColors(true, UCHAR);
	if constexpr (vcl::HasPerVertexScalar<Mesh>)
		if (vcl::isPerVertexScalarEnabled(m))
			setVertexScalars(true, getPropType<typename Mesh::VertexType::ScalarType>());
	if constexpr (vcl::HasPerVertexTexCoord<Mesh>)
		if (vcl::isPerVertexTexCoordEnabled(m))
			setVertexTexCoords(
				true,
				getPropType<typename Mesh::VertexType::TexCoordType::ScalarType>());
	if constexpr(vcl::HasPerVertexCustomComponents<Mesh>) {

	}

	if constexpr (vcl::HasFaces<Mesh>) {
		setFaces();
		setFaceVRefs();
		if (vcl::HasTriangles<Mesh>)
			setTriangleMesh();
		else if (vcl::HasQuads<Mesh>)
			setQuadMesh();
		else
			setPolygonMesh();
		if constexpr (vcl::HasPerFaceNormal<Mesh>)
			if (vcl::isPerFaceNormalEnabled(m))
				setFaceNormals(true, getPropType<typename Mesh::FaceType::NormalType::ScalarType>());
		if constexpr (vcl::HasPerFaceColor<Mesh>)
			if (vcl::isPerFaceColorEnabled(m))
				setFaceColors(true, UCHAR);
		if constexpr (vcl::HasPerFaceScalar<Mesh>)
			if (vcl::isPerFaceScalarEnabled(m))
				setFaceScalars(true, getPropType<typename Mesh::FaceType::ScalarType>());
		if constexpr (vcl::HasPerFaceWedgeTexCoords<Mesh>)
			if (vcl::isPerFaceWedgeTexCoordsEnabled(m))
				setFaceWedgeTexCoords(true, getPropType<typename Mesh::FaceType::WedgeTexCoordType::ScalarType>());
	}

	if constexpr (vcl::HasEdges<Mesh>) {
		setEdges();
		setEdgeVRefs();
//		if constexpr (vcl::HasPerEdgeColor<Mesh>)
//			if (vcl::isPerEdgeColorEnabled(m))
//				setEdgeColors(true, UCHAR);
	}

	if constexpr (vcl::HasTexturePaths<Mesh>) {
		if (m.textureNumber() > 0) {
			setTextures(true);
		}
	}
}

/**
 * @brief Returns true if the current object has Mesh type set to MeshType::QUAD_MESH.
 * @return true if the current Mesh type is set to MeshType::QUAD_MESH.
 */
inline bool FileMeshInfo::isTriangleMesh() const
{
	return type == TRIANGLE_MESH;
}

/**
 * @brief Returns true if the current object has Mesh type set to MeshType::TRIANGLE_MESH.
 * @return true if the current Mesh type is set to MeshType::TRIANGLE_MESH.
 */
inline bool FileMeshInfo::isQuadMesh() const
{
	return type == QUAD_MESH;
}

/**
 * @brief Returns true if the current object has Mesh type set to MeshType::POLYGON_MESH.
 * @return true if the current Mesh type is set to MeshType::POLYGON_MESH.
 */
inline bool FileMeshInfo::isPolygonMesh() const
{
	return type == POLYGON_MESH;
}

/**
 * @brief Returns true if the current object has Vertex Elements.
 * @return true if the current object has Vertex Elements.
 */
inline bool FileMeshInfo::hasVertices() const
{
	return mode[VERTICES];
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

inline bool FileMeshInfo::hasVertexTexCoords() const
{
	return mode[VERTEX_TEXCOORDS];
}

inline bool FileMeshInfo::hasVertexCustomComponents() const
{
	return mode[VERTEX_CUSTOM_COMPONENTS];
}

/**
 * @brief Returns true if the current object has Face Elements.
 * @return true if the current object has Face Elements.
 */
inline bool FileMeshInfo::hasFaces() const
{
	return mode[FACES];
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

inline bool FileMeshInfo::hasFaceWedgeTexCoords() const
{
	return mode[FACE_WEDGE_TEXCOORDS];
}

inline bool FileMeshInfo::hasFaceCustomComponents() const
{
	return mode[FACE_CUSTOM_COMPONENTS];
}

/**
 * @brief Returns true if the current object has Edge Elements.
 * @return true if the current object has Edge Elements.
 */
inline bool FileMeshInfo::hasEdges() const
{
	return mode[EDGES];
}

inline bool FileMeshInfo::hasEdgeVRefs() const
{
	return mode[EDGE_VREFS];
}

inline bool FileMeshInfo::hasEdgeColors() const
{
	return mode[EDGE_COLORS];
}

inline bool FileMeshInfo::hasTextures() const
{
	return mode[TEXTURES];
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

inline void FileMeshInfo::setVertices(bool b)
{
	mode[VERTICES] = b;
}

inline void FileMeshInfo::setVertexCoords(bool b, CompType t)
{
	mode[VERTEX_COORDS] = b;
	if (b)
		modeTypes[VERTEX_COORDS] = t;
}

inline void FileMeshInfo::setVertexNormals(bool b, CompType t)
{
	mode[VERTEX_NORMALS] = b;
	if (b)
		modeTypes[VERTEX_NORMALS] = t;
}

inline void FileMeshInfo::setVertexColors(bool b, CompType t)
{
	mode[VERTEX_COLORS] = b;
	if (b)
		modeTypes[VERTEX_COLORS] = t;
}

inline void FileMeshInfo::setVertexScalars(bool b, CompType t)
{
	mode[VERTEX_SCALAR] = b;
	if (b)
		modeTypes[VERTEX_SCALAR] = t;
}

void FileMeshInfo::setVertexTexCoords(bool b, CompType t)
{
	mode[VERTEX_TEXCOORDS] = b;
	if (b)
		modeTypes[VERTEX_TEXCOORDS] = t;
}

inline void FileMeshInfo::setVertexCustomComponents(bool b)
{
	mode[VERTEX_CUSTOM_COMPONENTS] = b;
}

inline void FileMeshInfo::setFaces(bool b)
{
	mode[FACES] = b;
}

inline void FileMeshInfo::setFaceVRefs(bool b)
{
	mode[FACE_VREFS] = b;
}

inline void FileMeshInfo::setFaceNormals(bool b, CompType t)
{
	mode[FACE_NORMALS] = b;
	if (b)
		modeTypes[FACE_NORMALS] = t;
}

inline void FileMeshInfo::setFaceColors(bool b, CompType t)
{
	mode[FACE_COLORS] = b;
	if (b)
		modeTypes[FACE_COLORS] = t;
}

inline void FileMeshInfo::setFaceScalars(bool b, CompType t)
{
	mode[FACE_SCALAR] = b;
	if (b)
		modeTypes[FACE_SCALAR] = t;
}

inline void FileMeshInfo::setFaceWedgeTexCoords(bool b, CompType t)
{
	mode[FACE_WEDGE_TEXCOORDS] = b;
	if (b)
		modeTypes[FACE_WEDGE_TEXCOORDS] = t;
}

inline void FileMeshInfo::setFaceCustomComponents(bool b)
{
	mode[FACE_CUSTOM_COMPONENTS] = b;
}

inline void FileMeshInfo::setEdges(bool b)
{
	mode[EDGES] = b;
}

inline void FileMeshInfo::setEdgeVRefs(bool b)
{
	mode[EDGE_VREFS] = b;
}

inline void FileMeshInfo::setEdgeColors(bool b, CompType t)
{
	mode[EDGE_COLORS] = b;
	if (b)
		modeTypes[EDGE_COLORS] = t;
}

inline void FileMeshInfo::setTextures(bool b)
{
	mode[TEXTURES] = b;
}

inline FileMeshInfo::CompType FileMeshInfo::vertexCoordsType() const
{
	return modeTypes[VERTEX_COORDS];
}

inline FileMeshInfo::CompType FileMeshInfo::vertexNormalsType() const
{
	return modeTypes[VERTEX_NORMALS];
}

inline FileMeshInfo::CompType FileMeshInfo::vertexColorsType() const
{
	return modeTypes[VERTEX_COLORS];
}

inline FileMeshInfo::CompType FileMeshInfo::vertexScalarsType() const
{
	return modeTypes[VERTEX_SCALAR];
}

inline FileMeshInfo::CompType FileMeshInfo::vertexTexCoordsType() const
{
	return modeTypes[VERTEX_TEXCOORDS];
}

inline FileMeshInfo::CompType FileMeshInfo::faceNormalsType() const
{
	return modeTypes[FACE_NORMALS];
}

inline FileMeshInfo::CompType FileMeshInfo::faceColorsType() const
{
	return modeTypes[FACE_COLORS];
}

inline FileMeshInfo::CompType FileMeshInfo::faceScalarsType() const
{
	return modeTypes[FACE_SCALAR];
}

inline FileMeshInfo::CompType FileMeshInfo::faceWedgeTexCoordsType() const
{
	return modeTypes[FACE_WEDGE_TEXCOORDS];
}

inline FileMeshInfo::CompType FileMeshInfo::edgeColorsType() const
{
	return modeTypes[EDGE_COLORS];
}

/**
 * @brief Returns a FileMeshInfo object that is the intersection between this and `info`.
 *
 * The intersection is a FileMeshInfo object that has Elements/Components enable only if they are
 * enabled both in this object and in `info`.
 *
 * @param[in] info: The info object to compute the intersection with.
 * @return The intersection between this and `info`.
 */
inline FileMeshInfo FileMeshInfo::intersect(const FileMeshInfo& info) const
{
	FileMeshInfo res;
	for (uint i = 0; i < NUM_MODES; ++i){
		res.mode[i] = mode[i] && info.mode[i];
		if (res.mode[i]){
			res.modeTypes[i] = mode[i] ? modeTypes[i] : info.modeTypes[i];
		}
	}
	if (type == info.type){
		res.type = type;
	}
	return res;
}

inline void FileMeshInfo::reset()
{
	mode.reset();
	modeTypes = {UNKNOWN};
	type = TRIANGLE_MESH;
}

template<typename T>
FileMeshInfo::CompType FileMeshInfo::getPropType()
{
	if constexpr (std::is_same_v<T, char>) return CHAR;
	if constexpr (std::is_same_v<T, unsigned char>) return UCHAR;
	if constexpr (std::is_same_v<T, short>) return SHORT;
	if constexpr (std::is_same_v<T, unsigned short>) return USHORT;
	if constexpr (std::is_same_v<T, int>) return INT;
	if constexpr (std::is_same_v<T, uint>) return UINT;
	if constexpr (std::is_integral_v<T>) return INT; // fallback to int
	if constexpr (std::is_same_v<T, float>) return FLOAT;
	if constexpr (std::is_same_v<T, double>) return DOUBLE;
	if constexpr (std::is_floating_point_v<T>) return FLOAT; // fallback to float
	return UNKNOWN;
}

} // namespace vcl
