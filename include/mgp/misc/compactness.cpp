/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "compactness.h"

namespace mgp {

/**
 * @brief compactVector will take care of compacting the vector vec, depending on the content of the
 * vector newIndices.
 *
 * Given the vector newIndices having the following features:
 * - have the same size of vec
 * - for each position i:
 *   - newIndices[i] contains the new position of the element vec[i] after the compactness
 *   - newIndices[i] contains a value < 0 if the element vec[i] must be deleted
 *
 * Non-negative elements of newIndices must unique, and their value must be less than the new size
 * of vec after the compactness. The new size of vec will be the number of non-negative elements of
 * newIndices.
 *
 * @param vec
 * @param newIndices
 */
template<typename T, typename...Args>
void compactVector(std::vector<T, Args...>& vec, const std::vector<int>& newIndices)
{
	assert(vec.size() == newIndices.size());
	unsigned int newSize = 0;
	for (unsigned int i = 0; i < newIndices.size(); ++i){
		if (newIndices[i] >= 0){
			++newSize;
			if ((unsigned int)newIndices[i] != i) {
				//must move the element from position i to position newIndices[i]
				vec[newIndices[i]] = std::move(vec[i]);
			}
		}
	}
	vec.resize(newSize);
}

}
