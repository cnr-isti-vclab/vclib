#include <iostream>

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <vclib/render/renderer.h>
#include <vclib/render/concepts/window_manager.h>
#include <vclib/bgfx/canvas.h>
#include <vclib/qt/widget_manager.h>

// #include <vclib/qt/viewer_widget.h>

// template<typename T>
// class Basic : public vcl::qt::ViewerWidget
// {
//     T t = 42;

// public:
//     Basic(QWidget* parent = nullptr) : vcl::qt::ViewerWidget(parent)
//     {
//         // add a QPushButton
//         QPushButton* button = new QPushButton("Click me", this);

//         // connect the button to a slot
//         connect(button, &QPushButton::clicked, this, &Basic::onButtonClicked);

//         // add the button to a layout
//         QVBoxLayout* layout = new QVBoxLayout(this);
//         layout->addWidget(button);
//         this->setLayout(layout);
//     }

//     void f() { std::cout << "C::f()" << t << std::endl; }

// public slots:

//     void onButtonClicked() { std::cout << "C::onButtonClicked()" << std::endl; }
// };

// template<template<typename> typename C, template<typename> typename E>
// class Minestrone : public C<Minestrone<C, E>>, public E<Minestrone<C, E>>
// {
// public:
//     using EventManagerType = E<Minestrone<C, E>>;

//     void foo() { C<Minestrone<C, E>>::drawWithInit(); };
// };

// template<class Derived>
// class Canvas
// {
// public:
//     void drawWithInit()
//     {
//         derived().init();
//         draw();
//     };

//     virtual void draw() { std::cerr << "draw\n"; };

// private:
//     auto& derived() { return *static_cast<Derived*>(this); };
// };

// template<class Derived>
// class EventManager
// {
// public:
//     void init() { std::cerr << "init\n"; };
// };

// class Gesu : public Minestrone<Canvas, EventManager>
// {
//     virtual void draw() override { std::cerr << "Gesu::draw\n"; }
// };

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    vcl::Renderer<vcl::qt::WidgetManager, vcl::CanvasBGFX> r;

    // does not compile, std::vector does not satisfy the WindowManagerConcept
    // vcl::Renderer<vcl::qt::WidgetManager, std::vector> v0;

    // does not compile, std::vector does not satisfy the CanvasConcept
    // vcl::Renderer<std::vector, vcl::CanvasBGFX> v1;

    // Gesu M;

    // M.foo();

    // Basic<int> c;

    // c.show();

    return app.exec();
}
