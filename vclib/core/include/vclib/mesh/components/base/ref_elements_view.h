// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BASE_REF_ELEMENTS_VIEW_H
#define VCL_MESH_COMPONENTS_BASE_REF_ELEMENTS_VIEW_H

#include <vclib/base.h>
#include <vclib/mesh/elements/base/base.h>

#include <ranges>

namespace vcl::comp {

namespace detail {

template<typename MeshType, ElemId REF_ELEM_ID>
struct ElementTransform
{
    MeshType* mMesh = nullptr;

    decltype(auto) operator()(auto idx) const
    {
        return mMesh->template element<REF_ELEM_ID>(idx);
    }
};

} // namespace detail

/**
 * @brief View class providing lazy access to referenced elements, with fast
 * component access paths that bypass ElementProxy construction.
 *
 * Inherits from `std::ranges::transform_view` to reuse standard C++ range and
 * iterator machinery with zero boilerplate.
 */
template<typename MeshType, typename ContainerType, ElemId REF_ELEM_ID>
class RefElementsView :
        public std::ranges::transform_view<
            std::ranges::views::all_t<ContainerType&>,
            detail::ElementTransform<MeshType, REF_ELEM_ID>>
{
    using Base = std::ranges::transform_view<
        std::ranges::views::all_t<ContainerType&>,
        detail::ElementTransform<MeshType, REF_ELEM_ID>>;

    MeshType*      mMesh      = nullptr;
    ContainerType* mContainer = nullptr;

public:
    RefElementsView() = default;

    RefElementsView(MeshType& mesh, ContainerType& container) :
            Base(
                std::views::all(container),
                detail::ElementTransform<MeshType, REF_ELEM_ID> {&mesh}),
            mMesh(&mesh), mContainer(&container)
    {
    }

    // Component shortcuts that allow views::positions, views::normals, etc.
    // to bypass ElementProxy construction and directly index SoA column buffers
    auto positions() const requires (REF_ELEM_ID == ElemId::VERTEX)
    {
        auto indices = vcl::View(mContainer->begin(), mContainer->end());
        return indices |
               std::views::transform([m = mMesh](auto idx) -> const auto& {
                   return m->vertex(idx).position();
               });
    }

    auto normals() const requires (REF_ELEM_ID == ElemId::VERTEX)
    {
        auto indices = vcl::View(mContainer->begin(), mContainer->end());
        return indices |
               std::views::transform([m = mMesh](auto idx) -> const auto& {
                   return m->vertex(idx).normal();
               });
    }

    auto colors() const requires (REF_ELEM_ID == ElemId::VERTEX)
    {
        auto indices = vcl::View(mContainer->begin(), mContainer->end());
        return indices |
               std::views::transform([m = mMesh](auto idx) -> const auto& {
                   return m->vertex(idx).color();
               });
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASE_REF_ELEMENTS_VIEW_H
