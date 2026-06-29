// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_BUFFERS_INDIRECT_BUFFER_H
#define VCL_BGFX_BUFFERS_INDIRECT_BUFFER_H

#include "generic_buffer.h"

namespace vcl {

class IndirectBuffer : public GenericBuffer<bgfx::IndirectBufferHandle>
{
    using Base = GenericBuffer<bgfx::IndirectBufferHandle>;

public:
    using Base::swap;

    /**
     * @brief Empty constructor.
     *
     * It creates an invalid IndirectBuffer object.
     */
    IndirectBuffer() = default;

    /**
     * @brief Creates the indirect buffer with the given number of indirect
     * calls.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @param[in] num: the number of indirect calls.
     */
    void create(uint num)
    {
        destroy();

        mHandle = bgfx::createIndirectBuffer(num);
    }

    /**
     * @brief Returns the handle of the indirect buffer.
     * @return the handle of the indirect buffer.
     */
    bgfx::IndirectBufferHandle handle() const { return mHandle; }

    void bind(uint stage, bgfx::Access::Enum access) const
    {
        if (bgfx::isValid(mHandle))
            bgfx::setBuffer(stage, mHandle, access);
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_INDIRECT_BUFFER_H
