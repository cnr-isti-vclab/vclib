// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include "base/component_predicates.h"
#include "base/container_references_component.h"

#include <vclib/mesh/elements/base/base.h>

#include <vclib/base.h>

#include <cassert>
#include <concepts>
#include <ranges>

namespace vcl::comp {

/**
 * @brief The VertexReferences class represents a component that stores a
 * container of indices to vertices that will be part of an Element
 * (e.g. Face, Edge, Tetrahedron, etc.).
 *
 * The container of Vertex references is a random access container that could
 * have static or dynamic size, depending on the value of the template argument
 * N (a negative number means dynamic).
 *
 * For example, if a Face element should represent only triangles, N could be
 * 3. In this case, the container will have static size and it can't be resized
 * in any way. If N is -1, the container will have dynamic size and it will be
 * possible to resize it (e.g. if the Face element should represent a polygon).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the VertexReferences
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * uint n = f.vertexCount();
 * auto v = f.vertex(0);
 * auto vi = f.vertexIndex(0); // Returns IndexType
 * @endcode
 *
 * ### Dual Index Spaces: Global Vertex Indices vs. Local Corner Positions
 *
 * It is essential to distinguish between the two indexing spaces in this
 * component:
 *
 * 1. **Global Vertex Index (`IndexType`)**:
 *    - The global index or ID of a vertex inside the mesh's `VertexContainer`
 *      (`m.vertex(vi)`).
 *    - Typed as `IndexType` (32 or 64 bit).
 *    - Sentinel value: `NULL_INDEX` (`vcl::NULL_INDEX<IndexType>`).
 *    - Returned by `vertexIndex(uint i)` and `vertexIndexMod(int i)`.
 *    - Passed to search functions like `containsVertex(IndexType vi)`,
 *      `indexOfVertex(IndexType vi)`, and `indexOfEdge(IndexType vi1, IndexType
 *      vi2)`.
 *
 * 2. **Local Corner / Vertex Position (`uint`)**:
 *    - The 0-based position or slot `i` within this element's vertex list
 *      (`0 <= i < vertexCount()`, e.g. 0, 1, 2 for a triangle face; 0 to $k-1$
 * for a polygon).
 *    - Always typed as `uint` (`uint32_t`).
 *    - Sentinel value: `vcl::UINT_NULL` (`0xFFFFFFFF`), indicating "not found".
 *    - Passed as the position `uint i` to `vertex(uint i)`, `vertexIndex(uint
 * i)`, and `setVertex(uint i, IndexType vi)`.
 *    - Returned by `indexOfVertex(IndexType vi)` and `indexOfEdge(IndexType
 * vi1, IndexType vi2)`.
 *
 * @note This component is usually the main component of an Element, and
 * therefore is automatically enabled in the Element's container and cannot be
 * disabled.
 *
 * @note Several components are *Tied To Vertex Count*: in other words, they
 * are composed by a container that has the same size of the VertexReferences
 * and, when the VertexReferences container is resized, also the container of
 * these components is resized automatically.
 *
 * @tparam N: The size of the container, that will represent the number of
 * storable vertices. If negative, the container is dynamic.
 * @tparam Index: The unsigned integral type used to store indices (default
 * uint).
 *
 * @ingroup components
 */
template<int N, std::unsigned_integral Index = uint>
class VertexReferences :
        public ContainerReferencesComponent<
            CompId::VERTEX_REFERENCES,
            Index,
            ElemId::VERTEX,
            N,
            false>
{
    using Base = ContainerReferencesComponent<
        CompId::VERTEX_REFERENCES,
        Index,
        ElemId::VERTEX,
        N,
        false>;

public:
    using IndexType                   = Index;
    static constexpr int VERTEX_COUNT = N;
    using Base::NULL_INDEX;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the VertexReferences container size is static, initializes all the
     * indices to `NULL_INDEX`, otherwise the container will be empty.
     */
    VertexReferences() = default;

    /* Member functions */

    /**
     * @brief Returns the number of vertices of the element.
     * @return The number of vertices of the element.
     */
    uint vertexCount(this auto&& self) { return Base::container(self).size(); }

    /**
     * @brief Returns the i-th vertex of the element.
     *
     * @param[in] i: the position of the required vertex in this container.
     * @return The i-th vertex of the element.
     */
    decltype(auto) vertex(this auto&& self, uint i)
    {
        assert(i < self.vertexCount());
        return Base::refElement(self, i);
    }

    /**
     * @brief Returns the global index in the VertexContainer of the i-th vertex
     * of the element.
     * @param[in] i: the 0-based local corner/position of the required vertex in
     * this container (0 <= i < vertexCount()).
     * @return The global index (IndexType) of the i-th vertex of the element.
     */
    IndexType vertexIndex(this auto&& self, uint i)
    {
        assert(i < self.vertexCount());
        return self.vertexIndices()[i];
    }

    /**
     * @brief Returns  the i-th vertex of the element, using as index the modulo
     * between i and the number of vertices. Works also for negative numbers.
     *
     * @param[in] i: the position of the required vertex in this container,
     * w.r.t. position 0; value is modularized on vertexCount().
     * @return The required vertex of the element.
     */
    decltype(auto) vertexMod(this auto&& self, int i)
    {
        int n = self.vertexCount();
        assert(n > 0);
        return self.vertex(((i % n) + n) % n);
    }

    /**
     * @brief Returns the index in the vertex container of the i-th vertex of
     * the element, using as index the modulo between i and the number of
     * vertices. Works also for negative numbers.
     *
     * @param[in] i: the position of the required vertex in this container,
     * w.r.t. position 0; value is modularized on vertexCount().
     * @return The index of the required vertex of the element.
     */
    IndexType vertexIndexMod(this auto&& self, int i)
    {
        int n = self.vertexCount();
        assert(n > 0);
        return self.vertexIndex(((i % n) + n) % n);
    }

    /**
     * @brief Sets the i-th vertex of the element.
     * @param[in] i: the position in this container on which set the vertex; the
     * value must be between 0 and the number of vertices.
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertex(this auto&& self, uint i, IndexType vi)
    {
        assert(i < self.vertexCount());
        Base::container(self)[i] = vi;
    }

    /**
     * @brief Sets the i-th vertex of the element.
     * @param[in] i: the position in this container on which set the vertex; the
     * value must be between 0 and the number of vertices.
     * @param[in] v: The vertex to set to the element.
     */
    template<typename V>
    void setVertex(this auto&& self, uint i, const V& v)
        requires (!std::unsigned_integral<V>)
    {
        assert(i < self.vertexCount());
        if constexpr (requires { v.index(); }) {
            Base::container(self)[i] = v.index();
        }
        else {
            Base::container(self)[i] = v;
        }
    }

    /**
     * @brief Sets the index of the i-th vertex of the element. Alias for
     * setVertex.
     * @param[in] i: the position in this container on which set the vertex
     * index.
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertexIndex(this auto&& self, uint i, IndexType vi)
    {
        self.setVertex(i, vi);
    }

    /**
     * @brief Sets the i-th vertex of the element, using as index the modulo
     * between i and the number of vertices. Works also for negative numbers.
     *
     * @param[in] i: the position in this container w.r.t. position 0 on
     * which to set the vertex; value is modularized on vertexCount().
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertexMod(this auto&& self, int i, IndexType vi)
    {
        int n = self.vertexCount();
        assert(n > 0);
        self.setVertex(((i % n) + n) % n, vi);
    }

    /**
     * @brief Sets the i-th vertex of the element, using as index the modulo
     * between i and the number of vertices. Works also for negative numbers.
     *
     * @param[in] i: the position in this container w.r.t. position 0 on
     * which to set the vertex; value is modularized on vertexCount().
     * @param[in] v: The vertex to set to the element.
     */
    template<typename V>
    void setVertexMod(this auto&& self, int i, const V& v)
        requires (!std::unsigned_integral<V>)
    {
        int n = self.vertexCount();
        assert(n > 0);
        self.setVertex(((i % n) + n) % n, v);
    }

    /**
     * @brief Sets all the vertices of the element from a range of vertices or
     * indices.
     *
     * If the size of the container is static, the size of the input range must
     * match N. If dynamic, the container is resized accordingly.
     *
     * @tparam Rng: The type of the range of vertices or vertex indices.
     * @param[in] rng: range of vertices or vertex indices to set.
     */
    template<std::ranges::input_range Rng>
    void setVertices(this auto&& self, Rng&& rng)
    {
        if constexpr (N >= 0) {
            Base::setRefs(self, std::forward<Rng>(rng));
        }
        else {
            if constexpr (std::ranges::sized_range<Rng>) {
                self.resizeVertices(std::ranges::size(rng));
                uint i = 0;
                for (auto&& v : rng) {
                    self.setVertex(i++, v);
                }
            }
            else if constexpr (std::ranges::forward_range<Rng>) {
                self.resizeVertices(std::ranges::distance(rng));
                uint i = 0;
                for (auto&& v : rng) {
                    self.setVertex(i++, v);
                }
            }
            else {
                self.resizeVertices(0);
                for (auto&& v : rng) {
                    self.pushVertex(v);
                }
            }
        }
    }

    /**
     * @brief Sets all the vertices of the element from a variadic list of
     * vertices or indices.
     *
     * @param[in] args: The vertices or vertex indices to set.
     */
    template<typename... Args>
    void setVertices(this auto&& self, const Args&... args)
        requires (sizeof...(Args) > 1 && (sizeof...(Args) == N || N < 0))
    {
        if constexpr (N >= 0) {
            Base::setRefs(self, args...);
        }
        else {
            self.resizeVertices(sizeof...(Args));
            uint i = 0;
            (self.setVertex(i++, args), ...);
        }
    }

    /**
     * @brief Returns `true` if the container of vertices contains the vertex
     * with the given index, `false` otherwise.
     *
     * @param[in] vi: the index of the vertex to search.
     * @return `true` if the container of vertices contains the vertex with the
     * given index, `false` otherwise.
     */
    bool containsVertex(this auto&& self, IndexType vi)
    {
        return Base::container(self).contains(vi);
    }

    /**
     * @brief Returns `true` if the container of vertices contains the given
     * vertex, `false` otherwise.
     *
     * @param[in] v: the vertex to search.
     * @return `true` if the container of vertices contains the given vertex,
     * `false` otherwise.
     */
    template<typename V>
    bool containsVertex(this auto&& self, const V& v)
        requires (!std::unsigned_integral<V>)
    {
        if constexpr (requires { v.index(); }) {
            return self.containsVertex(v.index());
        }
        else {
            return self.containsVertex(static_cast<IndexType>(v));
        }
    }

    /**
     * @brief Returns the 0-based local corner/position in this container of the
     * vertex with the given global index. If not found, returns UINT_NULL.
     *
     * @param[in] vi: the global index (IndexType) of the vertex to search.
     * @return The 0-based local position (uint) in this container, or UINT_NULL
     * if not found.
     */
    uint indexOfVertex(this auto&& self, IndexType vi)
    {
        return Base::container(self).indexOf(vi);
    }

    /**
     * @brief Returns the position in this container of the given vertex. If
     * not found, returns UINT_NULL.
     *
     * @param[in] v: the vertex to search.
     * @return the position in this container of the vertex, or UINT_NULL if not
     * found.
     */
    template<typename V>
    uint indexOfVertex(this auto&& self, const V& v)
        requires (!std::unsigned_integral<V>)
    {
        if constexpr (requires { v.index(); }) {
            return self.indexOfVertex(v.index());
        }
        else {
            return self.indexOfVertex(static_cast<IndexType>(v));
        }
    }

    /**
     * @brief Returns the 0-based local index of the edge composed of the two
     * vertices with the given global indices in this element.
     *
     * The order of the two vertices is not important. The local index of the
     * edge corresponds to the local position of the first vertex found in the
     * container (i.e. edge i connects vertex i and vertex (i+1)%n). If the edge
     * is not found, returns UINT_NULL.
     *
     * @param[in] vi1: the global index (IndexType) of the first vertex of the
     * edge.
     * @param[in] vi2: the global index (IndexType) of the second vertex of the
     * edge.
     * @return The 0-based local edge position (uint), or UINT_NULL if not
     * found.
     */
    uint indexOfEdge(this auto&& self, IndexType vi1, IndexType vi2)
    {
        uint vid = self.indexOfVertex(vi1);
        if (vid == UINT_NULL) {
            return UINT_NULL;
        }
        else if (self.vertexIndexMod(vid + 1) == vi2) {
            return vid;
        }
        else if (self.vertexIndexMod(static_cast<int>(vid) - 1) == vi2) {
            int n = self.vertexCount();
            return ((static_cast<int>(vid) - 1) % n + n) % n;
        }
        else {
            return UINT_NULL;
        }
    }

    /**
     * @brief Returns the index of the edge composed of the two given vertices
     * in this element.
     *
     * @param[in] v1: the first vertex of the edge.
     * @param[in] v2: the second vertex of the edge.
     * @return the index of the edge, or UINT_NULL if not found.
     */
    template<typename V1, typename V2>
    uint indexOfEdge(this auto&& self, const V1& v1, const V2& v2)
        requires (!std::unsigned_integral<V1> || !std::unsigned_integral<V2>)
    {
        auto toIdx = [](const auto& v) -> IndexType {
            if constexpr (requires { v.index(); }) {
                return v.index();
            }
            else {
                return static_cast<IndexType>(v);
            }
        };
        return self.indexOfEdge(toIdx(v1), toIdx(v2));
    }

    /* Member functions specific for dynamic vector of refs */

    /**
     * @brief Resizes the container of vertices to the given size.
     * @note Available only if the container has dynamic size (N < 0).
     * @param[in] n: The new size of the vertices container.
     */
    void resizeVertices(this auto&& self, uint n) requires (N < 0)
    {
        Base::container(self).resize(n);
        self.parentContainer().resizeTTVCComponents(self.index(), n);
    }

    /**
     * @brief Pushes in the back of the container the given vertex index.
     * @note Available only if the container has dynamic size (N < 0).
     * @param[in] vi: The index of the vertex to push back.
     */
    void pushVertex(this auto&& self, IndexType vi) requires (N < 0)
    {
        Base::container(self).pushBack(vi);
        self.parentContainer().pushBackTTVCComponents(self.index());
    }

    /**
     * @brief Pushes in the back of the container the given vertex.
     * @note Available only if the container has dynamic size (N < 0).
     * @param[in] v: The vertex to push back.
     */
    template<typename V>
    void pushVertex(this auto&& self, const V& v)
        requires (N < 0 && !std::unsigned_integral<V>)
    {
        if constexpr (requires { v.index(); }) {
            self.pushVertex(v.index());
        }
        else {
            self.pushVertex(static_cast<IndexType>(v));
        }
    }

    /**
     * @brief Inserts the given vertex index at position i.
     * @note Available only if the container has dynamic size (N < 0).
     * @param[in] i: The position in this container where to insert the vertex.
     * @param[in] vi: The index of the vertex to insert.
     */
    void insertVertex(this auto&& self, uint i, IndexType vi) requires (N < 0)
    {
        Base::container(self).insert(i, vi);
        self.parentContainer().insertTTVCComponents(self.index(), i);
    }

    /**
     * @brief Inserts the given vertex at position i.
     * @note Available only if the container has dynamic size (N < 0).
     * @param[in] i: The position in this container where to insert the vertex.
     * @param[in] v: The vertex to insert.
     */
    template<typename V>
    void insertVertex(this auto&& self, uint i, const V& v)
        requires (N < 0 && !std::unsigned_integral<V>)
    {
        if constexpr (requires { v.index(); }) {
            self.insertVertex(i, v.index());
        }
        else {
            self.insertVertex(i, static_cast<IndexType>(v));
        }
    }

    /**
     * @brief Removes the vertex at position i from the container.
     * @note Available only if the container has dynamic size (N < 0).
     * @param[in] i: The position of the vertex to remove.
     */
    void eraseVertex(this auto&& self, uint i) requires (N < 0)
    {
        Base::container(self).erase(i);
        self.parentContainer().eraseTTVCComponents(self.index(), i);
    }

    /**
     * @brief Clears the container of vertices, making it empty.
     * @note Available only if the container has dynamic size (N < 0).
     */
    void clearVertices(this auto&& self) requires (N < 0)
    {
        Base::container(self).clear();
        self.parentContainer().clearTTVCComponents(self.index());
    }

    decltype(auto) vertices(this auto&& self)
    {
        return Base::refElements(self);
    }

    auto vertexIndices(this auto&& self)
    {
        return vcl::View(
            Base::container(self).begin(), Base::container(self).end());
    }

protected:
    template<typename OtherElement>
    void importFrom(
        this auto&&         self,
        const OtherElement& other,
        bool                importRefs = true);
};

/* concepts */

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * VertexReferences component (COMP_ID = CompId::VERTEX_REFERENCES).
 */
template<typename T>
concept HasVertexReferences = HasComponent<CompId::VERTEX_REFERENCES, T>;

template<typename T>
concept HasStaticVertexReferences =
    HasComponent<CompId::VERTEX_REFERENCES, T> &&
    (ComponentTypeT<CompId::VERTEX_REFERENCES, T>::VERTEX_COUNT > 0);

template<typename T>
concept HasEdgeVertexReferences =
    HasComponent<CompId::VERTEX_REFERENCES, T> &&
    (ComponentTypeT<CompId::VERTEX_REFERENCES, T>::VERTEX_COUNT == 2);

template<typename T>
concept HasTriangleVertexReferences =
    HasComponent<CompId::VERTEX_REFERENCES, T> &&
    (ComponentTypeT<CompId::VERTEX_REFERENCES, T>::VERTEX_COUNT == 3);

template<typename T>
concept HasPolygonVertexReferences =
    HasComponent<CompId::VERTEX_REFERENCES, T> &&
    (ComponentTypeT<CompId::VERTEX_REFERENCES, T>::VERTEX_COUNT == -1);

/* Implementation of member functions */

template<int N, std::unsigned_integral Index>
template<typename OtherElement>
void VertexReferences<N, Index>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool                importRefs)
{
    if (importRefs) {
        if constexpr (HasVertexReferences<OtherElement>) {
            using OtherComp =
                ComponentTypeT<CompId::VERTEX_REFERENCES, OtherElement>;
            constexpr int OTHER_N = OtherComp::VERTEX_COUNT;

            if constexpr (N > 0) {
                if constexpr (N == OTHER_N) {
                    for (uint i = 0; i < N; ++i) {
                        self.setVertex(
                            i, Base::castIndex(other.vertexIndex(i)));
                    }
                }
                else if constexpr (OTHER_N < 0) {
                    if (other.vertexCount() == N) {
                        for (uint i = 0; i < N; ++i) {
                            self.setVertex(
                                i, Base::castIndex(other.vertexIndex(i)));
                        }
                    }
                }
            }
            else {
                self.resizeVertices(other.vertexCount());
                for (uint i = 0; i < other.vertexCount(); ++i) {
                    self.setVertex(i, Base::castIndex(other.vertexIndex(i)));
                }
            }
        }
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
