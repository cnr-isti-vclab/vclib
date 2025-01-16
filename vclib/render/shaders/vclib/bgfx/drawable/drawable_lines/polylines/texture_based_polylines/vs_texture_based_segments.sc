$input a_position
$output v_color, v_uv, v_length, v_normal, v_is_start_end

#include <bgfx_compute.sh>
#include "../../polylines.sh"

IMAGE2D_RO(textureBuffer, rgba32f, 0);

uniform vec4 u_data;
uniform vec4 u_IndirectData;

#define maxInstancingNum        u_IndirectData.x
#define maxTextureSize          u_IndirectData.y
#define a_uv                    a_position 

void main() {
    vec4 u_general_color = uintToVec4FloatColor(floatBitsToUint(u_data.x));
    uint thickness_antialias_border_miterlimit = floatBitsToUint(u_data.y);
    uint caps_join_color = floatBitsToUint(u_data.w);
    
    float u_screenWidth  = u_viewRect.z;
    float u_screenHeigth = u_viewRect.w;

    float u_thickness    = float((thickness_antialias_border_miterlimit >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_miterlimit >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_miterlimit >> uint(8))  & uint(0xFF));
    float u_miter_limit  = float(thickness_antialias_border_miterlimit               & uint(0xFF));
    
    float u_leftCap      = float((caps_join_color >> uint(6)) & uint(0x3));
    float u_rigthCap     = float((caps_join_color >> uint(4)) & uint(0x3));
    float u_join         = float((caps_join_color >> uint(2)) & uint(0x3));
    float u_color_to_use = float((caps_join_color)            & uint(0x3));

    vec4 element0     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 5), maxTextureSize));
    vec4 element1     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 5) + 1, maxTextureSize));
    vec4 element2     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 5) + 2, maxTextureSize));
    vec4 element3     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 5) + 3, maxTextureSize));
    vec4 element4     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 5) + 4, maxTextureSize));

    vec4 a_prev     = vec4(element0.xyz, 0);
    vec4 a_curr     = vec4(element1.xyz, 0);
    vec4 a_next     = vec4(element2.xyz, 0);
    vec4 a_nextnext = vec4(element3.xyz, 0);
    vec4 color0     = uintToVec4FloatColor(floatBitsToUint(element3.w));
    vec4 color1     = uintToVec4FloatColor(floatBitsToUint(element4.w));
    vec3 normal0    = vec3(element0.w, element1.w, element2.w);
    vec3 normal1    = vec3(element4.xyz);

    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    vec4 prev_px = calculatePointWithMVP(prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(next, u_screenWidth, u_screenHeigth);

    v_color = (((color0 * (1 - a_uv.x)) + (color1 * a_uv.x)) * (1 - sign(u_color_to_use))) + (u_general_color * sign(u_color_to_use));
    v_normal = (normal0 * (1 - a_uv.x)) + (normal1 * a_uv.x);
    v_length = length(((next_px - curr_px) * (1 - a_uv.x)) + ((curr_px - prev_px) * (a_uv.x)));

    v_uv = calculatePolylinesUV(prev, curr, next, a_uv, u_thickness, v_length, u_leftCap, u_rigthCap, u_join);
    
    bool is_start = curr.x == prev.x && curr.y == prev.y;
    bool is_end = curr.x == next.x && curr.y == next.y;
    v_is_start_end = (sign(length(curr - prev)) * (1 - a_uv.x)) + (sign(length(curr-next)) * a_uv.x);
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
}

