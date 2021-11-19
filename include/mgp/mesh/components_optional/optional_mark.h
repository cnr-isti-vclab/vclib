/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_MARK_H
#define MGP_MESH_COMPONENTS_OPTIONAL_MARK_H

#include "optional_info.h"

namespace mgp::comp {

template<typename T>
class OptionalMark : public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;
	unsigned int thisId() const { return B::index((T*)this); }

public:
	OptionalMark() {}
	int mark() const { return B::optCont().mark(thisId()); }
	void resetMark() { return B::optCont().mark(thisId()) = 0; }

	template<typename E>
	bool hasSameMark(const E& e) const
	{
		if constexpr (std::is_pointer<E>::value) {
			return e->mark() == B::optCont().mark(thisId());
		}
		else {
			return e.mark() == B::optCont().mark(thisId());
		}
	}

	void incrementMark() { B::optCont().mark(thisId())++; }
	void decrementMark() { B::optCont().mark(thisId())--; }
};

/**
 * Detector to check if a class has (inherits) OptionalColor
 */

template<typename T>
using hasOptionalMarkT = std::is_base_of<OptionalMark<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalMark = typename std::enable_if<hasOptionalMarkT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalMark()
{
	return hasOptionalMarkT<T>::value;
}

}

#endif // MGP_MESH_COMPONENTS_OPTIONAL_MARK_H
