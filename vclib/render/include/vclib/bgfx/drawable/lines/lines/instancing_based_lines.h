#pragma once 
#include <vclib/bgfx/drawable/lines/drawable_lines.h>

namespace vcl::lines {
    class InstancingBasedLines : public DrawableLines {

        std::vector<float>          mVertices = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
        std::vector<uint32_t>       mIndexes = { 0, 1, 2, 1, 3, 2 };
        std::vector<LinesVertex>    mPoints;

        bgfx::VertexBufferHandle    mVerticesBH;
        bgfx::IndexBufferHandle     mIndexesBH;
        bgfx::InstanceDataBuffer    mInstanceDB;

        public:
            InstancingBasedLines() = default;

            InstancingBasedLines(const std::vector<LinesVertex> &points);

            InstancingBasedLines(const InstancingBasedLines& other);

            InstancingBasedLines(InstancingBasedLines&& other);

            ~InstancingBasedLines();

            InstancingBasedLines& operator=(InstancingBasedLines other);

            void swap(InstancingBasedLines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateInstanceDataBuffer();

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();
    }; 
}