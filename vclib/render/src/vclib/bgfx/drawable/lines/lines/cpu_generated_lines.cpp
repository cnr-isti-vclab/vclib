#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>
#include <vclib/bgfx/context/load_program.h>


namespace vcl::lines {
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<LinesVertex> &points) :
        mPointsSize(points.size())
    {
        allocateVertexBuffer();
        allocateIndexBuffer();
        generateBuffers(points);
    }

    CPUGeneratedLines::CPUGeneratedLines(const CPUGeneratedLines& other) : DrawableLines(other) {
        mPointsSize = other.mPointsSize;
        mIndexes = other.mIndexes;
        mVertices = other.mVertices;

        allocateIndexBuffer();
        bgfx::update(mIndexesBH, 0, bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()));

        allocateVertexBuffer();
        bgfx::update(mVerticesBH, 0, bgfx::makeRef(&mVertices[0], sizeof(float) * mVertices.size()));
    }

    CPUGeneratedLines::CPUGeneratedLines(CPUGeneratedLines&& other) : DrawableLines(other) {
        swap(other);
    }

    CPUGeneratedLines::~CPUGeneratedLines() {
        if(bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if(bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);
    }

    CPUGeneratedLines& CPUGeneratedLines::operator=(CPUGeneratedLines other) {
        swap(other);
        return *this;
    }

    void CPUGeneratedLines::swap(CPUGeneratedLines& other) {
        std::swap(mSettings, other.mSettings);
        std::swap(mVisible, other.mVisible);
        
        std::swap(mPointsSize, other.mPointsSize);
        
        std::swap(mIndexesBH, other.mIndexesBH);
        std::swap(mVerticesBH, other.mVerticesBH);

        std::swap(mVertices, other.mVertices);
        std::swap(mIndexes, other.mIndexes);
    }

    std::shared_ptr<vcl::DrawableObject> CPUGeneratedLines::clone() const {
        return std::make_shared<CPUGeneratedLines>(*this);
    }

    void CPUGeneratedLines::update(const std::vector<LinesVertex> &points) {
        mVertices.clear();
        mIndexes.clear();

        if(mPointsSize > points.size()) {
            bgfx::destroy(mVerticesBH);
            bgfx::destroy(mIndexesBH);

            allocateVertexBuffer();
            allocateIndexBuffer();
        }

        mPointsSize = points.size();
        generateBuffers(points);
    }

    void CPUGeneratedLines::draw(uint viewId) const {
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
        bgfx::setState(state);
        bgfx::submit(viewId, mLinesPH);
    }

    void CPUGeneratedLines::generateBuffers(const std::vector<LinesVertex> points) {
        for(uint i = 1; i < points.size(); i+=2) {

            for(uint k = 0; k < 2; k++) {

                for(uint j = 0; j < 2; j++) {

                    mVertices.push_back(points[i - 1].X);
                    mVertices.push_back(points[i - 1].Y);
                    mVertices.push_back(points[i - 1].Z);
                    
                    mVertices.push_back(points[i].X);
                    mVertices.push_back(points[i].Y);
                    mVertices.push_back(points[i].Z);

                    mVertices.push_back(points[i - (1 - k)].getReverseColor());

                    mVertices.push_back(points[i - (1 - k)].xN);
                    mVertices.push_back(points[i - (1 - k)].yN);
                    mVertices.push_back(points[i - (1 - k)].zN);

                    mVertices.push_back(k);
                    mVertices.push_back(j);
                }
            }

            uint32_t index = (4 * (i / 2));
            mIndexes.push_back(index);
            mIndexes.push_back(index + 3);
            mIndexes.push_back(index + 1);

            mIndexes.push_back(index);
            mIndexes.push_back(index + 2);
            mIndexes.push_back(index + 3);
        }

        bgfx::update(mVerticesBH, 0, bgfx::makeRef(&mVertices[0], sizeof(float) * mVertices.size()));
        bgfx::update(mIndexesBH, 0, bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()));
    }

    void CPUGeneratedLines::allocateVertexBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
         .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
         .end();

        mVerticesBH = bgfx::createDynamicVertexBuffer(
            (mPointsSize / 2) * 4, layout, 
            BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void CPUGeneratedLines::allocateIndexBuffer() {
        mIndexesBH = bgfx::createDynamicIndexBuffer(
            (mPointsSize / 2) * 6, 
            BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32
        );
    }
}