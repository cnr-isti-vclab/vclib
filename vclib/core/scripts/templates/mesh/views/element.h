%HEADER%

#ifndef VCL_VIEWS_MESH_ELEMENTS_%EL_U%_H
#define VCL_VIEWS_MESH_ELEMENTS_%EL_U%_H

#include <vclib/mesh/concepts/per_%EL%.h>

namespace vcl::views {
namespace detail {

struct %EL_UC%sView
{
    constexpr %EL_UC%sView() = default;

    template <%EL_UC%MeshConcept R>
    friend constexpr auto operator|(R&& r, %EL_UC%sView)
    {
        return r.%EL_C%s();
    }
};

} // namespace detail

/**
 * @brief A view that allows to iterate overt the %EL_UC% elements of an
 * object.
 *
 * This view can be applied to objects having type that satisfies the
 * %EL_UC%MeshConcept.
 *
 * @ingroup views
 */
inline constexpr detail::%EL_UC%sView %EL_C%s;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_ELEMENTS_%EL_U%_H
