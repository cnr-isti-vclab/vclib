/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "compactness.h"

namespace vcl {

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
