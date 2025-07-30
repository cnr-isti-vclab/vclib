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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_MESH_COMPONENTS_ADJACENT_FACES_H

#include "bases/reference_container_component.h"

#include <vclib/concepts.h>
#include <vclib/types.h>

namespace vcl::comp {

/**
 * @brief The AdjacentFaces class is a container of Face indices or pointers. It
 * could be used by any Element to save adjacencies information (also the Face
 * element itself).
 *
 * It is a random access container having static or dynamic size, depending on
 * the value of N (a negative number means dynamic).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentFaces
 * component, you'll be able to access to this component member functions from
 * `v`:
 *
 * @code{.cpp}
 * v.adjFacesNumber();
 * auto* f = v.adjFace(0);
 * uint vi = v.adjFaceIndex(0);
 * @endcode
 *
 * @note This component could be *Tied To Vertex Number*: it means that the size
 * of the container, if dynamic, will change automatically along the Vertex
 * Number of the Component. Check the `TTVN` template value on the
 * specialization of your component to check if it is tied to the Vertex Number.
 * For further details check the documentation of the @ref ContainerComponent
 * class.
 *
 * @tparam STORE_INDICES: If true, the component will store indices, otherwise
 * pointers to Face.
 * @tparam Face: The type of the adjacent Face element.
 * @tparam N: The size of the container, that will represent the number of
 * storable adjacent faces. If negative, the container is dynamic.
 * @tparam TTVN: If true, the size of the container will be tied to the Vertex
 * Number of the component (this is used mostly on Face elements).
 * @tparam ParentElemType: This type is used to get access to the Element that
 * has the component (and, in case, to the Mesh that has the Element). If the
 * component doesn't need to access the Element, this type can be void. Note:
 * if the component is vertical (or optional), this type cannot be void.
 * @tparam VERT: If true, the component will be stored vertically. This argument
 * is considered only if the ElementType is not void.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<
    bool STORE_INDICES,
    typename Face,
    int  N,
    bool TTVN,
    typename ParentElemType = void,
    bool VERT               = false,
    bool OPT                = false>
class AdjacentFaces :
        public ReferenceContainerComponent<
            STORE_INDICES,
            AdjacentFaces<
                STORE_INDICES,
                Face,
                N,
                TTVN,
                ParentElemType,
                VERT,
                OPT>,
            CompId::ADJACENT_FACES,
            Face,
            N,
            ParentElemType,
            VERT,
            OPT,
            TTVN>
{
    using Base = ReferenceContainerComponent<
        STORE_INDICES,
        AdjacentFaces<STORE_INDICES, Face, N, TTVN, ParentElemType, VERT, OPT>,
        CompId::ADJACENT_FACES,
        Face,
        N,
        ParentElemType,
        VERT,
        OPT,
        TTVN>;

public:
    /**
     * @brief Expose the type of the Adjacent Face.
     */
    using AdjacentFaceType = Face;

    /* Iterator Types declaration */

    using AdjacentFaceIterator           = Base::Iterator;
    using ConstAdjacentFaceIterator      = Base::ConstIterator;
    using ConstAdjacentFaceIndexIterator = Base::ConstIndexIterator;

    /**
     * @brief Static size of the container. If the container is dynamic, this
     * value will be negative and you should use the adjFacesNumber() member
     * function.
     */
    static const int ADJ_FACE_NUMBER = Base::SIZE;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the Adjacent Faces container size is static, initializes all the
     * Adjacent Faces to `nullptr`, otherwise the container will be empty.
     */
    AdjacentFaces() = default;

    /* Member functions */

    /**
     * @brief Returns the number of adjacent faces of this element.
     * @return The number of adjacent faces of this element.
     */
    uint adjFacesNumber() const { return Base::size(); }

    /**
     * @brief Returns the pointer to the i-th adjacent face of this element.
     *
     * @param[in] i: the position of the required adjacent face in this
     * container; the value must be between 0 and the number of adj faces.
     * @return The pointer to the i-th adjacent face of this element.
     */
    Face* adjFace(uint i) { return Base::element(i); }

    /**
     * @brief Returns a const pointer to the i-th adjacent face of this element.
     * @param[in] i: the position of the required adjacent face in this
     * container; the value must be between 0 and the number of adj faces.
     * @return The pointer to the i-th adjacent face of this element.
     */
    const Face* adjFace(uint i) const { return Base::element(i); }

    /**
     * @brief Returns the index in the face container of the i-th adjacent face
     * of the element.
     * @param[in] i: the position of the required face in this container.
     * @return The index of the i-th adjacent face of the element.
     */
    uint adjFaceIndex(uint i) const { return Base::elementIndex(i); }

