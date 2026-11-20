%HEADER%

#ifndef VCL_MESH_ELEMENTS_%EL_U%_H
#define VCL_MESH_ELEMENTS_%EL_U%_H

#include "%EL%_components.h"

%REQUIRED_INCLUDES%#include <vclib/mesh/elements/base/base.h>
#include <vclib/mesh/elements/base/element_proxy.h>

#include <ranges>

namespace vcl {

/**
 * @brief The %EL_UC% class represents the runtime proxy (view) of %EL_A%
 * %EL_UC% element in a mesh container.
 *
 * It inherits from ElementProxy and provides object-oriented access to
 * the per-%EL_TXT% components stored in the container's Structure of Arrays.
 *
 * @tparam ContainerType: The type of the container (mutable or const).
 *
 * @ingroup elements
 */
template<typename ContainerType>
class %EL_UC% : public ElementProxy<ElemId::%EL_U%, ContainerType>
{
    using Base = ElementProxy<ElemId::%EL_U%, ContainerType>;

public:
    using typename Base::Components;
    using typename Base::IndexType;
    using typename Base::ParentMeshType;
    using Base::NULL_INDEX;

//@if vertex_references
    static constexpr int VERTEX_COUNT = comp::
        ComponentTypeT<CompId::VERTEX_REFERENCES, Components>::VERTEX_COUNT;

//@endif
%CLASS_STATIC_ASSERTS%

    %EL_UC%() = default;

    // Implicit conversion constructor from mutable %EL_UC% to const %EL_UC%
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    %EL_UC%(const %EL_UC%<OtherContainerType>& other) : Base(other)
    {
    }

    /**
     * @brief Swaps this %EL_UC% proxy handle with another one.
     */
    void swap(%EL_UC%& other) noexcept { Base::swap(other); }

    /**
     * @brief Swaps two %EL_UC% proxy handles.
     */
    friend void swap(%EL_UC%& a, %EL_UC%& b) noexcept { a.swap(b); }

protected:
    %EL_UC%(ContainerType& container, IndexType index) : Base(container, index) {}

private:
    /// friend classes declarations

    template<
        typename,
        typename,
        std::unsigned_integral,
        template<typename...> class>
    friend class mesh::ElementContainer;

    template<typename>
    friend class mesh::ElementIterator;

    template<typename>
    friend class %EL_UC%;
};

/**
 * @brief The %EL_UC%Components struct defines a type-wrapper for the components
 * of %EL_A% %EL_UC%, checking at compile-time that the required components
 * are present: %REQUIRED_TEXT%.
 *
 * @tparam Comps: The components that compose the %EL_UC%.
 *
 * @ingroup elements
 */
template<typename... Comps>
struct %EL_UC%Components : vcl::TypeWrapper<Comps...>
{
%COMPONENTS_STATIC_ASSERTS%
};

template<typename... Comps>
struct %EL_UC%Components<vcl::TypeWrapper<Comps...>> : %EL_UC%Components<Comps...>
{
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T is %EL_A% %EL_UC% element.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept %EL_UC%Concept =
    ElementConcept<T> &&
    (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::%EL_U%)%ELEMENT_CONCEPT_CLAUSES%;

/**
 * @brief Concept that checks whether a type T is a valid component list for %EL_A%
 * %EL_UC%.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept HasRequired%EL_UC%Components =
    %REQUIRED_CONCEPT_CLAUSES%;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_%EL_U%_H
