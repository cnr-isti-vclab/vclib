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

#ifndef VCL_SPACE_COLOR_H
#define VCL_SPACE_COLOR_H

#include "point.h"

namespace vcl {

/**
 * @brief The Color class represents an RGBA color.
 *
 * @ingroup space
 */
class Color : public Point4<uint8_t>
{
public:
	/// ARGB enum with some standard colors.
	/// Can be used to initialize a color with an ARGB integer
	enum ColorRGBA {
		Black     = 0x000000ff,
		DarkGray  = 0x404040ff,
		Gray      = 0x808080ff,
		LightGray = 0xc0c0c0ff,
		White     = 0xffffffff,

		Red   = 0xff0000ff,
		Green = 0x00ff00ff,
		Blue  = 0x0000ffff,

		Cyan    = 0x00ffffff,
		Yellow  = 0xffff00ff,
		Magenta = 0xff00ffff,

		LightRed   = 0xff8080ff,
		LightGreen = 0x80ff80ff,
		LightBlue  = 0x8080ffff,

		LightCyan    = 0x80ffffff,
		LightYellow  = 0xffff80ff,
		LightMagenta = 0xff80ffff,

		DarkRed   = 0x400000ff,
		DarkGreen = 0x004000ff,
		DarkBlue  = 0x000040ff,

		DarkCyan    = 0x004040ff,
		DarkYellow  = 0x404000ff,
		DarkMagenta = 0x400040ff
	};

	/// List of Color Maps
	enum ColorMap {
		RedBlue,
		Parula,
		GreyShade
	};

	Color();
	Color(ColorRGBA cc);
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	Color(const Point4<uint8_t>& p);

	uint8_t red() const;
	uint8_t green() const;
	uint8_t blue() const;
	uint8_t alpha() const;

	uint8_t& red();
	uint8_t& green();
	uint8_t& blue();
	uint8_t& alpha();

	float redF() const;
	float greenF() const;
	float blueF() const;
	float alphaF() const;

	uint8_t hsvHue() const;
	uint8_t hsvSaturation() const;

	float hsvHueF() const;
	float hsvSaturationF() const;
	
	unsigned short toUnsignedR5G5B5() const;
	unsigned short toUnsignedB5G5R5() const;

	void setAlpha(uint8_t alpha);
	void setRed(uint8_t red);
	void setGreen(uint8_t green);
	void setBlue(uint8_t blue);

	void setRgb(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	void setHsv(uint h, uint8_t s, uint8_t v, uint8_t alpha = 255);

	void setAlphaF(float alpha);
	void setRedF(float red);
	void setGreenF(float green);
	void setBlueF(float blue);

	void setRgbF(float red, float green, float blue, float alpha = 1.0);
	void setHsvF(float hf, float sf, float vf, float alpha = 1.0);

	void setFromUnsignedR5G5B5(unsigned short val);
	void setFromUnsignedB5G5R5(unsigned short val);

	bool operator==(const Color& otherColor) const;
	bool operator!=(const Color& otherColor) const;
	bool operator<(const Color& otherColor) const;

	/// @private
	friend std::ostream& operator<<(std::ostream& out, const Color& p);
};

std::ostream& operator<<(std::ostream& out, const Color& p);

Color colorLerp(const Color& c0, const Color& c1, float value);

Color colorFromInterval(float value, Color::ColorMap cm);
Color colorFromInterval(float min, float max, float value, Color::ColorMap cm);

Color colorFromIntervalRedBlue(float value);
Color colorFromIntervalRedBlue(float min, float max, float value);

Color colorFromIntervalParula(float value);
Color colorFromIntervalParula(float min, float max, float value);

Color colorFromIntervalGreyShade(float value);
Color colorFromIntervalGreyShade(float min, float max, float value);

std::vector<Color> colorScattering(uint n, float sat = 0.3, float val = 0.9);

} // namespace vcl

#include "color.cpp"

#endif // VCL_SPACE_COLOR_H
