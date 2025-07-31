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

#ifndef VCL_MESH_COMPONENTS_TEXTURE_IMAGES_H
#define VCL_MESH_COMPONENTS_TEXTURE_IMAGES_H

#include "base/component.h"

#include <vclib/mesh/concepts/components/texture_images.h>

#include <vclib/space/core.h>
#include <vclib/types.h>

#include <string>
#include <vector>

namespace vcl::comp {

namespace detail {

struct TData
{
    std::vector<Texture> textures;
    std::string          meshPath;

    auto paths()
    {
        return textures | std::views::transform([](Texture& t) -> std::string& {
                   return t.path();
               });
    }

    auto paths() const
    {
        return textures | std::views::transform(
                              [](const Texture& t) -> const std::string& {
                                  return t.path();
                              });
    }

    auto pathBegin() { return std::begin(paths()); }

    auto pathBegin() const { return std::begin(paths()); }

    auto pathEnd() { return std::end(paths()); }

    auto pathEnd() const { return std::end(paths()); }
};

} // namespace detail

/**
 * @brief The TextureImages class represents a component that stores the
 * textures used by a mesh. This component makes sense only if it is used by
 * meshes, and therefore it cannot be stored vertically or be optional (no
 * template arguments are needed for these options).
 *
 * The TextureImages component stores a vector of textures that are composed by
 * an image and a path relative to the mesh path. The mesh path is stored as
 * well, and it is used to construct the absolute paths of the textures.
 *
 * The member functions of this class will be available in the instance of any
 * Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the TextureImages component, you'll
 * be able to access to this component member functions from `m`:
 *
 * @code{.cpp}
 * for (const auto& texture : m.textures()) {
 *     // do something with texture
 * }
 * @endcode
 *
 * @ingroup components
 */
class TextureImages :
        public Component<
            TextureImages,
            CompId::TEXTURE_IMAGES,
            detail::TData,
            void,
            false,
            false>
{
    using Base = Component<
        TextureImages,
        CompId::TEXTURE_IMAGES,
        detail::TData,
        void,
        false,
        false>;

public:
    /**
     * @brief Expose the type of the Texture.
     */
    using TextureType = Texture;

    // iterators
    using TextureIterator      = std::vector<Texture>::iterator;
    using ConstTextureIterator = std::vector<Texture>::const_iterator;

    using TexFileNamesIterator =
        decltype(std::declval<detail::TData>().pathBegin());
    using ConstTexFileNamesIterator =
        decltype(std::declval<const detail::TData>().pathBegin());

    /* Constructors */

    /**
     * @brief Initializes the component with an empty vector of textures
     * and an empty string as mesh base path.
     */
    TextureImages() = default;

    /* Member functions */

    /**
     * @brief Returns the number of texture of the mesh.
     */
    uint textureNumber() const { return texs().size(); }

    /**
     * @brief Returns the i-th texture of the mesh. The path of the texture is
     * relative to the mesh base path.
     * @param[in] i: The index of the texture.
     */
    const Texture& texture(uint i) const { return texs()[i]; }

    /**
     * @brief Returns a reference to the i-th texture of the mesh.
     * The path of the texture is relative to the mesh base path.
     * @param[in] i: The index of the texture.
     */
    Texture& texture(uint i) { return texs()[i]; }

    /**
     * @brief Returns the path of the i-th texture of the mesh. The path is
     * relative to the mesh base path.
     *
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `texture(uint).path()` member function instead.
     *
     * @param[in] i: The index of the texture.
     */
    const std::string& texturePath(uint i) const { return texs()[i].path(); }

    /**
     * @brief Returns a reference to the path of the i-th texture of the mesh.
     * The path is relative to the mesh base path.
     *
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `texture(uint).path()` member function instead.
     *
     * @param[in] i: The index of the texture.
     */
    std::string& texturePath(uint i) { return texs()[i].path(); }

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
        auto it =
            std::find(Base::data().pathBegin(), Base::data().pathEnd(), path);
        if (it != Base::data().pathEnd()) {
            return static_cast<uint>(
                std::distance(Base::data().pathBegin(), it));
        }
        return UINT_NULL;
    }

    /**
     * @brief Clears the vector of textures.
     */
    void clearTextures() { texs().clear(); }

