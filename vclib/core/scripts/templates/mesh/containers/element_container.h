%HEADER%

#ifndef VCL_MESH_CONTAINER_%EL_U%_CONTAINER_H
#define VCL_MESH_CONTAINER_%EL_U%_CONTAINER_H

#include "element_container.h"

#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/mesh/elements/%EL%.h>
#include <vclib/mesh/elements/%EL%_components.h>

namespace vcl {
namespace mesh {

/**
 * @brief The %EL_UC%Container class represents a container of %EL_UC%
 * elements that can be used in a Mesh class.
 * 
 * This class adds a container (vector) of %EL_UC%s to the Mesh, making
 * available the accessors members to them and their components.
 * 
 * It provides all the add, delete, reserve, compact and other functions to
 * manage the %EL_UC%s, plus the iterators and views to iterate over them.
 * It also provides the functions to enable and disable the optional components
 * of the %EL_UC%s.
 *
 * @tparam T: The type of the %EL_UC% elements. It must satisfy the
 * %EL_UC%Concept.
 *
 * @ingroup containers
 */
template<%EL_UC%Concept T>
class %EL_UC%Container : public ElementContainer<T>
{
    template <%EL_UC%Concept U>
    friend class %EL_UC%Container;

    using %EL_UC%ContainerType = %EL_UC%Container<T>;
    using Base = ElementContainer<T>;

public:
    using %EL_UC%              = T;
    using %EL_UC%Type          = T;
    using %EL_UC%Iterator      = Base::ElementIterator;
    using Const%EL_UC%Iterator = Base::ConstElementIterator;

    /**
     * @brief Empty constructor that creates an empty container of
     * %EL_UC%s.
     */
    %EL_UC%Container() = default;

    /**
     * @brief Returns a const reference of the %EL_UC% at the i-th
     * position in the %EL_UC% Container of the Mesh, which will be the
     * %EL_UC% having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * %EL_C%ContainerSize(), this function will return a valid
     * %EL_UC% reference (note that the %EL_UC% may have been flagged
     * as deleted).
     *
     * @param[in] i: the index of the %EL_UC% that will be returned.
     */
    const %EL_UC%Type& %EL_C%(uint i) const
    {
        return Base::element(i);
    }

    /**
     * @brief Returns a reference of the %EL_UC% at the i-th position in the
     * %EL_UC% Container of the Mesh, which will be the %EL_UC% having
     * index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * %EL_C%ContainerSize(), this function will return a valid %EL_UC%
     * reference (note that the %EL_UC% may have been flagged as deleted).
     *
     * @param[in] i: the index of the %EL_UC% that will be returned.
     */
    %EL_UC%Type& %EL_C%(uint i) { return Base::element(i); }

    /**
     * @brief Returns the number of **non-deleted** %EL_UC%s contained in
     * the %EL_UC% container of the Mesh.
     *
     * If %EL_C%Number() != %EL_C%ContainerSize(), it means that there
     * are some %EL_UC%s that are flagged as deleted.
     *
     * @return the number of non-deleted %EL_UC%s of the Mesh.
     */
    uint %EL_C%Number() const { return Base::elementNumber(); }

    /**
     * @brief Returns the number of %EL_UC%s (also deleted) contained in
     * the %EL_UC% container of the Mesh.
     *
     * If %EL_C%Number() != %EL_C%ContainerSize(), it means that there are
     * some %EL_UC%s that are flagged as deleted.
     *
     * @return the number of all the %EL_UC%s contained in the Mesh.
     */
    uint %EL_C%ContainerSize() const
    {
        return Base::elementContainerSize();
    }

    /**
     * @brief Returns the number of deleted %EL_UC%s in the %EL_UC%
     * container, that is %EL_C%ContainerSize() - %EL_C%Number().
     *
     * @return The number of deleted %EL_UC%s in the container.
     */
    uint deleted%EL_UC%Number() const
    {
        return Base::deletedElementNumber();
    }

