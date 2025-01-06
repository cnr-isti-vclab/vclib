$input a_position
$output v_color, v_uv, v_length

#include <bgfx_compute.sh>
#include "../../polylines.sh"

BUFFER_RO(pointsBuffer, float, 1);
#define p(pos)      vec4(pointsBuffer[0 + ((pos) * 7)], pointsBuffer[1 + ((pos) * 7)], pointsBuffer[2 + ((pos) * 7)], 0.0)
#define color(pos)  vec4(pointsBuffer[3 + ((pos) * 7)], pointsBuffer[4 + ((pos) * 7)], pointsBuffer[5 + ((pos) * 7)], pointsBuffer[6 + ((pos) * 7)])

uniform vec4 u_data1;
uniform vec4 u_data2;
uniform vec4 u_color;
uniform vec4 u_IndirectData;

#define a_uv                    a_position 
#define u_screenWidth           u_data1.x
#define u_screenHeigth          u_data1.y
#define u_miter_limit           u_data1.z
#define u_thickness             u_data1.w

#define u_leftCap               u_data2.x
#define u_rigthCap              u_data2.y
#define u_join                  u_data2.z

void main() {
    vec4 a_prev = p(gl_InstanceID - sign(gl_InstanceID));
    vec4 a_curr = p(gl_InstanceID);
    vec4 a_next = p(gl_InstanceID + 1);
    vec4 a_nextnext = p(gl_InstanceID + 1 + sign(u_IndirectData.x - 1 - gl_InstanceID));

    vec4 color0 = color(gl_InstanceID);
    vec4 color1 = color(gl_InstanceID + 1);

    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    vec4 prev_px = calculatePointWithMVP(prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(next, u_screenWidth, u_screenHeigth);

    v_color = (color0 * (1 - a_uv.x)) + (color1 * a_uv.x);
    v_length = length(((next_px - curr_px) * (1 - a_uv.x)) + ((curr_px - prev_px) * (a_uv.x)));

    v_uv = calculatePolylinesUV(prev, curr, next, a_uv, u_thickness, v_length, u_leftCap, u_rigthCap, u_join);

    bool is_start = curr.x == prev.x && curr.y == prev.y;
    bool is_end = curr.x == next.x && curr.y == next.y;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
}
