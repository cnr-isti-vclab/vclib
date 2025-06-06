#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>

#define SHADING float depthOffset = 0.0;\
vec3 normal = normalize(v_normal);\
vec4 light = computeLight(u_lightDir, u_lightColor, normal);\
vec3 specular = computeSpecular(\
        v_position,\
        vec3(0.0, 0.0, 0.0),\
        u_lightDir,\
        u_lightColor,\
        normal);
