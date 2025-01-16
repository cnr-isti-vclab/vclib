#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class IndirectBasedPolylines : public DrawablePolylines {

        std::vector<LinesVertex>            mPoints;
        std::vector<float>                  mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        std::vector<uint32_t>               mIndexes = {0, 3, 1, 0, 2, 3};

        bgfx::VertexBufferHandle            mVerticesBH;
        bgfx::IndexBufferHandle             mIndexesBH;
        bgfx::DynamicVertexBufferHandle     mPointsBH;
        bgfx::IndirectBufferHandle          mSegmentsIndirectBH;
        bgfx::IndirectBufferHandle          mJoinesIndirectBH;

        bgfx::ProgramHandle                 mJoinesPH;
        bgfx::ProgramHandle                 mComputeIndirectPH;            
        bgfx::UniformHandle                 mComputeIndirectDataUH;

        public:
            IndirectBasedPolylines() = default;

            IndirectBasedPolylines(const std::vector<LinesVertex> &points);

            IndirectBasedPolylines(const IndirectBasedPolylines& other);

            IndirectBasedPolylines(IndirectBasedPolylines&& other);

            ~IndirectBasedPolylines();

            IndirectBasedPolylines& operator=(IndirectBasedPolylines other);

            void swap(IndirectBasedPolylines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateIndirectBuffers();

            void allocatePointsBuffer();

            void allocateVerticesBuffer();

            void allocateIndexesBuffers();
    };
}
