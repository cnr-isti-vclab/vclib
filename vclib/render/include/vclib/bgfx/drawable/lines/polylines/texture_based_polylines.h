#pragma once
#include "../drawable_polylines.h"

namespace vcl {

namespace lines {
class TextureBasedPolylines : public Polylines
{
public:
    TextureBasedPolylines(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth,
        const uint32_t            maxTextureSize);

    ~TextureBasedPolylines();

    std::shared_ptr<DrawableObjectI> clone() const override
    {
        return std::make_shared<TextureBasedPolylines>(*this);
    }

    void draw(uint viewId) const override;

    void update(const std::vector<Point>& points) override;

private:
    void generateIndirectBuffers();

    void generateTextureBuffer();

    void allocateTextureBuffer();

    void allocatePointsBuffer();

    std::vector<float>    m_Vertices;
    std::vector<uint32_t> m_Indices;

    bgfx::VertexBufferHandle m_Vbh;
    bgfx::IndexBufferHandle  m_Ibh;

    bgfx::IndirectBufferHandle m_SegmentsIndirectBuffer;
    bgfx::IndirectBufferHandle m_JoinsIndirectBuffer;

    bgfx::ProgramHandle m_JoinsProgram;
    bgfx::ProgramHandle m_ComputeIndirect;
    bgfx::UniformHandle m_IndirectDataUniform;

    bgfx::TextureHandle             m_TextureBuffer;
    bgfx::DynamicVertexBufferHandle m_PointsBuffer;
    bgfx::ProgramHandle             m_ComputeTexture;

    uint32_t m_PointsSize;
    uint32_t m_MaxTextureSize;
};
} // namespace lines
} // namespace vcl
