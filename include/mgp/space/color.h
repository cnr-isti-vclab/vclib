/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_COLOR_H
#define MGP_COLOR_H

#ifdef QT_GUI_LIB

#include <QColor>

namespace mgp {

typedef QColor Color;

} // namespace mgp

bool operator<(const QColor& c1, const QColor& c2);

#else

#include "point/point4.h"

namespace mgp {

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
	// toimplement
	int   hsvHue() const;
	int   hsvSaturation() const;
	float hsvHueF() const;
	float hsvSaturationF() const;
	void  setAlpha(uint8_t alpha);
	void  setRed(uint8_t red);
	void  setGreen(uint8_t green);
	void  setBlue(uint8_t blue);
	void
	setRgb(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	void setHsv(unsigned int h, uint8_t s, uint8_t v, uint8_t alpha = 255);
	void setAlphaF(float alpha);
	void setRedF(float red);
	void setGreenF(float green);
	void setBlueF(float blue);
	void setRgbF(float red, float green, float blue, float alpha = 1.0);
	void setHsvF(float hf, float sf, float vf, float alpha = 1.0);

	bool operator==(const Color& otherColor) const;
	bool operator!=(const Color& otherColor) const;
	bool operator<(const Color& otherColor) const;

	friend std::ostream& operator<<(std::ostream& out, const Color& p);
};

std::ostream& operator<<(std::ostream& out, const Color& p);

} // namespace mgp

#include "color.cpp"

#endif

#endif // MGP_COLOR_H
