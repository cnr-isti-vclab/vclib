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

#include "mesh.h"

#include <vclib/views/mesh.h>
#include <vclib/space/polygon.h>

namespace vcl {

/**
 * @brief Empty constructor, constructs an empty mesh.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>::Mesh()
{
	// Set to all element containers their parent mesh (this)
	updateAllParentMeshPointers();
}

/**
 * @brief Copy constructor of the Mesh. Will create a deep copy of the given input mesh,
 * taking care of copying everithing and then update all the pointers
 *
 * @param oth: the mesh from which constructo this Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>::Mesh(const Mesh<Args...>& oth) :
		Args(oth)... // call auto copy constructors for all the container elements and components
{
	// Set to all element containers their parent mesh (this)
	updateAllParentMeshPointers();

	// save base pointer of each container of the other mesh
	constexpr uint N_CONTAINERS = NumberOfTypes<typename Mesh<Args...>::Containers>::value;
	std::array<const void*, N_CONTAINERS> othBases = Mesh<Args...>::getContainerBases(oth);

	// update all the pointers contained on each container
	// use the base pointer of each container of oth to update all the pointers in this mesh
	// each pointer of oth that was copied in this mesh, will be updated computing its offset wrt
	// the base of oth, and then adding that offset to the new base pointer of this mesh
	(Mesh<Args...>::template updatePointersOfContainerType<Args>(*this, othBases), ...);
}

/**
 * @brief Move constructor, moves the given mesh into this one, without any other
 * resource acquisition.
 *
 * @param oth: the mesh that will be moved into this Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>::Mesh(Mesh<Args...>&& oth)
{
	swap(oth); // use copy ans swap idiom: this (empty) mesh is swapped with the input one
}

/**
 * @brief Clears all the Elements contained in the mesh.
 * @todo manage also other components
 */
template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::clear()
{
    (clearContainer<Args>(), ...);
}

template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::compact()
{
	(compactContainer<Args>(), ...);
}

/**
 * @brief The HasContainerOf struct sets the bool `value` to true if this Mesh has a container
 * of elements having the same Element ID of the template Element El.
 *
 * This means that this the only value checked is the ELEMENT_TYPE unsigned int exposed by the
 * Element, meaning that it does not check if the Elements of this mesh are exactly the same of
 * El.
 *
 * In other words, it returns true also if we pass an Element of another mesh that is of the
 * same ELEMENT_TYPE (both Vertices, Faces, ecc).
 *
 * Example of usage (Note: EdgeMesh has Vertices, but not Faces):
 *
 * @code{.cpp}
 * static_assert(vcl::EdgeMesh::hasContainerOf<vcl::TriMesh::Vertex>(),
 *					"EdgeMesh does not have Vertices");
 * static_assert(!vcl::EdgeMesh::hasContainerOf<vcl::TriMesh::Face>(),
 *					"EdgeMesh has Faces");
 * @endcode
 *
 * HasContainerOf sets its value to true when El is the TriMesh::Vertex, because EdgeMesh has
 * a Container of Vertices (Vertices of TriMesh and EdgeMesh are defined in different ways, but
 * they have the same ELEMENT_TYPE id).
 * HasContainerOf sets its value to false when El is the TriMesh::Face, because EdgeMesh does
 * not have a Container of Faces.
 */
template<typename... Args> requires HasVertices<Args...>
template<ElementConcept El>
constexpr bool Mesh<Args...>::hasContainerOf()
{
	return mesh::HasContainerOfPred<El, Mesh<Args...>>::value;
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
constexpr bool Mesh<Args...>::hasContainerOf()
{
	return mesh::HasContainerOfElementPred<EL_TYPE, Mesh<Args...>>::value;
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE, uint COMP_TYPE>
constexpr bool Mesh<Args...>::hasPerElementOptionalComponent()
{
	return mesh::HasPerElementOptionalComponent<Mesh<Args...>, EL_TYPE, COMP_TYPE>;
}

/**
 * @brief Enables all the OptionalComponents of this mesh according to the Components available
 * on the OtherMeshType m.
 *
 * This function is useful to call before importing data from another MeshType, to be sure that
 * all the available data contained in the MeshType mesh will be imported.
 *
 * This function:
 * - disables all the optional components that are not available in m
 * - enables all the optional components that are available in m (which can be both optional or not)
 *
 * Example of usage:
 *
 * @code{.cpp}
 * MeshType m1;
 * OtherMeshType m2;
 *
 * // do stuff on m2
 *
 * m1.enableSameOptionalComponentsOf(m2); // m1 enables all the available components of m2
 * m1.importFrom(m2); // m1 will import all the data contained in m2 that can be stored in m1
 * @endcode
 *
 * @param m
 */
template<typename... Args> requires HasVertices<Args...>
template<typename OtherMeshType>
void Mesh<Args...>::enableSameOptionalComponentsOf(const OtherMeshType& m)
{
	// enable all optional components of this Mesh depending on what's available in the
	// OtherMeshType

	(enableSameOptionalComponentsOf<Args>(m), ...);
}

/**
 * @brief Imports all the components that can be imported from another type of mesh.
 *
 * This function can be called from any Mesh type having all the Elements and Components that
 * implement the member function importFrom.
 *
 * Note that this function does not enable optional components that are disabled.
 * If you want to import all the possible data including also disabled components of this mesh, you
 * should call the function m1.enableSameOptionalComponentsOf(m2) before this function.
 *
 * @param[in] m: the mesh from which import all the data.
 */
template<typename... Args> requires HasVertices<Args...>
template<typename OtherMeshType>
void Mesh<Args...>::importFrom(const OtherMeshType& m)
{
	// This function will first:
	// Call, for each Container and Component of the mesh, its importFrom function.
	// In case of containers, it first creates the same number of elements in the container,
	// and then calls the importFrom function for each new element.
	// Pointers are not managed here, since they need additional parameters to be imported

	(Args::importFrom(m), ...);

	// Set to all element containers their parent mesh (this)
	updateAllParentMeshPointers();

	// after importing ordinary components, we need to convert the pointers between containers.
	// each container can import more than one pointer type, e.g.:
	// - VertexContainer could import vertex pointers (adjacent vertices), face pointers
	//   (adjacent faces), and so on;
	// - FaceContainer will always import vertex pointers, but could also import face pointers
	//   (adjacent faces), edge pointers (adjacent edges)...
	// for each container of this Mesh, we'll call the importPointers passing the container (Args)
	// as template parameter. This parameter will be used to call all the possible import functions
	// available (vertices, faces, edges, half edges)

	(importPointers<Args>(m), ...);

	if constexpr(mesh::HasFaceContainer<Mesh<Args...>> && mesh::HasFaceContainer<OtherMeshType>) {
		// Now I need to manage imports between different types of meshes (same type of meshes are
		// already managed from importFrom and importPointersFrom member functions).
		//
		// Generally speaking, Polygon meshes can import from any other type of mesh.
		// We need to take care when this mesh has static vertex pointers number in the face
		// container (VERTEX_NUMBER >= 3).
		//
		// The follwing case don't need to be managed:
		// - import polygon mesh from triangle mesh
		//
		// I can manage the following cases:
		// - import triangle mesh from polygon mesh: need triangulation
		//
		// I cannot manage the follwing cases:
		// - import static non-triangle mesh from polygon mesh or from a mesh with different
		//   VERTEX_NUMBER

		// in case of import from poly to triangle mesh, I need to manage
		// triangulation of polygons and create additional triangle faces for each of the
		// imported polygons. This function statically asserts that the import can be done.
		manageImportTriFromPoly(m);
	}
}

/**
 * @brief Swaps this mesh with the other input Mesh m2.
 * @param m2: the Mesh to swap with this Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::swap(Mesh& m2)
{
	Mesh<Args...>& m1 = *this;

	constexpr uint N_CONTAINERS = NumberOfTypes<typename Mesh<Args...>::Containers>::value;
	static_assert(N_CONTAINERS != 0);

	// container bases of each container for m1 and m2
	// we save the bases of the containers before swap
	std::array<const void*, N_CONTAINERS> m1Bases = Mesh<Args...>::getContainerBases(m1);
	std::array<const void*, N_CONTAINERS> m2Bases = Mesh<Args...>::getContainerBases(m2);

	// actual swap of all the containers and the components of the mesh
	// using pack expansion: swap will be called for each of the containers (or components!) that
	// compose the Mesh
	using std::swap;
	(swap((Args&) m1, (Args&) m2), ...);

	// Set to all elements their parent mesh
	m1.updateAllParentMeshPointers();
	m2.updateAllParentMeshPointers();

	// update all the pointers to m1 and m2: old base of m1 is now "old base" of m2, and viceversa
	(Mesh<Args...>::template updatePointersOfContainerType<Args>(m1, m2Bases), ...);
	(Mesh<Args...>::template updatePointersOfContainerType<Args>(m2, m1Bases), ...);
}

/**
 * @brief Assignment operator of the Mesh.
 * @param oth: the Mesh from which will create a copy and assign to this Mesh
 * @return a reference to this Mesh after the assignemnt.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>& Mesh<Args...>::operator=(Mesh<Args...> oth)
{
	swap(oth);
	return *this;
}

/**
 * @brief Returns the index of the given element in its Container of the Mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type El. Otherwise, a compiler
 * error will be triggered.
 *
 * @tparam El: The type of the Element, it must satisfy the ElementConcept.
 * @param e: a reference of an element of the Mesh.
 * @return the index of the given element.
 */
template<typename... Args> requires HasVertices<Args...>
template<ElementConcept El>
uint Mesh<Args...>::index(const El& e) const requires (hasContainerOf<El>())
{
	using Container = typename ContainerOf<El>::type;
	return Container::index(&e);
}

/**
 * @brief Returns the index of the given element in its Container of the Mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type El. Otherwise, a compiler
 * error will be triggered.
 *
 * @tparam El: The type of the Element, it must satisfy the ElementConcept.
 * @param e: a pointer to an element of the Mesh.
 * @return the index of the given element.
 */
template<typename... Args> requires HasVertices<Args...>
template<ElementConcept El>
uint Mesh<Args...>::index(const El* e) const requires (hasContainerOf<El>())
{
	using Container = typename ContainerOf<El>::type;
	return Container::index(e);
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
const auto& Mesh<Args...>::element(uint i) const requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::element(i);
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
auto& Mesh<Args...>::element(uint i) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::element(i);
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
uint Mesh<Args...>::elementNumber() const requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementNumber();
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
uint Mesh<Args...>::elementContainerSize() const requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementContainerSize();
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
uint Mesh<Args...>::deletedElementNumber() const requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::deletedElementNumber();
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
uint Mesh<Args...>::addElement() requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::addElement();
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
uint Mesh<Args...>::addElements(uint n) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::addElements(n); // add the number elements
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
void Mesh<Args...>::reserveElements(uint n) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::reserveElements(n);
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE>
void Mesh<Args...>::compactElements() requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::compactElements();
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE, uint COMP_TYPE>
bool Mesh<Args...>::isPerElementComponentEnabled() const
	requires (hasPerElementOptionalComponent<EL_TYPE, COMP_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::template isOptionalComponentEnabled<COMP_TYPE>();
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE, uint COMP_TYPE>
void Mesh<Args...>::enablePerElementComponent()
	requires (hasPerElementOptionalComponent<EL_TYPE, COMP_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::template enableOptionalComponent<COMP_TYPE>();
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE, uint COMP_TYPE>
void Mesh<Args...>::disablePerElementComponent()
	requires (hasPerElementOptionalComponent<EL_TYPE, COMP_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::template disableOptionalComponent<COMP_TYPE>();
}

/*********************
 * Protected Members *
 *********************/

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::compactContainer()
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		if (Cont::elementNumber() != Cont::elementContainerSize()) {
			Cont::compactElements();
		}
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::clearContainer()
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::clearElements();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<ElementConcept Element>
void Mesh<Args...>::updateAllPointers(const Element* oldBase, const Element* newBase)
{
	if (oldBase != nullptr && oldBase != newBase)
		(updatePointers<Args>(oldBase, newBase), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename Element>
void Mesh<Args...>::updatePointers(
	const Element* oldBase,
	const Element* newBase)
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::updatePointers(oldBase, newBase);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<ElementConcept Element>
void Mesh<Args...>::updateAllPointersAfterCompact(
	const Element* base,
	const std::vector<int>& newIndices)
{
	(updatePointersAfterCompact<Args>(base, newIndices), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename Element>
void Mesh<Args...>::updatePointersAfterCompact(
	const Element*          base,
	const std::vector<int>& newIndices)
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::updatePointersAfterCompact(base, newIndices);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename OtherMeshType>
void Mesh<Args...>::enableSameOptionalComponentsOf(const OtherMeshType& m)
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::enableOptionalComponentsOf(m);
	}
}

template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::updateAllParentMeshPointers()
{
	(setParentMeshPointers<Args>(), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::setParentMeshPointers()
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::setParentMeshPointers(this);
	}
}

/**
 * This function will import, for a given container of this mesh that is passed as a template
 * parameter Cont, all the pointers of all the elements from the other mesh m.
 */
template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename OthMesh>
void Mesh<Args...>::importPointers(const OthMesh &m)
{
	// will loop again on Args. Args will be the element pointers imported on Cont
	(importPointersOfElement<Cont, Args>(m), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename ElemCont, typename OthMesh>
void Mesh<Args...>::importPointersOfElement(const OthMesh& m)
{
	// if Cont and ElemCont are containers (could be mesh components)
	if constexpr(mesh::ElementContainerConcept<Cont> && mesh::ElementContainerConcept<ElemCont>) {
		// import in Cont the ElemCont pointers from m
		Cont::importPointersFrom(m, ElemCont::vec.data());
	}
}

/**
 * @brief This function manages the case where we try to import into a TriMesh a PolyMesh
 * Faces have been already imported, but without vertex pointers and other components that
 * depend on the number of vertices (e.g. wedges)
 */
template<typename... Args> requires HasVertices<Args...>
template<typename OthMesh>
void Mesh<Args...>::manageImportTriFromPoly(const OthMesh &m)
{
	using VertexType = typename Mesh<Args...>::VertexType;
	using MVertexType = typename OthMesh::VertexType;
	using MCoordType = typename MVertexType::CoordType;
	using FaceType = typename Mesh<Args...>::FaceType;
	using MFaceType = typename OthMesh::FaceType;

	using VertexContainer = typename Mesh<Args...>::VertexContainer;
	using FaceContainer   = typename Mesh<Args...>::FaceContainer;

	// if this is not a triangle mesh nor a polygon mesh (meaning that we can't control the
	// number of vertex pointers in this mesh), and this mesh does not have the same
	// number of vertex pointers of the other, it means that we don't know how to convert
	// these type of meshes (e.g. we don't know how to convert a polygon mesh into a quad
	// mesh, or convert a quad mesh into a pentagonal mesh...)
	static_assert(
		!(FaceType::VERTEX_NUMBER != 3 && FaceType::VERTEX_NUMBER > 0 &&
		  FaceType::VERTEX_NUMBER != MFaceType::VERTEX_NUMBER),
		"Cannot import from that type of Mesh. Don't know how to convert faces.");

	// we need to manage conversion from poly or faces with cardinality > 3 (e.g. quads) to
	// triangle meshes. In this case, we triangulate the polygon using the earcut algorithm.
	if constexpr (
		FaceType::VERTEX_NUMBER == 3 &&
		(MFaceType::VERTEX_NUMBER > 3 || MFaceType::VERTEX_NUMBER < 0)) {

		VertexType* base = VertexContainer::vec.data();
		const MVertexType* mvbase = &m.vertex(0);

		for (const MFaceType& mf : m.faces()) {
			// if the current face has the same number of vertices of this faces (3), then
			// the vertex pointers have been correctly imported from the import pointers
			// function. The import pointers function does nothing when importing from a face
			// with at least 4 vertices
			if (mf.vertexNumber() != FaceType::VERTEX_NUMBER) {
				// triangulate mf; the first triangle of the triangulation will be
				// this->face(m.index(mf));
				// the other triangles will be added at the end of the container
				std::vector<uint> tris =
					Polygon<MCoordType>::earCut(mf.vertices() | vcl::views::coords);
				FaceType& f = FaceContainer::face(m.index(mf));
				importTriPointersHelper(f, mf, base, mvbase, tris, 0);

				// number of other faces to add
				uint nf = tris.size() / 3 - 1;
				uint fid = FaceContainer::addElements(nf);

				uint i = 3; // index that cycles into tris
				for (; fid < FaceContainer::faceContainerSize(); ++fid) {
					FaceType& f = FaceContainer::face(fid);
					importTriPointersHelper(f, mf, base, mvbase, tris, i);
					i+=3;
				}
			}
		}
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename FaceType, typename MFaceType, typename VertexType, typename MVertexType>
void Mesh<Args...>::importTriPointersHelper(
	FaceType&                f,
	const MFaceType&         mf,
	VertexType*              base,
	const MVertexType*       mvbase,
	const std::vector<uint>& tris,
	uint                     basetri)
{
	f.importFrom(mf); // import all the components from mf
	for (uint i = basetri, j = 0; i < basetri+3; i++, j++) {
		f.vertex(j) = base + (mf.vertex(tris[i]) - mvbase);

		// wedge colors
		if constexpr(face::HasWedgeColors<FaceType> && face::HasWedgeColors<MFaceType>) {
			if (comp::isWedgeColorsEnabledOn(f) && comp::isWedgeColorsEnabledOn(mf)) {
				f.wedgeColor(j) = mf.wedgeColor(tris[i]);
			}
		}

		// wedge texcoords
		if constexpr(face::HasWedgeTexCoords<FaceType> && face::HasWedgeTexCoords<MFaceType>) {
			if (comp::isWedgeTexCoordsEnabledOn(f) && comp::isWedgeTexCoordsEnabledOn(mf)) {
				f.wedgeTexCoord(j) =
					mf.wedgeTexCoord(tris[i])
						.template cast<typename FaceType::WedgeTexCoordType::ScalarType>();
			}
		}
	}
}

/**
 * This function sets the Ith position of the array bases, where I is an index of a container in
 * a TypeWrapper of containers
 *
 * In the Ith position will be placed the base pointer of the vector of the elements contained
 * in the container Cont.
 */
template<typename... Args> requires HasVertices<Args...>
template<uint I, typename Cont, typename Array, typename... A>
void Mesh<Args...>::setContainerBase(const Mesh<A...>& m, Array& bases)
{
	// since this function is called using pack expansion, it means that Cont could be a mesh
	// component and not a cointainer. We check if Cont is a container
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		static_assert(I >= 0 && I != UINT_NULL);
		bases[I] = m.Cont::vec.data();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename... A>
auto Mesh<Args...>::getContainerBases(const Mesh<A...>& m)
{
	using Containers = typename Mesh<A...>::Containers;

	// the number of containers in Mesh<A...>
	constexpr uint N_CONTAINERS = NumberOfTypes<Containers>::value;
	// each element of this array will contain the base pointer of the vector of elements contained
	// in each container of Mesh<A...>
	std::array<const void*, N_CONTAINERS> bases;

	// for each container/component of Mesh<A...>, we set call the function that sets
	// the base of the container in its index
	(setContainerBase<IndexInTypes<A, Containers>::value, A>(m, bases), ...);

	return bases;
}

/**
 * This function is called for each container of the mesh.
 *
 * In general, for each container, we need to update all the pointers contained in it,
 * that may of any element of the mesh (example: in the VertexContainer there could be
 * Vertex pointers, but also Face or Edge pointers).
 *
 * Here in this function, we loop into the containers of the Mesh m using pack expansion, and
 * we use the Cont type to choose which pointers type we are updating.
 *
 * bases contains the old bases (the ones of the other mesh) for each container.
 */
template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename Array, typename... A>
void Mesh<Args...>::updatePointersOfContainerType(Mesh<A...>& m, const Array& bases)
{
	// since this function is called using pack expansion, it means that Cont could be a mesh
	// component and not a cointainer. We check if Cont is a container
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		// The element type contained in the container
		// We need it to get back the actual type of the element from the old bases
		using ElType = typename Cont::ElementType;

		using Containers = typename Mesh<A...>::Containers;
		constexpr uint I = IndexInTypes<Cont, Containers>::value;
		static_assert(I >= 0 && I != UINT_NULL);

		// for each Container A in m, we update the pointers of ElType.
		// old base is contained in the array bases, the new base is the base of the container
		(m.template updatePointers<A>((const ElType*)bases[I], m.Cont::vec.data()), ...);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<uint EL_TYPE, typename T>
uint Mesh<Args...>::elementIndex(const T* el) const
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;
	using ElType = typename Cont::ElementType;
	return index(static_cast<const ElType*>(el));
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
auto& Mesh<Args...>::customComponents()
{
	using ElCont = typename ContainerOf<El>::type;

	return ElCont::ccVecMap;
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
const auto& Mesh<Args...>::customComponents() const
{
	using ElCont = typename ContainerOf<El>::type;

	return ElCont::ccVecMap;
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
auto& Mesh<Args...>::verticalComponents()
{
	using ElCont = typename ContainerOf<El>::type;

	return ElCont::vcVecTuple;
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
const auto& Mesh<Args...>::verticalComponents() const
{
	using ElCont = typename ContainerOf<El>::type;

	return ElCont::vcVecTuple;
}

} // namespace vcl
