/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
 *                                                                           *
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
 
#ifndef VCL_COMPARATORS_H
#define VCL_COMPARATORS_H

#include <algorithm>
#include <utility>

namespace vcl {

/**
 * @brief The UnorderedPairComparator struct is an utility comparator to allow to sort unordered
 * std::pair<T, T>, that means that it is not important the order of the first and the second
 * element of the pair. This menas that pairs (1, 2) and (2, 1) are considered equal, and
 * (3, 1) < (2, 3).
 */
template<typename T>
struct UnorderedPairComparator
{
	bool operator()(const std::pair<T, T>& p1, const std::pair<T, T>& p2) const
	{
		if (std::min(p1.first, p1.second) < std::min(p2.first, p2.second)) {
			return true;
		}
		else if (std::min(p1.first, p1.second) == std::min(p2.first, p2.second)) {
			return std::max(p1.first, p1.second) < std::max(p2.first, p2.second);
		}
		else {
			return false;
		}
	}
};

} // namespace vcl

#endif // VCL_COMPARATORS_H
