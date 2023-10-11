#ifndef VCLIB_SPACE_BIT_SET_BIT_PROXY_H
#define VCLIB_SPACE_BIT_SET_BIT_PROXY_H

#include <concepts>
#include <functional>

#include <vclib/types.h>

namespace vcl {

// See: https://stackoverflow.com/a/10145050/5851101

/**
 * @brief The BitProxy class allows to access to a bool reference from a bit
 * saved in a mask, and then allow assignment.
 *
 * @ingroup space
 */
template<std::integral T>
class BitProxy
{
    std::reference_wrapper<T> mask;
    const uint index;

public:
    /**
     * @brief Constructs the BitProxy with the given mask and index.
     * @param[in] mask: the mask containing the bit to access.
     * @param[in] index: the index of the bit to access.
     */
    BitProxy(T& mask, uint index) : mask(mask), index(index) {}

    operator bool() const { return mask.get() & (1 << index); }

    void operator=(bool bit)
    {
        mask.get() = (mask.get() & ~(bit << index)) | (bit << index);
    }

    BitProxy& operator|=(bool bit)
    {
        mask.get() |= (bit << index);
        return *this;
    }

    BitProxy& operator&=(bool bit)
    {
        mask.get() &= ~(bit << index);
        return *this;
    }

    BitProxy& operator/=(bool bit)
    {
        mask.get() ^= (bit << index);
        return *this;
    }
};

} // namespace vcl

#endif // VCLIB_SPACE_BIT_SET_BIT_PROXY_H
