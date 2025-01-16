#include <vclib/bgfx/drawable/lines/drawable_lines.h>
#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>
#include <vclib/bgfx/drawable/lines/lines/gpu_generated_lines.h>
#include <vclib/bgfx/drawable/lines/lines/instancing_based_lines.h>
#include <vclib/bgfx/drawable/lines/lines/indirect_based_lines.h>
#include <vclib/bgfx/drawable/lines/lines/texture_based_lines.h>

#include <vclib/bgfx/context.h>


namespace vcl::lines {

    std::unique_ptr<DrawableLines> DrawableLines::create(const std::vector<LinesVertex> &points, LinesTypes type) {
        const bgfx::Caps* caps = bgfx::getCaps();

        switch (type) {
            case LinesTypes::CPU_GENERATED: {
                return std::make_unique<CPUGeneratedLines>(points);
            }

            case LinesTypes::GPU_GENERATED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                assert((void("GPU compute not supported"), computeSupported));
                return std::make_unique<GPUGeneratedLines>(points);
            }

            case LinesTypes::INSTANCING_BASED: {
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                assert((void("Instancing not supported"), instancingSupported));
                return std::make_unique<InstancingBasedLines>(points);
            }

            case LinesTypes::INDIRECT_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);

                assert((void("Instancing or compute are not supported"), instancingSupported && computeSupported && indirectSupported));
                return std::make_unique<IndirectBasedLines>(points);
            }

            case LinesTypes::TEXTURE_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                const bool textureSupported = !!(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

                assert((void("Instancing or compute or indirect or texture are not supported"), instancingSupported && computeSupported && indirectSupported && textureSupported));
                return std::make_unique<TextureBasedLines>(points, caps->limits.maxTextureSize);
            }
        }

        assert((void("Lines type is incorrect"), true));
        throw std::invalid_argument("Invalid enum case");
    }

    DrawableLines::DrawableLines(const std::string& vs_name,  const std::string& fs_name) {
        mLinesPH = Context::instance().programManager().getProgram(VclProgram::DRAWABLE_CPU_GENERATED_LINES);
        assert(bgfx::isValid(mLinesPH));
    }

    DrawableLines::DrawableLines(const DrawableLines& other) {
        mLinesPH = Context::instance().programManager().getProgram(VclProgram::DRAWABLE_CPU_GENERATED_LINES);
        assert(bgfx::isValid(mLinesPH));
    }

    DrawableLines::DrawableLines(DrawableLines&& other) {
        mLinesPH = Context::instance().programManager().getProgram(VclProgram::DRAWABLE_CPU_GENERATED_LINES);
        assert(bgfx::isValid(mLinesPH));
    }

    DrawableLines::~DrawableLines() {
        if(bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    } 
}