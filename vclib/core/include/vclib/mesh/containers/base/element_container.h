// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_BASE_ELEMENT_CONTAINER_H
#define VCL_MESH_CONTAINERS_BASE_ELEMENT_CONTAINER_H

#include "base.h"
#include "component_view.h"
#include "custom_component_vector_handle.h"
#include "custom_components_vector_map.h"
#include "element_container_proxy.h"

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/base/component_predicates.h>
#include <vclib/mesh/components/bit_flags.h>
#include <vclib/mesh/components/custom_components.h>
#include <vclib/mesh/components/vertex_references.h>
#include <vclib/mesh/elements/base/element_proxy.h>
#include <vclib/mesh/iterators/element_iterator.h>

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <algorithm>
#include <cassert>
#include <concepts>
#include <istream>
#include <ostream>
#include <utility>

namespace vcl::mesh {

/**
 * @brief The ElementContainer class is the low-level base container that
 * manages the Structure of Arrays (SoA) storage for a specific mesh element
 * type.
 *
 * It acts as the backbone for concrete element containers (such as
 * VertexContainer, FaceContainer, and EdgeContainer). For each component in the
 * ComponentList, ElementContainer maintains a dedicated, contiguous vector in a
 * ParallelVectorsTuple.
 *
 * Key responsibilities:
 * - Manages memory allocation, resizing, and element count tracking (both total
 *   allocated size and active non-deleted count).
 * - Tracks deleted elements via the mandatory BitFlags component and provides
 *   compacting routines (compactElements, elementCompactIndices).
 * - Controls dynamic enabling and disabling of optional components.
 * - Coordinates topological updates and reference remapping across containers
 *   when elements are compacted (updateReferences).
 * - Exposes low-level access to elements as strongly-typed flyweight proxies
 *   via the template template parameter Element (e.g. Vertex, Face).
 *
 * @tparam ComponentList: A vcl::TypeWrapper wrapping the list of components
 * that compose the element (e.g. VertexComponents, FaceComponents). Each
 * component determines a parallel vector column in the underlying SoA storage.
 * Must include the BitFlags component.
 *
 * @tparam ParentMesh: The enclosing Mesh class type to which this container
 * belongs, or void if used as an unbound placeholder prior to mesh rebinding.
 * Allows element proxies to navigate across containers (e.g. from Face to
 * Vertex) through container.parentMesh().
 *
 * @tparam Index: An unsigned integral type used for element indices.
 * Determines index representation width across the container (e.g. uint32_t
 * or uint64_t).
 *
 * @tparam Element: A template template parameter representing the concrete
 * element proxy class template (e.g. Vertex, Face, Edge), instantiated
 * internally as Element<ElementContainer> (ElementType) and
 * Element<const ElementContainer> (ConstElementType). This allows
 * ElementContainer and its iterators to instantiate and return strongly-typed
 * element proxies directly without intermediate type erasure.
 *
 * @ingroup containers
 */
template<
    typename ComponentList,
    typename ParentMesh,
    std::unsigned_integral Index,
    template<typename...> class Element>
class ElementContainer
{
    static_assert(
        comp::HasBitFlags<ComponentList>,
        "An Element must have the BitFlags component "
        "(vcl::comp::BitFlags).");

    using VectorComponents = typename FilterTypesByCondition<
        detail::HasVectorColumnPred,
        ComponentList>::type;

    using VectorTupleType = typename detail::ParallelVectorsTupleFromComponents<
        VectorComponents>::type;

    ParentMesh* mParentMesh = nullptr;

    /**
     * @brief The number of elements in the container. Could be different
     * from elements.size() due to elements marked as deleted into the
     * container.
     */
    std::size_t mElemCount = 0;

    /**
     * @brief The tuple of vectors that stores the components of the elements.
     *
     * For each component in the Components list, there is a corresponding
     * vector in this tuple that stores the data for that component for all
     * elements in the container.
     */
    VectorTupleType mElemComponents;

    /**
     * @brief The map that associates a string to a vector of custom
     * components
     */
    [[no_unique_address]] detail::CustomComponentsVectorMap<
        comp::HasCustomComponents<ComponentList>> mCustomComponents;

public:
    using Iterator         = ElementIterator<ElementContainer>;
    using ConstIterator    = ElementIterator<const ElementContainer>;
    using Components       = ComponentList;
    using IndexType        = Index;
    using ParentMeshType   = ParentMesh;
    using ElementType      = Element<ElementContainer>;
    using ConstElementType = Element<const ElementContainer>;

    /**
     * @brief The null/invalid sentinel value for this container's index type.
     */
    static constexpr IndexType NULL_INDEX = vcl::NULL_INDEX<IndexType>;

    static constexpr ElemId ELEMENT_ID = ElementType::ELEMENT_ID;

    /**
     * @brief Empty constructor. Enables non-optional components.
     */
    ElementContainer() { enableNonOptionalComponents(Components {}); }

    /**
     * @brief Copy constructor. Deep copies elements, components, and state.
     */
    ElementContainer(const ElementContainer&) = default;

    /**
     * @brief Move constructor. Moves all element storage into this container,
     * leaving other in a valid empty state (elementCount() == 0,
     * elementContainerSize() == 0).
     *
     * @param other The container to move from.
     */
    ElementContainer(ElementContainer&& other) noexcept :
            mParentMesh(std::exchange(other.mParentMesh, nullptr)),
            mElemCount(std::exchange(other.mElemCount, 0)),
            mElemComponents(std::move(other.mElemComponents)),
            mCustomComponents(std::move(other.mCustomComponents))
    {
    }

