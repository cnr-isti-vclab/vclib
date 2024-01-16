#ifndef VCL_EXT_BGFX_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_SH

uniform vec4 u_lightDirPack;
uniform vec4 u_lightColorPack;

#define u_lightDir u_lightDirPack.xyz
#define u_lightColor u_lightColorPack.xyz

#endif // VCL_EXT_BGFX_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_SH