    /**
     * @brief Clears the vector of textures.
     *
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `clearTextures()` member function instead.
     */
    void clearTexturePaths() { clearTextures(); }

    /**
     * @brief Adds a texture to the vector of textures.
     * @param[in] texture: The texture to add.
     */
    void pushTexture(const Texture& texture) { texs().push_back(texture); }

    /**
     * @brief Adds a texture to the vector of textures. The image of the
     * texture is left empty.
     *
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `pushTexture()` member function instead.
     *
     * @param[in] textPath: The texture path to add.
     */
    void pushTexturePath(const std::string& textPath)
    {
        texs().push_back(Texture());
        texs().back().path() = textPath;
    }

    /**
     * @brief Returns an iterator to the beginning of the vector of textures.
     * @return an iterator to the beginning of the vector of textures.
     */
    TextureIterator textureBegin() { return texs().begin(); }

    /**
     * @brief Returns an iterator to the end of the vector of textures.
     * @return an iterator to the end of the vector of textures.
     */
    TextureIterator textureEnd() { return texs().end(); }

    /**
     * @brief Returns a const iterator to the beginning of the vector of
     * textures.
     * @return a const iterator to the beginning of the vector of textures.
     */
    ConstTextureIterator textureBegin() const { return texs().begin(); }

    /**
     * @brief Returns a const iterator to the end of the vector of textures.
     * @return a const iterator to the end of the vector of textures.
     */
    ConstTextureIterator textureEnd() const { return texs().end(); }

    /**
     * @brief Returns an iterator to the beginning of the vector of texture
     * paths.
     *
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `textureBegin()` member function instead.
     *
     * @return an iterator to the beginning of the vector of texture paths.
     */
    TexFileNamesIterator texturePathBegin() { return Base::data().pathBegin(); }

    /**
     * @brief Returns an iterator to the end of the vector of texture paths.
     *
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `texturePathEnd()` member function instead.
     *
     * @return an iterator to the end of the vector of texture paths.
     */
    TexFileNamesIterator texturePathEnd() { return Base::data().pathEnd(); }

    /**
     * @brief Returns a const iterator to the beginning of the vector of texture
     * paths.
     *
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `textureBegin()` member function instead.
     *
     * @return a const iterator to the beginning of the vector of texture paths.
     */
    ConstTexFileNamesIterator texturePathBegin() const
    {
        return Base::data().pathBegin();
    }

    /**
     * @brief Returns a const iterator to the end of the vector of texture
     * paths.
     *
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `texturePathEnd()` member function instead.
     *
     * @return a const iterator to the end of the vector of texture paths.
     */
    ConstTexFileNamesIterator texturePathEnd() const
    {
        return Base::data().pathEnd();
    }

    /**
     * @brief Returns a lightweigth view object that stores the begin and end
     * iterators of the vector of textures. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (auto& texture : m.texturess()) {
     *     // Do something with texture
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of textures.
     */
    View<TextureIterator> textures()
    {
        return View(textureBegin(), textureEnd());
    }

    /**
     * @brief Returns a lightweigth const view object that stores the begin and
     * end iterators of the vector of textures. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (const auto& texture : m.textures()) {
     *     // Do something with texture
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of textures.
     */
    View<ConstTextureIterator> textures() const
    {
        return View(textureBegin(), textureEnd());
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
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `textures()` member function instead.
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of texture paths.
     */
    auto texturePaths() { return Base::data().paths(); }

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
     * @note This member function is provided for compatibility with the
     * TexturePaths component. It is recommended to use the
     * `textures()` member function instead.
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of texture paths.
     */
    auto texturePaths() const { return Base::data().paths(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        if constexpr (HasTextureImages<Element>) {
            texs()         = e.texs();
            meshBasePath() = e.meshBasePath();
        }
        else if constexpr (HasTexturePaths<Element>) {
            texs().clear();
            for (const auto& tpath : e.texturePaths()) {
                pushTexture(tpath);
            }
            meshBasePath() = e.meshBasePath();
        }
    }

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, texs());
        vcl::serialize(os, meshBasePath());
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, texs());
        vcl::deserialize(is, meshBasePath());
    }

private:
    // members that allow to access the data
    std::vector<Texture>& texs() { return Base::data().textures; }

    const std::vector<Texture>& texs() const { return Base::data().textures; }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TEXTURE_IMAGES_H
