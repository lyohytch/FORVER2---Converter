#include <QtGui/QApplication>
#include "converter.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    converter w(0, &a);
    w.show();
    return a.exec();
}
