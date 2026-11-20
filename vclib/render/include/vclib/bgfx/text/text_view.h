// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_TEXT_TEXT_VIEW_H
#define VCL_BGFX_TEXT_TEXT_VIEW_H

#include "../context.h"
#include "text_manager.h"

#include <unordered_map>

namespace vcl {

/**
 * @brief Manages 2D screen-space text rendering as an overlay on top of bgfx
 * views.
 *
 * In bgfx, all rendering operations are submitted to views (identified by
 * bgfx::ViewId), each defining an independent pass with its own viewport,
 * target framebuffer, and view/projection matrices.
 *
 * TextView represents a 2D text overlay layer. It bridges 3D/canvas rendering
 * passes with 2D text rendering (such as mesh info labels, HUD elements, and
 * screen annotations):
 *
 * - **View Association & Multiplexing**: Each parent view (e.g., a 3D scene
 * view) can have an associated overlay text view managed by this class. When
 * @ref frame is called with a parent view ID and framebuffer handle, TextView
 * assigns or reuses a dedicated bgfx::ViewId that renders into the same target
 * framebuffer on top of the parent view.
 *
 * - **Screen-Space Coordinate System**: TextView maintains an orthographic
 * projection matrix where coordinates correspond to pixel positions on the
 * screen, with
 *   `(0, 0)` at the top-left and `(width, height)` at the bottom-right.
 *
 * - **Text Buffering**: Through an internal @ref TextManager, TextView supports
 * both persistent text (@ref appendStaticText, preserved until @ref clearText
 * is called) and per-frame text (@ref appendTransientText, cleared after each
 * frame submission).
 *
 * - **Resource & Lifecycle Management**: TextView allocates secondary view IDs
 * from the global Context on demand and automatically releases them when text
 * is disabled (@ref enableText(false)) or upon destruction.
 */
class TextView
{
    TextManager mTextManager;

    // association between the parent view and the text view
    // (each parent view has its own text view, so that the text can be rendered
    // on top of it)
    std::unordered_map<bgfx::ViewId, bgfx::ViewId> mViews;

    bool  mTextEnabled = false;
    float mTextViewMatrix[16];
    float mTextProjMatrix[16];
    uint  mWidth  = 0;
    uint  mHeight = 0;

public:
    /**
     * @brief Constructs an uninitialized TextView.
     */
    TextView();

    /**
     * @brief Destructor. Disables text rendering and releases all managed
     * bgfx views back to the Context.
     */
    ~TextView();

    /**
     * @brief Initializes the text view dimensions, camera matrices, and the
     * internal TextManager.
     *
     * @param[in] width: Viewport width in pixels.
     * @param[in] height: Viewport height in pixels.
     */
    void init(uint width, uint height);

    /**
     * @brief Enables or disables text rendering.
     *
     * When disabling text rendering, all allocated overlay view IDs are
     * released back to the Context.
     *
     * @param[in] b: True to enable text rendering, false to disable.
     */
    void enableText(bool b = true);

    /**
     * @brief Checks if text rendering is currently enabled.
     *
     * @return True if text rendering is enabled, false otherwise.
     */
    bool isTextEnabled() const;

    /**
     * @brief Sets the active font using an embedded font enum and specifies its
     * size.
     *
     * @param[in] font: The embedded font identifier.
     * @param[in] fontSize: The font size in pixels/points.
     */
    void setTextFont(VclFont::Enum font, uint fontSize);

    /**
     * @brief Sets the active font by name and specifies its size.
     *
     * @param[in] fontName: The name of the registered font.
     * @param[in] fontSize: The font size in pixels/points.
     */
    void setTextFont(const std::string& fontName, uint fontSize);

    /**
     * @brief Clears all text currently stored in the text buffers.
     */
    void clearText();

    /**
     * @brief Appends static (persistent) text to be drawn at the given 2D
     * screen coordinates.
     *
     * Static text remains in the buffer and is drawn every frame until
     * @ref clearText() is called.
     *
     * @param[in] pos: 2D screen-space position (in pixels).
     * @param[in] text: The text string to render.
     * @param[in] color: Text color (defaults to black).
     */
    void appendStaticText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    /**
     * @brief Appends transient (single-frame) text to be drawn at the given 2D
     * screen coordinates.
     *
     * Transient text is rendered for the current frame and automatically
     * discarded after submission.
     *
     * @param[in] pos: 2D screen-space position (in pixels).
     * @param[in] text: The text string to render.
     * @param[in] color: Text color (defaults to black).
     */
    void appendTransientText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    /**
     * @brief Renders the text overlay for a given parent view and framebuffer.
     *
     * If an overlay view has not yet been allocated for @p parentViewId, one is
     * requested from the Context. The overlay view is configured with the
     * target framebuffer @p fbh and the 2D screen-space transform, and text
     * draw calls are submitted.
     *
     * @param[in] parentViewId: The view ID of the parent view to overlay text
     * on.
     * @param[in] fbh: The framebuffer handle targeted by the parent view.
     */
    void frame(bgfx::ViewId parentViewId, bgfx::FrameBufferHandle fbh);

    /**
     * @brief Updates the viewport size and recalculates the projection matrix.
     *
     * Also updates the view rectangle for all currently active overlay views.
     *
     * @param[in] width: New viewport width in pixels.
     * @param[in] height: New viewport height in pixels.
     */
    void resize(uint width, uint height);

private:
    /**
     * @brief Recalculates the 2D orthographic projection matrix based on
     * current dimensions and bgfx depth capabilities.
     */
    void updateProjMatrix();
};

} // namespace vcl

#endif // VCL_BGFX_TEXT_TEXT_VIEW_H
