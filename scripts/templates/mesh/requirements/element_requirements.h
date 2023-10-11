%HEADER%

#ifndef VCL_MESH_REQUIREMENTS_%EL_U%_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_%EL_U%_REQUIREMENTS_H

#include <vclib/concepts/mesh.h>
#include <vclib/exceptions/mesh_exceptions.h>

#include "element_requirements.h"
#include "../containers/%EL%_container.h"

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

/**
 * @brief Returns `true` if the given mesh has its %EL_UC%Container compact.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy
 * the %EL_UC%MeshConcept.
 *
 * @param m: the mesh on which check the compactness of the 
 * %EL_UC%Container.
 * @return `true` if the %EL_UC%Container is compact.
 *
 * @ingroup %EL%_requirements
 */
template<%EL_UC%MeshConcept MeshType>
bool is%EL_UC%ContainerCompact(const MeshType& m)
{
	return isElementContainerCompact<%EL_U%>(m);
}

%IS_ENABLE_COMPONENTS%
/*********************
 * require functions *
 *********************/

// %EL_UC% Compactness

/**
 * @brief This function asserts that a Mesh has the %EL_UC%Container 
 * compact (no elements flagged as deleted).
 *
 * If the %EL_UC%Container is not compact, a
 * vcl::MissingCompactnessException will be thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * %EL_UC%MeshConcept.
 *
 * @throws vcl::MissingCompactnessException if the %EL_UC%Container
 * is not compact.
 *
 * @param[in] m: the mesh on which check the compactness of the
 * %EL_UC%Container.
 *
 * @ingroup %EL%_requirements
 */
template<%EL_UC%MeshConcept MeshType>
void require%EL_UC%ContainerCompactness(const MeshType& m)
{
	requireElementContainerCompactness<%EL_U%>(m);
}

%REQUIRE_COMPONENTS%
} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_%EL_U%_REQUIREMENTS_H
