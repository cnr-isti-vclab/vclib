#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl {
namespace lines {
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<Point> &points, const float width, const float heigth) :
        Lines(width, heigth, "lines/cpu_generated_lines/vs_cpu_generated_lines", "lines/cpu_generated_lines/fs_cpu_generated_lines"),
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

    void CPUGeneratedLines::update(const std::vector<Point> &points) {
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
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void CPUGeneratedLines::generateBuffers(const std::vector<Point> points) {
        for(uint i = 1; i < points.size(); i+=2) {

            for(uint k = 0; k < 2; k++) {

                for(uint j = 0; j < 2; j++) {

                    m_Vertices.push_back(points[i - 1].x);
                    m_Vertices.push_back(points[i - 1].y);
                    m_Vertices.push_back(points[i - 1].z);
                    
                    m_Vertices.push_back(points[i].x);
                    m_Vertices.push_back(points[i].y);
                    m_Vertices.push_back(points[i].z);

                    m_Vertices.push_back(points[i - (1 - k)].color.r);
                    m_Vertices.push_back(points[i - (1 - k)].color.g);
                    m_Vertices.push_back(points[i - (1 - k)].color.b);
                    m_Vertices.push_back(points[i - (1 - k)].color.a);

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
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
         .end();

        m_Vbh = bgfx::createDynamicVertexBuffer(m_PointsSize * 4, layout, BGFX_BUFFER_ALLOW_RESIZE);
    }

    void CPUGeneratedLines::allocateIndexBuffer() {
        m_Ibh = bgfx::createDynamicIndexBuffer(m_PointsSize * 6, BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
    }
}
}