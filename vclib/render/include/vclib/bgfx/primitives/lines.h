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

#ifndef VCL_BGFX_PRIMITIVES_LINES_H
#define VCL_BGFX_PRIMITIVES_LINES_H

#include <vclib/bgfx/primitives/lines/cpu_generated_lines.h>
#include <vclib/bgfx/primitives/lines/gpu_instancing_lines.h>
#include <vclib/bgfx/primitives/lines/primitive_lines.h>

#include <vclib/bgfx/uniform.h>

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <variant>

#include <bgfx/bgfx.h>

namespace vcl {

// TODO: add shading per-line (flat, per-line normal) (?)
/**
 * @brief The Lines class provides an abstraction for rendering 3D lines
 * with variable thickness and different implementation strategies.
 *
 * The Lines class supports several implementation types that are generally
 * determined by the hardware capabilities. Each implementation type has its
 * own advantages and disadvantages in terms of performance and memory usage.
 *
 * A line can be instantiated using std::vectors or bgfx buffers (VertexBuffer
 * and IndexBuffer). When using std::vectors, the buffers are created and
 * managed internally by the Lines class. When using bgfx buffers, the user
 * is responsible for creating and managing the buffers, and they must remain
 * valid for the lifetime of the Lines object (this is especially valid when
 * the implementation type is PRIMITIVE).
 */
class Lines
{
public:
    enum class ColorToUse {
        PER_VERTEX, // Select color form vertex color
        PER_EDGE,   // Select color from edge buffer color
        GENERAL     // Use general color in uniform data
    };

    enum class ImplementationType {
        PRIMITIVE      = 0, // Use bgfx primitive lines (not implemented)
        CPU_GENERATED  = 1, // Buffers pre-generated in CPU
        GPU_INSTANCING = 2, // Using Instancing with buffer generated in GPU
                            // computes

        // TODO: uncomment when they will be implemented
        // CPU_INSTANCING,    // Using Instancing with buffers generated in CPU
        // GPU_INSTANCING,    // Using Instancing with buffer generated in GPU
        //                    // computes
        // TEXTURE_INSTANCING, // Using Instancing with textures generated in
        // GPU computes

        COUNT
    };

private:
    using LinesImplementation = std::variant<
        detail::PrimitiveLines,
        detail::CPUGeneratedLines,
        detail::GPUInstancingLines>;

    float mThickness = 5.0f;

    // TODO: shading should become a enum with options: PER_VERTEX, PER_EDGE,
    // NONE
    // PER_EDGE means that we use a buffer of normals for each edge
    bool mShadingPerVertexCapability = false; // true if normals provided
    bool mShadingPerVertex           = false;

    BitSet8    mColorCapability = {false, false, true}; // general color only
    ColorToUse mColorToUse      = ColorToUse::GENERAL;
    Color      mGeneralColor    = Color::ColorABGR::LightGray;

    ImplementationType mType = ImplementationType::COUNT;

    Uniform mSettingUH = Uniform("u_settings", bgfx::UniformType::Vec4);
    LinesImplementation mLinesImplementation;

public:
    /**
     * @brief Creates a Lines object with default parameters and without
     * points.
     *
     * If the implementation type is not specified, it will be chosen
     * depending on the capabilities of the current hardware.
     *
     * @param[in] type: implementation type to use. If the provided type is
     * not supported by the current hardware, an exception is thrown. If the
     * type is COUNT, the default implementation type will be chosen
     * depending on the capabilities of the current hardware.
     */
    Lines(ImplementationType type = ImplementationType::COUNT)
    {
        if (type == ImplementationType::COUNT)
            type = defaultImplementationType(false);
        setImplementationType(type);
    };

