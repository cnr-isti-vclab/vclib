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

#include "file_mesh_info.h"
#include <vclib/mesh/requirements.h>

namespace vcl::io {

inline FileMeshInfo::FileMeshInfo()
{
}

template<typename Mesh>
inline FileMeshInfo::FileMeshInfo(const Mesh& m)
{
	if (vcl::hasVertices<Mesh>()){
		setVertices();
		setVertexCoords(getPropType<typename Mesh::VertexType::CoordType::ScalarType>());
		if constexpr (vcl::hasPerVertexNormal<Mesh>())
			if (vcl::isPerVertexNormalEnabled(m))
				setVertexNormals(getPropType<typename Mesh::VertexType::NormalType::ScalarType>());
		if constexpr (vcl::hasPerVertexColor<Mesh>())
			if (vcl::isPerVertexColorEnabled(m))
				setVertexColors(CHAR);
		if constexpr (vcl::hasPerVertexScalar<Mesh>())
			if (vcl::isPerVertexScalarEnabled(m))
				setVertexScalars(getPropType<typename Mesh::VertexType::ScalarType>());
		if constexpr (vcl::hasPerVertexTexCoord<Mesh>())
			if (vcl::isPerVertexTexCoordEnabled(m))
				setVertexTexCoords(
					getPropType<typename Mesh::VertexType::TexCoordType::ScalarType>());
	}

	if (vcl::hasFaces<Mesh>()){
		setFaces();
		setFaceVRefs();
		if (vcl::hasTriangles<Mesh>())
			setTriangleMesh();
		else if (vcl::hasQuads<Mesh>())
			setQuadMesh();
		else
			setPolygonMesh();
		if constexpr (vcl::hasPerFaceNormal<Mesh>())
			if (vcl::isPerFaceNormalEnabled(m))
				setFaceNormals(getPropType<typename Mesh::FaceType::NormalType::ScalarType>());
		if constexpr (vcl::hasPerFaceColor<Mesh>())
			if (vcl::isPerFaceColorEnabled(m))
				setFaceColors(CHAR);
		if constexpr (vcl::hasPerFaceScalar<Mesh>())
			if (vcl::isPerFaceScalarEnabled(m))
				setFaceScalars(getPropType<typename Mesh::FaceType::ScalarType>());
		if constexpr (vcl::hasPerFaceWedgeTexCoords<Mesh>())
			if (vcl::isPerFaceWedgeTexCoordsEnabled(m))
				setFaceWedgeTexCoords(getPropType<typename Mesh::FaceType::WedgeTexCoordType::ScalarType>());
	}
	if (vcl::hasTextureFileNames<Mesh>()) {
		if (m.textureNumber() > 0) {
			setTextures(true);
		}
		else {
			setTextures(false);
		}
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

bool FileMeshInfo::hasVertexTexCoords() const
{
	return mode[VERTEX_TEXCOORDS];
}

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

bool FileMeshInfo::hasFaceWedgeTexCoords() const
{
	return mode[FACE_WEDGE_TEXCOORDS];
}

inline bool FileMeshInfo::hasEdges() const
{
	return mode[EDGES];
}

inline bool FileMeshInfo::hasEdgeColors() const
{
	return mode[EDGE_COLORS];
}

bool FileMeshInfo::hasTextures() const
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

inline void FileMeshInfo::setVertexCoords(bool b, PropType t)
{
	mode[VERTEX_COORDS] = b;
	if (b)
		modeTypes[VERTEX_COORDS] = t;
}

inline void FileMeshInfo::setVertexNormals(bool b, PropType t)
{
	mode[VERTEX_NORMALS] = b;
	if (b)
		modeTypes[VERTEX_NORMALS] = t;
}

inline void FileMeshInfo::setVertexColors(bool b, PropType t)
{
	mode[VERTEX_COLORS] = b;
	if (b)
		modeTypes[VERTEX_COLORS] = t;
}

inline void FileMeshInfo::setVertexScalars(bool b, PropType t)
{
	mode[VERTEX_SCALAR] = b;
	if (b)
		modeTypes[VERTEX_SCALAR] = t;
}

void FileMeshInfo::setVertexTexCoords(bool b, PropType t)
{
	mode[VERTEX_TEXCOORDS] = b;
	if (b)
		modeTypes[VERTEX_TEXCOORDS] = t;
}

inline void FileMeshInfo::setFaces(bool b)
{
	mode[FACES] = b;
}

inline void FileMeshInfo::setFaceVRefs(bool b)
{
	mode[FACE_VREFS] = b;
}

inline void FileMeshInfo::setFaceNormals(bool b, PropType t)
{
	mode[FACE_NORMALS] = b;
	if (b)
		modeTypes[FACE_NORMALS] = t;
}

inline void FileMeshInfo::setFaceColors(bool b, PropType t)
{
	mode[FACE_COLORS] = b;
	if (b)
		modeTypes[FACE_COLORS] = t;
}

inline void FileMeshInfo::setFaceScalars(bool b, PropType t)
{
	mode[FACE_SCALAR] = b;
	if (b)
		modeTypes[FACE_SCALAR] = t;
}

void FileMeshInfo::setFaceWedgeTexCoords(bool b, PropType t)
{
	mode[FACE_WEDGE_TEXCOORDS] = b;
	if (b)
		modeTypes[FACE_WEDGE_TEXCOORDS] = t;
}

inline void FileMeshInfo::setEdges(bool b)
{
	mode[EDGES] = b;
}

inline void FileMeshInfo::setEdgeColors(bool b, PropType t)
{
	mode[EDGE_COLORS] = b;
	if (b)
		modeTypes[EDGE_COLORS] = t;
}

void FileMeshInfo::setTextures(bool b)
{
	mode[TEXTURES] = b;
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

FileMeshInfo::PropType FileMeshInfo::vertexTexCoordsType() const
{
	return modeTypes[VERTEX_TEXCOORDS];
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

FileMeshInfo::PropType FileMeshInfo::faceWedgeTexCoordsType() const
{
	return modeTypes[FACE_WEDGE_TEXCOORDS];
}

inline FileMeshInfo::PropType FileMeshInfo::edgeColorsType() const
{
	return modeTypes[EDGE_COLORS];
}

FileMeshInfo FileMeshInfo::intersect(const FileMeshInfo& info) const
{
	FileMeshInfo res;
	for (uint i = 0; i < NUM_MODES; ++i){
		res.mode[i] = mode[i] & info.mode[i];
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
FileMeshInfo::PropType FileMeshInfo::getPropType()
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

} // namespace vcl::io
