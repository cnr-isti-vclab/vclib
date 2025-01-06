#ifndef UTILS
#define UTILS

#include <bgfx_shader.sh>

vec4 screenToClip(vec4 coordinate, float screen_width, float screen_heigth) {
    return vec4(
                  (2 * coordinate.x / screen_width), 
                  (2 * coordinate.y / screen_heigth), 
                  coordinate.z, 
                  coordinate.w
                );
}

vec4 clipToScreen(vec4 coordinate, float screen_width, float screen_heigth) {
    return vec4(
                  (coordinate.x * screen_width) / 2, 
                  (coordinate.y * screen_heigth) / 2, 
                  coordinate.z, 
                  coordinate.w
                );
}

vec4 calculatePointWithMVP(vec4 p, float screen_width, float screen_heigth) {
    vec4 NDC_p = mul(u_modelViewProj, vec4(p.xyz, 1.0));

    vec4 screen_p = vec4(((NDC_p.xy / NDC_p.w)).xy, 0.0, 0.0);

    vec4 p_px = clipToScreen(screen_p, screen_width, screen_heigth);
    return vec4(p_px.xy, NDC_p.z, NDC_p.w);
}

#endif