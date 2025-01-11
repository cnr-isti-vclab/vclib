#include <vclib/bgfx/drawable/lines/polylines/texture_based_polylines.h>
#include <vclib/bgfx/context/load_program.h>
namespace vcl::lines {

    TextureBasedPolylines::TextureBasedPolylines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth, const uint32_t maxTextureSize) :
        DrawablePolylines(width, heigth, "polylines/texture_based_polylines/vs_texture_based_segments", "polylines/texture_based_polylines/fs_texture_based_polylines"),
        m_PointsSize(points.size()),
        m_MaxTextureSize(maxTextureSize)
    {
        m_JoinsIndirectBuffer = bgfx::createIndirectBuffer(1);
        m_SegmentsIndirectBuffer = bgfx::createIndirectBuffer(1);

        m_ComputeDataUniform = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        m_JoinsProgram = vcl::loadProgram("polylines/texture_based_polylines/vs_texture_based_joins", "polylines/texture_based_polylines/fs_texture_based_polylines");
        m_ComputeTexture = bgfx::createProgram(vcl::loadShader("polylines/texture_based_polylines/cs_compute_texture"), true);

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

        allocatePointsBuffer();
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()));
        
        allocateTextureBuffer();
        generateTextureBuffer();
    }

    TextureBasedPolylines::~TextureBasedPolylines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_SegmentsIndirectBuffer);
        bgfx::destroy(m_JoinsIndirectBuffer);

        bgfx::destroy(m_JoinsProgram);
        bgfx::destroy(m_ComputeDataUniform);

        bgfx::destroy(m_TextureBufferSegments);
        bgfx::destroy(m_TextureBufferJoins);
        bgfx::destroy(m_PointsBuffer);
        bgfx::destroy(m_ComputeTexture);
    }

    void TextureBasedPolylines::draw(uint viewId) const {
        m_Settings.bindUniformPolylines();

        float indirectData[] = {static_cast<float>(m_PointsSize - 1), static_cast<float>(m_MaxTextureSize), 0, 0};
        bgfx::setUniform(m_ComputeDataUniform, indirectData);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_Vbh);
        bgfx::setIndexBuffer(m_Ibh);
        bgfx::setImage(0, m_TextureBufferSegments, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program, m_SegmentsIndirectBuffer, 0);

        if(m_Settings.getJoin() != 0 && m_PointsSize > 2) {
            bgfx::setVertexBuffer(0, m_Vbh);
            bgfx::setIndexBuffer(m_Ibh);
            bgfx::setImage(0, m_TextureBufferJoins, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
            bgfx::setState(state);
            bgfx::submit(viewId, m_JoinsProgram, m_JoinsIndirectBuffer, 0);
        }
    }

    void TextureBasedPolylines::update(const std::vector<LinesVertex> &points) {
        int oldSize = m_PointsSize;
        m_PointsSize = points.size();
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()));
        
        if(oldSize < m_PointsSize) {
            allocateTextureBuffer();
        }

        generateTextureBuffer();
    }

    void TextureBasedPolylines::generateTextureBuffer() {
        float data[] = {static_cast<float>(m_MaxTextureSize), static_cast<float>(m_PointsSize - 1), 0, 0};
        bgfx::setUniform(m_ComputeDataUniform, data);

        bgfx::setBuffer(0, m_PointsBuffer, bgfx::Access::Read);
        bgfx::setImage(1, m_TextureBufferSegments, 0, bgfx::Access::Write);
        bgfx::setImage(2, m_TextureBufferJoins, 0, bgfx::Access::Write);
		bgfx::setBuffer(3, m_SegmentsIndirectBuffer, bgfx::Access::Write);
		bgfx::setBuffer(4, m_JoinsIndirectBuffer, bgfx::Access::Write);
        bgfx::dispatch(0, m_ComputeTexture, m_PointsSize - 1, 1, 1);
    }

    void TextureBasedPolylines::allocateTextureBuffer() {
        uint16_t Y_Segments = ((m_PointsSize - 1) * 5) / (m_MaxTextureSize + 1);
        uint16_t X_Segments = Y_Segments == 0 ? ((m_PointsSize - 1) * 5) : m_MaxTextureSize;
        
        m_TextureBufferSegments = bgfx::createTexture2D(
            X_Segments, Y_Segments + 1, false, 1, bgfx::TextureFormat::RGBA32F,
            BGFX_TEXTURE_COMPUTE_WRITE
        );

        if(m_PointsSize > 2) {
            uint16_t Y_Joins = ((m_PointsSize - 2) * 4) / (m_MaxTextureSize + 1);
            uint16_t X_Joins = Y_Joins == 0 ? ((m_PointsSize - 2) * 4) : m_MaxTextureSize;

            m_TextureBufferJoins = bgfx::createTexture2D(
                X_Joins, Y_Joins + 1, false, 1, bgfx::TextureFormat::RGBA32F,
                BGFX_TEXTURE_COMPUTE_WRITE
            );
        }
    }

    void TextureBasedPolylines::allocatePointsBuffer() {
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
}