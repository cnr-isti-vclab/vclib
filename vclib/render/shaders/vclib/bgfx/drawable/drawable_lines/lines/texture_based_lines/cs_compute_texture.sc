#include "bgfx_compute.sh"

BUFFER_RO(segmentsBuffer,        float,  0);
IMAGE2D_WO(textureBuffer,        rgba32f,  1);

uniform vec4 u_IndirectData;
#define maxTextureSize u_IndirectData.x

#define p(pos)      vec3(segmentsBuffer[0 + ((pos) * 7)], segmentsBuffer[1 + ((pos) * 7)], segmentsBuffer[2 + ((pos) * 7)])
#define color(pos)  vec4(segmentsBuffer[3 + ((pos) * 7)], segmentsBuffer[4 + ((pos) * 7)], segmentsBuffer[5 + ((pos) * 7)], segmentsBuffer[6 + ((pos) * 7)])

NUM_THREADS(1, 1, 1)
void main() {
    vec3 p0     = p((gl_WorkGroupID.x * 2));
    vec3 p1     = p((gl_WorkGroupID.x * 2) + 1);
    vec4 color0 = color((gl_WorkGroupID.x * 2));
    vec4 color1 = color((gl_WorkGroupID.x * 2) + 1);

    uint p0_Y = (gl_WorkGroupID.x * 4) / maxTextureSize;
    uint p0_X = (gl_WorkGroupID.x * 4) - (p0_Y * maxTextureSize);
    
    uint p1_Y = ((gl_WorkGroupID.x * 4) + 1) / maxTextureSize;
    uint p1_X = ((gl_WorkGroupID.x * 4) + 1) - (p1_Y * maxTextureSize);

    uint color0_Y = ((gl_WorkGroupID.x * 4) + 2) / maxTextureSize;
    uint color0_X = ((gl_WorkGroupID.x * 4) + 2) - (color0_Y * maxTextureSize);

    uint color1_Y = ((gl_WorkGroupID.x * 4) + 3) / maxTextureSize;
    uint color1_X = ((gl_WorkGroupID.x * 4) + 3) - (color1_Y * maxTextureSize);

    imageStore(textureBuffer, ivec2(p0_X, p0_Y),         vec4(p0, 0.0));
    imageStore(textureBuffer, ivec2(p1_X, p1_Y),         vec4(p1, 0.0));
    imageStore(textureBuffer, ivec2(color0_X, color0_Y), color0);
    imageStore(textureBuffer, ivec2(color1_X, color1_Y), color1);
}