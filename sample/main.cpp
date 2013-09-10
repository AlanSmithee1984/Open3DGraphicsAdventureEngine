#include <QApplication>

#include "testwindow.h"

#include "ogreengine.h"
#include "ogrewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow* window = new TestWindow;
    window->show();
//    window->resize(1000, 1000);

    OgreEngine::instance()->initialize(window, 60);
    OgreWidget* widget = OgreEngine::instance()->createOgreWidget(window);

    widget->resize(window->size());

    widget->show();

    return a.exec();
}
