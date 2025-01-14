$input a_position
$input a_color0

$output v_color

#include <bgfx_shader.sh>

void main()
{
    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
    v_color = a_color0;
}
