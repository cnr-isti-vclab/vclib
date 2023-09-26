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

#ifndef VCL_MISC_MESH_INFO_H
#define VCL_MISC_MESH_INFO_H

#include <array>
#include <list>
#include <string>
#include <typeindex>
#include <vector>
#include <bitset>

#include <vclib/mesh/requirements.h>

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief A simple class that allows to store which elements and their
 * components have been imported/loaded or are going to be exported/saved on a
 * mesh file or some other data structure.
 *
 * For example, when loading a Mesh from a file, an object of this type is used
 * to know which Elements/Components have been loaded from the file, using the
 * getter functions:
 *
 * @code{.cpp}
 * MeshInfo info;
 * AMeshType m = vcl::load<AMeshType>("meshfile.ply", info);
 *
 * if (info.hasFaces()) { // the file had faces
 *     (info.hasFaceColors()) { // the file had face colors
 *         // ...
 *     }
 * }
 * @endcode
 *
 * When saving a Mesh to a file, an object of this type is used to tell which
 * Elements/Components save on the file and, when the file format supports it,
 * to choose the type used to store a specific component:
 *
 * @code{.cpp}
 * AMeshType m;
 * MeshInfo info(m); // compute the default MeshInfo object from the
 * Mesh
 *
 * info.setVertexCoords(true, MeshInfo::FLOAT); // force to store vertex
 * coords using floats info.setVertexColors(false); // do not store
 *
 * vcl::save(m, "meshfile.ply", info);
 * @endcode
 *
 * @ingroup load_save
 * @ingroup miscellaneous
 */
class MeshInfo
{
public:
	/**
	 * @brief Enum used to describe the type of the Mesh - by default, a mesh is
	 * considered polygonal
	 */
	enum MeshType { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH };

	/**
	 * @brief Enum used to describe the type of Elements that can be found in a
	 * file.
	 *
	 * @note: MESH is not an element, but it is used since some components can
	 * be stored per mesh.
	 */
	enum Element { VERTEX, FACE, EDGE, MESH, NUM_ELEMENTS };

	/**
	 * @brief Enum used to describe the type of Components that each Element can
	 * have.
	 */
	enum Component {
		COORD,
		VREFS,
		NORMAL,
		COLOR,
		QUALITY,
		TEXCOORD,
		WEDGE_TEXCOORDS,
		CUSTOM_COMPONENTS,
		TEXTURES,
		NUM_COMPONENTS
	};

	/**
	 * @brief Enum used to describe the type of Data stored in a component
	 */
	enum DataType {
		CHAR,
		UCHAR,
		SHORT,
		USHORT,
		INT,
		UINT,
		FLOAT,
		DOUBLE,
		UNKNOWN
	};

	/**
	 * @brief The CustomComponent struct is a simple structure that describes a
	 * custom component of an Element (or of the Mesh)
	 */
	struct CustomComponent {
		std::string name;
		DataType type;
		CustomComponent(std::string n, DataType t) : name(n), type(t) {}
	};

	MeshInfo();

	template<MeshConcept Mesh>
	MeshInfo(const Mesh& m);

	bool isTriangleMesh() const;
	bool isQuadMesh() const;
	bool isPolygonMesh() const;

	/*
	 * Getter Elements/Components functions: they are used mostly after the
	 * loading of a Mesh from a file, to know if Elements/Components have been
	 * loaded.
	 */

	bool hasElement(Element el) const;
	bool hasPerElementComponent(Element el, Component comp) const;

	bool hasVertices() const;
	bool hasVertexCoords() const;
	bool hasVertexNormals() const;
	bool hasVertexColors() const;
	bool hasVertexQuality() const;
	bool hasVertexTexCoords() const;
	bool hasVertexCustomComponents() const;
	bool hasFaces() const;
	bool hasFaceVRefs() const;
	bool hasFaceNormals() const;
	bool hasFaceColors() const;
	bool hasFaceQuality() const;
	bool hasFaceWedgeTexCoords() const;
	bool hasFaceCustomComponents() const;
	bool hasEdges() const;
	bool hasEdgeVRefs() const;
	bool hasEdgeColors() const;
	bool hasTextures() const;

	/*
	 * Setter functions: they are used by the load functions to tell which
	 * Elements/Components are loaded from a file, and they can be used by the
	 * user that wants to save in a file only a specific set of
	 * Elements/Components of a Mesh.
	 */

	void setTriangleMesh();
	void setQuadMesh();
	void setPolygonMesh();
	void setMeshType(MeshType t);

	void setElement(Element el, bool b = true);
	void setElementComponents(Element el, Component c, bool b, DataType t);