    /**
     * @brief Creates a Lines object with given points and parameters.
     *
     * Each line is defined by two consecutive vertices in the vertCoords
     * vector (and related buffers). So the number of lines is equal to
     * vertCoords.size()/6.
     *
     * If the implementation type is not specified, it will be chosen
     * depending on the capabilities of the current hardware.
     *
     * @param[in] vertCoords: vector of vertex coordinates (3 floats per
     * vertex).
     * @param[in] vertNormals: vector of vertex normals (3 floats per vertex),
     * it can be empty.
     * @param[in] vertColors: vector of vertex colors (1 uint per vertex),
     * it can be empty.
     * @param[in] lineColors: vector of line colors (1 uint per line),
     * it can be empty.
     * @param[in] thickness: thickness of the lines (in pixels).
     * @param[in] shadingPerVertex: if true, the lighting is computed using
     * the vertex normals, otherwise no lighting is applied. If this settings
     * is not consistent with the provided buffers, an exception is thrown.
     * @param[in] colorToUse: specifies which color to use for rendering the
     * lines. If this settings is not consistent with the provided buffers,
     * an exception is thrown.
     * @param[in] type: implementation type to use. If the provided type is
     * not supported by the current hardware, an exception is thrown. If the
     * type is COUNT, the default implementation type will be chosen
     * depending on the capabilities of the current hardware.
     */
    Lines(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors,
        float                     thickness        = 5.0f,
        bool                      shadingPerVertex = false,
        ColorToUse                colorToUse       = ColorToUse::GENERAL,
        ImplementationType        type = ImplementationType::COUNT) :
            mThickness(thickness)
    {
        setPoints(vertCoords, vertNormals, vertColors, lineColors, type);
        setShading(shadingPerVertex);
        setColorToUse(colorToUse);
    }

    /**
     * @brief Creates a Lines object with given points and parameters.
     *
     * Each line is defined by two consecutive indices in the lineIndices
     * vector, which refer to vertices in the vertCoords vector (and related
     * buffers). So the number of lines is equal to lineIndices.size()/2.
     *
     * If the implementation type is not specified, it will be chosen
     * depending on the capabilities of the current hardware.
     *
     * @param[in] vertCoords: vector of vertex coordinates (3 floats per
     * vertex).
     * @param[in] lineIndices: vector of line indices (2 uint per line).
     * @param[in] vertNormals: vector of vertex normals (3 floats per vertex),
     * it can be empty.
     * @param[in] vertColors: vector of vertex colors (1 uint per vertex),
     * it can be empty.
     * @param[in] lineColors: vector of line colors (1 uint per line),
     * it can be empty.
     * @param[in] thickness: thickness of the lines (in pixels).
     * @param[in] shadingPerVertex: if true, the lighting is computed using
     * the vertex normals, otherwise no lighting is applied. If this settings
     * is not consistent with the provided buffers, an exception is thrown.
     * @param[in] colorToUse: specifies which color to use for rendering the
     * lines. If this settings is not consistent with the provided buffers,
     * an exception is thrown.
     * @param[in] type: implementation type to use. If the provided type is
     * not supported by the current hardware, an exception is thrown. If the
     * type is COUNT, the default implementation type will be chosen
     * depending on the capabilities of the current hardware.
     */
    Lines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors,
        float                     thickness        = 5.0f,
        bool                      shadingPerVertex = false,
        ColorToUse                colorToUse       = ColorToUse::GENERAL,
        ImplementationType        type = ImplementationType::COUNT) :
            mThickness(thickness)
    {
        setPoints(
            vertCoords, lineIndices, vertNormals, vertColors, lineColors, type);
        setShading(shadingPerVertex);
        setColorToUse(colorToUse);
    }

    Lines(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexNormals,
        const VertexBuffer& vertexColors ,
        const IndexBuffer&  lineColors,
        float                     thickness        = 5.0f,
        bool                      shadingPerVertex = false,
        ColorToUse                colorToUse       = ColorToUse::GENERAL,
        ImplementationType        type = ImplementationType::COUNT) :
            mThickness(thickness)
    {
        setPoints(pointsSize, vertexCoords, vertexNormals, vertexColors, lineColors, type);
        setShading(shadingPerVertex);
        setColorToUse(colorToUse);
    }

