#ifndef VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
#define VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H

#include <vclib/concepts/space/sampler.h>
#include <vclib/iterators/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

namespace detail {

template<VertexConcept VertexType, bool CNST = false>
class VertexSampler
{
    using VP = std::conditional_t<CNST, const VertexType*, VertexType*>;
    using VPar = std::conditional_t<CNST, const VertexType&, VertexType&>;

    std::vector<VP> samplesVec;

public:
    using PointType = VertexType::CoordType;

    VertexSampler() {}

    const std::vector<VP> samples() const { return samplesVec; }

    const typename VertexType::CoordType& sample(uint i) const
    {
        return samplesVec[i]->coord();
    }

    std::size_t size() const { return samplesVec.size(); }

    void clear() { samplesVec.clear(); }

    void reserve(uint n) { samplesVec.reserve(n); }

    void resize(uint n) { samplesVec.resize(n); }

    void add(VPar v) { samplesVec.push_back(&v); }

    void set(uint i, VPar v) { samplesVec[i] = &v; }

    auto begin() const { return std::begin(samplesVec | views::coords); }

    auto end() const { return std::end(samplesVec | views::coords); }
};

} // namespace vcl::detail

template<VertexConcept VertexType>
using VertexSampler = detail::VertexSampler<VertexType, false>;

template<VertexConcept VertexType>
using ConstVertexSampler = detail::VertexSampler<VertexType, true>;

} // namespace vcl

#endif // VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
