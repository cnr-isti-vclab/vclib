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

#ifndef VCL_MESH_COMPONENTS_MARK_H
#define VCL_MESH_COMPONENTS_MARK_H

#include "bases/component.h"

#include <vclib/concepts/mesh/components/mark.h>
#include <vclib/io/serialization.h>

namespace vcl::comp {

/**
 * @brief The Mark class is an utility class useful to un-mark components in
 * constant time.
 *
 * Its implementation is just an integer that can be incremented and
 * decremented.
 *
 * Assuming that two Elements (or a Mesh and an Element) have the Mark
 * component: you can consider one of the elements "marked" if it has the same
 * mark value of the other element/mesh.
 *
 * Example: suppose that you have a Mesh with Vertex Elements, and both Mesh and
 * Vertices have the Mark component. In initialization, all the elements are
 * considered marked, because the elements have the same mark value of the Mesh.
 * To un-mark all the vertices of the mesh:
 *
 * @code{.cpp}
 * m.incrementMark();
 * @endcode
 *
 * Now all the vertices (and all the other elements) are un-marked because they
 * have a different mark value w.r.t. the one of the mesh.
 *
 * Then, if you want to mark the vertices having index 3 and 5:
 *
 * @code{.cpp}
 * m.vertex(3).incrementMark();
 * m.vertex(5).incrementMark();
 * @endcode
 *
 * And to check if vertices 4 and 5 are marked:
 *
 * @code{.cpp}
 * m.hasSameMark(m.vertex(4)); // or: m.vertex(4).hasSameMark(m)
 * m.hasSameMark(m.vertex(5)); // or: m.vertex(5).hasSameMark(m)
 * @endcode
 *
 * @note This component can be both used for Elements and Meshes.
 *
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ParentElemType = void, bool OPT = false>
class Mark :
        public Component<
            Mark<ParentElemType, OPT>,
            CompId::MARK,
            int,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        Mark<ParentElemType, OPT>,
        CompId::MARK,
        int,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /* Constructors */

    /**
     * @brief Constructor that initializes the mark to 0.
     */
    Mark()
    {
        if constexpr (!Base::IS_VERTICAL) {
            init();
        }
    }

    /**
     * @private
     * @brief Initializes the mark to 0.
     *
     * It is made in the init function since the component could be not
     * available during construction (e.g. if the component is optional and not
     * enabled).
     *
     * This member function is hidden by the element that inherits this class.
     */
    void init() { markRef() = 0; }

    /* Member functions */

    /**
     * @brief Returns the value of the mark.
     * @return the value of the mark.
     */
    int mark() const { return Base::data(); }

    /**
     * @brief Resets the mark to 0.
     */
    void resetMark() { markRef() = 0; }

    /**
     * @brief Checks if the current element/mesh has the same mark of the given
     * input element/mesh `e`.
     *
     * @tparam E: the type of the input element/mesh.
     * @param e: the input element/mesh.
     */
    template<typename E>
    bool hasSameMark(const E& e) const
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
    void incrementMark() { markRef()++; }

    /**
     * @brief Decrements the mark of the current element/mesh by 1.
     */
    void decrementMark() { markRef()--; }

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        if constexpr (HasMark<Element>) {
            if (isMarkAvailableOn(e)) {
                markRef() = e.mark();
            }
        }
    }

    void serialize(std::ostream& os) const { vcl::serialize(os, mark()); }

    void deserialize(std::istream& is) { vcl::deserialize(is, markRef()); }

private:
    int& markRef() { return Base::data(); }
};

/* Detector function to check if a class has Mark available */

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
bool isMarkAvailableOn(const ElementOrMeshConcept auto& element)
{
    return isComponentAvailableOn<CompId::MARK>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_MARK_H
