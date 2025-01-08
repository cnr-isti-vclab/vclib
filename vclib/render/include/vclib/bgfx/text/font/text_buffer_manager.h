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

/*
 * Copyright 2013 Jeremie Roy. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#ifndef VCL_BGFX_TEXT_FONT_TEXT_BUFFER_MANAGER_H
#define VCL_BGFX_TEXT_FONT_TEXT_BUFFER_MANAGER_H

#include "font_manager.h"

namespace bgfx {

BGFX_HANDLE(TextBufferHandle)

#define MAX_TEXT_BUFFER_COUNT 64

/// type of vertex and index buffer to use with a TextBuffer
struct BufferType
{
	enum Enum
	{
		Static,
		Dynamic,
		Transient,
	};
};

/// special style effect (can be combined)
enum TextStyleFlags
{
	STYLE_NORMAL = 0,
	STYLE_OVERLINE = 1,
	STYLE_UNDERLINE = 1 << 1,
	STYLE_STRIKE_THROUGH = 1 << 2,
	STYLE_BACKGROUND = 1 << 3,
};

struct TextRectangle
{
	float width, height;
};

class TextBuffer;
class TextBufferManager
{
public:
	TextBufferManager(FontManager* _fontManager);
	~TextBufferManager();

	TextBufferHandle createTextBuffer(uint32_t _type, BufferType::Enum _bufferType);
	void destroyTextBuffer(TextBufferHandle _handle);
	void submitTextBuffer(TextBufferHandle _handle, bgfx::ViewId _id, int32_t _depth = 0);

	void setStyle(TextBufferHandle _handle, uint32_t _flags = STYLE_NORMAL);
	void setTextColor(TextBufferHandle _handle, uint32_t _rgba = 0x000000FF);
	void setBackgroundColor(TextBufferHandle _handle, uint32_t _rgba = 0x000000FF);

	void setOverlineColor(TextBufferHandle _handle, uint32_t _rgba = 0x000000FF);
	void setUnderlineColor(TextBufferHandle _handle, uint32_t _rgba = 0x000000FF);
	void setStrikeThroughColor(TextBufferHandle _handle, uint32_t _rgba = 0x000000FF);

	void setOutlineWidth(TextBufferHandle _handle, float _outlineWidth = 3.0f);
	void setOutlineColor(TextBufferHandle _handle, uint32_t _rgba = 0x000000FF);

	void setDropShadowOffset(TextBufferHandle _handle, float _u, float _v);
	void setDropShadowColor(TextBufferHandle _handle, uint32_t _rgba = 0x000000FF);
	void setDropShadowSoftener(TextBufferHandle _handle, float smoother = 1.0f);

	void setPenPosition(TextBufferHandle _handle, float _x, float _y);

	/// Append an ASCII/utf-8 string to the buffer using current pen position and color.
	void appendText(TextBufferHandle _handle, FontHandle _fontHandle, const char* _string, const char* _end = NULL);

	/// Append a wide char unicode string to the buffer using current pen position and color.
	void appendText(TextBufferHandle _handle, FontHandle _fontHandle, const wchar_t* _string, const wchar_t* _end = NULL);

	/// Append a whole face of the atlas cube, mostly used for debugging and visualizing atlas.
	void appendAtlasFace(TextBufferHandle _handle, uint16_t _faceIndex);

	/// Clear the text buffer and reset its state (pen/color).
	void clearTextBuffer(TextBufferHandle _handle);

	/// Return the rectangular size of the current text buffer (including all its content).
	TextRectangle getRectangle(TextBufferHandle _handle) const;

private:
	struct BufferCache
	{
		uint16_t indexBufferHandleIdx;
		uint16_t vertexBufferHandleIdx;
		TextBuffer* textBuffer;
		BufferType::Enum bufferType;
		uint32_t fontType;
	};

	BufferCache* m_textBuffers;
	bx::HandleAllocT<MAX_TEXT_BUFFER_COUNT> m_textBufferHandles;
	FontManager* m_fontManager;
	bgfx::VertexLayout m_vertexLayout;
	bgfx::UniformHandle s_texColor;
	bgfx::UniformHandle u_dropShadowColor;
	bgfx::UniformHandle u_params;
};

} // namespace bgfx

#endif // VCL_BGFX_TEXT_FONT_TEXT_BUFFER_MANAGER_H
