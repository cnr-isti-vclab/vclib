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

#include <vclib/meshes.h>
#include <vclib/processing.h>

int main()
{
    using namespace vcl::proc;

    ParameterVector parameters;

    parameters.pushBack(BoolParameter("bool", true));
    parameters.pushBack(IntParameter("int", 42));
    parameters.pushBack(UintParameter("uint", 42));

    for (vcl::uint i = 0; const auto& p : parameters) {
        std::cout << i << ": ";
        switch (p->type()) {
        case ParameterType::BOOL:
            std::cout << "Bool parameter: " << p->name();
            std::cout << "; Value: " << p->value<bool>() << std::endl;
            break;
        case ParameterType::INT:
            std::cout << "Int parameter: " << p->name();
            std::cout << "; Value: " << p->value<int>() << std::endl;
            break;
        case ParameterType::UINT:
            std::cout << "Uint parameter: " << p->name();
            std::cout << "; Value: " << p->value<vcl::uint>() << std::endl;
            break;
        default:
            std::cout << "Unknown parameter type: " << p->name() << std::endl;
            break;
        }
        ++i;
    }

    auto vec = filterMeshActions();

    std::cerr << "Id of TriEdgeMesh: "
              << vcl::toUnderlying(meshTypeId<vcl::TriEdgeMesh>()) << std::endl;

    std::cerr << "Id of PolyEdgeMesh: "
              << vcl::toUnderlying(meshTypeId<vcl::PolyEdgeMesh>())
              << std::endl;

    return 0;
}
