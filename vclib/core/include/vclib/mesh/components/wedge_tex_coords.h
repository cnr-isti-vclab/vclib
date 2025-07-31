/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include "base/container_component.h"
#include "base/predicates.h"

#include <vclib/space/core.h>
#include <vclib/types.h>

namespace vcl::comp {

/**
 * @brief The WedgeTexCoords class is a container of texture coordinates
 * associated to the wedges of a Face element.
 *
 * The component is composed of a static or dynamic size container, depending on
 * the value of the template argument N (a negative value indicates a dynamic
 * size), plus a texture index that represents the index of the texture used
 * by all the texture coordinates stored in the container.
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component, altough it is usually used (and it
 * makes sense only) on the Face element.
 *
 * For example, if you have a Face Element `f` that has the WedgeTexCoords
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * auto t = f.wedgeTexCoord(0);
 * ushort tid = f.textureIndex();
 * @endcode
 *
 * @note This component is *Tied To Vertex Number*: it means that the size of
 * the container, if dynamic, will change automatically along the Vertex Number
 * of the Component. For further details check the documentation of the @ref
 * ContainerComponent class.
 *
 * @tparam Scalar: The Scalar type used for the texture coordinates.
 * @tparam N: The size of the container, that will represent the number of
 * storable wedge texcoords. If N is negative, the container will be dynamic.
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
template<
    typename Scalar,
    int N,
    typename ParentElemType = void,
    bool OPT                = false>
class WedgeTexCoords :
        public ContainerComponent<
            WedgeTexCoords<Scalar, N, ParentElemType, OPT>,
            CompId::WEDGE_TEX_COORDS,
            vcl::TexCoord<Scalar>,
            N,
            ushort,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT,
            true>
{
    using Base = ContainerComponent<
        WedgeTexCoords<Scalar, N, ParentElemType, OPT>,
        CompId::WEDGE_TEX_COORDS,
        vcl::TexCoord<Scalar>,
        N,
        ushort,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT,
        true>;

public:
    /**
     * @brief Expose the type of the Texture Coordinate.
     */
    using WedgeTexCoordType = vcl::TexCoord<Scalar>;

    /* Iterator Types declaration */

    using WedgeTexCoordsIterator = Vector<vcl::TexCoord<Scalar>, N>::Iterator;
    using ConstWedgeTexCoordsIterator =
        Vector<vcl::TexCoord<Scalar>, N>::ConstIterator;

    static const int WEDGE_TEX_COORD_NUMBER = N;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the Wedge TexCoords container size is static, initializes all the
     * Wedge TexCoords to with the vcl::TexCoord empty constructor, otherwise
     * the container will be empty.
     */
    WedgeTexCoords() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the i-th wedge texcoord of the element.
     *
     * You can use this function to set the i-th texcoord of the element:
     *
     * @code{.cpp}
     * f.wedgeTexCoord(0) = {0.5, 0.5};
     * @endcode
     *
     * @param[in] i: the index of the wedge texcoord to return. The value must
     * be between 0 and the number of vertices of the element.
     * @return A reference to the i-th wedge texcoord of the element.
     */
    vcl::TexCoord<Scalar>& wedgeTexCoord(uint i) { return texCoords().at(i); }

    /**
     * @brief Returns a const reference to the i-th wedge texcoord of the
     * element.
     *
     * @param[in] i: the index of the wedge texcoord to return. The value must
     * be between 0 and the number of vertices of the element.
     * @return A const reference to the i-th wedge texcoord of the element.
     */
    const vcl::TexCoord<Scalar>& wedgeTexCoord(uint i) const
    {
        return texCoords().at(i);
    }