    /**
     * @brief Add a %EL_UC% to the container, returning its index.
     *
     * If the call of this function will cause a reallocation of the
     * %EL_UC%Container, the function will automatically take care
     * of updating all the %EL_UC% pointers contained in the Mesh.
     *
     * @return the id of the newly added %EL_UC%.
     */
    uint add%EL_UC%() { return Base::addElement(); }
    
    /**
     * @brief Add an arbitrary number of n %EL_UC%s, returning the id
     * of the first added %EL_UC%.
     *
     * This means that, if you want to add 5 %EL_UC%s and this member
     * function returns 4, the added %EL_UC%s will have id from 4 to
     * id 8 included.
     *
     * If the call of this function will cause a reallocation of the
     * %EL_UC%Container, the function will automatically take care of
     * updating all the %EL_UC% pointers contained in the Mesh.
     *
     * @param n: the number of %EL_UC%s to add to the mesh.
     * @return the id of the first added %EL_UC%.
     */
    uint add%EL_UC%s(uint n) { return Base::addElements(n); }

    /**
     * @brief Clears the %EL_UC% container of the Mesh, deleting all
     * the %EL_UC%s.
     *
     * The contained %EL_UC%s are actually removed from the container,
     * not only marked as deleted. Therefore, the container will have size 0
     * (`%EL_C%ContainerSize() == 0`) after the call of this function.
     *
     * @note This function does not cause a reallocation of the %EL_UC%
     * container.
     *
     * @warning Any pointer to %EL_UC%s in the Mesh will be left
     * unchanged, and therefore will point to invalid %EL_UC%s. This
     * means that, if you have a pointer to a %EL_UC% and you call this
     * function, you will have a dangling pointer.
     */
    void clear%EL_UC%s() { Base::clearElements(); }
    
    /**
     * @brief Resizes the %EL_UC% container to contain `n` %EL_UC%s.
     *
     * If the new size is greater than the old one, new %EL_UC%s are
     * added to the container, and a reallocation may happen. If the new size is
     * smaller than the old one, the container will keep its first non-deleted
     * `n` %EL_UC%s, and the remaining %EL_UC%s are marked as deleted.
     *
     * If the call of this function will cause a reallocation of the
     * %EL_UC% container, the function will automatically take care of
     * updating all the %EL_UC% pointers contained in the Mesh.
     *
     * @warning The given size `n` is relative to the number of non-deleted
     * %EL_UC%s, not to the size of the %EL_UC% container. For example,
     * if you have a mesh with 10 %EL_UC%s and
     * %EL_C%ContainerSize() == 20, calling resize%EL_UC%s(5) will
     * not cause a reallocation of the container, but will mark as deleted the
     * least 5 non-deleted %EL_UC%s of the container. In the same
     * scenario, calling resize%EL_UC%s(15) will result in a %EL_UC%
     * container having 15 %EL_UC%s and %EL_C%ContainerSize() == 25.
     * The latest 5 %EL_UC%s will be the newly added.
     *
     * @warning Any pointer to deleted %EL_UC%s in the Mesh will be left
     * unchanged, and therefore will point to invalid %EL_UC%s. This
     * means that if you call this member function with a lower number of
     * %EL_UC%s, you'll need to manually manage the pointers to the deleted
     * %EL_UC%s.
     *
     * @param[in] n: the new size of the %EL_UC% container.
     */
    void resize%EL_UC%s(uint n) { Base::resizeElements(n); }
    
    /**
     * @brief Reserve a number of %EL_UC%s in the container of %EL_UC%s.
     * This is useful when you know (or you have an idea) of how much
     * %EL_UC%s are going to add into a newly of existing mesh. Calling
     * this function before any add%EL_UC%() call will avoid unuseful
     * reallocations of the container, saving execution time.
     *
     * The filosofy of this function is similar to the one of the
     * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
     * function of the
     * [std::vector class](https://en.cppreference.com/w/cpp/container/vector).
     *
     * If the call of this function will cause a reallocation of the
     * %EL_UC% container, the function will automatically take care of
     * updating all the %EL_UC% pointers contained in the Mesh.
     *
     * @param n: the new capacity of the %EL_UC% container.
     */
    void reserve%EL_UC%s(uint n) { Base::reserveElements(n); }

