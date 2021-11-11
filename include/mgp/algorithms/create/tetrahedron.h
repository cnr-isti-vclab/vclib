/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#ifndef MGP_ALGORITHMS_CREATE_TETRAHEDRON_H
#define MGP_ALGORITHMS_CREATE_TETRAHEDRON_H

namespace mgp {

template<typename MeshType>
MeshType createTetrahedron();

template<typename MeshType, typename CoordType>
MeshType createTetrahedron(
	const CoordType& p0,
	const CoordType& p1,
	const CoordType& p2,
	const CoordType& p3);

} // namespace mgp

#include "tetrahedron.cpp"

#endif // MGP_ALGORITHMS_CREATE_TETRAHEDRON_H
