#include "container_component.h"

namespace vcl::comp {

/*
 * Create a container of T objects.
 * If this Container is a static array, all its element will be initialized to T().
 * If this Container is a dynamic vector, it will be an empty container.
 */
template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
ContainerComponent<T, N, AD, El, o, TTVN, PT...>::ContainerComponent()
{
	if constexpr (!Base::IS_VERTICAL) {
		if constexpr (N >= 0) {
			Base::data((void*)nullptr).fill(T());
		}
	}
}

/*
 * Create a container of Objects.
 * If this Container is a static array, all its element will be initialized to T().
 * If this Container is a dynamic vector, it will be an empty container.
 */
template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
template<typename Comp>
void ContainerComponent<T, N, AD, El, o, TTVN, PT...>::init(Comp* comp)
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

template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
template<typename Comp>
Vector<T, N>& ContainerComponent<T, N, AD, El, o, TTVN, PT...>::container(Comp* comp)
{
	if constexpr (HAS_ADDITIONAL_DATA) {
		return std::get<0>(Base::data(comp));
	}
	else {
		return Base::data(comp);
	}
}

template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
template<typename Comp>
const Vector<T, N>& ContainerComponent<T, N, AD, El, o, TTVN, PT...>::container(const Comp* comp) const
{
	if constexpr (HAS_ADDITIONAL_DATA) {
		return std::get<0>(Base::data(comp));
	}
	else {
		return Base::data(comp);
	}
}

//template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
//template<typename Comp, typename AdDt>
//AdDt& ContainerComponent<T, N, AD, El, o, TTVN, PT...>::additionalData(Comp* comp)
//	requires(HAS_ADDITIONAL_DATA)
//{
//	return std::get<1>(Base::data(comp));
//}

//template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
//template<typename Comp, typename AdDt>
//const AdDt& ContainerComponent<T, N, AD, El, o, TTVN, PT...>::additionalData(const Comp* comp) const
//	requires(HAS_ADDITIONAL_DATA)
//{
//	return std::get<1>(Base::data(comp));
//}

template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
void ContainerComponent<T, N, AD, El, o, TTVN, PT...>::resize(uint n) requires (N < 0)
{
	container().resize(n);
}

template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
void ContainerComponent<T, N, AD, El, o, TTVN, PT...>::pushBack(const T& c) requires (N < 0)
{
	container().pushBack(c);
}

template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
void ContainerComponent<T, N, AD, El, o, TTVN, PT...>::insert(uint i, const T& c) requires (N < 0)
{
	container().insert(i, c);
}

template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
void ContainerComponent<T, N, AD, El, o, TTVN, PT...>::erase(uint i) requires (N < 0)
{
	container().erase(i);
}

template<typename T, int N, typename AD, typename El, bool o, bool TTVN, typename... PT>
void ContainerComponent<T, N, AD, El, o, TTVN, PT...>::clear() requires (N < 0)
{
	container().clear();
}

} // namespace vcl::comp