    /**
     * @brief Returns the pointer to the i-th adjacent face of this element but
     * using as index the module between i and the number of adjacent faces.
     *
     * You can use this function if you need to get the "next
     * adjacent face after position k", without check if it is less than the
     * number of adj faces. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adjacent face
     * auto* next = e.adjFaceMod(k+1); // the adj face next to k, that may also
     *                                 // be at pos 0
     * auto* last = e.adjFaceMod(-1); // the adj face in position
     *                                // adjFacesNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required adjacent face in this
     * container, w.r.t. the position 0; value is modularized on
     * adjFacesNumber().
     * @return The pointer to the required adjacent face of this element.
     */
    Face* adjFaceMod(int i) { return Base::elementMod(i); }

    /**
     * @brief Same of adjFaceMod, but returns a const Pointer to the adjacent
     * face.
     * @param[in] i: the position of the required adjacent face in this
     * container, w.r.t. the position 0; value is modularized on
     * adjFacesNumber().
     * @return The pointer to the required adjacent face of this element.
     */
    const Face* adjFaceMod(int i) const { return Base::elementMod(i); }

    /**
     * @brief Returns the index in the face container of the i-th adjacent face
     * of the element, but using as index the module between i and the number of
     * adjacent faces. You can use this function if you need to get the "index
     * of the adjacent face next to position k", without check if it is less
     * than the number of adjacent faces. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adjacent face
     * auto idx = e.adjFaceIndexMod(k+1); // the index of the adjacent face next
     *                                    // to k, that may also be at pos 0
     * auto lastIdx = e.adjFaceIndexMod(-1); // the index of the adjacent face
     *                                       // in position adjFacesNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required adjacent face in this
     * container, w.r.t. the position 0; value is modularized on
     * adjFacesNumber().
     * @return The index of the required adjacent face of the element.
     */
    uint adjFaceIndexMod(int i) const { return Base::elementIndexMod(i); }

    /**
     * @brief Sets the i-th adjacent face of this element.
     * @param[in] i: the position in this container on which set the adj face;
     * the value must be between 0 and the number of adj faces.
     * @param[in] f: The pointer to the adjacent face to set to this element.
     */
    void setAdjFace(uint i, Face* f) { Base::setElement(i, f); }

    /**
     * @brief Sets the i-th adjacent face of the element.
     * @param[in] i: the position in this container on which set the adj face;
     * the value must be between 0 and the number of adj faces.
     * @param[in] fi: The index in the face container of the face to set.
     */
    void setAdjFace(uint i, uint fi) { Base::setElement(i, fi); }

    /**
     * @brief Sets the adjacent face pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * face; the value must be between begin() and end().
     * @param[in] f: The pointer to the adjacent face to set to the element.
     */
    void setAdjFace(ConstAdjacentFaceIterator it, Face* f)
    {
        Base::setElement(it, f);
    }

    /**
     * @brief Sets the adjacent face pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * face; the value must be between begin() and end().
     * @param[in] fi: The index in the face container of the face to set.
     */
    void setAdjFace(ConstAdjacentFaceIterator it, uint fi)
    {
        Base::setElement(it, fi);
    }

    /**
     * @brief Sets the adjacent face pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * face; the value must be between begin() and end().
     * @param[in] f: The pointer to the adjacent face to set to the element.
     */
    void setAdjFace(ConstAdjacentFaceIndexIterator it, Face* f)
    {
        Base::setElement(it, f);
    }

    /**
     * @brief Sets the adjacent face pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * face; the value must be between begin() and end().
     * @param[in] fi: The index in the face container of the face to set.
     */
    void setAdjFace(ConstAdjacentFaceIndexIterator it, uint fi)
    {
        Base::setElement(it, fi);
    }

    /**
     * @brief Sets the i-th adjacent face of the element, but using as index the
     * module between i and the number of adjacent faces. You can use this
     * function if you need to set the "next adjacent face after position k",
     * without check if it is less than the number of adjacent faces. Works also
     * for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adj face
     * e.setAdjFaceMod(k+1, aFace); // set the adj face next to k, that may also
     *                               // be at pos 0
     * e.setAdjFaceMod(-1, aFace); // set the adj face in position
     *                              // adjFacesNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the adj face; value is modularized on adjFacesNumber().
     * @param[in] f: The pointer to the adj face to set to the element.
     */
    void setAdjFaceMod(int i, Face* f) { Base::setElementMod(i, f); }

