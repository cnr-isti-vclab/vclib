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

#ifndef VCL_ALGORITHMS_MESH_CREATE_SPHERE_H
#define VCL_ALGORITHMS_MESH_CREATE_SPHERE_H

#include "icosahedron.h"

#include <vclib/algorithms/mesh/clean.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/core/sphere.h>

namespace vcl {

/**
 * @brief The CreateSphereArgs structs contains a series of parameters to
 * generate a sphere.
 *
 * The main parameter is `mode`, an enumeration that indicates the method used
 * to generate the sphere mesh. Its values can be: `UV` (default),
 * `NORMALIZED_CUBE`, `SPHERIFIED_CUBE` and `ICOSAHEDRON`. Please refer to
 * https://github.com/caosdoar/spheres for more details about the available
 * modes.
 *
 * Other arguments are:
 * - parallels (default 10): used when mode == `UV`;
 * - meridians (default 20): used when mode == `UV`;
 * - divisions (default 20): used when mode == `NORMALIZED_CUBE`,
 *   `SPHERIFIED_CUBE`, `ICOSAHEDRON`
 *
 * @ingroup create
 */
struct CreateSphereArgs
{
    typedef enum {
        UV = 0,
        NORMALIZED_CUBE,
        SPHERIFIED_CUBE,
        ICOSAHEDRON
    } CreateSphereMode;

    CreateSphereMode mode = UV;

    // used for mode = UV
    uint parallels = 10;
    uint meridians = 20;

