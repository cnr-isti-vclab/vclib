#ifndef VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
#define VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H

#include <vclib/concepts/mesh/elements/vertex.h>
#include <vclib/concepts/space/sampler.h>
#include <vclib/iterators/mesh.h>
#include <vclib/mesh/views/components.h>

namespace vcl {

namespace detail {

template<VertexConcept VertexType, bool CNST = false>
class VertexSampler
{
    using VP   = std::conditional_t<CNST, const VertexType*, VertexType*>;
    using VPar = std::conditional_t<CNST, const VertexType&, VertexType&>;

    std::vector<VP> mSamples;

public:
    using PointType = VertexType::CoordType;

    VertexSampler() {}

    const std::vector<VP> samples() const { return mSamples; }

    const typename VertexType::CoordType& sample(uint i) const
    {
        return mSamples[i]->coord();
    }

    std::size_t size() const { return mSamples.size(); }

    void clear() { mSamples.clear(); }

    void reserve(uint n) { mSamples.reserve(n); }

    void resize(uint n) { mSamples.resize(n); }

    void add(VPar v) { mSamples.push_back(&v); }

    void set(uint i, VPar v) { mSamples[i] = &v; }

    auto begin() const { return std::begin(mSamples | views::coords); }

    auto end() const { return std::end(mSamples | views::coords); }
};

} // namespace detail

template<VertexConcept VertexType>
using VertexSampler = detail::VertexSampler<VertexType, false>;

template<VertexConcept VertexType>
using ConstVertexSampler = detail::VertexSampler<VertexType, true>;

} // namespace vcl

#endif // VCL_SPACE_SAMPLER_VERTEX_SAMPLER_H
