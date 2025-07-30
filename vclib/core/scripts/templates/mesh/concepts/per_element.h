%HEADER%

#ifndef VCL_MESH_CONCEPTS_PER_%EL_U%_H
#define VCL_MESH_CONCEPTS_PER_%EL_U%_H

#include "mesh_concept.h"

#include <vclib/mesh/containers/%EL%_container.h>

/**
 * @defgroup %EL%_mesh_concepts %EL_UC%Mesh Concepts
 * @ingroup mesh_concepts
 *
 * @brief List of concepts for types related to Meshes having %EL_UC%s.
 * They allow to check the presence of specific components for %EL_UC%s.
 */

namespace vcl {

/**
 * @brief The %EL_UC%MeshConcept is evaluated true if the type T is a Mesh (it
 * satisfies the @ref vcl::MeshConcept) and has a %EL_UC%Container.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept %EL_UC%MeshConcept = MeshConcept<T> && mesh::Has%EL_UC%Container<T>;

%PER_ELEM_CONCEPTS%

} // namespace vcl

#endif // VCL_MESH_CONCEPTS_PER_%EL_U%_H
