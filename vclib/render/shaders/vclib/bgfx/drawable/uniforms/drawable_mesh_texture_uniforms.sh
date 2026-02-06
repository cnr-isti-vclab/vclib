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

vec4 textureStage(uint stage, vec2 texcoord)
{
    switch (stage) {
        case 0u:
            return texture2D(s_tex0, texcoord);
        case 1u:
            return texture2D(s_tex1, texcoord);
        case 2u:
            return texture2D(s_tex2, texcoord);
        case 3u:
            return texture2D(s_tex3, texcoord);
        case 4u:
            return texture2D(s_tex4, texcoord);
        case 5u:
            return texture2D(s_tex5, texcoord);
        default:
            // should neve happen, return a magenta color to easily spot the error
            return vec4(1.0, 0.0, 1.0, 1.0);
    }
}

vec4 baseColorTex(vec2 texcoord)
{
    return textureStage(0u, texcoord);
}

vec4 metallicRoughnessTex(vec2 texcoord)
{
    return textureStage(1u, texcoord);
}

vec4 normalTex(vec2 texcoord)
{
    return textureStage(2u, texcoord);
}

vec4 occlusionTex(vec2 texcoord)
{
    return textureStage(3u, texcoord);
}

vec4 emissiveTex(vec2 texcoord)
{
    return textureStage(4u, texcoord);
}

vec4 brdfLutTex(vec2 texcoord)
{
    return textureStage(5u, texcoord);
}

#endif // VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH
