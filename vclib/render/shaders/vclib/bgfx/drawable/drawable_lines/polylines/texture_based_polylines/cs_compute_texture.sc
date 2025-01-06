#include "bgfx_compute.sh"

BUFFER_RO(pointsBuffer,          float,  0);
IMAGE2D_WO(textureBuffer,        rgba32f,  1);

uniform vec4 u_IndirectData;
#define maxTextureSize u_IndirectData.x

#define p(pos)      vec4(pointsBuffer[0 + ((pos) * 7)], pointsBuffer[1 + ((pos) * 7)], pointsBuffer[2 + ((pos) * 7)], 0.0)
#define color(pos)  vec4(pointsBuffer[3 + ((pos) * 7)], pointsBuffer[4 + ((pos) * 7)], pointsBuffer[5 + ((pos) * 7)], pointsBuffer[6 + ((pos) * 7)])

NUM_THREADS(1, 1, 1)
void main() {
    uint p_Y = (gl_WorkGroupID.x * 2) / maxTextureSize;
    uint p_X = (gl_WorkGroupID.x * 2) - (p_Y * maxTextureSize);

    uint color_Y = ((gl_WorkGroupID.x * 2) + 1) / maxTextureSize;
    uint color_X = ((gl_WorkGroupID.x * 2) + 1) - (color_Y * maxTextureSize);

    vec4 p     = p(gl_WorkGroupID.x);
    vec4 color = color(gl_WorkGroupID.x);

    imageStore(textureBuffer, ivec2(p_X, p_Y),         p);
    imageStore(textureBuffer, ivec2(color_X, color_Y), color);
}