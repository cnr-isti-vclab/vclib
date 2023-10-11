/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_CONTAINER_VERTICAL_COMPONENTS_VECTOR_TUPLE_H
#define VCL_MESH_CONTAINER_VERTICAL_COMPONENTS_VECTOR_TUPLE_H

#include <array>
#include <vector>
#include <tuple>

#include <vclib/concepts/mesh/components/component.h>
#include <vclib/misc/compactness.h>
#include <vclib/misc/tuple.h>
#include <vclib/types.h>

namespace vcl::mesh {

template<typename ...Comp>
class VerticalComponentsVectorTuple
{
    using ComponentTypes = std::tuple<Comp...>;

    static constexpr uint COMP_NUMBER =
        std::tuple_size_v<std::tuple<std::vector<Comp>...>>;

    std::tuple<std::vector<typename Comp::DataValueType>...> tuple;

    std::array<bool, COMP_NUMBER> vecEnabled;
    std::size_t siz = 0;

public:
    VerticalComponentsVectorTuple()
    {
        (setComponentEnabled<Comp, !vcl::comp::IsOptionalComponent<Comp>>(), ...);
    }

    static constexpr uint componentsNumber() { return COMP_NUMBER; }

    template<typename C>
    constexpr std::vector<typename C::DataValueType>& vector()
    {
        constexpr uint ind = indexOfType<C>();
        return std::get<ind>(tuple);
    }

    template<typename C>
    constexpr const std::vector<typename C::DataValueType>& vector() const
    {
        constexpr uint ind = indexOfType<C>();
        return std::get<ind>(tuple);
    }

    std::size_t size() const { return siz; }

    void resize(std::size_t size)
    {
        if constexpr (componentsNumber() > 0) {
            vectorResize<componentsNumber()-1>(size);
        }
        siz = size;
    }

    void reserve(std::size_t size)
    {
        if constexpr (componentsNumber() > 0) {
            vectorReserve<componentsNumber()-1>(size);
        }
    }

    void compact(const std::vector<uint>& newIndices)
    {
        if constexpr (componentsNumber() > 0) {
            vectorCompact<componentsNumber()-1>(newIndices);
        }
    }

    void clear()
    {
        auto function =
            [](auto&... args)
        {
            ((args.clear()), ...);
        };

        std::apply(function, tuple);
        siz = 0;
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
        return vecEnabled[ind];
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
        vecEnabled[ind] = true;
        vector<C>().resize(siz);
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
        vecEnabled[ind] = false;
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
        return vcl::IndexInTypes<C, Comp...>::value;
    }

    template<std::size_t N>
    void vectorResize(std::size_t size)
    {
        if (vecEnabled[N]) {
            std::get<N>(tuple).resize(size);
        }
        if constexpr (N != 0)
            vectorResize<N-1>(size);
    }

    template<std::size_t N>
    void vectorReserve(std::size_t size)
    {
        if (vecEnabled[N]) {
            std::get<N>(tuple).reserve(size);
        }
        if constexpr (N != 0)
            vectorReserve<N-1>(size);
    }

    template<std::size_t N>
    void vectorCompact(const std::vector<uint>& newIndices)
    {
        if (vecEnabled[N]) {
            vcl::compactVector(std::get<N>(tuple), newIndices);
        }
        if constexpr (N != 0)
            vectorCompact<N-1>(newIndices);
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
        if constexpr (vcl::comp::IsOptionalComponent<C>) {
            setComponentEnabled<C, E>();
        }
    }
};

/*
 * Crucial specialization - allows to catch components that are passed with a
 * TypeWrapper
 */
template<typename ...Comp>
class VerticalComponentsVectorTuple<TypeWrapper<Comp...>> :
        public VerticalComponentsVectorTuple<Comp...>
{
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINER_VERTICAL_COMPONENTS_VECTOR_TUPLE_H
