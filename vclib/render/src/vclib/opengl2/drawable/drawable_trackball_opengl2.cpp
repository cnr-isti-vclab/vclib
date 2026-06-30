// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/opengl2/drawable/drawable_trackball_opengl2.h>

#include <vclib/algorithms/core/create.h>
#include <vclib/space/core/color.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

#include <vector>
#include <utility>
#include <concepts>

namespace vcl {

namespace detail {

template<typename ScalarType>
struct TrackballVertex
{
    ScalarType x, y, z;
    uint32_t   color;
};

/**
 * @brief Create a 3D trackball with 3 circles in the x, y, and z planes.
 */
template<typename ScalarType, std::unsigned_integral IndexType>
std::pair<std::vector<TrackballVertex<ScalarType>>, std::vector<IndexType>>
createTrackballData(ScalarType radius, IndexType pointsPerCircle)
{
    using Vertex = TrackballVertex<ScalarType>;

    vcl::Polygon2<ScalarType> circle =
        vcl::createCircle<vcl::Polygon2<ScalarType>>(pointsPerCircle, radius);

    std::vector<IndexType> edges;
    std::vector<Vertex>    vertexData;
    vertexData.reserve(pointsPerCircle * 3);

    static const uint32_t COLOR_X = Color(Color::Red).abgr();
    static const uint32_t COLOR_Y = Color(Color::Green).abgr();
    static const uint32_t COLOR_Z = Color(Color::Blue).abgr();

    // x
    uint first = 0;
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertexData.push_back({0, p.x(), p.y(), COLOR_X});
        edges.push_back(first + i);
        edges.push_back(first + (i + 1) % circle.size());
    }

    // y
    first = circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertexData.push_back({p.x(), 0, p.y(), COLOR_Y});
        edges.push_back(i + first);
        edges.push_back((i + 1) % circle.size() + first);
    }

    // z
    first = 2 * circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertexData.push_back({p.x(), p.y(), 0, COLOR_Z});
        edges.push_back(i + first);
        edges.push_back((i + 1) % circle.size() + first);
    }

    return std::make_pair(std::move(vertexData), std::move(edges));
}

} // namespace detail

static const uint16_t N_POINTS = 128;
static const auto     TRACKBALL_DATA =
    detail::createTrackballData<float, uint16_t>(1.0, N_POINTS);

void DrawableTrackBallOpenGL2::updateDragging(bool isDragging)
{
    mIsDragging = isDragging;
}

void DrawableTrackBallOpenGL2::setTransform(const vcl::Matrix44f& mtx)
{
    mTransform = mtx;
}

void DrawableTrackBallOpenGL2::draw(const DrawObjectSettings&)
{
    if (!isVisible()) {
        return;
    }

    glPushAttrib(
        GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT |
        GL_LIGHTING_BIT);

    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(mTransform.data());

    float alpha = mIsDragging ? 0.9f : 0.5f;

    glBegin(GL_LINES);
    for (auto idx : TRACKBALL_DATA.second) {
        const auto& v = TRACKBALL_DATA.first[idx];
        Color       c(v.color, Color::Format::ABGR);
        glColor4f(c.redF(), c.greenF(), c.blueF(), alpha);
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();

    glPopMatrix();
    glPopAttrib();
}

Box3d DrawableTrackBallOpenGL2::boundingBox() const
{
    return Box3d();
}

std::shared_ptr<DrawableObject> DrawableTrackBallOpenGL2::clone() const&
{
    return std::make_shared<DrawableTrackBallOpenGL2>(*this);
}

std::shared_ptr<DrawableObject> DrawableTrackBallOpenGL2::clone() &&
{
    return std::make_shared<DrawableTrackBallOpenGL2>(std::move(*this));
}

bool DrawableTrackBallOpenGL2::isVisible() const
{
    return mVisible;
}

void DrawableTrackBallOpenGL2::setVisibility(bool vis)
{
    mVisible = vis;
}

} // namespace vcl
