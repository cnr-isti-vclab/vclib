/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
#define VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H

#include <vector>

#include <vclib/concepts/mesh/components.h>
#include <vclib/concepts/mesh/containers.h>
#include <vclib/concepts/mesh/elements/element.h>
#include <vclib/iterators/mesh/element_container_iterator.h>
#include <vclib/views/view.h>

#include "custom_component_vector_handle.h"
#include "custom_components_vector_map.h"
#include "vertical_components_vector_tuple.h"

namespace vcl::mesh {

template<ElementConcept T>
class ElementContainer : public ElementContainerTriggerer
{
    template<ElementConcept U>
    friend class ElementContainer;

    using ElementContainerType = ElementContainer<T>;

    // filter components of elements, taking only vertical ones
    using vComps = vcl::FilterTypesByCondition<
        comp::IsVerticalComponentPred,
        typename T::Components>::type;

public:
    using ElementType    = T;
    using ParentMeshType = T::ParentMeshType;

protected:
    ParentMeshType* mParentMesh = nullptr;

    /**
     * @brief The number of elements in the container. Could be different
     * from elements.size() due to elements marked as deleted into the
     * container.
     */
    uint mElemNumber = 0;

    /**
     * @brief The vector of elements: will contain the set of elements,
     * each one of these will contain the data of the horizontal components and
     * a pointer to the parent mesh
     */
    std::vector<T> mElemVec;

    /**
     * @brief The tuple of vectors of all the vertical components of
     * the element. Contains both the optional and the persistent vertical
     * components
     */
    VerticalComponentsVectorTuple<vComps> mVerticalCompVecTuple;

    /**
     * @brief The map that associates a string to a vector of custom
     * components
     */
    CustomComponentsVectorMap<comp::HasCustomComponents<T>> mCustomCompVecMap;

public:
    static const uint ELEMENT_ID = T::ELEMENT_ID;

    /**
     * @brief Empty constructor that creates an empty container of Elements.
     */
    ElementContainer() = default;

protected:
    /* Members that are directly inherited by Containers (just renaming them) */
    using ElementIterator      = ElementContainerIterator<std::vector, T>;
    using ConstElementIterator = ConstElementContainerIterator<std::vector, T>;

    /**
     * @brief Returns a const reference of the element at the i-th position in
     * the Element Container of the Mesh, which will be the element having index
     * = i.
     *
     * This function does not perform any sanity check: if i is less than
     * elementContainerSize(), this function will return a valid Element
     * reference (note that the Element may have been flagged as deleted).
     *
     * @param[in] i: the index of the element that will be returned.
     */
    const T& element(uint i) const { return mElemVec[i]; }

    /**
     * @brief Returns a reference of the element at the i-th position in the
     * Element Container of the Mesh, which will be the element having index =
     * i.
     *
     * This function does not perform any sanity check: if i is less than
     * elementContainerSize(), this function will return a valid Element
     * reference (note that the Element may have been flagged as deleted).
     *
     * @param[in] i: the index of the element that will be returned.
     */
    T& element(uint i) { return mElemVec[i]; }

    /**
     * @brief Returns the number of **non-deleted** elements contained in the
     * Element container of the Mesh.
     *
     * If elementNumber() != elementContainerSize(), it means that there are
     * some elements that are flagged as deleted.
     *
     * @return The number of non-deleted elements of the Mesh.
     */
    uint elementNumber() const { return mElemNumber; }

    /**
     * @brief Returns the number of elements (also deleted) contained in the
     * Element container of the Mesh.
     *
     * If elementNumber() != elementContainerSize(), it means that there are
     * some elements that are flagged as deleted.
     *
     * @return The number of all the elements contained in the Mesh.
     */
    uint elementContainerSize() const { return mElemVec.size(); }

    /**
     * @brief Returns the number of deleted elements in the Element container,
     * that is elementContainerSize() - elementNumber().
     *
     * @return The number of deleted elements in the container.
     */
    uint deletedElementNumber() const
    {
        return elementContainerSize() - elementNumber();
    }

