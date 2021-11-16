/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_ELEMENT_REFERENCES_H
#define MGP_MESH_COMPONENTS_ELEMENT_REFERENCES_H

#include "generic_container.h"

namespace mgp::comp {

/**
 * @brief The ElementReferences class is a generic container of references to another Element
 * (that could be Vertex, Face...). This class is meant to be inherited and used by Components
 * like Vertex References, Face References: in general, a class that need to store a static or
 * dynamic number of references of another element.
 *
 * Its major use is for adjacencies.
 */
template<typename Elem, int N>
class ElementReferences : protected GenericContainer<Elem*, N>
{
private:
	using Base = GenericContainer<Elem*, N>;

public:
	/** Constructor **/

	ElementReferences();

protected:
	void updateElementReferences(const Elem* oldBase, const Elem* newBase);

	void updateElementReferencesAfterCompact(const Elem* base, const std::vector<int>& newIndices);
};

} // namespace mgp::comp

#include "element_references.cpp"

#endif // MGP_MESH_COMPONENTS_ELEMENT_REFERENCES_H