    Lines(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const IndexBuffer&  lineIndices,
        const VertexBuffer& vertexNormals,
        const VertexBuffer& vertexColors,
        const IndexBuffer&  lineColors,
        float                     thickness        = 5.0f,
        bool                      shadingPerVertex = false,
        ColorToUse                colorToUse       = ColorToUse::GENERAL,
        ImplementationType        type = ImplementationType::COUNT) :
            mThickness(thickness)
    {
        setPoints(pointsSize, vertexCoords, lineIndices, vertexNormals, vertexColors, lineColors, type);
        setShading(shadingPerVertex);
        setColorToUse(colorToUse);
    }

    /**
     * @brief Sets the points of the lines.
     *
     * Each line is defined by two consecutive vertices in the vertCoords
     * vector (and related buffers). So the number of lines is equal to
     * vertCoords.size()/6.
     *
     * If the implementation type is not specified, it will be chosen
     * depending on the capabilities of the current hardware.
     *
     * @param[in] vertCoords: vector of vertex coordinates (3 floats per
     * vertex).
     * @param[in] vertNormals: vector of vertex normals (3 floats per vertex),
     * it can be empty.
     * @param[in] vertColors: vector of vertex colors (1 uint per vertex),
     * it can be empty.
     * @param[in] lineColors: vector of line colors (1 uint per line),
     * it can be empty.
     * @param[in] type: implementation type to use. If the provided type is
     * not supported by the current hardware, an exception is thrown. If the
     * type is COUNT, the default implementation type will be chosen
     * depending on the capabilities of the current hardware.
     */
    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors,
        ImplementationType        type = ImplementationType::COUNT)
    {
        using enum ImplementationType;
        using namespace detail;

        if (type == COUNT)
            type = defaultImplementationType(true);
        setImplementationType(type);

        switch (mType) {
        case PRIMITIVE: // always supported
            std::get<PrimitiveLines>(mLinesImplementation)
                .setPoints(vertCoords, vertNormals, vertColors, lineColors);
            break;

        case CPU_GENERATED: // always supported
            std::get<CPUGeneratedLines>(mLinesImplementation)
                .setPoints(vertCoords, vertNormals, vertColors, lineColors);
            break;

        case GPU_INSTANCING:
            std::get<detail::GPUInstancingLines>(mLinesImplementation)
                .setPoints(vertCoords, vertNormals, vertColors, lineColors);
            break;
        default: break;
        }
        updateShadingCapability(vertNormals);
        updateColorCapability(vertColors, lineColors);
    }

    /**
     * @brief Sets the points of the lines.
     *
     * Each line is defined by two consecutive indices in the lineIndices
     * vector, which refer to vertices in the vertCoords vector (and related
     * buffers). So the number of lines is equal to lineIndices.size()/2.
     *
     * If the implementation type is not specified, it will be chosen
     * depending on the capabilities of the current hardware.
     *
     * @param[in] vertCoords: vector of vertex coordinates (3 floats per
     * vertex).
     * @param[in] lineIndices: vector of line indices (2 uint per line).
     * @param[in] vertNormals: vector of vertex normals (3 floats per vertex),
     * it can be empty.
     * @param[in] vertColors: vector of vertex colors (1 uint per vertex),
     * it can be empty.
     * @param[in] lineColors: vector of line colors (1 uint per line),
     * it can be empty.
     * @param[in] type: implementation type to use. If the provided type is
     * not supported by the current hardware, an exception is thrown. If the
     * type is COUNT, the default implementation type will be chosen
     * depending on the capabilities of the current hardware.
     */
    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors,
        ImplementationType        type = ImplementationType::COUNT)
    {
        using enum ImplementationType;
        using namespace detail;

        if (type == COUNT)
            type = defaultImplementationType(true);
        setImplementationType(type);

        switch (mType) {
        case PRIMITIVE: // always supported
            std::get<PrimitiveLines>(mLinesImplementation)
                .setPoints(
                    vertCoords,
                    lineIndices,
                    vertNormals,
                    vertColors,
                    lineColors);
            break;

        case CPU_GENERATED: // always supported
            std::get<CPUGeneratedLines>(mLinesImplementation)
                .setPoints(
                    vertCoords,
                    lineIndices,
                    vertNormals,
                    vertColors,
                    lineColors);
            break;

        case GPU_INSTANCING:
            std::get<detail::GPUInstancingLines>(mLinesImplementation)
                .setPoints(
                    vertCoords,
                    lineIndices,
                    vertNormals,
                    vertColors,
                    lineColors);
            break;
        default: break;
        }
        updateShadingCapability(vertNormals);
        updateColorCapability(vertColors, lineColors);
    }

    void setPoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors  = VertexBuffer(),
        const IndexBuffer&  lineColors    = IndexBuffer(),
        ImplementationType  type          = ImplementationType::COUNT)
    {
        IndexBuffer emptyIndices;
        setPoints(
            pointsSize,
            vertexCoords,
            emptyIndices,
            vertexNormals,
            vertexColors,
            lineColors,
            type);
    }

    void setPoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const IndexBuffer&  lineIndices,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors  = VertexBuffer(),
        const IndexBuffer&  lineColors    = IndexBuffer(),
        ImplementationType  type          = ImplementationType::COUNT)
    {
        using enum ImplementationType;
        using namespace detail;

        if (type == COUNT)
            type = ImplementationType::GPU_INSTANCING;
        setImplementationType(type);

        switch (mType) {
        case GPU_INSTANCING:
            std::get<detail::GPUInstancingLines>(mLinesImplementation)
                .setPoints(
                    pointsSize,
                    vertexCoords,
                    lineIndices,
                    vertexNormals,
                    vertexColors,
                    lineColors);
            break;
        default: break;
        }
        updateShadingCapability(vertexNormals);
        updateColorCapability(vertexColors, lineColors);
    }

    /**
     * @brief Returns the thickness of the lines (in pixels).
     * @return The thickness of the lines (in pixels).
     */
    float thickness() const { return mThickness; }

    /**
     * @brief Returns a reference to the thickness of the lines (in pixels).
     * This allows to modify the thickness directly.
     * @return A reference to the thickness of the lines (in pixels).
     */
    float& thickness() { return mThickness; }

    /**
     * @brief Returns true if shading is computed per vertex using vertex
     * normals, false if no shading is applied.
     * @return true if shading is computed per vertex using vertex normals.
     */
    bool shadingPerVertex() const { return mShadingPerVertex; }

    /**
     * @brief Returns the color that is used to render the lines.
     * @return The color that is used to render the lines.
     */
    ColorToUse colorToUse() const { return mColorToUse; }

    /**
     * @brief Returns the general color that is used to render the lines
     * when colorToUse() is set to ColorToUse::GENERAL.
     * @return The general color that is used to render the lines.
     */
    Color generalColor() const { return mGeneralColor; }

    /**
     * @brief Returns a reference to the general color that is used to render
     * the lines when colorToUse() is set to ColorToUse::GENERAL. This allows
     * to modify the color directly.
     * @return A reference to the general color that is used to render the
     * lines.
     */
    Color& generalColor() { return mGeneralColor; }

    /**
     * @brief Returns the current implementation type that is used to render
     * the lines.
     * @return The current implementation type that is used to render the
     * lines.
     */
    ImplementationType implementationType() const { return mType; }

    /**
     * @brief Sets wether to use per vertex shading (using vertex normals) or
     * not.
     *
     * If the provided setting is not consistent with the provided buffers,
     * an exception is thrown.
     *
     * @param[in] perVertex: if true, the lighting is computed using the vertex
     * normals, otherwise no lighting is applied. If this settings
     */
    void setShading(bool perVertex)
    {
        if (!mShadingPerVertexCapability && perVertex) {
            throw std::runtime_error(
                "Lines::setShading(): shading per vertex not supported by the "
                "current buffers.");
        }
        else {
            mShadingPerVertex = perVertex;
        }
    }

    /**
     * @brief Sets which color to use for rendering the lines.
     *
     * If the provided setting is not consistent with the provided buffers,
     * an exception is thrown.
     *
     * @param[in] color: specifies which color to use for rendering the lines.
     */
    void setColorToUse(ColorToUse color)
    {
        if (!mColorCapability[toUnderlying(color)]) {
            throw std::runtime_error(
                "Lines::setColorToUse(): color option not supported by the "
                "current buffers.");
        }
        else {
            mColorToUse = color;
        }
    }

    /**
     * @brief Draws in the given view the lines with the current settings.
     * @param[in] viewId: the view in which to draw the lines.
     */
    void draw(uint viewId) const
    {
        using enum ImplementationType;
        using namespace detail;

        bindSettingsUniform();
        if (mType == PRIMITIVE)
            std::get<PrimitiveLines>(mLinesImplementation).draw(viewId);

        if (mType == CPU_GENERATED)
            std::get<CPUGeneratedLines>(mLinesImplementation).draw(viewId);

        if (mType == GPU_INSTANCING)
            std::get<GPUInstancingLines>(mLinesImplementation).draw(viewId);
    }

