// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_MESH_VIEWER_H
#define VCL_RENDER_CONCEPTS_MESH_VIEWER_H

#include <vclib/base.h>
#include <vclib/meshes.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/editors.h>

#include <utility>

namespace vcl {

/**
 * @brief Concept that verifies if a class provides the standard mesh viewer
 * interface.
 *
 * A type satisfies MeshViewerConcept if it provides methods to add, insert,
 * remove, update, and clear meshes, as well as methods to query the selected
 * mesh, refresh editors, fit the scene, and show the window.
 *
 * @tparam T: The type to be checked against the MeshViewerConcept.
 */
template<typename T>
concept MeshViewerConcept = requires (
    T&&                                  obj,
    vcl::DrawableMesh<vcl::PolyMesh>&    obj1,
    vcl::DrawableMesh<vcl::PolyMesh>&&   obj2,
    std::shared_ptr<vcl::DrawableObject> obj3,
    uint                                 id) {
    typename RemoveRef<T>::ViewerType;
    typename RemoveRef<T>::EditorType;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.pushDrawableObject(obj1) } -> std::same_as<uint>;
        { obj.pushDrawableObject(std::move(obj2)) } -> std::same_as<uint>;
        { obj.pushDrawableObject(obj3) } -> std::same_as<uint>;

        { obj.insertDrawableObject(0, obj1) } -> std::same_as<bool>;
        { obj.insertDrawableObject(0, std::move(obj2)) } -> std::same_as<bool>;
        { obj.insertDrawableObject(0, obj3) } -> std::same_as<bool>;

        { obj.removeDrawableObject(id) } -> std::same_as<bool>;
        { obj.clearDrawableObjects() } -> std::same_as<void>;

        {
            obj.drawableObject(id)
        } -> std::same_as<std::shared_ptr<vcl::DrawableObject>>;

        { std::as_const(obj).selectedDrawableObject() } -> std::same_as<uint>;

        {
            std::as_const(obj).drawableObject(id)
        } -> std::same_as<std::shared_ptr<const vcl::DrawableObject>>;
        { std::as_const(obj).drawableObjectsCount() } -> std::same_as<uint>;
        {
            std::as_const(obj).drawableObjects()
        } -> std::same_as<const vcl::DrawableObjectVector&>;

        { obj.updateGUI() } -> std::same_as<void>;

        { obj.refreshEditors() } -> std::same_as<void>;

        { obj.fitScene() } -> std::same_as<void>;

        { obj.show() } -> std::same_as<void>;
        { obj.showMaximized() } -> std::same_as<void>;
    };
};

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_MESH_VIEWER_H
