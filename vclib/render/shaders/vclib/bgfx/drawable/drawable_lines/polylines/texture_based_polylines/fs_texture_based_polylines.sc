$input v_color, v_uv, v_length, v_normal, v_is_start_end
#include <bgfx_shader.sh>

#include "../../polylines.sh"

uniform vec4 u_data;

void main() {
    uint thickness_antialias_border_miterlimit = floatBitsToUint(u_data.y);
    uint caps_join_color = floatBitsToUint(u_data.w);

    float u_thickness    = float((thickness_antialias_border_miterlimit >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_miterlimit >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_miterlimit >> uint(8))  & uint(0xFF));
    
    float u_leftCap      = float((caps_join_color >> uint(6)) & uint(0x3));
    float u_rigthCap     = float((caps_join_color >> uint(4)) & uint(0x3));
    float u_join         = float((caps_join_color >> uint(2)) & uint(0x3));

	vec4 color = calculatePolylinesColor(v_uv.xy, u_thickness, v_length, u_leftCap, u_rigthCap, u_join, v_color, 1 - floor(v_is_start_end));
	if(color.w == 0)
		discard;
	else
		gl_FragColor = color;
}