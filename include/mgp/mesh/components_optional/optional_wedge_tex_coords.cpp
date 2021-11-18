/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "optional_wedge_tex_coords.h"

#include <algorithm>

namespace mgp::comp {

template<typename Scalar, int N, typename T>
OptionalWedgeTexCoords<Scalar, N, T>::OptionalWedgeTexCoords()
{
}

template<typename Scalar, int N, typename T>
mgp::TexCoord<Scalar>& OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoord(unsigned int i)
{
	assert(i < wedgeTexCoordsNumber());
	return B::optCont().wedgeTexCoords(thisId())[i];
}

template<typename Scalar, int N, typename T>
const mgp::TexCoord<Scalar>&
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoord(unsigned int i) const
{
	assert(i < wedgeTexCoordsNumber());
	return B::optCont().wedgeTexCoords(thisId())[i];
}

template<typename Scalar, int N, typename T>
mgp::TexCoord<Scalar>& OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordMod(int i)
{
	unsigned int n = wedgeTexCoordsNumber();
	return B::optCont().wedgeTexCoords(thisId())[(i % n + n) % n];
}

template<typename Scalar, int N, typename T>
const mgp::TexCoord<Scalar>& OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordMod(int i) const
{
	unsigned int n = wedgeTexCoordsNumber();
	return B::optCont().wedgeTexCoords(thisId())[(i % n + n) % n];
}

template<typename Scalar, int N, typename T>
void OptionalWedgeTexCoords<Scalar, N, T>::setWedgeTexCoord(
	const mgp::TexCoord<Scalar>& t,
	unsigned int                 i)
{
	assert(i < wedgeTexCoordsNumber());
	B::optCont().wedgeTexCoords(thisId())[i] = t;
}

template<typename Scalar, int N, typename T>
void OptionalWedgeTexCoords<Scalar, N, T>::setWedgeTexCoords(
	const std::vector<mgp::TexCoord<Scalar>>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		unsigned int i = 0;
		for (const auto& t : list) {
			setWedgeTexCoord(t, i);
			++i;
		}
	}
	else {
		B::optCont().wedgeTexCoords(thisId()) = list;
	}
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::WedgeTexCoordsIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordBegin()
{
	return B::optCont().wedgeTexCoords(thisId()).begin();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::WedgeTexCoordsIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordEnd()
{
	return B::optCont().wedgeTexCoords(thisId()).end();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::ConstWedgeTexCoordsIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordBegin() const
{
	return B::optCont().wedgeTexCoords(thisId()).begin();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::ConstWedgeTexCoordsIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordEnd() const
{
	return B::optCont().wedgeTexCoords(thisId()).end();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::WedgeTexCoordsRangeIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordIterator()
{
	return WedgeTexCoordsRangeIterator(
		B::optCont().wedgeTexCoords(thisId()),
		&OptionalWedgeTexCoords::wedgeTexCoordBegin,
		&OptionalWedgeTexCoords::wedgeTexCoordEnd);
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::ConstWedgeTexCoordsRangeIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordIterator() const
{
	return ConstWedgeTexCoordsRangeIterator(
		B::optCont().wedgeTexCoords(thisId()),
		&OptionalWedgeTexCoords::wedgeTexCoordBegin,
		&OptionalWedgeTexCoords::wedgeTexCoordEnd);
}

template<typename Scalar, int N, typename T>
bool OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordsEnabled() const
{
	return B::optCont().isWedgeTexCoordsEnabled();
}

template<typename Scalar, int N, typename T>
unsigned int OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordsNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::optCont().wedgeTexCoords(thisId()).size();
	}
}

template<typename Scalar, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalWedgeTexCoords<Scalar, N, T>::resizeWedgeTexCoords(unsigned int n)
{
	B::optCont().wedgeTexCoords(thisId()).resize(n);
}

template<typename Scalar, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalWedgeTexCoords<Scalar, N, T>::pushWedgeTexCoord(const mgp::TexCoord<Scalar>& t)
{
	B::optCont().wedgeTexCoords(thisId()).push_back(t);
}

template<typename Scalar, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalWedgeTexCoords<Scalar, N, T>::insertWedgeTexCoord(
	unsigned int                 i,
	const mgp::TexCoord<Scalar>& t)
{
	assert(i < wedgeTexCoordsNumber());
	B::optCont().wedgeTexCoords(thisId()).insert(
		B::optCont().wedgeTexCoords(thisId()).begin() + i, t);
}

template<typename Scalar, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalWedgeTexCoords<Scalar, N, T>::eraseWedgeTexCoord(unsigned int i)
{
	assert(i < wedgeTexCoordsNumber());
	B::optCont().wedgeTexCoords(thisId()).erase(B::optCont().wedgeTexCoords(thisId()).begin() + i);
}

template<typename Scalar, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalWedgeTexCoords<Scalar, N, T>::clearWedgeTexCoord()
{
	B::optCont().wedgeTexCoords(thisId()).clear();
}

} // namespace mgp::comp
