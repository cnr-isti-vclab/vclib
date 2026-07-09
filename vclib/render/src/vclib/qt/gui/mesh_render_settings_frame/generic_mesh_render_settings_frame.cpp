// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/mesh_render_settings_frame/generic_mesh_render_settings_frame.h>

namespace vcl::qt {

GenericMeshRenderSettingsFrame::GenericMeshRenderSettingsFrame(
    MeshRenderSettings& settings,
    QWidget*            parent) : QFrame(parent), mMRS(settings)
{
}

GenericMeshRenderSettingsFrame::~GenericMeshRenderSettingsFrame()
{
}

void GenericMeshRenderSettingsFrame::setButtonBackGround(
    QPushButton*  b,
    const QColor& c)
{
    QPalette px;
    px.setColor(QPalette::Button, c);
    b->setPalette(px);
    b->update();
}

QColor GenericMeshRenderSettingsFrame::getButtonBackGround(const QPushButton* b)
{
    QPalette px = b->palette();
    return px.color(QPalette::Button);
}

} // namespace vcl::qt