    uint addElement()
    {
        mVerticalCompVecTuple.resize(mElemVec.size() + 1);
        if constexpr (comp::HasCustomComponents<T>)
            mCustomCompVecMap.resize(mElemVec.size() + 1);

        T* oldB = mElemVec.data();
        mElemVec.emplace_back();
        T* newB = mElemVec.data();
        mElemNumber++;

        mElemVec.back().setParentMesh(mParentMesh);
        mElemVec.back().initVerticalComponents();

        if (oldB != newB) {
            setParentMeshPointers(mParentMesh);
            mParentMesh->updateAllPointers(oldB, newB);
        }

        return mElemVec.size() - 1;
    }

    /**
     * @brief Adds size elements to the Element Container.
     *
     * Returns the id of the first added element.
     *
     * @param size
     * @return the id of the first added element.
     */
    uint addElements(uint size)
    {
        mVerticalCompVecTuple.resize(mElemVec.size() + size);
        if constexpr (comp::HasCustomComponents<T>)
            mCustomCompVecMap.resize(mElemVec.size() + size);

        uint baseId = mElemVec.size();
        T*   oldB   = mElemVec.data();
        mElemVec.resize(mElemVec.size() + size);
        T* newB = mElemVec.data();
        mElemNumber += size;

        for (uint i = baseId; i < mElemVec.size(); ++i) {
            mElemVec[i].setParentMesh(mParentMesh);
            mElemVec[i].initVerticalComponents();
        }

        if (oldB != newB) {
            setParentMeshPointers(mParentMesh);
            mParentMesh->updateAllPointers(oldB, newB);
        }

        return baseId;
    }

    void clearElements()
    {
        mElemVec.clear();
        mElemNumber = 0;

        // clear vertical and custom components

        mVerticalCompVecTuple.clear();
        if constexpr (comp::HasCustomComponents<T>)
            mCustomCompVecMap.clear();
    }

    /**
     * @brief Resizes the element container to contain `size` elements.
     *
     * If the new size is greater than the old one, new elements are added to
     * the container, and a reallocation may happen. If the new size is smaller
     * than the old one, the container will keep its first non-deleted `size`
     * elements, and the remaining elements are marked as deleted.
     *
     * If the call of this function will cause a reallocation of the Element
     * container, the function will automatically take care of updating all the
     * Element pointers contained in the Mesh.
     *
     * @warning The given `size` is relative to the number of non-deleted
     * elements, not to the size of the element container. For example, if you
     * have a mesh with 10 elements and elementContainerSize() == 20, calling
     * resizeElements(5) will not cause a reallocation of the container, but
     * will mark as deleted the least 5 non-deleted elements of the container.
     * In the same scenario, calling resizeElements(15) will result in a element
     * container having 15 elements and elementContainerSize() == 25. The
     * latest 5 elements will be the newly added.
     *
     * @warning Any pointer to deleted elements in the Mesh will be left
     * unchanged, and therefore will point to invalid elements. This means that
     * if you call this member function with a lower number of elements, you'll
     * need to manually manage the pointers to the deleted elements.
     *
     * @param[in] size: the new size of the Element container.
     */
    void resizeElements(uint size)
    {
        if (size > mElemNumber) {
            addElements(size - mElemNumber);
        }
        else if (size < mElemNumber) {
            uint nToDelete = mElemNumber - size;
            for (uint i = mElemVec.size() - 1; nToDelete > 0; --i) {
                if (!mElemVec[i].isDeleted()) {
                    deleteElement(i);
                }
            }
        }
    }

    void reserveElements(uint size)
    {
        T* oldB = mElemVec.data();
        mElemVec.reserve(size);
        T* newB = mElemVec.data();

        mVerticalCompVecTuple.reserve(size);
        if constexpr (comp::HasCustomComponents<T>)
            mCustomCompVecMap.reserve(size);

        if (oldB != newB) {
            setParentMeshPointers(mParentMesh);
            mParentMesh->updateAllPointers(oldB, newB);
        }
    }

