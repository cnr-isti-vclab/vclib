/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_container.h"

namespace mgp::mesh {

template<class T>
Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::Container()
{
}

template<class T>
const typename Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::VertexType&
mgp::mesh::Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::vertex(unsigned int i) const
{
	return vertices[i];
}

template<class T>
typename Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::VertexType&
mgp::mesh::Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::vertex(unsigned int i)
{
	return vertices[i];
}

template<class T>
unsigned int Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::vertexNumber() const
{
	return vn;
}

template<class T>
void Container<T, mgp::ifIsBaseOf<VertexTriggerer, T> >::reserveVertices(unsigned int size)
{
	vertices.reserve(size);
	if constexpr (common::hasOptionalInfo<VertexType>::value) {
		optionalComponentsVector.reserve(size);
	}
}

template<class T>
template<class U>
typename std::enable_if<common::hasOptionalColor<U>::value, void>::type
Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::enableVertexColor()
{
	optionalComponentsVector.enableColor(vertexNumber());
}

template<class T>
template<class U>
typename std::enable_if<common::hasOptionalMutableBitFlags<U>::value, void>::type
Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::enableVertexMutableBitFlags()
{
	optionalComponentsVector.enableMutableBitFlags(vertexNumber());
}

template<class T>
template<class U>
typename std::enable_if<common::hasOptionalNormal<U>::value, void>::type
Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::enableVertexNormal()
{
	optionalComponentsVector.enableNormal(vertexNumber());
}

template<class T>
template<class U>
typename std::enable_if<common::hasOptionalScalar<U>::value, void>::type
Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::enableVertexScalar()
{
	optionalComponentsVector.enableScalar(vertexNumber());
}

template<class T>
unsigned int Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::addVertex()
{
	vertices.push_back(VertexType());
	++vn;
	vertices[vertices.size() - 1]._id = vertices.size() - 1;
	if constexpr (common::hasOptionalInfo<VertexType>::value) {
		vertices[vertices.size() - 1].setContainerPointer(&optionalComponentsVector);
		optionalComponentsVector.resize(vertices.size());
	}
	return vertices[vertices.size() - 1]._id;
}

} // namespace mgp::mesh
