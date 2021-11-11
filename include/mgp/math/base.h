/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MATH_BASE_H
#define MGP_MATH_BASE_H

#include <cmath>

namespace mgp::math {

template <typename  T>
bool isDegenerate(T t) { return std::isinf(t) || std::isnan(t); }

}

#endif // MGP_MATH_BASE_H
