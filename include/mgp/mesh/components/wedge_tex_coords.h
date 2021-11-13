/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define MGP_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include "generic_container.h"

#include <mgp/space/tex_coord.h>

namespace mgp::comp {

class WedgeTexCoordsTriggerer
{
};

template<typename Scalar, int N>
class WedgeTexCoords :
		protected GenericContainer<mgp::TexCoord<Scalar>, N>,
		public WedgeTexCoordsTriggerer
{
	using Base = GenericContainer<mgp::TexCoord<Scalar>, N>;

public:
	static const int WEDGE_TEXCOORD_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using WedgeTexCoordIterator           = typename Base::GCIterator;
	using ConstWedgeTexCoordIterator      = typename Base::ConstGCIterator;
	using WedgeTexCoordRangeIterator      = typename Base::GCRangeIterator;
	using ConstWedgeTexCoordRangeIterator = typename Base::ConstGCRangeIterator;

	WedgeTexCoords() : Base() {};

	/** Member functions **/

	mgp::TexCoord<Scalar>&       wedgeTexCoord(unsigned int i) { return Base::at(i); }
	const mgp::TexCoord<Scalar>& wedgeTexCoord(unsigned int i) const { return Base::at(i); }

	mgp::TexCoord<Scalar>&       wedgeTexCoordMod(int i) { return Base::atMod(i); }
	const mgp::TexCoord<Scalar>& wedgeTexCoordMod(int i) const { return Base::atMod(i); }

	void setWedgeTexCoord(const mgp::TexCoord<Scalar>& t, unsigned int i) { Base::set(t, i); }
	void setWedgeTexCoords(const std::vector<mgp::TexCoord<Scalar>>& list) { Base::set(list); }

	/** Iterator Member functions **/

	WedgeTexCoordIterator           wedgeTexCoordBegin() { return Base::begin(); }
	WedgeTexCoordIterator           wedgeTexCoordEnd() { return Base::end(); }
	ConstWedgeTexCoordIterator      wedgeTexCoordBegin() const { return Base::begin(); }
	ConstWedgeTexCoordIterator      wedgeTexCoordEnd() const { return Base::end(); }
	WedgeTexCoordRangeIterator      wedgeTexCoordIterator() { return Base::rangeIterator(); }
	ConstWedgeTexCoordRangeIterator wedgeTexCoordIterator() const { return Base::rangeIterator(); }

protected:
	using WedgeTexCoordScalarType = Scalar;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeWedgeTexCoords(unsigned int n)
	{
		Base::resize(n);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushWedgeTexCoord(const mgp::TexCoord<Scalar>& t)
	{
		Base::pushBack(t);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void>
	insertWedgeTexCoord(unsigned int i, const mgp::TexCoord<Scalar>& t)
	{
		Base::insert(i, t);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseWedgeTexCoord(unsigned int i)
	{
		Base::erase(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearWedgeTexCoord()
	{
		Base::clear();
	}
};

template<typename T>
using hasWedgeTexCoordsT = std::is_base_of<WedgeTexCoordsTriggerer, T>;

template<typename T>
bool constexpr hasWedgeTexCoords()
{
	return hasWedgeTexCoordsT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
