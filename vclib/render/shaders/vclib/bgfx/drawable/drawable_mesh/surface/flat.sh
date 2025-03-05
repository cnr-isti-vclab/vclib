#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(primitiveNormals, float, VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

#define SHADING float depthOffset = 0.0;\
vec3 normal = vec3(\
        primitiveNormals[gl_PrimitiveID * 3],\
        primitiveNormals[gl_PrimitiveID * 3 + 1],\
        primitiveNormals[gl_PrimitiveID * 3 + 2]);\
normal = mul(u_modelView, vec4(normal, 0.0)).xyz;\
normal = normalize(normal);\
vec4 light = computeLight(u_lightDir, u_lightColor, normal);\
vec3 specular = computeSpecular(\
            v_position,\
            u_cameraEyePos,\
            u_lightDir,\
            u_lightColor,\
            normal);