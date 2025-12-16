/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/bgfx/environment.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/io.h>
#include <vclib/io/image/hdr/load.h> // TODO: add hdr load to io/image/load.h

namespace vcl {

void Environment::drawBackground(const DrawObjectSettings& settings)
{
    if(!mBackgroundReady)
        prepareBackground();

    using enum VertFragProgram;
    ProgramManager& pm = Context::instance().programManager();

    mVertexBuffer.bindVertex(0);

    bgfx::setState(BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LEQUAL);

    bgfx::submit(
        settings.viewId, 
        pm.getProgram<DRAWABLE_BACKGROUND_PBR>()
    );
}

void Environment::prepareBackground()
{
    auto [vertices, releaseFn] =
        getAllocatedBufferAndReleaseFn<float>(mVertexNumber * 3);

    std::copy(mVertices, mVertices + mVertexNumber * 3, vertices);

    mVertexBuffer.create(
        vertices, 
        mVertexNumber,
        bgfx::Attrib::Enum::Position,
        3,           // attributes per vertex
        vcl::PrimitiveType::FLOAT,
        false,       // data is normalized
        releaseFn
    );

    mBackgroundReady = true;
}

} // namespace vcl
