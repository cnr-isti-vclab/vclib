#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class CPUGeneratedPolylines : public DrawablePolylines {

        public:
            CPUGeneratedPolylines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth);

            ~CPUGeneratedPolylines();

            std::shared_ptr<DrawableObjectI> clone() const override {
                return std::make_shared<CPUGeneratedPolylines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateBuffers(const std::vector<LinesVertex> points);

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_SegmentsIndices;
            std::vector<uint32_t> m_JoinsIndices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_SegmentsIbh;
            bgfx::IndexBufferHandle m_JoinsIbh;

    };
}