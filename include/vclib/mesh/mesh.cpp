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

namespace vcl {

/**
 * @brief Empty constructor, constructs an empty mesh.
 */
template<typename... Args>
Mesh<Args...>::Mesh()
{
	// Set to all element containers their parent mesh (this)
	updateAllParentMeshPointers();
}

/**
 * @brief Copy constructor of the Mesh. Will create a deep copy of the given
 * input mesh, taking care of copying everithing and then update all the
 * pointers
 *
 * @param oth: the mesh from which constructo this Mesh.
 */
template<typename... Args>
Mesh<Args...>::Mesh(const Mesh<Args...>& oth) :
		Args(oth)... // call auto copy constructors for all the container
					 // elements and components
{
	// Set to all element containers their parent mesh (this)
	updateAllParentMeshPointers();

	// for each container of oth, save its base pointer
	// will need it to update all the pointers of this mesh
	constexpr uint N_CONTAINERS =
		NumberOfTypes<typename Mesh<Args...>::Containers>::value;
	std::array<const void*, N_CONTAINERS> othBases =
		Mesh<Args...>::getContainerBases(oth);

	// update all the pointers contained on each container
	// use the base pointer of each container of oth to update all the pointers
	// in this mesh each pointer of oth that was copied in this mesh, will be
	// updated computing its offset wrt the base of oth, and then adding that
	// offset to the new base pointer of this mesh
	(updatePointersOfContainerType<Args>(*this, othBases), ...);
}

/**
 * @brief Move constructor, moves the given mesh into this one, without any
 * other resource acquisition.
 *
 * @param oth: the mesh that will be moved into this Mesh.
 */
template<typename... Args>
Mesh<Args...>::Mesh(Mesh<Args...>&& oth)
{
	swap(oth); // use copy and swap idiom: this (empty) mesh is swapped with the
			   // input one
}

/**
 * @brief Returns true if this mesh is compact, meaning that all its containers
 * have no deleted elements (size == element number).
 *
 * @return true if this mesh is compact, false otherwise.
 */
template<typename... Args>
bool Mesh<Args...>::isCompact() const
{
	bool isCompact = true;
	((isCompact &= isContainerCompact<Args>()), ...);
	return isCompact;
}

/**
 * @brief Returns true if this Mesh has a container of elements having the same
 * Element ID of the template Element El.
 *
 * This means that this the only value checked is the ELEMENT_TYPE unsigned int
 * exposed by the Element, meaning that it does not check if the Elements of
 * this mesh are exactly the same of El.
 *
 * In other words, it returns true also if we pass an Element of another mesh
 * that is of the same ELEMENT_TYPE (both Vertices, Faces, ecc).
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
 * @tparam El: the Element type to check.
 * @return true if this Mesh has a container of elements having the same
 * Element ID of the template Element El.
 */
template<typename... Args>
template<ElementConcept El>
constexpr bool Mesh<Args...>::hasContainerOf()
{
	return mesh::HasContainerOfPred<El, Mesh<Args...>>::value;
}

/**
 * @brief Returns true if this Mesh has a container of elements having the same
 * Element ID of the template EL_TYPE ID.
 *
 * Example of usage (Note: EdgeMesh has Vertices, but not Faces):
 *
 * @code{.cpp}
 * static_assert(vcl::EdgeMesh::hasContainerOf<vcl::VERTEX>(),
 *					"EdgeMesh does not have Vertices");
 * static_assert(!vcl::EdgeMesh::hasContainerOf<vcl::FACE>(),
 *					"EdgeMesh has Faces");
 * @endcode
 *
 * @tparam EL_TYPE: the Element type ID to check.
 * @return true if this Mesh has a container of elements having the same
 * Element ID of the template EL_TYPE ID.
 */
template<typename... Args>
template<uint EL_TYPE>
constexpr bool Mesh<Args...>::hasContainerOf()
{
	return mesh::HasContainerOfElementPred<EL_TYPE, Mesh<Args...>>::value;
}

/**
 * @brief Returns true if this Mesh has a container of elements having the same
 * Element ID of the template EL_TYPE ID and the Element of that container has
 * a Component having the same Component ID of the template COMP_TYPE ID.
 *
 * Example of usage (Note: TriMesh has per Vertex TexCoords, but not adj edges):
 *
 * @code{.cpp}
 * static_assert(vcl::TriMesh::hasPerElementComponent<
 *         vcl::VERTEX, vcl::TEX_COORDS>(),
 *         "TriMesh does not have per Vertex TexCoords");
 *
 * static_assert(!vcl::TriMesh::hasPerElementComponent<
 *        vcl::VERTEX, vcl::ADJ_EDGES>(),
 *        "TriMesh has per Vertex Adjacent Edges");
 * @endcode
 */
template<typename... Args>
template<uint EL_TYPE, uint COMP_TYPE>
constexpr bool Mesh<Args...>::hasPerElementComponent()
{
	return mesh::HasPerElementComponent<Mesh<Args...>, EL_TYPE, COMP_TYPE>;
}

/**
 * @brief Returns true if this Mesh has a container of elements having the same
 * Element ID of the template EL_TYPE ID and the Element of that container has
 * an Optional Component having the same Component ID of the template COMP_TYPE
 * ID.
 *
 * Example of usage (Note: TriMesh has per Vertex optional TexCoords, and
 * non-optional Normals):
 *
 * @code{.cpp}
 * static_assert(vcl::TriMesh::hasPerElementOptionalComponent<
 *         vcl::VERTEX, vcl::TEX_COORDS>(),
 *         "TriMesh does not have per Vertex Optional TexCoords");
 *
 * static_assert(!vcl::TriMesh::hasPerElementOptionalComponent<
 *        vcl::VERTEX, vcl::NORMALS>(),
 *        "TriMesh has per Vertex Optional Normals");
 * @endcode
 */
template<typename... Args>
template<uint EL_TYPE, uint COMP_TYPE>
constexpr bool Mesh<Args...>::hasPerElementOptionalComponent()
{
	return mesh::
		HasPerElementOptionalComponent<Mesh<Args...>, EL_TYPE, COMP_TYPE>;
}

/**
 * @brief Clears all the Elements contained in the mesh.
 * @todo manage also other components
 */
template<typename... Args>
void Mesh<Args...>::clear()
{
	(clearContainer<Args>(), ...);
}

/**
 * @brief Compacts all the containers of the mesh.
 *
 * Removes all the deleted elements from each container, compacting the
 * the containers and then updating automatically all the pointers.
 */
template<typename... Args>
void Mesh<Args...>::compact()
{
	(compactContainer<Args>(), ...);
}

/**
 * @brief Enables all the optional components of the elements of the containers
 * if the mesh.
 */
template<typename... Args>
void Mesh<Args...>::enableAllOptionalComponents()
{
	(enableAllOptionalComponentsInContainer<Args>(), ...);
}

/**
 * @brief Disables all the optional components of the elements of the containers
 * if the mesh.
 */
template<typename... Args>
void Mesh<Args...>::disableAllOptionalComponents()
{
	(disableAllOptionalComponentsInContainer<Args>(), ...);
}

/**
 * @brief Enables all the OptionalComponents of this mesh according to the
 * Components available on the OtherMeshType m.
 *
 * This function is useful to call before importing data from another MeshType,
 * to be sure that all the available data contained in the MeshType mesh will be
 * imported.
 *
 * This function:
 * - disables all the optional components that are not available in m
 * - enables all the optional components that are available in m (which can be
 * both optional or not)
 *
 * Example of usage:
 *
 * @code{.cpp}
 * MeshType m1;
 * OtherMeshType m2;
 *
 * // do stuff on m2
 *
 * m1.enableSameOptionalComponentsOf(m2); // m1 enables all the available
 *                                        // components of m2
 * m1.importFrom(m2); // m1 will import all the data contained
 *                    // in m2 that can be stored in m1
 * @endcode
 *
 * @param m
 */
template<typename... Args>
template<typename OtherMeshType>
void Mesh<Args...>::enableSameOptionalComponentsOf(const OtherMeshType& m)
{
	// enable all optional components of this Mesh depending on what's available
	// in the OtherMeshType

	(enableSameOptionalComponentsOf<Args>(m), ...);
}

/**
 * @brief Imports all the components that can be imported from another type of
 * mesh.
 *
 * This function can be called from any Mesh type having all the Elements and
 * Components that implement the member function importFrom.
 *
 * Note that this function does not enable optional components that are
 * disabled. If you want to import all the possible data including also disabled
 * components of this mesh, you should call the function
 * m1.enableSameOptionalComponentsOf(m2) before this function.
 *
 * @param[in] m: the mesh from which import all the data.
 */
template<typename... Args>
template<typename OtherMeshType>
void Mesh<Args...>::importFrom(const OtherMeshType& m)
{
	// This function will first:
	// Call, for each Container and Component of the mesh, its importFrom
	// function. In case of containers, it first creates the same number of
	// elements in the container, and then calls the importFrom function for
	// each new element. Pointers are not managed here, since they need
	// additional parameters to be imported

	(Args::importFrom(m), ...);

	// Set to all element containers their parent mesh (this)
	updateAllParentMeshPointers();

	// after importing ordinary components, we need to convert the pointers
	// between containers. each container can import more than one pointer type,
	// e.g.:
	// - VertexContainer could import vertex pointers (adjacent vertices), face
	//   pointers (adjacent faces), and so on;
	// - FaceContainer will always import vertex pointers, but could also import
	//   face pointers (adjacent faces), edge pointers (adjacent edges)...
	// for each container of this Mesh, we'll call the importPointers passing
	// the container (Args) as template parameter. This parameter will be used
	// to call all the possible import functions available (vertices, faces,
	// edges, half edges)

	(importPointers<Args>(m), ...);

	if constexpr (mesh::HasFaceContainer<Mesh<Args...>>) {
		// Now I need to manage imports between different types of meshes (same
		// type of meshes are already managed from importFrom and
		// importPointersFrom member functions).
		//
		// Generally speaking, Polygon meshes can import from any other type of
		// mesh. We need to take care when this mesh has static vertex pointers
		// number in the face container (VERTEX_NUMBER >= 3).
		//
		// The follwing case don't need to be managed:
		// - import polygon mesh from triangle mesh
		//
		// I can manage the following cases:
		// - import triangle mesh from polygon mesh: need triangulation
		//
		// I cannot manage the follwing cases:
		// - import static non-triangle mesh from polygon mesh or from a mesh
		//   with different VERTEX_NUMBER

		// in case of import from poly to triangle mesh, I need to manage
		// triangulation of polygons and create additional triangle faces for
		// each of the imported polygons. This function statically asserts that
		// the import can be done.
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		FaceContainer::manageImportTriFromPoly(m);
	}
}

/**
 * @brief Swaps this mesh with the other input Mesh m2.
 * @param m2: the Mesh to swap with this Mesh.
 */
template<typename... Args>
void Mesh<Args...>::swap(Mesh& m2)
{
	Mesh<Args...>& m1 = *this;

	constexpr uint N_CONTAINERS =
		NumberOfTypes<typename Mesh<Args...>::Containers>::value;
	static_assert(N_CONTAINERS != 0);

	// container bases of each container for m1 and m2
	// we save the bases of the containers before swap
	std::array<const void*, N_CONTAINERS> m1Bases =
		Mesh<Args...>::getContainerBases(m1);
	std::array<const void*, N_CONTAINERS> m2Bases =
		Mesh<Args...>::getContainerBases(m2);

	// actual swap of all the containers and the components of the mesh
	// using pack expansion: swap will be called for each of the containers (or
	// components!) that compose the Mesh
	using std::swap;
	(swap((Args&) m1, (Args&) m2), ...);

	// Set to all elements their parent mesh
	m1.updateAllParentMeshPointers();
	m2.updateAllParentMeshPointers();

	// update all the pointers to m1 and m2: old base of m1 is now "old base" of
	// m2, and viceversa
	(updatePointersOfContainerType<Args>(m1, m2Bases), ...);
	(updatePointersOfContainerType<Args>(m2, m1Bases), ...);
}

/**
 * @brief Assignment operator of the Mesh.
 * @param oth: the Mesh from which will create a copy and assign to this Mesh
 * @return a reference to this Mesh after the assignemnt.
 */
template<typename... Args>
Mesh<Args...>& Mesh<Args...>::operator=(Mesh<Args...> oth)
{
	swap(oth);
	return *this;
}

/**
 * @brief Returns the index of the given element in its Container of the Mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type El.
 * Otherwise, a compiler error will be triggered.
 *
 * @tparam El: The type of the Element, it must satisfy the ElementConcept.
 * @param e: a reference of an element of the Mesh.
 * @return the index of the given element.
 */
template<typename... Args>
template<ElementConcept El>
uint Mesh<Args...>::index(const El& e) const requires (hasContainerOf<El>())
{
	using Container = typename ContainerOf<El>::type;
	return Container::index(&e);
}

/**
 * @brief Returns the index of the given element in its Container of the Mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type El.
 * Otherwise, a compiler error will be triggered.
 *
 * @tparam El: The type of the Element, it must satisfy the ElementConcept.
 * @param e: a pointer to an element of the Mesh.
 * @return the index of the given element.
 */
template<typename... Args>
template<ElementConcept El>
uint Mesh<Args...>::index(const El* e) const requires (hasContainerOf<El>())
{
	using Container = typename ContainerOf<El>::type;
	return Container::index(e);
}

/**
 * @brief Returns the element of the given type at the given index inside its
 * container of this mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * The function does not perform any check on the index. If the index is out of
 * range, the behaviour is undefined.
 *
 * @tparam EL_TYPE: the type ID of the element to return.
 * @return the element of the given type ID at the given index inside its
 * container of this mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
const auto& Mesh<Args...>::element(uint i) const
	requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::element(i);
}

/**
 * @brief Returns the element of the given type at the given index inside its
 * container of this mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * The function does not perform any check on the index. If the index is out of
 * range, the behaviour is undefined.
 *
 * @tparam EL_TYPE: the type ID of the element to return.
 * @return the element of the given type ID at the given index inside its
 * container of this mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
auto& Mesh<Args...>::element(uint i) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::element(i);
}

/**
 * @brief Returns the number of elements of the given type in this mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @return the number of elements of the given type in this mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
uint Mesh<Args...>::number() const requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementNumber();
}

/**
 * @brief Returns the size of the container of elements of the given type in
 * this mesh.
 *
 * The size of a container may be different from the number of elements, if the
 * container has some deleted elements.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @return the size of the container of elements of the given type in this mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
uint Mesh<Args...>::containerSize() const requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementContainerSize();
}

/**
 * @brief Returns the number of deleted elements of the given type in this mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @return the number of deleted elements of the given type in this mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
uint Mesh<Args...>::deletedNumber() const requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::deletedElementNumber();
}

/**
 * @brief Adds a new element of the given type into its container, returning the
 * index of the added element in its container.
 *
 * If the call of this function will cause a reallocation of the container, the
 * function will automatically take care of updating all the pointers to the
 * elements stored in all the containers of the Mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @return the index of the added element in its container.
 */
template<typename... Args>
template<uint EL_TYPE>
uint Mesh<Args...>::add() requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::addElement();
}

