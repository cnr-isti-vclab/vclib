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

#ifndef VCL_SPACE_VECTOR_POINTER_VECTOR_H
#define VCL_SPACE_VECTOR_POINTER_VECTOR_H

#include "vector.h"

#include <vclib/concepts/pointers.h>
#include <vclib/iterators/const_pointer_iterator.h>

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
 * @ingroup space
 */
template<IsAnyPointer T, int N = -1>
class PointerVector : protected Vector<T, N>
{
    using Base = Vector<T, N>;

public:
    // types
    using ValueType = Base::ValueType;

    using ConstValueType = vcl::MakeConstPointerT<ValueType>;

    using Reference = Base::Reference;

    using ConstPointer = const ConstValueType*;

    using Pointer = Base::Pointer;

    using Iterator = Base::Iterator;

    using ConstIterator = ConstPointerIterator<typename Base::ConstIterator>;

    using Base::SIZE;

    // constructors
    using Base::Vector;

    // exposing members of base class, later we will redefine const member
    // functions
    using Base::at;
    using Base::atMod;
    using Base::back;
    using Base::clear;
    using Base::contains;
    using Base::data;
    using Base::erase;
    using Base::fill;
    using Base::find;
    using Base::front;
    using Base::indexOf;
    using Base::insert;
    using Base::pushBack;
    using Base::resize;
    using Base::set;
    using Base::size;
    using Base::swap;
    using Base::operator[];
    using Base::operator();
    using Base::begin;
    using Base::end;

    // const member function redefinitions: return types change to const

    ConstValueType at(uint i) const { return Base::at(i); }

    ConstValueType atMod(uint i) const { return Base::atMod(i); }

    ConstValueType front() const { return Base::front(); }

    ConstValueType back() const { return Base::back(); }

    ConstPointer data() const { return Base::data(); }

    ConstIterator find(const typename MakeConstPointer<T>::type& e) const
    {
        return Base::find(e);
    }

    ConstValueType operator[](uint i) const { return Base::operator[](i); }

    ConstValueType operator()(uint i) const { return Base::operator()(i); }

    ConstIterator begin() const { return Base::begin(); }

    ConstIterator end() const { return Base::end(); }
};

} // namespace vcl

#endif // VCL_SPACE_VECTOR_POINTER_VECTOR_H
