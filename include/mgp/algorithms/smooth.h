#ifndef MGP_ALGORITHMS_SMOOTH_H
#define MGP_ALGORITHMS_SMOOTH_H

namespace mgp {

template<typename MeshType>
void vertexCoordLaplacianSmoothing(
	MeshType&    m,
	unsigned int step,
	bool         smoothSelected  = false,
	bool         cotangentWeight = false /*, mgp::CallBackPos *cb = 0*/);
}

#include "smooth.cpp"

#endif // MGP_ALGORITHMS_SMOOTH_H
