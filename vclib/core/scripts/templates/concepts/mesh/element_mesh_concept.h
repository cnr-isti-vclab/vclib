%HEADER%

#ifndef VCL_CONCEPTS_MESH_%EL_U%_MESH_CONCEPT_H
#define VCL_CONCEPTS_MESH_%EL_U%_MESH_CONCEPT_H

#include "mesh_concept.h"
#include "per_%EL%.h"

namespace vcl {

/**
 * @brief The %EL_UC%MeshConcept is evaluated true if the type T is a Mesh (it
 * satisfies the @ref vcl::MeshConcept) and has an %EL_UC%Container.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept %EL_UC%MeshConcept =
    MeshConcept<T> && mesh::Has%EL_UC%Container<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_%EL_U%_MESH_CONCEPT_H