/**
 * @brief Adds `n` new elements of the given type into its container, returning
 * the index of the first added element in its container.
 *
 * If the call of this function will cause a reallocation of the container, the
 * function will automatically take care of updating all the pointers to the
 * elements stored in all the containers of the Mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] n: the number of elements to add.
 * @return the index of the first added element in its container.
 */
template<typename... Args>
template<uint EL_TYPE>
uint Mesh<Args...>::add(uint n) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::addElements(n); // add the number elements
}

/**
 * @brief Clears the container of EL_TYPE elements the Mesh, deleting all the
 * Elements.
 *
 * The contained elements are actually removed from the container, not only
 * marked as deleted. Therefore, the container will have size 0
 * (`mesh.containerSize<EL_TYPE>() == 0`) after the call of this function.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @note This function does not cause a reallocation of the container.
 *
 * @warning Any pointer to EL_TYPE elements in the Mesh will be left unchanged,
 * and therefore will point to invalid elements. This means that, if you have a
 * pointer to a EL_TYPE element and you call this function, you will have a
 * dangling pointer.
 *
 * @tparam EL_TYPE: the type ID of the element.
 */
template<typename... Args>
template<uint EL_TYPE>
void Mesh<Args...>::clearElements() requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::clearElements();
}

/**
 * @brief Resizes the Element container to contain `n` Elements of type EL_TYPE.
 *
 * If the new size is greater than the old one, new Elements are added to the
 * container, and a reallocation may happen. If the new size is smaller than the
 * old one, the container will keep its first non-deleted `n` Elements, and
 * the remaining Elements are marked as deleted.
 *
 * If the call of this function will cause a reallocation of the container, the
 * function will automatically take care of updating all the pointers to the
 * elements stored in all the containers of the Mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @warning The given size `n` is relative to the number of non-deleted
 * Elements, not to the size of the Element container. For example, if you
 * have a mesh with 10 Faces and faceContainerSize() == 20, calling
 * resize<FACE>(5) will not cause a reallocation of the container, but will
 * mark as deleted the least 5 non-deleted Faces of the container. In the
 * same scenario, calling resize<FACE>(15) will result in a Face
 * container having 15 new Faces and faceContainerSize() == 25.
 * The latest 5 Faces will be the newly added.
 *
 * @warning Any pointer to deleted Elements in the Mesh will be left
 * unchanged, and therefore will point to invalid Elements. This means that
 * if you call this member function with a lower number of Elements, you'll
 * need to manually manage the pointers to the deleted Elements.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] n: the new size of the container in the mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
void Mesh<Args...>::resize(uint n) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::resizeElements(n);
}

/**
 * @brief Reserves a number of elements of the given type in its container. The
 * function does not add any element to the container, but it just reserves a
 * number of elements that can be added without causing a reallocation of the
 * container.
 *
 * This is useful when you know (or you have an idea) of how much elements are
 * going to add in a mesh, and you want to avoid multiple (expansive)
 * reallocations of the container during the addition of the elements.
 *
 * If the call of this function will cause a reallocation of the container, the
 * function will automatically take care of updating all the pointers to the
 * elements stored in all the containers of the Mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] n: the new capacity of the container in the mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
void Mesh<Args...>::reserve(uint n) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::reserveElements(n);
}

/**
 * @brief Marks as deleted the element at the given index from its container,
 * deduced from the template index EL_TYPE.
 *
 * The function does not remove the element from the container, and therefore it
 * does not cause reallocation or compacting of the container. The element will
 * be removed from the container only when the container will be compacted.
 *
 * The complexity of this function is O(1).
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] i: the index of the element to delete.
 */