    /**
     * @brief Assignment operator of the ElementContainer using the Copy & Swap
     * idiom. Handles both copy assignment and move assignment seamlessly and
     * provides the strong exception safety guarantee.
     *
     * @param other The container from which to copy or move data.
     * @return Reference to this container after assignment.
     */
    ElementContainer& operator=(ElementContainer other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swaps the content of this container with another.
     *
     * @param other The container to swap with.
     */
    void swap(ElementContainer& other) noexcept
    {
        using std::swap;
        swap(mElemCount, other.mElemCount);
        swap(mElemComponents, other.mElemComponents);
        swap(mCustomComponents, other.mCustomComponents);
    }

    /**
     * @brief Swaps the contents of two ElementContainer instances.
     *
     * @param a The first container.
     * @param b The second container.
     */
    friend void swap(ElementContainer& a, ElementContainer& b) noexcept
    {
        a.swap(b);
    }

    /* Members that are inherited and wrapped by derived element containers */

    /**
     * @brief Returns a const proxy of the element at the i-th position in
     * the Element Container of the Mesh, which will be the element having index
     * = i.
     *
     * This function does not perform any sanity check: if i is less than
     * elementContainerSize(), this function will return a valid
     * ConstElementType (note that the Element may have been flagged as
     * deleted).
     *
     * @param[in] i: the index of the element that will be returned.
     * @return A const proxy of the i-th element of the Element Container.
     */
    ConstElementType element(IndexType i) const
    {
        assert(i < size() || i == NULL_INDEX);
        return ConstElementType(*this, i);
    }

    /**
     * @brief Returns a proxy of the element at the i-th position in the
     * Element Container of the Mesh, which will be the element having index =
     * i.
     *
     * This function does not perform any sanity check: if i is less than
     * elementContainerSize(), this function will return a valid ElementType
     * (note that the Element may have been flagged as deleted).
     *
     * @param[in] i: the index of the element that will be returned.
     * @return A proxy of the i-th element of the Element Container.
     */
    ElementType element(IndexType i)
    {
        assert(i < size() || i == NULL_INDEX);
        return ElementType(*this, i);
    }

    /**
     * @brief Returns the number of **non-deleted** elements contained in the
     * Element container of the Mesh.
     *
     * If elementCount() != elementContainerSize(), it means that there are
     * some elements that are flagged as deleted.
     *
     * @return The number of non-deleted elements of the Mesh.
     */
    std::size_t elementCount() const { return mElemCount; }

    /**
     * @brief Returns the number of elements (also deleted) contained in the
     * Element container of the Mesh.
     *
     * If elementCount() != elementContainerSize(), it means that there are
     * some elements that are flagged as deleted.
     *
     * @return The number of all the elements contained in the Mesh.
     */
    std::size_t elementContainerSize() const { return mElemComponents.size(); }

    /**
     * @brief Returns the number of deleted elements in the Element container,
     * that is elementContainerSize() - elementCount().
     *
     * @return The number of deleted elements in the container.
     */
    std::size_t deletedElementCount() const { return size() - elementCount(); }

    /**
     * @brief Returns whether the element at the given index is marked as
     * deleted.
     *
     * @param[in] i: the index of the element to check.
     * @return `true` if the element is marked as deleted, `false` otherwise.
     */
    bool isElementDeleted(IndexType i) const
    {
        assert(i < size());
        return vector<CompId::BIT_FLAGS>()[i][0];
    }

    /**
     * @brief Adds an element to the Element Container.
     *
     * Returns the index of the added element.
     *
     * @return The index of the added element.
     */
    IndexType addElement()
    {
        std::size_t id = mElemComponents.size();
        mElemComponents.resize(id + 1);
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.resize(id + 1);
        }
        ++mElemCount;
        initDefaultComponents(id, id + 1);
        return static_cast<IndexType>(id);
    }

    /**
     * @brief Adds n elements to the Element Container.
     *
     * Returns the index of the first added element.
     *
     * @param[in] n: the number of elements to add.
     * @return The index of the first added element.
     */
    IndexType addElements(std::size_t n)
    {
        std::size_t baseId = mElemComponents.size();
        mElemComponents.resize(baseId + n);
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.resize(baseId + n);
        }
        mElemCount += n;
        initDefaultComponents(baseId, baseId + n);
        return static_cast<IndexType>(baseId);
    }

