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

#ifndef BAR_COMPONENT_H
#define BAR_COMPONENT_H

#include <vclib/types.h>

/*
 * This is an example of a component class, that can be used as horizontal,
 * vertical and optional in an Element of a Mesh.
 * Due to the complexity of this class, VCLib provides a vcl::Component class
 * that can be used as a base class.
 *
 * In this example, the component stores two values: an integer that can be
 * accessed trough the member function `foo()`, and a double that can be
 * accessed trough the member function `bar()`.
 */

// a component needs a concept, that allows to test if other elements/meshes
// have the same component.
// the concept should just check if the element/mesh has the member functions
// that are part of the component class.
template<typename T>
concept HasBarComponent = requires(T t, const T& ct) {

	// accessor to the int value of the bar component, returns int&
	{ t.foo() } -> std::same_as<int&>;

	// const accessor to the int value of the bar component
	{ ct.foo() } -> std::same_as<int>;

	// accessor to the double value of the bar component, returns int&
	{ t.bar() } -> std::same_as<double&>;

	// const accessor to the double value of the bar component
	{ ct.bar() } -> std::same_as<double>;
};

// TODO: BarComponent inherit from vcl::Component
class BarComponent
{
public:
	// first requirement: an unique static const uint ID of the component
	// vcl::COMPONENTS_NUMBER is the number of components that are already
	// defined in VCLib. You can use it to avoid collisions.
	static const uint COMPONENT_ID = vcl::COMPONENTS_NUMBER + 0;

	// any member that you want to add to the component

	int& foo() { return fooData; }
	int foo() const { return fooData; }

	double& bar() { return barData; }
	double bar() const { return barData; }

protected:
	// second requirement: a protected `importFrom` member function
	template<typename Element> // another element, maybe from another mesh type
	void importFrom(const Element& e)
	{
		// will import the foo component from the element e only if it also has
		// the foo component
		if constexpr (HasBarComponent<Element>) { // compile time check
			fooData = e.foo();
			barData = e.bar();
		}
	}

private:
	// the data that you want to store in a component
	int fooData;
	double barData;
};

static_assert(
	HasBarComponent<BarComponent>,
	"Make sure that the concept is satisfied with its component.");

#endif // BAR_COMPONENT_H
