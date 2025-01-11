#include <vclib/bgfx/drawable/lines/lines/instancing_based_lines.h>

namespace vcl::lines {
    InstancingBasedLines::InstancingBasedLines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth) :
        DrawableLines(width, heigth, "lines/instancing_based_lines/vs_instancing_based_lines", "lines/instancing_based_lines/fs_instancing_based_lines") 
    {

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

        generateInstanceDataBuffer(points);
    }

    InstancingBasedLines::~InstancingBasedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void InstancingBasedLines::draw(uint viewId) const {
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
        bgfx::setInstanceDataBuffer(&m_IDBPoints);
        
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void InstancingBasedLines::update(const std::vector<LinesVertex> &points) {
        generateInstanceDataBuffer(points);
    }

    void InstancingBasedLines::generateInstanceDataBuffer(const std::vector<LinesVertex> &points) {
        const uint16_t stride = sizeof(float) * 16;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer((points.size() / 2), stride);
        bgfx::allocInstanceDataBuffer(&m_IDBPoints, linesNum, stride);

        uint8_t* data = m_IDBPoints.data;
        for(uint32_t i = 1; i < points.size(); i+=2) {

            float* p0 = reinterpret_cast<float*>(data);
            p0[0] = points[i-1].X;
            p0[1] = points[i-1].Y;
            p0[2] = points[i-1].Z;

            uint32_t* color0 = (uint32_t*)&data[12];
            color0[0] = points[i-1].getUintColor();

            float* p1 = (float*)&data[16];
            p1[0] = points[i].X;
            p1[1] = points[i].Y;
            p1[2] = points[i].Z;

            uint32_t* color1 = (uint32_t*)&data[28];
            color1[0] = points[i].getUintColor();

            float* n0 = (float*)&data[32];
            n0[0] = points[i-1].xN;
            n0[1] = points[i-1].yN;
            n0[2] = points[i-1].zN;
            n0[3] = 0;

            float* n1 = (float*)&data[48];
            n1[0] = points[i].xN;
            n1[1] = points[i].yN;
            n1[2] = points[i].zN;
            n1[3] = 0;

            data += stride;
        }
    }
}