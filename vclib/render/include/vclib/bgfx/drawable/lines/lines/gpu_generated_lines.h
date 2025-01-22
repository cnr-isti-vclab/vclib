#pragma once
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {
    class GPUGeneratedLines : public DrawableLines {
        
        bgfx::ProgramHandle mComputeVerticesPH = Context::instance().programManager().getProgram(
                                                    VclProgram::LINES_GPU_GENERATED_CS);

        bgfx::ProgramHandle mLinesPH = Context::instance().programManager().getProgram(
                                            VclProgram::LINES_CPU_GENERATED_VSFS);

        std::vector<LinesVertex>            mPoints;

        bgfx::DynamicIndexBufferHandle      mIndexesBH          = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle     mVerticesBH         = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle     mPointsBH           = BGFX_INVALID_HANDLE;

        public:
            GPUGeneratedLines() = default;

            GPUGeneratedLines(const std::vector<LinesVertex> &points);

            GPUGeneratedLines(const GPUGeneratedLines& other);

            GPUGeneratedLines(GPUGeneratedLines&& other);
            
            ~GPUGeneratedLines();

            GPUGeneratedLines& operator=(GPUGeneratedLines other);

            void swap(GPUGeneratedLines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocatePointsBuffer();
    };
}