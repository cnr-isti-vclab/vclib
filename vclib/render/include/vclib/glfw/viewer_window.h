#ifndef VIEWER_WINDOW_H
#define VIEWER_WINDOW_H

#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/renderer.h>

namespace vcl::glfw {

using ViewerWindow =
    vcl::Renderer<vcl::glfw::WindowManager, vcl::Canvas, vcl::ViewerDrawer>;

} // namespace vcl::glfw

#endif // VIEWER_WINDOW_H
