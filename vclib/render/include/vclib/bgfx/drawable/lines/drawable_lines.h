#pragma once
#include "lines_utils.h"
#include <algorithm>
#include <bgfx/bgfx.h>
#include <vclib/render/interfaces/drawable_object_i.h>

namespace vcl {
    
namespace lines {

class Lines : public DrawableObjectI
{
public:
    static std::unique_ptr<Lines> create(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth,
        Types                     type = Types::CPU_GENERATED);

    static std::unique_ptr<Lines> create(bgfx::VertexBufferHandle vbh);

    static std::unique_ptr<Lines> create(
        bgfx::VertexBufferHandle vbh,
        bgfx::IndexBufferHandle  ivh);

    Lines(
        const float        width,
        const float        heigth,
        const std::string& vs_name,
        const std::string& fs_name);

    virtual ~Lines();

    Box3d boundingBox() const override
    {
        return Box3d(Point3d(-1, -1, -1), Point3d(1, 1, 1));
    }

    bool isVisible() const override { return m_Visible; }

    void setVisibility(bool vis) override { m_Visible = vis; }

    virtual void update(const std::vector<Point>& points) = 0;

    void setThickness(float thickness) { m_Data.thickness = thickness; }

    void setLeftCap(const Caps& cap) { m_Data.leftCap = cap; }

    void setRigthCap(const Caps& cap) { m_Data.rigthCap = cap; }

    void setAntialias(float antialias) { m_Data.antialias = antialias; }

    void setBorder(float border) { m_Data.border = border; }

    void setBorderColor(const Color& color) { m_Data.borderColor = color; }

    void setScreenSize(float width, float heigth)
    {
        m_Data.screenSize[0] = width;
        m_Data.screenSize[1] = heigth;
    }

protected:
    bgfx::ProgramHandle m_Program;
    bgfx::UniformHandle m_UniformData1;
    bgfx::UniformHandle m_UniformData2;
    bgfx::UniformHandle m_UniformBorderColor;

    struct LineData
    {
        float thickness = 5.0;
        float antialias = 0.0;
        float border    = 0.0;
        float screenSize[2];
        Caps  leftCap  = Caps::SQUARE_CAP;
        Caps  rigthCap = Caps::SQUARE_CAP;
        Color borderColor;
    };

    LineData   m_Data;
    Box3d m_BoundingBox;
    bool       m_Visible = true;
};

} // namespace lines
} // namespace vcl