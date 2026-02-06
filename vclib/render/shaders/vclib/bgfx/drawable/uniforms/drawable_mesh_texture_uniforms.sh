#ifndef VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

// textures
SAMPLER2D(baseColorTex, VCL_MRB_TEXTURE0);
SAMPLER2D(metallicRoughnessTex, VCL_MRB_TEXTURE1);
SAMPLER2D(normalTex, VCL_MRB_TEXTURE2);
SAMPLER2D(occlusionTex, VCL_MRB_TEXTURE3);
SAMPLER2D(emissiveTex, VCL_MRB_TEXTURE4);
SAMPLER2D(s_brdf_lut, VCL_MRB_TEXTURE5);

#endif // VCL_EXT_BGFX_UNIFORMS_DRAWABLE_MESH_TEXTURE_UNIFORMS_SH
