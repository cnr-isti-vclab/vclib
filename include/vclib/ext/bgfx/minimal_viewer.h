/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_MINIMAL_VIEWER_H
#define VCL_EXT_BGFX_MINIMAL_VIEWER_H

#include <vclib/gui/desktop_trackball.h>
#include <vclib/render/drawable_object_vector.h>

#include "drawable_axis.h"
#include "shader_programs/drawable_axis_shader_program.h"
#include "shader_programs/drawable_mesh_shader_program.h"
#include "uniforms/camera_uniforms.h"
#include "uniforms/directional_light_uniforms.h"
#include "uniforms/mesh_render_settings_uniforms.h"

namespace vcl::bgf {

class MinimalViewer : public vcl::DesktopTrackBall<float>
{
    // this Viewer does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> drawList;
    DrawableAxis                          axis;

    CameraUniforms             cameraUniforms;
    DirectionalLightUniforms   directionalLightUniforms;
    MeshRenderSettingsUniforms meshRenderSettingsUniforms;

    // the programs must be created after the uniforms - bgfx issue on OpenGL
    vcl::bgf::DrawableMeshShaderProgram meshProgram;
    vcl::bgf::DrawableAxisShaderProgram axisProgram;

protected:
    using DTB = vcl::DesktopTrackBall<float>;

public:
    MinimalViewer(uint width = 1024, uint height = 768);

    MinimalViewer(
        std::shared_ptr<DrawableObjectVector> v,
        uint                                  width  = 1024,
        uint                                  height = 768);

    std::shared_ptr<const DrawableObjectVector> drawableObjectVector() const;

    void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v);

    void fitScene();

    void draw(uint viewID);

    void toggleAxisVisibility() { axis.setVisibility(!axis.isVisible()); }

    void keyPress(Key key);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_MINIMAL_VIEWER_H
