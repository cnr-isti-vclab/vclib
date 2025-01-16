#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class InstancingBasedPolylines : public DrawablePolylines {

        std::vector<float>          mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        std::vector<uint32_t>       mIndexes = {0, 3, 1, 0, 2, 3};

        bgfx::InstanceDataBuffer    mSegmentsInstanceDB;
        bgfx::InstanceDataBuffer    mJoinsInstanceDB;

        bgfx::VertexBufferHandle    mVerticesBH;
        bgfx::IndexBufferHandle     mIndexesBH;

        bgfx::ProgramHandle         mJoinesPH;

        public:
            InstancingBasedPolylines() = default;

            InstancingBasedPolylines(const std::vector<LinesVertex> &points);

            InstancingBasedPolylines(const InstancingBasedPolylines& other);

            InstancingBasedPolylines(InstancingBasedPolylines&& other);

            ~InstancingBasedPolylines();

            InstancingBasedPolylines& operator=(InstancingBasedPolylines other);

            void swap(InstancingBasedPolylines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateInstanceBuffer(const std::vector<LinesVertex> &points);

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();
    };
}