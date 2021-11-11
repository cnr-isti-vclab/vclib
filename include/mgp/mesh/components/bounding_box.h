/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_BOUNDING_BOX_H
#define MGP_MESH_COMPONENTS_BOUNDING_BOX_H

#include <mgp/space/box.h>

namespace mgp::comp {

class BoundingBoxTriggerer
{
};

/**
 * @brief The BoundingBox component class represent an axis aligned bounding box. This class is
 * usually used as a component of a Mesh.
 */
template<class PointType>
class BoundingBox : public BoundingBoxTriggerer
{
public:
	using BoundingBoxType = Box<PointType>;

	const BoundingBoxType boundingBox() const { return box; }
	BoundingBoxType&      boundingBox() { return box; }

private:
	Box<PointType> box;
};

using BoundingBox3f = BoundingBox<Point3f>;
using BoundingBox3d = BoundingBox<Point3d>;

/**
 * Detector to check if a class has (inherits) BoundingBox
 */

template<typename T>
using hasBoundingBoxT = std::is_base_of<BoundingBoxTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasBoundingBox = typename std::enable_if<hasBoundingBoxT<U>::value, T>::type;

template<typename T>
bool constexpr hasBoundingBox()
{
	return hasBoundingBoxT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_BOUNDING_BOX_H
