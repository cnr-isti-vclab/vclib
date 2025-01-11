/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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