    /**
     * @brief Compacts the element container, keeping only the non-deleted
     * elements.
     *
     * @return a vector that tells, for each old element index, the new index of
     * the element. Will contain UINT_NULL if the element has been deleted.
     */
    std::vector<uint> compactElements()
    {
        std::vector<uint> newIndices = elementCompactIndices();
        if (elementNumber() != elementContainerSize()) {
            compactVector(mElemVec, newIndices);

            mVerticalCompVecTuple.compact(newIndices);
            if constexpr (comp::HasCustomComponents<T>)
                mCustomCompVecMap.compact(newIndices);

            updateElementIndices(newIndices);
        }
        return newIndices;
    }

    /**
     * @brief Marks as deleted the element with the given id.
     *
     * This member function does not perform any reallocation of the elements:
     * the deleted elements will stay in the Element Container, but will be
     * marked as deleted.
     *
     * Deleted elements are automatically jumped by the iterators provided by
     * the Element Container.
     *
     * @warning If there were pointers to the deleted element in this or other
     * containers, they will not be updated.
     *
     * @param[in] i: the id of the element that will be marked as deleted.
     */
    void deleteElement(uint i)
    {
        mElemVec[i].deletedBit() = true;
        --mElemNumber;
    }

    /**
     * @brief Marks as deleted the given element, before asserting that the
     * element belongs to this container.
     *
     * This member function does not perform any reallocation of the elements:
     * the deleted elements will stay in the Element Container, but will be
     * marked as deleted.
     *
     * Deleted elements are automatically jumped by the iterators provided by
     * the Element Container.
     *
     * @warning If there were pointers to the deleted element in this or other
     * containers, they will not be updated.
     *
     * @param[in] e: the pointer of the element that will be marked as deleted.
     */
    void deleteElement(const T* e) { deleteElement(index(e)); }

    /**
     * @brief This is an utility member function that returns the index of an
     * element if the container would be compact, that is the number of
     * non-deleted elements before the element with the given index.
     *
     * Complexity: O(n), with n the number of elements in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of a element of the container.
     * @return The index that the element with index i would have if this
     * container would be compact.
     */
    uint elementIndexIfCompact(uint i) const
    {
        if (mElemVec.size() == mElemNumber)
            return i;
        else {
            uint cnt = 0;
            for (uint ii = 0; ii < i; ii++) {
                if (!mElemVec[ii].deleted())
                    ++cnt;
            }
            return cnt;
        }
    }

