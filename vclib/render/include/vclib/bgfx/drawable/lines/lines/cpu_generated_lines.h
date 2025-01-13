#pragma once 
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {

    class CPUGeneratedLines : public DrawableLines {

        public:
            CPUGeneratedLines() = default;

            CPUGeneratedLines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth);

            ~CPUGeneratedLines();

            std::shared_ptr<DrawableObjectI> clone() const override {
                return std::make_shared<CPUGeneratedLines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateBuffers(const std::vector<LinesVertex> points);

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::DynamicVertexBufferHandle m_Vbh;
            bgfx::DynamicIndexBufferHandle m_Ibh;
            uint32_t m_PointsSize;

    };
}