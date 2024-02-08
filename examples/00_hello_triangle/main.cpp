#include <QApplication>

#include "hello_triangle.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    HelloTriangle tw;

    tw.show();

    return app.exec();
}