template<typename... Args>
template<uint EL_TYPE>
void Mesh<Args...>::deleteElement(uint i) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::deleteElement(i);
}

/**
 * @brief Marks as deleted the given element from its container.
 *
 * The function does not remove the element from the container, and therefore it
 * does not cause reallocation or compacting of the container. The element will
 * be removed from the container only when the container will be compacted.
 *
 * The complexity of this function is O(1).
 *
 * The function requires that the Mesh has a Container of Elements of type
 * of the argument. Otherwise, a compiler error will be triggered.
 *
 * @tparam El: the type of the element to delete. It must satisfy the
 * ElementConcept.
 * @param[in] e: a pointer to the element to delete.
 */
template<typename... Args>
template<ElementConcept El>
void Mesh<Args...>::deleteElement(const El* e) const
	requires (hasContainerOf<El>())
{
	using Cont = typename ContainerOf<El>::type;
	return Cont::deleteElement(e);
}

/**
 * @brief Marks as deleted the given element from its container.
 *
 * The function does not remove the element from the container, and therefore it
 * does not cause reallocation or compacting of the container. The element will
 * be removed from the container only when the container will be compacted.
 *
 * The complexity of this function is O(1).
 *
 * The function requires that the Mesh has a Container of Elements of type
 * of the argument. Otherwise, a compiler error will be triggered.
 *
 * @tparam El: the type of the element to delete. It must satisfy the
 * ElementConcept.
 * @param[in] e: the element to delete.
 */
