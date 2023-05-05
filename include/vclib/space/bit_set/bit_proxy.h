#ifndef VCLIB_SPACE_BIT_SET_BIT_PROXY_H
#define VCLIB_SPACE_BIT_SET_BIT_PROXY_H

#include <concepts>
#include <functional>

#include <vclib/types.h>

namespace vcl {

/**
 * @brief The BitProxy class allows to access to a bool reference from a bit saved in a mask,
 * and then allow assignment.
 *
 * See: https://stackoverflow.com/a/10145050/5851101
 */
template<std::integral T>
class BitProxy
{
public:
	BitProxy(T& mask, uint index);

	void setIndex(uint ind);

	operator bool() const;

	void operator=(bool bit);

	BitProxy& operator|=(bool bit);

	BitProxy& operator&=(bool bit);

	BitProxy& operator/=(bool bit);

private:
	std::reference_wrapper<T> mask;
	uint index;
};

} // namespace vcl

#include "bit_proxy.cpp"

#endif // VCLIB_SPACE_BIT_SET_BIT_PROXY_H
