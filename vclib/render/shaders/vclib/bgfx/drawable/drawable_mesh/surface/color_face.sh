#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(primitiveColors, uint, VCL_MRB_PRIMITIVE_COLOR_BUFFER);

#define COLORING vec4 color = uintABGRToVec4Color(primitiveColors[gl_PrimitiveID]);