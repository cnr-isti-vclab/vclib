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

#include "mesh_info.h"
#include "vclib/concepts/mesh/per_vertex.h"
#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Default constructor.
 *
 * All the Elements/Components are disabled, their type is set to DataType::UNKNOWN and the Mesh
 * Type is set to MeshType::POLYGON_MESH.
 */
inline MeshInfo::MeshInfo()
{
	perElemComponentsType.fill(UNKNOWN);
}

/**
 * @brief Sets the current status of the MeshInfo object from the input mesh.
 *
 * @tparam Mesh: The type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] m: the mesh from which construct the MeshInfo object
 */
template<MeshConcept Mesh>
MeshInfo::MeshInfo(const Mesh& m)
{
	setVertices();
	setVertexCoords(true, getType<typename Mesh::VertexType::CoordType::ScalarType>());
	if constexpr (vcl::HasPerVertexNormal<Mesh>)
		if (vcl::isPerVertexNormalEnabled(m))
			setVertexNormals(true, getType<typename Mesh::VertexType::NormalType::ScalarType>());
	if constexpr (vcl::HasPerVertexColor<Mesh>)
		if (vcl::isPerVertexColorEnabled(m))
			setVertexColors(true, UCHAR);
	if constexpr (vcl::HasPerVertexQuality<Mesh>)
		if (vcl::isPerVertexQualityEnabled(m))
			setVertexQuality(true, getType<typename Mesh::VertexType::QualityType>());
	if constexpr (vcl::HasPerVertexTexCoord<Mesh>)
		if (vcl::isPerVertexTexCoordEnabled(m))
			setVertexTexCoords(
				true,
				getType<typename Mesh::VertexType::TexCoordType::ScalarType>());
	if constexpr(vcl::HasPerVertexCustomComponents<Mesh>) {
		auto names = m.perVertexCustomComponentNames();
		for (auto& name : names) {
			DataType dt = getType(m.perVertexCustomComponentType(name));
			if (dt != UNKNOWN) {
				addVertexCustomComponent(name, dt);
			}
		}
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
				setFaceNormals(true, getType<typename Mesh::FaceType::NormalType::ScalarType>());
		if constexpr (vcl::HasPerFaceColor<Mesh>)
			if (vcl::isPerFaceColorEnabled(m))
				setFaceColors(true, UCHAR);
		if constexpr (vcl::HasPerFaceQuality<Mesh>)
			if (vcl::isPerFaceQualityEnabled(m))
				setFaceQuality(true, getType<typename Mesh::FaceType::QualityType>());
		if constexpr (vcl::HasPerFaceWedgeTexCoords<Mesh>)
			if (vcl::isPerFaceWedgeTexCoordsEnabled(m))
				setFaceWedgeTexCoords(true, getType<typename Mesh::FaceType::WedgeTexCoordType::ScalarType>());
		if constexpr(vcl::HasPerFaceCustomComponents<Mesh>) {
			auto names = m.perFaceCustomComponentNames();
			for (auto& name : names) {
				DataType dt = getType(m.perFaceCustomComponentType(name));
				if (dt != UNKNOWN) {
					addFaceCustomComponent(name, dt);
				}
			}
		}
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
inline bool MeshInfo::isTriangleMesh() const
{
	return type == TRIANGLE_MESH;
}

/**
 * @brief Returns true if the current object has Mesh type set to MeshType::TRIANGLE_MESH.
 * @return true if the current Mesh type is set to MeshType::TRIANGLE_MESH.
 */
inline bool MeshInfo::isQuadMesh() const
{
	return type == QUAD_MESH;
}

/**
 * @brief Returns true if the current object has Mesh type set to MeshType::POLYGON_MESH.
 * @return true if the current Mesh type is set to MeshType::POLYGON_MESH.
 */
inline bool MeshInfo::isPolygonMesh() const
{
	return type == POLYGON_MESH;
}

inline bool MeshInfo::hasElement(Element el) const
{
	return elements[el];
}

inline bool MeshInfo::hasPerElementComponent(Element el, Component comp) const
{
	return perElemComponents[el][comp];
}

/**
 * @brief Returns true if the current object has Vertex Elements.
 * @return true if the current object has Vertex Elements.
 */
inline bool MeshInfo::hasVertices() const
{
	return hasElement(VERTEX);
}

/**
 * @brief Returns true if the current object has Vertex Coordinates.
 * @return true if the current object has Vertex Coordinates.
 */
inline bool MeshInfo::hasVertexCoords() const
{
	return hasPerElementComponent(VERTEX, COORD);
}

/**
 * @brief Returns true if the current object has Vertex Normals.
 * @return true if the current object has Vertex Normals.
 */
inline bool MeshInfo::hasVertexNormals() const
{
	return hasPerElementComponent(VERTEX, NORMAL);
}

/**
 * @brief Returns true if the current object has Vertex Colors.
 * @return true if the current object has Vertex Colors.
 */
inline bool MeshInfo::hasVertexColors() const
{
	return hasPerElementComponent(VERTEX, COLOR);
}

/**
 * @brief Returns true if the current object has Vertex Quality.
 * @return true if the current object has Vertex Quality.
 */
inline bool MeshInfo::hasVertexQuality() const
{
	return hasPerElementComponent(VERTEX, QUALITY);
}

/**
 * @brief Returns true if the current object has Vertex Texture Coordinates.
 * @return true if the current object has Vertex Texture Coordinates.
 */
inline bool MeshInfo::hasVertexTexCoords() const
{
	return hasPerElementComponent(VERTEX, TEXCOORD);
}

/**
 * @brief Returns true if the current object has Vertex Custom Components.
 * @return true if the current object has Vertex Custom Components.
 */
inline bool MeshInfo::hasVertexCustomComponents() const
{
	return hasPerElementComponent(VERTEX, CUSTOM_COMPONENTS);
}

/**
 * @brief Returns true if the current object has Face Elements.
 * @return true if the current object has Face Elements.
 */
inline bool MeshInfo::hasFaces() const
{
	return hasElement(FACE);
}

/**
 * @brief Returns true if the current object has per Face Vertex References.
 * @return true if the current object has per Face Vertex References.
 */
inline bool MeshInfo::hasFaceVRefs() const
{
	return hasPerElementComponent(FACE, VREFS);
}

inline bool MeshInfo::hasFaceNormals() const
{
	return hasPerElementComponent(FACE, NORMAL);
}

inline bool MeshInfo::hasFaceColors() const
{
	return hasPerElementComponent(FACE, COLOR);
}

inline bool MeshInfo::hasFaceQuality() const
{
	return hasPerElementComponent(FACE, QUALITY);
}

inline bool MeshInfo::hasFaceWedgeTexCoords() const
{
	return hasPerElementComponent(FACE, WEDGE_TEXCOORDS);
}

inline bool MeshInfo::hasFaceCustomComponents() const
{
	return hasPerElementComponent(FACE, CUSTOM_COMPONENTS);
}

/**
 * @brief Returns true if the current object has Edge Elements.
 * @return true if the current object has Edge Elements.
 */
inline bool MeshInfo::hasEdges() const
{
	return hasElement(EDGE);
}

inline bool MeshInfo::hasEdgeVRefs() const
{
	return hasPerElementComponent(EDGE, VREFS);
}

inline bool MeshInfo::hasEdgeColors() const
{
	return hasPerElementComponent(EDGE, COLOR);
}

inline bool MeshInfo::hasTextures() const
{
	return hasPerElementComponent(MESH, TEXTURES);
}

inline void MeshInfo::setTriangleMesh()
{
	type = TRIANGLE_MESH;
}

inline void MeshInfo::setQuadMesh()
{
	type = QUAD_MESH;
}

inline void MeshInfo::setPolygonMesh()
{
	type = POLYGON_MESH;
}

inline void MeshInfo::setMeshType(MeshType t)
{
	type = t;
}

void MeshInfo::setElement(Element el, bool b)
{
	elements[el] = b;
}

void MeshInfo::setElementComponents(Element el, Component c, bool b, DataType t)
{
	elements[el] = b;
	perElemComponents[el][c] = b;
	if (b)
		perElemComponentsType(el, c) = t;
}

inline void MeshInfo::setVertices(bool b)
{
	setElement(VERTEX, b);
}

inline void MeshInfo::setVertexCoords(bool b, DataType t)
{
	setElementComponents(VERTEX, COORD, b, t);
}

inline void MeshInfo::setVertexNormals(bool b, DataType t)
{
	setElementComponents(VERTEX, NORMAL, b, t);
}

inline void MeshInfo::setVertexColors(bool b, DataType t)
{
	setElementComponents(VERTEX, COLOR, b, t);
}

inline void MeshInfo::setVertexQuality(bool b, DataType t)
{
	setElementComponents(VERTEX, QUALITY, b, t);
}

void MeshInfo::setVertexTexCoords(bool b, DataType t)
{
	setElementComponents(VERTEX, TEXCOORD, b, t);
}

inline void MeshInfo::setVertexCustomComponents(bool b)
{
	setElementComponents(VERTEX, CUSTOM_COMPONENTS, b, UNKNOWN);
}

inline void MeshInfo::setFaces(bool b)
{
	setElement(FACE, b);
}

inline void MeshInfo::setFaceVRefs(bool b)
{
	setElementComponents(FACE, VREFS, b, UNKNOWN);
}

inline void MeshInfo::setFaceNormals(bool b, DataType t)
{
	setElementComponents(FACE, NORMAL, b, t);
}

inline void MeshInfo::setFaceColors(bool b, DataType t)
{
	setElementComponents(FACE, COLOR, b, t);
}

inline void MeshInfo::setFaceQuality(bool b, DataType t)
{
	setElementComponents(FACE, QUALITY, b, t);
}

inline void MeshInfo::setFaceWedgeTexCoords(bool b, DataType t)
{
	setElementComponents(FACE, WEDGE_TEXCOORDS, b, t);
}

inline void MeshInfo::setFaceCustomComponents(bool b)
{
	setElementComponents(FACE, CUSTOM_COMPONENTS, b, UNKNOWN);
}

inline void MeshInfo::setEdges(bool b)
{
	setElement(EDGE, b);
}

inline void MeshInfo::setEdgeVRefs(bool b)
{
	setElementComponents(EDGE, VREFS, b, UNKNOWN);
}

inline void MeshInfo::setEdgeColors(bool b, DataType t)
{
	setElementComponents(EDGE, COLOR, b, t);
}

inline void MeshInfo::setTextures(bool b)
{
	setElementComponents(MESH, TEXTURES, b, UNKNOWN);
}

inline void MeshInfo::addElementCustomComponent(Element el, const std::string& name, DataType t)
{
	setElementComponents(el, CUSTOM_COMPONENTS, true, UNKNOWN);
	perElemCustomComponents[el].emplace_back(name, t);
}

inline void MeshInfo::clearElementCustomComponents(Element el)
{
	setElementComponents(el, CUSTOM_COMPONENTS, false, UNKNOWN);
	perElemCustomComponents[el].clear();
}

inline void MeshInfo::addVertexCustomComponent(const std::string& name, DataType t)
{
	addElementCustomComponent(VERTEX, name, t);
}

inline void MeshInfo::clearVertexCustomComponents()
{
	clearElementCustomComponents(VERTEX);
}

inline void MeshInfo::addFaceCustomComponent(const std::string &name, DataType t)
{
	addElementCustomComponent(FACE, name, t);
}

inline void MeshInfo::clearFaceCustomComponents()
{
	clearElementCustomComponents(FACE);
}

MeshInfo::DataType MeshInfo::elementComponentType(Element el, Component comp) const
{
	return perElemComponentsType(el, comp);
}

inline MeshInfo::DataType MeshInfo::vertexCoordsType() const
{
	return elementComponentType(VERTEX, COORD);
}

inline MeshInfo::DataType MeshInfo::vertexNormalsType() const
{
	return elementComponentType(VERTEX, NORMAL);
}

inline MeshInfo::DataType MeshInfo::vertexColorsType() const
{
	return elementComponentType(VERTEX, COLOR);
}

inline MeshInfo::DataType MeshInfo::vertexQualityType() const
{
	return elementComponentType(VERTEX, QUALITY);
}

inline MeshInfo::DataType MeshInfo::vertexTexCoordsType() const
{
	return elementComponentType(VERTEX, TEXCOORD);
}

inline MeshInfo::DataType MeshInfo::faceNormalsType() const
{
	return elementComponentType(FACE, NORMAL);
}

inline MeshInfo::DataType MeshInfo::faceColorsType() const
{
	return elementComponentType(FACE, COLOR);
}

inline MeshInfo::DataType MeshInfo::faceQualityType() const
{
	return elementComponentType(FACE, QUALITY);
}

inline MeshInfo::DataType MeshInfo::faceWedgeTexCoordsType() const
{
	return elementComponentType(FACE, WEDGE_TEXCOORDS);
}

inline MeshInfo::DataType MeshInfo::edgeColorsType() const
{
	return elementComponentType(EDGE, COLOR);
}

inline const std::vector<MeshInfo::CustomComponent>& MeshInfo::vertexCustomComponents() const
{
	return perElemCustomComponents[VERTEX];
}

inline const std::vector<MeshInfo::CustomComponent>& MeshInfo::faceCustomComponents() const
{
	return perElemCustomComponents[FACE];
}

/**
 * @brief Returns a MeshInfo object that is the intersection between this and
 * `info`.
 *
 * The intersection is a MeshInfo object that has Elements/Components enable
 * only if they are enabled both in this object and in `info`. Types are
 * imported from this MeshInfo.
 *
 * @param[in] info: The info object to compute the intersection with.
 * @return The intersection between this and `info`.
 */
inline MeshInfo MeshInfo::intersect(const MeshInfo& info) const
{
	MeshInfo res;
	for (uint i = 0; i < NUM_ELEMENTS; ++i) {
		res.elements[i] = elements[i] && info.elements[i];
		for (uint j = 0; j < NUM_COMPONENTS; ++j) {
			res.perElemComponents[i][j] = perElemComponents[i][j] && info.perElemComponents[i][j];

			if (res.perElemComponents[i][j]){
				res.perElemComponentsType(i, j) = perElemComponentsType(i, j);
			}
		}
	}

	if (type == info.type){
		res.type = type;
	}
	res.perElemCustomComponents = perElemCustomComponents;

	return res;
}

inline void MeshInfo::reset()
{
	elements.reset();
	for (auto& comp : perElemComponents)
		comp.reset();
	perElemComponentsType.fill(UNKNOWN);

	for (auto& v : perElemCustomComponents)
		v.clear();

	type = TRIANGLE_MESH;
}

/**
 * @brief Given the template T, returns the correspoding enum DataType value of T.
 *
 * Returns DataType::UNKNOWN if the type T was not part of the type sypported by the DataType enum.
 * @return
 */
template<typename T>
MeshInfo::DataType MeshInfo::getType()
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

MeshInfo::DataType MeshInfo::getType(std::type_index ti)
{
	if (ti == typeid(char)) return CHAR;
	if (ti == typeid(unsigned char)) return UCHAR;
	if (ti == typeid(short)) return SHORT;
	if (ti == typeid(unsigned short)) return USHORT;
	if (ti == typeid(int)) return INT;
	if (ti == typeid(uint)) return UINT;
	if (ti == typeid(float)) return FLOAT;
	if (ti == typeid(double)) return DOUBLE;
	return UNKNOWN;
}

} // namespace vcl
