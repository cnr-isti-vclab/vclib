$input a_position, a_texcoord0, a_color0, a_normal, a_texcoord1
$output v_color, v_uv, v_length

#include <bgfx_shader.sh>
#include "../../lines.sh"

uniform vec4 u_data;

#define p0                    a_position
#define p1                    a_texcoord0
#define uv                    vec2(float(a_texcoord1.x), float(a_texcoord1.y))

void main() {
    uint screenSize = floatBitsToUint(u_data.x);
    uint thickness_antialias_border_caps = floatBitsToUint(u_data.y);
    
    float u_screenWidth  = float((screenSize >> uint(16)) & uint(0xFFFF));
    float u_screenHeigth = float(screenSize & uint(0xFFFF));

    float u_thickness    = float((thickness_antialias_border_caps >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_caps >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_caps >> uint(8))  & uint(0xFF));
    float u_leftCap      = float((thickness_antialias_border_caps >> uint(2))  & uint(0x3));
    float u_rigthCap     = float(thickness_antialias_border_caps               & uint(0x3));

    vec4 p0_px = calculatePointWithMVP(vec4(p0, 0.0), u_screenWidth, u_screenHeigth);
    vec4 p1_px = calculatePointWithMVP(vec4(p1, 0.0), u_screenWidth, u_screenHeigth);
    
    v_color = a_color0;
    v_length = length(p1_px - p0_px);

    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap);
}
