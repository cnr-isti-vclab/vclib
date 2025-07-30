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

#ifndef VCL_MESH_CONTAINERS_VERTICAL_COMPONENTS_VECTOR_TUPLE_H
#define VCL_MESH_CONTAINERS_VERTICAL_COMPONENTS_VECTOR_TUPLE_H

#include <vclib/concepts.h>
#include <vclib/miscellaneous.h>
#include <vclib/types.h>

#include <array>
#include <tuple>
#include <vector>

namespace vcl::mesh {

template<typename... Comp>
class VerticalComponentsVectorTuple
{
    using ComponentTypes = std::tuple<Comp...>;

    static constexpr uint COMP_NUMBER =
        std::tuple_size_v<std::tuple<std::vector<Comp>...>>;

    std::tuple<std::vector<typename Comp::DataValueType>...> mVecTuple;

    std::array<bool, COMP_NUMBER> mVecEnabled;
    std::size_t                   mSize = 0;

public:
    VerticalComponentsVectorTuple()
    {
        (setComponentEnabled<Comp, !comp::IsOptionalComponent<Comp>>(), ...);
    }

    static constexpr uint componentsNumber() { return COMP_NUMBER; }

    template<typename C>
    constexpr bool hasComponent() const
    {
        return indexOfType<C>() != UINT_NULL;
    }

    template<typename C>
    constexpr std::vector<typename C::DataValueType>& vector()
    {
        constexpr uint ind = indexOfType<C>();
        return std::get<ind>(mVecTuple);
    }

    template<typename C>
    constexpr const std::vector<typename C::DataValueType>& vector() const
    {
        constexpr uint ind = indexOfType<C>();
        return std::get<ind>(mVecTuple);
    }

    std::size_t size() const { return mSize; }

    void resize(std::size_t size)
    {
        if constexpr (componentsNumber() > 0) {
            vectorResize<componentsNumber() - 1>(size);
        }
        mSize = size;
    }

    void reserve(std::size_t size)
    {
        if constexpr (componentsNumber() > 0) {
            vectorReserve<componentsNumber() - 1>(size);
        }
    }

    void compact(const std::vector<uint>& newIndices)
    {
        if constexpr (componentsNumber() > 0) {
            vectorCompact<componentsNumber() - 1>(newIndices);
        }
    }

    void clear()
    {
        auto function = [](auto&... args) {
            ((args.clear()), ...);
        };

        std::apply(function, mVecTuple);
        mSize = 0;
    }

    void enableAllOptionalComponents()
    {
        (setComponentEnabledIfOptional<Comp, true>(), ...);
    }

    void disableAllOptionalComponents()
    {
        (setComponentEnabledIfOptional<Comp, false>(), ...);
    }

    template<typename C>
    bool isComponentEnabled() const
    {
        constexpr uint ind = indexOfType<C>();
        return mVecEnabled[ind];
    }

    template<uint COMP_ID>
    bool isComponentEnabled() const
    {
        using C = comp::ComponentOfType<COMP_ID, Comp...>;
        return isComponentEnabled<C>();
    }

    template<typename C>
    void enableComponent()
    {
        constexpr uint ind = indexOfType<C>();
        mVecEnabled[ind]   = true;
        vector<C>().resize(mSize);
    }

    template<uint COMP_ID>
    void enableComponent()
    {
        using C = comp::ComponentOfType<COMP_ID, Comp...>;
        enableComponent<C>();
    }

    template<typename C>
    void disableComponent()
    {
        constexpr uint ind = indexOfType<C>();
        mVecEnabled[ind]   = false;
        vector<C>().clear();
    }

    template<uint COMP_ID>
    void disableComponent()
    {
        using C = comp::ComponentOfType<COMP_ID, Comp...>;
        disableComponent<C>();
    }

private:
    template<typename C>
    static constexpr uint indexOfType()
    {
        return IndexInTypes<C, Comp...>::value;
    }

    template<std::size_t N>
    void vectorResize(std::size_t size)
    {
        if (mVecEnabled[N]) {
            std::get<N>(mVecTuple).resize(size);
        }
        if constexpr (N != 0)
            vectorResize<N - 1>(size);
    }

    template<std::size_t N>
    void vectorReserve(std::size_t size)
    {
        if (mVecEnabled[N]) {
            std::get<N>(mVecTuple).reserve(size);
        }
        if constexpr (N != 0)
            vectorReserve<N - 1>(size);
    }

    template<std::size_t N>
    void vectorCompact(const std::vector<uint>& newIndices)
    {
        if (mVecEnabled[N]) {
            compactVector(std::get<N>(mVecTuple), newIndices);
        }
        if constexpr (N != 0)
            vectorCompact<N - 1>(newIndices);
    }

    template<typename C, bool E>
    void setComponentEnabled()
    {
        if constexpr (E) {
            enableComponent<C>();
        }
        else {
            disableComponent<C>();
        }
    }

    template<typename C, bool E>
    void setComponentEnabledIfOptional()
    {
        if constexpr (comp::IsOptionalComponent<C>) {
            setComponentEnabled<C, E>();
        }
    }
};

/*
 * Crucial specialization - allows to catch components that are passed with a
 * TypeWrapper
 */
template<typename... Comp>
class VerticalComponentsVectorTuple<TypeWrapper<Comp...>> :
        public VerticalComponentsVectorTuple<Comp...>
{
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_VERTICAL_COMPONENTS_VECTOR_TUPLE_H