template<typename... Args>
template<ElementConcept El>
void Mesh<Args...>::deleteElement(const El& e) const
	requires (hasContainerOf<El>())
{
	using Cont = typename ContainerOf<El>::type;
	return Cont::deleteElement(&e);
}

/**
 * @brief Returns a vector that tells, for each element of the container of
 * EL_TYPE in the mesh, the new index of the element after the container has
 * been compacted. For each deleted element, its position will be set to
 * UINT_NULL.
 *
 * This function is useful when you need to know the indices of the elements
 * that they would have in a compact container, without considering the deleted
 * ones.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @return a vector containing, for each element index, the new index if the
 * container would be compacted.
 */
template<typename... Args>
template<uint EL_TYPE>
std::vector<uint> Mesh<Args...>::conpactIndices() const
	requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementConpactIndices();
}

/**
 * @brief Updates all the indices and pointers of the elements of the container
 * of EL_TYPE in the mesh, according to the mapping stored in the newIndices
 * vector, that tells for each old element index, the new index of the element
 * in the same container (or UINT_NULL if the element must be left as
 * unreferenced - useful when a vertex is deleted).
 *
 * This function is useful when you move some vertices (e.g. it is automatically
 * called after every compacting of a container), and you want to update the
 * indices/pointers of the elements stored in all the containers of the mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @note This function *does not change the position of the elements in their
 * container*. It just updates the indices/pointers of the elements stored in
 * their or other containers. This function should be called after the elements
 * have been actually moved in their container.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] newIndices: a vector that tells, for each old element index, the
 * new element index. If the old element must be left as unreferenced (setting
 * `nullptr` to the pointers), the value of the vector must be UINT_NULL.
 */
