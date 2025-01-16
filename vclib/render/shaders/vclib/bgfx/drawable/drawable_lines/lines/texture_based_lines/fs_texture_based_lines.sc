$input v_color, v_uv, v_length, v_normal
#include <bgfx_shader.sh>

#include "../../lines.sh"

uniform vec4 u_data;

void main() {
    uint thickness_antialias_border_caps = floatBitsToUint(u_data.y);
	vec4 u_borderColor   = uintToVec4FloatColor(floatBitsToUint(u_data.z));

    float u_thickness    = float((thickness_antialias_border_caps >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_caps >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_caps >> uint(8))  & uint(0xFF));
    float u_leftCap      = float((thickness_antialias_border_caps >> uint(4))  & uint(0x3));
    float u_rigthCap     = float((thickness_antialias_border_caps >> uint(2))  & uint(0x3));
	
	vec4 color = calculateLinesColor(v_uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap, v_color, u_borderColor);

	if(color.w == 0) 
		discard;
	else
		gl_FragColor = color;
}