    /**
     * @brief Compacts the %EL_UC%Container, removing all the %EL_UC%s
     * marked as deleted. %EL_UC%s indices will change accordingly. The
     * function will automatically take care of updating all the %EL_UC%
     * pointers contained in the Mesh.
     */
    void compact%EL_UC%s() { Base::compactElements(); }

    /**
     * @brief Marks as deleted the %EL_UC% with the given id.
     *
     * This member function does not perform any reallocation of the
     * %EL_UC%s: the deleted %EL_UC%s will stay in the %EL_UC%
     * Container, but will be marked as deleted.
     *
     * Deleted %EL_UC%s are automatically jumped by the iterators
     * provided by the %EL_UC% Container.
     *
     * @param[in] i: the id of the %EL_UC% that will be marked as deleted.
     */
    void delete%EL_UC%(uint i) { Base::deleteElement(i); }
    
    /**
     * @brief Marks as deleted the given %EL_UC%, before asserting that
     * the %EL_UC% belongs to this container.
     *
     * This member function does not perform any reallocation of the
     * %EL_UC%s: the deleted %EL_UC%s will stay in the %EL_UC%
     * Container, but will be marked as deleted.
     *
     * Deleted %EL_UC%s are automatically jumped by the iterators
     * provided by the %EL_UC% Container.
     *
     * @param[in] e: the pointer of the %EL_UC% that will be marked as
     * deleted.
     */
    void delete%EL_UC%(const %EL_UC%Type* e)
    {
        Base::deleteElement(e);
    }

    /**
     * @brief This is an utility member function that returns the index of an
     * element if the container would be compact, that is the number of
     * non-deleted elements before the %EL_UC% with the given index.
     *
     * Complexity: O(n), with n the number of %EL_UC%s in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of a %EL_UC% of the container.
     * @return The index that the %EL_UC% with index i would have if
     * this container would be compact.
     */
    uint %EL_C%IndexIfCompact(uint i) const
    {
        return Base::elementIndexIfCompact(i);
    }
    
    /**
     * @brief Returns a vector that tells, for each actual %EL_UC% index,
     * the new index that the %EL_UC% would have in a compacted container.
     * For each deleted %EL_UC% index, the value of the vector will be
     * UINT_NULL.
     *
     * This is useful if you need to know the indices of the %EL_UC%s that
     * they would have in a compact container, without considering the deleted
     * ones.
     *
     * @return A vector containing, for each %EL_UC% index, its index if
     * the container would be compact.
     */
    std::vector<uint> %EL_C%CompactIndices() const
    {
        return Base::elementCompactIndices();
    }

    /**
     * @brief Updates all the indices and pointers of the %EL_UC%s of
     * this container that are stored in any container of the mesh, according to
     * the mapping stored in the newIndices vector, that tells for each old
     * %EL_UC% index, the new %EL_UC% index.
     *
     * This function is useful when delete some %EL_UC%s, and you want
     * to update the indices/pointers stored in all the containers of the mesh
     * accordingly.
     *
     * E.g. Supposing you deleted a set of %EL_UC%s, you can give to this
     * function the vector telling, for each one of the old %EL_UC%
     * indices, the new %EL_UC% index (or UINT_NULL if you want to leave
     * it unreferenced).
     * This function will update all the pointers stored in the mesh containers
     * accordingly (if they store adjacencies to the %EL_UC%s).
     *
     * @note This function *does not change the position of the %EL_UC%s
     * in this container*. It just updates the indices/pointers of the
     * %EL_UC%s stored in this or other containers.
     *
     * @param[in] newIndices: a vector that tells, for each old %EL_UC%
     * index, the new %EL_UC% index. If the old %EL_UC% must be left as
     * unreferenced (setting `nullptr` to the pointers), the value of the vector
     * must be UINT_NULL.
     */
    void update%EL_UC%Indices(const std::vector<uint>& newIndices)
    {
        Base::updateElementIndices(newIndices);
    }

