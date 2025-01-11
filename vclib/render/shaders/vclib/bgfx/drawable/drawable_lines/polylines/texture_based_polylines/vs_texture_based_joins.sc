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

    vec4 prev    = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 4), maxTextureSize));
    vec4 curr    = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 4) + 1, maxTextureSize));
    vec4 next    = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 4) + 2, maxTextureSize));
    vec4 normal  = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 2) + 3, maxTextureSize));
    vec4 color   = uintToVec4FloatColor(floatBitsToUint(curr.w));
    
    curr.w = 0;

    vec4 prev_px = calculatePointWithMVP(prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(next, u_screenWidth, u_screenHeigth);

    v_color = color;
    v_normal = normal;
    v_uv = vec4(0);
    v_length = 0;
    
    bool is_start = false;
    bool is_end = false;
    v_is_start_end = 1;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
}
