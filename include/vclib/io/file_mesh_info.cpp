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
 * All the Elements/Components are disabled, their type is set to DataType::UNKNOWN and the Mesh
 * Type is set to MeshType::POLYGON_MESH.
 */
inline FileMeshInfo::FileMeshInfo()
{
	perElemComponentsType.fill(UNKNOWN);
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
	setVertexCoords(true, getType<typename Mesh::VertexType::CoordType::ScalarType>());
	if constexpr (vcl::HasPerVertexNormal<Mesh>)
		if (vcl::isPerVertexNormalEnabled(m))
			setVertexNormals(true, getType<typename Mesh::VertexType::NormalType::ScalarType>());
	if constexpr (vcl::HasPerVertexColor<Mesh>)
		if (vcl::isPerVertexColorEnabled(m))
			setVertexColors(true, UCHAR);
	if constexpr (vcl::HasPerVertexScalar<Mesh>)
		if (vcl::isPerVertexScalarEnabled(m))
			setVertexScalars(true, getType<typename Mesh::VertexType::ScalarType>());
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
		if constexpr (vcl::HasPerFaceScalar<Mesh>)
			if (vcl::isPerFaceScalarEnabled(m))
				setFaceScalars(true, getType<typename Mesh::FaceType::ScalarType>());
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

inline bool FileMeshInfo::hasElement(Element el) const
{
	return elements[el];
}

inline bool FileMeshInfo::hasPerElementComponent(Element el, Component comp) const
{
	return perElemComponents[el][comp];
}

/**
 * @brief Returns true if the current object has Vertex Elements.
 * @return true if the current object has Vertex Elements.
 */
inline bool FileMeshInfo::hasVertices() const
{
	return hasElement(VERTEX);
}

/**
 * @brief Returns true if the current object has Vertex Coordinates.
 * @return true if the current object has Vertex Coordinates.
 */
inline bool FileMeshInfo::hasVertexCoords() const
{
	return hasPerElementComponent(VERTEX, COORD);
}

/**
 * @brief Returns true if the current object has Vertex Normals.
 * @return true if the current object has Vertex Normals.
 */
inline bool FileMeshInfo::hasVertexNormals() const
{
	return hasPerElementComponent(VERTEX, NORMAL);
}

/**
 * @brief Returns true if the current object has Vertex Colors.
 * @return true if the current object has Vertex Colors.
 */
inline bool FileMeshInfo::hasVertexColors() const
{
	return hasPerElementComponent(VERTEX, COLOR);
}

/**
 * @brief Returns true if the current object has Vertex Scalars.
 * @return true if the current object has Vertex Scalars.
 */
inline bool FileMeshInfo::hasVertexScalars() const
{
	return hasPerElementComponent(VERTEX, SCALAR);
}

/**
 * @brief Returns true if the current object has Vertex Texture Coordinates.
 * @return true if the current object has Vertex Texture Coordinates.
 */
inline bool FileMeshInfo::hasVertexTexCoords() const
{
	return hasPerElementComponent(VERTEX, TEXCOORD);
}

/**
 * @brief Returns true if the current object has Vertex Custom Components.
 * @return true if the current object has Vertex Custom Components.
 */
inline bool FileMeshInfo::hasVertexCustomComponents() const
{
	return hasPerElementComponent(VERTEX, CUSTOM_COMPONENTS);
}

/**
 * @brief Returns true if the current object has Face Elements.
 * @return true if the current object has Face Elements.
 */
inline bool FileMeshInfo::hasFaces() const
{
	return hasElement(FACE);
}

/**
 * @brief Returns true if the current object has per Face Vertex References.
 * @return true if the current object has per Face Vertex References.
 */
inline bool FileMeshInfo::hasFaceVRefs() const
{
	return hasPerElementComponent(FACE, VREFS);
}

inline bool FileMeshInfo::hasFaceNormals() const
{
	return hasPerElementComponent(FACE, NORMAL);
}

inline bool FileMeshInfo::hasFaceColors() const
{
	return hasPerElementComponent(FACE, COLOR);
}

inline bool FileMeshInfo::hasFaceScalars() const
{
	return hasPerElementComponent(FACE, SCALAR);
}

inline bool FileMeshInfo::hasFaceWedgeTexCoords() const
{
	return hasPerElementComponent(FACE, WEDGE_TEXCOORDS);
}

inline bool FileMeshInfo::hasFaceCustomComponents() const
{
	return hasPerElementComponent(FACE, CUSTOM_COMPONENTS);
}

/**
 * @brief Returns true if the current object has Edge Elements.
 * @return true if the current object has Edge Elements.
 */
inline bool FileMeshInfo::hasEdges() const
{
	return hasElement(EDGE);
}

inline bool FileMeshInfo::hasEdgeVRefs() const
{
	return hasPerElementComponent(EDGE, VREFS);
}

inline bool FileMeshInfo::hasEdgeColors() const
{
	return hasPerElementComponent(EDGE, COLOR);
}

inline bool FileMeshInfo::hasTextures() const
{
	return hasPerElementComponent(MESH, TEXTURES);
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

void FileMeshInfo::setElement(Element el, bool b)
{
	elements[el] = b;
}

void FileMeshInfo::setElementComponents(Element el, Component c, bool b, DataType t)
{
	elements[el] = b;
	perElemComponents[el][c] = b;
	if (b)
		perElemComponentsType(el, c) = t;
}

inline void FileMeshInfo::setVertices(bool b)
{
	setElement(VERTEX, b);
}

inline void FileMeshInfo::setVertexCoords(bool b, DataType t)
{
	setElementComponents(VERTEX, COORD, b, t);
}

inline void FileMeshInfo::setVertexNormals(bool b, DataType t)
{
	setElementComponents(VERTEX, NORMAL, b, t);
}

inline void FileMeshInfo::setVertexColors(bool b, DataType t)
{
	setElementComponents(VERTEX, COLOR, b, t);
}

inline void FileMeshInfo::setVertexScalars(bool b, DataType t)
{
	setElementComponents(VERTEX, SCALAR, b, t);
}

void FileMeshInfo::setVertexTexCoords(bool b, DataType t)
{
	setElementComponents(VERTEX, TEXCOORD, b, t);
}

inline void FileMeshInfo::setVertexCustomComponents(bool b)
{
	setElementComponents(VERTEX, CUSTOM_COMPONENTS, b, UNKNOWN);
}

inline void FileMeshInfo::setFaces(bool b)
{
	setElement(FACE, b);
}

inline void FileMeshInfo::setFaceVRefs(bool b)
{
	setElementComponents(FACE, VREFS, b, UNKNOWN);
}

inline void FileMeshInfo::setFaceNormals(bool b, DataType t)
{
	setElementComponents(FACE, NORMAL, b, t);
}

inline void FileMeshInfo::setFaceColors(bool b, DataType t)
{
	setElementComponents(FACE, COLOR, b, t);
}

inline void FileMeshInfo::setFaceScalars(bool b, DataType t)
{
	setElementComponents(FACE, SCALAR, b, t);
}

inline void FileMeshInfo::setFaceWedgeTexCoords(bool b, DataType t)
{
	setElementComponents(FACE, WEDGE_TEXCOORDS, b, t);
}

inline void FileMeshInfo::setFaceCustomComponents(bool b)
{
	setElementComponents(FACE, CUSTOM_COMPONENTS, b, UNKNOWN);
}

inline void FileMeshInfo::setEdges(bool b)
{
	setElement(EDGE, b);
}

inline void FileMeshInfo::setEdgeVRefs(bool b)
{
	setElementComponents(EDGE, VREFS, b, UNKNOWN);
}

inline void FileMeshInfo::setEdgeColors(bool b, DataType t)
{
	setElementComponents(EDGE, COLOR, b, t);
}

inline void FileMeshInfo::setTextures(bool b)
{
	setElementComponents(MESH, TEXTURES, b, UNKNOWN);
}

inline void FileMeshInfo::addElementCustomComponent(Element el, const std::string &name, DataType t)
{
	setElementComponents(el, CUSTOM_COMPONENTS, true, UNKNOWN);
	perElemCustomComponents[el].push_back(CustomComponent{name, t});
}

inline void FileMeshInfo::addVertexCustomComponent(const std::string& name, DataType t)
{
	addElementCustomComponent(VERTEX, name, t);
}

inline void FileMeshInfo::addFaceCustomComponent(const std::string &name, DataType t)
{
	addElementCustomComponent(FACE, name, t);
}

FileMeshInfo::DataType FileMeshInfo::elementComponentType(Element el, Component comp) const
{
	return perElemComponentsType(el, comp);
}

inline FileMeshInfo::DataType FileMeshInfo::vertexCoordsType() const
{
	return elementComponentType(VERTEX, COORD);
}

inline FileMeshInfo::DataType FileMeshInfo::vertexNormalsType() const
{
	return elementComponentType(VERTEX, NORMAL);
}

inline FileMeshInfo::DataType FileMeshInfo::vertexColorsType() const
{
	return elementComponentType(VERTEX, COLOR);
}

inline FileMeshInfo::DataType FileMeshInfo::vertexScalarsType() const
{
	return elementComponentType(VERTEX, SCALAR);
}

inline FileMeshInfo::DataType FileMeshInfo::vertexTexCoordsType() const
{
	return elementComponentType(VERTEX, TEXCOORD);
}

inline FileMeshInfo::DataType FileMeshInfo::faceNormalsType() const
{
	return elementComponentType(FACE, NORMAL);
}

inline FileMeshInfo::DataType FileMeshInfo::faceColorsType() const
{
	return elementComponentType(FACE, COLOR);
}

inline FileMeshInfo::DataType FileMeshInfo::faceScalarsType() const
{
	return elementComponentType(FACE, SCALAR);
}

inline FileMeshInfo::DataType FileMeshInfo::faceWedgeTexCoordsType() const
{
	return elementComponentType(FACE, WEDGE_TEXCOORDS);
}

inline FileMeshInfo::DataType FileMeshInfo::edgeColorsType() const
{
	return elementComponentType(EDGE, COLOR);
}

inline const std::vector<FileMeshInfo::CustomComponent>& FileMeshInfo::vertexCustomComponents() const
{
	return perElemCustomComponents[VERTEX];
}

inline const std::vector<FileMeshInfo::CustomComponent>& FileMeshInfo::faceCustomComponents() const
{
	return perElemCustomComponents[FACE];
}

/**
 * @brief Returns a FileMeshInfo object that is the intersection between this and `info`.
 *
 * The intersection is a FileMeshInfo object that has Elements/Components enable only if they are
 * enabled both in this object and in `info`. Types are imported from this FileMeshInfo.
 *
 * @param[in] info: The info object to compute the intersection with.
 * @return The intersection between this and `info`.
 */
inline FileMeshInfo FileMeshInfo::intersect(const FileMeshInfo& info) const
{
	FileMeshInfo res;
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

inline void FileMeshInfo::reset()
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
FileMeshInfo::DataType FileMeshInfo::getType()
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

FileMeshInfo::DataType FileMeshInfo::getType(std::type_index ti)
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
