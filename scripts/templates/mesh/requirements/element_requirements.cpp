%HEADER%

#include "%EL%_requirements.h"

#include <vclib/exceptions/mesh_exceptions.h>

#include "../containers/%EL%_container.h"

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

/**
 * @brief Returns `true` if the given mesh has its %EL_UC%Container compact.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the %EL_UC%MeshConcept.
 *
 * @param m: the mesh on which check the compactness of the %EL_UC%Container.
 * @return `true` if the %EL_UC%Container is compact.
 *
 * @ingroup %EL%_requirements
 */
template<%EL_UC%MeshConcept MeshType>
bool is%EL_UC%ContainerCompact(const MeshType& m)
{
	return isElementContainerCompact<%EL_U%>(m);
}

%IS_ENABLE_COMPONENTS_SOURCE%
/*********************
 * require functions *
 *********************/

/**
 * @brief This function asserts that a Mesh has the %EL_UC%Container compact
 * (no elements flagged as deleted).
 *
 * If the %EL_UC%Container is not compact, a vcl::MissingCompactnessException will be thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the %EL_UC%MeshConcept.
 *
 * @throws vcl::MissingCompactnessException if the %EL_UC%Container is not compact.
 *
 * @param[in] m: the mesh on which check the compactness of the %EL_UC%Container.
 *
 * @ingroup %EL%_requirements
 */
template<%EL_UC%MeshConcept MeshType>
void require%EL_UC%ContainerCompactness(const MeshType& m)
{
	requireElementContainerCompactness<%EL_U%>(m);
}

%REQUIRE_COMPONENTS_SOURCE%
} // namespace vcl
