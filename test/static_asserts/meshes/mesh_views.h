#ifndef MESH_VIEWS_H
#define MESH_VIEWS_H

#include <ranges>

#include <vclib/mesh/views/elements.h>

template<typename MeshType>
void meshViewsStaticAsserts()
{
	MeshType m;
	const MeshType& cm = m;

	static_assert(
		std::ranges::range<typename MeshType::VertexView>,
		"The MeshType VertexView is not a valid range.");
	static_assert(
		std::ranges::range<typename MeshType::ConstVertexView>,
		"The MeshType ConstVertexView is not a valid range.");
	static_assert(
		std::ranges::range<decltype(m | vcl::views::vertices)>,
		"The view returned by pipe operation m | views::vertices is not a valid range.");
	static_assert(
		std::ranges::range<decltype(cm | vcl::views::vertices)>,
		"The view returned by pipe operation cm | views::vertices is not a valid range.");

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
	}

	if constexpr (vcl::HasHalfEdges<MeshType>) {
		static_assert(
			std::ranges::range<typename MeshType::HalfEdgeView>,
			"The MeshType HalfEdgeView is not a valid range.");
		static_assert(
			std::ranges::range<typename MeshType::ConstHalfEdgeView>,
			"The MeshType ConstHalfEdgeView is not a valid range.");
	}
}

#endif // MESH_VIEWS_H
