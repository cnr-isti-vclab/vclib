#include <vclib/bgfx/drawable/lines/lines/instancing_based_lines.h>

namespace vcl {
namespace lines {
    InstancingBasedLines::InstancingBasedLines(const std::vector<Point> &points, const float width, const float heigth) :
        Lines(width, heigth, "lines/instancing_based_lines/vs_instancing_based_lines", "lines/instancing_based_lines/fs_instancing_based_lines") 
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
        bgfx::setInstanceDataBuffer(&m_IDBPoints);
        
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void InstancingBasedLines::update(const std::vector<Point> &points) {
        generateInstanceDataBuffer(points);
    }

    void InstancingBasedLines::generateInstanceDataBuffer(const std::vector<Point> &points) {
        const uint16_t stride = sizeof(float) * 16;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer((points.size() / 2), stride);
        bgfx::allocInstanceDataBuffer(&m_IDBPoints, linesNum, stride);

        uint8_t* data = m_IDBPoints.data;
        for(uint32_t i = 1; i < points.size(); i+=2) {
            float* p0 = reinterpret_cast<float*>(data);
            p0[0] = points[i-1].x;
            p0[1] = points[i-1].y;
            p0[2] = points[i-1].z;
            p0[3] = 0.0f;

            float* p1 = (float*)&data[16];
            p1[0] = points[i].x;
            p1[1] = points[i].y;
            p1[2] = points[i].z;
            p1[3] = 0.0f;

            float* color0 = (float*)&data[32];
            color0[0] = points[i-1].color.r;
            color0[1] = points[i-1].color.g;
            color0[2] = points[i-1].color.b;
            color0[3] = points[i-1].color.a;

            float* color1 = (float*)&data[48];
            color1[0] = points[i].color.r;
            color1[1] = points[i].color.g;
            color1[2] = points[i].color.b;
            color1[3] = points[i].color.a;

            data += stride;
        }
    }
}
}