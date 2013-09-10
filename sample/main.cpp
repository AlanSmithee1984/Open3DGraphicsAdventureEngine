#include "ogreengine.h"

#include "testwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow* window = new TestWindow;
    window->show();

    OgreEngine::instance()->initialize(window, 60);
    OgreWidget* widget = OgreEngine::instance()->createOgreWidget(window);


    return a.exec();
}
