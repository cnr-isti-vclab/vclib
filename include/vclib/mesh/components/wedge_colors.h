/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_WEDGE_COLORS_H

#include <vclib/concepts/mesh/components/color.h>
#include <vclib/concepts/mesh/components/wedge_colors.h>
#include <vclib/space/vector.h>
#include <vclib/views/view.h>

#include "bases/container_component.h"

namespace vcl::comp {

/**
 * @brief The WedgeColors class is a container of colors associated to the
 * wedges of a Face element.
 *
 * It is a static or dynamic size container of colors, depending on the value of
 * the template argument N (a negative value indicates a dynamic size).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component, altough it is usually used (and it
 * makes sense only) on the Face element.
 *
 * For example, if you have a Face Element `f` that has the WedgeColors
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * vcl::Color c = f.wedgeColor(0);
 * @endcode
 *
 * @note This component is *Tied To Vertex Number*: it means that the size of
 * the container, if dynamic, will change automatically along the Vertex Number
 * of the Component. For further details check the documentation of the @ref
 * ContainerComponent class.
 *
 * @tparam N: The size of the container, that will represent the number of
 * storable wedge colors. If N is negative, the container will be dynamic.
 * In any case, N must be the same of the Vertex Number of the Element that
 * will contain this component.
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<int N, typename ParentElemType = void, bool OPT = false>
class WedgeColors :
        public ContainerComponent<
            WedgeColors<N, ParentElemType, OPT>,
            CompId::WEDGE_COLORS,
            vcl::Color,
            N,
            void,
            ParentElemType,
            OPT,
            true>
{
    using Base = ContainerComponent<
        WedgeColors<N, ParentElemType, OPT>,
        CompId::WEDGE_COLORS,
        vcl::Color,
        N,
        void,
        ParentElemType,
        OPT,
        true>;

public:
    static const int WEDGE_COLOR_NUMBER = Base::SIZE;

    /* Iterator Types declaration */

    using WedgeColorsIterator      = Base::Iterator;
    using ConstWedgeColorsIterator = Base::ConstIterator;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the Wedge Colors container size is static, initializes all the
     * Wedge Colors to with the vcl::Color empty constructor, otherwise the
     * container will be empty.
     */
    WedgeColors() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the i-th wedge color of the element.
     *
     * You can use this function to set the i-th color of the element:
     *
     * @code{.cpp}
     * f.wedgeColor(0) = vcl::Color::Red;
     * @endcode
     *
     * @param[in] i: the index of the wedge color to return. The value must be
     * between 0 and the number of vertices of the element.
     * @return A reference to the i-th wedge color of the element.
     */
    vcl::Color& wedgeColor(uint i) { return colors().at(i); }

    /**
     * @brief Returns a const reference to the i-th wedge color of the element.
     *
     * @param[in] i: the index of the wedge color to return. The value must be
     * between 0 and the number of vertices of the element.
     * @return A const reference to the i-th wedge color of the element.
     */
    const vcl::Color& wedgeColor(uint i) const { return colors().at(i); }

    /**
     * @brief Returns a reference to the i-th wedge color of the element but
     * using as index the module between i and the number of vertices of the
     * element. You can use this function if you need to get the "next wedge
     * color after position k", without check if it is less than the number of
     * vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * f.wedgeColorMod(-1) = vcl::Color::Red; // the wedge color in position
     *                                        // vertexNumber() - 1
     * @endcode
     *
     * @param[in] i: the position of the required wedge color in the container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return A reference to the required wedge color of the element.
     */
    vcl::Color& wedgeColorMod(int i) { return colors().atMod(i); }

    /**
     * @brief Same of wedgeColorMod(int) but returns a const reference.
     * @param[in] i: the position of the required wedge color in the container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return A const reference to the required wedge color of the element.
     */
    const vcl::Color& wedgeColorMod(int i) const { return colors().atMod(i); }

