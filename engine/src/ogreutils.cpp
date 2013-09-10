#include "ogreutils.h"

#include <QtGlobal>
#include <QDebug>
#include <QWidget>

#if defined(Q_WS_X11)
#include <QX11Info>
#include <X11/Xlib.h>
#endif

void createOgreRenderWindowParameterList(QWidget* pWidget, Ogre::NameValuePairList& params)
{
    Q_ASSERT(pWidget);

    qDebug() << pWidget;

    // Parameters to pass to Ogre::Root::createRenderWindow()
    params["useNVPerfHUD"] = "true";

    // enable anti-aliasing
    params["FSAA"] = "16"; // anti aliasing factor (0, 2, 4, 8, 16,...)// 16 gives excellent antialiasing but slows down the rendering!

    // The external windows handle parameters are platform-specific
    Ogre::String externalWindowHandleParams = createOgreWindowIdForQWidget(pWidget);

    // Add the external window handle parameters to the existing params set.
#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
    params["externalWindowHandle"] = externalWindowHandleParams;
#endif

#if defined(Q_WS_X11)
    params["parentWindowHandle"] = externalWindowHandleParams;
#endif

#if defined(Q_WS_MAC)
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
#endif

}

Ogre::String createOgreWindowIdForQWidget(QWidget* pWidget)
{
    Q_ASSERT(pWidget != NULL);
    Ogre::String winId;

#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
    // positive integer for W32 (HWND handle) - According to Ogre Docs
    winId = Ogre::StringConverter::toString((unsigned long)(pWidget->winId()));
#endif

#if defined(Q_WS_X11)
    // poslong:posint:poslong:poslong (display*:screen:windowHandle:XVisualInfo*) for GLX - According to Ogre Docs
    QX11Info info = pWidget->x11Info();
    winId = Ogre::StringConverter::toString((unsigned long) (info.display()));
    winId += ":";
    winId += Ogre::StringConverter::toString((unsigned int) (info.screen()));
    winId += ":";
    winId += Ogre::StringConverter::toString((unsigned long) (pWidget->winId()));

//        winId += ":";
//        winId += Ogre::StringConverter::toString((unsigned long)(info.visual()));

    Display* display = QX11Info::display();
    Q_ASSERT(display);
    XSync(display, False);

#endif

    return winId;
}

