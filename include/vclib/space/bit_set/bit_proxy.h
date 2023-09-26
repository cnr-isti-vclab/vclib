#ifndef VCLIB_SPACE_BIT_SET_BIT_PROXY_H
#define VCLIB_SPACE_BIT_SET_BIT_PROXY_H

#include <concepts>
#include <functional>

#include <vclib/types.h>

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief The BitProxy class allows to access to a bool reference from a bit saved in a mask,
 * and then allow assignment.
 *
 * See: https://stackoverflow.com/a/10145050/5851101
 *
 * @ingroup space
 */
template<std::integral T>
class BitProxy
{
public:
	BitProxy(T& mask, uint index);

	operator bool() const;

	void operator=(bool bit);

	BitProxy& operator|=(bool bit);

	BitProxy& operator&=(bool bit);

	BitProxy& operator/=(bool bit);

private:
	std::reference_wrapper<T> mask;
	const uint index;
};

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @brief Constructs the BitProxy with the given mask and index.
 * @param[in] mask: the mask containing the bit to access.
 * @param[in] index: the index of the bit to access.
 */
template<std::integral T>
BitProxy<T>::BitProxy(T& mask, uint index) : mask(mask), index(index)
{
}

template<std::integral T>
BitProxy<T>::operator bool() const
{
	return mask.get() & (1 << index);
}

template<std::integral T>
void BitProxy<T>::operator=(bool bit)
{
	mask.get() = (mask.get() & ~(bit << index)) | (bit << index);
}

template<std::integral T>
BitProxy<T>& BitProxy<T>::operator|=(bool bit)
{
	mask.get() |= (bit << index);
	return *this;
}

template<std::integral T>
BitProxy<T>& BitProxy<T>::operator&=(bool bit)
{
	mask.get() &= ~(bit << index);
	return *this;
}

template<std::integral T>
BitProxy<T>& BitProxy<T>::operator/=(bool bit)
{
	mask.get() ^= (bit << index);
	return *this;
}

} // namespace vcl

#endif // VCLIB_SPACE_BIT_SET_BIT_PROXY_H
