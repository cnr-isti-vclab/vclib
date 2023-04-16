#ifndef VCLIB_CONCEPTS_RANGE_H
#define VCLIB_CONCEPTS_RANGE_H

#include <ranges>

namespace vcl {

template<typename T>
concept Range = std::ranges::range<T>;

} // namespace vcl

#endif // VCLIB_CONCEPTS_RANGE_H
