/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_FACE_H
#define MGP_MESH_ELEMENTS_FACE_H

#include "face_properties.h"
#include "face_properties_optional.h"

namespace mgp::mesh {

// Container class declaration
template<typename, typename>
class Container;

// FaceOptionalContainer class declaration
template<typename, typename>
class FaceOptionalContainer;

} // namespace mgp::mesh

namespace mgp {

// Dummy class used to detect the a mgp::Face regardless of its template arguments
class FaceTriggerer
{
};

template<class... Args>
class Face : public FaceTriggerer, public Args...
{
	template<typename, typename>
	friend class mesh::Container;

	template<typename, typename>
	friend class mesh::FaceOptionalContainer;
public:
	unsigned int id() const { return _id; }
protected:
	Face() {}
	unsigned int _id = 0;
};

} // namespace mgp

#endif // MGP_MESH_ELEMENTS_FACE_H
