#include <vclib/bgfx/drawable/lines/polylines/indirect_based_polylines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines {
    IndirectBasedPolylines::IndirectBasedPolylines(const std::vector<LinesVertex> &points) :
        DrawablePolylines("polylines/indirect_based_polylines/vs_indirect_based_segments", "polylines/indirect_based_polylines/fs_indirect_based_polylines"),
        mPoints(points),
        mJoinesIndirectBH(bgfx::createIndirectBuffer(1)),
        mSegmentsIndirectBH(bgfx::createIndirectBuffer(1)),
        mComputeIndirectDataUH(bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4)),
        mComputeIndirectPH(bgfx::createProgram(vcl::loadShader("polylines/indirect_based_polylines/cs_compute_indirect"), true)),
        mJoinesPH(vcl::loadProgram("polylines/indirect_based_polylines/vs_indirect_based_joins", "polylines/indirect_based_polylines/fs_indirect_based_polylines"))
    {
        allocateIndexesBuffers();
        allocateVerticesBuffer();
        generateIndirectBuffers();

        allocatePointsBuffer();
        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }

    IndirectBasedPolylines::IndirectBasedPolylines(const IndirectBasedPolylines& other) : DrawablePolylines(other) {
        mPoints = other.mPoints;
        mJoinesIndirectBH = bgfx::createIndirectBuffer(1);
        mSegmentsIndirectBH = bgfx::createIndirectBuffer(1);
        mComputeIndirectDataUH = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        mComputeIndirectPH = bgfx::createProgram(vcl::loadShader("polylines/indirect_based_polylines/cs_compute_indirect"), true);
        mJoinesPH = vcl::loadProgram("polylines/indirect_based_polylines/vs_indirect_based_joins", "polylines/indirect_based_polylines/fs_indirect_based_polylines");

        allocateIndexesBuffers();
        allocateVerticesBuffer();
        generateIndirectBuffers();

        allocatePointsBuffer();
        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }

    IndirectBasedPolylines::IndirectBasedPolylines(IndirectBasedPolylines&& other) : DrawablePolylines(other) {
        swap(other);
    }

    IndirectBasedPolylines::~IndirectBasedPolylines() {
        if(bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);
        
        if(bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);

        if(bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);

        if(bgfx::isValid(mSegmentsIndirectBH))
            bgfx::destroy(mSegmentsIndirectBH);

        if(bgfx::isValid(mJoinesIndirectBH))
            bgfx::destroy(mJoinesIndirectBH);

        if(bgfx::isValid(mComputeIndirectPH))
            bgfx::destroy(mComputeIndirectPH);

        if(bgfx::isValid(mComputeIndirectDataUH))
            bgfx::destroy(mComputeIndirectDataUH);
    }

    IndirectBasedPolylines& IndirectBasedPolylines::operator=(IndirectBasedPolylines other) {
        swap(other);
        return *this;
    }

    void IndirectBasedPolylines::swap(IndirectBasedPolylines& other) {
        std::swap(mPoints, other.mPoints);
        
        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mIndexesBH, other.mIndexesBH);
        std::swap(mPointsBH, other.mPointsBH);
        std::swap(mSegmentsIndirectBH, other.mSegmentsIndirectBH);
        std::swap(mJoinesIndirectBH, other.mJoinesIndirectBH);

        std::swap(mJoinesPH, other.mJoinesPH);
        std::swap(mComputeIndirectPH, other.mComputeIndirectPH);
        std::swap(mComputeIndirectDataUH, other.mComputeIndirectDataUH);
    }

    std::shared_ptr<vcl::DrawableObjectI> IndirectBasedPolylines::clone() const {
        return std::make_shared<IndirectBasedPolylines>(*this);
    }

    void IndirectBasedPolylines::draw(uint viewId) const {
        mSettings.bindUniformPolylines();

        float indirectData[] = {static_cast<float>(mPoints.size() - 1), 0, 0, 0};
        bgfx::setUniform(mComputeIndirectDataUH, indirectData);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setBuffer(1, mPointsBH, bgfx::Access::Read);
        bgfx::setState(state);
        bgfx::submit(viewId, mLinesPH, mSegmentsIndirectBH, 0);

        if(mSettings.getJoin() != 0) {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mIndexesBH);
            bgfx::setBuffer(1, mPointsBH, bgfx::Access::Read);
            bgfx::setState(state);
            bgfx::submit(viewId, mJoinesPH, mJoinesIndirectBH, 0);
        }
    }

    void IndirectBasedPolylines::update(const std::vector<LinesVertex> &points) {
        int oldSize = mPoints.size();
        mPoints = points;
        
        if(oldSize != mPoints.size()) {
            generateIndirectBuffers();
        }

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }

    void IndirectBasedPolylines::generateIndirectBuffers() {
        float data[] = {static_cast<float>(mPoints.size()), 0, 0, 0};
        bgfx::setUniform(mComputeIndirectDataUH, data);

		bgfx::setBuffer(0, mSegmentsIndirectBH, bgfx::Access::Write);
		bgfx::setBuffer(1, mJoinesIndirectBH, bgfx::Access::Write);
		bgfx::dispatch(0, mComputeIndirectPH);
    }

    void IndirectBasedPolylines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8)
         .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
         .end();

        mPointsBH = bgfx::createDynamicVertexBuffer(mPoints.size(), layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void IndirectBasedPolylines::allocateVerticesBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
         .end();

        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(&mVertices[0], sizeof(float) * mVertices.size()),
            layout
        );
    }

    void IndirectBasedPolylines::allocateIndexesBuffers() {
        mIndexesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()),
            BGFX_BUFFER_INDEX32
        );
    }
}