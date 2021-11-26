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

#include "color.h"

#ifdef QT_GUI_LIB

inline bool operator<(const QColor& c1, const QColor& c2)
{
	if (c1.red() < c2.red())
		return true;
	if (c1.red() > c2.red())
		return false;
	if (c1.green() < c2.green())
		return true;
	if (c1.green() > c2.green())
		return false;
	if (c1.blue() < c2.blue())
		return true;
	if (c1.blue() > c2.blue())
		return false;
	if (c1.alpha() < c2.alpha())
		return true;
	return false;
}

#else

namespace vcl {

/**
 * @brief Color::Color
 * Default constructor.
 * Initializes w() black color (with alpha 255).
 */
inline Color::Color() : Point4(0, 0, 0, 255)
{
}

/**
 * @brief Color::Color
 * Color constructor.
 * @param[in] red: red component
 * @param[in] green: green component
 * @param[in] blue: blue component
 * @param[in] alpha: alpha component (default 255)
 */
inline Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
		Point4(red, green, blue, alpha)
{
}

/**
 * @brief Color::red
 * Returns the red component of this color [0-255]
 * @return red component of this color
 */
inline int Color::red() const
{
	return x();
}

/**
 * @brief Color::green
 * Returns the green component of this color [0-255]
 * @return green component of this color
 */
inline int Color::green() const
{
	return y();
}

/**
 * @brief Color::blue
 * Returns the blue component of this color [0-255]
 * @return blue component of this color
 */
inline int Color::blue() const
{
	return z();
}

/**
 * @brief Color::alpha
 * Returns the alpha component of this color [0-255]
 * @return alpha component of this color
 */
inline int Color::alpha() const
{
	return w();
}

/**
 * @brief Color::redF
 * Returns the float red component of this color [0-1]
 * @return float red component of this color
 */
inline float Color::redF() const
{
	return (float) x() / 255;
}

/**
 * @brief Color::greenF
 * Returns the float green component of this color [0-1]
 * @return float green component of this color
 */
inline float Color::greenF() const
{
	return (float) y() / 255;
}

/**
 * @brief Color::blueF
 * Returns the float blue component of this color [0-1]
 * @return float blue component of this color
 */
inline float Color::blueF() const
{
	return (float) z() / 255;
}

/**
 * @brief Color::alphaF
 * Returns the float alpha component of this color [0-1]
 * @return float alpha component of this color
 */
inline float Color::alphaF() const
{
	return (float) w() / 255;
}

/**
 * @brief Color::hsvHue
 * Returns the hue color component of this color [0-359]
 * @return hue color component of this color
 */
inline int Color::hsvHue() const
{
	uint8_t rgbMin, rgbMax;
	uint8_t h;

	rgbMin = x() < y() ? (x() < z() ? x() : z()) : (y() < z() ? y() : z());
	rgbMax = x() > y() ? (x() > z() ? x() : z()) : (y() > z() ? y() : z());

	if (rgbMax == 0) {
		return 0;
	}

	if (255 * long(rgbMax - rgbMin) / rgbMax == 0) {
		return 0;
	}

	if (rgbMax == x())
		h = 0 + 43 * (y() - z()) / (rgbMax - rgbMin);
	else if (rgbMax == y())
		h = 85 + 43 * (z() - x()) / (rgbMax - rgbMin);
	else
		h = 171 + 43 * (x() - y()) / (rgbMax - rgbMin);

	return h;
}

/**
 * @brief Color::hsvSaturation
 * Returns the saturation color component of this color [0-255]
 * @return saturation color component of this color
 */
inline int Color::hsvSaturation() const
{
	uint8_t rgbMin, rgbMax;

	rgbMin = x() < y() ? (x() < z() ? x() : z()) : (y() < z() ? y() : z());
	rgbMax = x() > y() ? (x() > z() ? x() : z()) : (y() > z() ? y() : z());

	if (rgbMax == 0) {
		return 0;
	}

	return 255 * long(rgbMax - rgbMin) / rgbMax;
}

/**
 * @brief Color::hsvHueF
 * Returns the float saturation color component of this color [0-1]
 * @return float saturation color component of this color
 */
inline float Color::hsvHueF() const
{
	return (float) hsvHue() / 255;
}

/**
 * @brief Color::hsvSaturationF
 * Returns the float saturation color component of this color [0-1]
 * @return float saturation color component of this color
 */
inline float Color::hsvSaturationF() const
{
	return (float) hsvSaturation() / 255;
}

/**
 * @brief Color::setAlpha
 * Sets the alpha of this color [0-255]
 * @param[in] alpha
 */
inline void Color::setAlpha(uint8_t alpha)
{
	w() = alpha;
}

/**
 * @brief Color::setRed
 * Sets the red of this color [0-255]
 * @param[in] red
 */
inline void Color::setRed(uint8_t red)
{
	x() = red;
}

/**
 * @brief Color::setGreen
 * Sets the green of this color [0-255]
 * @param[in] green
 */
inline void Color::setGreen(uint8_t green)
{
	y() = green;
}

/**
 * @brief Color::setBlue
 * Sets the blue of this color [0-255]
 * @param[in] blue
 */
inline void Color::setBlue(uint8_t blue)
{
	z() = blue;
}

/**
 * @brief Color::setRgb
 * Sets the RGB values of this color.
 * All the values must be in the range 0-255.
 * @param[in] red
 * @param[in] green
 * @param[in] blue
 * @param[in] alpha: default 255
 */
inline void Color::setRgb(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	x() = red;
	y() = green;
	z() = blue;
	w() = alpha;
}

/**
 * @brief Color::setHsv
 * Sets the HSV values of this color.
 * All the values must be in the range 0-255.
 * @param[in] h
 * @param[in] s
 * @param[in] v
 * @param[in] alpha: default 255
 */
inline void Color::setHsv(uint h, uint8_t s, uint8_t v, uint8_t alpha)
{
	w() = alpha;
	if (s == 0) {
		x() = v;
		y() = v;
		z() = v;
	}
	else {
		h = (h / 360.0) * 255;
		uint8_t region, remainder, p, q, t;
		region    = h / 43;
		remainder = (h - (region * 43)) * 6;

		p = (v * (255 - s)) >> 8;
		q = (v * (255 - ((s * remainder) >> 8))) >> 8;
		t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

		switch (region) {
		case 0:
			x() = v;
			y() = t;
			z() = p;
			break;
		case 1:
			x() = q;
			y() = v;
			z() = p;
			break;
		case 2:
			x() = p;
			y() = v;
			z() = t;
			break;
		case 3:
			x() = p;
			y() = q;
			z() = v;
			break;
		case 4:
			x() = t;
			y() = p;
			z() = v;
			break;
		default:
			x() = v;
			y() = p;
			z() = q;
			break;
		}
	}
}

/**
 * @brief Color::setAlphaF
 * Sets the alpha of this color [0-1]
 * @param[in] alpha
 */
inline void Color::setAlphaF(float alpha)
{
	w() = (uint8_t) (alpha * 255);
}

/**
 * @brief Color::setRedF
 * Sets the red of this color [0-1]
 * @param[in] red
 */
inline void Color::setRedF(float red)
{
	x() = (uint8_t) (red * 255);
}

/**
 * @brief Color::setGreenF
 * Sets the green of this color [0-1]
 * @param[in] green
 */
inline void Color::setGreenF(float green)
{
	y() = (uint8_t) (green * 255);
}

/**
 * @brief Color::setBlueF
 * Sets the blue of this color [0-1]
 * @param[in] blue
 */
inline void Color::setBlueF(float blue)
{
	z() = (uint8_t) (blue * 255);
}

/**
 * @brief Color::setRgbF
 * Sets the RGB values of this color.
 * All the values must be in the range 0-1.
 * @param[in] red
 * @param[in] green
 * @param[in] blue
 * @param[in] alpha: default 1
 */
inline void Color::setRgbF(float red, float green, float blue, float alpha)
{
	w() = (uint8_t) (alpha * 255);
	x() = (uint8_t) (red * 255);
	y() = (uint8_t) (green * 255);
	z() = (uint8_t) (blue * 255);
}

/**
 * @brief Color::setHsvF
 * Sets the HSV values of this color.
 * All the values must be in the range 0-1.
 * @param[in] hf
 * @param[in] sf
 * @param[in] vf
 * @param[in] alpha
 */
inline void Color::setHsvF(float hf, float sf, float vf, float alpha)
{
	setHsv((int) hf * 255, (int) sf * 255, (int) vf * 255, (int) alpha * 255);
}

/**
 * @brief Color::operatox() ==
 * Returns true if this color has the same RGB and alpha values
 * as otherColor; otherwise returns false.
 * @param otherColor
 * @return
 */
inline bool Color::operator==(const Color& otherColor) const
{
	return (
		x() == otherColor.x() && y() == otherColor.y() && z() == otherColor.z() &&
		w() == otherColor.w());
}

/**
 * @brief Color::operator !=
 * Returns false if this color has the same RGB and alpha values
 * as otherColor; otherwise returns true.
 * @param otherColor
 * @return
 */
inline bool Color::operator!=(const Color& otherColor) const
{
	return !(*this == otherColor);
}

/**
 * @brief Color::operator <
 * Returns true if this color is less than otherColor follwing
 * the RGBA order; otherwise returns false.
 * @param otherColor
 * @return
 */
inline bool Color::operator<(const Color& otherColor) const
{
	if (x() < otherColor.x())
		return true;
	if (x() > otherColor.x())
		return false;
	if (y() < otherColor.y())
		return true;
	if (y() > otherColor.y())
		return false;
	if (z() < otherColor.z())
		return true;
	if (z() > otherColor.z())
		return false;
	if (w() < otherColor.w())
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& out, const Color& c)
{
	out << c.p.cast<uint>();
	return out;
}

} // namespace vcl
#endif
