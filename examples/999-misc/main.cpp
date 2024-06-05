/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <iostream>
#include <vector>

#include <vclib/concepts.h>
#include <vclib/space.h>

struct Shape
{
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual void setScale(float scale) { s = scale; }
    virtual void printScale() const { std::cout << "Scale: " << s << '\n'; }
    virtual std::shared_ptr<Shape> clone() const = 0;
private:
    float s = 1.0f;
};

struct Circle : public Shape
{
    void draw() const override
    {
        std::cout << "Drawing a circle\n";
    }

    std::shared_ptr<Shape> clone() const override
    {
        return std::make_shared<Circle>(*this);
    }
};

struct Square : public Shape
{
    void draw() const override
    {
        std::cout << "Drawing a square\n";
    }

    std::shared_ptr<Shape> clone() const override
    {
        return std::make_shared<Square>(*this);
    }
};

int main()
{
    static_assert(vcl::Clonable<Shape>, "");

    vcl::PolymorphicObjectVector<Shape> vec;

    Circle circle;
    circle.setScale(2);

    vec.pushBack(circle);
    vec.pushBack(Square());

    const auto& constVec = vec;

    for (const auto& shape : vec) {
        shape->setScale(5);
    }

    for (const auto& shape : constVec) {
        shape->draw();
        shape->printScale();
    }

    // circle is not modified
    circle.printScale();

    return 0;
}
