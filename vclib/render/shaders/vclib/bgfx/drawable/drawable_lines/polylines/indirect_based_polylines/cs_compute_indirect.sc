#include <bgfx_compute.sh>

BUFFER_WO(segmentsIndirectBuffer, uvec4, 0);
BUFFER_WO(joinsIndirectBuffer,    uvec4, 1);

uniform vec4 u_IndirectData;

NUM_THREADS(1, 1, 1)
void main()
{
	drawIndexedIndirect(segmentsIndirectBuffer, 0, 6, u_IndirectData.x - 1, 0, 0, 0);
	drawIndexedIndirect(joinsIndirectBuffer,    0, 6, u_IndirectData.x - 2, 0, 0, 0);
}