    /**
     * @brief Returns an iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted %EL_UC%s
     * of the container. You can change this option by calling this function
     * with `jumpDeleted=false`.
     *
     * @param[in] jumpDeleted: boolean that tells if the iterator should jump
     * deleted %EL_UC%s.
     * @return An iterator the the first %EL_UC% of the container.
     */
    %EL_UC%Iterator %EL_C%Begin(bool jumpDeleted = true)
    {
        return Base::elementBegin(jumpDeleted);
    }
    
    /**
     * @brief Returns an iterator to the end of the container.
     * @return An iterator to the end of the container.
     */
    %EL_UC%Iterator %EL_C%End() { return Base::elementEnd(); }
    
    /**
     * @brief Returns a const iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted %EL_UC%s
     * of the container. You can change this option by calling this function
     * with `jumpDeleted=false`.
     *
     * @param[in] jumpDeleted: boolean that tells if the iterator should jump
     * deleted %EL_UC%s.
     * @return A const iterator the the first %EL_UC% of the container.
     */
    Const%EL_UC%Iterator %EL_C%Begin(bool jumpDeleted = true) const
    {
        return Base::elementBegin(jumpDeleted);
    }
    
    /**
     * @brief Returns a const iterator to the end of the container.
     * @return A const iterator to the end of the container.
     */
    Const%EL_UC%Iterator %EL_C%End() const
    {
        return Base::elementEnd();
    }
    
    /**
     * @brief Returns a small view object that allows to iterate over the
     * %EL_UC%s of the containers, providing two member functions begin()
     * and end().
     *
     * This member function is very useful when you want to iterate over the
     * %EL_UC%s using the C++ foreach syntax:
     *
     * @code{.cpp}
     * for (%EL_UC%& e : m.%EL_C%s()){
     *     // do something with this %EL_UC%
     * }
     * @endcode
     *
     * The iterator used to iterate over %EL_UC%s is automatically
     * initialized to jump deleted %EL_UC%s of the container. You can
     * change this option by calling this function with `jumpDeleted=false`.
     *
     * @param[in] jumpDeleted: boolean that tells if the iterator should jump
     * deleted %EL_UC%s.
     * @return An object having begin() and end() function, allowing to iterate
     * over the container.
     */
    auto %EL_C%s(bool jumpDeleted = true)
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns a view object that allows to iterate over the %EL_UC%s
     * of the container in the given range:
     *
     * @code{.cpp}
     * for (%EL_UC%& e : m.%EL_C%s(3, 10)){
     *     // iterate over the %EL_UC%s from index 3 to 10
     *     // do something with e
     * }
     * @endcode
     *
     * @note Unlike the %EL_C%s() function, this member function does not
     * automatically jump deleted %EL_C%s, but it iterates over the
     * %EL_C%s in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first %EL_C% to be included in the
     * range. It must be less or equal to %EL_C%ContainerSize() and less or
     * equal to the end index.
     * @param[in] end: the index of the last %EL_C% to be included in the
     * range.
     * @return An object having begin() and end() function, allowing to iterate
     * over the given range of the container.
     */
    auto %EL_C%s(uint begin, uint end = UINT_NULL)
    {
        return Base::elements(begin, end);
    }
    
