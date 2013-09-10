#ifndef OGREUTILS_H_
#define OGREUTILS_H_

#include <Ogre.h>

class QWidget;

void createOgreRenderWindowParameterList(QWidget* pWidget, Ogre::NameValuePairList& params);
Ogre::String createOgreWindowIdForQWidget(QWidget* pWidget);

#endif // OGREUTILS_H_
