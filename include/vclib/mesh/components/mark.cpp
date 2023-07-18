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

#include "mark.h"

namespace vcl::comp {

/**
 * @private
 * @brief Constructor that initializes the mark to 0.
 */
template<typename El, bool O>
Mark<El, O>::Mark()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

/**
 * @private
 * @brief Initializes the mark to 0.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename El, bool O>
void Mark<El, O>::init()
{
	mark() = 0;
}

/**
 * @private
 * @brief Returns `true` if the component is enabled, `false` otherwise.
 * This member function can return `false` only if the component is optional.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is enabled, `false` otherwise.
 */
template<typename El, bool O>
bool Mark<El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns the value of the mark.
 * @return the value of the mark.
 */
template<typename El, bool O>
int Mark<El, O>::mark() const
{
	return Base::data(this);
}

/**
 * @brief Resets the mark to 0.
 */
template<typename El, bool O>
void Mark<El, O>::resetMark()
{
	mark() = 0;
}

/**
 * @brief Checks if the current element/mesh has the same mark of the given
 * input element/mesh `e`.
 *
 * @tparam E: the type of the input element/mesh.
 * @param e: the input element/mesh.
 */
template<typename El, bool O>
template<typename E>
bool Mark<El, O>::hasSameMark(const E& e) const
{
	if constexpr (std::is_pointer<E>::value) {
		return e->mark() == mark();
	}
	else {
		return e.mark() == mark();
	}
}

/**
 * @brief Increments the mark of the current element/mesh by 1.
 */
template<typename El, bool O>
void Mark<El, O>::incrementMark()
{
	mark()++;
}

/**
 * @brief Decrements the mark of the current element/mesh by 1.
 */
template<typename El, bool O>
void Mark<El, O>::decrementMark()
{
	mark()--;
}

template<typename El, bool O>
template<typename Element>
void Mark<El, O>::importFrom(const Element& e)
{
	if constexpr (HasMark<Element>) {
		if (isMarkEnabledOn(e)) {
			mark() = e.mark();
		}
	}
}

template<typename El, bool O>
int& Mark<El, O>::mark()
{
	return Base::data(this);
}

/**
 * @brief Checks if the given Element/Mesh has the Mark component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element/mesh to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element/mesh has the Mark component available, `false`
 * otherwise.
 */
bool isMarkEnabledOn(const ElementOrMeshConcept auto& element)
{
	return isComponentEnabledOn<MARK>(element);
}

} // namespace vcl::comp
