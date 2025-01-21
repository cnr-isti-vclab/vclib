#include <vclib/bgfx/drawable/lines/polylines/gpu_generated_polylines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines { 
    GPUGeneratedPolylines::GPUGeneratedPolylines(const std::vector<LinesVertex> &points) :
        mPoints(points),
        mComputeVertexPH(Context::instance().programManager().getProgram(VclProgram::POLYLINES_GPU_GENERATED_CS)),
        mComputeDataUH(bgfx::createUniform("u_numWorksGroups", bgfx::UniformType::Vec4))
    {
        allocatePointsBuffer();
        allocateVertexBuffer();
        allocateIndexBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers();  
    }

    GPUGeneratedPolylines::GPUGeneratedPolylines(const GPUGeneratedPolylines& other) : DrawablePolylines(other) {
        mPoints = other.mPoints;
        mComputeVertexPH = Context::instance().programManager().getProgram(VclProgram::POLYLINES_GPU_GENERATED_CS);
        mComputeDataUH = bgfx::createUniform("u_numWorksGroups", bgfx::UniformType::Vec4);

        allocatePointsBuffer();
        allocateVertexBuffer();
        allocateIndexBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers(); 
    }

    GPUGeneratedPolylines::GPUGeneratedPolylines(GPUGeneratedPolylines&& other) : DrawablePolylines(other) {
        swap(other);
    }

    GPUGeneratedPolylines::~GPUGeneratedPolylines() {
        if(bgfx::isValid(mVertexBH))
            bgfx::destroy(mVertexBH);
        
        if(bgfx::isValid(mSegmentsIndexesBH))
            bgfx::destroy(mSegmentsIndexesBH);

        if(bgfx::isValid(mJoinesIndexesBH))
            bgfx::destroy(mJoinesIndexesBH);

        if(bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);

        if(bgfx::isValid(mComputeVertexPH))
            bgfx::destroy(mComputeVertexPH);

        if(bgfx::isValid(mComputeDataUH))
            bgfx::destroy(mComputeDataUH);
    }

    GPUGeneratedPolylines& GPUGeneratedPolylines::operator=(GPUGeneratedPolylines other) {
        swap(other);
        return *this;
    }
 
    void GPUGeneratedPolylines::swap(GPUGeneratedPolylines& other) {
        std::swap(mLinesPH, other.mLinesPH);
        std::swap(mSettings, other.mSettings);
        std::swap(mVisible, other.mVisible);

        std::swap(mPoints, other.mPoints);

        std::swap(mPointsBH, other.mPointsBH);
        std::swap(mVertexBH, other.mVertexBH);
        std::swap(mSegmentsIndexesBH, other.mSegmentsIndexesBH);
        std::swap(mJoinesIndexesBH, other.mJoinesIndexesBH);

        std::swap(mComputeVertexPH, other.mComputeVertexPH);
        std::swap(mComputeDataUH, other.mComputeDataUH);
    }

    std::shared_ptr<vcl::DrawableObjectI> GPUGeneratedPolylines::clone() const {
        return std::make_shared<GPUGeneratedPolylines>(*this);
    }

    void GPUGeneratedPolylines::draw(uint viewId) const {
        mSettings.bindUniformPolylines();

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, mVertexBH);
        bgfx::setIndexBuffer(mSegmentsIndexesBH);
        bgfx::setState(state);
        bgfx::submit(viewId, mLinesPH);

        if(mSettings.getJoin() != 0) {
            bgfx::setVertexBuffer(0, mVertexBH);
            bgfx::setIndexBuffer(mJoinesIndexesBH);
            bgfx::setState(state);
            bgfx::submit(viewId, mLinesPH);
        }
    }

    void GPUGeneratedPolylines::update(const std::vector<LinesVertex> &points) {
        int oldSize = mPoints.size();
        mPoints = points;

        if(oldSize != mPoints.size()) {
            bgfx::destroy(mSegmentsIndexesBH);
            bgfx::destroy(mJoinesIndexesBH);
            allocateIndexBuffer();
        }

        if(oldSize < mPoints.size()) {
            bgfx::destroy(mVertexBH);
            allocateVertexBuffer();
        } 

        if(oldSize > mPoints.size()) {
            bgfx::destroy(mPointsBH);
            allocatePointsBuffer();
        }

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers();
    }

    void GPUGeneratedPolylines::generateBuffers() {
        float data[] = {static_cast<float>(mPoints.size() - 1), 0, 0, 0};
        bgfx::setUniform(mComputeDataUH, data);

        bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
        bgfx::setBuffer(1, mVertexBH,         bgfx::Access::Write);
        bgfx::setBuffer(2, mSegmentsIndexesBH, bgfx::Access::Write);
        bgfx::setBuffer(3, mJoinesIndexesBH,    bgfx::Access::Write);
        bgfx::dispatch(0, mComputeVertexPH, mPoints.size() - 1, 1, 1);
    }

    void GPUGeneratedPolylines::allocateVertexBuffer() {

        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();
        
        mVertexBH = bgfx::createDynamicVertexBuffer(
            (mPoints.size() - 1) * 4, layout,
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void GPUGeneratedPolylines::allocateIndexBuffer() {
        mSegmentsIndexesBH = bgfx::createDynamicIndexBuffer(
            ((mPoints.size() - 1) * 6), 
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32
        );

        mJoinesIndexesBH = bgfx::createDynamicIndexBuffer(
            ((mPoints.size() - 2) * 6), 
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32
        );
    }

    void GPUGeneratedPolylines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8)
         .end();
        
        mPointsBH = bgfx::createDynamicVertexBuffer(
            mPoints.size(), layout,
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }
}