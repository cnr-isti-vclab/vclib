#include <vclib/bgfx/context/load_program.h>
#include <vclib/bgfx/drawable/lines/polylines/texture_based_polylines.h>

namespace vcl {
namespace lines {

    TextureBasedPolylines::TextureBasedPolylines(const std::vector<Point> &points, const float width, const float heigth, const uint32_t maxTextureSize) :
        Polylines(width, heigth, "polylines/texture_based_polylines/vs_texture_based_segments", "polylines/texture_based_polylines/fs_texture_based_polylines"),
        m_PointsSize(points.size()),
        m_MaxTextureSize(maxTextureSize)
    {
        m_JoinsIndirectBuffer = bgfx::createIndirectBuffer(1);
        m_SegmentsIndirectBuffer = bgfx::createIndirectBuffer(1);

        m_IndirectDataUniform = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        m_JoinsProgram = vcl::loadProgram("polylines/texture_based_polylines/vs_texture_based_joins", "polylines/texture_based_polylines/fs_texture_based_polylines");
        m_ComputeIndirect = bgfx::createProgram(vcl::loadShader("polylines/texture_based_polylines/cs_compute_indirect"), true);
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

        generateIndirectBuffers();

        allocatePointsBuffer();
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(Point) * points.size()));
        
        allocateTextureBuffer();
        generateTextureBuffer();
    }

    TextureBasedPolylines::~TextureBasedPolylines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_SegmentsIndirectBuffer);
        bgfx::destroy(m_JoinsIndirectBuffer);

        bgfx::destroy(m_JoinsProgram);
        bgfx::destroy(m_ComputeIndirect);
        bgfx::destroy(m_IndirectDataUniform);

        bgfx::destroy(m_TextureBuffer);
        bgfx::destroy(m_PointsBuffer);
        bgfx::destroy(m_ComputeTexture);
    }

    void TextureBasedPolylines::draw(uint viewId) const {
        float data1[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.miterLimit, m_Data.thickness};
        bgfx::setUniform(m_UniformData1, data1);

        float data2[] = {static_cast<float>(m_Data.leftCap), static_cast<float>(m_Data.rigthCap), static_cast<float>(m_Data.join), 0};
        bgfx::setUniform(m_UniformData2, data2);

        float indirectData[] = {static_cast<float>(m_PointsSize - 1), static_cast<float>(m_MaxTextureSize), 0, 0};
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
        bgfx::setImage(0, m_TextureBuffer, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program, m_SegmentsIndirectBuffer, 0);

        if(m_Data.join != 0) {
            bgfx::setVertexBuffer(0, m_Vbh);
            bgfx::setIndexBuffer(m_Ibh);
            bgfx::setImage(0, m_TextureBuffer, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
            bgfx::setState(state);
            bgfx::submit(viewId, m_JoinsProgram, m_JoinsIndirectBuffer, 0);
        }
    }

    void TextureBasedPolylines::update(const std::vector<Point> &points) {
        int oldSize = m_PointsSize;
        m_PointsSize = points.size();
        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(Point) * points.size()));
        
        if(oldSize < m_PointsSize) {
            allocateTextureBuffer();
        }

        if(oldSize != m_PointsSize) {
            generateIndirectBuffers();
        }
        generateTextureBuffer();
    }

    void TextureBasedPolylines::generateIndirectBuffers() {
        float data[] = {static_cast<float>(m_PointsSize), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);

		bgfx::setBuffer(0, m_SegmentsIndirectBuffer, bgfx::Access::Write);
		bgfx::setBuffer(1, m_JoinsIndirectBuffer, bgfx::Access::Write);
		bgfx::dispatch(0, m_ComputeIndirect);
    }

    void TextureBasedPolylines::generateTextureBuffer() {
        float data[] = {static_cast<float>(m_MaxTextureSize), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);

        bgfx::setBuffer(0, m_PointsBuffer, bgfx::Access::Read);
        bgfx::setImage(1, m_TextureBuffer, 0, bgfx::Access::Write);
        bgfx::dispatch(0, m_ComputeTexture, m_PointsSize, 1, 1);
    }

    void TextureBasedPolylines::allocateTextureBuffer() {
        uint16_t Y = (m_PointsSize * 2) / (m_MaxTextureSize + 1);
        uint16_t X = Y == 0 ? (m_PointsSize * 2) : m_MaxTextureSize;
        
        m_TextureBuffer = bgfx::createTexture2D(
            X, Y + 1, false, 1, bgfx::TextureFormat::RGBA32F,
            BGFX_TEXTURE_COMPUTE_WRITE
        );
    }

    void TextureBasedPolylines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .end();

        m_PointsBuffer = bgfx::createDynamicVertexBuffer(m_PointsSize, layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }
}
}