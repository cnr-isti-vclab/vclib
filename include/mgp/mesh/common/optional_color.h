/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_COLOR_H
#define MGP_MESH_COMMON_OPTIONAL_COLOR_H

#include "optional_info.h"

namespace mgp {
namespace common {

template<typename T>
class OptionalColor : public virtual OptionalInfo<T>
{
};

} // namespace common
} // namespace mgp

#endif // MGP_MESH_COMMON_OPTIONAL_COLOR_H
