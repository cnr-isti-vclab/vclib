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

vec4 unpackColor(uint color)
{
    return vec4(
        float(color & uint(0x000000FF)) / 255.0,
        float((color & uint(0x0000FF00)) >> uint(8)) / 255.0,
        float((color & uint(0x00FF0000)) >> uint(16)) / 255.0,
        float((color & uint(0xFF000000)) >> uint(24)) / 255.0
        );
}

vec4 computeLight(vec3 lightDir, vec3 lightColor, vec3 normal)
{
    float ambientStrength = 0.2;
    // ambient
    vec3 ambient = ambientStrength * lightColor;
    // diffuse
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * lightColor;

    return vec4(ambient + diffuse, 1.0);
}

vec3 computeSpecular(
    vec3 vPos,
    vec3 cameraEyePos,
    vec3 lightDir,
    vec3 lightColor,
    vec3 normal)
{
    float specularStrength = 0.3;
    vec3 viewDir = normalize(cameraEyePos - vPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * lightColor;
}

void main()
{
    uint drawModeInt = floatBitsToUint(drawMode.x);

    // if not drawing mesh, discard
    if (!bool(drawModeInt & VCL_MRS_DRAW_MESH)) {
        discard;
    }

    /***** compute light ******/
    // default values
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    vec3 normal = v_normal;

    // if flat shading, compute normal of face
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_SHADING_FLAT))) {
        vec3 X = dFdx(v_pos);
        vec3 Y = dFdy(v_pos);
        normal=normalize(cross(X,Y));
    }

    // if flat or smooth shading, compute light
    if (!bool(drawModeInt & (VCL_MRS_DRAW_SURF_SHADING_NONE))) {
        light = computeLight(lightDir.xyz, lightColor.xyz, normal);


        specular = computeSpecular(
            v_pos,
            cameraEyePos.xyz,
            lightDir.xyz,
            lightColor.xyz,
            normal);
    }

    /***** compute color ******/
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

    /***** compute wireframe ******/
    if (bool(drawModeInt & (VCL_MRS_DRAW_WIREFRAME))) {
        float wfThickness = 1;
        vec3 wfColor = vec3(1.0, 1.0, 1.0);

       vec3 fw = abs(dFdx(color)) + abs(dFdy(color));
       vec3 val = smoothstep(vec3_splat(0.0), fw*wfThickness, color);
       float edge = min(min(val.x, val.y), val.z);

       vec3 edgeCol = mix(color.xyz, wfColor, 1.0);
       color.xyz = mix(wfColor, color.xyz, edge);
    }

    gl_FragColor = light * color + vec4(specular, 0);
}
