/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_H
#define MGP_MESH_FACE_H

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

namespace mgp::face {

/**
 * @brief The Property fallback class, will be used when the template argument given to the Face
 * is not one of the known Properties.
 *
 * This class will just make the given template argument a Base of the Face, becoming a feature of
 * the Face.
 */
template<class T>
class Property : public T
{
};

} // namespace mgp::face

namespace mgp {

// Dummy class used to detect the a mgp::Face regardless of its template arguments
class FaceTriggerer
{
};

template<class... Args>
class Face : public FaceTriggerer, public face::Property<Args>...
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

#endif // MGP_MESH_FACE_H
