%HEADER%

#ifndef VCLIB_CONCEPTS_RANGES_MESH_%EL_U%_RANGE_H
#define VCLIB_CONCEPTS_RANGES_MESH_%EL_U%_RANGE_H

#include <vclib/concepts/pointers.h>
#include <vclib/concepts/ranges/range.h>
#include <vclib/concepts/mesh/elements/%EL%.h>

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
		std::remove_pointer_t<
			typename std::ranges::iterator_t<Rng>::value_type>>>;

} // namespace vcl

#endif // VCLIB_CONCEPTS_RANGES_MESH_%EL_U%_RANGE_H
