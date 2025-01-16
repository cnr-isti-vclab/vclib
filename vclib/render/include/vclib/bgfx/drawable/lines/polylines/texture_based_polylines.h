#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class TextureBasedPolylines : public DrawablePolylines {

        uint32_t mMaxTextureSize;
        
        std::vector<LinesVertex>            mPoints;
        std::vector<float>                  mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        std::vector<uint32_t>               mIndexes = {0, 3, 1, 0, 2, 3};

        bgfx::VertexBufferHandle            mVerticesBH;
        bgfx::IndexBufferHandle             mIndexesBH;
        bgfx::DynamicVertexBufferHandle     mPointsBH;

        bgfx::IndirectBufferHandle          mSegmentsIndirectBH;
        bgfx::IndirectBufferHandle          mJoinesIndirectBH;

        bgfx::TextureHandle                 mSegmentsTextureBH;
        bgfx::TextureHandle                 mJoinesTextureBH;
            
        bgfx::ProgramHandle                 mJoinesPH;      
        bgfx::ProgramHandle                 mComputeTexturePH; 
        bgfx::UniformHandle                 mComputeDataUH;        

        public:
            TextureBasedPolylines() = default;

            TextureBasedPolylines(const std::vector<LinesVertex> &points, const uint32_t maxTextureSize);

            TextureBasedPolylines(const TextureBasedPolylines& other);

            TextureBasedPolylines(TextureBasedPolylines&& other);

            ~TextureBasedPolylines();

            TextureBasedPolylines& operator=(TextureBasedPolylines other);

            void swap(TextureBasedPolylines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateTextureBuffer();

            void allocateTextureBuffer();

            void allocatePointsBuffer();

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();
    };
}