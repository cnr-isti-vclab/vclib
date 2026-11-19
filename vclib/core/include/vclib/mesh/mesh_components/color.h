// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_MESH_COMPONENTS_COLOR_H
#define VCL_MESH_MESH_COMPONENTS_COLOR_H

#include <vclib/mesh/components/color.h>

#include <istream>
#include <ostream>

namespace vcl::mesh {

/**
 * @brief The Color class represents a RGBA color that is part of a Mesh.
 *
 * Allows getting and setting the mesh-level color (e.g. default/diffuse color
 * for the entire mesh).
 *
 * For example, if you have a Mesh `m` with the Color component, you can access
 * and modify its color:
 *
 * @code{.cpp}
 * m.color() = vcl::Color::Red;
 * auto c = m.color();
 * @endcode
 *
 * @ingroup mesh_components
 */
class Color : public comp::Component<CompId::COLOR, vcl::Color>
{
    vcl::Color mColor;
public:
    /**
     * @brief Expose the type of the Color.
     */
    using ColorType = vcl::Color;

    /**
     * @brief Default constructor. Initializes the color using Color's default
     * (black with alpha 255).
     */
    Color() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the color of the mesh.
     * @return A reference to the color of the mesh.
     */
    decltype(auto) color(this auto&& self) { return (self.mColor); }

protected:
    /**
     * @brief Serializes the mesh color to the given output stream.
     * @param[out] os: The output stream.
     */
    void serialize(std::ostream& os) const { vcl::serialize(os, mColor); }

    /**
     * @brief Deserializes the mesh color from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is) { vcl::deserialize(is, mColor); }

    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        if constexpr (comp::HasColor<OtherMesh>) {
            mColor = m.color();
        }
    }
};

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the Color
 * component (COMP_ID = CompId::COLOR).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasColor = comp::HasColor<T>;


} // namespace vcl::mesh

#endif // VCL_MESH_MESH_COMPONENTS_COLOR_H
