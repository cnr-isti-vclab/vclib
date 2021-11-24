/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifdef QT_GUI_LIB

#include <QColor>

namespace vcl {

using Color = QColor;

} // namespace vcl

bool operator<(const QColor& c1, const QColor& c2);

#else

#include "point/point4.h"

namespace vcl {

class Color : public Point4<uint8_t>
{
public:
	Color();
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	int   red() const;
	int   green() const;
	int   blue() const;
	int   alpha() const;
	float redF() const;
	float greenF() const;
	float blueF() const;
	float alphaF() const;
	int   hsvHue() const;
	int   hsvSaturation() const;
	float hsvHueF() const;
	float hsvSaturationF() const;
	void  setAlpha(uint8_t alpha);
	void  setRed(uint8_t red);
	void  setGreen(uint8_t green);
	void  setBlue(uint8_t blue);
	void  setRgb(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	void  setHsv(uint h, uint8_t s, uint8_t v, uint8_t alpha = 255);
	void  setAlphaF(float alpha);
	void  setRedF(float red);
	void  setGreenF(float green);
	void  setBlueF(float blue);
	void  setRgbF(float red, float green, float blue, float alpha = 1.0);
	void  setHsvF(float hf, float sf, float vf, float alpha = 1.0);

	bool operator==(const Color& otherColor) const;
	bool operator!=(const Color& otherColor) const;
	bool operator<(const Color& otherColor) const;

	friend std::ostream& operator<<(std::ostream& out, const Color& p);
};

std::ostream& operator<<(std::ostream& out, const Color& p);

} // namespace vcl

#include "color.cpp"

#endif

#endif // VCL_COLOR_H
