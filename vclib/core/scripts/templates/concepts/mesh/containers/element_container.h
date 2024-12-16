%HEADER%

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_%EL_U%_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_%EL_U%_CONTAINER_H

#include <vclib/concepts/const_correctness.h>
#include <vclib/concepts/iterators.h>
#include <vclib/concepts/ranges/range.h>

#include <vector>

namespace vcl {
namespace mesh {

/**
 * @brief The Has%EL_UC%Container concept is satisfied only if a container class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to a @ref 
 * vcl::mesh::%EL_UC%Container container of a given mesh.
 * 
 * @ingroup containers_concepts
 */
template <typename T>
concept Has%EL_UC%Container = requires(
    T&&                                          obj,
    typename RemoveRef<T>::%EL_UC%Type           e,
    typename RemoveRef<T>::%EL_UC%Type*          eP,
    typename RemoveRef<T>::%EL_UC%Type&          eR,
    std::vector<uint>                            vec)
{
    typename RemoveRef<T>::%EL_UC%Type;
    typename RemoveRef<T>::%EL_UC%Iterator;
    typename RemoveRef<T>::Const%EL_UC%Iterator;

    { obj.%EL_C%(uint()) } -> std::convertible_to<decltype(e)>;
    { obj.%EL_C%Number() } -> std::same_as<uint>;
    { obj.%EL_C%ContainerSize() } -> std::same_as<uint>;
    { obj.deleted%EL_UC%Number() } -> std::same_as<uint>;

    { obj.%EL_C%IndexIfCompact(uint()) } -> std::same_as<uint>;
    { obj.%EL_C%CompactIndices() } -> std::same_as<decltype(vec)>;

    { obj.%EL_C%Begin() } -> InputIterator<decltype(e)>;
    { obj.%EL_C%End() } -> InputIterator<decltype(e)>;
    { obj.%EL_C%s() } -> InputRange<decltype(e)>;
    { obj.%EL_C%s(bool()) } -> InputRange<decltype(e)>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.%EL_C%(uint()) } -> std::same_as<decltype(eR)>;

        { obj.add%EL_UC%() } -> std::same_as<uint>;
        { obj.add%EL_UC%s(uint()) } -> std::same_as<uint>;
        { obj.clear%EL_UC%() } -> std::same_as<void>;
        { obj.resize%EL_UC%(uint()) } -> std::same_as<void>;
        { obj.reserve%EL_UC%(uint()) } -> std::same_as<void>;
        { obj.compact%EL_UC%() } -> std::same_as<void>;
        { obj.delete%EL_UC%(uint()) } -> std::same_as<void>;
        { obj.delete%EL_UC%(eP) } -> std::same_as<void>;
        { obj.update%EL_UC%Indices(vec) } -> std::same_as<void>;

        { obj.%EL_C%Begin() } -> OutputIterator<decltype(e)>;
        { obj.%EL_C%End() } -> OutputIterator<decltype(e)>;
        { obj.%EL_C%s() } -> OutputRange<decltype(e)>;
        { obj.%EL_C%s(bool()) } -> OutputRange<decltype(e)>;

        { obj.enableAllPer%EL_UC%OptionalComponents() } -> std::same_as<void>;
        { obj.disableAllPer%EL_UC%OptionalComponents() } -> std::same_as<void>;
    };
};

} // namespace vcl::mesh

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

#endif // VCL_CONCEPTS_MESH_CONTAINERS_%EL_U%_CONTAINER_H
