$input v_position, v_normal, v_color, v_texcoord0, v_texcoord1

#include "none.sh"
#include "color_mesh.sh"

void main()
{
    SHADING
    COLORING
    gl_FragColor = light * color + vec4(specular, 0);
    gl_FragDepth = gl_FragCoord.z - depthOffset;
}