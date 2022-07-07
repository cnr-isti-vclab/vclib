#ifndef VCL_MESH_COMPONENTS_OPTIONAL_COMPONENT_H
#define VCL_MESH_COMPONENTS_OPTIONAL_COMPONENT_H

#include "vertical_component.h"

namespace vcl::comp {

template<typename T>
class OptionalComponent : virtual public VerticalComponent<T>
{
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_OPTIONAL_COMPONENT_H
