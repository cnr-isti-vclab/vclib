#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class GPUGeneratedPolylines : public DrawablePolylines {

        std::vector<LinesVertex>            mPoints;

        bgfx::DynamicVertexBufferHandle     mPointsBH           = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle     mVertexBH           = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle      mSegmentsIndexesBH  = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle      mJoinesIndexesBH    = BGFX_INVALID_HANDLE;
            
        bgfx::ProgramHandle                 mComputeVertexPH    = BGFX_INVALID_HANDLE;
        bgfx::UniformHandle                 mComputeDataUH      = BGFX_INVALID_HANDLE;

        bgfx::ProgramHandle mLinesPH = Context::instance().programManager().getProgram(
                                            VclProgram::DRAWABLE_CPU_GENERATED_LINES);

        public:
            GPUGeneratedPolylines() = default;

            GPUGeneratedPolylines(const std::vector<LinesVertex> &points);

            GPUGeneratedPolylines(const GPUGeneratedPolylines& other);

            GPUGeneratedPolylines(GPUGeneratedPolylines&& other);

            ~GPUGeneratedPolylines();

            GPUGeneratedPolylines& operator=(GPUGeneratedPolylines other);

            void swap(GPUGeneratedPolylines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocatePointsBuffer();
    };
}