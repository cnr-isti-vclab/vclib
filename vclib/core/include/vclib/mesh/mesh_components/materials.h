// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_MESH_COMPONENTS_MATERIALS_H
#define VCL_MESH_MESH_COMPONENTS_MATERIALS_H

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/base/component_predicates.h>

#include <vclib/space/core.h>

#include <istream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace vcl::mesh {

namespace detail {

struct MData
{
    // The base path of the mesh, used to resolve relative texture paths.
    std::string meshPath;
    // A vector containing all the materials defined for the mesh.
    std::vector<Material> materials;
    // A map where keys are texture paths and values are the loaded Image data.
    std::map<std::string, Image> textureImages;

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, meshPath);
        vcl::serialize(os, materials);
        vcl::serialize(os, textureImages);
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, meshPath);
        vcl::deserialize(is, materials);
        vcl::deserialize(is, textureImages);
    }
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
 * @ingroup mesh_components
 */
class Materials : public comp::Component<CompId::MATERIALS, detail::MData>
{
    using Base = comp::Component<CompId::MATERIALS, detail::MData>;

    inline static const Image EMPTY_IMAGE;

    detail::MData mMaterials;

public:
    /// @brief Exposes the Material type used by this component.
    using MaterialType = Material;

    /// @brief Type used for indexing materials in the component.
    using MaterialIndexType = ushort;

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
    ushort materialCount() const { return static_cast<ushort>(mats().size()); }

    /**
     * @brief Returns the number of texture images stored in the component.
     * @return The total number of texture images.
     */
    uint textureImageCount() const { return txtImgs().size(); }

    /**
     * @brief Returns the mesh base path.
     * @return The mesh base path string.
     */
    decltype(auto) meshBasePath(this auto&& self)
    {
        return (self.mMaterials.meshPath);
    }

    /**
     * @brief Returns  the material at the specified index.
     * @param[in] i: The index of the material.
     * @return The Material object.
     */
    decltype(auto) material(this auto&& self, ushort i)
    {
        assert(i < self.materialCount());
        return self.mats()[i];
    }

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
    void pushMaterial(const Material& mat)
    {
        assert(mats().size() < USHORT_NULL);
        mats().push_back(mat);
    }

    /**
     * @brief Adds a material to the vector of materials (move version).
     * @param[in] mat: The material to add.
     */
    void pushMaterial(Material&& mat)
    {
        assert(mats().size() < USHORT_NULL);
        mats().push_back(std::move(mat));
    }

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
    decltype(auto) materialBegin(this auto&& self)
    {
        return self.mats().begin();
    }

    /**
     * @brief Returns an iterator to the end of the vector of materials.
     * @return an iterator to the end of the vector of materials.
     */
    decltype(auto) materialEnd(this auto&& self) { return self.mats().end(); }

    /**
     * @brief Returns an iterator to the beginning of the map of texture
     * images.
     * @return an iterator to the beginning of the map of texture images.
     */
    decltype(auto) textureImageBegin(this auto&& self)
    {
        return self.txtImgs().begin();
    }

    /**
     * @brief Returns an iterator to the end of the map of texture images.
     * @return an iterator to the end of the map of texture images.
     */
    decltype(auto) textureImageEnd(this auto&& self)
    {
        return self.txtImgs().end();
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
    auto materials(this auto&& self)
    {
        return View(self.materialBegin(), self.materialEnd());
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
    auto textureImages(this auto&& self)
    {
        return View(self.textureImageBegin(), self.textureImageEnd());
    }

protected:
    // Component interface functions
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m);

    void serialize(std::ostream& os) const { vcl::serialize(os, mMaterials); }

    void deserialize(std::istream& is) { vcl::deserialize(is, mMaterials); }

private:
    // members that allow to access the data
    std::vector<Material>& mats() { return mMaterials.materials; }

    const std::vector<Material>& mats() const { return mMaterials.materials; }

    std::map<std::string, Image>& txtImgs() { return mMaterials.textureImages; }

    const std::map<std::string, Image>& txtImgs() const
    {
        return mMaterials.textureImages;
    }
};

/* concept */

/**
 * @brief A concept that checks whether a type T (that should be a Mesh)
 * has the Materials component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::mesh::Materials, with any template arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasMaterials.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasMaterials = comp::HasComponent<CompId::MATERIALS, T>;

/* importFrom function */

template<typename OtherMesh>
void Materials::importFrom(const OtherMesh& m)
{
    if constexpr (HasMaterials<OtherMesh>) {
        clearMaterials();
        for (const auto& mat : m.materials()) {
            mats().push_back(mat);
        }
        for (const auto& [path, img] : m.textureImages()) {
            txtImgs()[path] = img;
        }
        meshBasePath() = m.meshBasePath();
    }
}

} // namespace vcl::mesh

#endif // VCL_MESH_MESH_COMPONENTS_MATERIALS_H
