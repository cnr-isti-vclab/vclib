#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>
#include <vclib/bgfx/context/load_program.h>


namespace vcl::lines {
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth) :
        DrawableLines(width, heigth, "lines/cpu_generated_lines/vs_cpu_generated_lines", "lines/cpu_generated_lines/fs_cpu_generated_lines"),
        m_PointsSize(points.size())
    {
        allocateVertexBuffer();
        allocateIndexBuffer();
        generateBuffers(points);
    }

    CPUGeneratedLines::~CPUGeneratedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void CPUGeneratedLines::update(const std::vector<LinesVertex> &points) {
        m_Vertices.clear();
        m_Indices.clear();

        if(m_PointsSize > points.size()) {
            bgfx::destroy(m_Vbh);
            bgfx::destroy(m_Ibh);

            allocateVertexBuffer();
            allocateIndexBuffer();
        }

        m_PointsSize = points.size();
        generateBuffers(points);
    }

    void CPUGeneratedLines::draw(uint viewId) const {
        m_Settings.bindUniformLines();

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_Vbh);
        bgfx::setIndexBuffer(m_Ibh);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void CPUGeneratedLines::generateBuffers(const std::vector<LinesVertex> points) {
        for(uint i = 1; i < points.size(); i+=2) {

            for(uint k = 0; k < 2; k++) {

                for(uint j = 0; j < 2; j++) {

                    m_Vertices.push_back(points[i - 1].X);
                    m_Vertices.push_back(points[i - 1].Y);
                    m_Vertices.push_back(points[i - 1].Z);
                    
                    m_Vertices.push_back(points[i].X);
                    m_Vertices.push_back(points[i].Y);
                    m_Vertices.push_back(points[i].Z);

                    m_Vertices.push_back(points[i - (1 - k)].getReverseColor());

                    m_Vertices.push_back(points[i - (1 - k)].xN);
                    m_Vertices.push_back(points[i - (1 - k)].yN);
                    m_Vertices.push_back(points[i - (1 - k)].zN);

                    m_Vertices.push_back(k);
                    m_Vertices.push_back(j);
                }
            }

            uint32_t index = (4 * (i / 2));
            m_Indices.push_back(index);
            m_Indices.push_back(index + 3);
            m_Indices.push_back(index + 1);

            m_Indices.push_back(index);
            m_Indices.push_back(index + 2);
            m_Indices.push_back(index + 3);
        }

        bgfx::update(m_Vbh, 0, bgfx::makeRef(&m_Vertices[0], sizeof(float) * m_Vertices.size()));
        bgfx::update(m_Ibh, 0, bgfx::makeRef(&m_Indices[0], sizeof(uint32_t) * m_Indices.size()));
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

        m_Vbh = bgfx::createDynamicVertexBuffer(m_PointsSize * 4, layout, BGFX_BUFFER_ALLOW_RESIZE);
    }

    void CPUGeneratedLines::allocateIndexBuffer() {
        m_Ibh = bgfx::createDynamicIndexBuffer(m_PointsSize * 6, BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
    }
}