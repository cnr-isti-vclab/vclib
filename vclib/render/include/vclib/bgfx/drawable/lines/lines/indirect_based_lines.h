#pragma once
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {

    class IndirectBasedLines : public DrawableLines {

        std::vector<float>                  mVertices = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        std::vector<uint32_t>               mIndexes = {0, 1, 2, 1, 3, 2};
        std::vector<LinesVertex>            mPoints;

        bgfx::VertexBufferHandle            mVerticesBH         = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle             mIndexesBH          = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle     mPointsBH           = BGFX_INVALID_HANDLE;

        bgfx::IndirectBufferHandle          mIndirectBH         = BGFX_INVALID_HANDLE;
        bgfx::ProgramHandle                 mComputeIndirectPH  = BGFX_INVALID_HANDLE;            
        bgfx::UniformHandle                 mIndirectDataUH     = BGFX_INVALID_HANDLE;

        bgfx::ProgramHandle mLinesPH = Context::instance().programManager().getProgram(
                                            VclProgram::LINES_INDIRECT_BASED_VSFS);

        public:
            IndirectBasedLines() = default;

            IndirectBasedLines(const std::vector<LinesVertex> &points);

            IndirectBasedLines(const IndirectBasedLines& other);

            IndirectBasedLines(IndirectBasedLines&& other);

            ~IndirectBasedLines();

            IndirectBasedLines& operator=(IndirectBasedLines other);

            void swap(IndirectBasedLines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void allocatePointsBuffer();

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();

            void generateIndirectBuffer();
    };  
}