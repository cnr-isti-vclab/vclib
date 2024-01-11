$input a_position
$input a_normal
$input a_color0

$output v_pos, v_normal, v_color

#include <bgfx_shader.sh>

uniform vec4 lightDir;
uniform vec4 lightColor;

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_pos = mul(u_modelView, vec4(a_position, 1.0)).xyz;
    v_normal = normalize(mul(u_modelView, vec4(a_normal, 0.0) ).xyz);

    v_color = a_color0;
}
