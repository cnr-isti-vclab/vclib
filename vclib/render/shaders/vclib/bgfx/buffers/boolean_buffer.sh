// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_BUFFERS_BOOLEAN_BUFFER_SH
#define VCL_BGFX_BUFFERS_BOOLEAN_BUFFER_SH

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

// Given a buffer composed of uints where each bit represents a boolean,
// returns the value of the bit at the given index.
// (bits are ordered MSB first: bit 31 = index 0)
#define getBoolFromBuffer(buffer, index) \
    ( (((buffer)[(index) / 32u] & (0x1u << (31u - ((index) % 32u)))) != 0u) )

// Sets the bit at the given index to 1 using an atomic OR operation.
// Requires the buffer to be bound as a read/write compute buffer.
#define atomicSetBoolInBuffer(buffer, index) \
    { uint _bgfx_tmp; atomicFetchAndOr(buffer[(index) / 32u], 0x1u << (31u - ((index) % 32u)), _bgfx_tmp); }

// Clears the bit at the given index to 0 using an atomic AND operation.
// Requires the buffer to be bound as a read/write compute buffer.
#define atomicClearBoolInBuffer(buffer, index) \
    { uint _bgfx_tmp; atomicFetchAndAnd(buffer[(index) / 32u], ~(0x1u << (31u - ((index) % 32u))), _bgfx_tmp); }

// Toggles the bit at the given index using an atomic XOR operation.
// Requires the buffer to be bound as a read/write compute buffer.
#define atomicToggleBoolInBuffer(buffer, index) \
    { uint _bgfx_tmp; atomicFetchAndXor(buffer[(index) / 32u], 0x1u << (31u - ((index) % 32u)), _bgfx_tmp); }

#endif // VCL_BGFX_BUFFERS_BOOLEAN_BUFFER_SH
