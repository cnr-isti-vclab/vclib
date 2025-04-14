#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(triTextureIds, uint, VCL_MRB_TRIANGLE_TEXTURE_ID_BUFFER);

SAMPLER2D(s_tex0, VCL_MRB_TEXTURE0);
SAMPLER2D(s_tex1, VCL_MRB_TEXTURE1);
SAMPLER2D(s_tex2, VCL_MRB_TEXTURE2);
SAMPLER2D(s_tex3, VCL_MRB_TEXTURE3);
SAMPLER2D(s_tex4, VCL_MRB_TEXTURE4);
SAMPLER2D(s_tex5, VCL_MRB_TEXTURE5);
SAMPLER2D(s_tex6, VCL_MRB_TEXTURE6);
SAMPLER2D(s_tex7, VCL_MRB_TEXTURE7);

vec4 getColorFromTexture(uint texId, vec2 uv) {
    switch (texId) {
        case 0: return texture2D(s_tex0, uv);
        case 1: return texture2D(s_tex1, uv);
        case 2: return texture2D(s_tex2, uv);
        case 3: return texture2D(s_tex3, uv);
        case 4: return texture2D(s_tex4, uv);
        case 5: return texture2D(s_tex5, uv);
        case 6: return texture2D(s_tex6, uv);
        case 7: return texture2D(s_tex7, uv);
        default: return vec4(0.0, 0.0, 0.0, 1.0);
    }
}

#define COLORING vec4 color = getColorFromTexture(triTextureIds[gl_PrimitiveID], v_texcoord0);