template<typename... Args>
template<uint EL_TYPE>
void Mesh<Args...>::updateIndices(const std::vector<uint>& newIndices)
	requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::updateElementIndices(newIndices);
}

/**
 * @brief Returns an iterator to the begining of the container of the elements
 * of type EL_TYPE in the mesh.
 *
 * The iterator is automatically initialized to jump deleted elements in the
 * container. You can change this option by calling this member function with
 * the `jumpDeleted` parameter set to `false`.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] jumpDeleted: if `true`, the iterator will be initialized to jump
 * deleted elements in the container.
 * @return an iterator to the begining of the container of the elements of type
 * EL_TYPE in the mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
auto Mesh<Args...>::begin(bool jumpDeleted) requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container of the elements of
 * type EL_TYPE in the mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @return an iterator to the end of the container of the elements of type
 * EL_TYPE in the mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
auto Mesh<Args...>::end() requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementEnd();
}

/**
 * @brief Returns a const iterator to the begining of the container of the
 * elements of type EL_TYPE in the mesh.
 *
 * The iterator is automatically initialized to jump deleted elements in the
 * container. You can change this option by calling this member function with
 * the `jumpDeleted` parameter set to `false`.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] jumpDeleted: if `true`, the iterator will be initialized to jump
 * deleted elements in the container.
 * @return a const iterator to the begining of the container of the elements of
 * type EL_TYPE in the mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
auto Mesh<Args...>::begin(bool jumpDeleted) const
	requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container of the elements
 * of type EL_TYPE in the mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @return a const iterator to the end of the container of the elements of type
 * EL_TYPE in the mesh.
 */
