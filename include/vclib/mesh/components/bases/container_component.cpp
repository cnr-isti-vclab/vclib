#include "container_component.h"

namespace vcl::comp {

/*
 * Create a container of T objects.
 * If this Container is a static array, all its element will be initialized to
 * T(). If this Container is a dynamic vector, it will be an empty container.
 */
template<
	typename DC,
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
ContainerComponent<DC, CT, T, N, AD, El, o, TTVN, PT...>::ContainerComponent()
{
	if constexpr (!Base::IS_VERTICAL) {
		if constexpr (N >= 0) {
			Base::data().fill(T());
		}
	}
}

/*
 * Create a container of Objects.
 * If this Container is a static array, all its element will be initialized to
 * T(). If this Container is a dynamic vector, it will be an empty container.
 */
template<
	typename DC,
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
void ContainerComponent<DC, CT, T, N, AD, El, o, TTVN, PT...>::init()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of objects.
		container().fill(T());
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of objects.
		container().clear();
	}
}

template<
	typename DC,
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
Vector<T, N>&
ContainerComponent<DC, CT, T, N, AD, El, o, TTVN, PT...>::container()
{
	if constexpr (HAS_ADDITIONAL_DATA) {
		return std::get<0>(Base::data());
	}
	else {
		return Base::data();
	}
}

template<
	typename DC,
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
const Vector<T, N>&
ContainerComponent<DC, CT, T, N, AD, El, o, TTVN, PT...>::container() const
{
	if constexpr (HAS_ADDITIONAL_DATA) {
		return std::get<0>(Base::data());
	}
	else {
		return Base::data();
	}
}

template<
	typename DC,
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
template<typename AdDt>
AdDt& ContainerComponent<DC, CT, T, N, AD, El, o, TTVN, PT...>::additionalData()
	requires (HAS_ADDITIONAL_DATA)
{
	return std::get<1>(Base::data());
}

template<
	typename DC,
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TTVN,
	typename... PT>
template<typename AdDt>
const AdDt&
ContainerComponent<DC, CT, T, N, AD, El, o, TTVN, PT...>::additionalData() const
	requires (HAS_ADDITIONAL_DATA)
{
	return std::get<1>(Base::data());
}

} // namespace vcl::comp
