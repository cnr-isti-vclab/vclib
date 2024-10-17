% HEADER %

#ifndef VCL_CONCEPTS_MESH_ELEMENTS_ % EL_U % _H
#define VCL_CONCEPTS_MESH_ELEMENTS_ % EL_U % _H

#include <vclib/concepts/mesh/components.h>

#include "element.h"

    /**
     * @defgroup %EL%_concepts %EL_UC% Concepts
     * @ingroup element_concepts
     *
     * @brief List of concepts for types related to the %EL_UC% Element.
     */

    namespace vcl
{
    namespace % EL_NS %
    {

        /**
         * @ingroup %EL%_concepts %EL%_components
         * @{
         */

        /* Port concepts into the %EL_NS% namespace */
        % COMPONENTS_CONCEPTS %

        /** @} */ // end of group

    } // namespace %EL_NS %

    /**
     * @brief The %EL_UC%Concept describes how a %EL_UC% element that can be
     * used for a %EL_UC%Container should be organized.
     *
     * The %EL_UC% concept is satisfied for a class E if ALL the following
     * sentences are true:
     * - The class E is vcl::%EL_UC%, or derives from it;
     * - The class E has the BitFlags component (or an equivalent one);
     *
     * @ingroup %EL%_concepts
     */
    template<typename T>
        concept % EL_UC % Concept =
        ElementConcept<T> &&
        T::ELEMENT_TYPE == ElemId:: % EL_U % && % EL_NS %: : HasBitFlags<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_ELEMENTS_%EL_U%_H
