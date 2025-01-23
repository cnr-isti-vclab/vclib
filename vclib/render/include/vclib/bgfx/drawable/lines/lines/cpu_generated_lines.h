#pragma once 
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {

    class CPUGeneratedLines : public DrawableLines {
        
        bgfx::ProgramHandle mLinesPH = Context::instance().programManager().getProgram(
                                            VclProgram::LINES_CPU_GENERATED_VSFS);

        uint32_t mPointsSize;

        std::vector<float>                  mVertices;
        std::vector<uint32_t>               mIndexes;

        bgfx::DynamicVertexBufferHandle     mVerticesBH     = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle      mIndexesBH      = BGFX_INVALID_HANDLE;

        public:
            CPUGeneratedLines() = default;

            CPUGeneratedLines(const std::vector<LinesVertex> &points);

            CPUGeneratedLines(const CPUGeneratedLines& other);

            CPUGeneratedLines(CPUGeneratedLines&& other);

            ~CPUGeneratedLines();

            CPUGeneratedLines& operator=(CPUGeneratedLines other);

            void swap(CPUGeneratedLines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateBuffers(const std::vector<LinesVertex> points);

            void allocateVertexBuffer();

            void allocateIndexBuffer();
    };
}