    /**
     * @brief Returns a reference to the i-th wedge texcoord of the element but
     * using as index the module between i and the number of vertices of the
     * element. You can use this function if you need to get the "next wedge
     * texcoord after position k", without check if it is less than the number
     * of vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * f.wedgeTexCoordMod(-1) = {0.1, 0.2}; // the wedge texcoord in position
     *                                      // vertexNumber() - 1
     * @endcode
     *
     * @param[in] i: the position of the required wedge texcoord in the
     * container, w.r.t. the position 0; value is modularized on vertexNumber().
     * @return A reference to the required wedge texcoord of the element.
     */
    vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i)
    {
        return texCoords().atMod(i);
    }

    /**
     * @brief Same of wedgeTexCoordMod(int) but returns a const reference.
     * @param[in] i: the position of the required wedge texcoord in the
     * container, w.r.t. the position 0; value is modularized on vertexNumber().
     * @return A const reference to the required wedge texcoord of the element.
     */
    const vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i) const
    {
        return texCoords().atMod(i);
    }

    /**
     * @brief Sets the i-th wedge texcoord of the element.
     * @param[in] i: the position in the container on which set the wedge
     * texcoord; the value must be between 0 and the number of vertices of the
     * element.
     * @param[in] t: the new wedge texcoord.
     */
    void setWedgeTexCoord(uint i, const vcl::TexCoord<Scalar>& t)
    {
        texCoords().set(i, t);
    }

    /**
     * @brief Sets all the wedge texcoords of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of wedge texcoords to set. The value
     * type of the range must be convertible to a vcl::TexCoord<Scalar>.
     *
     * @param[in] r: range of texcoords to set.
     */
    template<Range Rng>
    void setWedgeTexCoords(Rng&& r)
        requires InputRange<Rng, vcl::TexCoord<Scalar>>
    {
        texCoords().set(r);
    }

    /**
     * @brief Returns a reference to the texture index used to identify the
     * texture on which the wedge texture coordinates are mapped.
     *
     * @return A reference to the texture index.
     */
    ushort& textureIndex() { return Base::additionalData(); }

    /**
     * @brief Returns the texture index used to identify the texture on which
     * the wedge texture coordinates are mapped.
     *
     * @return The texture index.
     */
    ushort textureIndex() const { return Base::additionalData(); }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first wedge texcoord in the container
     * of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    WedgeTexCoordsIterator wedgeTexCoordBegin() { return texCoords().begin(); }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of this container.
     */
    WedgeTexCoordsIterator wedgeTexCoordEnd() { return texCoords().end(); }

    /**
     * @brief Returns a const iterator to the first wedge texcoord in the
     * container of this component.
     *
     * @return a const iterator pointing to the begin of this container.
     */
    ConstWedgeTexCoordsIterator wedgeTexCoordBegin() const
    {
        return texCoords().begin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return a const iterator pointing to the end of this container.
     */
    ConstWedgeTexCoordsIterator wedgeTexCoordEnd() const
    {
        return texCoords().end();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of wedge texcoords of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto& tc : el.wedgeTexCoords()) {
     *     // Do something with tc
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over wedge texcoords.
     */
    View<WedgeTexCoordsIterator> wedgeTexCoords()
    {
        return View(wedgeTexCoordBegin(), wedgeTexCoordEnd());
    }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the container of wedge texcoords of the element. The
     * view object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (const auto& tc : el.wedgeTexCoords()) {
     *     // Do something read-only with tc
     * }
     * @endcode
     *
     * @return a lightweight const view object that can be used in range-based
     * for loops to iterate over wedge texcoords.
     */
    View<ConstWedgeTexCoordsIterator> wedgeTexCoords() const
    {
        return View(wedgeTexCoordBegin(), wedgeTexCoordEnd());
    }

    // dummy member to discriminate between WedgeTexCoords and
    // FaceHalfEdgePointers
    void __wedgeTexCoords() const {}

protected:
    using WedgeTexCoordScalarType = Scalar;

    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const
    {
        texCoords().serialize(os);
        vcl::serialize(os, textureIndex());
    }

    void deserialize(std::istream& is)
    {
        texCoords().deserialize(is);
        vcl::deserialize(is, textureIndex());
    }

    // ContainerComponent interface functions
    void resize(uint n) requires (N < 0) { texCoords().resize(n); }

    void pushBack(const vcl::TexCoord<Scalar>& t = vcl::TexCoord<Scalar>())
        requires (N < 0)
    {
        texCoords().pushBack(t);
    }

    void insert(
        uint                         i,
        const vcl::TexCoord<Scalar>& t = vcl::TexCoord<Scalar>())
        requires (N < 0)
    {
        texCoords().insert(i, t);
    }

    void erase(uint i) requires (N < 0) { texCoords().erase(i); }

    void clear() requires (N < 0) { texCoords().clear(); }

private:
    template<typename Element>
    void importWedgeTexCoordsFrom(const Element& e)
    {
        for (uint i = 0; i < e.vertexNumber(); ++i) {
            wedgeTexCoord(i) = e.wedgeTexCoord(i).template cast<Scalar>();
        }
        textureIndex() = e.textureIndex();
    }

    Vector<vcl::TexCoord<Scalar>, N>& texCoords() { return Base::container(); }

    const Vector<vcl::TexCoord<Scalar>, N>& texCoords() const
    {
        return Base::container();
    }
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the WedgeTexCoords component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::WedgeTexCoords, with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal
 * WedgeTexCoords component and the vertical OptionalWedgeTexCoords component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides WedgeTexCoords component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasWedgeTexCoords.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasWedgeTexCoords =
    TITB::IsDerivedFromSpecializationOfV<T, WedgeTexCoords>;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the WedgeTexCoords component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the
 * HasOptionalWedgeTexCoords.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalWedgeTexCoords =
    HasWedgeTexCoords<T> &&
    IsOptionalComponent<typename RemoveRef<T>::WedgeTexCoords>;

/**
 * @private
 * @brief HasRightNumberOfWedgeTexCoords concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge texcoords must be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of wedge texcoords is the
 * same of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfWedgeTexCoords =
    RemoveRef<T>::VERTEX_NUMBER == RemoveRef<T>::WEDGE_TEX_COORD_NUMBER;

/**
 * @private
 * @brief SanityCheckWedgeTexCoords concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge texcoords must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have wedge texcoords;
 * - in case it has wedge texcoords, they have the same number of vertices of
 * the face.
 */
template<typename T>
concept SanityCheckWedgeTexCoords =
    !HasWedgeTexCoords<T> || HasRightNumberOfWedgeTexCoords<T>;

/* importFrom function */

template<typename Scalar, int N, typename ParentElemType, bool OPT>
template<typename Element>
void WedgeTexCoords<Scalar, N, ParentElemType, OPT>::importFrom(
    const Element& e,
    bool)
{
    if constexpr (HasWedgeTexCoords<Element>) {
        if (isWedgeTexCoordsAvailableOn(e)) {
            if constexpr (N > 0) {
                // same static size
                if constexpr (N == Element::WEDGE_TEX_COORD_NUMBER) {
                    importWedgeTexCoordsFrom(e);
                }
                // from dynamic to static, but dynamic size == static size
                else if constexpr (Element::WEDGE_TEX_COORD_NUMBER < 0) {
                    if (e.vertexNumber() == N) {
                        importWedgeTexCoordsFrom(e);
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
                importWedgeTexCoordsFrom(e);
            }
        }
    }
}

/* Detector function to check if a class has WedgeTexCoords available */

/**
 * @brief Checks if the given Element has WedgeTexCoords available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has WedgeTexCoords available, `false`
 * otherwise.
 */
bool isWedgeTexCoordsAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::WEDGE_TEX_COORDS>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
