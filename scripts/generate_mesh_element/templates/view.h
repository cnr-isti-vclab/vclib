%HEADER%

#ifndef VCL_MESH_VIEWS_ELEMENTS_%EL_U%_H
#define VCL_MESH_VIEWS_ELEMENTS_%EL_U%_H

#include "../element.h"

#include <vclib/mesh/elements/%EL%.h>

#include <ranges>

namespace vcl {

/**
 * @brief Concept that checks whether Rng is a valid range of %EL_TXT_PL%.
 *
 * @ingroup element_concepts
 */
template<typename Rng>
concept %EL_UC%RangeConcept =
    std::ranges::range<Rng> &&
    %EL_UC%Concept<typename std::ranges::iterator_t<Rng>::value_type>;

namespace views {

namespace detail {

struct %EL_UC_PL%View : std::ranges::range_adaptor_closure<%EL_UC_PL%View>
{
    constexpr %EL_UC_PL%View() = default;

    template<typename R>
    requires requires (R&& r) { { r.%EL_C_PL%() } -> std::ranges::range; }
    constexpr auto operator()(R&& r) const
    {
        return r.%EL_C_PL%();
    }

    template<typename R>
    requires (!std::ranges::range<R>) &&
             requires (R&& r) { { r.%EL_C_PL%() } -> std::ranges::range; }
    friend constexpr auto operator|(R&& r, %EL_UC_PL%View)
    {
        return r.%EL_C_PL%();
    }
};

} // namespace detail

/**
 * @brief A view adaptor that allows to iterate over the %EL_UC% elements of an object.
 *
 * Can be applied to Meshes: `mesh | views::%EL_C_PL%`
 *
 * Supports both pipe syntax (`mesh | views::%EL_C_PL%`) and function call syntax
 * (`views::%EL_C_PL%(mesh)`).
 *
 * @ingroup views
 */
inline constexpr detail::%EL_UC_PL%View %EL_C_PL%;

template<>
inline constexpr auto elements<ElemId::%EL_U%> = %EL_C_PL%;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_%EL_U%_H
