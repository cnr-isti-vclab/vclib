#ifndef POLYLINES
#define POLYLINES 

#include "utils.sh"

vec4 calculateLines(vec4 p0_px, vec4 p1_px, vec2 uv, float length_px, float thickness, float antialias, float border, 
                    float screen_width, float screen_heigth, float leftCap, float rigthCap) {
    vec4 T = normalize(p1_px - p0_px);
    vec4 N = vec4(-T.y, T.x, 0.0, 0.0);
    
    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    float width_px = (thickness / 2) + antialias + border;

    float activeCaps = sign((leftCap * (1 - uv.x)) + (rigthCap * uv.x));

    vec4 p = p0_px + (uv.x * T * length_px) + (v * N * width_px) + (u * T * ((activeCaps * (thickness/2)) + antialias + border)); 
    p = screenToClip(p, screen_width, screen_heigth);

    float z = ((1 - uv.x) * (p0_px.z)) + (uv.x * (p1_px.z));
	float w = ((1 - uv.x) * (p0_px.w)) + (uv.x * (p1_px.w));
    return vec4(p.xy * w, z, w); 
}

vec4 calculateLinesUV(vec4 p0_px, vec4 p1_px, vec2 uv, float length_px, float thickness, float antialias, float border, 
                      float leftCap, float rigthCap) 
{
    vec4 T = vec4(1.0, 0.0, 0.0, 0.0);
    vec4 N = vec4(0.0, 1.0, 0.0, 0.0);

    float width_px = (thickness / 2) + antialias + border;

    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    float activeCaps = sign((leftCap * (1 - uv.x)) + (rigthCap * uv.x));
    return (uv.x * length_px * T) + (u * T * ((activeCaps * (thickness/2)) + antialias + border)) + (v * N * width_px);
}

vec4 calculateLinesColor(vec4 uv, float length_px, float thickness, float antialias, float border, float leftCap, float rigthCap, vec4 color, vec4 borderColor) {
	float d = -1;
	float width_px = (thickness / 2) + antialias + border;
	vec4 final_border_color = (borderColor * sign(border)) + (color * (1 - sign(border)));

	if(uv.x < 0) {
		float square_cap    = (width_px - max(abs(uv.x), abs(uv.y))) * (1 - sign(abs(leftCap - 1)));
		float round_cap     = (width_px - length(uv))  			     * (1 - sign(abs(leftCap - 2)));
		float triangle_cap  = (width_px - (abs(uv.x) + abs(uv.y)))   * (1 - sign(abs(leftCap - 3)));

		d = square_cap + round_cap + triangle_cap;
	} else if(uv.x > length_px) {
		float square_cap    = (width_px - max(abs(uv.x - length_px), abs(uv.y))) * (1 - sign(abs(rigthCap - 1)));
		float round_cap     = (width_px - length(uv - vec4(length_px, 0, 0, 0))) * (1 - sign(abs(rigthCap - 2)));
		float triangle_cap  = (width_px - (abs(uv.x - length_px) + abs(uv.y)))   * (1 - sign(abs(rigthCap - 3)));

		d = square_cap + round_cap + triangle_cap;
	} else
		d = width_px - abs(uv.y);

	if(d > antialias + border)
		return color;

	else if(d > antialias) {
		return final_border_color;

	} else if (d > 0) {
		d /= 2;
		return vec4(final_border_color.xyz, d);

	} else {
		return vec4(0);
	}
}

#endif