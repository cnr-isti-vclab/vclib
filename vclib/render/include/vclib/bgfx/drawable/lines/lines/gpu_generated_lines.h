#pragma once
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {
    class GPUGeneratedLines : public DrawableLines {

        public:
            GPUGeneratedLines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth);

            ~GPUGeneratedLines();

            std::shared_ptr<DrawableObjectI> clone() const override {
                return std::make_shared<GPUGeneratedLines>(*this);
            };

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocatePointsBuffer();

            bgfx::DynamicIndexBufferHandle m_DIbh;
            bgfx::DynamicVertexBufferHandle m_DVbh;
            bgfx::DynamicVertexBufferHandle m_PointsBuffer;
            
            bgfx::ProgramHandle m_ComputeProgram;

            uint32_t m_PointsSize;
    };
}