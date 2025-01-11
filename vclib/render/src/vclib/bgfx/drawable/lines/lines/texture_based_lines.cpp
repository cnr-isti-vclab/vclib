#include <vclib/bgfx/drawable/lines/lines/texture_based_lines.h>
#include <vclib/bgfx/context/load_program.h>

namespace vcl::lines {
    TextureBasedLines::TextureBasedLines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth, const uint32_t maxTextureSize) :
        DrawableLines(width, heigth, "lines/texture_based_lines/vs_texture_based_lines", "lines/texture_based_lines/fs_texture_based_lines"),
        m_PointsSize(points.size()),
        m_MaxTextureSize(maxTextureSize)
    {
        m_IndirectBuffer = bgfx::createIndirectBuffer(1);
        m_IndirectDataUniform = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        m_ComputeTexture = bgfx::createProgram(vcl::loadShader("lines/texture_based_lines/cs_compute_texture"), true);

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

        allocatePointsBuffer();
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()));

        allocateTextureBuffer();
        generateTextureBuffer();
    }

    TextureBasedLines::~TextureBasedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_PointsBuffer); 
        bgfx::destroy(m_IndirectBuffer);
        bgfx::destroy(m_TextureBuffer);
        bgfx::destroy(m_IndirectDataUniform);
    }

    void TextureBasedLines::draw(uint viewId) const {
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
        bgfx::setImage(0, m_TextureBuffer, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program, m_IndirectBuffer, 0);
    }

    void TextureBasedLines::update(const std::vector<LinesVertex> &points) {
        int oldSize = m_PointsSize;
        m_PointsSize = points.size();
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()));

        if(oldSize < m_PointsSize) {
            allocateTextureBuffer();
        }
        
        generateTextureBuffer();
    }

    void TextureBasedLines::generateTextureBuffer() {
        float data[] = {static_cast<float>(m_MaxTextureSize), static_cast<float>(m_PointsSize / 2), 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);

        bgfx::setBuffer(0, m_PointsBuffer, bgfx::Access::Read);
        bgfx::setImage(1, m_TextureBuffer, 0, bgfx::Access::Write);
		bgfx::setBuffer(2, m_IndirectBuffer, bgfx::Access::ReadWrite);
        bgfx::dispatch(0, m_ComputeTexture, (m_PointsSize / 2), 1, 1);
    }

    void TextureBasedLines::allocateTextureBuffer() {
        uint16_t Y = (m_PointsSize * 3) / (m_MaxTextureSize + 1);
        uint16_t X = Y == 0 ? (m_PointsSize * 3) : m_MaxTextureSize;
        
        m_TextureBuffer = bgfx::createTexture2D(
            X, Y + 1, false, 1, bgfx::TextureFormat::RGBA32F,
            BGFX_TEXTURE_COMPUTE_WRITE
        );
    }

    void TextureBasedLines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8)
         .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
         .end();

        m_PointsBuffer = bgfx::createDynamicVertexBuffer(
            m_PointsSize, layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

}