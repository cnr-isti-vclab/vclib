$input a_position, i_data0, i_data1, i_data2, i_data3
$output v_color, v_uv, v_length, v_normal, v_is_start_end

#include <bgfx_compute.sh>
#include "../../polylines.sh"

uniform vec4 u_data;

#define a_uv                    a_position

#define a_prev                  i_data0
#define a_curr                  vec4(i_data1.xyz, 0.0)
#define a_next                  i_data2
#define color                   uintToVec4FloatColor(floatBitsToUint(i_data1.w))
#define normal                  vec3(i_data3.xyz)

void main() {
    uint screenSize = floatBitsToUint(u_data.x);
    uint thickness_antialias_border_miterlimit = floatBitsToUint(u_data.y);
    uint caps_join = floatBitsToUint(u_data.w);
    
    float u_screenWidth  = float((screenSize >> uint(16)) & uint(0xFFFF));
    float u_screenHeigth = float(screenSize & uint(0xFFFF));

    float u_thickness    = float((thickness_antialias_border_miterlimit >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_miterlimit >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_miterlimit >> uint(8))  & uint(0xFF));
    float u_miter_limit  = float(thickness_antialias_border_miterlimit               & uint(0xFF));
    
    float u_leftCap      = float((caps_join >> uint(4))  & uint(0x3));
    float u_rigthCap     = float((caps_join >> uint(2))  & uint(0x3));
    float u_join         = float(caps_join               & uint(0x3));

    vec4 prev_px = calculatePointWithMVP(a_prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(a_curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(a_next, u_screenWidth, u_screenHeigth);

    v_color = color;
    v_normal = normal;
    v_uv = vec4(0);
    v_length = 0;

    bool is_start = false;
    bool is_end = false;
    v_is_start_end = 1;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
}
