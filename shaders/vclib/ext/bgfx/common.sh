#ifndef VCL_EXT_BGFX_COMMON_SH
#define VCL_EXT_BGFX_COMMON_SH

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

vec4 uintToVec4Color(uint color)
{
    return vec4(
        float(color & uint(0x000000FF)) / 255.0,
        float((color & uint(0x0000FF00)) >> uint(8)) / 255.0,
        float((color & uint(0x00FF0000)) >> uint(16)) / 255.0,
        float((color & uint(0xFF000000)) >> uint(24)) / 255.0);
}

#endif // VCL_EXT_BGFX_COMMON_SH
