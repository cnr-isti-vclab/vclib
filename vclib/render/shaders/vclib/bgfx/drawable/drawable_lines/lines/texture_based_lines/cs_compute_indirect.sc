#include <bgfx_compute.sh>

BUFFER_WO(indirectBuffer, uvec4, 0);
uniform vec4 u_IndirectData;

NUM_THREADS(1, 1, 1)
void main()
{
	drawIndexedIndirect(indirectBuffer, 0, 6, u_IndirectData.x, 0, 0, 0);
}
