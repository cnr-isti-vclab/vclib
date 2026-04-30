#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next lines with:

BUFFER_RO(primitiveNormals, float, VCL_MRB_PRIMITIVE_NORMAL_BUFFER);
*/
BUFFER_RO(primitiveNormals, float, 14);

#define primitiveID (u_firstChunkPrimitiveID + gl_PrimitiveID)

#define SHADING float depthOffset = 0.0;\
vec3 normal = vec3(\
        primitiveNormals[primitiveID * 3],\
        primitiveNormals[primitiveID * 3 + 1],\
        primitiveNormals[primitiveID * 3 + 2]);\
normal = normalize(mul(u_normalMatrix, normal));\
vec4 light = computeLight(u_lightDir, u_lightColor, normal);\
vec3 specular = computeSpecular(\
        v_position,\
        vec3(0.0, 0.0, 0.0),\
        u_lightDir,\
        u_lightColor,\
        normal);
