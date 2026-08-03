// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_PROVIDERS_MESH_PROVIDER_H
#define VCL_MESH_PROVIDERS_MESH_PROVIDER_H

#include "mesh_provider_mixin.h"

#include <vclib/mesh/requirements/mesh_requirements.h>

namespace vcl {

/**
 * @brief A standalone MeshProvider that acts as a reference to an existing
 * Mesh.
 *
 * This class implements the AbstractMeshProvider interface without
 * requiring the mesh to inherit from the CRTP Mixin. It acts as an adapter,
 * delegating calls to the referenced mesh. Since it only holds a pointer to the
 * mesh, it must not outlive the mesh it references.
 *
 * Example of usage:
 * @code{.cpp}
 * vcl::TriMesh m;
 * // ... fill mesh ...
 * vcl::MeshProviderReference<vcl::TriMesh> provider(m);
 * vcl::Point3d p = provider.vertexPosition(0);
 * @endcode
 *
 * @tparam MeshType: The type of the mesh that provides the components.
 */
template<MeshConcept MeshType>
class MeshProviderReference :
        public MeshProviderMixin<
            MeshProviderReference<MeshType>,
            MeshType>
{
private:
    const MeshType* mMesh;

public:
    /**
     * @brief Constructor that takes a reference to an existing mesh.
     * @param mesh The mesh to reference.
     */
    MeshProviderReference(const MeshType& mesh) : mMesh(&mesh) {}

    /**
     * @brief Returns a reference to the referenced mesh.
     * Required by MeshProviderMixin to access mesh elements.
     */
    const MeshType& mesh() const { return *mMesh; }
};

/**
 * @brief A Mesh type that also acts as a MeshProvider.
 *
 * This class inherits from a base MeshType and from MeshProviderMixin.
 * It allows you to instantiate a mesh that inherently provides the
 * AbstractMeshProvider interface via CRTP, without needing external
 * wrappers.
 *
 * Example of usage:
 * @code{.cpp}
 * vcl::MeshProvider<vcl::TriMesh> m;
 * // ... fill mesh ...
 * const vcl::AbstractMeshProvider* provider = &m;
 * vcl::Point3d p = provider->vertexPosition(0);
 * @endcode
 *
 * @tparam MeshType: The base Mesh type to inherit from.
 */
template<MeshConcept MeshType>
class MeshProvider :
        public MeshType,
        public MeshProviderMixin<MeshProvider<MeshType>, MeshType>
{
};

} // namespace vcl

#endif // VCL_MESH_PROVIDERS_MESH_PROVIDER_H
