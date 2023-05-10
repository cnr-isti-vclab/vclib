#ifndef VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H

#include <vclib/space/vector.h>

#include "component.h"

namespace vcl::comp {

namespace internal {

template<typename T, int N, typename AD, typename El, bool o, bool TT, typename... PT>
using ContCompBase = std::conditional_t<
	std::is_same_v<AD, void>,
	Component<Vector<T, N>, El, o, PT...>,
	Component<std::tuple<Vector<T, N>, AD>, El, o, PT...>>;

} // namespace vcl::comp::internal

template<
	typename T,
	int N,
	typename AdditionalData,
	typename ElementType,
	bool optional,
	bool TTVN,
	typename... PointedTypes>
class ContainerComponent :
		public internal::
			ContCompBase<T, N, AdditionalData, ElementType, optional, TTVN, PointedTypes...>
{
	static constexpr bool HAS_ADDITIONAL_DATA = !std::is_same_v<AdditionalData, void>;

	using Base =
		internal::ContCompBase<T, N, AdditionalData, ElementType, optional, TTVN, PointedTypes...>;

public:
	/**
	 * @brief Boolean that tells if this component stores a container having its size tied to the
	 * number of the vertices of the Element.
	 *
	 * E.g. suppose to have a Polygonal Face f, having 5 vertices (and 5 edges).
	 * This means that it the Face has the AdjacentFaces component, then it should store 5 adjacent
	 * faces (the same number of the vertices). In this case, the AdjacentFaces component will have
	 * the boolean TIED_TO_VERTEX_NUMBER set to true.
	 */
	static const bool TIED_TO_VERTEX_NUMBER = TTVN;

	static const int SIZE = N;

protected:
	/* Iterator Types declaration */

	using Iterator      = typename Vector<T, N>::Iterator;
	using ConstIterator = typename Vector<T, N>::ConstIterator;

	/* Constructor */

	ContainerComponent();

	template<typename Comp>
	void init(Comp* comp);

	template<typename Comp>
	Vector<T, N>& container(Comp* comp);

	template<typename Comp>
	const Vector<T, N>& container(const Comp* comp) const;

	// TODO: move in cpp file when MSVC and CLang will solve this bug
	template<typename Comp, typename AdDt = AdditionalData>
	AdDt& additionalData(Comp* comp) requires (HAS_ADDITIONAL_DATA)
	{
		return std::get<1>(Base::data(comp));
	}

	// TODO: move in cpp file when MSVC and CLang will solve this bug
	template<typename Comp, typename AdDt = AdditionalData>
	const AdDt& additionalData(const Comp* comp) const requires (HAS_ADDITIONAL_DATA)
	{
		return std::get<1>(Base::data(comp));
	}

	void resize(uint n) requires (N < 0);
	void pushBack(const T& c = T()) requires (N < 0);
	void insert(uint i, const T& c = T()) requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);
};

} // namespace vcl::comp

#include "container_component.cpp"

#endif // VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H
