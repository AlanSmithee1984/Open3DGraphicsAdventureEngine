#include <QApplication>

#include "testwindow.h"
#include <QHBoxLayout>

#include "open3dgraphicsadventureengine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow* window = new TestWindow;
    window->resize(1700, 1200);

    QHBoxLayout *layout = new QHBoxLayout;
    window->setLayout(layout);


    Open3DGraphicsAdventureEngine* engine = new Open3DGraphicsAdventureEngine(window, NULL);

    QWidget* widget = engine->getRenderWidget();
    layout->addWidget(widget);

    window->show();


    return a.exec();
}
