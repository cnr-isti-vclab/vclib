#ifndef VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
#define VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H

#include <vclib/concept/space/sampler.h>
#include <vclib/mesh/iterators/vertex_coord_iterator.h>
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
	using ConstIterator = VertexPointerCoordIterator<typename std::vector<VP>::const_iterator>;

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

// makes sure that the VertexSampler satisfies SamplerConcept
static_assert(
	SamplerConcept<VertexSampler<internal::TMPSimplePolyMesh::Vertex>>,
	"VertexSampler does not satisfy the SamplerConcept");
static_assert(
	SamplerConcept<ConstVertexSampler<internal::TMPSimplePolyMesh::Vertex>>,
	"ConstVertexSampler does not satisfy the SamplerConcept");

} // namespace vcl

#endif // VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
