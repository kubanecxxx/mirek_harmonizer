#include "harmonizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Harmonizer w;
    w.show();
    
    return a.exec();
}
