/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_RENDER_DRAWABLE_OBJECT_H
#define VCL_RENDER_DRAWABLE_OBJECT_H

#include <vclib/space/point.h>

namespace vcl {

class DrawableObject
{
public:
	/**< @brief Empty constructor */
	DrawableObject() {}

	virtual ~DrawableObject() {}

	/**
	 * @brief This member function is called after the initialization of the GL Context.
	 * It must initialize and bind data to the GPU like buffers and textures.
	 */
	virtual void init() {};

	/**
	 * @brief This member function must draw the object through GL calls.
	 * It will be called at every frame.
	 */
	virtual void draw() const = 0;

	/**
	 * @brief This member function is used to find a good camera position
	 * to render object. It must return the position of the center of the object.
	 */
	virtual vcl::Point3d center() const = 0;

	/**
	 * @brief This member function is used to find a good camera position
	 * to render object. It should return the ray of the bounding sphere of the object, but also
	 * half diagonal of the bounding box of the object is a good approximation. Return
	 * -1 if the object shouldn't influence the position of the camera.
	 */
	virtual double radius() const = 0;

	/**
	 * @brief This member function is used to create a new copy of the DrawableObject.
	 * Each derived class must implement this member function, that returns a new dynamically
	 * allocated DrawableObject that is a copy of the current one.
	 * for more details about this paradigm, check polimorphism clone in modern c++:
	 * https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/
	 */
	virtual DrawableObject* clone() const = 0;

	virtual bool isVisible() const = 0;
	virtual void setVisibility(bool vis) = 0;

	const std::string& name() const { return n; }
	std::string& name() { return n; }
private:
	std::string n;
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_OBJECT_H
