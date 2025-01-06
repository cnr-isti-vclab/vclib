#pragma once
#include "../drawable_lines.h"

namespace vcl {
namespace lines {

class CPUGeneratedLines : public Lines
{
public:
    CPUGeneratedLines(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth);

    ~CPUGeneratedLines();

    std::shared_ptr<DrawableObjectI> clone() const override
    {
        return std::make_shared<CPUGeneratedLines>(*this);
    }

    void draw(uint viewId) const override;

    void update(const std::vector<Point>& points) override;

private:
    void generateBuffers(const std::vector<Point> points);

    void allocateVertexBuffer();

    void allocateIndexBuffer();

    std::vector<float>    m_Vertices;
    std::vector<uint32_t> m_Indices;

    bgfx::DynamicVertexBufferHandle m_Vbh;
    bgfx::DynamicIndexBufferHandle  m_Ibh;
    uint32_t                        m_PointsSize;
};
} // namespace lines
} // namespace vcl
