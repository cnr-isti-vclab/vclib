$input a_position, i_data0, i_data1, i_data2, i_data3
$output v_color, v_uv, v_length

#include <bgfx_shader.sh>
#include "../../lines.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;

#define p0                    i_data0
#define p1                    i_data1
#define color0                i_data2
#define color1                i_data3
#define uv                    a_position

#define u_screenWidth         u_data1.x
#define u_screenHeigth        u_data1.y
#define u_thickness           u_data1.z

#define u_leftCap             u_data1.w
#define u_rigthCap            u_data2.x

#define u_antialias           u_data2.y
#define u_border              u_data2.z

void main() {
    vec4 p0_px = calculatePointWithMVP(p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = calculatePointWithMVP(p1, u_screenWidth, u_screenHeigth);
    
    v_color = (color0 * (1 - uv.x)) + (color1 * uv.x);
    v_length = length(p1_px - p0_px);
    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap);
}
 