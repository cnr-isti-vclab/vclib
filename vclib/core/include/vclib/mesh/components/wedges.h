// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_WEDGES_H
#define VCL_MESH_COMPONENTS_WEDGES_H

#include "base/component_predicates.h"
#include "base/container_component.h"
#include "color.h"
#include "normal.h"
#include "tex_coord.h"
#include "wedges/wedge_container_proxy.h"
#include "wedges/wedge_iterator.h"
#include "wedges/wedge_proxy.h"
#include "wedges/wedge_traits.h"

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <cassert>
#include <concepts>
#include <utility>

namespace vcl::comp {

/**
 * @brief The Wedges component represents a collection of per-wedge attributes
 * associated to the vertices/corners of an Element (typically Face).
 *
 * It is a ContainerComponent that wraps a container of attributes (e.g. Color,
 * TexCoord, Normal) whose size matches the vertex count of the face (e.g. 3 for
 * triangles, dynamic for polygons).
 *
 * For dynamic polygons (N < 0), Wedges is automatically marked as
 * TIED_TO_VERTEX_COUNT = true, so resizing the polygon's vertices automatically
 * resizes the wedge buffers.
 *
 * ### Indexing in Wedges: Local Corner Positions vs. Global Vertex Indices
 *
 * As a ContainerComponent:
 * - **Local Corner Position (`uint`)**:
 *   Accessing wedges is always indexed by a 0-based local corner position `uint
 *   i` (`0 <= i < f.vertexCount()`). Calling `f.wedge(uint i)` returns a
 *   `WedgeProxy` bound to corner `i`. In addition, `wedge.index()` returns this
 *   local `uint` position.
 * - **Global Vertex Index (`IndexType`)**:
 *   Each wedge corner corresponds to a topological vertex of the face. Calling
 *   `wedge.vertexIndex()` delegates to `f.vertexIndex(i)` and returns the
 *   global index (`IndexType`) of the corresponding vertex in the mesh's
 *   `VertexContainer`.
 *
 * Access to individual wedges is provided via Face::wedge(uint i) (returning
 * WedgeProxy) or Face::wedges() (returning WedgeContainerProxy).
 *
 * @tparam Comp: The base component type (e.g. Color, TexCoordf, Normal3f) or
 * raw value type.
 * @tparam N: The number of wedges per element (default 3; negative for
 * dynamic/polygonal faces).
 * @tparam OPT: True if the component is optional (defaults to Comp::IS_OPTIONAL
 * if Comp is a Component).
 *
 * @ingroup components
 */
template<typename Comp, int N = 3, bool OPT = IsCompOptional<Comp>::value>
requires WedgeableComponent<Comp>
class Wedges :
        public ContainerComponent<
            WedgeTraits<Comp>::COMP_ID,
            typename WedgeTraits<Comp>::ValueType,
            N,
            (N < 0),
            OPT>
{
    using Base = ContainerComponent<
        WedgeTraits<Comp>::COMP_ID,
        typename WedgeTraits<Comp>::ValueType,
        N,
        (N < 0),
        OPT>;

public:
    /* Constructors */
    Wedges() = default;

    // wedge accessors (wedge() and wedges()) are provided by Face proxy
    // they cannot be provided here because Face may inherit from multiple
    // Wedges components (e.g. Color, TexCoord, Normal).

private:
    template<typename>
    friend class WedgeProxy;

    template<typename>
    friend class WedgeContainerProxy;

    /**
     * @brief Internal static accessor for friend proxies to retrieve the vector
     * of wedges.
     */
    static decltype(auto) wedgeData(auto&& self)
    {
        return Base::compData(self);
    }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Member function implementations */

template<typename Comp, int N, bool OPT>
requires WedgeableComponent<Comp>
template<typename OtherElement>
void Wedges<Comp, N, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool)
{
    static constexpr CompId COMP_ID = Base::COMPONENT_ID;
    using ValueType               = typename Base::ValueType;

    if constexpr (HasComponent<COMP_ID, OtherElement>) {
        if (other.template isComponentAvailable<COMP_ID>()) {
            auto&& selfContainer = Base::container(self);
            if constexpr (COMP_ID == CompId::WEDGE_COLORS) {
                const uint n = std::min(
                    (uint) selfContainer.size(),
                    (uint) other.wedges().colors().size());
                for (uint i = 0; i < n; ++i) {
                    selfContainer[i] = other.wedges().colors()[i];
                }
            }
            else if constexpr (COMP_ID == CompId::WEDGE_TEX_COORDS) {
                const uint n = std::min(
                    (uint) selfContainer.size(),
                    (uint) other.wedges().texCoords().size());
                for (uint i = 0; i < n; ++i) {
                    using S = typename ValueType::ScalarType;
                    selfContainer[i] =
                        other.wedges().texCoords()[i].template cast<S>();
                }
            }
            else if constexpr (COMP_ID == CompId::WEDGE_NORMALS) {
                const uint n = std::min(
                    (uint) selfContainer.size(),
                    (uint) other.wedges().normals().size());
                for (uint i = 0; i < n; ++i) {
                    using S = typename ValueType::ScalarType;
                    selfContainer[i] =
                        other.wedges().normals()[i].template cast<S>();
                }
            }
        }
    }
}

/* Component Aliases */
template<int N = 3, bool OPT = false>
using WedgeColors = Wedges<Color<false>, N, OPT>;

template<typename Scalar = float, int N = 3, bool OPT = false>
using WedgeTexCoords = Wedges<TexCoord<Scalar, false>, N, OPT>;

template<typename Point = Point3f, int N = 3, bool OPT = false>
using WedgesNormals = Wedges<Normal<Point, false>, N, OPT>;

template<typename Point = Point3f, int N = 3, bool OPT = false>
using WedgeNormals = WedgesNormals<Point, N, OPT>;

// Triangle wedge aliases (N = 3)
using TriangleWedgeColors          = WedgeColors<3, false>;
using OptionalTriangleWedgeColors  = WedgeColors<3, true>;

template<typename Scalar = float, bool OPT = false>
using TriangleWedgeTexCoords = WedgeTexCoords<Scalar, 3, OPT>;
template<typename Scalar = float>
using OptionalTriangleWedgeTexCoords = TriangleWedgeTexCoords<Scalar, true>;

using TriangleWedgeTexCoordsf          = WedgeTexCoords<float, 3, false>;
using OptionalTriangleWedgeTexCoordsf  = WedgeTexCoords<float, 3, true>;
using TriangleWedgeTexCoordsd          = WedgeTexCoords<double, 3, false>;
using OptionalTriangleWedgeTexCoordsd  = WedgeTexCoords<double, 3, true>;

using TriangleWedgeNormals3f          = WedgeNormals<Point3f, 3, false>;
using OptionalTriangleWedgeNormals3f  = WedgeNormals<Point3f, 3, true>;
using TriangleWedgeNormals3d          = WedgeNormals<Point3d, 3, false>;
using OptionalTriangleWedgeNormals3d  = WedgeNormals<Point3d, 3, true>;

// Polygon wedge aliases (N = -1)
using PolygonWedgeColors          = WedgeColors<-1, false>;
using OptionalPolygonWedgeColors  = WedgeColors<-1, true>;

template<typename Scalar = float, bool OPT = false>
using PolygonWedgeTexCoords = WedgeTexCoords<Scalar, -1, OPT>;
template<typename Scalar = float>
using OptionalPolygonWedgeTexCoords = PolygonWedgeTexCoords<Scalar, true>;

using PolygonWedgeTexCoordsf         = WedgeTexCoords<float, -1, false>;
using OptionalPolygonWedgeTexCoordsf = WedgeTexCoords<float, -1, true>;
using PolygonWedgeTexCoordsd         = WedgeTexCoords<double, -1, false>;
using OptionalPolygonWedgeTexCoordsd = WedgeTexCoords<double, -1, true>;

using PolygonWedgeNormals3f         = WedgeNormals<Point3f, -1, false>;
using OptionalPolygonWedgeNormals3f = WedgeNormals<Point3f, -1, true>;
using PolygonWedgeNormals3d         = WedgeNormals<Point3d, -1, false>;
using OptionalPolygonWedgeNormals3d = WedgeNormals<Point3d, -1, true>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGES_H
