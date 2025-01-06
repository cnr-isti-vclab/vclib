#pragma once
#include "../drawable_lines.h"

namespace vcl {
namespace lines {

class GPUGeneratedLines : public Lines
{
public:
    GPUGeneratedLines(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth);

    ~GPUGeneratedLines();

    std::shared_ptr<DrawableObjectI> clone() const override
    {
        return std::make_shared<GPUGeneratedLines>(*this);
    };

    void draw(uint viewId) const override;

    void update(const std::vector<Point>& points) override;

private:
    void generateBuffers();

    void allocateVertexBuffer();

    void allocateIndexBuffer();

    void allocatePointsBuffer();

    bgfx::DynamicIndexBufferHandle  m_DIbh;
    bgfx::DynamicVertexBufferHandle m_DVbh;
    bgfx::DynamicVertexBufferHandle m_PointsBuffer;

    bgfx::ProgramHandle m_ComputeProgram;

    uint32_t m_PointsSize;
};

} // namespace lines

} // namespace vcl
