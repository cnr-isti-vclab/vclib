// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
