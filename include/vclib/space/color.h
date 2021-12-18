/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_COLOR_H
#define VCL_COLOR_H

#include "point/point4.h"

namespace vcl {

/**
 * @brief The Color class represents an RGBA color.
 */
class Color : public Point4<uint8_t>
{
public:
	/// ARGB enum with some standard colors.
	/// Can be used to initialize a color with an ARGB integer
	enum ColorARGB {
		Black = 0xff000000,
		Gray  = 0xff808080,
		White = 0xffffffff,

		Red   = 0xff0000ff,
		Green = 0xff00ff00,
		Blue  = 0xffff0000,

		Cyan    = 0xffffff00,
		Yellow  = 0xff00ffff,
		Magenta = 0xffff00ff,

		LightGray  = 0xffc0c0c0,
		LightRed   = 0xff8080ff,
		LightGreen = 0xff80ff80,
		LightBlue  = 0xffff8080,

		DarkGray  = 0xff404040,
		DarkRed   = 0xff000040,
		DarkGreen = 0xff004000,
		DarkBlue  = 0xff400000
	};

	/// List of Color Maps
	enum ColorMap {
		RedBlue,
		Parula,
		GreyShade
	};

	Color();
	Color(ColorARGB cc);
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);

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

} // namespace vcl

#include "color.cpp"

#endif // VCL_COLOR_H