    /**
     * @brief Clears the Element container, deleting all the elements.
     *
     * The contained element components are actually removed from the container,
     * not only marked as deleted. Therefore, the container will have size 0
     * (`elementContainerSize() == 0`) after the call of this function.
     *
     * @warning Any index or reference to elements in this container stored in
     * this or other containers of the Mesh will become invalid.
     */
    void clearElements()
    {
        mElemComponents.clear();
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.clear();
        }
        mElemCount = 0;
    }

    /**
     * @brief Resizes the element container to contain `newSize` elements.
     *
     * If the new size is greater than the old one, new elements are added to
     * the container. If the new size is smaller than the old one, the container
     * is truncated to `newSize` elements (elements beyond `newSize` are
     * removed).
     *
     * @warning Any index to removed elements in the Mesh will be left
     * unchanged, and therefore will point to invalid elements.
     *
     * @param[in] newSize: the new size of the Element container.
     */
    void resizeElements(std::size_t newSize)
    {
        std::size_t oldSize = mElemComponents.size();
        mElemComponents.resize(newSize);
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.resize(newSize);
        }
        if (newSize > oldSize) {
            mElemCount += (newSize - oldSize);
            initDefaultComponents(oldSize, newSize);
        }
        else if (newSize < oldSize) {
            mElemCount = 0;
            for (std::size_t i = 0; i < newSize; ++i) {
                if (!isElementDeleted(i)) {
                    ++mElemCount;
                }
            }
        }
    }

    /**
     * @brief Reserves storage for a given number of elements in the container.
     *
     * Calling this function before multiple element additions avoids
     * unnecessary reallocations of the underlying parallel vectors.
     *
     * @param[in] capacity: the new capacity of the element container.
     */
    void reserveElements(std::size_t capacity)
    {
        mElemComponents.reserve(capacity);
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.reserve(capacity);
        }
    }

    /**
     * @brief Compacts the element container, keeping only the non-deleted
     * elements. References to elements will automatically change accordingly in
     * all the containers of the parent mesh if present.
     *
     * @return A vector that maps each old element index to its new index in the
     * compacted container (or NULL_INDEX if it was deleted).
     */
    std::vector<IndexType> compactElements()
    {
        std::vector<IndexType> newIndices = elementCompactIndices();
        if (!isElementContainerCompact()) {
            mElemComponents.compact(newIndices);
            if constexpr (comp::HasCustomComponents<Components>) {
                mCustomComponents.compact(newIndices);
            }
            assert(mElemComponents.size() == mElemCount);

            if constexpr (!std::is_void_v<ParentMesh>) {
                if (mParentMesh != nullptr) {
                    mParentMesh->template updateReferences<ELEMENT_ID>(
                        newIndices);
                }
            }
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
     * @warning If there were indices or references to the deleted element in
     * this or other containers, they will not be updated.
     *
     * @param[in] i: the index of the element that will be marked as deleted.
     */
    void deleteElement(IndexType i)
    {
        assert(i < size());
        auto e = element(i);
        if (!e.deleted()) {
            e.setDeleted(true);
            --mElemCount;
        }
    }

    /**
     * @brief Marks as deleted the given element.
     *
     * This member function does not perform any reallocation of the elements:
     * the deleted elements will stay in the Element Container, but will be
     * marked as deleted.
     *
     * Deleted elements are automatically jumped by the iterators provided by
     * the Element Container.
     *
     * @warning If there were indices or references to the deleted element in
     * this or other containers, they will not be updated.
     *
     * @param[in] e: the element that will be marked as deleted.
     */
    void deleteElement(const ElementType& e) { deleteElement(e.index()); }

    /**
     * @brief Adjusts the active element count when an element's deleted state
     * changes.
     *
     * @param[in] wasDeleted: Whether the element was previously marked as
     * deleted.
     * @param[in] isDeleted: Whether the element is now marked as deleted.
     */
    void adjustElemCountOnDeletedChange(
        bool wasDeleted,
        bool isDeleted) noexcept
    {
        if (wasDeleted != isDeleted) {
            if (isDeleted) {
                assert(mElemCount > 0);
                --mElemCount;
            }
            else {
                ++mElemCount;
            }
        }
    }

    /**
     * @brief Returns whether the container is compact, i.e. there are no
     * deleted elements.
     *
     * @return `true` if the container is compact, `false` otherwise.
     */
    bool isElementContainerCompact() const
    {
        return deletedElementCount() == 0;
    }

    /**
     * @brief This is a utility member function that returns the index of an
     * element if the container were compact, that is the number of
     * non-deleted elements before the element with the given index.
     *
     * Complexity: O(n), with n the number of elements in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of an element of the container.
     * @return The index that the element with index i would have if this
     * container were compact.
     */
    IndexType elementIndexIfCompact(IndexType i) const
    {
        assert(i < size());
        if (isElementDeleted(i)) {
            return NULL_INDEX;
        }
        if (isElementContainerCompact()) {
            return i;
        }
        else {
            IndexType cnt = 0;
            for (IndexType ii = 0; ii < i; ++ii) {
                if (!isElementDeleted(ii))
                    ++cnt;
            }
            return cnt;
        }
    }

    /**
     * @brief Returns a vector that tells, for each actual element index, the
     * new index that the element would have in a compacted container. For each
     * deleted element index, the value of the vector will be NULL_INDEX.
     *
     * This is useful if you need to know the indices of the elements that they
     * would have in a compact container, without considering the deleted ones.
     *
     * @return A vector containing, for each element index, its index if the
     * container were compact.
     */
    std::vector<IndexType> elementCompactIndices() const
    {
        std::vector<IndexType> newIndices(size());
        if (isElementContainerCompact()) {
            for (IndexType i = 0; i < size(); ++i) {
                newIndices[i] = i;
            }
            return newIndices;
        }

        IndexType currentIndex = 0;
        for (IndexType i = 0; i < size(); ++i) {
            if (!isElementDeleted(i)) {
                newIndices[i] = currentIndex++;
            }
            else {
                newIndices[i] = NULL_INDEX;
            }
        }
        return newIndices;
    }

    /**
     * @brief Returns an iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted elements of the
     * container. You can change this option by calling this function with
     * jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted elements.
     * @return An iterator to the first element of the container.
     */
    Iterator begin(bool jumpDeleted = true)
    {
        bool needJump = jumpDeleted && (size() != elementCount());
        return Iterator(*this, 0, needJump);
    }

    /**
     * @brief Returns an iterator to the end of the container.
     *
     * The jumpDeleted flag must match the one used for begin(), otherwise
     * distance computations between the two iterators (end() - begin()) and
     * backward iteration from end() are not consistent.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted elements.
     * @return An iterator to the end of the container.
     */
    Iterator end(bool jumpDeleted = true)
    {
        bool needJump = jumpDeleted && (size() != elementCount());
        return Iterator(*this, size(), needJump);
    }

    /**
     * @brief Returns a const iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted elements of the
     * container. You can change this option by calling this function with
     * jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted elements.
     * @return A const iterator to the first element of the container.
     */
    ConstIterator begin(bool jumpDeleted = true) const
    {
        bool needJump = jumpDeleted && (size() != elementCount());
        return ConstIterator(*this, 0, needJump);
    }

    /**
     * @brief Returns a const iterator to the end of the container.
     *
     * The jumpDeleted flag must match the one used for begin(), otherwise
     * distance computations between the two iterators (end() - begin()) and
     * backward iteration from end() are not consistent.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted elements.
     * @return A const iterator to the end of the container.
     */
    ConstIterator end(bool jumpDeleted = true) const
    {
        bool needJump = jumpDeleted && (size() != elementCount());
        return ConstIterator(*this, size(), needJump);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate
     * over the elements of the container, providing two member functions
     * begin() and end(), and allows access to the vector of components.
     *
     * This member function is very useful when you want to iterate over the
     * elements using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (ElementType e : m.elements()) {
     *     // do something with this element
     * }
     * @endcode
     *
     * The iterator used to iterate over elements is automatically initialized
     * to jump deleted elements of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted elements.
     * @return An object having begin() and end() functions, allowing to iterate
     * over the container.
     */
    auto elements(bool jumpDeleted = true)
    {
        return ElementContainerProxy<ElementContainer>(*this, jumpDeleted);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate
     * over the elements of the container, providing two member functions
     * begin() and end(), and allows access to the vector of components.
     *
     * This member function is very useful when you want to iterate over the
     * elements using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (ConstElementType e : m.elements()) {
     *     // do something with this element
     * }
     * @endcode
     *
     * The iterator used to iterate over elements is automatically initialized
     * to jump deleted elements of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted elements.
     * @return An object having begin() and end() functions, allowing to iterate
     * over the container.
     */
    auto elements(bool jumpDeleted = true) const
    {
        return ElementContainerProxy<const ElementContainer>(
            *this, jumpDeleted);
    }

    /**
     * @brief Returns a view object that allows to iterate over the elements of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the elements() function, this member function does not
     * automatically jump deleted elements, but it iterates over the
     * elements in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first element to be included in the
     * range. It must be less than or equal to elementContainerSize() and less
     * than or equal to the end index.
     * @param[in] end: the index of the last element to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * elementContainerSize(), it will be set to elementContainerSize().
     * @return A View object allowing to iterate over the given range of the
     * container.
     */
    View<Iterator> elements(IndexType begin, IndexType end = NULL_INDEX)
    {
        assert(begin <= elementContainerSize());
        if (end == NULL_INDEX || end > elementContainerSize()) {
            end = elementContainerSize();
        }
        assert(begin <= end);
        return View(Iterator(*this, begin, false), Iterator(*this, end, false));
    }

    /**
     * @brief Returns a view object that allows to iterate over the elements of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the elements() function, this member function does not
     * automatically jump deleted elements, but it iterates over the
     * elements in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first element to be included in the
     * range. It must be less than or equal to elementContainerSize() and less
     * than or equal to the end index.
     * @param[in] end: the index of the last element to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * elementContainerSize(), it will be set to elementContainerSize().
     * @return A View object allowing to iterate over the given range of the
     * container.
     */
    View<ConstIterator> elements(IndexType begin, IndexType end = NULL_INDEX)
        const
    {
        assert(begin <= elementContainerSize());
        if (end == NULL_INDEX || end > elementContainerSize()) {
            end = elementContainerSize();
        }
        assert(begin <= end);
        return View(
            ConstIterator(*this, begin, false),
            ConstIterator(*this, end, false));
    }

    // ### Enable - Disable

    /**
     * @brief Enables all optional components in the container.
     */
    void enableAllOptionalComponents()
    {
        enableAllOptionalComponentsImpl(Components {});
    }

    /**
     * @brief Disables all optional components in the container.
     */
    void disableAllOptionalComponents()
    {
        disableOptionalComponents(Components {});
    }

    /**
     * @brief Checks if the component with the given COMP_ID is available
     * (present at compile time and enabled at run time).
     *
     * Non-optional components are always available at compile-time (true).
     * Optional components return true if currently enabled, false otherwise.
     *
     * @tparam COMP_ID: The ID of the component to check.
     * @return `true` if currently available, `false` otherwise.
     */
    template<CompId COMP_ID>
    bool isComponentAvailable() const
    {
        if constexpr (comp::HasComponent<COMP_ID, Components>) {
            if constexpr (comp::HasOptionalComponent<COMP_ID, Components>) {
                constexpr uint N =
                    comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
                return mElemComponents.template isEnabled<N>();
            }
            else {
                return true;
            }
        }
        else {
            return false;
        }
    }

    /**
     * @brief Enables the component with the given COMP_ID.
     *
     * @tparam COMP_ID: The ID of the component to enable.
     */
    template<CompId COMP_ID>
    void enableOptionalComponent()
    {
        static_assert(
            comp::HasComponent<COMP_ID, Components>,
            "Component not found in container");
        if constexpr (comp::HasOptionalComponent<COMP_ID, Components>) {
            if (!isComponentAvailable<COMP_ID>()) {
                constexpr uint N =
                    comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
                mElemComponents.template enable<N>();
                using Comp = comp::ComponentTypeT<COMP_ID, Components>;
                if constexpr (comp::HasPolygonVertexReferences<Components>) {
                    if constexpr (comp::IsTiedToVertexCount<Comp>) {
                        for (auto e : elements(false)) {
                            e.Comp::resize(e.vertexCount());
                        }
                    }
                }
                if constexpr (comp::HasDefaultValue<Comp>) {
                    auto& vec = this->template vector<COMP_ID>();
                    std::fill(
                        vec.begin(), vec.end(), comp::defaultValue<Comp>());
                }
            }
        }
    }

    /**
     * @brief Disables the component with the given COMP_ID.
     * Note: Non-optional components cannot be disabled.
     *
     * @tparam COMP_ID: The ID of the component to disable.
     */
    template<CompId COMP_ID>
    void disableOptionalComponent()
    {
        static_assert(
            comp::HasComponent<COMP_ID, Components>,
            "Component not found in container");
        static_assert(
            comp::HasOptionalComponent<COMP_ID, Components>,
            "Cannot disable a non-optional component!");
        constexpr uint N =
            comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
        mElemComponents.template disable<N>();
    }

    /**
     * @brief Checks if the container is compact (no deleted elements).
     */
    bool isCompact() const { return deletedElementCount() == 0; }

    // Custom Components

    // --- Custom Components ---

    /**
     * @brief Returns true if the element has a custom component with the given
     * name, false otherwise.
     *
     * @param[in] name: The name of the custom component.
     * @return true if the custom component exists, false otherwise.
     */
    bool hasElemCustomComponent(const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.componentExists(name);
    }

    /**
     * @brief Returns a vector containing the names of all the custom components
     * associated to the elements of this container.
     *
     * @return A vector of strings containing the names of all custom
     * components.
     */
    std::vector<std::string> elemCustomComponentNames() const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.allComponentNames();
    }

    /**
     * @brief Returns true if the custom component of the given name is of the
     * type given as template argument, false otherwise.
     *
     * @tparam K: The type of the custom component to check.
     * @param[in] name: The name of the custom component.
     * @return true if the custom component is of type K, false otherwise.
     */
    template<typename K>
    bool isElemCustomComponentOfType(const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.template isComponentOfType<K>(name);
    }

    /**
     * @brief Returns the `std::type_index` of the custom component of the given
     * name.
     *
     * @param[in] name: The name of the custom component.
     * @return The `std::type_index` of the custom component.
     */
    std::type_index elemCustomComponentType(const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.componentType(name);
    }

    /**
     * @brief Returns a vector of strings containing the names of all the custom
     * components of type K.
     *
     * @tparam K: The type of the custom components.
     * @return A vector of strings containing the names of custom components of
     * type K.
     */
    template<typename K>
    std::vector<std::string> elemCustomComponentNamesOfType() const
        requires comp::HasCustomComponents<Components>
    {
        return mCustomComponents.template allComponentNamesOfType<K>();
    }

    /**
     * @brief Adds a new vector of custom components with the given name and
     * type. All existing elements are initialized with the default constructor
     * of K.
     *
     * @tparam K: The type of the custom component.
     * @param[in] name: The name of the custom component.
     */
    template<typename K>
    void addElemCustomComponent(const std::string& name)
        requires comp::HasCustomComponents<Components>
    {
        mCustomComponents.template addNewComponent<K>(name, size());
    }

    /**
     * @brief Deletes the custom component with the given name.
     *
     * @param[in] name: The name of the custom component to delete.
     */
    void deleteElemCustomComponent(const std::string& name)
        requires comp::HasCustomComponents<Components>
    {
        mCustomComponents.deleteComponent(name);
    }

    /**
     * @brief Returns a vector handle to access the custom component of type K
     * with the given name.
     *
     * @tparam K: The type of the custom component.
     * @param[in] name: The name of the custom component.
     * @return A CustomComponentVectorHandle<K>.
     */
    template<typename K>
    CustomComponentVectorHandle<K> customComponentVectorHandle(
        const std::string& name) requires comp::HasCustomComponents<Components>
    {
        return CustomComponentVectorHandle<K>(
            mCustomComponents.template componentVector<K>(name));
    }

    /**
     * @brief Returns a const vector handle to access the custom component of
     * type K with the given name.
     *
     * @tparam K: The type of the custom component.
     * @param[in] name: The name of the custom component.
     * @return A ConstCustomComponentVectorHandle<K>.
     */
    template<typename K>
    ConstCustomComponentVectorHandle<K> customComponentVectorHandle(
        const std::string& name) const
        requires comp::HasCustomComponents<Components>
    {
        return ConstCustomComponentVectorHandle<K>(
            mCustomComponents.template componentVector<K>(name));
    }

    /**
     * @brief Serializes all custom components of type K stored in this
     * container to the given output stream.
     *
     * @tparam K: The type of the custom components to serialize.
     * @param[out] os: The output stream to write to.
     */
    template<typename K>
    void serializeElemCustomComponentsOfType(std::ostream& os) const
        requires comp::HasCustomComponents<Components>
    {
        mCustomComponents.template serializeCustomComponentsOfType<K>(os);
    }

    /**
     * @brief Deserializes all custom components of type K from the given
     * input stream and stores them into this container.
     *
     * @tparam K: The type of the custom components to deserialize.
     * @param[in] is: The input stream to read from.
     */
    template<typename K>
    void deserializeElemCustomComponentsOfType(std::istream& is)
        requires comp::HasCustomComponents<Components>
    {
        mCustomComponents.template deserializeCustomComponentsOfType<K>(is);
    }

protected:
    /**
     * @brief Appends all elements from another container of the same type.
     *
     * @param[in] other: The other container whose elements will be appended.
     */
    void append(const ElementContainer& other)
    {
        appendImpl(Components {}, other);
    }

    /**
     * @brief Serializes the element container to the given output stream.
     *
     * Writes the active element count and the contents of the underlying
     * parallel vectors tuple (including size, component enable flags, and
     * data).
     *
     * @param[out] os: The output stream to write to.
     */
    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mElemCount);
        mElemComponents.serialize(os);
    }

    /**
     * @brief Deserializes the element container from the given input stream.
     *
     * Reads the active element count and the contents of the underlying
     * parallel vectors tuple. If custom components are supported, existing
     * custom components are cleared.
     *
     * @param[in] is: The input stream to read from.
     */
    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mElemCount);
        mElemComponents.deserialize(is);
        if constexpr (comp::HasCustomComponents<ComponentList>) {
            mCustomComponents.clear();
        }
    }

    /**
     * @brief Returns a reference to the enclosing parent mesh.
     *
     * @return A reference to the parent mesh.
     */
    decltype(auto) parentMesh() requires (!std::is_void_v<ParentMesh>)
    {
        assert(mParentMesh != nullptr && "Parent mesh pointer is null");
        return *mParentMesh;
    }

    /**
     * @brief Returns a const reference to the enclosing parent mesh.
     *
     * @return A const reference to the parent mesh.
     */
    decltype(auto) parentMesh() const requires (!std::is_void_v<ParentMesh>)
    {
        assert(mParentMesh != nullptr && "Parent mesh pointer is null");
        return std::as_const(*mParentMesh);
    }

    /**
     * @brief Sets the pointer to the enclosing parent mesh.
     *
     * @param[in] mesh: Pointer to the parent mesh.
     */
    void setParentMesh(ParentMesh* mesh) { mParentMesh = mesh; }

    /**
     * @brief Updates references to elements of type TARGET_ELEM_ID according to
     * the newIndices mapping vector.
     *
     * @tparam TARGET_ELEM_ID: The element ID whose references should be
     * updated.
     * @param[in] newIndices: A vector mapping old element indices to new ones.
     */
    template<ElemId TARGET_ELEM_ID>
    void updateReferences(const std::vector<IndexType>& newIndices)
    {
        if constexpr (ElementType::template referencesElement<
                          TARGET_ELEM_ID>()) {
            for (std::size_t i = 0; i < size(); ++i) {
                if (!isElementDeleted(i)) {
                    element(i).template updateReferences<TARGET_ELEM_ID>(
                        newIndices);
                }
            }
        }
    }

    /**
     * @brief Shifts references to elements of type TARGET_ELEM_ID starting from
     * a given index by an offset.
     *
     * @tparam TARGET_ELEM_ID: The element ID whose references should be
     * shifted.
     * @param[in] startFrom: The starting index.
     * @param[in] offset: The offset value to add to the references.
     */
    template<ElemId TARGET_ELEM_ID>
    void shiftReferences(IndexType startFrom, IndexType offset)
    {
        if constexpr (ElementType::template referencesElement<
                          TARGET_ELEM_ID>()) {
            if (offset != 0) {
                for (std::size_t i = startFrom; i < size(); ++i) {
                    if (!isElementDeleted(i)) {
                        element(i).template shiftReferences<TARGET_ELEM_ID>(
                            offset);
                    }
                }
            }
        }
    }

    /**
     * @brief Updates references to the elements of this container type across
     * all containers of the mesh according to the newIndices mapping vector.
     *
     * This function is useful when elements of this container were compacted,
     * permuted, or deleted, and you want to update all references stored in the
     * mesh to point to their new indices.
     *
     * @param[in] newIndices: A vector mapping old element indices to new ones
     * (or NULL_INDEX if the element was removed).
     */
    void updateElementReferences(const std::vector<IndexType>& newIndices)
    {
        if constexpr (!std::is_void_v<ParentMesh>) {
            if (mParentMesh != nullptr) {
                mParentMesh->template updateReferences<ELEMENT_ID>(newIndices);
            }
        }
    }

    /**
     * @brief Enables in this container all the optional components that are
     * available and enabled in the given other container.
     *
     * @tparam OtherContainer: The type of the container to check against.
     * @param[in] other: The other container.
     */
    template<typename OtherContainer>
    void enableSameOptionalComponentsOf(const OtherContainer& other)
    {
        enableSameOptionalComponentsOfImpl(Components {}, other);
    }

    /**
     * @brief Imports all elements from the corresponding container of the
     * other mesh into this container.
     *
     * @tparam OtherMesh: The type of the other mesh.
     * @param[in] m: The other mesh to import from.
     */
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        if constexpr (OtherMesh::template hasContainerOf<ELEMENT_ID>()) {
            const auto& c = m.template container<ELEMENT_ID>();
            clearElements();
            const std::size_t n = c.size();
            addElements(n);
            for (IndexType i = 0; i < n; ++i) {
                element(i).importFrom(c.element(i));
            }
            mElemCount = c.elementCount();
            if constexpr (
                comp::HasCustomComponents<Components> &&
                comp::HasCustomComponents<
                    typename std::remove_cvref_t<decltype(c)>::Components>) {
                mCustomComponents = c.mCustomComponents;
            }
        }
    }

    /**
     * @brief Returns a reference to the custom component of type K at index i.
     *
     * @tparam K: The type of the custom component.
     * @param[in] name: The name of the custom component.
     * @param[in] i: The index of the element.
     * @return A reference to the custom component value.
     */
    template<typename K>
    K& elemCustomComponent(const std::string& name, IndexType i)
        requires comp::HasCustomComponents<Components>
    {
        return std::any_cast<K&>(
            mCustomComponents.template componentVector<K>(name)[i]);
    }

    /**
     * @brief Returns a const reference to the custom component of type K at
     * index i.
     *
     * @tparam K: The type of the custom component.
     * @param[in] name: The name of the custom component.
     * @param[in] i: The index of the element.
     * @return A const reference to the custom component value.
     */
    template<typename K>
    const K& elemCustomComponent(const std::string& name, IndexType i) const
        requires comp::HasCustomComponents<Components>
    {
        return std::any_cast<const K&>(
            mCustomComponents.template componentVector<K>(name)[i]);
    }

