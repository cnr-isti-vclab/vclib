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

#ifndef VCL_IO_FILE_MESH_INFO_H
#define VCL_IO_FILE_MESH_INFO_H

#include <array>
#include <list>
#include <string>
#include <typeindex>
#include <vector>
#include <bitset>

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief A simple class that allows to store which components has been loaded or are going
 * to be saved on a mesh file.
 *
 * When loading a Mesh from a file, an object of this type is used to know which Elements/Components
 * have been loaded from the file, using the getter functions:
 *
 * @code{.cpp}
 * FileMeshInfo info;
 * AMeshType m = vcl::load<AMeshType>("meshfile.ply", info);
 *
 * if (info.hasFaces()) { // the file had faces
 *     (info.hasFaceColors()) { // the file had face colors
 *         // ...
 *     }
 * }
 * @endcode
 *
 * When saving a Mesh to a file, an object of this type is used to tell which Elements/Components
 * save on the file and, when the file format supports it, to choose the type used to store a
 * specific component:
 *
 * @code{.cpp}
 * AMeshType m;
 * FileMeshInfo info(m); // compute the default FileMeshInfo object from the Mesh
 *
 * info.setVertexCoords(true, FileMeshInfo::FLOAT); // force to store vertex coords using floats
 * info.setVertexColors(false); // do not store
 *
 * vcl::save(m, "meshfile.ply", info);
 * @endcode
 *
 * @ingroup load_save
 */
class FileMeshInfo
{
public:
	/**
	 * @brief Enum used to describe the type of the Mesh - by default, a mesh is considered
	 * polygonal
	 */
	enum MeshType { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH };

	/**
	 * @brief Enum used to describe the type of Elements that can be found in a file.
	 *
	 * @note: MESH is not an element, but it is used since some components can be stored per mesh.
	 */
	enum Element { VERTEX, FACE, EDGE, HALF_EDGE, MESH, NUM_ELEMENTS };

	/**
	 * @brief Enum used to describe the type of Components that each Element can have.
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
	enum DataType { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE, UNKNOWN };

	/**
	 * @brief The CustomComponent struct is a simple structure that describes a custom component
	 * of an Element (or of the Mesh)
	 */
	struct CustomComponent {
		std::string name;
		DataType type;
	};

	FileMeshInfo();

	template<MeshConcept Mesh>
	FileMeshInfo(const Mesh& m);

	bool isTriangleMesh() const;
	bool isQuadMesh() const;
	bool isPolygonMesh() const;

	/*
	 * Getter Elements/Components functions: they are used mostly after the loading of a Mesh from a
	 * file, to know if Elements/Components have been loaded.
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
	 * Setter functions: they are used by the load functions to tell which Elements/Components are
	 * loaded from a file, and they can be used by the user that wants to save in a file only a
	 * specific set of Elements/Components of a Mesh.
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

	void addElementCustomComponent(Element el, const std::string& name, DataType t);
	void clearElementCustomComponents(Element el);
	void addVertexCustomComponent(const std::string& name, DataType t);
	void clearVertexCustomComponents();
	void addFaceCustomComponent(const std::string& name, DataType t);
	void clearFaceCustomComponents();

	/*
	 * Getter Component type functions : they are used mostly by save functions to know the type
	 * that needs to use to save a given Component
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

	FileMeshInfo intersect(const FileMeshInfo& i) const;

	void reset();

private:
	// Tell if elements are present in the file
	std::bitset<NUM_ELEMENTS> elements = {false};

	// Tell if per element components are present in the file
	std::array<std::bitset<NUM_COMPONENTS>, NUM_ELEMENTS> perElemComponents = {false};

	// Tell the data type for each component of each element
	Eigen::Matrix<DataType, NUM_ELEMENTS, NUM_COMPONENTS> perElemComponentsType;

	// Store name and type of per element custom components
	std::array<std::vector<CustomComponent>, NUM_ELEMENTS> perElemCustomComponents;

	// Mesh Type
	MeshType type = POLYGON_MESH;

	template<typename T>
	static DataType getType();

	static DataType getType(std::type_index ti);
};

} // namespace vcl

#include "file_mesh_info.cpp"

#endif // VCL_IO_FILE_MESH_INFO_H
