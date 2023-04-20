#ifndef MESH_VIEWS_H
#define MESH_VIEWS_H

#include <ranges>

#include <vclib/views.h>

template<typename MeshType>
void meshViewsStaticAsserts()
{
	MeshType m;
	const MeshType& cm = m;

	static_assert(
		vcl::VertexRangeConcept<typename MeshType::VertexView>,
		"The MeshType VertexView is not a valid range of vertices.");

	static_assert(
		vcl::VertexRangeConcept<typename MeshType::ConstVertexView>,
		"The MeshType VertexView is not a valid range of vertices.");

#ifdef VCLIB_USES_RANGES
	static_assert(
		std::ranges::range<decltype(m | vcl::views::vertices)>,
		"The view returned by pipe operation m | views::vertices is not a valid range.");
	static_assert(
		std::is_same_v<decltype(m | vcl::views::vertices), typename MeshType::VertexView>,
		"The view returned by pipe operation m | views::vertices is not a VertexView.");
	static_assert(
		std::ranges::range<decltype(cm | vcl::views::vertices)>,
		"The view returned by pipe operation cm | views::vertices is not a valid range.");
	static_assert(
		std::is_same_v<decltype(cm | vcl::views::vertices), typename MeshType::ConstVertexView>,
		"The view returned by pipe operation m | views::vertices is not a ConstVertexView.");

	static_assert(
		vcl::VertexPointerRangeConcept<decltype(m.vertices() | vcl::views::reference)>,
		"The view returned by pipe operation m | views::reference is not a "
		"VertexPointerRangeConcept.");

	static_assert(
		vcl::VertexPointerRangeConcept<decltype(cm.vertices() | vcl::views::reference)>,
		"The view returned by pipe operation cm | views::reference is not a "
		"VertexPointerRangeConcept.");

	// assure that component ranges are ranges
	static_assert(
		std::ranges::range<decltype(m.vertices() | vcl::views::colors)>, "");
	static_assert(
		std::ranges::range<decltype(m.vertices() | vcl::views::coords)>, "");
	static_assert(
		std::ranges::range<decltype(m.vertices() | vcl::views::scalars)>, "");
	static_assert(
		std::ranges::range<decltype(m.vertices() | vcl::views::selection)>, "");
	static_assert(
		std::ranges::range<decltype(m.vertices() | vcl::views::selected)>, "");
	static_assert(
		std::ranges::range<decltype(m.vertices() | vcl::views::selected | vcl::views::colors)>, "");
	static_assert(
		std::ranges::range<decltype(m.vertices() | vcl::views::notSelected)>, "");

	if constexpr (vcl::HasFaces<MeshType>) {
		static_assert(
			std::ranges::range<typename MeshType::FaceView>,
			"The MeshType FaceView is not a valid range.");
		static_assert(
			std::ranges::range<typename MeshType::ConstFaceView>,
			"The MeshType ConstFaceView is not a valid range.");

		static_assert(
			std::ranges::range<decltype(m | vcl::views::faces)>,
			"The view returned by pipe operation m | views::faces is not a valid range.");
		static_assert(
			std::ranges::range<decltype(cm | vcl::views::faces)>,
			"The view returned by pipe operation cm | views::faces is not a valid range.");
	}

	if constexpr (vcl::HasEdges<MeshType>) {
		static_assert(
			std::ranges::range<typename MeshType::EdgeView>,
			"The MeshType EdgeView is not a valid range.");
		static_assert(
			std::ranges::range<typename MeshType::ConstEdgeView>,
			"The MeshType ConstEdgeView is not a valid range.");

		static_assert(
			std::ranges::range<decltype(m | vcl::views::edges)>,
			"The view returned by pipe operation m | views::edges is not a valid range.");
		static_assert(
			std::ranges::range<decltype(cm | vcl::views::edges)>,
			"The view returned by pipe operation cm | views::edges is not a valid range.");
	}

	if constexpr (vcl::HasHalfEdges<MeshType>) {
		static_assert(
			std::ranges::range<typename MeshType::HalfEdgeView>,
			"The MeshType HalfEdgeView is not a valid range.");
		static_assert(
			std::ranges::range<typename MeshType::ConstHalfEdgeView>,
			"The MeshType ConstHalfEdgeView is not a valid range.");

		static_assert(
			std::ranges::range<decltype(m | vcl::views::halfEdges)>,
			"The view returned by pipe operation m | views::halfEdges is not a valid range.");
		static_assert(
			std::ranges::range<decltype(cm | vcl::views::halfEdges)>,
			"The view returned by pipe operation cm | views::halfEdges is not a valid range.");
	}
#endif
}

#endif // MESH_VIEWS_H