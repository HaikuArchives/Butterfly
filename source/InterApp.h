#ifndef _INTERAPP_H_
#define _INTERAPP_H_

#include <AppKit.h>
#include <InterfaceKit.h>
#include "InterWindow.h"
#include "FSNav.h"

const char APP_VERSION[] = "1.6.2";
const char APP_SIGNATURE[] = "application/x-vnd.Dragfly-Butterfly";

class BFPrefs;

class InterApp : public BApplication
{
	public:
		InterApp();
		virtual ~InterApp();
	
//		virtual void ReadyToRun(void);
		virtual void AboutRequested(void);
		virtual void ArgvReceived(int32 argc, char **argv);
		virtual void RefsReceived(BMessage *message);
		
	public:
		BListView*		fileList;
		InterWindow*	theWin;
		FSNav*			nav;
		InterView*		picView;
		
		BFPrefs*    	prefs;
		
		char			initialFile[256];
};

#endif
