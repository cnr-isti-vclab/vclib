#ifndef MESH_VIEWS_H
#define MESH_VIEWS_H

#include <vclib/meshes.h>

#include <ranges>

template<typename MeshType>
void meshViewsStaticAsserts()
{
    MeshType        m;
    const MeshType& cm = m;

    static_assert(
        vcl::VertexRangeConcept<decltype(m.vertices())>,
        "The MeshType VertexView is not a valid range of vertices.");

    static_assert(
        vcl::VertexRangeConcept<decltype(cm.vertices())>,
        "The MeshType ConstVertexView is not a valid range of vertices.");

    static_assert(
        std::ranges::range<decltype(m | vcl::views::vertices)>,
        "The view returned by pipe operation m | views::vertices is not a "
        "valid range.");
    static_assert(
        std::ranges::range<decltype(cm | vcl::views::vertices)>,
        "The view returned by pipe operation cm | views::vertices is not a "
        "valid range.");

    static_assert(
        vcl::VertexPointerRangeConcept<
            decltype(m.vertices() | vcl::views::addrOf)>,
        "The view returned by pipe operation m | views::reference is not a "
        "VertexPointerRangeConcept.");

    static_assert(
        vcl::VertexPointerRangeConcept<
            decltype(cm.vertices() | vcl::views::addrOf)>,
        "The view returned by pipe operation cm | views::reference is not a "
        "VertexPointerRangeConcept.");

    // assure that component ranges are ranges
    static_assert(
        std::ranges::range<decltype(m.vertices() | vcl::views::colors)>, "");
    static_assert(
        std::ranges::range<decltype(m.vertices() | vcl::views::positions)>, "");
    static_assert(
        std::ranges::range<decltype(m.vertices() | vcl::views::quality)>, "");
    static_assert(
        std::ranges::range<decltype(m.vertices() | vcl::views::selection)>, "");
    static_assert(
        std::ranges::range<decltype(m.vertices() | vcl::views::selected)>, "");
    static_assert(
        std::ranges::range<
            decltype(m.vertices() | vcl::views::selected | vcl::views::colors)>,
        "");
    static_assert(
        std::ranges::range<decltype(m.vertices() | vcl::views::notSelected)>,
        "");

    if constexpr (vcl::HasFaces<MeshType>) {
        static_assert(
            std::ranges::range<decltype(m.faces())>,
            "The MeshType FaceView is not a valid range.");
        static_assert(
            std::ranges::range<decltype(cm.faces())>,
            "The MeshType ConstFaceView is not a valid range.");

        static_assert(
            std::ranges::range<decltype(m | vcl::views::faces)>,
            "The view returned by pipe operation m | views::faces is not a "
            "valid range.");
        static_assert(
            std::ranges::range<decltype(cm | vcl::views::faces)>,
            "The view returned by pipe operation cm | views::faces is not a "
            "valid range.");

        auto&       f  = m.face(0);
        const auto& cf = cm.face(0);

        static_assert(
            vcl::VertexRangeConcept<decltype(f.vertices() | vcl::views::deref)>,
            "The view returned by f.vertices() is not a "
            "VertexRangeConcept.");

        static_assert(
            vcl::VertexRangeConcept<
                decltype(cf.vertices() | vcl::views::deref)>,
            "The view returned by cf.vertices() is not a "
            "VertexRangeConcept.");

        // TODO: fix these
        // static_assert(
        //     vcl::VertexPointerRangeConcept<decltype(f.vertices())>,
        //     "The view returned by f.vertices() is not a "
        //     "VertexPointerRangeConcept.");

        // static_assert(
        //     vcl::VertexPointerRangeConcept<decltype(cf.vertices())>,
        //     "The view returned by cf.vertices() is not a "
        //     "VertexPointerRangeConcept.");
    }

    if constexpr (vcl::HasEdges<MeshType>) {
        static_assert(
            std::ranges::range<decltype(m.edges())>,
            "The MeshType EdgeView is not a valid range.");
        static_assert(
            std::ranges::range<decltype(cm.edges())>,
            "The MeshType ConstEdgeView is not a valid range.");

        static_assert(
            std::ranges::range<decltype(m | vcl::views::edges)>,
            "The view returned by pipe operation m | views::edges is not a "
            "valid range.");
        static_assert(
            std::ranges::range<decltype(cm | vcl::views::edges)>,
            "The view returned by pipe operation cm | views::edges is not a "
            "valid range.");
    }
}

#endif // MESH_VIEWS_H
