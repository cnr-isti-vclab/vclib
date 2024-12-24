#ifndef VIEWER_WIDGET_H
#define VIEWER_WIDGET_H

#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/renderer.h>
#include <vclib/qt/widget_manager.h>

namespace vcl::qt {

using ViewerWidget =
    vcl::Renderer<vcl::qt::WidgetManager, vcl::Canvas, vcl::ViewerDrawer>;

} // namespace vcl::qt

#endif // VIEWER_WIDGET_H
