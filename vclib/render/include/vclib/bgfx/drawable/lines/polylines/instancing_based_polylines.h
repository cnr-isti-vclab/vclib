#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class InstancingBasedPolylines : public DrawablePolylines {
        public:
            InstancingBasedPolylines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth);

            ~InstancingBasedPolylines();

            std::shared_ptr<DrawableObjectI> clone() const override {
                return std::make_shared<InstancingBasedPolylines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateInstanceBuffer(const std::vector<LinesVertex> &points);

            bgfx::InstanceDataBuffer m_IDBSegments;
            bgfx::InstanceDataBuffer m_IDBJoins;

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

            bgfx::ProgramHandle m_JoinsProgram;

    };
}