private:
    // convenience function to get the size of the element container
    std::size_t size() const { return elementContainerSize(); }

    // --- signle component vector accessors ---

    template<CompId COMP_ID>
    auto& vector()
    {
        static_assert(
            COMP_ID != CompId::CUSTOM_COMPONENTS,
            "CustomComponents do not have a column in the parallel vectors "
            "tuple.");
        constexpr uint N =
            comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
        return mElemComponents.template get<N>();
    }

    template<CompId COMP_ID>
    const auto& vector() const
    {
        static_assert(
            COMP_ID != CompId::CUSTOM_COMPONENTS,
            "CustomComponents do not have a column in the parallel vectors "
            "tuple.");
        constexpr uint N =
            comp::IndexOfCompId<COMP_ID, VectorComponents>::value;
        return mElemComponents.template get<N>();
    }

    // --- TTVC (Tied To Vertex Count) synchronization helpers ---

    template<typename... Comps>
    void resizeTTVCComponentsImpl(
        vcl::TypeWrapper<Comps...>,
        IndexType i,
        uint      n)
    {
        assert(i < size());
        auto p          = element(i);
        auto resizeComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::resize(n);
                }
            }
        };
        (resizeComp.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void pushBackTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i)
    {
        assert(i < size());
        auto p            = element(i);
        auto pushBackComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::pushBack();
                }
            }
        };
        (pushBackComp.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void insertTTVCComponentsImpl(
        vcl::TypeWrapper<Comps...>,
        IndexType i,
        uint      pos)
    {
        assert(i < size());
        auto p          = element(i);
        auto insertComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::insert(pos);
                }
            }
        };
        (insertComp.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void eraseTTVCComponentsImpl(
        vcl::TypeWrapper<Comps...>,
        IndexType i,
        uint      pos)
    {
        assert(i < size());
        auto p         = element(i);
        auto eraseComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::erase(pos);
                }
            }
        };
        (eraseComp.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void clearTTVCComponentsImpl(vcl::TypeWrapper<Comps...>, IndexType i)
    {
        assert(i < size());
        auto p         = element(i);
        auto clearComp = [&]<typename Comp>() {
            if constexpr (comp::IsTiedToVertexCount<Comp>) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    p.Comp::clear();
                }
            }
        };
        (clearComp.template operator()<Comps>(), ...);
    }

    void resizeTTVCComponents(IndexType i, uint n)
    {
        resizeTTVCComponentsImpl(Components {}, i, n);
    }

    void pushBackTTVCComponents(IndexType i)
    {
        pushBackTTVCComponentsImpl(Components {}, i);
    }

    void insertTTVCComponents(IndexType i, uint pos)
    {
        insertTTVCComponentsImpl(Components {}, i, pos);
    }

    void eraseTTVCComponents(IndexType i, uint pos)
    {
        eraseTTVCComponentsImpl(Components {}, i, pos);
    }

    void clearTTVCComponents(IndexType i)
    {
        clearTTVCComponentsImpl(Components {}, i);
    }

    // --- Optional Components helpers ---

    template<typename... Comps>
    void enableNonOptionalComponents(vcl::TypeWrapper<Comps...>)
    {
        auto enableIfNotOpt = [&]<typename Comp>() {
            if constexpr (
                detail::HasVectorColumnPred<Comp>::value &&
                !comp::IsOptionalComponent<Comp>) {
                constexpr uint N = comp::
                    IndexOfCompId<Comp::COMPONENT_ID, VectorComponents>::value;
                mElemComponents.template enable<N>();
            }
        };
        (enableIfNotOpt.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void disableOptionalComponents(vcl::TypeWrapper<Comps...>)
    {
        auto disableIfOpt = [&]<typename Comp>() {
            if constexpr (comp::IsOptionalComponent<Comp>) {
                constexpr uint N = comp::
                    IndexOfCompId<Comp::COMPONENT_ID, VectorComponents>::value;
                mElemComponents.template disable<N>();
            }
        };
        (disableIfOpt.template operator()<Comps>(), ...);
    }

    template<typename... Comps>
    void enableAllOptionalComponentsImpl(vcl::TypeWrapper<Comps...>)
    {
        auto enableIfOpt = [&]<typename Comp>() {
            if constexpr (comp::IsOptionalComponent<Comp>) {
                enableOptionalComponent<Comp::COMPONENT_ID>();
            }
        };
        (enableIfOpt.template operator()<Comps>(), ...);
    }

    // --- Default Component Values helpers ---

    template<typename... Comps>
    void initDefaultComponentsImpl(
        vcl::TypeWrapper<Comps...>,
        std::size_t begin,
        std::size_t end)
    {
        auto initComp = [&]<typename Comp>() {
            if constexpr (detail::HasVectorColumnPred<Comp>::value) {
                if constexpr (comp::HasDefaultValue<Comp>) {
                    if (this->template isComponentAvailable<
                            Comp::COMPONENT_ID>()) {
                        auto& vec = this->template vector<Comp::COMPONENT_ID>();
                        std::fill(
                            vec.begin() + begin,
                            vec.begin() + end,
                            comp::defaultValue<Comp>());
                    }
                }
            }
        };
        (initComp.template operator()<Comps>(), ...);
    }

    void initDefaultComponents(std::size_t begin, std::size_t end)
    {
        if (begin < end) {
            initDefaultComponentsImpl(Components {}, begin, end);
        }
    }

    template<typename... Comps, typename OtherContainer>
    void enableSameOptionalComponentsOfImpl(
        vcl::TypeWrapper<Comps...>,
        const OtherContainer& other)
    {
        auto enableIfAvailableInOther = [&]<typename Comp>() {
            if constexpr (comp::IsOptionalComponent<Comp>) {
                if (!this->template isComponentAvailable<
                        Comp::COMPONENT_ID>()) {
                    if constexpr (comp::HasComponent<
                                      Comp::COMPONENT_ID,
                                      OtherContainer>) {
                        if (other.template isComponentAvailable<
                                Comp::COMPONENT_ID>()) {
                            this->template enableOptionalComponent<
                                Comp::COMPONENT_ID>();
                        }
                    }
                }
            }
        };

        (enableIfAvailableInOther.template operator()<Comps>(), ...);
    }

    // --- Append Helpers ---

    template<typename... Comps>
    void appendImpl(vcl::TypeWrapper<Comps...>, const ElementContainer& other)
    {
        const std::size_t on = other.size();
        if (on == 0) {
            return;
        }

        const std::size_t n = size();

        // 1. Allocate space for 'on' new elements across all active component
        // vectors.
        //    Note: addElements(on) temporarily increments mElemCount by on
        //    (assuming all valid).
        addElements(on);

        // 2. Fast-copy component data: for each component enabled in both
        // containers,
        //    copy elements from other's vector into this vector starting at
        //    offset n.
        auto copyComponent = [&]<typename Comp>() {
            if constexpr (Comp::COMPONENT_ID != CompId::CUSTOM_COMPONENTS) {
                if (this->template isComponentAvailable<Comp::COMPONENT_ID>() &&
                    other.template isComponentAvailable<Comp::COMPONENT_ID>()) {
                    auto& thisVec = this->template vector<Comp::COMPONENT_ID>();
                    const auto& otherVec =
                        other.template vector<Comp::COMPONENT_ID>();
                    std::copy(
                        otherVec.begin(), otherVec.end(), thisVec.begin() + n);
                }
            }
        };

        (copyComponent.template operator()<Comps>(), ...);

        // 3. Preserve TTVC (Tied-To-Vertex-Count) invariant on polygon meshes:
        //    If an optional TTVC component (e.g., AdjacentFaces, AdjacentEdges,
        //    Wedges, PolygonBitFlags) is enabled in 'this' container but was
        //    disabled in 'other', addElements(on) initialized inner vectors to
        //    size 0. Since the vertex references were copied in step 2, we now
        //    have the correct vertexCount() for each appended polygon and can
        //    resize the missing TTVC component to match it.
        if constexpr (comp::HasPolygonVertexReferences<Components>) {
            auto resizeTTVCIfMissingInOther = [&]<typename Comp>() {
                if constexpr (comp::IsTiedToVertexCount<Comp>) {
                    if (this->template isComponentAvailable<
                            Comp::COMPONENT_ID>() &&
                        !other.template isComponentAvailable<
                            Comp::COMPONENT_ID>()) {
                        for (std::size_t i = n; i < n + on; ++i) {
                            auto p = element(i);
                            p.Comp::resize(p.vertexCount());
                        }
                    }
                }
            };

            (resizeTTVCIfMissingInOther.template operator()<Comps>(), ...);
        }

        // 4. Append custom dynamic components (if supported).
        if constexpr (comp::HasCustomComponents<Components>) {
            mCustomComponents.append(other.mCustomComponents, n, on);
        }

        // 5. Correct the valid element counter:
        //    addElements(on) assumed all 'on' elements were valid (+on).
        //    We deduct 'on' and add other.elementCount() so deleted elements in
        //    'other' are properly accounted for.
        mElemCount = (mElemCount - on) + other.elementCount();
    }

    // --- Friend classes ---

    // to access to the parallel vectors tuple of the container
    template<CompId, comp::ComponentData, bool>
    friend class comp::Component;

    // to access to the custom components map of the container
    template<CompId, std::unsigned_integral, ElemId, int, bool, bool>
    friend class comp::ContainerReferencesComponent;

    // to access to the custom components map of the container
    friend class comp::CustomComponents;

    // to access the parallel vectors tuple of the container
    template<typename>
    friend class ElementContainerProxy;

    // to allow ElementProxy to access component vectors for componentValue
    template<ElemId, typename, typename>
    friend class vcl::ElementProxy;

    // to access a specific component vector
    template<CompId, typename>
    friend class ComponentIterator;

    // to access a specific component vector
    template<CompId, typename>
    friend class ComponentView;

    // to allow TTVC resize, pushBack, insert, erase, clear members
    template<int, std::unsigned_integral>
    friend class comp::VertexReferences;

    // to allow access to ElementContainers with different template parameters
    template<
        typename,
        typename,
        std::unsigned_integral,
        template<typename...> class>
    friend class ElementContainer;
};

/**
 * @brief Concept that checks whether a type T is an ElementContainer.
 *
 * @tparam T: The type to check.
 */
template<typename T>
concept ElementContainerConcept = requires {
    { std::remove_cvref_t<T>::ELEMENT_ID } -> std::convertible_to<ElemId>;
    typename std::remove_cvref_t<T>::IndexType;
    typename std::remove_cvref_t<T>::ElementType;
    typename std::remove_cvref_t<T>::ConstElementType;
};

/**
 * @brief Concept that checks whether a type T is an ElementContainer of
 * Vertices.
 */
template<typename T>
concept VertexContainerConcept =
    ElementContainerConcept<T> &&
    (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::VERTEX);

/**
 * @brief Concept that checks whether a type T is an ElementContainer of Faces.
 */
template<typename T>
concept FaceContainerConcept =
    ElementContainerConcept<T> &&
    (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::FACE);

/**
 * @brief Concept that checks whether a type T is an ElementContainer of Edges.
 */
template<typename T>
concept EdgeContainerConcept =
    ElementContainerConcept<T> &&
    (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::EDGE);

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_BASE_ELEMENT_CONTAINER_H
