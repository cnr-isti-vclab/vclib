$input a_position
$output v_color, v_uv, v_length

#include <bgfx_compute.sh>
#include "../../lines.sh"

BUFFER_RO(pointsBuffer, float, 1);

#define p(pos)      vec4(pointsBuffer[0 + ((pos) * 7)], pointsBuffer[1 + ((pos) * 7)], pointsBuffer[2 + ((pos) * 7)], 0.0)
#define color(pos)  vec4(pointsBuffer[3 + ((pos) * 7)], pointsBuffer[4 + ((pos) * 7)], pointsBuffer[5 + ((pos) * 7)], pointsBuffer[6 + ((pos) * 7)])

uniform vec4 u_data1;
uniform vec4 u_data2;

#define uv                    a_position

#define u_screenWidth         u_data1.x
#define u_screenHeigth        u_data1.y
#define u_thickness           u_data1.z

#define u_leftCap             u_data1.w
#define u_rigthCap            u_data2.x

#define u_antialias           u_data2.y
#define u_border              u_data2.z

void main() {
    vec4 p0 =    p((gl_InstanceID * 2));
    vec4 p1 =    p((gl_InstanceID * 2) + 1);
    vec4 color = color((gl_InstanceID * 2) + (1 * uv.x));

    vec4 p0_px = calculatePointWithMVP(p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = calculatePointWithMVP(p1, u_screenWidth, u_screenHeigth);

    v_color = color;
    v_length = length(p1_px - p0_px);
    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap);
}