template<typename... Args>
template<uint EL_TYPE>
auto Mesh<Args...>::end() const requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elementEnd();
}

/**
 * Returns a lightweight view object that stores the begin and end iterators of
 * the container of the elements of type EL_TYPE in the mesh. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (auto& el : mesh.elements<VERTEX>()) {
 *     // Do something with el
 * }
 * @endcode
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] jumpDeleted: if `true`, the view will be initialized to jump
 * deleted elements in the container.
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over elements.
 */
template<typename... Args>
template<uint EL_TYPE>
auto Mesh<Args...>::elements(bool jumpDeleted)
	requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elements(jumpDeleted);
}

/**
 * Returns a lightweight const view object that stores the begin and end
 * const iterators of the container of the elements of type EL_TYPE in the mesh.
 * The view object exposes the iterators trough the `begin()` and `end()` member
 * functions, and therefore the returned object can be used in range-based for
 * loops:
 *
 * @code{.cpp}
 * for (const auto& el : mesh.elements<VERTEX>()) {
 *     // Do something read-only with el
 * }
 * @endcode
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE. Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @param[in] jumpDeleted: if `true`, the view will be initialized to jump
 * deleted elements in the container.
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over elements.
 */
template<typename... Args>
template<uint EL_TYPE>
auto Mesh<Args...>::elements(bool jumpDeleted) const
	requires (hasContainerOf<EL_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::elements(jumpDeleted);
}

/**
 * @brief Returns `true` if optional Component `COMP_TYPE` is enabled for
 * elements of type `EL_TYPE` in the mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE, and that the Element has a optional component of type COMP_TYPE.
 * Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @tparam COMP_TYPE: the type ID of the component.
 * @return `true` if optional Component `COMP_TYPE` is enabled for elements of
 * type `EL_TYPE` in the mesh.
 */
template<typename... Args>
template<uint EL_TYPE, uint COMP_TYPE>
bool Mesh<Args...>::isPerElementComponentEnabled() const
	requires (hasPerElementOptionalComponent<EL_TYPE, COMP_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	return Cont::template isOptionalComponentEnabled<COMP_TYPE>();
}

/**
 * @brief Enables the optional Component `COMP_TYPE` for elements of type
 * `EL_TYPE` in the mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE, and that the Element has a optional component of type COMP_TYPE.
 * Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @tparam COMP_TYPE: the type ID of the component.
 */
template<typename... Args>
template<uint EL_TYPE, uint COMP_TYPE>
void Mesh<Args...>::enablePerElementComponent()
	requires (hasPerElementOptionalComponent<EL_TYPE, COMP_TYPE>())
{
	using Cont = typename ContainerOfElement<EL_TYPE>::type;

	Cont::template enableOptionalComponent<COMP_TYPE>();
}

