%HEADER%

#ifndef VCL_MESH_CONTAINERS_%EL_U%_CONTAINER_H
#define VCL_MESH_CONTAINERS_%EL_U%_CONTAINER_H

#include <vclib/mesh/components/base/component_predicates.h>
#include <vclib/mesh/containers/base/element_container.h>
#include <vclib/mesh/containers/base/macros.h>
#include <vclib/mesh/elements/%EL%.h>
#include <vclib/mesh/elements/%EL%_components.h>

#include <vclib/base.h>

namespace vcl::mesh {

/**
 * @brief %EL_UC%Container stores the per-%EL_TXT% data in a Structure of Arrays (SoA),
 * and it is meant to be used as a template parameter of the Mesh class.
 *
 * It accepts the %EL_UC% component list (satisfying HasRequired%EL_UC%Components),
 * instantiates the underlying storage and exposes the %EL_UC% element view.
 *
 * @tparam CompList: The %EL_TXT% components type (e.g. %EL_UC%Components<BitFlags,
 * %EL_UC%VertexReferences<uint>>).
 * @tparam Index: The index type (default uint).
 * @tparam ParentMesh: The parent mesh type. On Mesh instantiation, this is
 * automatically set to the Mesh type. Default is void.
 *
 * @ingroup containers
 */
template<
    HasRequired%EL_UC%Components CompList,
    std::unsigned_integral    Index = uint,
    typename ParentMesh             = void>
class %EL_UC%Container :
    public ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        %EL_UC%>
{
    using Base = ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        %EL_UC%>;

public:
    using %EL_UC%           = typename Base::ElementType;
    using Const%EL_UC%      = typename Base::ConstElementType;
    using %EL_UC%Components = typename Base::Components;
    using Components     = %EL_UC%Components;
    using Base::NULL_INDEX;

    // Rebind idiom to allow changing the ParentMesh type while keeping the same
    // component list and index type.
    template<typename NewParent>
    using Rebind = %EL_UC%Container<CompList, Index, NewParent>;

    /**
     * @brief Empty constructor that creates an empty container of %EL_UC_PL%.
     */
    %EL_UC%Container() = default;

    friend void swap(%EL_UC%Container& a, %EL_UC%Container& b) noexcept
    {
        a.swap(b);
    }

    /**
     * @brief Returns the %EL_UC% at the i-th position in the %EL_UC% Container of
     * the Mesh, which will be the %EL_TXT% having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * %EL_C%ContainerSize(), this function will return a valid %EL_UC%
     * (note that the %EL_UC% may have been flagged as deleted).
     *
     * @param[in] i: the index of the %EL_TXT% that will be returned.
     *
     * @return The i-th %EL_UC% of the %EL_UC% Container.
     */
    %EL_UC% %EL_C%(Index i) { return Base::element(i); }

    /**
     * @brief Returns the const %EL_UC% at the i-th position in the %EL_UC%
     * Container of the Mesh, which will be the %EL_TXT% having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * %EL_C%ContainerSize(), this function will return a valid Const%EL_UC%
     * (note that the %EL_UC% may have been flagged as deleted).
     *
     * @param[in] i: the index of the %EL_TXT% that will be returned.
     *
     * @return The i-th const %EL_UC% of the %EL_UC% Container.
     */
    Const%EL_UC% %EL_C%(Index i) const { return Base::element(i); }

    /**
     * @brief Returns the number of **non-deleted** %EL_TXT_PL% contained in the
     * %EL_UC% container of the Mesh.
     *
     * If %EL_C%Count() != %EL_C%ContainerSize(), it means that there are some
     * %EL_TXT_PL% that are flagged as deleted.
     *
     * @return The number of non-deleted %EL_TXT_PL% of the Mesh.
     */
    std::size_t %EL_C%Count() const { return Base::elementCount(); }

    /**
     * @brief Returns the number of %EL_TXT_PL% (also deleted) contained in the
     * %EL_UC% container of the Mesh.
     *
     * If %EL_C%Count() != %EL_C%ContainerSize(), it means that there are some
     * %EL_TXT_PL% that are flagged as deleted.
     *
     * @return The number of all the %EL_TXT_PL% contained in the Mesh.
     */
    std::size_t %EL_C%ContainerSize() const
    {
        return Base::elementContainerSize();
    }

    /**
     * @brief Returns the number of deleted %EL_TXT_PL% in the %EL_UC% container,
     * that is %EL_C%ContainerSize() - %EL_C%Count().
     *
     * @return The number of deleted %EL_TXT_PL% in the container.
     */
    std::size_t deleted%EL_UC%Count() const { return Base::deletedElementCount(); }

    /**
     * @brief Adds a new %EL_TXT% into the %EL_UC% container, returning the index of
     * the added %EL_TXT%.
     *
     * @return The index of the new %EL_TXT%.
     */
    Index add%EL_UC%() { return Base::addElement(); }

//@if vertex_references
    /**
     * @brief Adds a new %EL_TXT% with the given vertices or vertex indices into the
     * %EL_UC% container, returning the index of the added %EL_TXT%.
     *
     * @tparam Args: The types of the vertex arguments (references or indices).
     * @param[in] args: The vertices or vertex indices of the new %EL_TXT%.
     * @return The index of the new %EL_TXT%.
     */
    template<typename... Args>
    Index add%EL_UC%(const Args&... args)%ADD_VERTICES_REQUIRES%
    {
        Index eid = add%EL_UC%();
        %EL_C%(eid).setVertices(args...);
        return eid;
    }

    /**
     * @brief Adds a new %EL_TXT% with the vertices or vertex indices contained in
     * the given range, returning the index of the added %EL_TXT%.
     *
     * @tparam Rng: An input range type containing vertices or vertex indices.
     * @param[in] rng: The range of vertices or vertex indices of the new %EL_TXT%.
     * @return The index of the new %EL_TXT%.
     */
    template<std::ranges::input_range Rng>
    Index add%EL_UC%(Rng&& rng)
    {
        Index eid = add%EL_UC%();
        %EL_C%(eid).setVertices(std::forward<Rng>(rng));
        return eid;
    }
//@endif

    /**
     * @brief Adds an arbitrary number of n %EL_TXT_PL%, returning the index of the
     * first added %EL_TXT%.
     *
     * @param[in] n: the number of %EL_TXT_PL% to add to the mesh.
     * @return The index of the first added %EL_TXT%.
     */
    Index add%EL_UC_PL%(std::size_t n) { return Base::addElements(n); }

//@if vertex_references
    /**
     * @brief Adds an arbitrary number of %EL_TXT_PL% with the vertices or vertex
     * indices contained in the given range of ranges, returning the index of
     * the first added %EL_TXT%.
     *
     * @tparam Rng: A range of ranges containing vertices or vertex indices.
     * @param[in] rng: A range of ranges containing the vertices or vertex
     * indices for each %EL_TXT%.
     * @return The index of the first added %EL_TXT%.
     */
    template<std::ranges::input_range Rng>
    requires std::ranges::input_range<std::ranges::range_value_t<Rng>>
    Index add%EL_UC_PL%(Rng&& rng)
    {
        Index baseId = %EL_C%ContainerSize();
        if constexpr (std::ranges::sized_range<Rng>) {
            reserve%EL_UC_PL%(baseId + std::ranges::size(rng));
        }
        for (auto&& e : rng) {
            add%EL_UC%(e);
        }
        return baseId;
    }
//@endif

    /**
     * @brief Clears the %EL_UC% container of the Mesh, deleting all the
     * %EL_TXT_PL%.
     *
     * The contained %EL_TXT% components are actually removed from the container,
     * not only marked as deleted. Therefore, the container will have size 0
     * (`%EL_C%ContainerSize() == 0`) after the call of this function.
     *
     * @warning Any index or reference to %EL_TXT_PL% in the Mesh will be left
     * unchanged, and therefore will point to invalid %EL_TXT_PL%.
     */
    void clear%EL_UC_PL%() { Base::clearElements(); }

    /**
     * @brief Resizes the %EL_UC% container to contain `n` %EL_TXT_PL%.
     *
     * If the new size is greater than the old one, new %EL_TXT_PL% are added to
     * the container. If the new size is smaller than the old one, the container
     * is truncated to `n` %EL_TXT_PL% (%EL_TXT_PL% beyond `n` are removed).
     *
     * @warning Any index to removed %EL_TXT_PL% in the Mesh will be left
     * unchanged, and therefore will point to invalid %EL_TXT_PL%.
     *
     * @param[in] n: the new size of the %EL_UC% container.
     */
    void resize%EL_UC_PL%(std::size_t n) { Base::resizeElements(n); }

    /**
     * @brief Reserves a number of %EL_TXT_PL% in the container of %EL_UC_PL%. This is
     * useful when you know (or you have an idea) of how many %EL_TXT_PL% will be
     * added to a new or existing mesh. Calling this function before any
     * add%EL_UC%() call will avoid unnecessary reallocations of the container,
     * saving execution time.
     *
     * The philosophy of this function is similar to the one of the
     * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
     * function of the [std::vector
     * class](https://en.cppreference.com/w/cpp/container/vector).
     *
     * @param[in] n: the new capacity of the %EL_TXT% container.
     */
    void reserve%EL_UC_PL%(std::size_t n) { Base::reserveElements(n); }

    /**
     * @brief Compacts the %EL_UC% Container, removing all the %EL_TXT_PL% marked as
     * deleted. %EL_UC% references will automatically change accordingly in all
     * the containers of the Mesh.
     *
     * @return A vector that maps each old %EL_TXT% index to its new index in the
     * compacted container (or NULL_INDEX if it was deleted).
     */
    std::vector<Index> compact%EL_UC_PL%() { return Base::compactElements(); }

    /**
     * @brief Marks as deleted the %EL_TXT% with the given id.
     *
     * This member function does not perform any reallocation of the %EL_TXT_PL%:
     * the deleted %EL_TXT_PL% will stay in the %EL_UC% Container, but will be
     * marked as deleted.
     *
     * Deleted %EL_TXT_PL% are automatically jumped by the iterators provided by
     * the %EL_UC% Container.
     *
     * @param[in] i: the index of the %EL_TXT% that will be marked as deleted.
     */
    void delete%EL_UC%(Index i) { Base::deleteElement(i); }

    /**
     * @brief Marks as deleted the given %EL_TXT%.
     *
     * This member function does not perform any reallocation of the %EL_TXT_PL%:
     * the deleted %EL_TXT_PL% will stay in the %EL_UC% Container, but will be
     * marked as deleted.
     *
     * Deleted %EL_TXT_PL% are automatically jumped by the iterators provided by
     * the %EL_UC% Container.
     *
     * @param[in] e: the %EL_TXT% that will be marked as deleted.
     */
    void delete%EL_UC%(const %EL_UC%& e) { Base::deleteElement(e.index()); }

    /**
     * @brief This is a utility member function that returns the index of an
     * element if the container were compact, that is the number of
     * non-deleted elements before the %EL_TXT% with the given index.
     *
     * Complexity: O(n), with n the number of %EL_TXT_PL% in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of %EL_A% %EL_TXT% of the container.
     * @return The index that the %EL_TXT% with index i would have if this
     * container were compact.
     */
    Index %EL_C%IndexIfCompact(Index i) const
    {
        return Base::elementIndexIfCompact(i);
    }

    /**
     * @brief Returns a vector that tells, for each actual %EL_TXT% index, the new
     * index that the %EL_TXT% would have in a compacted container. For each
     * deleted %EL_TXT% index, the value of the vector will be NULL_INDEX.
     *
     * This is useful if you need to know the indices of the %EL_TXT_PL% that they
     * would have in a compact container, without considering the deleted ones.
     *
     * @return A vector containing, for each %EL_TXT% index, its index if the
     * container were compact.
     */
    std::vector<Index> %EL_C%CompactIndices() const
    {
        return Base::elementCompactIndices();
    }

    /**
     * @brief Updates references to the %EL_TXT_PL% of this mesh according to the
     * mapping stored in the newIndices vector.
     *
     * This function is useful when %EL_TXT_PL% are deleted, compacted, or
     * permuted, and you want to update all references stored across the
     * mesh containers to point to their new indices.
     *
     * @note This function does not change the position of the %EL_TXT_PL% in this
     * container. It updates %EL_TXT% references stored in this and other
     * containers of the mesh.
     *
     * @param[in] newIndices: A vector mapping old %EL_TXT% indices to new ones.
     * If %EL_A% %EL_TXT% was deleted or should become unreferenced, its mapping entry
     * must be NULL_INDEX.
     */
    void update%EL_UC%References(const std::vector<Index>& newIndices)
    {
        Base::updateElementReferences(newIndices);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate over
     * the %EL_TXT_PL% of the container, providing begin() and end() member
     * functions.
     *
     * This member function is very useful when you want to iterate over the
     * %EL_TXT_PL% using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (%EL_UC% e : m.%EL_C_PL%()) {
     *     // do something with this %EL_TXT%
     * }
     * @endcode
     *
     * The iterator used to iterate over %EL_TXT_PL% is automatically initialized
     * to jump deleted %EL_TXT_PL% of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted %EL_TXT_PL%.
     * @return An object having begin() and end() functions, allowing to
     * iterate over the container.
     */
    auto %EL_C_PL%(bool jumpDeleted = true)
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate over
     * the %EL_TXT_PL% of the container, providing begin() and end() member
     * functions.
     *
     * This member function is very useful when you want to iterate over the
     * %EL_TXT_PL% using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (Const%EL_UC% e : m.%EL_C_PL%()) {
     *     // do something with this %EL_TXT%
     * }
     * @endcode
     *
     * The iterator used to iterate over %EL_TXT_PL% is automatically initialized
     * to jump deleted %EL_TXT_PL% of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted %EL_TXT_PL%.
     * @return An object having begin() and end() functions, allowing to
     * iterate over the container.
     */
    auto %EL_C_PL%(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns a view object that allows to iterate over the %EL_TXT_PL% of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the %EL_C_PL%() function, this member function does not
     * automatically jump deleted %EL_TXT_PL%, but it iterates over the
     * %EL_TXT_PL% in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first %EL_TXT% to be included in the
     * range. It must be less than or equal to %EL_C%ContainerSize() and less than
     * or equal to the end index.
     * @param[in] end: the index of the last %EL_TXT% to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * %EL_C%ContainerSize(), it will be set to %EL_C%ContainerSize().
     * @return A View object allowing to iterate over the given range of the container.
     */
    auto %EL_C_PL%(Index begin, Index end = NULL_INDEX)
    {
        return Base::elements(begin, end);
    }

    /**
     * @brief Returns a view object that allows to iterate over the %EL_TXT_PL% of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the %EL_C_PL%() function, this member function does not
     * automatically jump deleted %EL_TXT_PL%, but it iterates over the
     * %EL_TXT_PL% in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first %EL_TXT% to be included in the
     * range. It must be less than or equal to %EL_C%ContainerSize() and less than
     * or equal to the end index.
     * @param[in] end: the index of the last %EL_TXT% to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * %EL_C%ContainerSize(), it will be set to %EL_C%ContainerSize().
     * @return A View object allowing to iterate over the given range of the container.
     */
    auto %EL_C_PL%(Index begin, Index end = NULL_INDEX) const
    {
        return Base::elements(begin, end);
    }

    // --- Components ---

    /**
     * @brief Checks if the %EL_TXT% component with the given COMP_ID is
     * available (present at compile time and enabled at run time).
     *
     * Non-optional components are always available (true).
     * Optional components return true if currently enabled, false otherwise.
     *
     * @tparam COMP_ID: The ID of the component to check.
     * @return `true` if currently available, `false` otherwise.
     */
    template<CompId COMP_ID>
    bool is%EL_UC%ComponentAvailable() const
    {
        return Base::template isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Enables the optional %EL_TXT% component with the given COMP_ID.
     *
     * @tparam COMP_ID: The ID of the component to enable.
     */
    template<CompId COMP_ID>
    void enable%EL_UC%Component()
        requires comp::HasOptionalComponent<COMP_ID, %EL_UC%Components>
    {
        Base::template enableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Disables the optional %EL_TXT% component with the given COMP_ID.
     * Note: Non-optional components cannot be disabled.
     *
     * @tparam COMP_ID: The ID of the component to disable.
     */
    template<CompId COMP_ID>
    void disable%EL_UC%Component()
        requires comp::HasOptionalComponent<COMP_ID, %EL_UC%Components>
    {
        Base::template disableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Enables all optional %EL_TXT% components in the container.
     */
    void enableAll%EL_UC%OptionalComponents()
    {
        Base::enableAllOptionalComponents();
    }

    /**
     * @brief Disables all optional %EL_TXT% components in the container.
     */
    void disableAll%EL_UC%OptionalComponents()
    {
        Base::disableAllOptionalComponents();
    }

    /**
     * @brief Enables all optional %EL_TXT% components in the container.
     * Alias for enableAll%EL_UC%OptionalComponents().
     */
    void enableAll%EL_UC%Components()
    {
        Base::enableAllOptionalComponents();
    }

    /**
     * @brief Disables all optional %EL_TXT% components in the container.
     * Alias for disableAll%EL_UC%OptionalComponents().
     */
    void disableAll%EL_UC%Components()
    {
        Base::disableAllOptionalComponents();
    }

    VCL_COMMON_CONTAINER_COMPONENT_METHODS(%EL_UC%)
%CONTAINER_COMPONENT_METHODS%
    // --- Custom Components ---

    /**
     * @brief Checks if %EL_UC_PL% have a custom component with the given name.
     *
     * This function does not take into account the type of the custom
     * component.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component to check.
     * @return `true` if the %EL_UC% Element has a custom component with the given
     * name.
     */
    bool has%EL_UC%CustomComponent(const std::string& name) const
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        return Base::hasElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * of any type associated to the %EL_UC% Element.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @return A vector of strings representing all the names of the custom
     * components.
     */
    std::vector<std::string> %EL_C%CustomComponentNames() const
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        return Base::elemCustomComponentNames();
    }

    /**
     * @brief Checks if the custom component of the %EL_UC% Element having the
     * given name has the same type of the given template argument type of this
     * function.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component to check.
     * @param[in] name: The name of the custom component to check.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return `true` if the custom component is of the same type of the
     * template argument.
     */
    template<typename K>
    bool is%EL_UC%CustomComponentOfType(const std::string& name) const
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        return Base::template isElemCustomComponentOfType<K>(name);
    }

    /**
     * @brief Returns the std::type_index of the custom component of the %EL_UC%
     * Element having the given input name.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component to get the
     * std::type_index from.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return The std::type_index of the custom component having the given
     * input name.
     */
    std::type_index %EL_C%CustomComponentType(const std::string& name) const
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        return Base::elemCustomComponentType(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * associated to the %EL_UC% Element having the same type of the given
     * template argument type of this function.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component names.
     * @return A vector of strings representing the names of the custom
     * components of a given type.
     */
    template<typename K>
    std::vector<std::string> %EL_C%CustomComponentNamesOfType() const
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        return Base::template elemCustomComponentNamesOfType<K>();
    }

    /**
     * @brief Adds a custom component of type K to the %EL_UC%, having the given
     * name.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component added to the %EL_UC%.
     * @param[in] name: The name of the custom component added to the %EL_UC%.
     */
    template<typename K>
    void add%EL_UC%CustomComponent(const std::string& name)
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        Base::template addElemCustomComponent<K>(name);
    }

    /**
     * @brief Deletes the custom component of the given name from the %EL_UC%
     * Element.
     *
     * The function does nothing if the custom component does not exist.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component that will be removed
     * from the %EL_UC%.
     */
    void delete%EL_UC%CustomComponent(const std::string& name)
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        Base::deleteElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector handle to the custom component having the type K
     * and the given name.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component for which to return the handle.
     * @param[in] name: Name of the custom component for which to return the handle.
     * @return A vector handle that allows access to the custom component.
     */
    template<typename K>
    CustomComponentVectorHandle<K> %EL_C%CustomComponentVectorHandle(
        const std::string& name)
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Returns a const vector handle to the custom component having type
     * K and the given name.
     *
     * @note This function is available only if the %EL_UC% Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component for which to return the handle.
     * @param[in] name: Name of the custom component for which to return the handle.
     * @return A const vector handle that allows access to the custom
     * component.
     */
    template<typename K>
    ConstCustomComponentVectorHandle<K> %EL_C%CustomComponentVectorHandle(
        const std::string& name) const
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Serializes all per-%EL_TXT% custom components of type K stored in
     * this container to the given output stream.
     *
     * @tparam K: The type of the custom components to serialize.
     * @param os: The output stream to write to.
     */
    template<typename K>
    void serialize%EL_UC%CustomComponentsOfType(std::ostream& os) const
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        Base::template serializeElemCustomComponentsOfType<K>(os);
    }

    /**
     * @brief Deserializes all per-%EL_TXT% custom components of type K from the
     * given input stream and stores them into this container.
     *
     * @tparam K: The type of the custom components to deserialize.
     * @param is: The input stream to read from.
     */
    template<typename K>
    void deserialize%EL_UC%CustomComponentsOfType(std::istream& is)
        requires comp::HasCustomComponents<%EL_UC%Components>
    {
        Base::template deserializeElemCustomComponentsOfType<K>(is);
    }

protected:
    /**
     * @brief Imports all %EL_TXT_PL% from the corresponding container of the
     * other mesh into this container.
     *
     * @param[in] m: The other mesh to import from.
     */
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        Base::importFrom(m);
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_%EL_U%_CONTAINER_H
