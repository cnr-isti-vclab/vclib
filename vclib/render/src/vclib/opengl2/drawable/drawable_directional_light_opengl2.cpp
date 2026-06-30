// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/opengl2/drawable/drawable_directional_light_opengl2.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

namespace vcl {

DrawableDirectionalLightOpenGL2::DrawableDirectionalLightOpenGL2()
{
    mVertices.reserve(16 * 2 * 3);

    const float low  = -1.f;
    const float high = 1.f;

    const float zlow  = -2.f;
    const float zhigh = 2.f;
    const uint  n     = 4;
    const float step  = (high - low) / (n - 1);

    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < n; ++j) {
            mVertices.push_back(low + i * step);
            mVertices.push_back(low + j * step);
            mVertices.push_back(zlow);

            mVertices.push_back(low + i * step);
            mVertices.push_back(low + j * step);
            mVertices.push_back(zhigh);
        }
    }
}

void DrawableDirectionalLightOpenGL2::updateRotation(const vcl::Matrix44f& rot)
{
    mTransform = rot;
}

void DrawableDirectionalLightOpenGL2::setLinesColor(const vcl::Color& c)
{
    mColor = c;
}

void DrawableDirectionalLightOpenGL2::draw(const DrawObjectSettings& /*settings*/)
{
    if (!isVisible()) return;

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(mTransform.data());

    glColor4f(mColor.redF(), mColor.greenF(), mColor.blueF(), mColor.alphaF());

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mVertices.data());
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertices.size() / 3));
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
    
    glPopAttrib();
}

Box3d DrawableDirectionalLightOpenGL2::boundingBox() const
{
    return Box3d();
}

std::shared_ptr<DrawableObject> DrawableDirectionalLightOpenGL2::clone() const&
{
    return std::make_shared<DrawableDirectionalLightOpenGL2>(*this);
}

std::shared_ptr<DrawableObject> DrawableDirectionalLightOpenGL2::clone() &&
{
    return std::make_shared<DrawableDirectionalLightOpenGL2>(std::move(*this));
}

bool DrawableDirectionalLightOpenGL2::isVisible() const
{
    return mVisible;
}

void DrawableDirectionalLightOpenGL2::setVisibility(bool vis)
{
    mVisible = vis;
}

} // namespace vcl
