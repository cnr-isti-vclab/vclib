$input v_color, v_uv, v_length
#include <bgfx_shader.sh>

#include "../../polylines.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;

#define u_thickness           u_data1.w

#define u_leftCap             u_data2.x
#define u_rigthCap            u_data2.y
#define u_join           	  u_data2.z

void main() {
	vec4 color = calculatePolylinesColor(v_uv.xy, u_thickness, v_length, u_leftCap, u_rigthCap, u_join, v_color);
	if(color.w == 0)
		discard;
	else
		gl_FragColor = color;
}