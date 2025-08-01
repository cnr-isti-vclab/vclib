#ifndef VCL_SPACE_COMPLEX_SAMPLER_VERTEX_SAMPLER_H
#define VCL_SPACE_COMPLEX_SAMPLER_VERTEX_SAMPLER_H

#include <vclib/mesh.h>

namespace vcl {

template<VertexConcept Vertex, bool CNST = true>
class VertexSampler
{
    using VertexType = std::conditional_t<CNST, const Vertex, Vertex>;

    std::vector<VertexType*> mSamples;

public:
    using PointType = VertexType::PositionType;

    VertexSampler() {}

    const std::vector<VertexType*> samples() const { return mSamples; }

    const typename VertexType::PositionType& sample(uint i) const
    {
        return mSamples[i]->position();
    }

    std::size_t size() const { return mSamples.size(); }

    void clear() { mSamples.clear(); }

    void reserve(uint n) { mSamples.reserve(n); }

    void resize(uint n) { mSamples.resize(n); }

    void add(VertexType& v) { mSamples.push_back(&v); }

    void set(uint i, VertexType& v) { mSamples[i] = &v; }

    auto begin() const { return std::begin(mSamples | views::positions); }

    auto end() const { return std::end(mSamples | views::positions); }
};

template<VertexConcept VertexType>
using ConstVertexSampler = VertexSampler<VertexType, true>;

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_SAMPLER_VERTEX_SAMPLER_H