    /**
     * @brief Sets the i-th adjacent face of the element, but using as index the
     * module between i and the number of adjacent faces. You can use this
     * function if you need to set the "next adjacent face after position k",
     * without check if it is less than the number of adjacent faces. Works also
     * for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adj face
     * e.setAdjFaceMod(k+1, aFaceInd); // set the adj face next to k, that may
     *                                 // also be at pos 0
     * e.setAdjFaceMod(-1, aFaceInd); // set the adj face in position
     *                                // adjFacesNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the adj face; value is modularized on adjFacesNumber().
     * @param[in] fi: The index in the face containrt of the face to set.
     */
    void setAdjFaceMod(int i, uint fi) { Base::setElementMod(i, fi); }

    /**
     * @brief Sets all the adjacent faces of this element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of adjacent faces to set. The value
     * type of the range must be convertible to a pointer to an AdjacentFace.
     *
     * @param[in] r: range of face pointers to set.
     */
    template<Range Rng>
    void setAdjFaces(Rng&& r) requires InputRange<Rng, Face*>
    {
        Base::setElements(r);
    }

    /**
     * @brief Sets all the adjacent faces of this element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of adjacent faces to set. The value
     * type of the range must be convertible to an unsigned integer.
     *
     * @param[in] r: range of face indices to set.
     */
    template<Range Rng>
    void setAdjFaces(Rng&& r) requires InputRange<Rng, uint>
    {
        Base::setElements(r);
    }

    /**
     * @brief Returns `true` if the container of adjacent faces contains the
     * given face, `false` otherwise.
     *
     * @param[in] f: the pointer to the face to search.
     * @return `true` if the container of adjacent faces contains the given
     * face, `false` otherwise.
     */
    bool containsAdjFace(const Face* f) const
    {
        return Base::containsElement(f);
    }

    /**
     * @brief Returns `true` if the container of adjacent faces contains the
     * face with the given index, `false` otherwise.
     *
     * @param[in] fi: the index of the face to search.
     * @return `true` if the container of adjacent faces contains the face with
     * the given index, `false` otherwise.
     */
    bool containsAdjFace(uint fi) const { return Base::containsElement(fi); }

    /**
     * @brief Returns the index of the given adjacent face in the container of
     * this element. If the given adjacent face is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] f: the pointer to the adjacent face to search.
     * @return the index of the given adjacent face, or UINT_NULL if it is not
     * found.
     */
    uint indexOfAdjFace(const Face* f) const { return Base::indexOfElement(f); }

    /**
     * @brief Returns the index of the adjacent face with the given index in the
     * container of this element. If the adjacent face with the given index is
     * not in the container, returns UINT_NULL.
     *
     * @param[in] fi: the index of the adjacent face to search.
     * @return the index of the adjacent face with the given index, or UINT_NULL
     * if it is not found.
     */
    uint indexOfAdjFace(uint fi) const { return Base::indexOfElement(fi); }

    /* Member functions specific for dynamic vector adjacent faces */

    /**
     * @brief Resize the container of the adjacent faces to the given size.
     * @note This function is available only if the container of the Adjacent
     * Faces is has dynamic size.
     * @param[in] n: The new size of the adjacent faces container.
     */
    void resizeAdjFaces(uint n) requires (N < 0 && !TTVN) { Base::resize(n); }

    /**
     * @brief Pushes in the back of the container the given adjacent face.
     * @note This function is available only if the container of the Adjacent
     * Faces component has dynamic size.
     * @param[in] f: The pointer to the adjacent face to push in the back of the
     * container.
     */
    void pushAdjFace(Face* f) requires (N < 0 && !TTVN) { Base::pushBack(f); }

    /**
     * @brief Pushes in the back of the container the given adjacent face.
     * @note This function is available only if the container of the Adjacent
     * Faces component has dynamic size.
     * @param[in] fi: The index to the adjacent face to push in the back of the
     * container.
     */
    void pushAdjFace(uint fi) requires (N < 0 && !TTVN) { Base::pushBack(fi); }

    /**
     * @brief Inserts the given adjacent face in the container at the given
     * position.
     * @note This function is available only if the container of the Adjacent
     * Faces component has dynamic size.
     * @param[in] i: The position in this container where to insert the adjacent
     * face.
     * @param[in] f: The pointer to the adjacent face to insert in the
     * container.
     */
    void insertAdjFace(uint i, Face* f) requires (N < 0 && !TTVN)
    {
        Base::insert(i, f);
    }

