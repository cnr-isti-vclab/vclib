#ifndef VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

// textures
SAMPLER2D(s_tex0, VCL_MRB_TEXTURE0);
SAMPLER2D(s_tex1, VCL_MRB_TEXTURE1);
SAMPLER2D(s_tex2, VCL_MRB_TEXTURE2);
SAMPLER2D(s_tex3, VCL_MRB_TEXTURE3);
SAMPLER2D(s_tex4, VCL_MRB_TEXTURE4);
SAMPLER2D(s_tex5, VCL_MRB_TEXTURE5);

vec4 baseColorTex(vec2 texcoord)
{
    return texture2D(s_tex0, texcoord);
}

vec4 metallicRoughnessTex(vec2 texcoord)
{
    return texture2D(s_tex1, texcoord);
}

vec4 normalTex(vec2 texcoord)
{
    return texture2D(s_tex2, texcoord);
}

vec4 occlusionTex(vec2 texcoord)
{
    return texture2D(s_tex3, texcoord);
}

vec4 emissiveTex(vec2 texcoord)
{
    return texture2D(s_tex4, texcoord);
}

vec4 brdfLutTex(vec2 texcoord)
{
    return texture2D(s_tex5, texcoord);
}

#endif // VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH
