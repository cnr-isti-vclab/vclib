// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEMENTS_BASE_ELEMENT_PROXY_H
#define VCL_MESH_ELEMENTS_BASE_ELEMENT_PROXY_H

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/base/component_predicates.h>
#include <vclib/mesh/components/bit_flags.h>
#include <vclib/mesh/elements/base/macros.h>

#include <vclib/base.h>

#include <compare>
#include <concepts>
#include <limits>
#include <type_traits>
#include <utility>

namespace vcl {

namespace mesh {
template<
    typename ComponentList,
    typename ParentMesh,
    std::unsigned_integral Index,
    template<typename...> class Element>
class ElementContainer;

template<typename ContainerType>
class ElementIterator;
} // namespace mesh

template<
    ElemId ELEM_ID,
    typename ContainerType,
    typename ComponentListWrapper =
        typename std::remove_cv_t<ContainerType>::Components>
class ElementProxy;

/**
 * @brief Lightweight proxy object to access an element's data in the Structure
 * of Arrays (SoA) ElementContainer.
 *
 * The ElementProxy acts as an Object-Oriented view over the Structure of
 * Arrays. It is meant to be instantiated on the fly (passed by value) and
 * provides access to the components' data by delegating the calls to the
 * underlying Container.
 *
 * It will provide all the member functions of the components in the
 * ComponentListWrapper.
 */
template<ElemId ELEM_ID, typename ContainerType, typename... Comps>
class ElementProxy<ELEM_ID, ContainerType, vcl::TypeWrapper<Comps...>> :
        public Comps...
{
public:
    using Components = vcl::TypeWrapper<Comps...>;
    using IndexType  = typename std::remove_cv_t<ContainerType>::IndexType;
    using ParentMeshType =
        typename std::remove_cv_t<ContainerType>::ParentMeshType;

    /**
     * @brief The null/invalid sentinel value for this element's index type.
     */
    static constexpr IndexType NULL_INDEX = vcl::NULL_INDEX<IndexType>;

    static_assert(
        comp::HasBitFlags<Components>,
        "An Element must have the BitFlags component "
        "(vcl::comp::BitFlags).");

private:
    ContainerType* mContainer = nullptr;
    IndexType      mIndex     = NULL_INDEX;

public:
    static constexpr ElemId ELEMENT_ID = ELEM_ID;

    template<ElemId TARGET_ELEM_ID>
    static constexpr bool referencesElement()
    {
        return (comp::HasReferencesTo<TARGET_ELEM_ID, Comps> || ...);
    }

    // Methods accessed by the components ("deducing this"
    // self.parentContainer()).
    // TODO: When C++26 variadic friends are fully supported on MSVC, move
    // parentContainer() to protected so that ElementProxy is fully encapsulated
    ContainerType& parentContainer() { return *mContainer; }

    const ContainerType& parentContainer() const { return *mContainer; }

    /**
     * @brief Returns a reference to the enclosing parent mesh.
     */
    decltype(auto) parentMesh(this auto&& self)
        requires (!std::is_void_v<ParentMeshType>)
    {
        assert(self.isValid() && "Element proxy is invalid");
        using SelfType = std::remove_reference_t<decltype(self)>;
        if constexpr (std::is_const_v<SelfType> || std::is_const_v<ContainerType>) {
            return std::as_const(*self.mContainer).parentMesh();
        }
        else {
            return self.mContainer->parentMesh();
        }
    }

    /**
     * @brief Returns the index of this element in the container.
     */
    IndexType index() const { return mIndex; }

    /**
     * @brief Checks if this proxy points to a valid element.
     * A proxy is considered invalid if it points to a valid container and its
     * index is the NULL_INDEX value. This naturally represents missing topology
     * references (e.g., boundary faces).
     */
    bool isValid() const
    {
        return mContainer && mIndex != NULL_INDEX;
    }

    /**
     * @brief Implicit conversion to bool to check if the proxy is valid.
     */
    explicit operator bool() const { return isValid(); }

    /**
     * @brief Checks if two element proxies refer to the same element.
     * Two invalid proxies are considered equal (representing null/missing
     * element).
     */
    template<typename OtherContainer>
        requires std::same_as<
            std::remove_const_t<ContainerType>,
            std::remove_const_t<OtherContainer>>
    bool operator==(
        const ElementProxy<ELEM_ID, OtherContainer, vcl::TypeWrapper<Comps...>>&
            other) const
    {
        if (!isValid() && !other.isValid()) {
            return true;
        }
        return mContainer == other.mContainer && mIndex == other.mIndex;
    }

    /**
     * @brief Three-way comparison for element proxies.
     * Two invalid proxies are considered equal.
     */
    template<typename OtherContainer>
        requires std::same_as<
            std::remove_const_t<ContainerType>,
            std::remove_const_t<OtherContainer>>
    auto operator<=>(
        const ElementProxy<ELEM_ID, OtherContainer, vcl::TypeWrapper<Comps...>>&
            other) const
    {
        if (!isValid() && !other.isValid()) {
            return std::strong_ordering::equal;
        }
        if (!isValid()) {
            return std::strong_ordering::less;
        }
        if (!other.isValid()) {
            return std::strong_ordering::greater;
        }
        if (mContainer != other.mContainer) {
            return mContainer <=> other.mContainer;
        }
        return mIndex <=> other.mIndex;
    }

    /**
     * @brief Compares an element proxy with nullptr.
     * Returns true if the proxy is invalid.
     */
    bool operator==(std::nullptr_t) const { return !isValid(); }

    /**
     * @brief Swaps this element proxy handle with another one.
     *
     * @note This swaps only the lightweight proxy handles (container pointer
     * and element index), not the underlying element data in the container.
     */
    void swap(ElementProxy& other) noexcept
    {
        std::swap(mContainer, other.mContainer);
        std::swap(mIndex, other.mIndex);
    }

    /**
     * @brief Swaps two element proxy handles.
     *
     * @note This swaps only the lightweight proxy handles (container pointer
     * and element index), not the underlying element data in the container.
     */
    friend void swap(ElementProxy& a, ElementProxy& b) noexcept
    {
        a.swap(b);
    }

    /**
     * @brief Checks if the component with COMP_ID is available (present and enabled).
     */
    template<CompId COMP_ID>
    bool isComponentAvailable() const
    {
        return mContainer && mContainer->template isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Returns a reference to the value of the component with COMP_ID.
     *
     * Only attribute components (such as POSITION, NORMAL, COLOR, QUALITY,
     * TEX_COORD, TANGENT, PRINCIPAL_CURVATURE, MATERIAL_INDEX) can be accessed
     * through this function. Structural, topological, and TTVC components
     * (like VERTEX_REFERENCES, ADJACENT_*, BIT_FLAGS, WEDGES) are excluded
     * at compile time to protect container invariants.
     *
     * Constness is propagated automatically from the ElementProxy / Container.
     */
    template<CompId COMP_ID>
    decltype(auto) componentValue(this auto&& self)
        requires (comp::HasComponent<COMP_ID, Components> &&
                  comp::IsAttributeComponent<COMP_ID>)
    {
        assert(self.isValid());
        assert(
            self.template isComponentAvailable<COMP_ID>() &&
            "Component is not available in this element!");
        return self.parentContainer().template vector<COMP_ID>()[self.index()];
    }

    VCL_ALL_ELEMENT_COMPONENT_METHODS

    /**
     * @brief Imports the components of the other element into this element.
     *
     * Iterates over all components that compose this element and calls their
     * importFrom method on the other element if available/enabled.
     *
     * @param[in] other: The other element to import from.
     * @param[in] importRefs: Boolean flag indicating if references (e.g. vertex
     * indices) should be imported (default: true).
     */
    template<typename OtherElement>
    void importFrom(const OtherElement& other, bool importRefs = true)
        requires (!std::is_const_v<ContainerType>)
    {
        static_assert(
            ELEMENT_ID == OtherElement::ELEMENT_ID,
            "Cannot import from an element of a different type.");

        auto importSingleComponent = [&]<typename Comp>() {
            if constexpr (requires { this->Comp::importFrom(other, importRefs); }) {
                if constexpr (requires { Comp::COMPONENT_ID; }) {
                    if (this->template isComponentAvailable<Comp::COMPONENT_ID>()) {
                        this->Comp::importFrom(other, importRefs);
                    }
                }
                else {
                    this->Comp::importFrom(other, importRefs);
                }
            }
        };

        (importSingleComponent.template operator()<Comps>(), ...);
    }

protected:
    ElementProxy() = default;

    ElementProxy(ContainerType& container, IndexType index) :
            mContainer(&container), mIndex(index)
    {
    }

    // Implicit conversion constructor to allow conversion from a mutable
    // ElementProxy to a const ElementProxy.
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    ElementProxy(
        const ElementProxy<
            ELEM_ID,
            OtherContainerType,
            vcl::TypeWrapper<Comps...>>& other) :
            mContainer(other.mContainer), mIndex(other.mIndex)
    {
    }

    template<ElemId TARGET_ELEM_ID>
    void updateReferences(const std::vector<IndexType>& newIndices)
    {
        auto updateIfMatching = [&]<typename Comp>() {
            if constexpr (
                comp::HasReferencesTo<TARGET_ELEM_ID, Comp>) {
                if constexpr (requires { Comp::COMPONENT_ID; }) {
                    if (this->template isComponentAvailable<
                            Comp::COMPONENT_ID>()) {
                        this->Comp::template updateReferences<TARGET_ELEM_ID>(
                            newIndices);
                    }
                }
                else {
                    this->Comp::template updateReferences<TARGET_ELEM_ID>(
                        newIndices);
                }
            }
        };

        (updateIfMatching.template operator()<Comps>(), ...);
    }

    template<ElemId TARGET_ELEM_ID>
    void shiftReferences(IndexType offset)
    {
        auto shiftIfMatching = [&]<typename Comp>() {
            if constexpr (
                comp::HasReferencesTo<TARGET_ELEM_ID, Comp>) {
                if constexpr (requires { Comp::COMPONENT_ID; }) {
                    if (this->template isComponentAvailable<
                            Comp::COMPONENT_ID>()) {
                        this->Comp::template shiftReferences<TARGET_ELEM_ID>(
                            offset);
                    }
                }
                else {
                    this->Comp::template shiftReferences<TARGET_ELEM_ID>(
                        offset);
                }
            }
        };

        (shiftIfMatching.template operator()<Comps>(), ...);
    }

private:
    /// friend classes declarations

    template<ElemId, typename, typename>
    friend class ElementProxy;

    template<
        typename,
        typename,
        std::unsigned_integral,
        template<typename...> class>
    friend class mesh::ElementContainer;

    template<typename>
    friend class mesh::ElementIterator;

    // TODO: When C++26 (P2893 Variadic Friends) is fully supported across all
    // compilers (including MSVC), uncomment:
    // friend Comps...;
};

/**
 * @brief Concept that checks whether a type T is an Element proxy.
 *
 * An element proxy must have an ELEMENT_ID, an associated Components list,
 * an IndexType, and member functions index() and isValid().
 */
template<typename T>
concept ElementConcept = requires (T&& obj) {
    { std::remove_cvref_t<T>::ELEMENT_ID } -> std::convertible_to<ElemId>;
    typename std::remove_cvref_t<T>::Components;
    typename std::remove_cvref_t<T>::IndexType;
    { obj.index() } -> std::same_as<typename std::remove_cvref_t<T>::IndexType>;
    { obj.isValid() } -> std::same_as<bool>;
};

} // namespace vcl

namespace std {

template<vcl::ElementConcept Element>
struct hash<Element>
{
    std::size_t operator()(const Element& ep) const
    {
        if (!ep.isValid()) {
            return 0;
        }
        std::size_t seed = 0;
        vcl::hashCombine(seed, ep.index());
        return seed;
    }
};

} // namespace std

#endif // VCL_MESH_ELEMENTS_BASE_ELEMENT_PROXY_H
