#include <vclib/bgfx/drawable/lines/lines/indirect_based_lines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl {
namespace lines {
    IndirectBasedLines::IndirectBasedLines(const std::vector<Point> &points, const float width, const float heigth) :
        Lines(width, heigth, "lines/indirect_based_lines/vs_indirect_based_lines", "lines/indirect_based_lines/fs_indirect_based_lines"),
        m_PointsSize(points.size())
    {
        m_IndirectBuffer = bgfx::createIndirectBuffer(1);
        m_IndirectDataUniform = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        m_ComputeIndirect = bgfx::createProgram(vcl::loadShader("lines/indirect_based_lines/cs_compute_indirect"), true);

        m_Vertices = {
            0.0f, 1.0f, 
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f, 
        };

        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
         .end();

        m_Vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_Vertices[0], sizeof(float) * m_Vertices.size()),
            layout
        );

        m_Indices = {
            0, 1, 2,
            1, 3, 2,
        };

        m_Ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_Indices[0], sizeof(uint32_t) * m_Indices.size()),
            BGFX_BUFFER_INDEX32
        );

        generateIndirectBuffer();
        allocatePointsBuffer();
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(Point) * points.size()));
    }

    IndirectBasedLines::~IndirectBasedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_PointsBuffer);
        bgfx::destroy(m_IndirectBuffer);
        bgfx::destroy(m_ComputeIndirect);
    }

    void IndirectBasedLines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .end();

        m_PointsBuffer = bgfx::createDynamicVertexBuffer(m_PointsSize, layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void IndirectBasedLines::generateIndirectBuffer() {
        float data[] = {static_cast<float>(m_PointsSize / 2), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);
		bgfx::setBuffer(0, m_IndirectBuffer, bgfx::Access::Write);
		bgfx::dispatch(0, m_ComputeIndirect);
    }

    void IndirectBasedLines::draw(uint viewId) const {
        float data1[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.thickness, static_cast<float>(m_Data.leftCap)};
        bgfx::setUniform(m_UniformData1, data1);

        float data2[] = {static_cast<float>(m_Data.rigthCap), m_Data.antialias, m_Data.border, 0};
        bgfx::setUniform(m_UniformData2, data2);

        bgfx::setUniform(m_UniformBorderColor, &m_Data.borderColor);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_Vbh);
        bgfx::setIndexBuffer(m_Ibh);
        
        bgfx::setBuffer(1, m_PointsBuffer, bgfx::Access::Read);

        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(viewId, m_Program, m_IndirectBuffer, 0);
    }

    void IndirectBasedLines::update(const std::vector<Point> &points) {
        int oldSize = m_PointsSize;
        m_PointsSize = points.size();

        if(oldSize != m_PointsSize) {
            generateIndirectBuffer();
        }
         
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(Point) * points.size()));
    }
}
}