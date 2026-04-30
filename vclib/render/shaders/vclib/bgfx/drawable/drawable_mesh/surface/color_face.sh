#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next lines with:

BUFFER_RO(primitiveColors, uint, VCL_MRB_PRIMITIVE_COLOR_BUFFER);
*/
BUFFER_RO(primitiveColors, uint, 13);

#define primitiveID (u_firstChunkPrimitiveID + gl_PrimitiveID)

#define COLORING vec4 color = uintABGRToVec4Color(primitiveColors[primitiveID]);
