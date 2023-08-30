#include "container_component.h"

namespace vcl::comp {

/*
 * Create a container of T objects.
 * If this Container is a static array, all its element will be initialized to
 * T(). If this Container is a dynamic vector, it will be an empty container.
 */
template<
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
ContainerComponent<CT, T, N, AD, El, o, TTVN, PT...>::ContainerComponent()
{
	if constexpr (!Base::IS_VERTICAL) {
		if constexpr (N >= 0) {
			// the void* nullptr here is ignored, since the component is
			// horizontal
			Base::data((void*)nullptr).fill(T());
		}
	}
}

/*
 * Create a container of Objects.
 * If this Container is a static array, all its element will be initialized to
 * T(). If this Container is a dynamic vector, it will be an empty container.
 */
template<
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
template<typename Comp>
void ContainerComponent<CT, T, N, AD, El, o, TTVN, PT...>::init(Comp* comp)
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of objects.
		container(comp).fill(T());
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of objects.
		container(comp).clear();
	}
}

template<
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
template<typename Comp>
Vector<T, N>& ContainerComponent<CT, T, N, AD, El, o, TTVN, PT...>::container(
	Comp* comp)
{
	if constexpr (HAS_ADDITIONAL_DATA) {
		return std::get<0>(Base::data(comp));
	}
	else {
		return Base::data(comp);
	}
}

template<
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
template<typename Comp>
const Vector<T, N>& ContainerComponent<CT, T, N, AD, El, o, TTVN, PT...>::
	container(const Comp* comp) const
{
	if constexpr (HAS_ADDITIONAL_DATA) {
		return std::get<0>(Base::data(comp));
	}
	else {
		return Base::data(comp);
	}
}

template<
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
template<typename Comp, typename AdDt>
AdDt& ContainerComponent<CT, T, N, AD, El, o, TTVN, PT...>::additionalData(
	Comp* comp) requires (HAS_ADDITIONAL_DATA)
{
	return std::get<1>(Base::data(comp));
}

template<
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
template<typename Comp, typename AdDt>
const AdDt& ContainerComponent<CT, T, N, AD, El, o, TTVN, PT...>::
	additionalData(const Comp* comp) const requires (HAS_ADDITIONAL_DATA)
{
	return std::get<1>(Base::data(comp));
}

} // namespace vcl::comp
