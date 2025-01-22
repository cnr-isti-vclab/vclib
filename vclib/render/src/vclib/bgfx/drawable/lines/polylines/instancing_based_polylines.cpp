#include <vclib/bgfx/drawable/lines/polylines/instancing_based_polylines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines {
    InstancingBasedPolylines::InstancingBasedPolylines(const std::vector<LinesVertex> &points) :
        mJoinesPH(Context::instance().programManager().getProgram(VclProgram::POLYLINES_INSTANCING_BASED_JOINS_VSFS))
    {
        allocateVerticesBuffer();
        allocateIndexesBuffer();
        generateInstanceBuffer(points);
    }

    InstancingBasedPolylines::InstancingBasedPolylines(const InstancingBasedPolylines& other) : DrawablePolylines(other) {
        mJoinesPH = Context::instance().programManager().getProgram(VclProgram::POLYLINES_INSTANCING_BASED_JOINS_VSFS);
        allocateVerticesBuffer();
        allocateIndexesBuffer();

        uint32_t linesNumSegments = bgfx::getAvailInstanceDataBuffer(other.mSegmentsInstanceDB.num, other.mSegmentsInstanceDB.stride);
        bgfx::allocInstanceDataBuffer(&mSegmentsInstanceDB, linesNumSegments, other.mSegmentsInstanceDB.stride);

        for(uint32_t i = 0; i < other.mSegmentsInstanceDB.size; i++) {
            mSegmentsInstanceDB.data[i] = other.mSegmentsInstanceDB.data[i];
        }

        uint32_t linesNumJoins = bgfx::getAvailInstanceDataBuffer(other.mJoinsInstanceDB.num, other.mJoinsInstanceDB.stride);
        bgfx::allocInstanceDataBuffer(&mJoinsInstanceDB, linesNumSegments, other.mJoinsInstanceDB.stride);

        for(uint32_t i = 0; i < other.mJoinsInstanceDB.size; i++) {
            mJoinsInstanceDB.data[i] = other.mJoinsInstanceDB.data[i];
        }
    }

    InstancingBasedPolylines::InstancingBasedPolylines(InstancingBasedPolylines&& other) : DrawablePolylines(other) {
        swap(other);
    }

    InstancingBasedPolylines::~InstancingBasedPolylines() {
        if(bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);
        
        if(bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);
    }

    InstancingBasedPolylines& InstancingBasedPolylines::operator=(InstancingBasedPolylines other) {
        swap(other);
        return *this;
    }

    void InstancingBasedPolylines::swap(InstancingBasedPolylines& other) {
        std::swap(mLinesPH, other.mLinesPH);
        std::swap(mSettings, other.mSettings);
        std::swap(mVisible, other.mVisible);
        
        std::swap(mSegmentsInstanceDB, other.mSegmentsInstanceDB);
        std::swap(mJoinsInstanceDB, other.mJoinsInstanceDB);

        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mIndexesBH, other.mIndexesBH);

        std::swap(mJoinesPH, other.mJoinesPH);
    }

    std::shared_ptr<vcl::DrawableObject> InstancingBasedPolylines::clone() const {
        return std::make_shared<InstancingBasedPolylines>(*this);
    }

    void InstancingBasedPolylines::draw(uint viewId) const {
        mSettings.bindUniformPolylines();

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setInstanceDataBuffer(&mSegmentsInstanceDB);
        bgfx::setState(state);
        bgfx::submit(viewId, mLinesPH);

        if(mSettings.getJoin() != 0) {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mIndexesBH);
            bgfx::setInstanceDataBuffer(&mJoinsInstanceDB);
            bgfx::setState(state);
            bgfx::submit(viewId, mJoinesPH);
        }
    }

    void InstancingBasedPolylines::update(const std::vector<LinesVertex> &points) {
        generateInstanceBuffer(points);
    }

    void InstancingBasedPolylines::generateInstanceBuffer(const std::vector<LinesVertex> &points) {
        const uint16_t strideSegments = sizeof(float) * 20;
        uint32_t linesNumSegments = bgfx::getAvailInstanceDataBuffer(points.size() - 1, strideSegments);
        bgfx::allocInstanceDataBuffer(&mSegmentsInstanceDB, linesNumSegments, strideSegments);

        const uint16_t strideJoins = sizeof(float) * 16;
        if(points.size() > 2) {
            uint32_t linesNumJoins = bgfx::getAvailInstanceDataBuffer(points.size() - 2, strideJoins);
            bgfx::allocInstanceDataBuffer(&mJoinsInstanceDB, linesNumJoins, strideJoins);
        } 

        uint8_t* dataSegments = mSegmentsInstanceDB.data;
        uint8_t* dataJoins    = mJoinsInstanceDB.data;

        for(uint32_t i = 0; i < linesNumSegments; i++) {
            float* prevSegments = reinterpret_cast<float*>(dataSegments);
            prevSegments[0] = points[i - !!i].X;
            prevSegments[1] = points[i - !!i].Y;
            prevSegments[2] = points[i - !!i].Z;
            prevSegments[3] = points[i].xN;

            float* currSegments = (float*)&dataSegments[16];
            currSegments[0] = points[i].X;
            currSegments[1] = points[i].Y;
            currSegments[2] = points[i].Z;

            uint32_t* color0 = (uint32_t*)&dataSegments[28];
            color0[0] = points[i].getUintColor();

            float* nextSegments = (float*)&dataSegments[32];
            nextSegments[0] = points[i + 1].X;
            nextSegments[1] = points[i + 1].Y;
            nextSegments[2] = points[i + 1].Z;

            uint32_t* color1 = (uint32_t*)&dataSegments[44];
            color1[0] = points[i + 1].getUintColor();

            float* next_nextSegments = (float*)&dataSegments[48];
            next_nextSegments[0] = points[i + 1 + (!!(linesNumSegments - 1 - i))].X;
            next_nextSegments[1] = points[i + 1 + (!!(linesNumSegments - 1 - i))].Y;
            next_nextSegments[2] = points[i + 1 + (!!(linesNumSegments - 1 - i))].Z;
            next_nextSegments[3] = points[i].yN;

            float* normalSegments = (float*)&dataSegments[64];
            normalSegments[0] = points[i].zN;
            normalSegments[1] = points[i + 1].xN;
            normalSegments[2] = points[i + 1].yN;
            normalSegments[3] = points[i + 1].zN;

            if(i > 0) {
                float* prevJoin = reinterpret_cast<float*>(dataJoins);
                prevJoin[0] = points[i - 1].X;
                prevJoin[1] = points[i - 1].Y;
                prevJoin[2] = points[i - 1].Z;
                prevJoin[3] = 0.0f;

                float* currJoin = (float*)&dataJoins[16];
                currJoin[0] = points[i].X;
                currJoin[1] = points[i].Y;
                currJoin[2] = points[i].Z;

                uint32_t* colorJoin = (uint32_t*)&dataJoins[28];
                colorJoin[0] = points[i].getUintColor();

                float* nextJoin = (float*)&dataJoins[32];
                nextJoin[0] = points[i + 1].X;
                nextJoin[1] = points[i + 1].Y;
                nextJoin[2] = points[i + 1].Z;
                nextJoin[3] = 0.0f;

                float* normalJoin = (float*)&dataJoins[48];
                normalJoin[0] = points[i].xN;
                normalJoin[1] = points[i].yN;
                normalJoin[2] = points[i].zN;
                normalJoin[3] = 0;

                dataJoins+=strideJoins;
            }
        
            dataSegments+=strideSegments;
        }
    }

    void InstancingBasedPolylines::allocateVerticesBuffer() {
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

    void InstancingBasedPolylines::allocateIndexesBuffer() {
        mIndexesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()),
            BGFX_BUFFER_INDEX32
        );
    }
}