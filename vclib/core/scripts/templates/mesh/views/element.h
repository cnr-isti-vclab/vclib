%HEADER%

#ifndef VCL_VIEWS_MESH_ELEMENTS_%EL_U%_H
#define VCL_VIEWS_MESH_ELEMENTS_%EL_U%_H

#include <vclib/mesh/requirements/%EL%_requirements.h>

namespace vcl {

/**
 * @brief The %EL_UC%RangeConcept evaluates to true if Rng is a valid Range
 * on %EL_UC%s.
 *
 * This means that Rng must be a Range of %EL_UC%Concept: the iterated type
 * must satisfy the %EL_UC%Concept.
 *
 * @ingroup %EL%_concepts
 */
template<typename Rng>
    concept %EL_UC%RangeConcept =
    Range<Rng> &&
    %EL_UC%Concept<typename std::ranges::iterator_t<Rng>::value_type>;

/**
 * @brief The %EL_UC%PointerRangeConcept evaluates to true if Rng is a valid
 * Range on %EL_UC% Pointers.
 *
 * This means that Rng must be a Range of pointers to a type that satisfy the
 * %EL_UC%Concept.
 *
 * @ingroup %EL%_concepts
 */
template<typename Rng>
    concept %EL_UC%PointerRangeConcept =
    Range<Rng> &&
    IsPointer<typename std::ranges::iterator_t<Rng>::value_type> &&
    %EL_UC%Concept<typename std::decay_t<
                  RemovePtr<
                      typename std::ranges::iterator_t<Rng>::value_type>>>;

namespace views {
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

} // namespace views
} // namespace vcl

#endif // VCL_VIEWS_MESH_ELEMENTS_%EL_U%_H
