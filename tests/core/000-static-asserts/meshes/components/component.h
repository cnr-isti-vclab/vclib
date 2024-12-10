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

#ifndef COMP_COMPONENT_H
#define COMP_COMPONENT_H

#include <vclib/meshes.h>

void componentStaticAsserts()
{
    using namespace vcl;

    using TEMVertex = triedgemesh::Vertex<float, false>;
    using TEMFace   = triedgemesh::Face<float, false>;
    using TEMEdge   = triedgemesh::Edge<float, false>;

    using VerticalColor = vert::VerticalColor<trimesh::Vertex<float, false>>;
    using OptionalColor = vert::OptionalColor<trimesh::Vertex<float, false>>;

    using OptionalAdjEdges =
        face::OptionalAdjacentEdges<false, TEMEdge, TEMFace>;
    using OptionalWedgeColors = face::OptionalPolygonWedgeColors<TEMFace>;

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
        comp::ComponentConcept<VerticalColor>,
        "vert::VerticalColor does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<const VerticalColor>,
        "const vert::VerticalColor does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<VerticalColor&>,
        "vert::VerticalColor& does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<const VerticalColor&>,
        "const vert::VerticalColor& does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<VerticalColor&&>,
        "vert::VerticalColor&& does not satisfy the Component concept");

    static_assert(
        comp::ComponentConcept<OptionalColor>,
        "vert::OptionalColor does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<const OptionalColor>,
        "const vert::OptionalColor does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<OptionalColor&>,
        "vert::OptionalColor& does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<const OptionalColor&>,
        "const vert::OptionalColor& does not satisfy the Component concept");
    static_assert(
        comp::ComponentConcept<OptionalColor&&>,
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

    // IsVerticalComponent
    static_assert(
        !comp::IsVerticalComponent<vert::Color>,
        "vert::Color does satisfy the VerticalComponent concept");
    static_assert(
        !comp::IsVerticalComponent<const vert::Color>,
        "const vert::Color does satisfy the VerticalComponent concept");
    static_assert(
        !comp::IsVerticalComponent<vert::Color&>,
        "vert::Color& does satisfy the VerticalComponent concept");
    static_assert(
        !comp::IsVerticalComponent<const vert::Color&>,
        "const vert::Color& does satisfy the VerticalComponent concept");
    static_assert(
        !comp::IsVerticalComponent<vert::Color&&>,
        "vert::Color&& does satisfy the VerticalComponent concept");

    static_assert(
        comp::IsVerticalComponent<VerticalColor>,
        "vert::VerticalColor does not satisfy the VerticalComponent concept");
    static_assert(
        comp::IsVerticalComponent<const VerticalColor>,
        "const vert::VerticalColor does not satisfy the VerticalComponent "
        "concept");
    static_assert(
        comp::IsVerticalComponent<VerticalColor&>,
        "vert::VerticalColor& does not satisfy the VerticalComponent concept");
    static_assert(
        comp::IsVerticalComponent<const VerticalColor&>,
        "const vert::VerticalColor& does not satisfy the VerticalComponent "
        "concept");
    static_assert(
        comp::IsVerticalComponent<VerticalColor&&>,
        "vert::VerticalColor&& does not satisfy the VerticalComponent concept");

    static_assert(
        comp::IsVerticalComponent<OptionalColor>,
        "vert::OptionalColor does not satisfy the VerticalComponent concept");
    static_assert(
        comp::IsVerticalComponent<const OptionalColor>,
        "const vert::OptionalColor does not satisfy the VerticalComponent "
        "concept");
    static_assert(
        comp::IsVerticalComponent<OptionalColor&>,
        "vert::OptionalColor& does not satisfy the VerticalComponent concept");
    static_assert(
        comp::IsVerticalComponent<const OptionalColor&>,
        "const vert::OptionalColor& does not satisfy the VerticalComponent "
        "concept");
    static_assert(
        comp::IsVerticalComponent<OptionalColor&&>,
        "vert::OptionalColor&& does not satisfy the VerticalComponent concept");

    // IsOptionalComponent
    static_assert(
        !comp::IsOptionalComponent<vert::Color>,
        "vert::Color doesIsTiedToVertexNumber satisfy the OptionalComponent concept");
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
        !comp::IsOptionalComponent<VerticalColor>,
        "vert::VerticalColor does satisfy the OptionalComponent concept");
    static_assert(
        !comp::IsOptionalComponent<const VerticalColor>,
        "const vert::VerticalColor does satisfy the OptionalComponent concept");
    static_assert(
        !comp::IsOptionalComponent<VerticalColor&>,
        "vert::VerticalColor& does satisfy the OptionalComponent concept");
    static_assert(
        !comp::IsOptionalComponent<const VerticalColor&>,
        "const vert::VerticalColor& does satisfy the OptionalComponent "
        "concept");
    static_assert(
        !comp::IsOptionalComponent<VerticalColor&&>,
        "vert::VerticalColor&& does satisfy the OptionalComponent concept");

    static_assert(
        comp::IsOptionalComponent<OptionalColor>,
        "vert::OptionalColor does not satisfy the OptionalComponent concept");
    static_assert(
        comp::IsOptionalComponent<const OptionalColor>,
        "const vert::OptionalColor does not satisfy the OptionalComponent "
        "concept");
    static_assert(
        comp::IsOptionalComponent<OptionalColor&>,
        "vert::OptionalColor& does not satisfy the OptionalComponent concept");
    static_assert(
        comp::IsOptionalComponent<const OptionalColor&>,
        "const vert::OptionalColor& does not satisfy the OptionalComponent "
        "concept");
    static_assert(
        comp::IsOptionalComponent<OptionalColor&&>,
        "vert::OptionalColor&& does not satisfy the OptionalComponent concept");

    // HasInitMemberFunction
    static_assert(
        !comp::HasInitMemberFunction<vert::Color>,
        "vert::Color does satisfy the HasInitMemberFunction concept");
    static_assert(
        !comp::HasInitMemberFunction<const vert::Color>,
        "const vert::Color does satisfy the HasInitMemberFunction concept");
    static_assert(
        !comp::HasInitMemberFunction<vert::Color&>,
        "vert::Color& does satisfy the HasInitMemberFunction concept");
    static_assert(
        !comp::HasInitMemberFunction<const vert::Color&>,
        "const vert::Color& does satisfy the HasInitMemberFunction concept");
    static_assert(
        !comp::HasInitMemberFunction<vert::Color&&>,
        "vert::Color&& does satisfy the HasInitMemberFunction concept");

    static_assert(
        comp::HasInitMemberFunction<mesh::TransformMatrixd>,
        "mesh::TransformMatrixd does not satisfy the HasInitMemberFunction "
        "concept");
    static_assert(
        !comp::HasInitMemberFunction<const mesh::TransformMatrixd>,
        "const mesh::TransformMatrixd does satisfy the "
        "HasInitMemberFunction concept");
    static_assert(
        comp::HasInitMemberFunction<mesh::TransformMatrixd&>,
        "mesh::TransformMatrixd& does not satisfy the HasInitMemberFunction "
        "concept");
    static_assert(
        !comp::HasInitMemberFunction<const mesh::TransformMatrixd&>,
        "const mesh::TransformMatrixd& does satisfy the "
        "HasInitMemberFunction concept");
    static_assert(
        comp::HasInitMemberFunction<mesh::TransformMatrixd&&>,
        "mesh::TransformMatrixd&& does not satisfy the HasInitMemberFunction "
        "concept");

    // HasIsAvailableMemberFunction
    static_assert(
        comp::HasIsAvailableMemberFunction<vert::Color>,
        "vert::Color does not satisfy the HasIsAvailableMemberFunction "
        "concept");
    static_assert(
        comp::HasIsAvailableMemberFunction<const vert::Color>,
        "const vert::Color does not satisfy the HasIsAvailableMemberFunction "
        "concept");
    static_assert(
        comp::HasIsAvailableMemberFunction<vert::Color&>,
        "vert::Color& does not satisfy the HasIsAvailableMemberFunction "
        "concept");
    static_assert(
        comp::HasIsAvailableMemberFunction<const vert::Color&>,
        "const vert::Color& does not satisfy the HasIsAvailableMemberFunction "
        "concept");
    static_assert(
        comp::HasIsAvailableMemberFunction<vert::Color&&>,
        "vert::Color&& does not satisfy the HasIsAvailableMemberFunction "
        "concept");

    // IsTiedToVertexNumber
    static_assert(
        !comp::IsTiedToVertexNumber<OptionalAdjEdges>,
        "OptionalAdjEdges does satisfy the IsTiedToVertexNumber "
        "concept");
    static_assert(
        !comp::IsTiedToVertexNumber<const OptionalAdjEdges>,
        "const OptionalAdjEdges does satisfy the IsTiedToVertexNumber "
        "concept");
    static_assert(
        !comp::IsTiedToVertexNumber<OptionalAdjEdges&>,
        "OptionalAdjEdges& does satisfy the IsTiedToVertexNumber "
        "concept");
    static_assert(
        !comp::IsTiedToVertexNumber<const OptionalAdjEdges&>,
        "const OptionalAdjEdges& does satisfy the IsTiedToVertexNumber "
        "concept");
    static_assert(
        !comp::IsTiedToVertexNumber<OptionalAdjEdges&&>,
        "OptionalAdjEdges&& does satisfy the IsTiedToVertexNumber "
        "concept");

    static_assert(
        comp::IsTiedToVertexNumber<OptionalWedgeColors>,
        "OptionalWedgeColors does not satisfy the IsTiedToVertexNumber "
        "concept");
    static_assert(
        comp::IsTiedToVertexNumber<const OptionalWedgeColors>,
        "const OptionalWedgeColors does not satisfy the IsTiedToVertexNumber "
        "concept");
    static_assert(
        comp::IsTiedToVertexNumber<OptionalWedgeColors&>,
        "OptionalWedgeColors& does not satisfy the IsTiedToVertexNumber "
        "concept");
    static_assert(
        comp::IsTiedToVertexNumber<const OptionalWedgeColors&>,
        "const OptionalWedgeColors& does not satisfy the IsTiedToVertexNumber "
        "concept");
    static_assert(
        comp::IsTiedToVertexNumber<OptionalWedgeColors&&>,
        "OptionalWedgeColors&& does not satisfy the IsTiedToVertexNumber "
        "concept");

    // HasReferencesOfType
    static_assert(
        comp::HasReferencesOfType<OptionalAdjEdges, TEMEdge>,
        "OptionalAdjEdges does not satisfy the HasReferencesOfType TEMEdge "
        "concept");
    static_assert(
        comp::HasReferencesOfType<const OptionalAdjEdges, TEMEdge>,
        "const OptionalAdjEdges does not satisfy the HasReferencesOfType "
        "TEMEdge concept");
    static_assert(
        comp::HasReferencesOfType<OptionalAdjEdges&, TEMEdge>,
        "OptionalAdjEdges& does not satisfy the HasReferencesOfType TEMEdge "
        "concept");
    static_assert(
        comp::HasReferencesOfType<const OptionalAdjEdges&, TEMEdge>,
        "const OptionalAdjEdges& does not satisfy the HasReferencesOfType "
        "TEMEdge concept");
    static_assert(
        comp::HasReferencesOfType<OptionalAdjEdges&&, TEMEdge>,
        "OptionalAdjEdges&& does not satisfy the HasReferencesOfType TEMEdge "
        "concept");
    static_assert(
        comp::HasReferencesOfType<OptionalAdjEdges, TEMEdge&>,
        "OptionalAdjEdges does not satisfy the HasReferencesOfType TEMEdge& "
        "concept");
    static_assert(
        comp::HasReferencesOfType<OptionalAdjEdges, TEMEdge&&>,
        "OptionalAdjEdges does not satisfy the HasReferencesOfType TEMEdge&& "
        "concept");
    static_assert(
        !comp::HasReferencesOfType<OptionalAdjEdges, const TEMEdge&>,
        "OptionalAdjEdges does satisfy the HasReferencesOfType const TEMEdge& "
        "concept");

    static_assert(
        !comp::HasReferencesOfType<OptionalAdjEdges, TEMFace>,
        "OptionalAdjEdges does satisfy the HasReferencesOfType TEMFace concept");
    static_assert(
        !comp::HasReferencesOfType<const OptionalAdjEdges, TEMFace>,
        "const OptionalAdjEdges does satisfy the HasReferencesOfType TEMFace "
        "concept");
    static_assert(
        !comp::HasReferencesOfType<OptionalAdjEdges&, TEMFace>,
        "OptionalAdjEdges& does satisfy the HasReferencesOfType TEMFace concept");
    static_assert(
        !comp::HasReferencesOfType<const OptionalAdjEdges&, TEMFace>,
        "const OptionalAdjEdges& does satisfy the HasReferencesOfType TEMFace "
        "concept");
    static_assert(
        !comp::HasReferencesOfType<OptionalAdjEdges&&, TEMFace>,
        "OptionalAdjEdges&& does satisfy the HasReferencesOfType TEMFace concept");
    static_assert(
        !comp::HasReferencesOfType<OptionalAdjEdges, TEMFace&>,
        "OptionalAdjEdges does satisfy the HasReferencesOfType TEMFace concept");
    static_assert(
        !comp::HasReferencesOfType<OptionalAdjEdges, TEMFace&&>,
        "OptionalAdjEdges does satisfy the HasReferencesOfType TEMFace concept");
    static_assert(
        !comp::HasReferencesOfType<OptionalAdjEdges, const TEMFace&>,
        "OptionalAdjEdges does satisfy the HasReferencesOfType TEMFace concept");

    // HasOptionalReferencesOfType
    static_assert(
        comp::HasOptionalReferencesOfType<OptionalAdjEdges, TEMEdge>,
        "OptionalAdjEdges does not satisfy the HasOptionalReferencesOfType "
        "TEMEdge concept");
    static_assert(
        comp::HasOptionalReferencesOfType<const OptionalAdjEdges, TEMEdge>,
        "const OptionalAdjEdges does not satisfy the "
        "HasOptionalReferencesOfType TEMEdge concept");
    static_assert(
        comp::HasOptionalReferencesOfType<OptionalAdjEdges&, TEMEdge>,
        "OptionalAdjEdges& does not satisfy the HasOptionalReferencesOfType "
        "TEMEdge concept");
    static_assert(
        comp::HasOptionalReferencesOfType<const OptionalAdjEdges&, TEMEdge>,
        "const OptionalAdjEdges& does not satisfy the "
        "HasOptionalReferencesOfType TEMEdge concept");
    static_assert(
        comp::HasOptionalReferencesOfType<OptionalAdjEdges&&, TEMEdge>,
        "OptionalAdjEdges&& does not satisfy the HasOptionalReferencesOfType "
        "TEMEdge concept");
    static_assert(
        comp::HasOptionalReferencesOfType<OptionalAdjEdges, TEMEdge&>,
        "OptionalAdjEdges does not satisfy the HasOptionalReferencesOfType "
        "TEMEdge& concept");
    static_assert(
        comp::HasOptionalReferencesOfType<OptionalAdjEdges, TEMEdge&&>,
        "OptionalAdjEdges does not satisfy the HasOptionalReferencesOfType "
        "TEMEdge&& concept");
    static_assert(
        !comp::HasOptionalReferencesOfType<OptionalAdjEdges, const TEMEdge&>,
        "OptionalAdjEdges does satisfy the HasOptionalReferencesOfType const "
        "TEMEdge& concept");
}

#endif // COMP_COMPONENT_H
