#pragma once
#include "../drawable_lines.h"

namespace vcl {
namespace lines {

class InstancingBasedLines : public Lines
{
public:
    InstancingBasedLines(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth);

    ~InstancingBasedLines();

    std::shared_ptr<DrawableObjectI> clone() const override
    {
        return std::make_shared<InstancingBasedLines>(*this);
    }

    void draw(uint viewId) const override;

    void update(const std::vector<Point>& points) override;

private:
    void generateInstanceDataBuffer(const std::vector<Point>& points);

    bgfx::InstanceDataBuffer m_IDBPoints;
    std::vector<float>       m_Vertices;
    std::vector<uint32_t>    m_Indices;

    bgfx::VertexBufferHandle m_Vbh;
    bgfx::IndexBufferHandle  m_Ibh;
};

} // namespace lines
} // namespace vcl
