#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class TextureBasedPolylines : public DrawablePolylines {

        public:
            TextureBasedPolylines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth, const uint32_t maxTextureSize);

            ~TextureBasedPolylines();

            std::shared_ptr<DrawableObjectI> clone() const override {
                return std::make_shared<TextureBasedPolylines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateTextureBuffer();

            void allocateTextureBuffer();

            void allocatePointsBuffer();

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

            bgfx::IndirectBufferHandle m_SegmentsIndirectBuffer;
            bgfx::IndirectBufferHandle m_JoinsIndirectBuffer;

            bgfx::ProgramHandle m_JoinsProgram;      

            bgfx::TextureHandle m_TextureBufferSegments;
            bgfx::TextureHandle m_TextureBufferJoins;
            
            bgfx::DynamicVertexBufferHandle m_PointsBuffer;
            bgfx::ProgramHandle m_ComputeTexture; 
            bgfx::UniformHandle m_ComputeDataUniform;        

            uint32_t m_PointsSize;
            uint32_t m_MaxTextureSize;
    };
}