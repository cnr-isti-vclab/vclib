/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <iostream>

#include <vclib/concepts.h>
#include <vclib/space.h>

struct Shape
{
    virtual ~Shape()          = default;
    virtual void draw() const = 0;

    virtual void setScale(float scale) { s = scale; }

    virtual void printScale() const { std::cout << "Scale: " << s << '\n'; }

    virtual std::shared_ptr<Shape> clone() const& = 0;

    virtual std::shared_ptr<Shape> clone() && = 0;

private:
    float s = 1.0f;
};

struct Circle : public Shape
{
    void draw() const override { std::cout << "Drawing a circle\n"; }

    std::shared_ptr<Shape> clone() const& override
    {
        std::cerr << "Circle copy\n";
        return std::make_shared<Circle>(*this);
    }

    std::shared_ptr<Shape> clone() && override
    {
        std::cerr << "Circle move\n";
        return std::make_shared<Circle>(std::move(*this));
    }
};

struct Square : public Shape
{
    void draw() const override { std::cout << "Drawing a square\n"; }

    std::shared_ptr<Shape> clone() const& override
    {
        std::cerr << "Square copy\n";
        return std::make_shared<Square>(*this);
    }

    std::shared_ptr<Shape> clone() && override
    {
        std::cerr << "Square move\n";
        return std::make_shared<Square>(std::move(*this));
    }
};

int main()
{
    static_assert(vcl::Cloneable<Shape>, "");
    static_assert(vcl::Cloneable<const Shape>, "");
    static_assert(vcl::Cloneable<Shape&>, "");
    static_assert(vcl::Cloneable<const Shape&>, "");
    static_assert(vcl::Cloneable<Shape&&>, "");

    vcl::PolymorphicObjectVector<Shape> vec;

    Circle circle;
    circle.setScale(2);

    vec.pushBack(circle);
    vec.pushBack(Square());

    const auto& constVec = vec;

    for (auto& shape : vec) {
        // shape = std::make_shared<Square>(); <- does not build
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
