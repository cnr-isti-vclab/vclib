#include "bit_proxy.h"

namespace vcl {

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
