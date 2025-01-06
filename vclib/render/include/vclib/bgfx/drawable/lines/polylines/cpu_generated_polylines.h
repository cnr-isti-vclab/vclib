#pragma once
#include "../drawable_polylines.h"

namespace vcl {
namespace lines {

class CPUGeneratedPolylines : public Polylines
{
public:
    CPUGeneratedPolylines(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth);

    ~CPUGeneratedPolylines();

    std::shared_ptr<DrawableObjectI> clone() const override
    {
        return std::make_shared<CPUGeneratedPolylines>(*this);
    }

    void draw(uint viewId) const override;

    void update(const std::vector<Point>& points) override;

private:
    void generateBuffers(const std::vector<Point> points);

    std::vector<float>    m_Vertices;
    std::vector<uint32_t> m_SegmentsIndices;
    std::vector<uint32_t> m_JoinsIndices;

    bgfx::VertexBufferHandle m_Vbh;
    bgfx::IndexBufferHandle  m_SegmentsIbh;
    bgfx::IndexBufferHandle  m_JoinsIbh;
};

} // namespace lines
} // namespace vcl
