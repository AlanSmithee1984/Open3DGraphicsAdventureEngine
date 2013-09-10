#include <QApplication>

#include "testwindow.h"

#include "ogreengine.h"
#include "ogrewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow* window = new TestWindow;
    window->show();

    OgreEngine::instance()->initialize(window, 60);
    OgreWidget* widget = OgreEngine::instance()->createOgreWidget(window);
    widget->show();


    return a.exec();
}
