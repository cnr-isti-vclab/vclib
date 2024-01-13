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
uniform vec4 userSurfaceColor;
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

    // if not drawing mesh, discard
    if (!bool(drawModeInt & VCL_MRS_DRAW_MESH)) {
        discard;
    }

    vec3 normal = v_normal;

    // if flat shading, compute normal of face
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_FLAT))) {
        vec3 X = dFdx(v_pos);
        vec3 Y = dFdy(v_pos);
        normal=normalize(cross(X,Y));
    }

    // compute light
    float ambientStrength = 0.2;
    float specularStrength = 0.3;
    // ambient
    vec3 ambient = (ambientStrength * lightColor).xyz;
    // diffuse
    float diff = max(dot(normal, lightDir.xyz), 0);
    vec3 diffuse = diff * lightColor.xyz;
    // specular
    vec3 viewDir = normalize(cameraEyePos.xyz - v_pos);
    vec3 reflectDir = reflect(-lightDir.xyz, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor.xyz;

    vec4 light = vec4(ambient + diffuse, 1.0);

    vec4 color = userSurfaceColor;

    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_VERTEX))) {
        color = v_color;
    }
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_MESH))) {
        color = meshColor;
    }
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_FACE))) {
        color = unpackColor(triangleColors[gl_PrimitiveID]);
    }

    if (bool(drawModeInt & (VCL_MRS_DRAW_WIREFRAME))) {
        float wfThickness = 1;
        vec3 wfColor = vec3(1.0, 1.0, 1.0);

       vec3 fw = abs(dFdx(color)) + abs(dFdy(color));
       vec3 val = smoothstep(vec3_splat(0.0), fw*wfThickness, color);
       float edge = min(min(val.x, val.y), val.z); // Gets to 0.0 when close to edges.

       vec3 edgeCol = mix(color.xyz, wfColor, 1.0);
       color.xyz = mix(wfColor, color.xyz, edge);
    }

    gl_FragColor = light * color + vec4(specular, 0);
}
