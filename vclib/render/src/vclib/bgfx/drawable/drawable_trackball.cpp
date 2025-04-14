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

#include <vclib/bgfx/drawable/drawable_trackball.h>

namespace vcl {

namespace detail {

template<typename ScalarType>
struct TrackballVertex {
    ScalarType x, y, z;
    uint32_t color;
};

/**
 * @brief Create a 3D trackball with 3 circles in the x, y, and z planes;
 * each vertex is packed as 3 coordinates and a 32-bit color
 * encoded into a float).
 *
 * @param pointsPerCircle: the number of points per circle,
 * unsigned integer.
 * @return a pair of vectors containing the vertices and edges of
 * the trackball.
 */
template<typename ScalarType, std::unsigned_integral IndexType>
std::pair<std::vector<TrackballVertex<ScalarType>>, std::vector<IndexType>>
createTrackballData(ScalarType radius, IndexType pointsPerCircle)
{
    using Vertex = TrackballVertex<ScalarType>;
    static_assert(
        sizeof(Vertex) == 3*sizeof(ScalarType) + sizeof(uint32_t),
        "vertex struct size mismatch (not packed)");
    assert(pointsPerCircle > 0 &&
        "number of points per circle must be greater than 0");
    assert(radius > 0 && "radius must be greater than 0");
    
    vcl::Polygon2<ScalarType> circle =
    vcl::createCircle<vcl::Polygon2<ScalarType>>(pointsPerCircle, 1.0);
    
    std::vector<IndexType> edges;
    std::vector<Vertex>    vertexData;
    vertexData.reserve(pointsPerCircle);

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
static const auto TRACKBALL_DATA =
    detail::createTrackballData<float, uint16_t>(1.0, N_POINTS);

DrawableTrackBall::DrawableTrackBall()
{
    createBuffers();
}

DrawableTrackBall::DrawableTrackBall(const DrawableTrackBall& other) :
            mVisible(other.mVisible), mProgram(other.mProgram),
            mUniforms(other.mUniforms), mTransform(other.mTransform)
{
    // copy all the members that can be copied, and then re-create the
    // buffers
    createBuffers();
}

void DrawableTrackBall::swap(DrawableTrackBall& other)
{
    using std::swap;
    swap(mVisible, other.mVisible);
    swap(mVertexCoordsColorBuffer, other.mVertexCoordsColorBuffer);
    swap(mEdgeIndexBuffer, other.mEdgeIndexBuffer);
    swap(mProgram, other.mProgram);
    swap(mUniforms, other.mUniforms);
    swap(mTransform, other.mTransform);
}

/**
 * @brief Update the dragging status of the trackball.
 * @param[in] isDragging: true if the trackball is being dragged, false
 * otherwise.
 */
void DrawableTrackBall::updateDragging(bool isDragging)
{
    mUniforms.setDragging(isDragging);
}

void DrawableTrackBall::setTransform(const vcl::Matrix44f& mtx)
{
    mTransform = mtx;
}

DrawableTrackBall& DrawableTrackBall::operator=(DrawableTrackBall other)
{
    swap(other);
    return *this;
}

void DrawableTrackBall::draw(uint viewId) const
{
    if (isVisible()) {
        if (bgfx::isValid(mProgram)) {
            bgfx::setState(
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z |
                BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_PT_LINES |
                BGFX_STATE_BLEND_ALPHA);

            mVertexCoordsColorBuffer.bind(0);
            mEdgeIndexBuffer.bind();

            bgfx::setTransform(mTransform.data());

            mUniforms.bind();

            bgfx::submit(viewId, mProgram);
        }
    }
}

Box3d DrawableTrackBall::boundingBox() const
{
    return Box3d();
}

std::shared_ptr<DrawableObject> DrawableTrackBall::clone() const&
{
    return std::make_shared<DrawableTrackBall>(*this);
}

std::shared_ptr<DrawableObject> DrawableTrackBall::clone() &&
{
    return std::make_shared<DrawableTrackBall>(std::move(*this));
}

bool DrawableTrackBall::isVisible() const { return mVisible; }

void DrawableTrackBall::setVisibility(bool vis) { mVisible = vis; }

void DrawableTrackBall::createBuffers()
{
    // vertex layout
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    // vertex buffer with color
    mVertexCoordsColorBuffer.create(
        bgfx::makeRef(
            TRACKBALL_DATA.first.data(),
            TRACKBALL_DATA.first.size() *
            sizeof(detail::TrackballVertex<float>)),
        layout,
        BGFX_BUFFER_NONE);

    // edge index buffer
    mEdgeIndexBuffer.create(
        TRACKBALL_DATA.second.data(), TRACKBALL_DATA.second.size(), false);
}

} // namespace vcl
