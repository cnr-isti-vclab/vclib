// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_WEDGES_WEDGE_CONTAINER_PROXY_H
#define VCL_MESH_COMPONENTS_WEDGES_WEDGE_CONTAINER_PROXY_H

#include "wedge_iterator.h"
#include "wedge_proxy.h"
#include "wedge_traits.h"

#include <vclib/base.h>
#include <vclib/base/views/view.h>

#include <cassert>
#include <concepts>
#include <ranges>
#include <utility>

namespace vcl::comp {

/**
 * @brief The WedgeContainerProxy class represents the container view of all
 * wedges (corners) of a specific Face.
 *
 * It provides:
 * 1. Range iteration over WedgeProxy (for auto w : f.wedges())
 * 2. Component views for direct column access (f.wedges().colors(),
 *    f.wedges().normals(), etc.)
 *
 * @tparam FaceType: The type of the parent Face (mutable or const).
 *
 * @ingroup elements
 */
template<typename FaceType>
class WedgeContainerProxy :
        public std::ranges::view_interface<WedgeContainerProxy<FaceType>>
{
    FaceType mFace;

    template<typename Self>
    using ResolvedFace = std::conditional_t<
        std::is_const_v<std::remove_reference_t<Self>>,
        typename std::remove_cvref_t<
            decltype(std::declval<FaceType&>()
                         .parentContainer())>::ConstElementType,
        FaceType>;

public:
    template<typename>
    friend class WedgeContainerProxy;

    WedgeContainerProxy(FaceType face) : mFace(face) {}

    const FaceType& parentFace() const { return mFace; }

    // Converting constructor from mutable to const (or convertible) FaceType
    template<typename OtherFace>
    requires std::convertible_to<OtherFace, FaceType>
    WedgeContainerProxy(const WedgeContainerProxy<OtherFace>& other) :
            mFace(other.parentFace())
    {
    }

    /**
     * @brief Number of wedges in the face (matches vertex count).
     */
    uint size() const { return mFace.vertexCount(); }

    /**
     * @brief True if the face has no wedges.
     */
    bool empty() const { return size() == 0; }

    /**
     * @brief Access the i-th wedge proxy by its local corner position.
     * @param[in] i: The 0-based local corner index (uint, 0 <= i < size()).
     * @return A WedgeProxy for the i-th corner.
     */
    decltype(auto) operator[](this auto&& self, uint i)
    {
        return WedgeProxy<ResolvedFace<decltype(self)>>(self.mFace, i);
    }

    /**
     * @brief Bounds-checked access to the i-th wedge proxy by its local corner
     * position.
     * @param[in] i: The 0-based local corner index (uint, 0 <= i < size()).
     * @return A WedgeProxy for the i-th corner.
     */
    decltype(auto) at(this auto&& self, uint i)
    {
        assert(i < self.size());
        return WedgeProxy<ResolvedFace<decltype(self)>>(self.mFace, i);
    }

    decltype(auto) front(this auto&& self) { return self[0]; }

    decltype(auto) back(this auto&& self) { return self[self.size() - 1]; }

    /* Range Iteration over WedgeProxy */

    auto begin(this auto&& self)
    {
        return WedgeIterator<ResolvedFace<decltype(self)>>(self.mFace, 0);
    }

    auto end(this auto&& self)
    {
        return WedgeIterator<ResolvedFace<decltype(self)>>(
            self.mFace, self.size());
    }

    /* Component Views */

    /**
     * @brief View of the face's wedge colors.
     */
    auto colors(this auto&& self) requires HasWedgeColors<FaceType>
    {
        using Comp =
            ComponentTypeT<CompId::WEDGE_COLORS, typename FaceType::Components>;
        auto&& d = Comp::wedgeData(self.mFace);
        return vcl::View(d.begin(), d.end());
    }

    /**
     * @brief View of the face's wedge texture coordinates.
     */
    auto texCoords(this auto&& self) requires HasWedgeTexCoords<FaceType>
    {
        using Comp = ComponentTypeT<
            CompId::WEDGE_TEX_COORDS,
            typename FaceType::Components>;
        auto&& d = Comp::wedgeData(self.mFace);
        return vcl::View(d.begin(), d.end());
    }

    /**
     * @brief View of the face's wedge normals.
     */
    auto normals(this auto&& self) requires HasWedgeNormals<FaceType>
    {
        using Comp = ComponentTypeT<
            CompId::WEDGE_NORMALS,
            typename FaceType::Components>;
        auto&& d = Comp::wedgeData(self.mFace);
        return vcl::View(d.begin(), d.end());
    }

    /* Availability checks */

    bool isColorsAvailable() const requires HasWedgeColors<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_COLORS>();
    }

    bool isTexCoordsAvailable() const requires HasWedgeTexCoords<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_TEX_COORDS>();
    }

    bool isNormalsAvailable() const requires HasWedgeNormals<FaceType>
    {
        return mFace.template isComponentAvailable<CompId::WEDGE_NORMALS>();
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGES_WEDGE_CONTAINER_PROXY_H
