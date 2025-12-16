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

#ifndef VCL_MESH_COMPONENTS_MATERIALS_H
#define VCL_MESH_COMPONENTS_MATERIALS_H

#include "base/component.h"

#include <vclib/space/core.h>

namespace vcl::comp {

namespace detail {

struct MData
{
    // The base path of the mesh, used to resolve relative texture paths.
    std::string meshPath;
    // A vector containing all the materials defined for the mesh.
    std::vector<Material> materials;
    // A map where keys are texture paths and values are the loaded Image data.
    std::map<std::string, Image> textureImages;
};

} // namespace detail

/**
 * @brief A component that manages materials and textures for a mesh.
 *
 * The Materials component stores a collection of materials and their
 * associated texture images. It also keeps track of the base path of the mesh,
 * which is used to resolve relative paths for texture files.
 *
 * This component provides functionalities to:
 * - Add and retrieve materials.
 * - Add and retrieve texture images, mapping them by their file paths.
 * - Iterate over the stored materials and texture images.
 * - Clear all stored material and texture data.
 *
 * @ingroup components
 */
class Materials :
        public Component<
            Materials,
            CompId::MATERIALS,
            detail::MData,
            void,
            false,
            false>
{
    using Base = Component<
        Materials,
        CompId::MATERIALS,
        detail::MData,
        void,
        false,
        false>;

    inline static const Image EMPTY_IMAGE;

public:
    /// @brief Exposes the Material type used by this component.
    using MaterialType = Material;

    // iterators

    /// @brief Iterator for the vector of materials.
    using MaterialIterator = std::vector<Material>::iterator;
    /// @brief Const iterator for the vector of materials.
    using ConstMaterialIterator = std::vector<Material>::const_iterator;
    /// @brief Iterator for the map of texture images
    using TextureImageIterator = std::map<std::string, Image>::iterator;
    /// @brief Const iterator for the map of texture images.
    using ConstTextureImageIterator =
        std::map<std::string, Image>::const_iterator;

    /**
     * @brief Default constructor for the Materials component.
     */
    Materials() = default;

    /* Member functions */

    /**
     * @brief Returns the number of materials of the mesh.
     * @return The total number of materials.
     */
    uint materialsNumber() const { return mats().size(); }

    /**
     * @brief Returns the number of texture images stored in the component.
     * @return The total number of texture images.
     */
    uint textureImagesNumber() const { return txtImgs().size(); }

    /**
     * @brief Returns the mesh base path.
     *
     * This path is typically used to resolve relative paths of texture files.
     *
     * @return A const reference to the mesh base path string.
     */
    const std::string& meshBasePath() const { return Base::data().meshPath; }

    /**
     * @brief Returns a reference to the mesh base path.
     * @return A reference to the mesh base path string.
     */
    std::string& meshBasePath() { return Base::data().meshPath; }

    /**
     * @brief Returns the material at the specified index.
     * @param[in] i: The index of the material.
     * @return A const reference to the Material object
     */
    const Material& material(uint i) const { return mats()[i]; }

    /**
     * @brief Returns a mutable reference to the material at the specified
     * index.
     * @param[in] i: The index of the material.
     * @return A reference to the Material object.
     */
    Material& material(uint i) { return mats()[i]; }

    /**
     * @brief Returns the texture image associated to the given texture path.
     *
     * If no image is found for the given texture path, an empty image is
     * returned.
     *
     * @param[in] texturePath: The path of the texture.
     * @return the texture image associated to the given texture path, or an
     * empty image if no image is found.
     */
    const Image& textureImage(const std::string& texturePath) const
    {
        auto it = txtImgs().find(texturePath);
        if (it == txtImgs().end()) {
            return EMPTY_IMAGE;
        }
        return it->second;
    }

    /**
     * @brief Clears the vector of materials.
     *
     * This function clears the vector of materials and the map of texture
     * images, resetting the component to an empty state.
     */
    void clearMaterials()
    {
        mats().clear();
        txtImgs().clear();
    }

    /**
     * @brief Adds a material to the vector of materials.
     * @param[in] mat: The material to add.
     */
    void pushMaterial(const Material& mat) { mats().push_back(mat); }

    /**
     * @brief Adds a texture image to the map of texture images.
     *
     * If an image with the same texture path already exists, it is
     * overwritten.
     *
     * @param[in] texturePath: The path of the texture.
     * @param[in] img: The image to add.
     */
    void pushTextureImage(const std::string& texturePath, const Image& img)
    {
        txtImgs()[texturePath] = img;
    }

    /**
     * @brief Adds a texture image to the map of texture images (move version).
     *
     * If an image with the same texture path already exists, it is
     * overwritten.
     *
     * @param[in] texturePath: The path of the texture.
     * @param[in] img: The image to add.
     */
    void pushTextureImage(const std::string& texturePath, Image&& img)
    {
        txtImgs()[texturePath] = std::move(img);
    }

    /**
     * @brief Returns an iterator to the beginning of the vector of materials.
     * @return an iterator to the beginning of the vector of materials.
     */
    MaterialIterator materialBegin() { return mats().begin(); }

    /**
     * @brief Returns an iterator to the end of the vector of materials.
     * @return an iterator to the end of the vector of materials.
     */
    MaterialIterator materialEnd() { return mats().end(); }

    /**
     * @brief Returns a const iterator to the beginning of the vector of
     * materials.
     * @return a const iterator to the beginning of the vector of materials.
     */
    ConstMaterialIterator materialBegin() const { return mats().begin(); }

    /**
     * @brief Returns a const iterator to the end of the vector of materials.
     * @return a const iterator to the end of the vector of materials.
     */
    ConstMaterialIterator materialEnd() const { return mats().end(); }

    /**
     * @brief Returns an iterator to the beginning of the map of texture
     * images.
     * @return an iterator to the beginning of the map of texture images.
     */
    TextureImageIterator textureImageBegin() { return txtImgs().begin(); }

    /**
     * @brief Returns an iterator to the end of the map of texture images.
     * @return an iterator to the end of the map of texture images.
     */
    TextureImageIterator textureImageEnd() { return txtImgs().end(); }

    /**
     * @brief Returns a const iterator to the beginning of the map of texture
     * images.
     * @return a const iterator to the beginning of the map of texture images.
     */
    ConstTextureImageIterator textureImageBegin() const
    {
        return txtImgs().begin();
    }

    /**
     * @brief Returns a const iterator to the end of the map of texture
     * images.
     * @return a const iterator to the end of the map of texture images.
     */
    ConstTextureImageIterator textureImageEnd() const
    {
        return txtImgs().end();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the vector of materials. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (auto& mat : m.materials()) {
     *     // Do something with mat
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of materials.
     */
    View<MaterialIterator> materials()
    {
        return View(materialBegin(), materialEnd());
    }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the vector of materials. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (const auto& mat : m.materials()) {
     *     // Do something with mat
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of materials.
     */
    View<ConstMaterialIterator> materials() const
    {
        return View(materialBegin(), materialEnd());
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the map of texture images. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (auto& [path, img] : m.textureImages()) {
     *     // Do something with path and img
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the map of texture images.
     */
    View<TextureImageIterator> textureImages()
    {
        return View(textureImageBegin(), textureImageEnd());
    }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the map of texture images. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (const auto& [path, img] : m.textureImages()) {
     *     // Do something with path and img
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the map of texture images.
     */
    View<ConstTextureImageIterator> textureImages() const
    {
        return View(textureImageBegin(), textureImageEnd());
    }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, meshBasePath());
        vcl::serialize(os, mats());
        vcl::serialize(os, txtImgs());
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, meshBasePath());
        vcl::deserialize(is, mats());
        vcl::deserialize(is, txtImgs());
    }

private:
    // members that allow to access the data
    std::vector<Material>& mats() { return Base::data().materials; }

    const std::vector<Material>& mats() const { return Base::data().materials; }

    std::map<std::string, Image>& txtImgs()
    {
        return Base::data().textureImages;
    }

    const std::map<std::string, Image>& txtImgs() const
    {
        return Base::data().textureImages;
    }
};

/* concept */

/**
 * @brief A concept that checks whether a type T (that should be a Mesh)
 * has the Materials component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::Materials, with any template arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasMaterials.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasMaterials = std::derived_from<std::remove_cvref_t<T>, Materials>;

/* imoportFrom function */

template<typename Element>
void Materials::importFrom(const Element& e, bool)
{
    if constexpr (HasMaterials<Element>) {
        mats().clear();
        txtImgs().clear();
        for (const auto& mat : e.materials()) {
            mats().push_back(mat);
        }
        for (const auto& [path, img] : e.textureImages()) {
            txtImgs()[path] = img;
        }
        meshBasePath() = e.meshBasePath();
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_MATERIALS_H
