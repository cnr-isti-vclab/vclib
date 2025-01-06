#pragma once
#include "../drawable_polylines.h"

namespace vcl {
namespace lines {

class GPUGeneratedPolylines : public Polylines
{
public:
    GPUGeneratedPolylines(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth);

    ~GPUGeneratedPolylines();

    std::shared_ptr<DrawableObjectI> clone() const override
    {
        return std::make_shared<GPUGeneratedPolylines>(*this);
    }

    void draw(uint viewId) const override;

    void update(const std::vector<Point>& points) override;

private:
    void generateBuffers();

    void allocateVertexBuffer();

    void allocateIndexBuffer();

    void allocatePointsBuffer();

    bgfx::DynamicVertexBufferHandle m_DVbh;
    bgfx::DynamicIndexBufferHandle  m_SegmentsDIbh;
    bgfx::DynamicIndexBufferHandle  m_JoinsDIbh;

    bgfx::DynamicVertexBufferHandle m_PointsBuffer;

    bgfx::ProgramHandle m_ComputeProgram;
    bgfx::UniformHandle m_NumWorksGroupUniform;

    uint32_t m_PointsSize;
};

} // namespace lines
} // namespace vcl
