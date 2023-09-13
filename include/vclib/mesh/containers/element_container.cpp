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

#include "element_container.h"

#include <vclib/concepts/mesh/components.h>

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Elements.
 */
template<ElementConcept T>
ElementContainer<T>::ElementContainer()
{
}

/**
 * @brief Returns a const reference of the element at the i-th position in the
 * Element Container of the Mesh, which will be the element having index = i.
 *
 * This function does not perform any sanity check: if i is less than
 * elementContainerSize(), this function will return a valid Element reference
 * (note that the Element may have been flagged as deleted).
 *
 * @param[in] i: the index of the element that will be returned.
 */
template<ElementConcept T>
const T& ElementContainer<T>::element(uint i) const
{
	return vec[i];
}

/**
 * @brief Returns a reference of the element at the i-th position in the Element
 * Container of the Mesh, which will be the element having index = i.
 *
 * This function does not perform any sanity check: if i is less than
 * elementContainerSize(), this function will return a valid Element reference
 * (note that the Element may have been flagged as deleted).
 *
 * @param[in] i: the index of the element that will be returned.
 */
template<ElementConcept T>
T& ElementContainer<T>::element(uint i)
{
	return vec[i];
}

/**
 * @brief Returns the number of **non-deleted** elements contained in the Element container of the
 * Mesh.
 *
 * If elementNumber() != elementContainerSize(), it means that there are some elements that are
 * flagged as deleted.
 *
 * @return The number of non-deleted elements of the Mesh.
 */
template<ElementConcept T>
uint ElementContainer<T>::elementNumber() const
{
	return en;
}

/**
 * @brief Returns the number of elements (also deleted) contained in the Element container of the
 * Mesh.
 *
 * If elementNumber() != elementContainerSize(), it means that there are some elements that are
 * flagged as deleted.
 *
 * @return The number of all the elements contained in the Mesh.
 */
template<ElementConcept T>
uint ElementContainer<T>::elementContainerSize() const
{
	return vec.size();
}

/**
 * @brief Returns the number of deleted elements in the Element container, that is
 * elementContainerSize() - elementNumber().
 *
 * @return The number of deleted elements in the container.
 */
template<ElementConcept T>
uint ElementContainer<T>::deletedElementNumber() const
{
	return elementContainerSize() - elementNumber();
}

template<ElementConcept T>
uint ElementContainer<T>::addElement()
{
	vcVecTuple.resize(vec.size() + 1);
	if constexpr (comp::HasCustomComponents<T>)
		ccVecMap.resize(vec.size() + 1);

	T* oldB = vec.data();
	vec.emplace_back();
	T* newB = vec.data();
	en++;

	vec.back().setParentMesh(parentMesh);
	vec.back().initVerticalComponents();

	if (oldB != newB) {
		setParentMeshPointers(parentMesh);
		parentMesh->updateAllPointers(oldB, newB);
	}

	return vec.size() - 1;
}

/**
 * @brief Adds size elements to the Element Container.
 *
 * Returns the id of the first added element.
 *
 * @param size
 * @return the id of the first added element.
 */
template<ElementConcept T>
uint ElementContainer<T>::addElements(uint size)
{
	vcVecTuple.resize(vec.size() + size);
	if constexpr (comp::HasCustomComponents<T>)
		ccVecMap.resize(vec.size() + size);

	uint baseId = vec.size();
	T*   oldB   = vec.data();
	vec.resize(vec.size() + size);
	T* newB = vec.data();
	en += size;

	for (uint i = baseId; i < vec.size(); ++i) {
		vec[i].setParentMesh(parentMesh);
		vec[i].initVerticalComponents();
	}

	if (oldB != newB) {
		setParentMeshPointers(parentMesh);
		parentMesh->updateAllPointers(oldB, newB);
	}

	return baseId;
}

template<ElementConcept T>
void ElementContainer<T>::clearElements()
{
	vec.clear();
	en = 0;

	// clear vertical and custom components

	vcVecTuple.clear();
	if constexpr (comp::HasCustomComponents<T>)
		ccVecMap.clear();
}

