/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_MARK_H
#define MGP_MESH_COMPONENTS_MARK_H

#include <type_traits>

namespace mgp::comp {

/**
 * @brief The Mark class is an utility class useful to un-mark components in constant time.
 *
 * Its implementation is just an integer that can be incremented and decremented.
 *
 * Assuming that two Elements (or a Mesh and an Element) have the Mark component: you can consider
 * one of the elements "marked" if it has the same mark value of the other element/mesh.
 *
 * Example: suppose that you have a Mesh with Vertex Elements, and both Mesh and Vertices have the
 * Mark component. In initialization, all the elements are considered marked, because the elements
 * have the same mark value of the Mesh. To un-mark all the vertices of the mesh:
 *
 * @code{.cpp}
 * m.incrementMark();
 * @endcode
 *
 * Now all the vertices (and all the other elements) are un-marked because they have a different
 * mark value w.r.t. the one of the mesh.
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
 */
class Mark
{
public:
	Mark() {}

	int mark() const { return m; }
	void resetMark() { m = 0; }

	template<typename E>
	bool hasSameMark(const E& e) const
	{
		if constexpr (std::is_pointer<E>::value) {
			return e->mark() == m;
		}
		else {
			return e.mark() == m;
		}
	}

	void incrementMark() { m++; }
	void decrementMark() { m--; }

private:
	int m = 0;
};

/**
 * Detector to check if a class has (inherits) Mark
 */

template<typename T>
using hasMarkT = std::is_base_of<Mark, T>;

template<typename T>
constexpr bool hasMark()
{
	return hasMarkT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_MARK_H
