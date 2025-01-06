$input a_position, i_data0, i_data1, i_data2, i_data3, i_data4
$output v_color, v_uv, v_length

#include <bgfx_compute.sh>
#include "../../polylines.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;
uniform vec4 u_color;

#define a_uv                    a_position
#define a_prev                  i_data0
#define a_curr                  i_data1
#define a_next                  i_data2
#define a_nextnext              i_data3
#define color                   i_data4

#define u_screenWidth           u_data1.x
#define u_screenHeigth          u_data1.y
#define u_miter_limit           u_data1.z
#define u_thickness             u_data1.w

#define u_leftCap               u_data2.x
#define u_rigthCap              u_data2.y
#define u_join                  u_data2.z

void main() {
    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    vec4 prev_px = calculatePointWithMVP(prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(next, u_screenWidth, u_screenHeigth);

    vec4 color0 = vec4(
        float(((uint(color.x)) >> 24) & uint(255)) / 255.0,
        float(((uint(color.x)) >> 16) & uint(255)) / 255.0,
        float(((uint(color.x)) >> 8)  & uint(255)) / 255.0,
        1.0
    );

    vec4 color1 = vec4(
        float(((uint(color.y)) >> 24) & uint(255)) / 255.0,
        float(((uint(color.y)) >> 16) & uint(255)) / 255.0,
        float(((uint(color.y)) >> 8)  & uint(255)) / 255.0,
        1.0
    );

    v_color = (color0 * (1 - a_uv.x)) + (color1 * a_uv.x);
    v_length = length(((next_px - curr_px) * (1 - a_uv.x)) + ((curr_px - prev_px) * (a_uv.x)));

    v_uv = calculatePolylinesUV(prev, curr, next, a_uv, u_thickness, v_length, u_leftCap, u_rigthCap, u_join);

    bool is_start = curr.x == prev.x && curr.y == prev.y;
    bool is_end = curr.x == next.x && curr.y == next.y;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
}
