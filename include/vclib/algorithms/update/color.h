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

#ifndef VCL_ALGORITHMS_UPDATE_COLOR_H
#define VCL_ALGORITHMS_UPDATE_COLOR_H

#include <set>

#include <vclib/algorithms/mesh/stat.h>
#include <vclib/math/perlin_noise.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/color.h>
#include <vclib/views/mesh.h>

namespace vcl {

namespace detail {

struct ColorAvgInfo
{
    vcl::Point4<uint> c   = vcl::Point4<uint>(0, 0, 0, 0);
    uint              cnt = 0;
};

} // namespace detail

/**
 * @brief Sets the color of the vertices. If the `onlySelected` flag is set to
 * `true`, only the color of the selected vertices will be set. Otherwise, all
 * the vertices will have the same color.
 *
 * @throws vcl::MissingComponentException if the mesh does not have the
 * per vertex color component available.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the MeshConcept.
 *
 * @param[in,out] m: the mesh on which set the vertex color.
 * @param[in] c: the color to set to the vertices of the mesh.
 * @param[in] onlySelected: if `true`, the color will be set just on the
 * selected vertices.
 *
 * @ingroup update
 */
template<MeshConcept MeshType>
void setPerVertexColor(
    MeshType&  m,
    vcl::Color c            = vcl::Color::White,
    bool       onlySelected = false)
{
    vcl::requirePerVertexColor(m);

    if (onlySelected) {
        std::ranges::fill(m.vertices() | views::selected | views::colors, c);
    }
    else {
        std::ranges::fill(m.vertices() | views::colors, c);
    }
}

/**
 * @brief Sets the color of the faces. If the `onlySelected` flag is set to
 * `true`, only the color of the selected faces will be set. Otherwise, all the
 * faces will have the same color.
 *
 * @throws vcl::MissingComponentException if the mesh does not have the
 * per face color component available.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the
 * FaceMeshConcept.
 *
 * @param[in,out] m: the mesh on which set the face color.
 * @param[in] c: the color to set to the faces of the mesh.
 * @param[in] onlySelected: if `true`, the color will be set just on the
 * selected faces.
 *
 * @ingroup update
 */
template<FaceMeshConcept MeshType>
void setPerFaceColor(
    MeshType&  m,
    vcl::Color c            = vcl::Color::White,
    bool       onlySelected = false)
{
    vcl::requirePerFaceColor(m);

    if (onlySelected) {
        std::ranges::fill(m.faces() | views::selected | views::colors, c);
    }
    else {
        std::ranges::fill(m.faces() | views::colors, c);
    }
}

/**
 * @brief Sets the color component of a mesh.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the HasColor
 * concept.
 *
 * @param[in,out] m: the mesh on which set the color.
 * @param[in] c: the color to set to the mesh.
 *
 * @ingroup update
 */
template<HasColor MeshType>
void setMeshColor(MeshType& m, vcl::Color c = vcl::Color::White)
{
    m.color() = c;
}

/**
 * @brief Sets the vertex colors from its incident face colors, computing a
 * plain average of the face colors.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *   - Faces:
 *     - Color
 *
 * @param[in,out] m: mesh on which transfer the face color into the vertex
 * color.
 *
 * @ingroup update
 */
template<FaceMeshConcept MeshType>
void setPerVertexColorFromFaceColor(MeshType& m)
{
    vcl::requirePerVertexColor(m);
    vcl::requirePerFaceColor(m);

    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    std::vector<detail::ColorAvgInfo> csi(m.vertexContainerSize());

    for (const FaceType& f : m.faces()) {
        for (const VertexType* v : f.vertices()) {
            csi[m.index(v)].c += v->color();
            csi[m.index(v)].cnt++;
        }
    }

    for (VertexType& v : m.vertices()) {
        v.color() = csi[m.index(v)].c / csi[m.index(v)].cnt;
    }
}

/**
 * @brief Sets the face colors from its incident vertex colors, computing a
 * plain average of the vertex colors.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *   - Faces:
 *     - Color
 *
 * @param[in,out] m: mesh on which transfer the vertex color into the face
 * color.
 *
 * @ingroup update
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorFromVertexColor(MeshType& m)
{
    vcl::requirePerVertexColor(m);
    vcl::requirePerFaceColor(m);

    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    for (FaceType& f : m.faces()) {
        vcl::Point4<uint> avg(0, 0, 0, 0);
        for (const VertexType* v : f.vertices()) {
            avg += v->color().template cast<uint>();
        }
        avg /= f.vertexNumber();
        f.color() = avg.cast<uint8_t>();
    }
}

/**
 * @brief Sets the vertex colors from the quality values by computing a shading
 * in the given color map (default is Red to Blue color map), in the given
 * interval [minQuality, maxQuality].
 *
 * If minQuality and maxQuality are not set (or if they are equal), the range is
 * automatically computed.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *     - Quality
 *
 * @param[in,out] m: mesh on which compute the vertex color.
 * @param[in] colorMap: the colormap to use to color the vertices of the mesh
 * (default: RedBlue).
 * @param[in] minQuality: the minimum value of the range to use for coloring
 * (default: 0).
 * @param[in] maxQuality: the maximum value of the range to use for coloring
 * (default: 0).
 *
 * @ingroup update
 */
template<MeshConcept MeshType>
void setPerVertexColorFromQuality(
    MeshType&                                  m,
    vcl::Color::ColorMap                       colorMap   = vcl::Color::RedBlue,
    typename MeshType::VertexType::QualityType minQuality = 0,
    typename MeshType::VertexType::QualityType maxQuality = 0)
{
    vcl::requirePerVertexColor(m);
    vcl::requirePerVertexQuality(m);

    using VertexType  = MeshType::VertexType;
    using QualityType = VertexType::QualityType;

    if (minQuality == maxQuality) {
        std::pair<QualityType, QualityType> pair = vertexQualityMinMax(m);
        minQuality                               = pair.first;
        maxQuality                               = pair.second;
    }
    for (VertexType& v : m.vertices()) {
        v.color() =
            colorFromInterval(minQuality, maxQuality, v.quality(), colorMap);
    }
}

/**
 * @brief Sets the face colors from the quality values by computing a shading in
 * the given color map (default is Red to Blue color map), in the given interval
 * [minQuality, maxQuality].
 *
 * If minQuality and maxQuality are not set (or if they are equal), the range is
 * automatically computed.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Color
 *     - Quality
 *
 * @param[in,out] m: mesh on which compute the face color.
 * @param[in] colorMap: the colormap to use to color the faces of the mesh
 * (default: RedBlue).
 * @param[in] minQuality: the minimum value of the range to use for coloring
 * (default: 0).
 * @param[in] maxQuality: the maximum value of the range to use for coloring
 * (default: 0).
 *
 * @ingroup update
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorFromQuality(
    MeshType&                                m,
    vcl::Color::ColorMap                     colorMap   = vcl::Color::RedBlue,
    typename MeshType::FaceType::QualityType minQuality = 0,
    typename MeshType::FaceType::QualityType maxQuality = 0)
{
    vcl::requirePerFaceColor(m);
    vcl::requirePerFaceQuality(m);

    using FaceType    = MeshType::FaceType;
    using QualityType = FaceType::QualityType;

    if (minQuality == maxQuality) {
        std::pair<QualityType, QualityType> pair = faceQualityMinMax(m);

        minQuality = pair.first;
        maxQuality = pair.second;
    }
    for (FaceType& f : m.faces()) {
        f.color() =
            colorFromInterval(minQuality, maxQuality, f.quality(), colorMap);
    }
}

/**
 * @brief Color the vertices of the mesh that are on border, using the border
 * flags of the faces.
 *
 * Before using this function, you should update the face border flags
 * accordingly.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *   - Faces:
 *     - TriangleFlags/PolygonFlags
 *
 * @param[in,out] m: the mesh on which update the vertex color from the border
 * face flags.
 * @param[in] borderColor: the color of the vertices that are part of edges that
 * are all marked as on border.
 * @param[in] internalColor: the color of the vertices that are part of edges
 * that are all marked as non on border.
 * @param[in] mixColor: the color of vertices that are part of edges that are
 * both on border and non on border.
 *
 * @ingroup update
 */
template<FaceMeshConcept MeshType>
void setPerVertexColorFromFaceBorderFlag(
    MeshType& m,
    Color     borderColor   = vcl::Color::Blue,
    Color     internalColor = vcl::Color::White,
    Color     mixColor      = vcl::Color::Cyan)
{
    vcl::requirePerVertexColor(m);

    using FaceType = MeshType::FaceType;

    const vcl::Color baseColor = Color::Green;

    setPerVertexColor(m, baseColor);

    for (FaceType& f : m.faces()) {
        for (uint i = 0; i < f.vertexNumber(); ++i) {
            if (f.edgeOnBorder(i)) {
                if (f.vertex(i).color() == baseColor)
                    f.vertex(i).color() = borderColor;
                if (f.vertex(i).color() == internalColor)
                    f.vertex(i).color() = mixColor;
                if (f.vertexMod(i + 1).color() == baseColor)
                    f.vertexMod(i + 1).color() = borderColor;
                if (f.vertexMod(i + 1).color() == internalColor)
                    f.vertexMod(i + 1).color() = mixColor;
            }
            else {
                if (f.vertex(i).color() == baseColor)
                    f.vertex(i).color() = internalColor;
                if (f.vertex(i).color() == borderColor)
                    f.vertex(i).color() = mixColor;
                if (f.vertexMod(i + 1).color() == baseColor)
                    f.vertexMod(i + 1).color() = internalColor;
                if (f.vertexMod(i + 1).color() == borderColor)
                    f.vertexMod(i + 1).color() = mixColor;
            }
        }
    }
}

/**
 * @brief Given an already computed vector of sets of connected components (see
 * vcl::connectedComponents(m) in `vclib/algorithms/clean.h`), sets face colors
 * according from connected components of the mesh. Each connected component
 * will have a different per face color.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Color
 *
 * @param[in,out] m: the mesh on which set the face colors according to its
 * connected components.
 * @param[in] connectedComponents: a vector of sets, each one of them containing
 * the face ids of a connected component.
 *
 * @ingroup update
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorFromConnectedComponents(
    MeshType&                          m,
    const std::vector<std::set<uint>>& connectedComponents)
{
    std::vector<Color> vc = colorScattering(connectedComponents.size());

    uint cid = 0;
    for (const std::set<uint>& connComp : connectedComponents) {
        for (const uint& fid : connComp) {
            m.face(fid).color() = vc[cid];
        }
        cid++;
    }
}

/**
 * @brief Sets face colors according from connected components of the mesh. Each
 * connected component will have a different per face color. Since this function
 * will need to compute connected components of the mesh, also per Face
 * AdjacentFaces component is required.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - AdjacentFaces
 *     - Color
 *
 * @param[in,out] m: the mesh on which set the face colors according to its
 * connected components.
 *
 * @ingroup update
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorFromConnectedComponents(MeshType& m)
{
    vcl::requirePerFaceColor(m);

    std::vector<std::set<uint>> connComps = connectedComponents(m);

    setPerFaceColorFromConnectedComponents(m, connComps);
}

/**
 * @brief This function colors each face of the mesh using a given number of
 * scattering colors (default number is 50). By default, colors uniformely
 * adjacent faces having faux edges in common. This check is made only if the
 * option `checkFauxEdges` is set to true, and if the per Face AdjacentFaces
 * component is available in the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Color
 *
 * Optional:
 * - Mesh:
 *   - Faces:
 *     - AdjacentFaces
 *
 * @param[in,out] m: mesh on which set the face colors.
 * @param[in] nColors: number of colors used for the scattering (default 50).
 * @param[in] checkFauxEdges: if true, colors uniformely adjacent faces with
 * faux edges (default `true`).
 *
 * @ingroup update
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorScattering(
    MeshType& m,
    uint      nColors        = 50,
    bool      checkFauxEdges = true)
{
    vcl::requirePerFaceColor(m);

    using FaceType = MeshType::FaceType;

    Color baseColor = Color::Black;
    setPerFaceColor(m, baseColor);

    std::vector<Color> vc = colorScattering(nColors);

    for (FaceType& f : m.faces()) {
        if (f.color() == baseColor) {
            f.color() = vc[m.index(f) % nColors];
        }
        if constexpr (HasPerFaceAdjacentFaces<MeshType>) {
            if (checkFauxEdges && isPerFaceAdjacentFacesAvailable(m)) {
                for (uint i = 0; i < f.vertexNumber(); ++i) {
                    if (f.edgeFaux(i)) {
                        assert(f.adjFace(i) != nullptr);
                        f.adjFace(i)->color = f.color();
                    }
                }
            }
        }
    }
}

/**
 * @brief Set the vertex color according to a perlin noise computed on the
 * vertex coordinates.
 *
 * To make things weirder each color band can have its own offset and frequency.
 * Period is expressed in absolute terms.
 * So as period it is meaningful could be to use something in the range of 1/10
 * of the bbox diag.
 *
 * @throws vcl::MissingComponentException if the mesh does not have a per vertex
 * color component.
 *
 * @tparam MeshType: the type of the mesh. It must satisfy the MeshConcept.
 *
 * @param m
 * @param period
 * @param offset
 * @param onSelected
 *
 * @ingroup update
 */
template<MeshConcept MeshType, PointConcept PointType>
void setPerVertexColorPerlinNoise(
    MeshType& m,
    PointType period,
    PointType offset     = PointType(0, 0, 0),
    bool      onSelected = false)
{
    vcl::requirePerVertexColor(m);

    using VertexType = MeshType::VertexType;

    PointType p[3];

    for (VertexType& v : m.vertices()) {
        if (!onSelected || v.selected()) {
            p[0]      = (v.coord() / period[0]) + offset;
            p[1]      = (v.coord() / period[1]) + offset;
            p[2]      = (v.coord() / period[2]) + offset;
            v.color() = Color(
                127 + 128.0 * perlinNoise(p[0][0], p[0][1], p[0][2]),
                127 + 128.0 * perlinNoise(p[1][0], p[1][1], p[1][2]),
                127 + 128.0 * perlinNoise(p[2][0], p[2][1], p[2][2]),
                255);
        }
    }
}

/**
 * @brief Simple Perlin color mixing. `color1` and `color2` are mixed according
 * the perlin noise function, with `period` and `offset`.
 *
 * @throws vcl::MissingComponentException if the mesh does not have a per vertex
 * color component.
 *
 * @tparam MeshType: the type of the mesh. It must satisfy the MeshConcept.
 *
 * @param m
 * @param period
 * @param offset
 * @param color1
 * @param color2
 * @param onSelected
 *
 * @ingroup update
 */
template<MeshConcept MeshType, PointConcept PointType>
void setPerVertexPerlinColor(
    MeshType& m,
    double    period,
    PointType offset     = PointType(0, 0, 0),
    Color     color1     = Color::Black,
    Color     color2     = Color::White,
    bool      onSelected = false)
{
    vcl::requirePerVertexColor(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        if (!onSelected || v.selected()) {
            PointType p = v.coord() / period + offset;

            double factor = (perlinNoise(p[0], p[1], p[2]) + 1.0) / 2.0;

            int rr = (color1[0] * factor) + (color2[0] * (1.0 - factor));
            int gg = (color1[1] * factor) + (color2[1] * (1.0 - factor));
            int bb = (color1[2] * factor) + (color2[2] * (1.0 - factor));
            int aa = (color1[3] * factor) + (color2[3] * (1.0 - factor));

            v.color() = Color(rr, gg, bb, aa);
        }
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_UPDATE_COLOR_H
