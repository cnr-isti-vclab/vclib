// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_VIEWS_VIEW_H
#define VCL_BASE_VIEWS_VIEW_H

#define ZIP_VIEW_INJECT_STD_VIEWS_NAMESPACE
#if __has_include(<zip_view.hpp>)
#include <zip_view.hpp>
#else
// inclusion for usage of vclib without CMake - not ideal but necessary for
// header only
#include "../../../../../../3rdparty/zip-views-1.0/zip_view.hpp"
#endif
#undef ZIP_VIEW_INJECT_STD_VIEWS_NAMESPACE

#include <ranges>

namespace vcl {

/**
 * @brief The View class is a simple class that stores and exposes two iterators
 * begin and end.
 *
 * It is useful for classes that expose multiple containers, and they do not
 * expose the classic member functions begin()/end(). In these cases, it is
 * possible to expose the view of a selected container by returning a View
 * object initialized with the begin/end iterators.
 *
 * For example, a Mesh can expose Vertex and Face containers.
 * The mesh exposes the member functions:
 * - vertexBegin()
 * - vertexEnd()
 * - faceBegin()
 * - faceEnd()
 * To allow view iteration over vertices, the Mesh could expose a vertices()
 * member function that returns a View object that is constructed in this way:
 *
 * @code {.cpp}
 * auto vertices() { return View{vertexBegin(), vertexEnd()}; }
 * @endcode
 *
 * @ingroup views
 * @ingroup base
 */
template<typename It>
class View : public std::ranges::view_interface<View<It>>
{
public:
    using iterator       = It;
    using const_iterator = It;

    View() = default;

    View(It begin, It end) : mBegin(begin), mEnd(end) {}

    auto begin() const { return mBegin; }

    auto end() const { return mEnd; }

protected:
    It mBegin, mEnd;
};

} // namespace vcl

#endif // VCL_BASE_VIEWS_VIEW_H