    /**
     * @brief Inserts the adjacent face with the given index in the container at
     * the given position.
     * @note This function is available only if the container of the Adjacent
     * Faces component has dynamic size.
     * @param[in] i: The position in this container where to insert the adjacent
     * face.
     * @param[in] fi: The index to the adjacent face to insert in the container.
     */
    void insertAdjFace(uint i, uint fi) requires (N < 0 && !TTVN)
    {
        Base::insert(i, fi);
    }

    /**
     * @brief Removes the adjacent face at the given position from the
     * container.
     * @note This function is available only if the container of the Adjacent
     * Faces component has dynamic size.
     * @param[in] i: The position of the adjacent face to remove from this
     * container.
     */
    void eraseAdjFace(uint i) requires (N < 0 && !TTVN) { Base::erase(i); }

    /**
     * @brief Clears the container of adjacent faces, making it empty.
     * @note This function is available only if the container of the Adjacent
     * Faces component has dynamic size.
     */
    void clearAdjFaces() requires (N < 0 && !TTVN) { Base::clear(); }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first adjacent face in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    AdjacentFaceIterator adjFaceBegin() { return Base::elementBegin(); }

    /**
     * @brief Returns an iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    AdjacentFaceIterator adjFaceEnd() { return Base::elementEnd(); }

    /**
     * @brief Returns a const iterator to the first adjacent face in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    ConstAdjacentFaceIterator adjFaceBegin() const
    {
        return Base::elementBegin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstAdjacentFaceIterator adjFaceEnd() const { return Base::elementEnd(); }

    /**
     * @brief Returns an iterator to the first adjacent face index in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of the adjacent face indices.
     */
    ConstAdjacentFaceIndexIterator adjFaceIndexBegin() const
    {
        return Base::elementIndexBegin();
    }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of the adjacent face indices.
     */
    ConstAdjacentFaceIndexIterator adjFaceIndexEnd() const
    {
        return Base::elementIndexEnd();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and
     * end iterators of the container of adjacent faces of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto* adjFace : el.adjFaces()) {
     *     // Do something with adjFace
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent faces.
     */
    View<AdjacentFaceIterator> adjFaces() { return Base::elements(); }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the container of adjacent faces of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (const auto* adjFace : el.adjFaces()) {
     *     // Do something read-only with adjFace
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent faces.
     */
    View<ConstAdjacentFaceIterator> adjFaces() const
    {
        return Base::elements();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of adjacent face indices of the element. The
     * view object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (uint eid : el.adjFaceIndices()) {
     *     // Do something with adj face index...
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent face indices.
     */
    View<ConstAdjacentFaceIndexIterator> adjFaceIndices() const
    {
        return Base::elementIndices();
    }

    // dummy member to discriminate between AdjacentFaces and
    // FaceHalfEdgePointers
    void __adjacentFaces() const {}

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool importRefs = true)
    {
        if (importRefs) {
            if constexpr (HasAdjacentFaces<Element>) {
                if (isAdjacentFacesAvailableOn(e)) {
                    if constexpr (N > 0) {
                        // same static size
                        if constexpr (N == Element::ADJ_FACE_NUMBER) {
                            importIndicesFrom(e);
                        }
                        // from dynamic to static, but dynamic size == static
                        // size
                        else if constexpr (Element::ADJ_FACE_NUMBER < 0) {
                            if (e.adjFacesNumber() == N) {
                                importIndicesFrom(e);
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
                        Base::resize(e.adjFacesNumber());
                        importIndicesFrom(e);
                    }
                }
            }
        }
    }

    void serialize(std::ostream& os) const
    {
        // regardless of the type of the container (indices or pointers), the
        // serialization is always done using the indices
        if constexpr (N < 0) {
            vcl::serialize(os, adjFacesNumber());
        }
        for (uint i = 0; i < adjFacesNumber(); ++i) {
            vcl::serialize(os, adjFaceIndex(i));
        }
    }

    void deserialize(std::istream& is)
    {
        if constexpr (N < 0) {
            uint n;
            vcl::deserialize(is, n);
            Base::resize(n);
        }
        for (uint i = 0; i < adjFacesNumber(); ++i) {
            uint afi;
            vcl::deserialize(is, afi);
            setAdjFace(i, afi);
        }
    }

private:
    template<typename Element>
    void importIndicesFrom(const Element& e)
    {
        for (uint i = 0; i < e.adjFacesNumber(); ++i) {
            setAdjFace(i, e.adjFaceIndex(i));
        }
    }
};

/* Detector function to check if a class has AdjacentFaces available */

/**
 * @brief Checks if the given Element has AdjacentFaces available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has AdjacentFaces available, `false` otherwise.
 */
bool isAdjacentFacesAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::ADJACENT_FACES>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_FACES_H
