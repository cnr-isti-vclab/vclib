/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_H
#define MGP_MESH_FACE_H

#include "face/component.h"

namespace mgp {

template<class... Args>
class Face : public face::Component<Args>...
{
};

}

#endif // MGP_MESH_FACE_H
