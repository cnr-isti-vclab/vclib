#include <vclib/bgfx/drawable/lines/lines/instancing_based_lines.h>

namespace vcl::lines {
    InstancingBasedLines::InstancingBasedLines(const std::vector<LinesVertex> &points) :
        mPoints(points)
    {
        allocateVerticesBuffer();
        allocateIndexesBuffer();
    }

    InstancingBasedLines::InstancingBasedLines(const InstancingBasedLines& other) : DrawableLines(other) {
        mPoints = other.mPoints;
        allocateVerticesBuffer();
        allocateIndexesBuffer();
    }

    InstancingBasedLines::InstancingBasedLines(InstancingBasedLines&& other) : DrawableLines(other) {
        swap(other);
    }

    InstancingBasedLines::~InstancingBasedLines() {
        if(bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if(bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);
    }

    InstancingBasedLines& InstancingBasedLines::operator=(InstancingBasedLines other) {
        swap(other);
        return *this;
    }

    void InstancingBasedLines::swap(InstancingBasedLines& other) {
        std::swap(mLinesPH, other.mLinesPH);
        std::swap(mSettings, other.mSettings);
        std::swap(mVisible, other.mVisible);

        std::swap(mPoints, other.mPoints);

        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mIndexesBH, other.mIndexesBH);
        std::swap(mInstanceDB, other.mInstanceDB);
    }

    std::shared_ptr<vcl::DrawableObject> InstancingBasedLines::clone() const {
        return std::make_shared<InstancingBasedLines>(*this);
    }

    void InstancingBasedLines::draw(uint viewId) const {
        generateInstanceDataBuffer();
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
        bgfx::setInstanceDataBuffer(&mInstanceDB);
        
        bgfx::setState(state);
        bgfx::submit(viewId, mLinesPH);
    }

    void InstancingBasedLines::update(const std::vector<LinesVertex> &points) {
        mPoints = points;
    }

    void InstancingBasedLines::generateInstanceDataBuffer() const {
        const uint16_t stride = sizeof(float) * 16;
        uint size = (mPoints.size() / 2);

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer(size, stride);
        bgfx::allocInstanceDataBuffer(&mInstanceDB, size, stride);

        uint8_t* data = mInstanceDB.data;
        for(uint32_t i = 1; i < mPoints.size(); i+=2) {

            float* p0 = reinterpret_cast<float*>(data);
            p0[0] = mPoints[i-1].X;
            p0[1] = mPoints[i-1].Y;
            p0[2] = mPoints[i-1].Z;

            uint32_t* color0 = (uint32_t*)&data[12];
            color0[0] = mPoints[i-1].getUintColor();

            float* p1 = (float*)&data[16];
            p1[0] = mPoints[i].X;
            p1[1] = mPoints[i].Y;
            p1[2] = mPoints[i].Z;

            uint32_t* color1 = (uint32_t*)&data[28];
            color1[0] = mPoints[i].getUintColor();

            float* n0 = (float*)&data[32];
            n0[0] = mPoints[i-1].xN;
            n0[1] = mPoints[i-1].yN;
            n0[2] = mPoints[i-1].zN;
            n0[3] = 0;

            float* n1 = (float*)&data[48];
            n1[0] = mPoints[i].xN;
            n1[1] = mPoints[i].yN;
            n1[2] = mPoints[i].zN;
            n1[3] = 0;

            data += stride;
        }
    }

    void InstancingBasedLines::allocateVerticesBuffer() {
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

    void InstancingBasedLines::allocateIndexesBuffer() {
        mIndexesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()),
            BGFX_BUFFER_INDEX32
        );
    }
}