/**
 * @brief Resizes the element container to contain `size` elements.
 *
 * If the new size is greater than the old one, new elements are added to the
 * container, and a reallocation may happen. If the new size is smaller than the
 * old one, the container will keep its first non-deleted `size` elements, and
 * the remaining elements are marked as deleted.
 *
 * If the call of this function will cause a reallocation of the Element
 * container, the function will automatically take care of updating all the
 * Element pointers contained in the Mesh.
 *
 * @warning The given `size` is relative to the number of non-deleted elements,
 * not to the size of the element container. For example, if you have a mesh
 * with 10 elements and elementContainerSize() == 20, calling resizeElements(5)
 * will not cause a reallocation of the container, but will mark as deleted the
 * least 5 non-deleted elements of the container. In the same scenario, calling
 * resizeElements(15) will result in a element container having 15 new elements
 * and elementContainerSize() == 25. The latest 5 elements will be the newly
 * added.
 *
 * @warning Any pointer to deleted elements in the Mesh will be left unchanged,
 * and therefore will point to invalid elements. This means that if you call
 * this member function with a lower number of elements, you'll need to manually
 * manage the pointers to the deleted elements.
 *
 * @param[in] size: the new size of the Element container.
 */
template<ElementConcept T>
void ElementContainer<T>::resizeElements(uint size)
{
	if (size > en) {
		addElements(size - en);
	}
	else if (size < en) {
		uint nToDelete = en - size;
		for (uint i = vec.size() - 1; nToDelete > 0; --i) {
			if (!vec[i].isDeleted()) {
				deleteElement(i);
			}
		}
	}
}

template<ElementConcept T>
void ElementContainer<T>::reserveElements(uint size)
{
	T* oldB = vec.data();
	vec.reserve(size);
	T* newB = vec.data();

	vcVecTuple.reserve(size);
	if constexpr (comp::HasCustomComponents<T>)
		ccVecMap.reserve(size);

	if (oldB != newB) {
		setParentMeshPointers(parentMesh);
		parentMesh->updateAllPointers(oldB, newB);
	}
}

/**
 * @brief Compacts the element container, keeping only the non-deleted elements.
 *
 * @return a vector that tells, for each old element index, the new index of the element. Will
 * contain UINT_NULL if the element has been deleted.
 */
template<ElementConcept T>
std::vector<uint> ElementContainer<T>::compactElements()
{
	std::vector<uint> newIndices = elementCompactIndices();
	if (elementNumber() != elementContainerSize()) {
		compactVector(vec, newIndices);

		vcVecTuple.compact(newIndices);
		if constexpr (comp::HasCustomComponents<T>)
			ccVecMap.compact(newIndices);

		updateElementIndices(newIndices);
	}
	return newIndices;
}

/**
 * @brief Marks as deleted the element with the given id.
 *
 * This member function does not perform any reallocation of the elements: the
 * deleted elements will stay in the Element Container, but will be marked as
 * deleted.
 *
 * Deleted elements are automatically jumped by the iterators provided by the
 * Element Container.
 *
 * @warning If there were pointers to the deleted element in this or other
 * containers, they will not be updated.
 *
 * @param[in] i: the id of the element that will be marked as deleted.
 */
template<ElementConcept T>
void ElementContainer<T>::deleteElement(uint i)
{
	vec[i].deletedBit() = true;
	--en;
}

/**
 * @brief Marks as deleted the given element, before asserting that the element
 * belongs to this container.
 *
 * This member function does not perform any reallocation of the elements: the
 * deleted elements will stay in the Element Container, but will be marked as
 * deleted.
 *
 * Deleted elements are automatically jumped by the iterators provided by the
 * Element Container.
 *
 * @warning If there were pointers to the deleted element in this or other
 * containers, they will not be updated.
 *
 * @param[in] e: the pointer of the element that will be marked as deleted.
 */
