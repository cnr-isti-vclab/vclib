#ifndef VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
#define VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H

#include <vclib/concept/space/sampler.h>
#include <vclib/mesh/iterator.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

namespace internal {

template<VertexConcept VertexType, bool CNST = false>
class VertexSampler
{
	using VP = typename std::conditional_t<CNST, const VertexType*, VertexType*>;
	using VPar = typename std::conditional_t<CNST, const VertexType&, VertexType&>;

public:
	using PointType = typename VertexType::CoordType;
	using ConstIterator = CoordIterator<typename std::vector<VP>::const_iterator>;

	VertexSampler();

	const std::vector<VP> samples() const;

	const typename VertexType::CoordType& sample(uint i) const;

	std::size_t size() const;

	void clear();
	void reserve(uint n);
	void resize(uint n);

	void add(VPar v);
	void set(uint i, VPar v);

	ConstIterator begin() const;
	ConstIterator end() const;

private:
	std::vector<VP> samplesVec;
};

} // namespace vcl::internal

template<VertexConcept VertexType>
using VertexSampler = internal::VertexSampler<VertexType, false>;

template<VertexConcept VertexType>
using ConstVertexSampler = internal::VertexSampler<VertexType, true>;

} // namespace vcl

#include "vertex_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
