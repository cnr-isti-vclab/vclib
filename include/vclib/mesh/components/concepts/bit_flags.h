#ifndef VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H

#include <vclib/misc/types.h>

namespace vcl::comp {

class PolygonBitFlags;
class TriangleBitFlags;

/**
 * @brief HasBitFlags concept
 *
 * Checks if a class has a member function that 'isDeleted()' which returns a bool
 */
template<typename T>
concept HasBitFlags = requires(T v) // requires that an object of type T has the following members
{
	{ v.isDeleted() } -> std::same_as<bool>;
};

template<typename T>
concept HasPolygonBitFlags = std::derived_from<T, PolygonBitFlags>;

template<typename T>
concept HasTriangleBitFlags = std::derived_from<T, TriangleBitFlags>;

/* Detector function to check if a class has BitFlags, PolygonBitFlags or TriangleBitFlags */

template<typename T>
bool constexpr hasBitFlags()
{
	return HasBitFlags<T>;
}

template<typename T>
bool constexpr hasPolygonBitFlags()
{
	return HasPolygonBitFlags<T>;
}

template<typename T>
bool constexpr hasTriangleBitFlags()
{
	return HasTriangleBitFlags<T>;
}

template<typename T>
bool constexpr hasFaceBitFlags()
{
	return HasPolygonBitFlags<T> || HasTriangleBitFlags<T>;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
