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

#ifndef VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
#define VCL_MESH_COMPONENTS_TEXTURE_PATHS_H

#include "base/component.h"
#include "concepts/texture_paths.h"

#include <vclib/serialization.h>
#include <vclib/types.h>

#include <string>
#include <vector>

namespace vcl::comp {

namespace detail {

struct TPData
{
    std::vector<std::string> texPaths;
    std::string              meshPath;
};

} // namespace detail

/**
 * @brief The TexturePaths class represents a component that stores the paths of
 * the textures used by a mesh. This component makes sense only if it is used by
 * meshes, and therefore it cannot be stored vertically or be optional (no
 * template arguments are needed for these options).
 *
 * The TexturePaths component stores a vector of texture paths that are relative
 * to the mesh path. The mesh path is stored as well, and it is used to
 * construct the absolute paths of the textures.
 *
 * The member functions of this class will be available in the instance of any
 * Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the TexturePaths component, you'll
 * be able to access to this component member functions from `m`:
 *
 * @code{.cpp}
 * for (const auto& texPath : m.texturePaths()) {
 *     auto absPath = m.meshBasePath() + texPath;
 *     // do something with absPath
 * }
 * @endcode
 *
 * @ingroup components
 */
class TexturePaths :
        public Component<
            TexturePaths,
            CompId::TEXTURE_PATHS,
            detail::TPData,
            void,
            false,
            false>
{
    using Base = Component<
        TexturePaths,
        CompId::TEXTURE_PATHS,
        detail::TPData,
        void,
        false,
        false>;

public:
    // iterators
    using TexFileNamesIterator      = std::vector<std::string>::iterator;
    using ConstTexFileNamesIterator = std::vector<std::string>::const_iterator;

    /* Constructors */

    /**
     * @brief Initializes the component with an empty vector of texture paths,
     * and an empty string as mesh base path.
     */
    TexturePaths() = default;

    /* Member functions */

    /**
     * @brief Returns the number of texture paths of the mesh.
     */
    uint textureNumber() const { return texPaths().size(); }

    /**
     * @brief Returns the path of the i-th texture of the mesh. The path is
     * relative to the mesh base path.
     * @param[in] i: The index of the texture.
     */
    const std::string& texturePath(uint i) const { return texPaths()[i]; }

    /**
     * @brief Returns a reference to the path of the i-th texture of the mesh.
     * The path is relative to the mesh base path.
     * @param[in] i: The index of the texture.
     */
    std::string& texturePath(uint i) { return texPaths()[i]; }

    /**
     * @brief Returns the mesh base path.
     */
    const std::string& meshBasePath() const { return Base::data().meshPath; }

    /**
     * @brief Returns a reference to the mesh base path.
     */
    std::string& meshBasePath() { return Base::data().meshPath; }

    /**
     * @brief Returns the index of the texture with the given path, or UINT_NULL
     * if the texture is not found.
     *
     * @param[in] path: the path of the texture to search for.
     */
    uint indexOfTexturePath(const std::string& path) const
    {
        auto it = std::find(texPaths().begin(), texPaths().end(), path);
        if (it != texPaths().end()) {
            return static_cast<uint>(std::distance(texPaths().begin(), it));
        }
        return UINT_NULL;
    }

    /**
     * @brief Clears the vector of texture paths.
     */
    void clearTexturePaths() { texPaths().clear(); }

    /**
     * @brief Adds a texture path to the vector of texture paths.
     * @param[in] textName: The texture path to add.
     */
    void pushTexturePath(const std::string& textName)
    {
        texPaths().push_back(textName);
    }

    /**
     * @brief Returns an iterator to the beginning of the vector of texture
     * paths.
     * @return an iterator to the beginning of the vector of texture paths.
     */
    TexFileNamesIterator texturePathBegin() { return texPaths().begin(); }

    /**
     * @brief Returns an iterator to the end of the vector of texture paths.
     * @return an iterator to the end of the vector of texture paths.
     */
    TexFileNamesIterator texturePathEnd() { return texPaths().end(); }

    /**
     * @brief Returns a const iterator to the beginning of the vector of texture
     * paths.
     * @return a const iterator to the beginning of the vector of texture paths.
     */
    ConstTexFileNamesIterator texturePathBegin() const
    {
        return texPaths().begin();
    }

    /**
     * @brief Returns a const iterator to the end of the vector of texture
     * paths.
     * @return a const iterator to the end of the vector of texture paths.
     */
    ConstTexFileNamesIterator texturePathEnd() const
    {
        return texPaths().end();
    }

    /**
     * @brief Returns a lightweigth view object that stores the begin and end
     * iterators of the vector of texture paths. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto& tpath : m.texturePaths()) {
     *     // Do something with tpath
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of texture paths.
     */
    View<TexFileNamesIterator> texturePaths()
    {
        return View(texturePathBegin(), texturePathEnd());
    }

    /**
     * @brief Returns a lightweigth const view object that stores the begin and
     * end iterators of the vector of texture paths. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (const auto& tpath : m.texturePaths()) {
     *     // Do something with tpath
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of texture paths.
     */
    View<ConstTexFileNamesIterator> texturePaths() const
    {
        return View(texturePathBegin(), texturePathEnd());
    }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        if constexpr (HasTexturePaths<Element>) {
            texPaths().clear();
            for (const auto& tex : e.textures()) {
                texPaths().push_back(tex.path());
            }
            meshBasePath() = e.meshBasePath();
        }
    }

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, texPaths());
        vcl::serialize(os, meshBasePath());
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, texPaths());
        vcl::deserialize(is, meshBasePath());
    }

private:
    // members that allow to access the data
    std::vector<std::string>& texPaths() { return Base::data().texPaths; }

    const std::vector<std::string>& texPaths() const
    {
        return Base::data().texPaths;
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
