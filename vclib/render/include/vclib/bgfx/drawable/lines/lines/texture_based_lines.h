#pragma once
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {
    class TextureBasedLines : public DrawableLines {
        public:
            TextureBasedLines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth, const uint32_t maxTextureSize);

            ~TextureBasedLines();

            std::shared_ptr<DrawableObjectI> clone() const override {
                return std::make_shared<TextureBasedLines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateTextureBuffer();

            void allocateTextureBuffer();

            void allocatePointsBuffer();

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::TextureHandle m_TextureBuffer;
            bgfx::DynamicVertexBufferHandle m_PointsBuffer;
            bgfx::ProgramHandle m_ComputeTexture;

            bgfx::IndirectBufferHandle m_IndirectBuffer;      
            bgfx::UniformHandle m_IndirectDataUniform;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

            uint32_t m_PointsSize;
            uint32_t m_MaxTextureSize;
    };
}