    // used for mode = NORMALIZED_CUBE, SPHERIFIED_CUBE, ICOSAHEDRON
    uint divisions = 20;
};

namespace detail {

namespace cts { // Cube to Sphere

const Point3d origins[6] = {
    Point3d(-1.0, -1.0, -1.0),
    Point3d(1.0, -1.0, -1.0),
    Point3d(1.0, -1.0, 1.0),
    Point3d(-1.0, -1.0, 1.0),
    Point3d(-1.0, 1.0, -1.0),
    Point3d(-1.0, -1.0, 1.0)};

const Point3d rights[6] = {
    Point3d(2.0, 0.0, 0.0),
    Point3d(0.0, 0.0, 2.0),
    Point3d(-2.0, 0.0, 0.0),
    Point3d(0.0, 0.0, -2.0),
    Point3d(2.0, 0.0, 0.0),
    Point3d(2.0, 0.0, 0.0)};

const Point3d ups[6] = {
    Point3d(0.0, 2.0, 0.0),
    Point3d(0.0, 2.0, 0.0),
    Point3d(0.0, 2.0, 0.0),
    Point3d(0.0, 2.0, 0.0),
    Point3d(0.0, 0.0, 2.0),
    Point3d(0.0, 0.0, -2.0)};

} // namespace cts

} // namespace detail

/**
 * @brief Creates and returns a sphere mesh using the UV mode, starting from a
 * sphere object.
 *
 * @tparam MeshType: The type of the mesh to create, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] sp: A sphere object (its type must satisfy the SphereConcept).
 * @param[in] parallels: The number of parallels in the returned sphere mesh.
 * @param[in] meridians: The number of meridians in the returned sphere mesh.
 * @return A sphere mesh.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createSphereUV(
    const SphereConcept auto& sp,
    uint                      parallels = 10,
    uint                      meridians = 20)
{
    using VertexType = MeshType::VertexType;
    using PositionType  = VertexType::PositionType;
    using Facetype   = MeshType::FaceType;

    MeshType mesh;

    mesh.addVertex(PositionType(0, 1, 0));
    for (uint j = 0; j < parallels - 1; ++j) {
        const double polar = M_PI * double(j + 1) / double(parallels);
        const double sp    = std::sin(polar);
        const double cp    = std::cos(polar);
        for (uint i = 0; i < meridians; ++i) {
            const double azimuth = 2.0 * M_PI * double(i) / double(meridians);
            const double sa      = std::sin(azimuth);
            const double ca      = std::cos(azimuth);
            const double x       = sp * ca;
            const double y       = cp;
            const double z       = sp * sa;
            mesh.addVertex(PositionType(x, y, z));
        }
    }
    mesh.addVertex(PositionType(0, -1, 0));

    for (uint i = 0; i < meridians; ++i) {
        VertexType* v = &mesh.vertex(0);
        VertexType* a = &mesh.vertex(i + 1);
        VertexType* b = &mesh.vertex((i + 1) % meridians + 1);
        mesh.addFace(0, b, a);
    }

    for (uint32_t j = 0; j < parallels - 2; ++j) {
        uint32_t aStart = j * meridians + 1;
        uint32_t bStart = (j + 1) * meridians + 1;
        for (uint32_t i = 0; i < meridians; ++i) {
            VertexType* a  = &mesh.vertex(aStart + i);
            VertexType* a1 = &mesh.vertex(aStart + (i + 1) % meridians);
            VertexType* b  = &mesh.vertex(bStart + i);
            VertexType* b1 = &mesh.vertex(bStart + (i + 1) % meridians);
            if constexpr (HasTriangles<MeshType>) {
                mesh.addFace(a, a1, b1);
                mesh.addFace(b1, b, a);
            }
            else {
                mesh.addFace(a, a1, b1, b);
            }
        }
    }

    for (uint32_t i = 0; i < meridians; ++i) {
        VertexType* a = &mesh.vertex(i + meridians * (parallels - 2) + 1);
        VertexType* b =
            &mesh.vertex((i + 1) % meridians + meridians * (parallels - 2) + 1);
        VertexType* v = &mesh.vertex(mesh.vertexNumber() - 1);
        mesh.addFace(v, a, b);
    }

    scale(mesh, sp.radius());
    translate(mesh, sp.center());

    return mesh;
}

/**
 * @brief Creates and returns a sphere mesh using the normalized cube mode,
 * starting from a sphere object.
 *
 * @tparam MeshType: The type of the mesh to create, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] sp: A sphere object (its type must satisfy the SphereConcept).
 * @param[in] divisions: The number of divisions in the returned sphere mesh.
 * @return A sphere mesh.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createSphereNormalizedCube(
    const SphereConcept auto& sp,
    uint                      divisions)
{
    using VertexType = MeshType::VertexType;
    using PositionType  = VertexType::PositionType;
    using Facetype   = MeshType::FaceType;
    using ScalarType = PositionType::ScalarType;

    MeshType mesh;

    const double    step = 1.0 / double(divisions);
    const PositionType step3(step, step, step);

    for (uint face = 0; face < 6; ++face) {
        const PositionType origin = detail::cts::origins[face].cast<ScalarType>();
        const PositionType right  = detail::cts::rights[face].cast<ScalarType>();
        const PositionType up     = detail::cts::ups[face].cast<ScalarType>();
        for (uint j = 0; j < divisions + 1; ++j) {
            const PositionType j3(j, j, j);
            for (uint i = 0; i < divisions + 1; ++i) {
                const PositionType i3(i, i, i);
                const PositionType p =
                    origin + step3.mul(i3.mul(right) + j3.mul(up));

                mesh.addVertex(p.normalized());
            }
        }
    }

    const uint k = divisions + 1;
    for (uint face = 0; face < 6; ++face) {
        for (uint j = 0; j < divisions; ++j) {
            const bool bottom = j < (divisions / 2);
            for (uint i = 0; i < divisions; ++i) {
                const bool  left = i < (divisions / 2);
                VertexType* a    = &mesh.vertex((face * k + j) * k + i);
                VertexType* b    = &mesh.vertex((face * k + j) * k + i + 1);
                VertexType* c    = &mesh.vertex((face * k + j + 1) * k + i);
                VertexType* d    = &mesh.vertex((face * k + j + 1) * k + i + 1);

                if constexpr (HasTriangles<MeshType>) {
                    if (bottom ^ left) {
                        mesh.addFace(a, c, b);
                        mesh.addFace(c, d, b);
                    }
                    else {
                        mesh.addFace(a, c, d);
                        mesh.addFace(a, d, b);
                    }
                }
                else {
                    mesh.addFace(a, c, d, b);
                }
            }
        }
    }

    scale(mesh, sp.radius());
    translate(mesh, sp.center());

    return mesh;
}

/**
 * @brief Creates and returns a sphere mesh using the spherified cube mode,
 * starting from a sphere object.
 *
 * @tparam MeshType: The type of the mesh to create, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] sp: A sphere object (its type must satisfy the SphereConcept).
 * @param[in] divisions: The number of divisions in the returned sphere mesh.
 * @return A sphere mesh.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createSphereSpherifiedCube(
    const SphereConcept auto& sp,
    uint                      divisions)
{
    using VertexType = MeshType::VertexType;
    using PositionType  = VertexType::PositionType;
    using ScalarType = PositionType::ScalarType;
    using Facetype   = MeshType::FaceType;

    MeshType mesh;

    const double    step = 1.0 / double(divisions);
    const PositionType step3(step, step, step);

    for (uint face = 0; face < 6; ++face) {
        const PositionType origin = detail::cts::origins[face].cast<ScalarType>();
        const PositionType right  = detail::cts::rights[face].cast<ScalarType>();
        const PositionType up     = detail::cts::ups[face].cast<ScalarType>();
        for (uint j = 0; j < divisions + 1; ++j) {
            const PositionType j3(j, j, j);
            for (uint i = 0; i < divisions + 1; ++i) {
                const PositionType i3(i, i, i);
                const PositionType p =
                    origin + step3.mul(i3.mul(right) + j3.mul(up));
                const PositionType p2 = p.mul(p);
                const PositionType n(
                    p.x() * std::sqrt(
                                1.0 - 0.5 * (p2.y() + p2.z()) +
                                p2.y() * p2.z() / 3.0),
                    p.y() * std::sqrt(
                                1.0 - 0.5 * (p2.z() + p2.x()) +
                                p2.z() * p2.x() / 3.0),
                    p.z() * std::sqrt(
                                1.0 - 0.5 * (p2.x() + p2.y()) +
                                p2.x() * p2.y() / 3.0));
                mesh.addVertex(n);
            }
        }
    }

    const uint k = divisions + 1;
    for (uint face = 0; face < 6; ++face) {
        for (uint j = 0; j < divisions; ++j) {
            const bool bottom = j < (divisions / 2);
            for (uint i = 0; i < divisions; ++i) {
                const bool  left = i < (divisions / 2);
                VertexType* a    = &mesh.vertex((face * k + j) * k + i);
                VertexType* b    = &mesh.vertex((face * k + j) * k + i + 1);
                VertexType* c    = &mesh.vertex((face * k + j + 1) * k + i);
                VertexType* d    = &mesh.vertex((face * k + j + 1) * k + i + 1);
                if constexpr (HasTriangles<MeshType>) {
                    if (bottom ^ left) {
                        mesh.addFace(a, c, b);
                        mesh.addFace(c, d, b);
                    }
                    else {
                        mesh.addFace(a, c, d);
                        mesh.addFace(a, d, b);
                    }
                }
                else {
                    mesh.addFace(a, c, d, b);
                }
            }
        }
    }

    scale(mesh, sp.radius());
    translate(mesh, sp.center());

    return mesh;
}

/**
 * @brief Creates and returns a sphere mesh using the icosahedron mode, starting
 * from a sphere object.
 *
 * @tparam MeshType: The type of the mesh to create, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] sp: A sphere object (its type must satisfy the SphereConcept).
 * @param[in] divisions: The number of divisions in the returned sphere mesh.
 * @return A sphere mesh.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createSphereIcosahedron(const SphereConcept auto& sp, uint divisions)
{
    using VertexType = MeshType::VertexType;
    using PositionType  = VertexType::PositionType;
    using FaceType   = MeshType::FaceType;

    MeshType mesh = createIcosahedron<MeshType>(true);

    for (uint d = 0; d < divisions; d++) {
        uint nf = mesh.faceNumber();
        for (uint f = 0; f < nf; f++) {
            FaceType&   f0   = mesh.face(f);
            VertexType& v0   = *f0.vertex(0);
            VertexType& v1   = *f0.vertex(1);
            VertexType& v2   = *f0.vertex(2);
            uint        v1id = mesh.index(v1);
            uint        v2id = mesh.index(v2);

            PositionType pa = (v0.position() + v1.position());
            pa.normalize();
            PositionType pb = (v1.position() + v2.position());
            pb.normalize();
            PositionType pc = (v2.position() + v0.position());
            pc.normalize();
            uint vaid = mesh.addVertex(pa);
            uint vbid = mesh.addVertex(pb);
            uint vcid = mesh.addVertex(pc);

            f0.setVertex(1, vaid);
            f0.setVertex(2, vcid);
            mesh.addFace(vaid, v1id, vbid);
            mesh.addFace(vcid, vbid, v2id);
            mesh.addFace(vaid, vbid, vcid);
        }
    }

    removeDuplicatedVertices(mesh);

    scale(mesh, sp.radius());
    translate(mesh, sp.center());

    return mesh;
}

/**
 * @brief Creates a Sphere Mesh starting from a sphere object, using the
 * generation method given in the argument `args.mode` (see
 * https://github.com/caosdoar/spheres for more details).
 *
 * @tparam MeshType: the type of the mesh to be generated, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] sp: A sphere object (its type must satisfy the SphereConcept).
 * @param[in] args: Arguments for the sphere generation.
 *
 * @return A sphere mesh.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createSphere(
    const SphereConcept auto& sp,
    const CreateSphereArgs&   args = CreateSphereArgs())
{
    MeshType m;
    switch (args.mode) {
    case CreateSphereArgs::UV:
        m = createSphereUV<MeshType>(sp, args.parallels, args.meridians);
        break;
    case CreateSphereArgs::NORMALIZED_CUBE:
        m = createSphereNormalizedCube<MeshType>(sp, args.divisions);
        break;
    case CreateSphereArgs::SPHERIFIED_CUBE:
        m = createSphereSpherifiedCube<MeshType>(sp, args.divisions);
        break;
    case CreateSphereArgs::ICOSAHEDRON:
        m = createSphereIcosahedron<MeshType>(sp, args.divisions);
        break;
    }

    return m;
}

/**
 * @brief Creates a Sphere Mesh using the UV mode
 * (https://github.com/caosdoar/spheres), centered in (0, 0, 0), having radius
 * 1, with 10 parallels and 20 meridias.
 *
 * @tparam MeshType: the type of the mesh to be generated, it must satisfy the
 * FaceMeshConcept.
 *
 * @return A sphere mesh.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createSphere()
{
    return createSphere<MeshType, double>({Point3d(), 1});
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CREATE_SPHERE_H