/**
 * @brief Disables the optional Component `COMP_TYPE` for elements of type
 * `EL_TYPE` in the mesh.
 *
 * The function requires that the Mesh has a Container of Elements of type
 * EL_TYPE, and that the Element has a optional component of type COMP_TYPE.
 * Otherwise, a compiler error will be triggered.
 *
 * @tparam EL_TYPE: the type ID of the element.
 * @tparam COMP_TYPE: the type ID of the component.
 */
template<typename... Args>
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

template<typename... Args>
template<typename Cont>
bool Mesh<Args...>::isContainerCompact() const
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		return Cont::elementNumber() == Cont::elementContainerSize();
	}
	else {
		return true; // does not count as a container
	}
}

/**
 * This function will compact the Cont container of this mesh (if Cont is
 * actually a container).
 *
 * This function is made to be called trough pack expansion:
 * @code{.cpp}
 * (compactContainer<Args>(), ...);
 * @endcode
 */
template<typename... Args>
template<typename Cont>
void Mesh<Args...>::compactContainer()
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		if (Cont::elementNumber() != Cont::elementContainerSize()) {
			Cont::compactElements();
		}
	}
}

/**
 * This function will enable all the optional components of the element of the
 * Cont container of this mesh (if Cont is actually a container).
 *
 * This function is made to be called trough pack expansion:
 * @code{.cpp}
 * (enableAllOptionalComponentsInContainer<Args>(), ...);
 * @endcode
 */
template<typename... Args>
template<typename Cont>
void Mesh<Args...>::enableAllOptionalComponentsInContainer()
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		Cont::enableAllOptionalComponents();
	}
}

/**
 * This function will disable all the optional components of the element of the
 * Cont container of this mesh (if Cont is actually a container).
 *
 * This function is made to be called trough pack expansion:
 * @code{.cpp}
 * (disableAllOptionalComponentsInContainer<Args>(), ...);
 * @endcode
 */
template<typename... Args>
template<typename Cont>
void Mesh<Args...>::disableAllOptionalComponentsInContainer()
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		Cont::disableAllOptionalComponents();
	}
}

/**
 * This function will clear the Cont container of this mesh (if Cont is
 * actually a container).
 *
 * This function is made to be called trough pack expansion:
 * @code{.cpp}
 * (clearContainer<Args>(), ...);
 * @endcode
 */
template<typename... Args>
template<typename Cont>
void Mesh<Args...>::clearContainer()
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		Cont::clearElements();
	}
}

template<typename... Args>
template<ElementConcept Element>
void Mesh<Args...>::updateAllPointers(
	const Element* oldBase,
	const Element* newBase)
{
	if (oldBase != nullptr && oldBase != newBase)
		(updatePointers<Args>(oldBase, newBase), ...);
}

template<typename... Args>
template<typename Cont, typename Element>
void Mesh<Args...>::updatePointers(
	const Element* oldBase,
	const Element* newBase)
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		Cont::updatePointers(oldBase, newBase);
	}
}

template<typename... Args>
template<ElementConcept Element>
void Mesh<Args...>::updateAllPointers(
	const Element*           base,
	const std::vector<uint>& newIndices)
{
	(updatePointers<Args>(base, newIndices), ...);
}

template<typename... Args>
template<typename Cont, typename Element>
void Mesh<Args...>::updatePointers(
	const Element*           base,
	const std::vector<uint>& newIndices)
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		Cont::updatePointers(base, newIndices);
	}
}

template<typename... Args>
template<typename Cont, typename OtherMeshType>
void Mesh<Args...>::enableSameOptionalComponentsOf(const OtherMeshType& m)
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		Cont::enableOptionalComponentsOf(m);
	}
}

template<typename... Args>
void Mesh<Args...>::updateAllParentMeshPointers()
{
	(setParentMeshPointers<Args>(), ...);
}

template<typename... Args>
template<typename Cont>
void Mesh<Args...>::setParentMeshPointers()
{
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		Cont::setParentMeshPointers(this);
	}
}

/**
 * This function will import, for a given container of this mesh that is passed
 * as a template parameter Cont, all the pointers of all the elements from the
 * other mesh m.
 */
template<typename... Args>
template<typename Cont, typename OthMesh>
void Mesh<Args...>::importPointers(const OthMesh& m)
{
	// will loop again on Args. Args will be the element pointers imported on
	// Cont
	(importPointersOfElement<Cont, Args>(m), ...);
}

