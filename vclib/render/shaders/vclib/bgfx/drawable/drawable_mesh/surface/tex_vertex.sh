#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

SAMPLER2D(baseColorTex, VCL_MRB_TEXTURE0);

#define COLORING vec4 color = texture2D(baseColorTex, v_texcoord0);
