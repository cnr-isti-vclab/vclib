// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_MESH_COMPONENTS_NAME_H
#define VCL_MESH_MESH_COMPONENTS_NAME_H

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/base/component_predicates.h>

#include <vclib/base.h>

#include <istream>
#include <ostream>
#include <string>

namespace vcl::mesh {

/**
 * @brief The Name component class represents a simple name stored as string.
 *
 * The member functions of this class will be available in the instance of any
 * Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the Name component, you'll be able
 * to access to this component member functions from `m`:
 *
 * @code{.cpp}
 * m.name();
 * @endcode
 *
 * @ingroup mesh_components
 */
class Name : public comp::Component<CompId::NAME, std::string>
{
    std::string mName;

public:
    /* Constructors */

    /**
     * @brief Initializes an empty name.
     */
    Name() = default;

    /* Member functions */

    /**
     * @brief Returns the name of this object.
     * @return The name of this object.
     */
    decltype(auto) name(this auto&& self) { return (self.mName); }

protected:
    // Component interface functions
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m);

    void serialize(std::ostream& os) const { vcl::serialize(os, mName); }

    void deserialize(std::istream& is) { vcl::deserialize(is, mName); }
};

/* concept */

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * Name component (COMP_ID = CompId::NAME).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasName = comp::HasComponent<CompId::NAME, T>;

/* importFrom function */

template<typename OtherMesh>
void Name::importFrom(const OtherMesh& m)
{
    if constexpr (HasName<OtherMesh>) {
        mName = m.name();
    }
}

} // namespace vcl::mesh

#endif // VCL_MESH_MESH_COMPONENTS_NAME_H
