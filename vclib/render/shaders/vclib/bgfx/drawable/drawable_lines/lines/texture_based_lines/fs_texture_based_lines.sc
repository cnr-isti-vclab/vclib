$input v_color, v_uv, v_length
#include <bgfx_shader.sh>

#include "../../lines.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;
uniform vec4 u_borderColor;

#define u_thickness           u_data1.z
#define u_leftCap             u_data1.w
#define u_rigthCap            u_data2.x

#define u_antialias           u_data2.y
#define u_border              u_data2.z

void main() {
	vec4 color = calculateLinesColor(v_uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap, v_color, u_borderColor);

	if(color.w == 0) 
		discard;
	else
		gl_FragColor = color;

}