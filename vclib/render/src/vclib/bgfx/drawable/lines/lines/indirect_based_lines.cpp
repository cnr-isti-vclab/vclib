#include <vclib/bgfx/drawable/lines/lines/indirect_based_lines.h>
#include <vclib/bgfx/context/load_program.h>


namespace vcl::lines {
    IndirectBasedLines::IndirectBasedLines(const std::vector<LinesVertex> &points) :
        mPoints(points),
        mIndirectBH(bgfx::createIndirectBuffer(1)),
        mIndirectDataUH(bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
    {
        assert(bgfx::isValid(mComputeIndirectPH));
        allocateVerticesBuffer();
        allocateIndexesBuffer();

        generateIndirectBuffer();
        allocatePointsBuffer();
        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }

    IndirectBasedLines::IndirectBasedLines(const IndirectBasedLines& other) : DrawableLines(other) {
        mPoints = other.mPoints;
        mIndirectBH = bgfx::createIndirectBuffer(1);
        mIndirectDataUH = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);

        assert(bgfx::isValid(mComputeIndirectPH));

        allocateVerticesBuffer();
        allocateIndexesBuffer();
        allocatePointsBuffer();
        generateIndirectBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }

    IndirectBasedLines::IndirectBasedLines(IndirectBasedLines&& other) : DrawableLines(other) {
        swap(other);
    }

    IndirectBasedLines::~IndirectBasedLines() {
        if(bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if(bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);

        if(bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);

        if(bgfx::isValid(mIndirectBH))
            bgfx::destroy(mIndirectBH);

        if(bgfx::isValid(mIndirectDataUH)) 
            bgfx::destroy(mIndirectDataUH);
    }

    IndirectBasedLines& IndirectBasedLines::operator=(IndirectBasedLines other) {
        swap(other);
        return *this;
    }

    void IndirectBasedLines::swap(IndirectBasedLines& other) {
        std::swap(mSettings, other.mSettings);
        std::swap(mVisible, other.mVisible);

        std::swap(mPoints, other.mPoints);

        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mIndexesBH, other.mIndexesBH);
        std::swap(mPointsBH, other.mPointsBH);
        
        std::swap(mIndirectBH, other.mIndirectBH);
        std::swap(mIndirectDataUH, other.mIndirectDataUH);
    }

    std::shared_ptr<vcl::DrawableObjectI> IndirectBasedLines::clone() const {
        return std::make_shared<IndirectBasedLines>(*this);
    }

    void IndirectBasedLines::allocatePointsBuffer() {
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

    void IndirectBasedLines::allocateVerticesBuffer() {
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

    void IndirectBasedLines::allocateIndexesBuffer() {
        mIndexesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()),
            BGFX_BUFFER_INDEX32
        );
    }

    void IndirectBasedLines::generateIndirectBuffer() {
        float data[] = {static_cast<float>(mPoints.size() / 2), 0, 0, 0};
        bgfx::setUniform(mIndirectDataUH, data);
		bgfx::setBuffer(0, mIndirectBH, bgfx::Access::Write);
		bgfx::dispatch(0, mComputeIndirectPH);
    }


    void IndirectBasedLines::draw(uint viewId) const {
        mSettings.bindUniformLines();

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
        bgfx::submit(viewId, mLinesPH, mIndirectBH, 0);
    }

    void IndirectBasedLines::update(const std::vector<LinesVertex> &points) {
        int oldSize = mPoints.size();
        mPoints = points;

        if(oldSize != mPoints.size()) {
            generateIndirectBuffer();
        }
         
        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }
}