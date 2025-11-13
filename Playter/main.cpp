#include "Playter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Playter window;
    window.show();
    return app.exec();
}
