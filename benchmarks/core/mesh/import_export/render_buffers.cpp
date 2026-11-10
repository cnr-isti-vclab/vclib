// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>

#include <list>
#include <utility>
#include <vector>

BENCH(
    "import_export/render_buffers/build_render_buffers",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& mesh = s.mesh();

        if (mesh.vertexCount() == 0) {
            return s.skip("the mesh has no vertices");
        }

        // Vertex attributes
        bool hasVertexNormals = false;
        if constexpr (vcl::HasPerVertexNormal<Mesh>) {
            hasVertexNormals = vcl::isPerVertexNormalAvailable(mesh);
        }

        bool hasVertexColors = false;
        if constexpr (vcl::HasPerVertexColor<Mesh>) {
            hasVertexColors = vcl::isPerVertexColorAvailable(mesh);
        }

        bool hasVertexTexCoords = false;
        if constexpr (vcl::HasPerVertexTexCoord<Mesh>) {
            hasVertexTexCoords = vcl::isPerVertexTexCoordAvailable(mesh);
        }

        bool hasWedgeTexCoords = false;
        if constexpr (vcl::HasPerFaceWedgeTexCoords<Mesh>) {
            hasWedgeTexCoords = mesh.isPerFaceWedgeTexCoordsEnabled();
        }

        // Face attributes
        bool hasFaces       = false;
        bool hasFaceNormals = false;
        bool hasFaceColors  = false;
        bool hasWireframe   = false;
        if constexpr (vcl::FaceMeshConcept<Mesh>) {
            hasFaces = mesh.faceCount() > 0;
            if (hasFaces) {
                hasWireframe = true;
                if constexpr (vcl::HasPerFaceNormal<Mesh>) {
                    hasFaceNormals = vcl::isPerFaceNormalAvailable(mesh);
                }
                if constexpr (vcl::HasPerFaceColor<Mesh>) {
                    hasFaceColors = vcl::isPerFaceColorAvailable(mesh);
                }
            }
        }

        // Edge attributes
        bool hasEdges       = false;
        bool hasEdgeNormals = false;
        bool hasEdgeColors  = false;
        if constexpr (vcl::EdgeMeshConcept<Mesh>) {
            hasEdges = mesh.edgeCount() > 0;
            if (hasEdges) {
                if constexpr (vcl::HasPerEdgeNormal<Mesh>) {
                    hasEdgeNormals = vcl::isPerEdgeNormalAvailable(mesh);
                }
                if constexpr (vcl::HasPerEdgeColor<Mesh>) {
                    hasEdgeColors = vcl::isPerEdgeColorAvailable(mesh);
                }
            }
        }

        // Precompute sizes
        vcl::uint numVerts = mesh.vertexCount();
        if constexpr (vcl::HasPerFaceWedgeTexCoords<Mesh>) {
            if (hasWedgeTexCoords) {
                std::vector<std::pair<vcl::uint, vcl::uint>> initVertWedgeMap;
                std::list<vcl::uint> initVertsToDuplicate;
                std::list<std::list<std::pair<vcl::uint, vcl::uint>>>
                     initFacesToReassign;
                auto nV = vcl::verticesToDuplicateByWedgeTexCoordsCount(
                    mesh,
                    initVertWedgeMap,
                    initVertsToDuplicate,
                    initFacesToReassign);
                numVerts += nV;
            }
        }

        vcl::uint numTris           = 0;
        vcl::uint numWireframeLines = 0;
        if constexpr (vcl::FaceMeshConcept<Mesh>) {
            if (hasFaces) {
                numTris           = vcl::triangulatedFaceCount(mesh);
                numWireframeLines = vcl::faceVertexReferencesCount(mesh);
            }
        }

        vcl::uint numEdges = 0;
        if constexpr (vcl::EdgeMeshConcept<Mesh>) {
            if (hasEdges) {
                numEdges = mesh.edgeCount();
            }
        }

        // Preallocate output buffers
        std::vector<float>     posBuffer(numVerts * 3);
        std::vector<float>     normBuffer(hasVertexNormals ? numVerts * 3 : 0);
        std::vector<vcl::uint> colBuffer(hasVertexColors ? numVerts : 0);
        std::vector<float>     texCoordBuffer(
            hasVertexTexCoords ? numVerts * 2 : 0);
        std::vector<float> wedgeTexCoordBuffer(
            hasWedgeTexCoords ? numVerts * 2 : 0);

        std::vector<vcl::uint> triIndexBuffer(hasFaces ? numTris * 3 : 0);
        std::vector<float> triNormalBuffer(hasFaceNormals ? numTris * 3 : 0);
        std::vector<vcl::uint> triColorBuffer(hasFaceColors ? numTris : 0);

        std::vector<vcl::uint> wireframeIndexBuffer(
            hasWireframe ? numWireframeLines * 2 : 0);

        std::vector<vcl::uint> edgeIndexBuffer(hasEdges ? numEdges * 2 : 0);
        std::vector<float> edgeNormalBuffer(hasEdgeNormals ? numEdges * 3 : 0);
        std::vector<vcl::uint> edgeColorBuffer(hasEdgeColors ? numEdges : 0);

        // Auxiliary structures reused per iteration
        std::vector<std::pair<vcl::uint, vcl::uint>>          vertWedgeMap;
        std::list<vcl::uint>                                  vertsToDuplicate;
        std::list<std::list<std::pair<vcl::uint, vcl::uint>>> facesToReassign;
        vcl::TriPolyIndexBiMap                                indexMap;

        s.run([&]() {
            vertWedgeMap.clear();
            vertsToDuplicate.clear();
            facesToReassign.clear();
            indexMap = vcl::TriPolyIndexBiMap();

            if constexpr (vcl::HasPerFaceWedgeTexCoords<Mesh>) {
                if (hasWedgeTexCoords) {
                    vcl::verticesToDuplicateByWedgeTexCoordsCount(
                        mesh, vertWedgeMap, vertsToDuplicate, facesToReassign);
                }
            }

            vcl::vertexPositionsToBuffer(mesh, posBuffer.data());
            if (!vertsToDuplicate.empty()) {
                vcl::appendDuplicateVertexPositionsToBuffer(
                    mesh, vertsToDuplicate, posBuffer.data());
            }

            if constexpr (vcl::HasPerVertexNormal<Mesh>) {
                if (hasVertexNormals) {
                    vcl::vertexNormalsToBuffer(mesh, normBuffer.data());
                    if (!vertsToDuplicate.empty()) {
                        vcl::appendDuplicateVertexNormalsToBuffer(
                            mesh, vertsToDuplicate, normBuffer.data());
                    }
                }
            }

            if constexpr (vcl::HasPerVertexColor<Mesh>) {
                if (hasVertexColors) {
                    vcl::vertexColorsToBuffer(
                        mesh, colBuffer.data(), vcl::Color::Format::ABGR);
                    if (!vertsToDuplicate.empty()) {
                        vcl::appendDuplicateVertexColorsToBuffer(
                            mesh,
                            vertsToDuplicate,
                            colBuffer.data(),
                            vcl::Color::Format::ABGR);
                    }
                }
            }

            if constexpr (vcl::HasPerVertexTexCoord<Mesh>) {
                if (hasVertexTexCoords) {
                    vcl::vertexTexCoordsToBuffer(mesh, texCoordBuffer.data());
                    if (!vertsToDuplicate.empty()) {
                        vcl::appendDuplicateVertexTexCoordsToBuffer(
                            mesh, vertsToDuplicate, texCoordBuffer.data());
                    }
                }
            }

            if constexpr (vcl::HasPerFaceWedgeTexCoords<Mesh>) {
                if (hasWedgeTexCoords) {
                    vcl::wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
                        mesh,
                        vertWedgeMap,
                        facesToReassign,
                        wedgeTexCoordBuffer.data());
                }
            }

            if constexpr (vcl::FaceMeshConcept<Mesh>) {
                if (hasFaces) {
                    vcl::triangulatedFaceVertexIndicesToBuffer(
                        mesh,
                        triIndexBuffer.data(),
                        indexMap,
                        vcl::MatrixStorageType::ROW_MAJOR,
                        numTris);

                    if (!vertsToDuplicate.empty()) {
                        vcl::
                            replaceTriangulatedFaceVertexIndicesByVertexDuplicationToBuffer(
                                mesh,
                                vertsToDuplicate,
                                facesToReassign,
                                indexMap,
                                triIndexBuffer.data());
                    }

                    using FaceType = typename Mesh::FaceType;
                    auto faceComp  = [&](const FaceType& f1,
                                        const FaceType& f2) {
                        if constexpr (vcl::HasPerVertexMaterialIndex<Mesh>) {
                            if (vcl::isPerVertexMaterialIndexAvailable(mesh)) {
                                vcl::uint id1 = f1.vertex(0)->materialIndex();
                                vcl::uint id2 = f2.vertex(0)->materialIndex();
                                if (id1 != id2)
                                    return id1 < id2;
                            }
                        }
                        if constexpr (vcl::HasPerFaceMaterialIndex<Mesh>) {
                            if (vcl::isPerFaceMaterialIndexAvailable(mesh)) {
                                vcl::uint id1 = f1.materialIndex();
                                vcl::uint id2 = f2.materialIndex();
                                if (id1 != id2)
                                    return id1 < id2;
                            }
                        }
                        return mesh.index(f1) < mesh.index(f2);
                    };

                    vcl::permuteFaceVertexIndicesByFunctionToBuffer(
                        mesh, triIndexBuffer.data(), faceComp, indexMap);

                    if constexpr (vcl::HasPerFaceNormal<Mesh>) {
                        if (hasFaceNormals) {
                            vcl::triangulatedFaceNormalsToBuffer(
                                mesh, triNormalBuffer.data(), indexMap);
                        }
                    }

                    if constexpr (vcl::HasPerFaceColor<Mesh>) {
                        if (hasFaceColors) {
                            vcl::triangulatedFaceColorsToBuffer(
                                mesh,
                                triColorBuffer.data(),
                                indexMap,
                                vcl::Color::Format::ABGR);
                        }
                    }

                    if (hasWireframe) {
                        vcl::wireframeVertexIndicesToBuffer(
                            mesh, wireframeIndexBuffer.data());
                    }
                }
            }

            if constexpr (vcl::EdgeMeshConcept<Mesh>) {
                if (hasEdges) {
                    vcl::edgeVertexIndicesToBuffer(
                        mesh, edgeIndexBuffer.data());

                    if constexpr (vcl::HasPerEdgeNormal<Mesh>) {
                        if (hasEdgeNormals) {
                            vcl::edgeNormalsToBuffer(
                                mesh, edgeNormalBuffer.data());
                        }
                    }

                    if constexpr (vcl::HasPerEdgeColor<Mesh>) {
                        if (hasEdgeColors) {
                            vcl::edgeColorsToBuffer(
                                mesh,
                                edgeColorBuffer.data(),
                                vcl::Color::Format::ABGR);
                        }
                    }
                }
            }

            return posBuffer.size() + triIndexBuffer.size();
        });
    });
