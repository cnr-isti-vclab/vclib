#include "vertex_sampler.h"

namespace vcl::internal {

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

} // namespace vcl::internal
