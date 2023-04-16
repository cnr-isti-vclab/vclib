#ifndef VCLIB_CONCEPTS_RANGE_H
#define VCLIB_CONCEPTS_RANGE_H

#ifndef VCLIB_USES_RANGES
#include <__ranges/access.h>
#endif

#include <ranges>

namespace vcl {

#ifdef VCLIB_USES_RANGES

template<typename T>
concept Range = std::ranges::range<T>;

#else

template<typename T>
concept Range = requires(T o)
{
	std::ranges::begin(o);
	std::ranges::end(o);
};

#endif

}

#endif // VCLIB_CONCEPTS_RANGE_H
