#include <vclib/bgfx/drawable/lines/polylines/gpu_generated_polylines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl {
namespace lines { 
    GPUGeneratedPolylines::GPUGeneratedPolylines(const std::vector<Point> &points, const float width, const float heigth) :
        Polylines(width, heigth, "polylines/cpu_generated_polylines/vs_cpu_generated_polylines", "polylines/cpu_generated_polylines/fs_cpu_generated_polylines"),
        m_PointsSize(points.size())
    {
        m_ComputeProgram = bgfx::createProgram(vcl::loadShader("polylines/gpu_generated_polylines/cs_compute_buffers"), true);
        m_NumWorksGroupUniform = bgfx::createUniform("u_numWorksGroups", bgfx::UniformType::Vec4);

        allocatePointsBuffer();
        allocateVertexBuffer();
        allocateIndexBuffer();

        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(Point) * points.size()));
        generateBuffers();  
    }

    GPUGeneratedPolylines::~GPUGeneratedPolylines() {
        bgfx::destroy(m_DVbh);
        bgfx::destroy(m_SegmentsDIbh);
        bgfx::destroy(m_JoinsDIbh);
        bgfx::destroy(m_PointsBuffer);
        bgfx::destroy(m_ComputeProgram);
    }

    void GPUGeneratedPolylines::draw(uint viewId) const {
        float data1[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.miterLimit, m_Data.thickness};
        bgfx::setUniform(m_UniformData1, data1);

        float data2[] = {static_cast<float>(m_Data.leftCap), static_cast<float>(m_Data.rigthCap), static_cast<float>(m_Data.join), 0};
        bgfx::setUniform(m_UniformData2, data2);
        
        bgfx::setUniform(m_UniformColor, &m_Data.color);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_DVbh);
        bgfx::setIndexBuffer(m_SegmentsDIbh);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);

        if(m_Data.join != 0) {
            bgfx::setVertexBuffer(0, m_DVbh);
            bgfx::setIndexBuffer(m_JoinsDIbh);
            bgfx::setState(state);
            bgfx::submit(viewId, m_Program);
        }
    }

    void GPUGeneratedPolylines::update(const std::vector<Point> &points) {
        int oldSize = m_PointsSize;
        m_PointsSize = points.size();

        if(oldSize != m_PointsSize) {
            bgfx::destroy(m_SegmentsDIbh);
            bgfx::destroy(m_JoinsDIbh);
            allocateIndexBuffer();
        }

        if(oldSize < m_PointsSize) {
            bgfx::destroy(m_DVbh);
            allocateVertexBuffer();
        } 

        if(oldSize > m_PointsSize) {
            bgfx::destroy(m_PointsBuffer);
            allocatePointsBuffer();
        }

        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(Point) * points.size()));
        generateBuffers();
    }

    void GPUGeneratedPolylines::generateBuffers() {
        float data[] = {static_cast<float>(m_PointsSize - 1), 0, 0, 0};
        bgfx::setUniform(m_NumWorksGroupUniform, data);

        bgfx::setBuffer(0, m_PointsBuffer, bgfx::Access::Read);
        bgfx::setBuffer(1, m_DVbh, bgfx::Access::Write);
        bgfx::setBuffer(2, m_SegmentsDIbh, bgfx::Access::Write);
        bgfx::setBuffer(3, m_JoinsDIbh, bgfx::Access::Write);
        bgfx::dispatch(0, m_ComputeProgram, m_PointsSize - 1, 1, 1);
    }

    void GPUGeneratedPolylines::allocateVertexBuffer() {

        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();
        
        m_DVbh = bgfx::createDynamicVertexBuffer(
            (m_PointsSize - 1) * 4, layout,
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void GPUGeneratedPolylines::allocateIndexBuffer() {
        m_SegmentsDIbh = bgfx::createDynamicIndexBuffer(
            ((m_PointsSize - 1) * 6), 
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32
        );

        m_JoinsDIbh = bgfx::createDynamicIndexBuffer(
            ((m_PointsSize - 2) * 6), 
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32
        );
    }

    void GPUGeneratedPolylines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
         .end();
        
        m_PointsBuffer = bgfx::createDynamicVertexBuffer(
            m_PointsSize, layout,
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }
}
}