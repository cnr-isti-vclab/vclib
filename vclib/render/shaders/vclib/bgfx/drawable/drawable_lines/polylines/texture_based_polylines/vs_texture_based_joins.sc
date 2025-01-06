$input a_position
$output v_color, v_uv, v_length

#include <bgfx_compute.sh>
#include "../../polylines.sh"

IMAGE2D_RO(textureBuffer, rgba32f, 0);

uniform vec4 u_data1;
uniform vec4 u_data2;
uniform vec4 u_color;
uniform vec4 u_IndirectData;

#define maxInstancingNum        u_IndirectData.x
#define maxTextureSize          u_IndirectData.y

#define a_uv                    a_position 
#define u_screenWidth           u_data1.x
#define u_screenHeigth          u_data1.y
#define u_miter_limit           u_data1.z
#define u_thickness             u_data1.w

#define u_leftCap               u_data2.x
#define u_rigthCap              u_data2.y
#define u_join                  u_data2.z

ivec2 calculateTextureCoord(uint index) {
    uint Y = index / maxTextureSize;
    uint X = index - (Y * maxTextureSize);
    return ivec2(X, Y);
}

void main() {
    uint index = gl_InstanceID + 1;

    vec4 prev = imageLoad(textureBuffer, calculateTextureCoord((index - 1) * 2));
    vec4 curr = imageLoad(textureBuffer, calculateTextureCoord(index * 2));
    vec4 next = imageLoad(textureBuffer, calculateTextureCoord((index + 1) * 2));

    vec4 color = imageLoad(textureBuffer, calculateTextureCoord((index * 2) + 1));

    vec4 prev_px = calculatePointWithMVP(prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(next, u_screenWidth, u_screenHeigth);

    v_color = color;
    v_uv = vec4(0);
    v_length = 0;
    bool is_start = curr.x == prev.x && curr.y == prev.y;
    bool is_end = curr.x == next.x && curr.y == next.y;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
}
