#include <vclib/bgfx/context/load_program.h>
#include <vclib/bgfx/drawable/lines/polylines/indirect_based_polylines.h>

namespace vcl {
namespace lines {
    IndirectBasedPolylines::IndirectBasedPolylines(const std::vector<Point> &points, const float width, const float heigth) :
        Polylines(width, heigth, "polylines/indirect_based_polylines/vs_indirect_based_segments", "polylines/indirect_based_polylines/fs_indirect_based_polylines"),
        m_PointsSize(points.size())
    {
        m_JoinsIndirectBuffer = bgfx::createIndirectBuffer(1);
        m_SegmentsIndirectBuffer = bgfx::createIndirectBuffer(1);


        m_IndirectDataUniform = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        m_JoinsProgram = vcl::loadProgram("polylines/indirect_based_polylines/vs_indirect_based_joins", "polylines/indirect_based_polylines/fs_indirect_based_polylines");
        m_ComputeIndirect = bgfx::createProgram(vcl::loadShader("polylines/indirect_based_polylines/cs_compute_indirect"), true);

        m_Vertices = {
            0.0f, 0.0f,
            0.0f, 1.0f, 
            1.0f, 0.0f, 
            1.0f, 1.0f,
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
            0, 3, 1,
            0, 2, 3,
        };

        m_Ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_Indices[0], sizeof(uint32_t) * m_Indices.size()),
            BGFX_BUFFER_INDEX32
        );

        generateIndirectBuffers();

        allocatePointsBuffer();
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(Point) * points.size()));
    }

    IndirectBasedPolylines::~IndirectBasedPolylines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_PointsBuffer);
        bgfx::destroy(m_SegmentsIndirectBuffer);
        bgfx::destroy(m_JoinsIndirectBuffer);
        bgfx::destroy(m_ComputeIndirect);
    }


    void IndirectBasedPolylines::draw(uint viewId) const {
        float data1[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.miterLimit, m_Data.thickness};
        bgfx::setUniform(m_UniformData1, data1);

        float data2[] = {static_cast<float>(m_Data.leftCap), static_cast<float>(m_Data.rigthCap), static_cast<float>(m_Data.join), 0};
        bgfx::setUniform(m_UniformData2, data2);

        float indirectData[] = {static_cast<float>(m_PointsSize - 1), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, indirectData);

        bgfx::setUniform(m_UniformColor, &m_Data.color);

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
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program, m_SegmentsIndirectBuffer, 0);

        if(m_Data.join != 0) {
            bgfx::setVertexBuffer(0, m_Vbh);
            bgfx::setIndexBuffer(m_Ibh);
            bgfx::setBuffer(1, m_PointsBuffer, bgfx::Access::Read);
            bgfx::setState(state);
            bgfx::submit(viewId, m_JoinsProgram, m_JoinsIndirectBuffer, 0);
        }
    }

    void IndirectBasedPolylines::update(const std::vector<Point> &points) {
        int oldSize = m_PointsSize;
        m_PointsSize = points.size();
        
        if(oldSize != m_PointsSize) {
            generateIndirectBuffers();
        }

        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(Point) * points.size()));
    }

    void IndirectBasedPolylines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .end();

        m_PointsBuffer = bgfx::createDynamicVertexBuffer(m_PointsSize, layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void IndirectBasedPolylines::generateIndirectBuffers() {
        float data[] = {static_cast<float>(m_PointsSize), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);

		bgfx::setBuffer(0, m_SegmentsIndirectBuffer, bgfx::Access::Write);
		bgfx::setBuffer(1, m_JoinsIndirectBuffer, bgfx::Access::Write);
		bgfx::dispatch(0, m_ComputeIndirect);
    }
}
}