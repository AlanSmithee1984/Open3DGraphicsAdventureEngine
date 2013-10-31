#include <QApplication>

#include "testwindow.h"

#include "open3dgraphicsadventureengine.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow* window = new TestWindow;
    window->show();
    window->resize(2000, 1000);

    Open3DGraphicsAdventureEngine* engine = new Open3DGraphicsAdventureEngine(window);

    return a.exec();
}
