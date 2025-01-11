#include <vclib/bgfx/drawable/lines/polylines/indirect_based_polylines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines {
    IndirectBasedPolylines::IndirectBasedPolylines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth) :
        DrawablePolylines(width, heigth, "polylines/indirect_based_polylines/vs_indirect_based_segments", "polylines/indirect_based_polylines/fs_indirect_based_polylines"),
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
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()));
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
        m_Settings.bindUniformPolylines();

        float indirectData[] = {static_cast<float>(m_PointsSize - 1), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, indirectData);

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

        if(m_Settings.getJoin() != 0) {
            bgfx::setVertexBuffer(0, m_Vbh);
            bgfx::setIndexBuffer(m_Ibh);
            bgfx::setBuffer(1, m_PointsBuffer, bgfx::Access::Read);
            bgfx::setState(state);
            bgfx::submit(viewId, m_JoinsProgram, m_JoinsIndirectBuffer, 0);
        }
    }

    void IndirectBasedPolylines::update(const std::vector<LinesVertex> &points) {
        int oldSize = m_PointsSize;
        m_PointsSize = points.size();
        
        if(oldSize != m_PointsSize) {
            generateIndirectBuffers();
        }

        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()));
    }

    void IndirectBasedPolylines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8)
         .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
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