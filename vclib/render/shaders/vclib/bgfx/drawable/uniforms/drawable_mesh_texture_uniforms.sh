#ifndef VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

// macro function that, given a 32 bit uint and pos that represent the index
// of the texture stage (each one of 4 bits), returns the value of that stage
#define textureStageBitField(value, pos) uint((value >> (pos * 4)) & 0xF)

#define u_textureStagesY floatBitsToUint(u_meshData.y)

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

// stages for each texture type are defined in the u_textureStagesY uniform
// you can view this unifor as an array of 8 uints, each one representing the
// stage index for a specific texture type (each uint is 4 bits)
// positions for each texture type:
// 0 -> base color
// 1 -> metallic roughness
// 2 -> normal
// 3 -> occlusion
// 4 -> emissive
// 5 -> brdf lut
//
// to get the actual stage index: textureStageBitField(u_textureStagesY, pos)

vec4 baseColorTex(vec2 texcoord)
{
    return textureStage(textureStageBitField(u_textureStagesY, 0), texcoord);
}

vec4 metallicRoughnessTex(vec2 texcoord)
{
    return textureStage(textureStageBitField(u_textureStagesY, 1), texcoord);
}

vec4 normalTex(vec2 texcoord)
{
    return textureStage(textureStageBitField(u_textureStagesY, 2), texcoord);
}

vec4 occlusionTex(vec2 texcoord)
{
    return textureStage(textureStageBitField(u_textureStagesY, 3), texcoord);
}

vec4 emissiveTex(vec2 texcoord)
{
    return textureStage(textureStageBitField(u_textureStagesY, 4), texcoord);
}

vec4 brdfLutTex(vec2 texcoord)
{
    return textureStage(textureStageBitField(u_textureStagesY, 5), texcoord);
}

#endif // VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH
