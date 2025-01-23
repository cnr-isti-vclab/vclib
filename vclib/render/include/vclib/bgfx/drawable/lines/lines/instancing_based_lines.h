#pragma once 
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {
    class InstancingBasedLines : public DrawableLines {

        bgfx::ProgramHandle mLinesPH = Context::instance().programManager().getProgram(
                                            VclProgram::LINES_INSTANCING_BASED_VSFS);

        static inline const std::vector<float>          mVertices = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
        static inline const std::vector<uint32_t>       mIndexes = { 0, 1, 2, 1, 3, 2 };

        std::vector<LinesVertex>                        mPoints;
        mutable bgfx::InstanceDataBuffer                mInstanceDB;

        bgfx::VertexBufferHandle                        mVerticesBH     = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle                         mIndexesBH      = BGFX_INVALID_HANDLE;


        public:
            InstancingBasedLines() = default;

            InstancingBasedLines(const std::vector<LinesVertex> &points);

            InstancingBasedLines(const InstancingBasedLines& other);

            InstancingBasedLines(InstancingBasedLines&& other);

            ~InstancingBasedLines();

            InstancingBasedLines& operator=(InstancingBasedLines other);

            void swap(InstancingBasedLines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateInstanceDataBuffer() const;

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();
    }; 
}