    /**
     * @brief Returns a small view object that allows to iterate over the
     * %EL_UC%s of the containers, providing two member functions begin()
     * and end().
     *
     * This member function is very useful when you want to iterate over the
     * %EL_UC%s using the C++ foreach syntax:
     *
     * @code{.cpp}
     * for (const %EL_UC%& e : m.%EL_C%s()){
     *     // do something with this %EL_UC%
     * }
     * @endcode
     *
     * The iterator used to iterate over %EL_UC%s is automatically 
     * initialized to jump deleted %EL_UC%s of the container. You can 
     * change this option by calling this function with `jumpDeleted=false`.
     *
     * @param[in] jumpDeleted: boolean that tells if the iterator should jump
     * deleted %EL_UC%s.
     * @return An object having begin() and end() function, allowing to iterate 
     * over the container.
     */
    auto %EL_C%s(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns a view object that allows to iterate over the %EL_UC%s
     * of the container in the given range:
     *
     * @code{.cpp}
     * for (const %EL_UC%& e : m.%EL_C%s(3, 10)){
     *     // iterate over the %EL_UC%s from index 3 to 10
     *     // do something with e
     * }
     * @endcode
     *
     * @note Unlike the %EL_C%s() function, this member function does not
     * automatically jump deleted %EL_C%s, but it iterates over the
     * %EL_C%s in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first %EL_C% to be included in the
     * range. It must be less or equal to %EL_C%ContainerSize() and less or
     * equal to the end index.
     * @param[in] end: the index of the last %EL_C% to be included in the
     * range.
     * @return An object having begin() and end() function, allowing to iterate
     * over the given range of the container.
     */
    auto %EL_C%s(uint begin, uint end = UINT_NULL) const
    {
        return Base::elements(begin, end);
    }

    /**
     * @brief Enables all the optional components associated to the
     * %EL_UC% type contained in the %EL_UC%Container.
     */
    void enableAllPer%EL_UC%OptionalComponents()
    {
        Base::enableAllOptionalComponents();
    }
    
    /**
     * @brief Disables all the optional components associated to the
     * %EL_UC% type contained in the %EL_UC%Container.
     */
    void disableAllPer%EL_UC%OptionalComponents()
    {
        Base::disableAllOptionalComponents();
    }

%OPTIONAL_COMPONENTS%

};

/* Concepts */

/**
 * @brief A concept that checks whether a class has (inherits from) an
 * %EL_UC%Container class.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a %EL_UC%Container class having any %EL_UC% element type.
 *
 * @tparam T: The type to be tested for conformity to the Has%EL_UC%Container.
 *
 * @ingroup containers
 * @ingroup containers_concepts
 */
template<typename T>
concept Has%EL_UC%Container = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    %EL_UC%Container<typename RemoveRef<T>::%EL_UC%Type>>;

} // namespace mesh

/**
 * @brief Has%EL_UC%s concepts is satisfied when at least one of its
 * template types is (or inherits from) a @ref vcl::mesh::%EL_UC%Container.
 * It can be used both to check if a Mesh has %EL_C%s, or if in a list of
 * types there is a %EL_UC%Container.
 *
 * In the following example, a MyMesh type can be instantiated only if one of
 * its template Args is a %EL_UC%Container:
 * @code{.cpp}
 * template <typename... Args> requires Has%EL_UC%s<Args...>
 * class MyMesh {
 *     // ...
 * };
 *
 * // ...
 *
 * MyMesh<vcl::VertexContainer<MyVertex>> m1; // not ok
 * MyMesh<vcl::%EL_UC%Container<My%EL_UC%>> m2; // ok
 * MyMesh<
 *     vcl::VertexContainer<MyVertex>,
 *     vcl::%EL_UC%Container<My%EL_UC%>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) %EL_UC%Container:
 * @code{.cpp}
 * if constexpr (vcl::Has%EL_UC%s<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 *
 * @note This concept does not check if a Mesh is a valid %EL_UC%Mesh.
 * To do that, use the %EL_UC%MeshConcept.
 *
 * @ingroup containers_concepts
 */
template<typename... Args>
concept Has%EL_UC%s = (mesh::Has%EL_UC%Container<Args> || ...);

} // namespace vcl

#endif // VCL_MESH_CONTAINER_%EL_U%_CONTAINER_H