template<ElementConcept T>
void ElementContainer<T>::deleteElement(const T* e)
{
	deleteElement(index(e));
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the element with the given
 * index.
 *
 * Complexity: O(n), with n the number of elements in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a element of the container.
 * @return The index that the element with index i would have if this container would be compact.
 */
template<ElementConcept T>
uint ElementContainer<T>::elementIndexIfCompact(uint i) const
{
	if (vec.size() == en)
		return i;
	else {
		uint cnt = 0;
		for (uint ii = 0; ii < i; ii++) {
			if (!vec[ii].deleted())
				++cnt;
		}
		return cnt;
	}
}

/**
 * @brief Returns a vector that tells, for each actual element index, the new index that the element
 * would have in a compacted container. For each deleted element index, the value of the vector will
 * be UINT_NULL.
 *
 * This is useful if you need to know the indices of the elements that they would have in a
 * compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each element index, its index if the container would be compact.
 */
template<ElementConcept T>
std::vector<uint> ElementContainer<T>::elementCompactIndices() const
{
	std::vector<uint> newIndices(vec.size());
	uint              k = 0;
	for (uint i = 0; i < vec.size(); ++i) {
		if (!vec[i].deleted()) {
			newIndices[i] = k;
			k++;
		}
		else {
			newIndices[i] = UINT_NULL;
		}
	}
	return newIndices;
}

/**
 * @brief Updates all the indices and pointers of the elements of this container that are stored in
 * any container of the mesh, according to the mapping stored in the newIndices vector, that tells
 * for each old element index, the new element index.
 *
 * This function is useful when some elements of this container have been deleted, and you want to
 * update the indices/pointers stored in all the containers of the mesh accordingly.
 *
 * E.g.: if this is a vertex container, this function will update the indices and pointers of the
 * vertices stored in all the continers of the mesh (e.g. the face container that stores vertex
 * pointers, or the vertex container itself that stores adjacent vertices) according to the mapping
 * stored in the newIndices vector.
 *
 * Supposing you deleted a set of vertices, you can give to this function the vector telling, for each
 * of the old vertex indices, the new vertex index (or UINT_NULL if you want to leave unreferences that
 * vertices). This function will update all the pointers stored in the mesh containers accordingly.
 *
 * @note This function *does not change the position of the elements in this container*. It just updates
 * the indices/pointers of the elements stored in this or other containers.
 *
 * @param[in] newIndices: a vector that tells, for each old element index, the new element index. If the
 * old element must be left as unreferenced (setting `nullptr` to the pointers), the value of the vector
 * must be UINT_NULL.
 */
template<ElementConcept T>
void ElementContainer<T>::updateElementIndices(const std::vector<uint>& newIndices)
{
	T* base = vec.data();

	parentMesh->updateAllPointers(base, newIndices);
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted elements of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted elements.
 * @return An iterator the the first element of the container.
 */
template<ElementConcept T>
auto ElementContainer<T>::elementBegin(bool jumpDeleted) -> ElementIterator
{
	auto it = vec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted elements, and the first element is deleted, we need
		// to move forward until we find the first non-deleted element
		while (it != vec.end() && it->deleted()) {
			++it;
		}
	}
	return ElementIterator(it, vec, jumpDeleted && vec.size() != en);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<ElementConcept T>
auto ElementContainer<T>::elementEnd() -> ElementIterator
{
	return ElementIterator(vec.end(), vec);
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted elements of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted elements.
 * @return A const iterator the the first element of the container.
 */
template<ElementConcept T>
auto ElementContainer<T>::elementBegin(bool jumpDeleted) const -> ConstElementIterator
{
	auto it = vec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted elements, and the first element is deleted, we need
		// to move forward until we find the first non-deleted element
		while (it != vec.end() && it->deleted()) {
			++it;
		}
	}
	return ConstElementIterator(it, vec, jumpDeleted && vec.size() != en);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<ElementConcept T>
auto ElementContainer<T>::elementEnd() const -> ConstElementIterator
{
	return ConstElementIterator(vec.end(), vec);
}

/**
 * @brief Returns a view object that allows to iterate over the elements of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the elements using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (Element& f : m.elements()){
 *     // do something with this element
 * }
 * @endcode
 *
 * The iterator used to iterate over elements is automatically initialized to jump deleted elements of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted elements.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<ElementConcept T>
auto ElementContainer<T>::elements(bool jumpDeleted) -> View<ElementIterator>
{
	return vcl::View(elementBegin(jumpDeleted && vec.size() != en), elementEnd());
}

/**
 * @brief Returns a view object that allows to iterate over the elements of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the elements using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (const Element& f : m.elements()){
 *     // do something with this element
 * }
 * @endcode
 *
 * The iterator used to iterate over elements is automatically initialized to jump deleted elements of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted elements.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<ElementConcept T>
auto ElementContainer<T>::elements(bool jumpDeleted) const -> View<ConstElementIterator>
{
	return vcl::View(elementBegin(jumpDeleted && vec.size() != en), elementEnd());
}

template<ElementConcept T>
void ElementContainer<T>::enableAllOptionalComponents()
{
	vcVecTuple.enableAllOptionalComponents();
}

template<ElementConcept T>
void ElementContainer<T>::disableAllOptionalComponents()
{
	vcVecTuple.disableAllOptionalComponents();
}

template<ElementConcept T>
template<typename C>
bool ElementContainer<T>::isOptionalComponentEnabled() const
{
	return vcVecTuple.template isComponentEnabled<C>();
}

template<ElementConcept T>
template<uint COMP_TYPE>
bool ElementContainer<T>::isOptionalComponentEnabled() const
{
	return vcVecTuple.template isComponentEnabled<COMP_TYPE>();
}

template<ElementConcept T>
template<typename C>
void ElementContainer<T>::enableOptionalComponent()
{
	vcVecTuple.template enableComponent<C>();
	// first call init on all the just enabled components
	if constexpr (comp::HasInitMemberFunction<C>) {
		for (auto& e : elements()) {
			e.C::init();
		}
	}
	// then resize the component containers with tied size to vertex number
	if constexpr (comp::IsTiedToVertexNumber<C>) {
		static const int N = T::VERTEX_NUMBER;
		if constexpr (N < 0) {
			for (auto& e : elements()) {
				e.C::resize(e.vertexNumber());
			}
		}
	}
}

template<ElementConcept T>
template<uint COMP_TYPE>
void ElementContainer<T>::enableOptionalComponent()
{
	using C = comp::ComponentOfType<COMP_TYPE, typename T::Components>;
	enableOptionalComponent<C>();
}

template<ElementConcept T>
template<typename C>
void ElementContainer<T>::disableOptionalComponent()
{
	vcVecTuple.template disableComponent<C>();
}

template<ElementConcept T>
template<uint COMP_TYPE>
void ElementContainer<T>::disableOptionalComponent()
{
	vcVecTuple.template disableComponent<COMP_TYPE>();
}

template<ElementConcept T>
bool ElementContainer<T>::hasElemCustomComponent(const std::string& name) const
	requires comp::HasCustomComponents<T>
{
	return ccVecMap.componentExists(name);
}

template<ElementConcept T>
std::vector<std::string> ElementContainer<T>::elemCustomComponentNames() const
	requires comp::HasCustomComponents<T>
{
	return ccVecMap.allComponentNames();
}

template<ElementConcept T>
template<typename K>
bool ElementContainer<T>::isElemCustomComponentOfType(const std::string& name) const
	requires comp::HasCustomComponents<T>
{
	return ccVecMap.template isComponentOfType<K>(name);
}

template<ElementConcept T>
std::type_index ElementContainer<T>::elemComponentType(const std::string &name) const
	requires comp::HasCustomComponents<T>
{
	return ccVecMap.componentType(name);
}

template<ElementConcept T>
template<typename K>
std::vector<std::string> ElementContainer<T>::elemCustomComponentNamesOfType() const
	requires comp::HasCustomComponents<T>
{
	return ccVecMap.template allComponentNamesOfType<K>();
}

template<ElementConcept T>
template<typename K>
void ElementContainer<T>::addElemCustomComponent(const std::string& name)
	requires comp::HasCustomComponents<T>
{
	ccVecMap.template addNewComponent<K>(name, elementContainerSize());
}

template<ElementConcept T>
void ElementContainer<T>::deleteElemCustomComponent(const std::string& name)
	requires comp::HasCustomComponents<T>
{
	ccVecMap.deleteComponent(name);
}

template<ElementConcept T>
template<typename K>
CustomComponentVectorHandle<K> ElementContainer<T>::customComponentVectorHandle(
	const std::string& name) requires comp::HasCustomComponents<T>
{
	std::vector<std::any>& cc = ccVecMap.template componentVector<K>(name);
	CustomComponentVectorHandle<K> v(cc);
	return v;
}

template<ElementConcept T>
template<typename K>
ConstCustomComponentVectorHandle<K> ElementContainer<T>::customComponentVectorHandle(
	const std::string& name) const requires comp::HasCustomComponents<T>
{
	const std::vector<std::any>& cc = ccVecMap.template componentVector<K>(name);
	ConstCustomComponentVectorHandle<K> v(cc);
	return cc;
}

template<ElementConcept T>
inline uint ElementContainer<T>::index(const T* e) const
{
	assert(!vec.empty() && e >= vec.data() && e <= &vec.back());
	return e - vec.data();
}

template<ElementConcept T>
void ElementContainer<T>::setParentMeshPointers(void* pm)
{
	parentMesh = static_cast<ParentMeshType*>(pm);
	for (auto& e : elements(false)) {
		e.setParentMesh(pm);
	}
}

template<ElementConcept T>
template<typename Element>
void ElementContainer<T>::updatePointers(
	const Element* oldBase,
	const Element* newBase)
{
	using Comps = typename T::Components;

	updatePointersOnComponents(oldBase, newBase, Comps());
}

template<ElementConcept T>
template<typename Element>
void ElementContainer<T>::updatePointers(
	const Element*          base,
	const std::vector<uint>& newIndices)
{
	using Comps = typename T::Components;

	updatePointersOnComponents(base, newIndices, Comps());
}

template<ElementConcept T>
template<typename OtherMesh>
void ElementContainer<T>::enableOptionalComponentsOf(const OtherMesh &m)
{
	if constexpr (OtherMesh::template hasContainerOf<T>()) {
		// get the container type of the other mesh for T - used to upcast othMesh
		using OContainer = typename OtherMesh::template ContainerOf<T>::type;

		const OContainer& c = static_cast<const OContainer&>(m);

		enableSameOptionalComponents(typename T::Components(), c);
	}
}

template<ElementConcept T>
template<typename OtherMesh>
void ElementContainer<T>::importFrom(const OtherMesh &m)
{
	if constexpr (OtherMesh::template hasContainerOf<T>()) {
		// get the container type of the other mesh for T - used to upcast
		// othMesh
		using Container = typename OtherMesh::template ContainerOf<T>::type;

		const Container& c = (const Container&)m;

		clearElements();

		// recreate a container having the same number of elements as the other
		// mesh
		// this call will also update the parentMesh pointers and sets the
		// vertical components vectors in vcVecTuple
		addElements(c.elementContainerSize());
		unsigned int eid = 0;
		for (const typename Container::ElementType& e : c.elements(false)) {
			// import the components of the e, when they are available
			// note that will set also the deleted flag of e, therefore if e is
			// deleted, then also element(eid) will be deleted.
			element(eid).importFrom(e);
			++eid;
		}
		// set the number of elements (different from the container size)
		en = c.en;
		if constexpr (
			comp::HasCustomComponents<T> &&
			comp::HasCustomComponents<typename Container::ElementType>)
		{
			ccVecMap = c.ccVecMap;
		}
	}
}

/**
 * This function imports from another mesh, the pointers of the element ElPtrBase stored on this
 * container.
 *
 * Checks if the other mesh has two containers: the one of T and the one of ElPtrBase.
 * Only if both containers exists in othMesh, then the import makes sense (e.g. we can import per
 * Vertex Face pointers (T = Vertex, ElPtrBase = Face) if othMesh has both a container of Vertices
 * and a Container of Faces).
 */
template<ElementConcept T>
template<typename OtherMesh, typename ElPtrBase>
void ElementContainer<T>::ElementContainer::importPointersFrom(
	const OtherMesh& othMesh,
	ElPtrBase*      base)
{
	// We need to be sure that the other mesh has two containers (that can be the same, see later):
	// - the one of Elements of same type as T
	// - the one of Elements of same type as ElPtrBase (the references that we are actually
	// importing on this Container of T elements)
	// Note that ElPtrBase may be the same of T (e.g. Vertex[T] has pointers of other
	// Vertices[ElPtrBase]) or different (e.g. Vertex[T] has pointers of Faces[ElPtrBase])
	if constexpr (
		OtherMesh::template hasContainerOf<T>() && OtherMesh::template hasContainerOf<ElPtrBase>()) {

		// get the containe type of the other mesh for MyBase - used for get the base pointer
		using OthBaseContainer = typename OtherMesh::template ContainerOf<ElPtrBase>::type;
		// get the container type of the other mesh for T - used to upcast othMesh
		using OthTContainer = typename OtherMesh::template ContainerOf<T>::type;

		// get the container base of the other mesh, that we use to import pointers
		const auto* cbase = othMesh.OthBaseContainer::vec.data();

		// upcast the other mesh to the container and import the pointers from the OthTContainer
		importPointersFromContainer((const OthTContainer&)othMesh, base, cbase);
	}
}

template<ElementConcept T>
template<typename ElPtr, typename... Comps>
void ElementContainer<T>::updatePointersOnComponents(
	const ElPtr* oldBase,
	const ElPtr* newBase,
	TypeWrapper<Comps...>)
{
	(updatePointersOnComponent<Comps>(oldBase, newBase), ...);
}

template<ElementConcept T>
template<typename ElPtr, typename... Comps>
void ElementContainer<T>::updatePointersOnComponents(
	const ElPtr* base,
	const std::vector<uint>& newIndices,
	TypeWrapper<Comps...>)
{
	(updatePointersOnComponent<Comps>(base, newIndices), ...);
}

/**
 * This member function is called when this Element container needs to import vertex references from
 * another Container c.
 *
 * It is called when an import from another mesh type is performed. The import first creates all
 * the elements in the newly created mesh using the importFrom function, and then imports references
 * (pointers) from each element of the other mesh.
 *
 * For each element of this and the other container, we compute the offset between the any vertex
 * reference contained in the other element and its base (the pointer of the vertex 0) in the other
 * mesh, and then add this offset to the base (the pointer of the vertex 0) in this mesh.
 *
 * Takes the following inputs:
 * - c: another container of the same element of this container (but with different components)
 * - base: the pointer of the first Vertex reference of this mesh. We will use it to compute the
 *         new vertex pointers in this container
 * - cbase: the pointer of the first Vertex reference of the other mesh. We will use it to compute
 *          the offset between any vertex reference contained in the other element and the base,
 *          and the offset is then used to compute the new vertex pointer for this container
 */
template<ElementConcept T>
template<typename Container, typename MyBase, typename CBase>
void ElementContainer<T>::importPointersFromContainer(
	const Container& c,
	MyBase*          base,
	const CBase*     cbase)
{
	using Comps = typename T::Components;

	importPointersOnComponentsFrom(c, base, cbase, Comps());
}

template<ElementConcept T>
template<typename Container, typename ElPtr, typename CBase, typename... Comps>
void ElementContainer<T>::importPointersOnComponentsFrom(
	const Container& c,
	ElPtr*           base,
	const CBase*     cbase,
	TypeWrapper<Comps...>)
{
	(importPointersOnComponentFrom<Comps>(c, base, cbase), ...);
}

/*
 * This function is called for each component of the element.
 *
 * Only if a component has references of the type ElPtr, then the updatePointers
 * on each element will be executed
 */
template<ElementConcept T>
template<typename Comp, typename ElPtr>
void ElementContainer<T>::updatePointersOnComponent(
	const ElPtr* oldBase,
	const ElPtr* newBase)
{
	if constexpr (comp::HasPointersOfType<Comp, ElPtr>) {
		if constexpr (comp::HasOptionalPointersOfType<Comp, ElPtr>) {
			if(isOptionalComponentEnabled<Comp>()) {
				for (T& e : elements()) {
					e.Comp::updatePointers(oldBase, newBase);
				}
			}
		}
		else {
			for (T& e : elements()) {
				e.Comp::updatePointers(oldBase, newBase);
			}
		}
	}
}

template<ElementConcept T>
template<typename Comp, typename ElPtr>
void ElementContainer<T>::updatePointersOnComponent(
	const ElPtr* base,
	const std::vector<uint>& newIndices)
{
	if constexpr (comp::HasPointersOfType<Comp, ElPtr>) {
		if constexpr (comp::HasOptionalPointersOfType<Comp, ElPtr>) {
			if(isOptionalComponentEnabled<Comp>()) {
				for (T& e : elements()) {
					e.Comp::updatePointers(base, newIndices);
				}
			}
		}
		else {
			for (T& e : elements()) {
				e.Comp::updatePointers(base, newIndices);
			}
		}
	}
}

template<ElementConcept T>
template<typename Comp, typename Container, typename ElPtr, typename CBase>
void ElementContainer<T>::importPointersOnComponentFrom(
	const Container& c,
	ElPtr*           base,
	const CBase*     cbase)
{
	if constexpr (comp::HasPointersOfType<Comp, ElPtr>) {
		if constexpr (comp::HasOptionalPointersOfType<Comp, ElPtr>) {
			if(isOptionalComponentEnabled<Comp>()) {
				for (uint i = 0; i < elementContainerSize(); ++i) {
					element(i).Comp::importPointersFrom(c.element(i), base, cbase);
				}
			}
		}
		else {
			for (uint i = 0; i < elementContainerSize(); ++i) {
				element(i).Comp::importPointersFrom(c.element(i), base, cbase);
			}
		}
	}
}

template<ElementConcept T>
template<typename Cont2, typename... Comps>
void ElementContainer<T>::enableSameOptionalComponents(TypeWrapper<Comps...>, const Cont2& c2)
{
	// for each Component in Comps, will call enableSameOptionalComponent
	(enableSameOptionalComponent<Comps>(c2), ...);
}

/**
 * This function enables the component Comp in this container if it is available in Cont2
 */
template<ElementConcept T>
template<typename Comp, typename Cont2>
void ElementContainer<T>::enableSameOptionalComponent(const Cont2& c2)
{
	// if Comp is an optional component in This container
	if constexpr (comp::IsOptionalComponent<Comp>) {
		// if Comp is available in Cont2
		if constexpr (comp::HasComponentOfType<typename Cont2::ElementType, Comp::COMPONENT_TYPE>){
			// if Comp is optional in Cont2
			if constexpr (comp::HasOptionalComponentOfType<typename Cont2::ElementType, Comp::COMPONENT_TYPE>) {
				// if Comp is enabled in Cont2, we enable it in this container
				if (c2.template isOptionalComponentEnabled<Comp::COMPONENT_TYPE>()) {
					enableOptionalComponent<Comp::COMPONENT_TYPE>();
				}
			}
			else { // if Comp is not optional (but is available), we enable it in this container
				enableOptionalComponent<Comp::COMPONENT_TYPE>();
			}
		}
	}
}

} // namespace vcl::mesh
