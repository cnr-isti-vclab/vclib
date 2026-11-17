// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_CORE_VECTOR_POINTER_VECTOR_H
#define VCL_SPACE_CORE_VECTOR_POINTER_VECTOR_H

#include "vector.h"

namespace vcl {

/**
 * @brief The `PointerVector` class is a utility container that stores a
 * sequence of pointers that preserve their constness when the container is
 * constant.
 *
 * It is a wrapper around the `Vector` class, and it provides the same
 * interface. The main difference is that the `PointerVector` class is designed
 * to store pointers that, when accessed from the const container, are returned
 * as const pointers. This is useful when you want to store a sequence of
 * objects that are not owned by the container, and you want to ensure that the
 * objects are not modified through the container when the container is const.
 *
 * For example:
 * @code{.cpp}
 * std::vector<int*> v;
 * vcl::PointerVector<int*> pv;
 *
 * // populate v and pv
 *
 * // const references to the vectors (e.g. when passing the vectors to a
 * // function)
 * const std::vector<int*>& refV = v;
 * const vcl::PointerVector<int*>& refPv = pv;
 *
 * // the following code compiles:
 * int* p1 = refV[0]; // const std::vector returns a non-const pointer!!
 * *p1 = 42;
 * // the following code does not compile:
 * // int* p2 = refPv[0]; // const PointerVector returns a const pointer
 * // *p2 = 42;
 * @endcode
 *
 * @tparam T: the type of the objects stored in the container. It must be a
 * pointer type.
 * @tparam N: the size of the container. If `N` is greater than or equal to
 * zero, the container will have a fixed size of `N` elements, and it will use
 * an array to store the elements. If `N` is less than zero, the container will
 * have a dynamic size, and it will use a vector to store the elements.
 *
 * @ingroup space_core
 */
template<IsAnyPointer T, int N = -1>
class PointerVector : protected Vector<T, N>
{
    using Base = Vector<T, N>;

    friend Base;

public:
    // types
    using ValueType = Base::ValueType;

    using ConstValueType = MakeConstPointerT<ValueType>;

    using Reference = Base::Reference;

    using ConstPointer = const ConstValueType*;

    using Pointer = Base::Pointer;

    using Iterator = Base::Iterator;

    using ConstIterator = ConstPointerIterator<typename Base::ConstIterator>;

    using Base::SIZE;

    // constructors
    using Base::Vector;

    // exposing members of base class, later we will redefine the accessor
    // member functions
    using Base::clear;
    using Base::contains;
    using Base::empty;
    using Base::erase;
    using Base::fill;
    using Base::indexOf;
    using Base::insert;
    using Base::pushBack;
    using Base::resize;
    using Base::set;
    using Base::size;
    using Base::swap;

    // accessor member function redefinitions: when const, return types change
    // to const (pointers to const).
    // here deducing this cannot be used because we are changing the return
    // type of the const member function, so we need to define both of them

    Reference at(uint i) { return Base::at(i); }

    ConstValueType at(uint i) const { return Base::at(i); }

    Reference atMod(int i) { return Base::atMod(i); }

    ConstValueType atMod(int i) const { return Base::atMod(i); }

    Reference front() { return Base::front(); }

    ConstValueType front() const { return Base::front(); }

    Reference back() { return Base::back(); }

    ConstValueType back() const { return Base::back(); }

    Pointer data() { return Base::data(); }

    ConstPointer data() const { return Base::data(); }

    Iterator find(const MakeConstPointer<T>& e) { return Base::find(e); }

    ConstIterator find(const MakeConstPointer<T>& e) const
    {
        return Base::find(e);
    }

    Reference operator[](uint i) { return Base::operator[](i); }

    ConstValueType operator[](uint i) const { return Base::operator[](i); }

    Reference operator()(uint i) { return Base::operator()(i); }

    ConstValueType operator()(uint i) const { return Base::operator()(i); }

    Iterator begin() { return Base::begin(); }

    ConstIterator begin() const { return Base::begin(); }

    Iterator end() { return Base::end(); }

    ConstIterator end() const { return Base::end(); }
};

} // namespace vcl

#endif // VCL_SPACE_CORE_VECTOR_POINTER_VECTOR_H
