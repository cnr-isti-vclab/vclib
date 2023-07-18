%HEADER%

#include "%EL%_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of %EL_UC%s.
 */
template<%EL_UC%Concept T>
%EL_UC%Container<T>::%EL_UC%Container()
{
}

/**
 * @brief Returns a const reference of the %EL_UC% at the i-th position
 * in the %EL_UC% Container of the Mesh, which will be the %EL_UC% having
 * index = i.
 *
 * This function does not perform any sanity check: if i is less than
 * %EL_C%ContainerSize(), this function will return a valid %EL_UC%
 * reference (note that the %EL_UC% may have been flagged as deleted).
 *
 * @param[in] i: the index of the %EL_UC% that will be returned.
 */
template<%EL_UC%Concept T>
const typename %EL_UC%Container<T>::%EL_UC%Type&
%EL_UC%Container<T>::%EL_C%(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the %EL_UC% at the i-th position in the
 * %EL_UC% Container of the Mesh, which will be the %EL_UC% having
 * index = i.
 *
 * This function does not perform any sanity check: if i is less than
 * %EL_C%ContainerSize(), this function will return a valid %EL_UC%
 * reference (note that the %EL_UC% may have been flagged as deleted).
 *
 * @param[in] i: the index of the %EL_UC% that will be returned.
 */
template<%EL_UC%Concept T>
typename %EL_UC%Container<T>::%EL_UC%Type&
%EL_UC%Container<T>::%EL_C%(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** %EL_UC%s contained in the
 * %EL_UC% container of the Mesh.
 *
 * If %EL_C%Number() != %EL_C%ContainerSize(), it means that there are some
 * %EL_UC%s that are flagged as deleted.
 *
 * @return the number of non-deleted %EL_UC%s of the Mesh.
 */
template<%EL_UC%Concept T>
uint %EL_UC%Container<T>::%EL_C%Number() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of %EL_UC%s (also deleted) contained in the
 * %EL_UC% container of the Mesh.
 *
 * If %EL_C%Number() != %EL_C%ContainerSize(), it means that there are some
 * %EL_UC%s that are flagged as deleted.
 *
 * @return the number of all the %EL_UC%s contained in the Mesh.
 */
template<%EL_UC%Concept T>
uint %EL_UC%Container<T>::%EL_C%ContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted %EL_UC%s in the %EL_UC% container,
 * that is %EL_C%ContainerSize() - %EL_C%Number().
 *
 * @return The number of deleted %EL_UC%s in the container.
 */
template<%EL_UC%Concept T>
uint %EL_UC%Container<T>::deleted%EL_UC%Number() const
{
	return Base::deletedElementNumber();
}

/**
 * @brief Add a %EL_UC% to the container, returning its index.
 *
 * If the call of this function will cause a reallocation of the
 * %EL_UC%Container, the function will automatically take care of updating
 * all the %EL_UC% pointers contained in the Mesh.
 *
 * @return the id of the newly added %EL_UC%.
 */
template<%EL_UC%Concept T>
uint %EL_UC%Container<T>::add%EL_UC%()
{
	return Base::addElement();
}

/**
 * @brief Add an arbitrary number of n %EL_UC%s, returning the id of the
 * first added %EL_UC%.
 *
 * This means that, if you want to add 5 %EL_UC%s and this member function
 * returns 4, the added %EL_UC%s will have id from 4 to id 8 included.
 *
 * If the call of this function will cause a reallocation of the
 * %EL_UC%Container, the function will automatically take care of updating
 * all the %EL_UC% pointers contained in the Mesh.
 *
 * @param n: the number of %EL_UC%s to add to the mesh.
 * @return the id of the first added %EL_UC%.
 */
template<%EL_UC%Concept T>
uint %EL_UC%Container<T>::add%EL_UC%s(uint n)
{
	return Base::addElements(n);
}

/**
 * @brief Reserve a number of %EL_UC%s in the container of %EL_UC%s. This
 * is useful when you know (or you have an idea) of how much %EL_UC%s are
 * going to add into a newly of existing mesh. Calling this function before any
 * add%EL_UC%() call will avoid unuseful reallocations of the container,
 * saving execution time.
 *
 * The filosofy of this function is similar to the one of the
 * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
 * function of the
 * [std::vector class](https://en.cppreference.com/w/cpp/container/vector).
 *
 * If the call of this function will cause a reallocation of the %EL_UC%
 * container, the function will automatically take care of updating all the
 * %EL_UC% pointers contained in the Mesh.
 *
 * @param n: the new capacity of the %EL_UC% container.
 */
template<%EL_UC%Concept T>
void %EL_UC%Container<T>::reserve%EL_UC%s(uint n)
{
	Base::reserveElements(n);
}

/**
 * @brief Compacts the %EL_UC%Container, removing all the %EL_UC%s marked
 * as deleted. %EL_UC%s indices will change accordingly. The function will
 * automatically take care of updating all the %EL_UC% pointers contained
 * in the Mesh.
 */
template<%EL_UC%Concept T>
void %EL_UC%Container<T>::compact%EL_UC%s()
{
	Base::compactElements();
}

/**
 * @brief Marks as deleted the %EL_UC% with the given id.
 *
 * This member function does not perform any reallocation of the %EL_UC%s:
 * the deleted %EL_UC%s will stay in the %EL_UC% Container, but will be
 * marked as deleted.
 *
 * Deleted %EL_UC%s are automatically jumped by the iterators provided by
 * the %EL_UC% Container.
 *
 * @param[in] i: the id of the %EL_UC% that will be marked as deleted.
 */
template<%EL_UC%Concept T>
void %EL_UC%Container<T>::delete%EL_UC%(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given %EL_UC%, before asserting that the
 * %EL_UC% belongs to this container.
 *
 * This member function does not perform any reallocation of the %EL_UC%s:
 * the deleted %EL_UC%s will stay in the %EL_UC% Container, but will be
 * marked as deleted.
 *
 * Deleted %EL_UC%s are automatically jumped by the iterators provided by
 * the %EL_UC% Container.
 *
 * @param[in] e: the pointer of the %EL_UC% that will be marked as deleted.
 */
template<%EL_UC%Concept T>
void %EL_UC%Container<T>::delete%EL_UC%(const %EL_UC%Type* e)
{
	Base::deleteElement(e);
}

/**
 * @brief This is an utility member function that returns the index of an 
 * element if the container would be compact, that is the number of non-deleted
 * elements before the %EL_UC% with the given index.
 *
 * Complexity: O(n), with n the number of %EL_UC%s in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a %EL_UC% of the container.
 * @return The index that the %EL_UC% with index i would have if this
 * container would be compact.
 */
template<%EL_UC%Concept T>
uint %EL_UC%Container<T>::%EL_C%IndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
}

/**
 * @brief Returns a vector that tells, for each actual %EL_UC% index, the
 * new index that the %EL_UC% would have in a compacted container. For each
 * deleted %EL_UC% index, the value of the vector will be UINT_NULL.
 *
 * This is useful if you need to know the indices of the %EL_UC%s that they
 * would have in a compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each %EL_UC% index, its index if the
 * container would be compact.
 */
template<%EL_UC%Concept T>
std::vector<uint> %EL_UC%Container<T>::%EL_C%CompactIndices() const
{
	return Base::elementCompactIndices();
}

/**
 * @brief Updates all the indices and pointers of the %EL_UC%s of this
 * container that are stored in any container of the mesh, according to
 * the mapping stored in the newIndices vector, that tells for each old
 * %EL_UC% index, the new %EL_UC% index.
 *
 * This function is useful when delete some %EL_UC%s, and you want to update
 * the indices/pointers stored in all the containers of the mesh accordingly.
 *
 * E.g. Supposing you deleted a set of %EL_UC%s, you can give to this
 * function the vector telling, for each one of the old %EL_UC% indices,
 * the new %EL_UC% index (or UINT_NULL if you want to leave it unreferenced).
 * This function will update all the pointers stored in the mesh containers
 * accordingly (if they store adjacencies to the %EL_UC%s).
 *
 * @note This function *does not change the position of the %EL_UC%s in
 * this container*. It just updates the indices/pointers of the %EL_UC%s
 * stored in this or other containers.
 *
 * @param[in] newIndices: a vector that tells, for each old %EL_UC% index,
 * the new %EL_UC% index. If the old %EL_UC% must be left as unreferenced
 * (setting `nullptr` to the pointers), the value of the vector must be
 * UINT_NULL.
 */
template<%EL_UC%Concept T>
void %EL_UC%Container<T>::update%EL_UC%Indices(
	const std::vector<uint>& newIndices)
{
	Base::updateElementIndices(newIndices);
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted %EL_UC%s of
 * the container. You can change this option by calling this function with
 * `jumpDeleted=false`.
 *
 * @param[in] jumpDeleted: boolean that tells if the iterator
 * should jump deleted %EL_UC%s.
 * @return An iterator the the first %EL_UC% of the container.
 */
template<%EL_UC%Concept T>
typename %EL_UC%Container<T>::%EL_UC%Iterator
%EL_UC%Container<T>::%EL_C%Begin(bool jumpDeleted)
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<%EL_UC%Concept T>
typename %EL_UC%Container<T>::%EL_UC%Iterator %EL_UC%Container<T>::%EL_C%End()
{
	return Base::elementEnd();
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted %EL_UC%s of
 * the container. You can change this option by calling this function with
 * `jumpDeleted=false`.
 *
 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
 * deleted %EL_UC%s.
 * @return A const iterator the the first %EL_UC% of the container.
 */
template<%EL_UC%Concept T>
typename %EL_UC%Container<T>::Const%EL_UC%Iterator
%EL_UC%Container<T>::%EL_C%Begin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<%EL_UC%Concept T>
typename %EL_UC%Container<T>::Const%EL_UC%Iterator
%EL_UC%Container<T>::%EL_C%End() const
{
	return Base::elementEnd();
}

/**
 * @brief Returns a small view object that allows to iterate over the
 * %EL_UC%s of the containers, providing two member functions begin()
 * and end().
 *
 * This member function is very useful when you want to iterate over the
 * %EL_UC%s using the C++ foreach syntax:
 *
 * @code{.cpp}
 * for (%EL_UC%& e : m.%EL_C%s()){
 *     // do something with this %EL_UC%
 * }
 * @endcode
 *
 * The iterator used to iterate over %EL_UC%s is automatically initialized
 * to jump deleted %EL_UC%s of the container. You can change this option by
 * calling this function with `jumpDeleted=false`.
 *
 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
 * deleted %EL_UC%s.
 * @return An object having begin() and end() function, allowing to iterate
 * over the container.
 */
template<%EL_UC%Concept T>
auto %EL_UC%Container<T>::%EL_C%s(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Returns a small view object that allows to iterate over the
 * %EL_UC%s of the containers, providing two member functions begin()
 * and end().
 *
 * This member function is very useful when you want to iterate over the
 * %EL_UC%s using the C++ foreach syntax:
 *
 * @code{.cpp}
 * for (const %EL_UC%& e : m.%EL_C%s()){
 *     // do something with this %EL_UC%
 * }
 * @endcode
 *
 * The iterator used to iterate over %EL_UC%s is automatically initialized
 * to jump deleted %EL_UC%s of the container. You can change this option by
 * calling this function with `jumpDeleted=false`.
 *
 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
 * deleted %EL_UC%s.
 * @return An object having begin() and end() function, allowing to iterate over
 * the container.
 */
template<%EL_UC%Concept T>
auto %EL_UC%Container<T>::%EL_C%s(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Enables all the optional components associated to the %EL_UC%
 * type contained in the %EL_UC%Container.
 */
template<%EL_UC%Concept T>
void %EL_UC%Container<T>::enableAllPer%EL_UC%OptionalComponents()
{
	Base::enableAllOptionalComponents();
}

/**
 * @brief Disables all the optional components associated to the %EL_UC%
 * type contained in the %EL_UC%Container.
 */
template<%EL_UC%Concept T>
void %EL_UC%Container<T>::disableAllPer%EL_UC%OptionalComponents()
{
	Base::disableAllOptionalComponents();
}

%OPTIONAL_COMPONENTS%

} // namespace vcl::mesh
