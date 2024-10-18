%HEADER%

#ifndef VCL_MESH_ELEMENTS_%EL_U%_COMPONENTS_H
#define VCL_MESH_ELEMENTS_%EL_U%_COMPONENTS_H

#include "../components/parent_mesh_pointer.h"

%INCLUDE_COMPONENTS%

/**
 * @defgroup %EL%_components %EL_UC% Components
 * @ingroup components
 *
 * @brief This group lists all the Components that may be used by
 * the %EL_UC% element.
 *
 * All these components are listed inside the vcl::%EL_NS% namespace.
 */

namespace vcl::%EL_NS% {

/// TODO: Check this file and update the components according to the element

/**
 * @addtogroup %EL%_components
 * @{
 */

%ELEMENT_COMPONENTS%

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/** @} */ // end of %EL%_components group

} // namespace vcl::%EL_NS%

#endif // VCL_MESH_ELEMENTS_%EL_U%_COMPONENTS_H
