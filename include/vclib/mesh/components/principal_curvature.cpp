/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "principal_curvature.h"

namespace vcl::comp {

/**
 * @private
 * @brief Returns `true` if the component is available, `false` otherwise.
 *
 * This member function can return `false` only if the component is optional,
 * and it is not enabled.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is available, `false` otherwise.
 */
template<typename Scalar, typename El, bool O>
bool PrincipalCurvature<Scalar, El, O>::isAvailable() const
{
	return Base::isAvailable(this);
}

/**
 * @brief Returns a const reference of the principal curvature of the element.
 * @return a const reference of the principal curvature of the element.
 */
template<typename Scalar, typename El, bool O>
auto PrincipalCurvature<Scalar, El, O>::principalCurvature() const
	-> const PrincipalCurvatureType&
{
	return Base::data(this);
}

/**
 * @brief Returns a reference of the principal curvature of the element.
 * @return a reference of the principal curvature of the element.
 */
template<typename Scalar, typename El, bool O>
auto PrincipalCurvature<Scalar, El, O>::principalCurvature()
	-> PrincipalCurvatureType&
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool O>
template<typename Element>
void PrincipalCurvature<Scalar, El, O>::importFrom(const Element& e)
{
	if constexpr (HasPrincipalCurvature<Element>) {
		if (isPrincipalCurvatureAvailableOn(e)) {
			principalCurvature() =
				e.principalCurvature().template cast<Scalar>();
		}
	}
}

/**
 * @brief Checks if the given Element has PrincipalCurvature component
 * available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that
 * satisfies the ElementConcept.
 * @return `true` if the element has PrincipalCurvature component available,
 * `false` otherwise.
 */
bool isPrincipalCurvatureAvailableOn(const ElementConcept auto& element)
{
	return isComponentAvailableOn<PRINCIPAL_CURVATURE>(element);
}

} // namespace vcl::comp
