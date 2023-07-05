%HEADER%

#ifndef VCL_MESH_REQUIREMENTS_%EL_U%_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_%EL_U%_REQUIREMENTS_H

#include <vclib/concepts/mesh.h>

#include "element_requirements.h"

/**
 * @defgroup %EL%_requirements %EL_UC% Requirements
 * @ingroup mesh_requirements
 *
 * @brief List functions that allows to check, enable and assert the
 * availability of components in the %EL_UC% Elements of a Mesh.
 */

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

// %EL_UC% Compactness

template<%EL_UC%MeshConcept MeshType>
bool is%EL_UC%ContainerCompact(const MeshType&);

%IS_ENABLE_COMPONENTS_HEADER%
/*********************
 * require functions *
 *********************/

// %EL_UC% Compactness

template<%EL_UC%MeshConcept MeshType>
void require%EL_UC%ContainerCompactness(const MeshType& m);

%REQUIRE_COMPONENTS_HEADER%
} // namespace vcl

#include "%EL%_requirements.cpp"

#endif // VCL_MESH_REQUIREMENTS_%EL_U%_REQUIREMENTS_H
