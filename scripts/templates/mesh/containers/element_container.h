%HEADER%

#ifndef VCL_MESH_CONTAINER_%EL_U%_CONTAINER_H
#define VCL_MESH_CONTAINER_%EL_U%_CONTAINER_H

#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/mesh/elements/%EL%.h>
#include <vclib/mesh/elements/%EL%_components.h>

#include "element_container.h"

namespace vcl::mesh {

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
	 * @brief Empty constructor that creates an empty container of %EL_UC%s.
	 */
	%EL_UC%Container() = default;

	const %EL_UC%Type& %EL_C%(uint i) const;
	%EL_UC%Type&       %EL_C%(uint i);

	uint %EL_C%Number() const;
	uint %EL_C%ContainerSize() const;
	uint deleted%EL_UC%Number() const;

	uint add%EL_UC%();
	uint add%EL_UC%s(uint n);

	void clear%EL_UC%s();
	void resize%EL_UC%s(uint n);
	void reserve%EL_UC%s(uint n);

	void compact%EL_UC%s();

	void delete%EL_UC%(uint i);
	void delete%EL_UC%(const %EL_UC%Type* e);

	uint              %EL_C%IndexIfCompact(uint id) const;
	std::vector<uint> %EL_C%CompactIndices() const;

	void update%EL_UC%Indices(const std::vector<uint>& newIndices);

	%EL_UC%Iterator      %EL_C%Begin(bool jumpDeleted = true);
	%EL_UC%Iterator      %EL_C%End();
	Const%EL_UC%Iterator %EL_C%Begin(bool jumpDeleted = true) const;
	Const%EL_UC%Iterator %EL_C%End() const;
	auto              %EL_C%s(bool jumpDeleted = true);
	auto              %EL_C%s(bool jumpDeleted = true) const;

	void enableAllPer%EL_UC%OptionalComponents();
	void disableAllPer%EL_UC%OptionalComponents();

%OPTIONAL_COMPONENTS%

};

} // namespace vcl::mesh

#include "%EL%_container.cpp"

#endif // VCL_MESH_CONTAINER_%EL_U%_CONTAINER_H
