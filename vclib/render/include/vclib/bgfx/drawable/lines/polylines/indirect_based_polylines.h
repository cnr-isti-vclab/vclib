#pragma once
#include "../drawable_polylines.h"

namespace vcl {
namespace lines {
class IndirectBasedPolylines : public Polylines
{
public:
    IndirectBasedPolylines(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth);

    ~IndirectBasedPolylines();

    std::shared_ptr<DrawableObjectI> clone() const override
    {
        return std::make_shared<IndirectBasedPolylines>(*this);
    }

    void draw(uint viewId) const override;

    void update(const std::vector<Point>& points) override;

private:
    void allocatePointsBuffer();

    void generateIndirectBuffers();

    std::vector<float>    m_Vertices;
    std::vector<uint32_t> m_Indices;

    bgfx::VertexBufferHandle m_Vbh;
    bgfx::IndexBufferHandle  m_Ibh;

    bgfx::DynamicVertexBufferHandle m_PointsBuffer;

    bgfx::IndirectBufferHandle m_SegmentsIndirectBuffer;
    bgfx::IndirectBufferHandle m_JoinsIndirectBuffer;

    bgfx::ProgramHandle m_JoinsProgram;
    bgfx::ProgramHandle m_ComputeIndirect;
    bgfx::UniformHandle m_IndirectDataUniform;

    uint32_t m_PointsSize;
};

} // namespace lines
} // namespace vcl
