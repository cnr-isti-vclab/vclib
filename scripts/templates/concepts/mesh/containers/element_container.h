%HEADER%

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_%EL_U%_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_%EL_U%_CONTAINER_H

#include <ranges>
#include <vector>

#include "element_container.h"

namespace vcl {
namespace mesh {

template <typename T>
concept Has%EL_UC%Container = requires(
	T o,
	const T& co,
	typename T::%EL_UC%Type* e)
{
	typename T::%EL_UC%Type;
	typename T::%EL_UC%Iterator;
	typename T::Const%EL_UC%Iterator;
	
	{ o.%EL_C%(uint()) } -> std::same_as<typename T::%EL_UC%Type&>;
	{ co.%EL_C%(uint()) } -> std::same_as<const typename T::%EL_UC%Type&>;
	
	{ co.%EL_C%Number() } -> std::same_as<uint>;
	{ co.%EL_C%ContainerSize() } -> std::same_as<uint>;
	{ co.deleted%EL_UC%Number() } -> std::same_as<uint>;
	o.delete%EL_UC%(uint());
	o.delete%EL_UC%(e);
	{ o.%EL_C%IndexIfCompact(uint()) } -> std::same_as<uint>;
	{ o.%EL_C%CompactIndices() } -> std::same_as<std::vector<int>>;

	{ o.add%EL_UC%() } -> std::same_as<uint>;
	{ o.add%EL_UC%s(uint()) } -> std::same_as<uint>;
	{ o.reserve%EL_UC%s(uint()) } -> std::same_as<void>;
	{ o.compact%EL_UC%s() } -> std::same_as<void>;

	{ o.%EL_C%Begin() } -> std::same_as<typename T::%EL_UC%Iterator>;
	{ co.%EL_C%Begin() } -> std::same_as<typename T::Const%EL_UC%Iterator>;
	{ o.%EL_C%End() } -> std::same_as<typename T::%EL_UC%Iterator>;
	{ co.%EL_C%End() } -> std::same_as<typename T::Const%EL_UC%Iterator>;

#ifdef VCLIB_USES_RANGES
	std::ranges::range<decltype(o.%EL_C%s())>;
	std::ranges::range<decltype(co.%EL_C%s())>;
#endif
};

} // namespace vcl::mesh

/**
 * @brief Has%EL_UC%s concepts is satisfied when at least one of its template types is (or inherits
 * from) a %EL_UC%Container. It can be used both to check if a Mesh has %EL_C%s, or if in a list of types
 * there is a %EL_UC%Container.
 *
 * In the following example, a MyMesh type can be instatiated only if one of its template Args is a
 * %EL_UC%Container:
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
 * MyMesh<vcl::VertexContainer<MyVertex>, vcl::%EL_UC%Container<My%EL_UC%>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) %EL_UC%Container:
 * @code{.cpp}
 * if constexpr (vcl::Has%EL_UC%s<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 *
 * @note This concept does not check if a Mesh is a valid %EL_UC%Mesh. To do that, use the %EL_UC%MeshConcept.
 */
template<typename... Args>
concept Has%EL_UC%s = (vcl::mesh::Has%EL_UC%Container<Args> || ...);

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_CONTAINERS_%EL_U%_CONTAINER_H
