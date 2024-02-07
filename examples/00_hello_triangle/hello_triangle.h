#ifndef HELLOTRIANGLE_H
#define HELLOTRIANGLE_H

#include <vclib/ext/qt/bgfx/canvas_widget.h>

class HelloTriangle : public vcl::qbgf::CanvasWidget
{
public:
    HelloTriangle();

    ~HelloTriangle();

    void setupWhatToDraw();

    void setupHowToDraw();

    void draw() override;

private:
    float positions[6] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f
    };

    bgfx::VertexBufferHandle vbh;

    bgfx::ProgramHandle program;
};

#endif // HELLOTRIANGLE_H
