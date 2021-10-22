/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
#define MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H

#include <vector>

#include "assert.h"

#include <mgp/misc/type_traits.h>

namespace mgp {
namespace mesh {

// declare the member function that you are looking for:
template<typename T>
using hasOptionalColor_t = decltype(std::declval<T&>().__optional_color__());

// create a type trait for that member in a templated T class
template <typename T>
using hasOptionalColor = typename detector<hasOptionalColor_t, void, T>::type;

template<typename, typename = void>
struct ColorMembers {};

template<typename T>
struct ColorMembers<T, std::enable_if_t<hasOptionalColor<T>::value>>
{
	bool isColorEnabled = false;
	std::vector<typename T::ColorType> colors;
};

template<typename T>
class OptionalComponentsVector
{
public:
	template<typename TT = void>
	typename std::enable_if<hasOptionalColor<T>::value, TT>::type
	enableColor(unsigned int size) {_c.isColorEnabled = true; _c.colors.resize(size);}

	//template<typename T = typename T::ColorType>
	//typename std::enable_if<hasOptionalColor<T>::value, TT>::type&
	//color(unsigned int i) {assert(_c.isColorEnabled); return _c.colors[i];}

	//template<typename TT = typename T::ColorType>
	//const typename std::enable_if<hasColor<T>::value, TT>::type&
	//color (unsigned int i) const {assert(_c.isColorEnabled); return _c.colors[i];}

private:
	ColorMembers<T> _c;
};

} // namespace mesh
} // namespace mgp

#endif // MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
