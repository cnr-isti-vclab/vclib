#include <vclib/bgfx/context/load_program.h>
#include <vclib/bgfx/drawable/lines/polylines/instancing_based_polylines.h>
#include <cmath>

namespace vcl {
namespace lines {
    InstancingBasedPolylines::InstancingBasedPolylines(const std::vector<Point> &points, const float width, const float heigth) :
        Polylines(width, heigth, "polylines/instancing_based_polylines/vs_instancing_based_segments", "polylines/instancing_based_polylines/fs_instancing_based_polylines")

    {
        m_JoinsProgram = vcl::loadProgram("polylines/instancing_based_polylines/vs_instancing_based_joins", "polylines/instancing_based_polylines/fs_instancing_based_polylines");
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

        generateIDBSegments(points);
        generateIDBJoins(points);
    }

    InstancingBasedPolylines::~InstancingBasedPolylines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void InstancingBasedPolylines::draw(uint viewId) const {
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

        bgfx::setVertexBuffer(0, m_Vbh);
        bgfx::setIndexBuffer(m_Ibh);
        bgfx::setInstanceDataBuffer(&m_IDBSegments);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);

        if(m_Data.join != 0) {
            bgfx::setVertexBuffer(0, m_Vbh);
            bgfx::setIndexBuffer(m_Ibh);
            bgfx::setInstanceDataBuffer(&m_IDBJoins);
            bgfx::setState(state);
            bgfx::submit(viewId, m_JoinsProgram);
        }

    }

    void InstancingBasedPolylines::update(const std::vector<Point> &points) {
        generateIDBSegments(points);
        generateIDBJoins(points);
    }

    void InstancingBasedPolylines::generateIDBSegments(const std::vector<Point> &points) {
        const uint16_t stride = sizeof(float) * 20;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer(points.size() - 1, stride);
        bgfx::allocInstanceDataBuffer(&m_IDBSegments, linesNum, stride);

        uint8_t* data = m_IDBSegments.data;
        for(uint32_t i = 0; i < linesNum; i++) {
            float* prev = reinterpret_cast<float*>(data);
            prev[0] = points[i - !!i].x;
            prev[1] = points[i - !!i].y;
            prev[2] = points[i - !!i].z;
            prev[3] = 0.0f;

            float* curr = (float*)&data[16];
            curr[0] = points[i].x;
            curr[1] = points[i].y;
            curr[2] = points[i].z;
            curr[3] = 0.0f;

            float* next = (float*)&data[32];
            next[0] = points[i + 1].x;
            next[1] = points[i + 1].y;
            next[2] = points[i + 1].z;
            next[3] = 0.0f;

            float* next_next = (float*)&data[48];
            next_next[0] = points[i + 1 + (!!(linesNum - 1 - i))].x;
            next_next[1] = points[i + 1 + (!!(linesNum - 1 - i))].y;
            next_next[2] = points[i + 1 + (!!(linesNum - 1 - i))].z;
            next_next[3] = 0.0f;

            uint32_t color0_hex = static_cast<uint8_t>(std::round(points[i].color.r * 255)) << 24 |
                                  static_cast<uint8_t>(std::round(points[i].color.g * 255)) << 16 |
                                  static_cast<uint8_t>(std::round(points[i].color.b * 255)) << 8  |
                                  static_cast<uint8_t>(std::round(points[i].color.a * 255));

            uint32_t color1_hex = static_cast<uint8_t>(std::round(points[i + 1].color.r * 255)) << 24 |
                                  static_cast<uint8_t>(std::round(points[i + 1].color.g * 255)) << 16 |
                                  static_cast<uint8_t>(std::round(points[i + 1].color.b * 255)) << 8  |
                                  static_cast<uint8_t>(std::round(points[i + 1].color.a * 255));

            float* color = (float*)&data[64];
            color[0] = static_cast<float>(color0_hex);
            color[1] = static_cast<float>(color1_hex);
            color[2] = 0.0;
            color[3] = 0.0;
        
            data+=stride;
        }
    }

    void InstancingBasedPolylines::generateIDBJoins(const std::vector<Point> &points) {
        const uint16_t stride = sizeof(float) * 16;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer(points.size() - 2, stride);
        bgfx::allocInstanceDataBuffer(&m_IDBJoins, linesNum, stride);

        uint8_t* data = m_IDBJoins.data;
        for(uint32_t i = 1; i < linesNum + 1; i++) {
            float* prev = reinterpret_cast<float*>(data);
            prev[0] = points[i - 1].x;
            prev[1] = points[i - 1].y;
            prev[2] = points[i - 1].z;
            prev[3] = 0.0f;

            float* curr = (float*)&data[16];
            curr[0] = points[i].x;
            curr[1] = points[i].y;
            curr[2] = points[i].z;
            curr[3] = 0.0f;

            float* next = (float*)&data[32];
            next[0] = points[i + 1].x;
            next[1] = points[i + 1].y;
            next[2] = points[i + 1].z;
            next[3] = 0.0f;

            float* color = (float*)&data[48];
            color[0] = points[i].color.r;
            color[1] = points[i].color.g;
            color[2] = points[i].color.b;
            color[3] = points[i].color.a;

            data+=stride;
        }
    }
}
}