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

namespace vcl {

/**
 * @brief Default constructor.
 * Initializes w() black color (with alpha 255).
 */
inline Color::Color() : Point4(0, 0, 0, 255)
{
}

Color::Color(ColorARGB cc)
{
	x() = cc % 256;
	y() = (cc >> 8) % 256;
	z() = (cc >> 16) % 256;
	w() = (cc >> 24) % 256;
}

/**
 * @brief Color constructor.
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
 * @brief Returns the red component of this color [0-255]
 * @return red component of this color
 */
inline uint8_t Color::red() const
{
	return x();
}

/**
 * @brief Returns the green component of this color [0-255]
 * @return green component of this color
 */
inline uint8_t Color::green() const
{
	return y();
}

/**
 * @brief Returns the blue component of this color [0-255]
 * @return blue component of this color
 */
inline uint8_t Color::blue() const
{
	return z();
}

/**
 * @brief Returns the alpha component of this color [0-255]
 * @return alpha component of this color
 */
inline uint8_t Color::alpha() const
{
	return w();
}

/**
 * @brief Returns the red component of this color [0-255]
 * @return red component of this color
 */
inline uint8_t& Color::red()
{
	return x();
}

/**
 * @brief Returns the green component of this color [0-255]
 * @return green component of this color
 */
inline uint8_t& Color::green()
{
	return y();
}

/**
 * @brief Returns the blue component of this color [0-255]
 * @return blue component of this color
 */
inline uint8_t& Color::blue()
{
	return z();
}

/**
 * @brief Returns the alpha component of this color [0-255]
 * @return alpha component of this color
 */
inline uint8_t& Color::alpha()
{
	return w();
}

/**
 * @brief Returns the float red component of this color [0-1]
 * @return float red component of this color
 */
inline float Color::redF() const
{
	return (float) x() / 255;
}

/**
 * @brief Returns the float green component of this color [0-1]
 * @return float green component of this color
 */
inline float Color::greenF() const
{
	return (float) y() / 255;
}

/**
 * @brief Returns the float blue component of this color [0-1]
 * @return float blue component of this color
 */
inline float Color::blueF() const
{
	return (float) z() / 255;
}

/**
 * @brief Returns the float alpha component of this color [0-1]
 * @return float alpha component of this color
 */
inline float Color::alphaF() const
{
	return (float) w() / 255;
}

/**
 * @brief Returns the hue color component of this color [0-359]
 * @return hue color component of this color
 */
inline uint8_t Color::hsvHue() const
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
 * @brief Returns the saturation color component of this color [0-255]
 * @return saturation color component of this color
 */
inline uint8_t Color::hsvSaturation() const
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
 * @brief Returns the float saturation color component of this color [0-1]
 * @return float saturation color component of this color
 */
inline float Color::hsvHueF() const
{
	return (float) hsvHue() / 255;
}

/**
 * @brief Returns the float saturation color component of this color [0-1]
 * @return float saturation color component of this color
 */
inline float Color::hsvSaturationF() const
{
	return (float) hsvSaturation() / 255;
}

/**
 * @brief Sets the alpha of this color [0-255]
 * @param[in] alpha
 */
inline void Color::setAlpha(uint8_t alpha)
{
	w() = alpha;
}

/**
 * @brief Sets the red of this color [0-255]
 * @param[in] red
 */
inline void Color::setRed(uint8_t red)
{
	x() = red;
}

/**
 * @brief Sets the green of this color [0-255]
 * @param[in] green
 */
inline void Color::setGreen(uint8_t green)
{
	y() = green;
}

/**
 * @brief Sets the blue of this color [0-255]
 * @param[in] blue
 */
inline void Color::setBlue(uint8_t blue)
{
	z() = blue;
}

/**
 * @brief Sets the RGB values of this color.
 *
 * All the values must be in the range 0-255.
 *
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
 * @brief Sets the HSV values of this color.
 *
 * All the values must be in the range 0-255.
 *
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
 * @brief Sets the alpha of this color [0-1]
 * @param[in] alpha
 */
inline void Color::setAlphaF(float alpha)
{
	w() = (uint8_t) (alpha * 255);
}

/**
 * @brief Sets the red of this color [0-1]
 * @param[in] red
 */
inline void Color::setRedF(float red)
{
	x() = (uint8_t) (red * 255);
}

/**
 * @brief Sets the green of this color [0-1]
 * @param[in] green
 */
inline void Color::setGreenF(float green)
{
	y() = (uint8_t) (green * 255);
}

/**
 * @brief Sets the blue of this color [0-1]
 * @param[in] blue
 */
inline void Color::setBlueF(float blue)
{
	z() = (uint8_t) (blue * 255);
}

/**
 * @brief Sets the RGB values of this color.
 *
 * All the values must be in the range 0-1.
 *
 * @param[in] red: red component of the color in as a float value between 0 and 1.
 * @param[in] green: green component of the color in as a float value between 0 and 1.
 * @param[in] blue: blue component of the color in as a float value between 0 and 1.
 * @param[in] alpha: : alpha component of the color in as a float value between 0 and 1, default
 * value is 1.
 */
inline void Color::setRgbF(float red, float green, float blue, float alpha)
{
	w() = (uint8_t) (alpha * 255);
	x() = (uint8_t) (red * 255);
	y() = (uint8_t) (green * 255);
	z() = (uint8_t) (blue * 255);
}

/**
 * @brief Sets the HSV values of this color.
 *
 * All the values must be in the range 0-1.
 * The color will be converted in RGBA and then stored in this color.
 *
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
 * @brief Returns true if this color has the same RGB and alpha values
 * as otherColor; otherwise returns false.
 *
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
 * @brief Returns false if this color has the same RGB and alpha values as otherColor; otherwise
 * returns true.
 *
 * @param otherColor
 * @return
 */
inline bool Color::operator!=(const Color& otherColor) const
{
	return !(*this == otherColor);
}

/**
 * @brief Returns true if this color is less than otherColor follwing the RGBA order; otherwise
 * returns false.
 *
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

/**
 * @brief Overload of stream operator to allow a pretty print of a vcl::Color.
 */
inline std::ostream& operator<<(std::ostream& out, const Color& c)
{
	out << c.p.cast<uint>();
	return out;
}

inline Color colorLerp(const Color& c0, const Color& c1, float value)
{
	Color c;
	if (value < 0) // out of range - left
		value = 0;
	else if (value > 1) // out of range - right
		value = 1;
	for (uint i = 0; i < 4; i++)
		c(i) = c1(i)*value + c0(i)*(1-value);
	return c;
}

inline Color colorFromInterval(float value, Color::ColorMap cm)
{
	switch (cm) {
	case Color::RedBlue:
		return colorFromIntervalRedBlue(value);
	case Color::Parula:
		return colorFromIntervalParula(value);
	default:
		assert(0);
		return Color::Gray;
	}
}

inline Color colorFromInterval(float min, float max, float value, Color::ColorMap cm)
{
	if (min == max) { // no range
		return Color::Gray;
	}

	value = std::abs((value - min) / (max - min));
	return colorFromInterval(value, cm);
}

inline Color colorFromIntervalRedBlue(float value)
{
	Color c;

	if (value < 0) // out of range - left
		value = 0;
	else if (value > 1) // out of range - right
		value = 1;

	c.setHsv(value * 240, 255, 255);
	return c;
}

/**
 * @brief Returns a color in the interval from Red to Blue depending on the position of the given
 * value in the given interval [min, max].
 *
 * If the given value is less than the minimum value of the interval, the Red color will be
 * returned. If the given value is higher than the maximum value of the interval, the Blue color
 * will be returned. If min and max are equal, the Grey color will be returned.
 *
 * This function works also if the values of the interval are swapped.
 *
 * @param[in] min: minimum value of the interval
 * @param[in] max: maximum value of the interval
 * @param[in] value: the value in the interval to ramp.
 * @return A color between Red and Blue representing the position of value in the interval [min,
 * max].
 */
inline Color colorFromIntervalRedBlue(float min, float max, float value)
{
	Color c;
	if (min == max) { // no range
		return Color::Gray;
	}

	value = std::abs((value - min) / (max - min));
	return colorFromIntervalRedBlue(value);
}

inline Color colorFromIntervalParula(float value)
{
	if (value < 0) // out of range - left
		value = 0;
	else if (value > 1) // out of range - right
		value = 1;

	static uint paruVal[9] = {
		0xff801627,
		0xffe16303,
		0xffd48514,
		0xffc6a706,
		0xff9eb938,
		0xff73bf92,
		0xff56bad9,
		0xff2ecefc,
		0xff0afaff};

	int ind = int(floor(value * 8.0f));
	float div = (value * 8.0f - ind);

	if (div < 0)
		div = 0;
	else if (div > 1)
		div = 1;

	return colorLerp((Color::ColorARGB) paruVal[ind], (Color::ColorARGB) paruVal[ind + 1], div);
}

inline Color colorFromIntervalParula(float min, float max, float value)
{
	Color c;
	if (min == max) { // no range
		return Color::Gray;
	}

	value = std::abs((value - min) / (max - min));
	return colorFromIntervalParula(value);
}

} // namespace vcl
