/*
* Copyright 2013 Jeremie Roy. All rights reserved.
* License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
*/

#ifndef VCL_BGFX_TEXT_FONT_TEXT_METRICS_H
#define VCL_BGFX_TEXT_FONT_TEXT_METRICS_H

#include "font_manager.h"

namespace bgfx {

class TextMetrics
{
public:
	TextMetrics(FontManager* _fontManager);

	/// Append an ASCII/utf-8 string to the metrics helper.
	void appendText(FontHandle _fontHandle, const char* _string);

	/// Append a wide char string to the metrics helper.
	void appendText(FontHandle _fontHandle, const wchar_t* _string);

	/// Return the width of the measured text.
	float getWidth() const { return m_width; }

	/// Return the height of the measured text.
	float getHeight() const { return m_height; }

	/// Clear the width and height of the measured text.
	void clearText();

private:
	FontManager* m_fontManager;
	float m_width;
	float m_height;
	float m_x;
	float m_lineHeight;
	float m_lineGap;
};

/// Compute text crop area for text using a single font.
class TextLineMetrics
{
public:
	TextLineMetrics(const FontInfo& _fontInfo);

	/// Return the height of a line of text using the given font.
	float getLineHeight() const { return m_lineHeight; }

	/// Return the number of text line in the given text.
	uint32_t getLineCount(const bx::StringView& _string) const;

	/// Return the first and last character visible in the [_firstLine, _lastLine] range.
	void getSubText(const bx::StringView& _str, uint32_t _firstLine, uint32_t _lastLine, const char*& _begin, const char*& _end);

private:
	float m_lineHeight;
};

} // namespace bgfx

#endif // VCL_BGFX_TEXT_FONT_TEXT_METRICS_H