	void setVertices(bool b = true);
	void setVertexCoords(bool b = true, DataType t = DOUBLE);
	void setVertexNormals(bool b = true, DataType t = FLOAT);
	void setVertexColors(bool b = true, DataType t = UCHAR);
	void setVertexQuality(bool b = true, DataType t = DOUBLE);
	void setVertexTexCoords(bool b = true, DataType t = FLOAT);
	void setVertexCustomComponents(bool b = true);
	void setFaces(bool b = true);
	void setFaceVRefs(bool b = true);
	void setFaceNormals(bool b = true, DataType t = FLOAT);
	void setFaceColors(bool b = true, DataType t = UCHAR);
	void setFaceQuality(bool b = true, DataType t = DOUBLE);
	void setFaceWedgeTexCoords(bool b = true, DataType t = FLOAT);
	void setFaceCustomComponents(bool b = true);
	void setEdges(bool b = true);
	void setEdgeVRefs(bool b = true);
	void setEdgeColors(bool b = true, DataType t = UCHAR);
	void setTextures(bool b = true);

	void addElementCustomComponent(
		Element el, const std::string& name, DataType t);
	void clearElementCustomComponents(Element el);
	void addVertexCustomComponent(const std::string& name, DataType t);
	void clearVertexCustomComponents();
	void addFaceCustomComponent(const std::string& name, DataType t);
	void clearFaceCustomComponents();

	/*
	 * Getter Component type functions : they are used mostly by save functions
	 * to know the type that needs to use to save a given Component
	 */

	DataType elementComponentType(Element el, Component comp) const;

	DataType vertexCoordsType() const;
	DataType vertexNormalsType() const;
	DataType vertexColorsType() const;
	DataType vertexQualityType() const;
	DataType vertexTexCoordsType() const;
	DataType faceNormalsType() const;
	DataType faceColorsType() const;
	DataType faceQualityType() const;
	DataType faceWedgeTexCoordsType() const;
	DataType edgeColorsType() const;

	const std::vector<CustomComponent>& vertexCustomComponents() const;
	const std::vector<CustomComponent>& faceCustomComponents() const;

	MeshInfo intersect(const MeshInfo& i) const;

	void reset();

private:
	// Tell if elements are present in the file
	std::bitset<NUM_ELEMENTS> elements = {false};

	// Tell if per element components are present in the file
	std::array<std::bitset<NUM_COMPONENTS>, NUM_ELEMENTS> perElemComponents = {
		false};

	// Tell the data type for each component of each element
	Eigen::Matrix<DataType, NUM_ELEMENTS, NUM_COMPONENTS> perElemComponentsType;

	// Store name and type of per element custom components
	std::array<std::vector<CustomComponent>, NUM_ELEMENTS>
		perElemCustomComponents;

	// Mesh Type
	MeshType type = POLYGON_MESH;

	template<typename T>
	static DataType getType();

	static DataType getType(std::type_index ti);
};

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

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
		if (vcl::isPerVertexNormalAvailable(m))
			setVertexNormals(true, getType<typename Mesh::VertexType::NormalType::ScalarType>());
	if constexpr (vcl::HasPerVertexColor<Mesh>)
		if (vcl::isPerVertexColorAvailable(m))
			setVertexColors(true, UCHAR);
	if constexpr (vcl::HasPerVertexQuality<Mesh>)
		if (vcl::isPerVertexQualityAvailable(m))
			setVertexQuality(true, getType<typename Mesh::VertexType::QualityType>());
	if constexpr (vcl::HasPerVertexTexCoord<Mesh>)
		if (vcl::isPerVertexTexCoordAvailable(m))
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
			if (vcl::isPerFaceNormalAvailable(m))
				setFaceNormals(true, getType<typename Mesh::FaceType::NormalType::ScalarType>());
		if constexpr (vcl::HasPerFaceColor<Mesh>)
			if (vcl::isPerFaceColorAvailable(m))
				setFaceColors(true, UCHAR);
		if constexpr (vcl::HasPerFaceQuality<Mesh>)
			if (vcl::isPerFaceQualityAvailable(m))
				setFaceQuality(true, getType<typename Mesh::FaceType::QualityType>());
		if constexpr (vcl::HasPerFaceWedgeTexCoords<Mesh>)
			if (vcl::isPerFaceWedgeTexCoordsAvailable(m))
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
		//			if (vcl::isPerEdgeColorAvailable(m))
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
 * The intersection is a MeshInfo object that has Elements/Components enabled
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

#endif // VCL_MISC_MESH_INFO_H