    /**
     * @brief Returns a vector that tells, for each actual element index, the
     * new index that the element would have in a compacted container. For each
     * deleted element index, the value of the vector will be UINT_NULL.
     *
     * This is useful if you need to know the indices of the elements that they
     * would have in a compact container, without considering the deleted ones.
     *
     * @return A vector containing, for each element index, its index if the
     * container would be compact.
     */
    std::vector<uint> elementCompactIndices() const
    {
        std::vector<uint> newIndices(mElemVec.size());
        uint              k = 0;
        for (uint i = 0; i < mElemVec.size(); ++i) {
            if (!mElemVec[i].deleted()) {
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
     * @brief Appends the elements of the given container to this one.
     *
     * This function does not update the pointers contained in the element
     * container: it only creates new elements and copies the data from the
     * given container. It imports also all the data contained in the
     * vertical/optional components.
     *
     * The parent mesh will take care of updating the pointers, after calling
     * this function.
     *
     * @param[in] other: the container from which the elements will be copied
     * and appended.
     */
    void append(const ElementContainer<T>& other)
    {
        using Comps = T::Components;

        uint on = other.elementNumber();
        uint n  = elementContainerSize();
        addElements(on);
        for (uint i = 0; i < on; ++i) {
            // copy everything from the other elements, also the pointers:
            element(n + i) = other.element(i);
            element(n + i).setParentMesh(mParentMesh);
        }
        // importing also optional, vertical and custom components:
        appendVerticalComponents(other, vComps());
        appendCustomComponents(other);
    }

    /**
     * @brief Returns an iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted elements of the
     * container. You can change this option by calling this function with
     * jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted elements.
     * @return An iterator the the first element of the container.
     */
    ElementIterator elementBegin(bool jumpDeleted = true)
    {
        auto it = mElemVec.begin();
        if (jumpDeleted) {
            // if the user asked to jump the deleted elements, and the first
            // element is deleted, we need to move forward until we find the
            // first non-deleted element
            while (it != mElemVec.end() && it->deleted()) {
                ++it;
            }
        }
        return ElementIterator(
            it, mElemVec, jumpDeleted && mElemVec.size() != mElemNumber);
    }

    /**
     * @brief Returns an iterator to the end of the container.
     * @return An iterator to the end of the container.
     */
    ElementIterator elementEnd()
    {
        return ElementIterator(mElemVec.end(), mElemVec);
    }

    /**
     * @brief Returns a const iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted elements of the
     * container. You can change this option by calling this function with
     * jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted elements.
     * @return A const iterator the the first element of the container.
     */
    ConstElementIterator elementBegin(bool jumpDeleted = true) const
    {
        auto it = mElemVec.begin();
        if (jumpDeleted) {
            // if the user asked to jump the deleted elements, and the first
            // element is deleted, we need to move forward until we find the
            // first non-deleted element
            while (it != mElemVec.end() && it->deleted()) {
                ++it;
            }
        }
        return ConstElementIterator(
            it, mElemVec, jumpDeleted && mElemVec.size() != mElemNumber);
    }

    /**
     * @brief Returns a const iterator to the end of the container.
     * @return A const iterator to the end of the container.
     */
    ConstElementIterator elementEnd() const
    {
        return ConstElementIterator(mElemVec.end(), mElemVec);
    }

    /**
     * @brief Returns a view object that allows to iterate over the elements of
     * the containers, providing two member functions begin() and end().
     *
     * This member function is very useful when you want to iterate over the
     * elements using the C++ foreach syntax:
     *
     * @code{.cpp}
     * for (Element& f : m.elements()){
     *     // do something with this element
     * }
     * @endcode
     *
     * The iterator used to iterate over elements is automatically initialized
     * to jump deleted elements of the container. You can change this option by
     * calling this function with jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted elements.
     * @return An object having begin() and end() function, allowing to iterate
     * over the container.
     */
    View<ElementIterator> elements(bool jumpDeleted = true)
    {
        return vcl::View(
            elementBegin(jumpDeleted && mElemVec.size() != mElemNumber),
            elementEnd());
    }

    /**
     * @brief Returns a view object that allows to iterate over the elements of
     * the containers, providing two member functions begin() and end().
     *
     * This member function is very useful when you want to iterate over the
     * elements using the C++ foreach syntax:
     *
     * @code{.cpp}
     * for (const Element& f : m.elements()){
     *     // do something with this element
     * }
     * @endcode
     *
     * The iterator used to iterate over elements is automatically initialized
     * to jump deleted elements of the container. You can change this option by
     * calling this function with jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted elements.
     * @return An object having begin() and end() function, allowing to iterate
     * over the container.
     */
    View<ConstElementIterator> elements(bool jumpDeleted = true) const
    {
        return vcl::View(
            elementBegin(jumpDeleted && mElemVec.size() != mElemNumber),
            elementEnd());
    }

    void enableAllOptionalComponents()
    {
        mVerticalCompVecTuple.enableAllOptionalComponents();
    }

    void disableAllOptionalComponents()
    {
        mVerticalCompVecTuple.disableAllOptionalComponents();
    }

    template<typename C>
    bool isOptionalComponentEnabled() const
    {
        return mVerticalCompVecTuple.template isComponentEnabled<C>();
    }

    template<uint COMP_ID>
    bool isOptionalComponentEnabled() const
    {
        return mVerticalCompVecTuple.template isComponentEnabled<COMP_ID>();
    }

    template<typename C>
    void enableOptionalComponent()
    {
        mVerticalCompVecTuple.template enableComponent<C>();
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

    template<uint COMP_ID>
    void enableOptionalComponent()
    {
        using C = comp::ComponentOfType<COMP_ID, typename T::Components>;
        enableOptionalComponent<C>();
    }

    template<typename C>
    void disableOptionalComponent()
    {
        mVerticalCompVecTuple.template disableComponent<C>();
    }

    template<uint COMP_ID>
    void disableOptionalComponent()
    {
        mVerticalCompVecTuple.template disableComponent<COMP_ID>();
    }

    // Custom Components

    bool hasElemCustomComponent(const std::string& name) const
        requires comp::HasCustomComponents<T>
    {
        return mCustomCompVecMap.componentExists(name);
    }

    std::vector<std::string> elemCustomComponentNames() const
        requires comp::HasCustomComponents<T>
    {
        return mCustomCompVecMap.allComponentNames();
    }

    template<typename K>
    bool isElemCustomComponentOfType(const std::string& name) const
        requires comp::HasCustomComponents<T>
    {
        return mCustomCompVecMap.template isComponentOfType<K>(name);
    }

    std::type_index elemComponentType(const std::string& name) const
        requires comp::HasCustomComponents<T>
    {
        return mCustomCompVecMap.componentType(name);
    }

    template<typename K>
    std::vector<std::string> elemCustomComponentNamesOfType() const
        requires comp::HasCustomComponents<T>
    {
        return mCustomCompVecMap.template allComponentNamesOfType<K>();
    }

    template<typename K>
    void addElemCustomComponent(const std::string& name)
        requires comp::HasCustomComponents<T>
    {
        mCustomCompVecMap.template addNewComponent<K>(
            name, elementContainerSize());
    }

    void deleteElemCustomComponent(const std::string& name)
        requires comp::HasCustomComponents<T>
    {
        mCustomCompVecMap.deleteComponent(name);
    }

    template<typename K>
    CustomComponentVectorHandle<K> customComponentVectorHandle(
        const std::string& name) requires comp::HasCustomComponents<T>
    {
        std::vector<std::any>& cc =
            mCustomCompVecMap.template componentVector<K>(name);
        CustomComponentVectorHandle<K> v(cc);
        return v;
    }

    template<typename K>
    ConstCustomComponentVectorHandle<K> customComponentVectorHandle(
        const std::string& name) const requires comp::HasCustomComponents<T>
    {
        const std::vector<std::any>& cc =
            mCustomCompVecMap.template componentVector<K>(name);
        ConstCustomComponentVectorHandle<K> v(cc);
        return cc;
    }

    uint index(const T* e) const
    {
        assert(
            !mElemVec.empty() && e >= mElemVec.data() && e <= &mElemVec.back());
        return e - mElemVec.data();
    }

    void setParentMeshPointers(void* pm)
    {
        mParentMesh = static_cast<ParentMeshType*>(pm);
        for (auto& e : elements(false)) {
            e.setParentMesh(pm);
        }
    }

    template<typename Element>
    void updatePointers(
        const Element* oldBase,
        const Element* newBase,
        uint           firstElementToProcess = 0,
        uint           offset                = 0)
    {
        using Comps = T::Components;

        updatePointersOnComponents(
            oldBase, newBase, Comps(), firstElementToProcess, offset);
    }

    template<typename Element>
    void updatePointers(
        const Element*           base,
        const std::vector<uint>& newIndices)
    {
        using Comps = T::Components;

        updatePointersOnComponents(base, newIndices, Comps());
    }

    /**
     * @brief Updates all the indices and pointers of the elements of this
     * container that are stored in any container of the mesh, according to the
     * mapping stored in the newIndices vector, that tells for each old element
     * index, the new element index.
     *
     * This function is useful when some elements of this container have been
     * deleted, and you want to update the indices/pointers stored in all the
     * containers of the mesh accordingly.
     *
     * E.g.: if this is a vertex container, this function will update the
     * indices and pointers of the vertices stored in all the continers of the
     * mesh (e.g. the face container that stores vertex pointers, or the vertex
     * container itself that stores adjacent vertices) according to the mapping
     * stored in the newIndices vector.
     *
     * Supposing you deleted a set of vertices, you can give to this function
     * the vector telling, for each of the old vertex indices, the new vertex
     * index (or UINT_NULL if you want to leave unreferences that vertices).
     * This function will update all the pointers stored in the mesh containers
     * accordingly.
     *
     * @note This function *does not change the position of the elements in this
     * container*. It just updates the indices/pointers of the elements stored
     * in this or other containers.
     *
     * @param[in] newIndices: a vector that tells, for each old element index,
     * the new element index. If the old element must be left as unreferenced
     * (setting `nullptr` to the pointers), the value of the vector must be
     * UINT_NULL.
     */
    void updateElementIndices(const std::vector<uint>& newIndices)
    {
        T* base = mElemVec.data();

        mParentMesh->updateAllPointers(base, newIndices);
    }

    template<typename OtherMesh>
    void enableOptionalComponentsOf(const OtherMesh& m)
    {
        if constexpr (OtherMesh::template hasContainerOf<T>()) {
            // get the container type of the other mesh for T - used to upcast
            // othMesh
            using OContainer = OtherMesh::template ContainerOf<T>::type;

            const OContainer& c = static_cast<const OContainer&>(m);

            enableSameOptionalComponents(typename T::Components(), c);
        }
    }

    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        if constexpr (OtherMesh::template hasContainerOf<T>()) {
            // get the container type of the other mesh for T - used to upcast
            // othMesh
            using Container = OtherMesh::template ContainerOf<T>::type;

            const Container& c = (const Container&) m;

            clearElements();

            // recreate a container having the same number of elements as the
            // other mesh. this call will also update the parentMesh pointers
            // and sets the vertical components vectors in vcVecTuple
            addElements(c.elementContainerSize());
            unsigned int eid = 0;
            for (const typename Container::ElementType& e : c.elements(false)) {
                // import the components of the e, when they are available.
                // note that will set also the deleted flag of e, therefore if e
                // is deleted, then also element(eid) will be deleted.
                element(eid).importFrom(e);
                ++eid;
            }
            // set the number of elements (different from the container size)
            mElemNumber = c.mElemNumber;
            if constexpr (
                comp::HasCustomComponents<T> &&
                comp::HasCustomComponents<typename Container::ElementType>)
            {
                mCustomCompVecMap = c.mCustomCompVecMap;
            }
        }
    }

    /**
     * This function imports from another mesh, the pointers of the element
     * OtherElement stored on this container.
     *
     * Checks if the other mesh has two containers: the one of T and the one of
     * OtherElement. Only if both containers exists in othMesh, then the import
     * makes sense (e.g. we can import per Vertex Face pointers (T = Vertex,
     * OtherElement = Face) if othMesh has both a container of Vertices and a
     * Container of Faces).
     */
    template<typename OtherMesh, typename OtherElement>
    void importPointersFrom(const OtherMesh& othMesh, OtherElement* base)
    {
        // We need to be sure that the other mesh has two containers (that can
        // be the same, see later):
        // - the one of Elements of same type as T
        // - the one of Elements of same type as OtherElement (the pointers
        //   that we are actually importing on this Container of T elements)
        // Note that OtherElement may be the same of T (e.g. Vertex[T] has
        // pointers of other Vertices[OtherElement]) or different (e.g.
        // Vertex[T] has pointers of Faces[OtherElement])
        if constexpr (
            OtherMesh::template hasContainerOf<T>() &&
            OtherMesh::template hasContainerOf<OtherElement>())
        {
            // get the containe type of the other mesh for MyBase - used for get
            // the base pointer
            using OthBaseContainer =
                OtherMesh::template ContainerOf<OtherElement>::type;
            // get the container type of the other mesh for T - used to upcast
            // othMesh
            using OthTContainer = OtherMesh::template ContainerOf<T>::type;

            // get the container base of the other mesh, that we use to import
            // pointers
            const auto* cbase = othMesh.OthBaseContainer::mElemVec.data();

            // upcast the other mesh to the container and import the pointers
            // from the OthTContainer
            importPointersFromContainer(
                (const OthTContainer&) othMesh, base, cbase);
        }
    }

private:
    template<typename ElPtr, typename... Comps>
    void updatePointersOnComponents(
        const ElPtr* oldBase,
        const ElPtr* newBase,
        TypeWrapper<Comps...>,
        uint firstElementToProcess = 0,
        uint offset                = 0)
    {
        (updatePointersOnComponent<Comps>(
             oldBase, newBase, firstElementToProcess, offset),
         ...);
    }

    template<typename ElPtr, typename... Comps>
    void updatePointersOnComponents(
        const ElPtr*             base,
        const std::vector<uint>& newIndices,
        TypeWrapper<Comps...>)
    {
        (updatePointersOnComponent<Comps>(base, newIndices), ...);
    }

    /**
     * This member function is called when this Element container needs to
     * import element pointers from another Container c.
     *
     * It is called when an import from another mesh type is performed. The
     * import first creates all the elements in the newly created mesh using the
     * importFrom function, and then imports pointers from each element of the
     * other mesh.
     *
     * For each element of this and the other container, we compute the offset
     * between the any element pointer contained in the other element and its
     * base (the pointer of the element 0) in the other mesh, and then add this
     * offset to the base (the pointer of the element 0) in this mesh.
     *
     * Takes the following inputs:
     * - c: another container of the same element of this container (but
     *   probably with different components)
     * - base: the pointer of the first Element of this mesh. We will use it to
     *   compute the new element pointers in this container
     * - cbase: the pointer of the first Element of the other mesh. We will use
     *   it to compute the offset between any element contained in the other
     *   element and the base. The offset is then used to compute the new
     *   element pointer for this container
     */
    template<typename Container, typename MyBase, typename CBase>
    void importPointersFromContainer(
        const Container& c,
        MyBase*          base,
        const CBase*     cbase)
    {
        using Comps = T::Components;

        importPointersOnComponentsFrom(c, base, cbase, Comps());
    }

    template<
        typename Container,
        typename ElPtr,
        typename CBase,
        typename... Comps>
    void importPointersOnComponentsFrom(
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
     * Only if a component has references of the type ElPtr, then the
     * updatePointers on each element will be executed.
     *
     * firstElementToProcess and offset are used only when an append operation
     * has been executed. In this case, the firstElementToProcess is the index
     * of the first element that has been appended, and offset is the number of
     * ElPtr elements that have been appended (the offset that must be added
     * to the newBase w.r.t. the oldBase that was the other container from which
     * the elements have been copied).
     */
    template<typename Comp, typename ElPtr>
    void updatePointersOnComponent(
        const ElPtr* oldBase,
        const ElPtr* newBase,
        uint         firstElementToProcess = 0,
        uint         offset                = 0)
    {
        if constexpr (comp::HasReferencesOfType<Comp, ElPtr>) {
            // lambda to avoid code duplication
            auto loop = [&]() {
                for (uint i = firstElementToProcess; i < elementContainerSize();
                     i++)
                {
                    T& e = element(i);
                    if (!e.deleted()) {
                        e.Comp::updatePointers(oldBase, newBase, offset);
                    }
                }
            };

            if constexpr (comp::HasOptionalReferencesOfType<Comp, ElPtr>) {
                if (isOptionalComponentEnabled<Comp>()) {
                    loop();
                }
            }
            else {
                loop();
            }
        }
    }

    template<typename Comp, typename ElPtr>
    void updatePointersOnComponent(
        const ElPtr*             base,
        const std::vector<uint>& newIndices)
    {
        if constexpr (comp::HasReferencesOfType<Comp, ElPtr>) {
            if constexpr (comp::HasOptionalReferencesOfType<Comp, ElPtr>) {
                if (isOptionalComponentEnabled<Comp>()) {
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

    template<typename Comp, typename Container, typename ElPtr, typename CBase>
    void importPointersOnComponentFrom(
        const Container& c,
        ElPtr*           base,
        const CBase*     cbase)
    {
        if constexpr (comp::HasReferencesOfType<Comp, ElPtr>) {
            if constexpr (comp::HasOptionalReferencesOfType<Comp, ElPtr>) {
                if (isOptionalComponentEnabled<Comp>()) {
                    for (uint i = 0; i < elementContainerSize(); ++i) {
                        element(i).Comp::importPointersFrom(
                            c.element(i), base, cbase);
                    }
                }
            }
            else {
                for (uint i = 0; i < elementContainerSize(); ++i) {
                    element(i).Comp::importPointersFrom(
                        c.element(i), base, cbase);
                }
            }
        }
    }

    template<typename... Comps>
    void appendVerticalComponents(
        const ElementContainer<T>& other,
        TypeWrapper<Comps...>)
    {
        (appendVerticalComponent<Comps>(other), ...);
    }

    template<typename Comp>
    void appendVerticalComponent(const ElementContainer<T>& other)
    {
        uint on = other.elementNumber();
        uint n = elementContainerSize() - on;

        if (mVerticalCompVecTuple.template isComponentEnabled<Comp>()
            && other.mVerticalCompVecTuple.template isComponentEnabled<Comp>())
        {
            auto& vc = mVerticalCompVecTuple.template vector<Comp>();
            const auto& ovc =
                other.mVerticalCompVecTuple.template vector<Comp>();

            for (uint i = 0; i < on; ++i) {
                vc[n + i] = ovc[i];
            }
        }
    }

    void appendCustomComponents(const ElementContainer<T>& other)
    {
        if constexpr (comp::HasCustomComponents<T>) {
            uint on = other.elementNumber();
            uint n = elementContainerSize() - on;

            std::vector<std::string> ccNames =
                mCustomCompVecMap.allComponentNames();

            for (const std::string& name : ccNames) {
                for (uint i = 0; i < on; ++i) {
                    mCustomCompVecMap.importSameCustomComponentFrom(
                        n+ i, i, name, other.mCustomCompVecMap);
                }
            }
        }
    }

    template<typename Cont2, typename... Comps>
    void enableSameOptionalComponents(TypeWrapper<Comps...>, const Cont2& c2)
    {
        // for each Component in Comps, will call enableSameOptionalComponent
        (enableSameOptionalComponent<Comps>(c2), ...);
    }

    /**
     * This function enables the component Comp in this container if it is
     * available in Cont2
     */
    template<typename Comp, typename Cont2>
    void enableSameOptionalComponent(const Cont2& c2)
    {
        // if Comp is an optional component in This container
        if constexpr (comp::IsOptionalComponent<Comp>) {
            // if Comp is available in Cont2
            if constexpr (comp::HasComponentOfType<
                              typename Cont2::ElementType,
                              Comp::COMPONENT_ID>)
            {
                // if Comp is optional in Cont2
                if constexpr (comp::HasOptionalComponentOfType<
                                  typename Cont2::ElementType,
                                  Comp::COMPONENT_ID>)
                {
                    // if Comp is enabled in Cont2, we enable it in this
                    // container
                    if (c2.template isOptionalComponentEnabled<
                            Comp::COMPONENT_ID>())
                    {
                        enableOptionalComponent<Comp::COMPONENT_ID>();
                    }
                }
                // if Comp is not optional (but is available), we enable it in
                // this container
                else {
                    enableOptionalComponent<Comp::COMPONENT_ID>();
                }
            }
        }
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
