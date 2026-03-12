#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_texture_uniforms.sh>

#define COLORING \
    vec4 color; \
    if (isBaseColorTextureAvailable()) \
        color = baseColorTex(v_texcoord1); \
    else \
        color = vec4(0.0, 0.0, 0.0, 1.0);
