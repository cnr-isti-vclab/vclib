/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
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

#ifndef VCL_MISC_BIT_PROXY_H
#define VCL_MISC_BIT_PROXY_H

#include "types.h"

namespace vcl {

/**
 * @brief The BitProxy class allows to access to a bool reference from a bit saved in a mask,
 * and then allow assignment.
 *
 * See: https://stackoverflow.com/a/10145050/5851101
 */
class BitProxy
{
public:
	BitProxy(int& mask, uint index) : mask(mask), index(index) {}

	void setIndex(uint ind) { index = ind; }

	operator bool() const { return mask.get() & (1 << index); }

	void operator=(bool bit) { mask.get() = (mask.get() & ~(bit << index)) | (bit << index); }

	BitProxy& operator|=(bool bit)
	{
		mask.get() |= (bit << index);
		return *this;
	}

	BitProxy& operator&=(bool bit)
	{
		mask.get() &= ~(bit << index);
		return *this;
	}

	BitProxy& operator/=(bool bit)
	{
		mask.get() ^= (bit << index);
		return *this;
	}

private:
	std::reference_wrapper<int> mask;
	uint index;
};

} // namespace vcl

#endif // VCL_MISC_BIT_PROXY_H