    /**
     * @brief Sets the i-th wedge color of the element.
     * @param[in] i: the position in the container on which set the wedge color;
     * the value must be between 0 and the number of vertices of the element.
     * @param[in] c: the new wedge color.
     */
    void setWedgeColor(uint i, const vcl::Color& c) { colors().set(i, c); }

    /**
     * @brief Sets all the wedge colors of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of wedge colors to set. The value type
     * of the range must be convertible to a vcl::Color.
     *
     * @param[in] r: range of colors to set.
     */
    template<Range Rng>
    void setWedgeColors(Rng&& r) requires RangeOfConvertibleTo<Rng, vcl::Color>
    {
        colors().set(r);
    }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first wedge color in the container of
     * this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    WedgeColorsIterator wedgeColorBegin() { return colors().begin(); }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of this container.
     */
    WedgeColorsIterator wedgeColorEnd() { return colors().end(); }

    /**
     * @brief Returns a const iterator to the first wedge color in the container
     * of this component.
     *
     * @return a const iterator pointing to the begin of this container.
     */
    ConstWedgeColorsIterator wedgeColorBegin() const
    {
        return colors().begin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstWedgeColorsIterator wedgeColorEnd() const { return colors().end(); }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of wedge colors of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto& wc : el.wedgeColors()) {
     *     // Do something with wc
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over wedge colors.
     */
    View<WedgeColorsIterator> wedgeColors()
    {
        return View(wedgeColorBegin(), wedgeColorEnd());
    }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the container of wedge colors of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (const auto& wc : el.wedgeColors()) {
     *     // Do something read-only with wc
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over wedge colors.
     */
    View<ConstWedgeColorsIterator> wedgeColors() const
    {
        return View(wedgeColorBegin(), wedgeColorEnd());
    }

    // dummy member to discriminate between WedgeColors and FaceHalfEdgePointers
    void __wedgeColors() const {}

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e)
    {
        if constexpr (HasWedgeColors<Element>) {
            if (isWedgeColorsAvailableOn(e)) {
                if constexpr (N > 0) {
                    // same static size
                    if constexpr (N == Element::WEDGE_COLOR_NUMBER) {
                        importWedgeColorsFrom(e);
                    }
                    // from dynamic to static, but dynamic size == static size
                    else if constexpr (Element::WEDGE_COLOR_NUMBER < 0) {
                        if (e.vertexNumber() == N) {
                            importWedgeColorsFrom(e);
                        }
                    }
                    else {
                        // do not import in this case: cannot import from
                        // dynamic size != static size
                    }
                }
                else {
                    // from static/dynamic to dynamic size: need to resize
                    // first, then import
                    resize(e.vertexNumber());
                    importWedgeColorsFrom(e);
                }
            }
        }
    }

    // ContainerComponent interface functions
    void resize(uint n) requires (N < 0) { colors().resize(n); }

    void pushBack(const vcl::Color& c = vcl::Color()) requires (N < 0)
    {
        colors().pushBack(c);
    }

    void insert(uint i, const vcl::Color& c = vcl::Color()) requires (N < 0)
    {
        colors().insert(i, c);
    }

    void erase(uint i) requires (N < 0) { colors().erase(i); }

    void clear() requires (N < 0) { colors().clear(); }

private:
    template<typename Element>
    void importWedgeColorsFrom(const Element& e)
    {
        for (uint i = 0; i < e.vertexNumber(); ++i) {
            wedgeColor(i) = e.wedgeColor(i);
        }
    }

    Vector<vcl::Color, N>& colors() { return Base::container(); }

    const Vector<vcl::Color, N>& colors() const { return Base::container(); }
};

/* Detector function to check if a class has WedgeColors available */

/**
 * @brief Checks if the given Element has WedgeColors available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that satisfies
 * the ElementConcept.
 * @return `true` if the element has WedgeColors available, `false` otherwise.
 */
bool isWedgeColorsAvailableOn(const ElementConcept auto& element)
{
    return isComponentAvailableOn<CompId::WEDGE_COLORS>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGE_COLORS_H
