/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_COMPACTNESS_H
#define MGP_COMPACTNESS_H

#include <vector>

namespace mgp {

template <typename T, typename ...Args>
void compactVector(std::vector<T, Args...>& vec, const std::vector<int>& newIndices);

}

#include "compactness.cpp"

#endif // MGP_COMPACTNESS_H
