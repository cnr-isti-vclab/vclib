#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class InstancingBasedPolylines : public DrawablePolylines {
        
        bgfx::ProgramHandle mJoinesPH = Context::instance().programManager().getProgram(
                                            VclProgram::POLYLINES_INSTANCING_BASED_JOINS_VSFS);

        bgfx::ProgramHandle mLinesPH = Context::instance().programManager().getProgram(
                                            VclProgram::POLYLINES_INSTANCING_BASED_VSFS);


        static const inline std::vector<float>          mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        static const inline std::vector<uint32_t>       mIndexes = {0, 3, 1, 0, 2, 3};

        std::vector<LinesVertex>                        mPoints;

        mutable bgfx::InstanceDataBuffer                mSegmentsInstanceDB;
        mutable bgfx::InstanceDataBuffer                mJoinsInstanceDB;

        bgfx::VertexBufferHandle                        mVerticesBH           = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle                         mIndexesBH            = BGFX_INVALID_HANDLE;

        public:
            InstancingBasedPolylines() = default;

            InstancingBasedPolylines(const std::vector<LinesVertex> &points);

            InstancingBasedPolylines(const InstancingBasedPolylines& other);

            InstancingBasedPolylines(InstancingBasedPolylines&& other);

            ~InstancingBasedPolylines();

            InstancingBasedPolylines& operator=(InstancingBasedPolylines other);

            void swap(InstancingBasedPolylines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateInstanceBuffer() const;

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();
    };
}