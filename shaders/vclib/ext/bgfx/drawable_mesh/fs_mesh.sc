$input v_pos
$input v_normal
$input v_color

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>
#include <vclib/render/mesh_render_settings_macros.h>

uniform vec4 cameraEyePos;
uniform vec4 lightDir;
uniform vec4 lightColor;

uniform vec4 meshColor;
uniform vec4 userColor;
uniform vec4 drawMode;

BUFFER_RO(triangleColors, uint, 3);

vec4 unpackColor(uint color) {
    return vec4(
        float(color & uint(0x000000FF)) / 255.0,
        float((color & uint(0x0000FF00)) >> uint(8)) / 255.0,
        float((color & uint(0x00FF0000)) >> uint(16)) / 255.0,
        float((color & uint(0xFF000000)) >> uint(24)) / 255.0
        );
}

void main()
{
    uint drawModeInt = floatBitsToUint(drawMode.x);

    // compute light
    float ambientStrength = 0.2;
    float specularStrength = 0.3;
    // ambient
    vec3 ambient = (ambientStrength * lightColor).xyz;
    // diffuse
    float diff = max(dot(v_normal, lightDir.xyz), 0);
    vec3 diffuse = diff * lightColor.xyz;
    // specular
    vec3 viewDir = normalize(cameraEyePos.xyz - v_pos);
    vec3 reflectDir = reflect(-lightDir.xyz, v_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor.xyz;

    vec4 light = vec4(ambient + diffuse, 1.0);

    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_VERTEX))) {
        gl_FragColor = light * v_color + vec4(specular, 0);
        return;
    }
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_MESH))) {
        gl_FragColor = light * meshColor + vec4(specular, 0);
        return;
    }
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_FACE))) {
        gl_FragColor = light * unpackColor(triangleColors[gl_PrimitiveID]) + vec4(specular, 0);
        return;
    }

    gl_FragColor = light * userColor + vec4(specular, 0);
}
