/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_RENDER_READ_BUFFER_TYPES_H
#define VCL_RENDER_READ_BUFFER_TYPES_H

#include <cstdint>
#include <functional>
#include <variant>
#include <vector>

namespace vcl {

struct ReadBufferTypes
{
    using FloatData          = std::vector<float>;
    using ByteData           = std::vector<uint8_t>;
    using ReadData           = std::variant<FloatData, ByteData>;
    using CallbackReadBuffer = std::function<void(const ReadData&)>;
};

} // namespace vcl

#endif // VCL_RENDER_READ_BUFFER_TYPES_H
