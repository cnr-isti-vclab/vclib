$input a_position
$output v_color, v_uv, v_length, v_normal

#include <bgfx_compute.sh>
#include "../../lines.sh"

BUFFER_RO(pointsBuffer, float, 1);

#define p(pos)        vec4(pointsBuffer[((pos) * 7) + 0], pointsBuffer[((pos) * 7) + 1], pointsBuffer[((pos) * 7) + 2], 0)
#define color(pos)    pointsBuffer[((pos) * 7) + 3]
#define normal(pos)   vec3(pointsBuffer[((pos) * 7) + 4], pointsBuffer[((pos) * 7) + 5], pointsBuffer[((pos) * 7) + 6]) 

uniform vec4 u_data;

#define uv                    a_position

void main() {
    vec4 u_general_color = uintToVec4FloatColor(floatBitsToUint(u_data.x));
    uint thickness_antialias_border_caps = floatBitsToUint(u_data.y);
    
    float u_screenWidth  = u_viewRect.z;
    float u_screenHeigth = u_viewRect.w;

    float u_thickness    = float((thickness_antialias_border_caps >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_caps >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_caps >> uint(8))  & uint(0xFF));
    float u_leftCap      = float((thickness_antialias_border_caps >> uint(4))  & uint(0x3));
    float u_rigthCap     = float((thickness_antialias_border_caps >> uint(2))  & uint(0x3));
    float u_color_to_use = float((thickness_antialias_border_caps >> uint(0))  & uint(0x3));

    vec4 p0      = p((gl_InstanceID * 2));
    vec4 p1      = p((gl_InstanceID * 2) + 1);
    vec4 color   = uintToVec4FloatColor(floatBitsToUint(color((gl_InstanceID * 2) + (1 * uv.x))));
    vec3 normal0 = normal(gl_InstanceID * 2);
    vec3 normal1 = normal((gl_InstanceID * 2) + 1);

    vec4 p0_px = calculatePointWithMVP(p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = calculatePointWithMVP(p1, u_screenWidth, u_screenHeigth);

    v_color = (color * (1 - sign(u_color_to_use))) + (u_general_color * sign(u_color_to_use));
    v_normal = (normal0 * (1 - sign(uv.x))) + (normal1 * uv.x);
    v_length = length(p1_px.xyz - p0_px.xyz);
    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap);
}