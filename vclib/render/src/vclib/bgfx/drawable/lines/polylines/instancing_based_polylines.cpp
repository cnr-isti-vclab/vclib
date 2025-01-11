#include <vclib/bgfx/drawable/lines/polylines/instancing_based_polylines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines {
    InstancingBasedPolylines::InstancingBasedPolylines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth) :
        DrawablePolylines(width, heigth, "polylines/instancing_based_polylines/vs_instancing_based_segments", "polylines/instancing_based_polylines/fs_instancing_based_polylines")

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

        generateInstanceBuffer(points);
    }

    InstancingBasedPolylines::~InstancingBasedPolylines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void InstancingBasedPolylines::draw(uint viewId) const {
        m_Settings.bindUniformPolylines();

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

        if(m_Settings.getJoin() != 0) {
            bgfx::setVertexBuffer(0, m_Vbh);
            bgfx::setIndexBuffer(m_Ibh);
            bgfx::setInstanceDataBuffer(&m_IDBJoins);
            bgfx::setState(state);
            bgfx::submit(viewId, m_JoinsProgram);
        }

    }

    void InstancingBasedPolylines::update(const std::vector<LinesVertex> &points) {
        generateInstanceBuffer(points);
    }

    void InstancingBasedPolylines::generateInstanceBuffer(const std::vector<LinesVertex> &points) {
        const uint16_t strideSegments = sizeof(float) * 20;
        uint32_t linesNumSegments = bgfx::getAvailInstanceDataBuffer(points.size() - 1, strideSegments);
        bgfx::allocInstanceDataBuffer(&m_IDBSegments, linesNumSegments, strideSegments);

        const uint16_t strideJoins = sizeof(float) * 16;
        if(points.size() > 2) {
            uint32_t linesNumJoins = bgfx::getAvailInstanceDataBuffer(points.size() - 2, strideJoins);
            bgfx::allocInstanceDataBuffer(&m_IDBJoins, linesNumJoins, strideJoins);
        } 

        uint8_t* dataSegments = m_IDBSegments.data;
        uint8_t* dataJoins    = m_IDBJoins.data;

        for(uint32_t i = 0; i < linesNumSegments; i++) {
            float* prevSegments = reinterpret_cast<float*>(dataSegments);
            prevSegments[0] = points[i - !!i].X;
            prevSegments[1] = points[i - !!i].Y;
            prevSegments[2] = points[i - !!i].Z;
            prevSegments[3] = points[i].xN;

            float* currSegments = (float*)&dataSegments[16];
            currSegments[0] = points[i].X;
            currSegments[1] = points[i].Y;
            currSegments[2] = points[i].Z;

            uint32_t* color0 = (uint32_t*)&dataSegments[28];
            color0[0] = points[i].getUintColor();

            float* nextSegments = (float*)&dataSegments[32];
            nextSegments[0] = points[i + 1].X;
            nextSegments[1] = points[i + 1].Y;
            nextSegments[2] = points[i + 1].Z;

            uint32_t* color1 = (uint32_t*)&dataSegments[44];
            color1[0] = points[i + 1].getUintColor();

            float* next_nextSegments = (float*)&dataSegments[48];
            next_nextSegments[0] = points[i + 1 + (!!(linesNumSegments - 1 - i))].X;
            next_nextSegments[1] = points[i + 1 + (!!(linesNumSegments - 1 - i))].Y;
            next_nextSegments[2] = points[i + 1 + (!!(linesNumSegments - 1 - i))].Z;
            next_nextSegments[3] = points[i].yN;

            float* normalSegments = (float*)&dataSegments[64];
            normalSegments[0] = points[i].zN;
            normalSegments[1] = points[i + 1].xN;
            normalSegments[2] = points[i + 1].yN;
            normalSegments[3] = points[i + 1].zN;

            if(i > 0) {
                float* prevJoin = reinterpret_cast<float*>(dataJoins);
                prevJoin[0] = points[i - 1].X;
                prevJoin[1] = points[i - 1].Y;
                prevJoin[2] = points[i - 1].Z;
                prevJoin[3] = 0.0f;

                float* currJoin = (float*)&dataJoins[16];
                currJoin[0] = points[i].X;
                currJoin[1] = points[i].Y;
                currJoin[2] = points[i].Z;

                uint32_t* colorJoin = (uint32_t*)&dataJoins[28];
                colorJoin[0] = points[i].getUintColor();

                float* nextJoin = (float*)&dataJoins[32];
                nextJoin[0] = points[i + 1].X;
                nextJoin[1] = points[i + 1].Y;
                nextJoin[2] = points[i + 1].Z;
                nextJoin[3] = 0.0f;

                float* normalJoin = (float*)&dataJoins[48];
                normalJoin[0] = points[i].xN;
                normalJoin[1] = points[i].yN;
                normalJoin[2] = points[i].zN;
                normalJoin[3] = 0;

                dataJoins+=strideJoins;
            }
        
            dataSegments+=strideSegments;
        }
    }
}