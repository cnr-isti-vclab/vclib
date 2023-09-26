#ifndef VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
#define VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H

#include <vclib/concepts/space/sampler.h>
#include <vclib/iterators/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

namespace internal {

template<VertexConcept VertexType, bool CNST = false>
class VertexSampler
{
	using VP = std::conditional_t<CNST, const VertexType*, VertexType*>;
	using VPar = std::conditional_t<CNST, const VertexType&, VertexType&>;

public:
	using PointType = VertexType::CoordType;

	VertexSampler();

	const std::vector<VP> samples() const;

	const typename VertexType::CoordType& sample(uint i) const;

	std::size_t size() const;

	void clear();
	void reserve(uint n);
	void resize(uint n);

	void add(VPar v);
	void set(uint i, VPar v);

	auto begin() const;
	auto end() const;

private:
	std::vector<VP> samplesVec;
};

} // namespace vcl::internal

template<VertexConcept VertexType>
using VertexSampler = internal::VertexSampler<VertexType, false>;

template<VertexConcept VertexType>
using ConstVertexSampler = internal::VertexSampler<VertexType, true>;

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

namespace internal {

template<VertexConcept VertexType, bool CNST>
VertexSampler<VertexType, CNST>::VertexSampler()
{
}

template<VertexConcept VertexType, bool CNST>
const std::vector<typename VertexSampler<VertexType, CNST>::VP>
VertexSampler<VertexType, CNST>::samples() const
{
	return samplesVec;
}

template<VertexConcept VertexType, bool CNST>
const typename VertexType::CoordType& VertexSampler<VertexType, CNST>::sample(uint i) const
{
	return samplesVec[i]->coord();
}

template<VertexConcept VertexType, bool CNST>
std::size_t vcl::internal::VertexSampler<VertexType, CNST>::size() const
{
	return samplesVec.size();
}

template<VertexConcept VertexType, bool CNST>
void VertexSampler<VertexType, CNST>::clear()
{
	samplesVec.clear();
}

template<VertexConcept VertexType, bool CNST>
void VertexSampler<VertexType, CNST>::reserve(uint n)
{
	samplesVec.reserve(n);
}

template<VertexConcept VertexType, bool CNST>
void VertexSampler<VertexType, CNST>::resize(uint n)
{
	samplesVec.resize(n);
}

template<VertexConcept VertexType, bool CNST>
void VertexSampler<VertexType, CNST>::add(VPar v)
{
	samplesVec.push_back(&v);
}

template<VertexConcept VertexType, bool CNST>
void VertexSampler<VertexType, CNST>::set(uint i, VPar v)
{
	samplesVec[i] = &v;
}

template<VertexConcept VertexType, bool CNST>
auto VertexSampler<VertexType, CNST>::begin() const
{
	return std::begin(samplesVec | views::coords);
}

template<VertexConcept VertexType, bool CNST>
auto VertexSampler<VertexType, CNST>::end() const
{
	return std::end(samplesVec | views::coords);
}

} // namespace vcl::internal

} // namespace vcl

#endif // VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
