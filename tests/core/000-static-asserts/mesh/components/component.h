// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_COMPONENT_H
#define COMP_COMPONENT_H

#include <vclib/meshes.h>

void componentStaticAsserts()
{
    using namespace vcl;

    using TEMVertex = VertexType<TriEdgeMesh>;
    using TEMFace   = FaceType<TriEdgeMesh>;
    using TEMEdge   = EdgeType<TriEdgeMesh>;

    using OptionalAdjEdges    = face::OptionalAdjacentEdges<>;
    using OptionalWedgeColors = face::OptionalPolygonWedgeColors;

    // ComponentData
    static_assert(
        comp::ComponentData<void>,
        "void does not satisfy the ComponentData concept");
    static_assert(
        comp::ComponentData<int>,
        "int does not satisfy the ComponentData concept");
    static_assert(
        comp::ComponentData<double>,
        "double does not satisfy the ComponentData concept");
    static_assert(
        comp::ComponentData<Point3f>,
        "Point3f does not satisfy the ComponentData concept");
    static_assert(
        comp::ComponentData<Color>,
        "Color does not satisfy the ComponentData concept");
    static_assert(
        comp::ComponentData<std::string>,
        "std::string does not satisfy the ComponentData concept");
    static_assert(
        comp::ComponentData<std::vector<int>>,
        "std::vector<int> does not satisfy the ComponentData concept");
    static_assert(
        comp::ComponentData<vcl::mesh::detail::MData>,
        "MData does not satisfy the ComponentData concept");

    struct NonSerializableData
    {
        int a;
    };
    static_assert(
        !comp::ComponentData<NonSerializableData>,
        "NonSerializableData does satisfy the ComponentData concept");

    struct NonCopyableData
    {
        NonCopyableData() = default;
        NonCopyableData(const NonCopyableData&) = delete;
        void serialize(std::ostream&) const {}
        void deserialize(std::istream&) {}
    };
    static_assert(
        !comp::ComponentData<NonCopyableData>,
        "NonCopyableData does satisfy the ComponentData concept");

    struct NonDefaultConstructibleData
    {
        NonDefaultConstructibleData(int) {}
        void serialize(std::ostream&) const {}
        void deserialize(std::istream&) {}
    };
    static_assert(
        !comp::ComponentData<NonDefaultConstructibleData>,
        "NonDefaultConstructibleData does satisfy the ComponentData concept");

    // ComponentConcept
    static_assert(
        comp::ComponentConcept<vert::Color>,
        "vert::Color does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<const vert::Color>,
        "const vert::Color does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<vert::Color&>,
        "vert::Color& does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<const vert::Color&>,
        "const vert::Color& does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<vert::Color&&>,
        "vert::Color&& does not satisfy the Component concept");

    static_assert(
        comp::ComponentConcept<vert::OptionalColor>,
        "vert::OptionalColor does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<const vert::OptionalColor>,
        "const vert::OptionalColor does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<vert::OptionalColor&>,
        "vert::OptionalColor& does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<const vert::OptionalColor&>,
        "const vert::OptionalColor& does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<vert::OptionalColor&&>,
        "vert::OptionalColor&& does not satisfy the Component concept");

    static_assert(
        !comp::ComponentConcept<TEMVertex>,
        "TEMVertex does satisfy the Component concept");
    static_assert(
        !comp::ComponentConcept<const TEMVertex>,
        "const TEMVertex does satisfy the Component concept");
    static_assert(
        !comp::ComponentConcept<TEMVertex&>,
        "TEMVertex& does satisfy the Component concept");
    static_assert(
        !comp::ComponentConcept<const TEMVertex&>,
        "const TEMVertex& does satisfy the Component concept");
    static_assert(
        !comp::ComponentConcept<TEMVertex&&>,
        "TEMVertex&& does satisfy the Component concept");

    // IsOptionalComponent
    static_assert(
        !comp::IsOptionalComponent<vert::Color>,
        "vert::Color does satisfy the OptionalComponent concept");
    static_assert(
        !comp::IsOptionalComponent<const vert::Color>,
        "const vert::Color does satisfy the OptionalComponent concept");
    static_assert(
        !comp::IsOptionalComponent<vert::Color&>,
        "vert::Color& does satisfy the OptionalComponent concept");
    static_assert(
        !comp::IsOptionalComponent<const vert::Color&>,
        "const vert::Color& does satisfy the OptionalComponent concept");
    static_assert(
        !comp::IsOptionalComponent<vert::Color&&>,
        "vert::Color&& does satisfy the OptionalComponent concept");

    static_assert(
        comp::IsOptionalComponent<vert::OptionalColor>,
        "vert::OptionalColor does not satisfy the OptionalComponent concept");
    static_assert(
        comp::IsOptionalComponent<const vert::OptionalColor>,
        "const vert::OptionalColor does not satisfy the OptionalComponent "
        "concept");
    static_assert(
        comp::IsOptionalComponent<vert::OptionalColor&>,
        "vert::OptionalColor& does not satisfy the OptionalComponent concept");
    static_assert(
        comp::IsOptionalComponent<const vert::OptionalColor&>,
        "const vert::OptionalColor& does not satisfy the OptionalComponent "
        "concept");
    static_assert(
        comp::IsOptionalComponent<vert::OptionalColor&&>,
        "vert::OptionalColor&& does not satisfy the OptionalComponent concept");

    // IsTiedToVertexCount
    static_assert(
        !comp::IsTiedToVertexCount<OptionalAdjEdges>,
        "OptionalAdjEdges does satisfy the IsTiedToVertexCount concept");
    static_assert(
        !comp::IsTiedToVertexCount<const OptionalAdjEdges>,
        "const OptionalAdjEdges does satisfy the IsTiedToVertexCount concept");
    static_assert(
        !comp::IsTiedToVertexCount<OptionalAdjEdges&>,
        "OptionalAdjEdges& does satisfy the IsTiedToVertexCount concept");
    static_assert(
        !comp::IsTiedToVertexCount<const OptionalAdjEdges&>,
        "const OptionalAdjEdges& does satisfy the IsTiedToVertexCount concept");
    static_assert(
        !comp::IsTiedToVertexCount<OptionalAdjEdges&&>,
        "OptionalAdjEdges&& does satisfy the IsTiedToVertexCount concept");

    static_assert(
        comp::IsTiedToVertexCount<OptionalWedgeColors>,
        "OptionalWedgeColors does not satisfy the IsTiedToVertexCount concept");
    static_assert(
        comp::IsTiedToVertexCount<const OptionalWedgeColors>,
        "const OptionalWedgeColors does not satisfy the IsTiedToVertexCount "
        "concept");
    static_assert(
        comp::IsTiedToVertexCount<OptionalWedgeColors&>,
        "OptionalWedgeColors& does not satisfy the IsTiedToVertexCount "
        "concept");
    static_assert(
        comp::IsTiedToVertexCount<const OptionalWedgeColors&>,
        "const OptionalWedgeColors& does not satisfy the IsTiedToVertexCount "
        "concept");
    static_assert(
        comp::IsTiedToVertexCount<OptionalWedgeColors&&>,
        "OptionalWedgeColors&& does not satisfy the IsTiedToVertexCount "
        "concept");

    // HasReferencesTo
    static_assert(
        comp::HasReferencesTo<ElemId::EDGE, OptionalAdjEdges>,
        "OptionalAdjEdges does not satisfy the HasReferencesTo EDGE concept");
    static_assert(
        comp::HasReferencesTo<ElemId::EDGE, const OptionalAdjEdges>,
        "const OptionalAdjEdges does not satisfy the HasReferencesTo EDGE "
        "concept");
    static_assert(
        comp::HasReferencesTo<ElemId::EDGE, OptionalAdjEdges&>,
        "OptionalAdjEdges& does not satisfy the HasReferencesTo EDGE concept");
    static_assert(
        comp::HasReferencesTo<ElemId::EDGE, const OptionalAdjEdges&>,
        "const OptionalAdjEdges& does not satisfy the HasReferencesTo EDGE "
        "concept");
    static_assert(
        comp::HasReferencesTo<ElemId::EDGE, OptionalAdjEdges&&>,
        "OptionalAdjEdges&& does not satisfy the HasReferencesTo EDGE concept");

    static_assert(
        !comp::HasReferencesTo<ElemId::FACE, OptionalAdjEdges>,
        "OptionalAdjEdges does satisfy the HasReferencesTo FACE concept");
    static_assert(
        !comp::HasReferencesTo<ElemId::FACE, const OptionalAdjEdges>,
        "const OptionalAdjEdges does satisfy the HasReferencesTo FACE concept");
    static_assert(
        !comp::HasReferencesTo<ElemId::FACE, OptionalAdjEdges&>,
        "OptionalAdjEdges& does satisfy the HasReferencesTo FACE concept");
    static_assert(
        !comp::HasReferencesTo<ElemId::FACE, const OptionalAdjEdges&>,
        "const OptionalAdjEdges& does satisfy the HasReferencesTo FACE "
        "concept");
    static_assert(
        !comp::HasReferencesTo<ElemId::FACE, OptionalAdjEdges&&>,
        "OptionalAdjEdges&& does satisfy the HasReferencesTo FACE concept");

    static_assert(
        comp::HasReferencesTo<ElemId::EDGE, TEMFace>,
        "TEMFace does not satisfy the HasReferencesTo EDGE concept");
    static_assert(
        comp::HasReferencesTo<ElemId::VERTEX, TEMFace>,
        "TEMFace does not satisfy the HasReferencesTo VERTEX concept");
    static_assert(
        comp::HasReferencesTo<ElemId::EDGE, TEMVertex>,
        "TEMVertex does not satisfy the HasReferencesTo EDGE concept");

    // HasOptionalReferencesTo
    static_assert(
        comp::HasOptionalReferencesTo<ElemId::EDGE, OptionalAdjEdges>,
        "OptionalAdjEdges does not satisfy the HasOptionalReferencesTo EDGE "
        "concept");
    static_assert(
        comp::HasOptionalReferencesTo<ElemId::EDGE, const OptionalAdjEdges>,
        "const OptionalAdjEdges does not satisfy the HasOptionalReferencesTo "
        "EDGE concept");
    static_assert(
        comp::HasOptionalReferencesTo<ElemId::EDGE, OptionalAdjEdges&>,
        "OptionalAdjEdges& does not satisfy the HasOptionalReferencesTo EDGE "
        "concept");
    static_assert(
        comp::HasOptionalReferencesTo<ElemId::EDGE, const OptionalAdjEdges&>,
        "const OptionalAdjEdges& does not satisfy the HasOptionalReferencesTo "
        "EDGE concept");
    static_assert(
        comp::HasOptionalReferencesTo<ElemId::EDGE, OptionalAdjEdges&&>,
        "OptionalAdjEdges&& does not satisfy the HasOptionalReferencesTo EDGE "
        "concept");

    static_assert(
        !comp::HasOptionalReferencesTo<ElemId::FACE, OptionalAdjEdges>,
        "OptionalAdjEdges does satisfy the HasOptionalReferencesTo FACE "
        "concept");
    static_assert(
        !comp::HasOptionalReferencesTo<ElemId::FACE, const OptionalAdjEdges>,
        "const OptionalAdjEdges does satisfy the HasOptionalReferencesTo FACE "
        "concept");
    static_assert(
        !comp::HasOptionalReferencesTo<ElemId::FACE, OptionalAdjEdges&>,
        "OptionalAdjEdges& does satisfy the HasOptionalReferencesTo FACE "
        "concept");
    static_assert(
        !comp::HasOptionalReferencesTo<ElemId::FACE, const OptionalAdjEdges&>,
        "const OptionalAdjEdges& does satisfy the HasOptionalReferencesTo FACE "
        "concept");
    static_assert(
        !comp::HasOptionalReferencesTo<ElemId::FACE, OptionalAdjEdges&&>,
        "OptionalAdjEdges&& does satisfy the HasOptionalReferencesTo FACE "
        "concept");

    static_assert(
        comp::HasOptionalReferencesTo<ElemId::EDGE, TEMFace>,
        "TEMFace does not satisfy the HasOptionalReferencesTo EDGE concept");
    static_assert(
        !comp::HasOptionalReferencesTo<ElemId::VERTEX, TEMFace>,
        "TEMFace does satisfy the HasOptionalReferencesTo VERTEX concept");

    // HasComponent
    static_assert(
        comp::HasComponent<CompId::POSITION, TEMVertex>,
        "TEMVertex does not satisfy the HasComponent POSITION concept");
    static_assert(
        comp::HasComponent<CompId::POSITION, const TEMVertex>,
        "const TEMVertex does not satisfy the HasComponent POSITION concept");
    static_assert(
        comp::HasComponent<CompId::POSITION, TEMVertex&>,
        "TEMVertex& does not satisfy the HasComponent POSITION concept");
    static_assert(
        comp::HasComponent<CompId::POSITION, const TEMVertex&>,
        "const TEMVertex& does not satisfy the HasComponent POSITION concept");
    static_assert(
        comp::HasComponent<CompId::POSITION, TEMVertex&&>,
        "TEMVertex&& does not satisfy the HasComponent POSITION concept");

    static_assert(
        comp::HasComponent<CompId::COLOR, TEMVertex>,
        "TEMVertex does not satisfy the HasComponent COLOR concept");
    static_assert(
        comp::HasComponent<CompId::COLOR, const TEMVertex>,
        "const TEMVertex does not satisfy the HasComponent COLOR concept");
    static_assert(
        comp::HasComponent<CompId::COLOR, TEMVertex&>,
        "TEMVertex& does not satisfy the HasComponent COLOR concept");
    static_assert(
        comp::HasComponent<CompId::COLOR, const TEMVertex&>,
        "const TEMVertex& does not satisfy the HasComponent COLOR concept");
    static_assert(
        comp::HasComponent<CompId::COLOR, TEMVertex&&>,
        "TEMVertex&& does not satisfy the HasComponent COLOR concept");

    static_assert(
        !comp::HasComponent<CompId::VERTEX_REFERENCES, TEMVertex>,
        "TEMVertex does satisfy the HasComponent VERTEX_REFERENCES concept");
    static_assert(
        !comp::HasComponent<CompId::VERTEX_REFERENCES, const TEMVertex>,
        "const TEMVertex does satisfy the HasComponent VERTEX_REFERENCES "
        "concept");
    static_assert(
        !comp::HasComponent<CompId::VERTEX_REFERENCES, TEMVertex&>,
        "TEMVertex& does satisfy the HasComponent VERTEX_REFERENCES concept");
    static_assert(
        !comp::HasComponent<CompId::VERTEX_REFERENCES, const TEMVertex&>,
        "const TEMVertex& does satisfy the HasComponent VERTEX_REFERENCES "
        "concept");
    static_assert(
        !comp::HasComponent<CompId::VERTEX_REFERENCES, TEMVertex&&>,
        "TEMVertex&& does satisfy the HasComponent VERTEX_REFERENCES concept");

    // HasOptionalComponent
    static_assert(
        !comp::HasOptionalComponent<CompId::POSITION, TEMVertex>,
        "TEMVertex does satisfy the HasOptionalComponent POSITION concept");
    static_assert(
        !comp::HasOptionalComponent<CompId::POSITION, const TEMVertex>,
        "const TEMVertex does satisfy the HasOptionalComponent POSITION "
        "concept");
    static_assert(
        !comp::HasOptionalComponent<CompId::POSITION, TEMVertex&>,
        "TEMVertex& does satisfy the HasOptionalComponent POSITION concept");
    static_assert(
        !comp::HasOptionalComponent<CompId::POSITION, const TEMVertex&>,
        "const TEMVertex& does satisfy the HasOptionalComponent POSITION "
        "concept");
    static_assert(
        !comp::HasOptionalComponent<CompId::POSITION, TEMVertex&&>,
        "TEMVertex&& does satisfy the HasOptionalComponent POSITION concept");

    static_assert(
        comp::HasOptionalComponent<CompId::COLOR, TEMVertex>,
        "TEMVertex does not satisfy the HasOptionalComponent COLOR concept");
    static_assert(
        comp::HasOptionalComponent<CompId::COLOR, const TEMVertex>,
        "const TEMVertex does not satisfy the HasOptionalComponent COLOR "
        "concept");
    static_assert(
        comp::HasOptionalComponent<CompId::COLOR, TEMVertex&>,
        "TEMVertex& does not satisfy the HasOptionalComponent COLOR concept");
    static_assert(
        comp::HasOptionalComponent<CompId::COLOR, const TEMVertex&>,
        "const TEMVertex& does not satisfy the HasOptionalComponent COLOR "
        "concept");
    static_assert(
        comp::HasOptionalComponent<CompId::COLOR, TEMVertex&&>,
        "TEMVertex&& does not satisfy the HasOptionalComponent COLOR concept");
}

#endif // COMP_COMPONENT_H
