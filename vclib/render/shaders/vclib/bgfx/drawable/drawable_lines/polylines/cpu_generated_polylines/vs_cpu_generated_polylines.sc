$input a_position, a_texcoord0, a_texcoord1, a_color0, a_texcoord2
$output v_color, v_uv, v_length

#include "../../polylines.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;
uniform vec4 u_color;

#define a_prev                  a_position
#define a_curr                  a_texcoord0
#define a_next                  a_texcoord1
#define a_uv                    a_texcoord2

#define u_screenWidth           u_data1.x
#define u_screenHeigth          u_data1.y
#define u_miter_limit           u_data1.z
#define u_thickness             u_data1.w

#define u_leftCap               u_data2.x
#define u_rigthCap              u_data2.y
#define u_join                  u_data2.z

void main() { 
    vec4 prev_px = calculatePointWithMVP(vec4(a_prev, 0.0), u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(vec4(a_curr, 0.0), u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(vec4(a_next, 0.0), u_screenWidth, u_screenHeigth);

    v_color = a_color0;
    v_length = length(((next_px - curr_px) * (1 - a_uv.x)) + ((curr_px - prev_px) * (a_uv.x)));

    v_uv = calculatePolylinesUV(vec4(a_prev, 0.0), vec4(a_curr, 0), vec4(a_next, 0), a_uv, u_thickness, v_length, u_leftCap, u_rigthCap, u_join);

    bool is_start = a_curr.x == a_prev.x && a_curr.y == a_prev.y;
    bool is_end = a_curr.x == a_next.x && a_curr.y == a_next.y;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
} 