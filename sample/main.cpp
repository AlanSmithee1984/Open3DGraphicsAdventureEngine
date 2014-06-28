#include <QApplication>

#include "testwindow.h"
#include <QHBoxLayout>

#include "open3dgraphicsadventureengine.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow* window = new TestWindow;

    QHBoxLayout *layout = new QHBoxLayout;
    window->setLayout(layout);

    Open3DGraphicsAdventureEngine* engine = new Open3DGraphicsAdventureEngine(window);

    layout->addWidget(engine->getRenderWidget());

    window->show();
    window->resize(1700, 1200);

    return a.exec();
}
