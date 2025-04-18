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

#include <vclib/render/automation/actions.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <memory>

namespace vcl {

template<typename BmarkDrawer>
class ChangeShaderAutomationAction :
        public AbstractAutomationAction<BmarkDrawer>
{
    using Base    = AbstractAutomationAction<BmarkDrawer>;
    using SrfPrgs = DrawableMesh<TriMesh>::SurfaceProgramsType;

    SrfPrgs                               prg;
    std::shared_ptr<DrawableObjectVector> objVec;

public:
    ChangeShaderAutomationAction(
        std::shared_ptr<DrawableObjectVector> objVec,
        SrfPrgs                               prg) : objVec {objVec}, prg {prg}
    {
    }

    std::string getDescription() override
    {
        std::string progString;
        switch (prg) {
        case SrfPrgs::UBER: progString = "UBER"; break;
        case SrfPrgs::SPLIT: progString = "SPLIT"; break;
        case SrfPrgs::UBER_WITH_STATIC_IF:
            progString = "UBER WITH STATIC IF";
            break;
        };
        return std::string("Changed shader used to ") + progString;
    }

    using Base::start;

    void doAction() override
    {
        Base::doAction();
        for (auto& obj : *objVec) {
            auto* mesh = dynamic_cast<DrawableMesh<TriMesh>*>(obj.get());
            if (mesh) {
                mesh->setSurfaceProgramType(prg);
            }
        }
        end();
    }

    using Base::end;

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<ChangeShaderAutomationAction<BmarkDrawer>>(
            *this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<ChangeShaderAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl
