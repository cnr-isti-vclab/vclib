$input a_position

#include <bgfx_shader.sh>

void main()
{
	gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
}