template<typename... Args>
template<typename Cont, typename ElemCont, typename OthMesh>
void Mesh<Args...>::importPointersOfElement(const OthMesh& m)
{
	// if Cont and ElemCont are containers (could be mesh components)
	if constexpr (
		mesh::ElementContainerConcept<Cont> &&
		mesh::ElementContainerConcept<ElemCont>)
	{
		// import in Cont the ElemCont pointers from m
		Cont::importPointersFrom(m, ElemCont::vec.data());
	}
}

/**
 * This function sets the Ith position of the array bases, where I is an index
 * of a container in a TypeWrapper of containers
 *
 * In the Ith position will be placed the base pointer of the vector of the
 * elements contained in the container Cont.
 */
template<typename... Args>
template<uint I, typename Cont, typename Array, typename... A>
void Mesh<Args...>::setContainerBase(const Mesh<A...>& m, Array& bases)
{
	// since this function is called using pack expansion, it means that Cont
	// could be a mesh component and not a cointainer. We check if Cont is a
	// container
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		static_assert(I >= 0 && I != UINT_NULL);
		bases[I] = m.Cont::vec.data();
	}
}

template<typename... Args>
template<typename... A>
auto Mesh<Args...>::getContainerBases(const Mesh<A...>& m)
{
	using Containers = typename Mesh<A...>::Containers;

	// the number of containers in Mesh<A...>
	constexpr uint N_CONTAINERS = NumberOfTypes<Containers>::value;
	// each element of this array will contain the base pointer of the vector of
	// elements contained in each container of Mesh<A...>
	std::array<const void*, N_CONTAINERS> bases;

	// for each container/component of Mesh<A...>, we set call the function that
	// sets the base of the container in its index
	(setContainerBase<IndexInTypes<A, Containers>::value, A>(m, bases), ...);

	return bases;
}

/**
 * This function *is static*, and is generally called for each container of the
 * mesh, that is the template argument Cont.
 *
 * In general, for each container, we need to update all the pointers contained
 * in it, that may of any element of the mesh (example: in the VertexContainer
 * there could be Vertex pointers, but also Face or Edge pointers).
 *
 * Here in this function, we loop into the containers of the Mesh m using pack
 * expansion, and we use the Cont type to choose which pointers type we are
 * updating.
 *
 * bases contains the old bases (the ones of the other mesh) for each container.
 */
template<typename... Args>
template<typename Cont, typename Array, typename... A>
void Mesh<Args...>::updatePointersOfContainerType(
	Mesh<A...>&  m,
	const Array& bases)
{
	// since this function is called using pack expansion, it means that Cont
	// could be a mesh component and not a cointainer. We check if Cont is a
	// container
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		// The element type contained in the container
		// We need it to get back the actual type of the element from the old
		// bases
		using ElType = typename Cont::ElementType;

		using Containers = typename Mesh<A...>::Containers;
		constexpr uint I = IndexInTypes<Cont, Containers>::value;
		static_assert(I >= 0 && I != UINT_NULL);

		// for each Container A in m, we update the pointers of ElType.
		// old base is contained in the array bases, the new base is the base of
		// the container
		(m.template updatePointers<A>(
			 (const ElType*) bases[I], m.Cont::vec.data()),
		 ...);
	}
}

template<typename... Args>
template<uint EL_TYPE, typename T>
uint Mesh<Args...>::elementIndex(const T* el) const
{
	using Cont   = typename ContainerOfElement<EL_TYPE>::type;
	using ElType = typename Cont::ElementType;
	return index(static_cast<const ElType*>(el));
}

template<typename... Args>
template<typename El>
auto& Mesh<Args...>::customComponents()
{
	using ElCont = typename ContainerOf<El>::type;

	return ElCont::ccVecMap;
}

template<typename... Args>
template<typename El>
const auto& Mesh<Args...>::customComponents() const
{
	using ElCont = typename ContainerOf<El>::type;

	return ElCont::ccVecMap;
}

template<typename... Args>
template<typename El>
auto& Mesh<Args...>::verticalComponents()
{
	using ElCont = typename ContainerOf<El>::type;

	return ElCont::vcVecTuple;
}

template<typename... Args>
template<typename El>
const auto& Mesh<Args...>::verticalComponents() const
{
	using ElCont = typename ContainerOf<El>::type;

	return ElCont::vcVecTuple;
}

} // namespace vcl