private:
    /**
     * @brief Returns the default supported implementation type that can be
     * used, depending on the capabilities of the current hardware.
     * @param[in] cpuMemPointsProvided: true if point info of the lines is
     * provided trough CPU memory, false if it is provided through GPU buffers.
     * @return the default supported implementation type
     */
    ImplementationType defaultImplementationType(
        bool cpuMemPointsProvided) const
    {
        using enum ImplementationType;

        if (cpuMemPointsProvided)
            return CPU_GENERATED;
        else
            return PRIMITIVE;
    }

    bool setImplementationType(ImplementationType type)
    {
        using enum ImplementationType;

        if (mType == type)
            return false; // no change

        switch (type) {
        case PRIMITIVE: // always supported
            mLinesImplementation = detail::PrimitiveLines();
            mType                = type;
            return true;

        case CPU_GENERATED: // always supported
            mLinesImplementation = detail::CPUGeneratedLines();
            mType                = type;
            return true;

        case GPU_INSTANCING:
            mLinesImplementation = detail::GPUInstancingLines();
            mType                = type;
            return true;

        default: return false; // not supported
        }
    }

    void updateShadingCapability(const std::vector<float>& vertNormals)
    {
        mShadingPerVertexCapability = !vertNormals.empty();
        if (!mShadingPerVertexCapability)
            mShadingPerVertex = false;
    }

    void updateColorCapability(
        const std::vector<uint>& vertColors,
        const std::vector<uint>& lineColors)
    {
        using enum ColorToUse;

        mColorCapability[toUnderlying(PER_VERTEX)] = !vertColors.empty();
        mColorCapability[toUnderlying(PER_EDGE)]   = !lineColors.empty();

        if (!mColorCapability[toUnderlying(mColorToUse)])
            mColorToUse = GENERAL;
    }

    void updateShadingCapability(const VertexBuffer& vertexNormals)
    {
        mShadingPerVertexCapability = vertexNormals.isValid();
        if (!mShadingPerVertexCapability)
            mShadingPerVertex = false;
    }

    void updateColorCapability(
        const VertexBuffer& vertexColors,
        const IndexBuffer&  lineColors)
    {
        using enum ColorToUse;

        mColorCapability[toUnderlying(PER_VERTEX)] = vertexColors.isValid();
        mColorCapability[toUnderlying(PER_EDGE)]   = lineColors.isValid();

        if (!mColorCapability[toUnderlying(mColorToUse)])
            mColorToUse = GENERAL;
    }

    void bindSettingsUniform() const
    {
        float data[] = {
            mThickness,
            static_cast<float>(mColorToUse),
            std::bit_cast<float>(mGeneralColor.abgr()),
            static_cast<float>(mShadingPerVertex)};
        mSettingUH.bind(data);
    }
};

} // namespace vcl

#endif // VCL_BGFX_PRIMITIVES_LINES_H
