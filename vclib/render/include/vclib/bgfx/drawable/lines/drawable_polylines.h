#pragma once
#include "lines_utils.h"
#include <bgfx/bgfx.h>
#include <vclib/render/interfaces/drawable_object_i.h>

namespace vcl {

namespace lines {

class Polylines : public DrawableObjectI
{
public:
    static std::unique_ptr<Polylines> create(
        const std::vector<Point>& points,
        const float               width,
        const float               heigth,
        Types                     type = Types::CPU_GENERATED);

    static std::unique_ptr<Polylines> create(bgfx::VertexBufferHandle vbh);

    static std::unique_ptr<Polylines> create(
        bgfx::VertexBufferHandle vbh,
        bgfx::IndexBufferHandle  ivh);

    Polylines(
        const float        width,
        const float        heigth,
        const std::string& vs_name,
        const std::string& fs_name);

    virtual ~Polylines();

    Box3d boundingBox() const override
    {
        return Box3d(Point3d(-1, -1, -1), Point3d(1, 1, 1));
    }

    bool isVisible() const override { return m_Visible; }

    void setVisibility(bool vis) override { m_Visible = vis; }

    virtual void update(const std::vector<Point>& points) = 0;

    void setThickness(float thickness) { m_Data.thickness = thickness; }

    void setMiterLimit(float miterLimit) { m_Data.miterLimit = miterLimit; }

    void setColor(Color color) { m_Data.color = color; }

    void setLeftCap(const Caps& cap) { m_Data.leftCap = cap; }

    void setRigthCap(const Caps& cap) { m_Data.rigthCap = cap; }

    void setJoin(const Joins& join) { m_Data.join = join; }

    void setScreenSize(float width, float heigth)
    {
        m_Data.screenSize[0] = width;
        m_Data.screenSize[1] = heigth;
    }

protected:
    bgfx::ProgramHandle m_Program;
    bgfx::UniformHandle m_UniformData1;
    bgfx::UniformHandle m_UniformData2;

    bgfx::UniformHandle m_UniformColor;

    struct LineData
    {
        float thickness  = 5.0;
        float miterLimit = 10.0;
        float screenSize[2];
        float lineLength = 0;
        Caps  leftCap    = Caps::BUTT_CAP;
        Caps  rigthCap   = Caps::BUTT_CAP;
        Joins join       = Joins::MITER_JOIN;
        Color color;
    };

    LineData m_Data;
    Box3d    m_BoundingBox;
    bool     m_Visible = true;
};

} // namespace lines

} // namespace vcl
