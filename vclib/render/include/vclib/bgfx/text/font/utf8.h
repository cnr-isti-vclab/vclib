// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#ifndef VCL_BGFX_TEXT_FONT_UTF8_H
#define VCL_BGFX_TEXT_FONT_UTF8_H

#include <stdint.h>

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

namespace bgfx {

uint32_t utf8_decode(uint32_t* _state, uint32_t* _codep, uint8_t _ch);

} // namespace bgfx

#endif // VCL_BGFX_TEXT_FONT_UTF8_H
