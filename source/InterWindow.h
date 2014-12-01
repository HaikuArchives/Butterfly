#ifndef _INTERWINDOW_H_
#define _INTERWINDOW_H_

#include <TranslationKit.h>
#include <InterfaceKit.h>
#include "InterView.h"
#include "PrefsView.h"
#include "PrefWindow.h"

// Window dimensions
const int DISPLAY_X = 600;
const int DISPLAY_Y = 400;


class InterWindow : public BWindow
{
	public:
		InterWindow();
		virtual ~InterWindow();
		virtual bool QuitRequested(void);
		virtual void MessageReceived(BMessage *message);
		virtual void FrameResized(float width, float height);
		void SwapFullScreenNoMenu();
		virtual void WorkspacesChanged(uint32 oldWorkspaces, uint32 newWorkspaces); 
		BRect NoneFullScreenFrame();
		
	private:
		void UpdateLimits(float width, float height);
		void SwapFullScreen();
		void TranslatePicture();
		void OpenSaveFilePanel(BMessage *msg);
		void SetAllDesktop(uint32 mode);
		void SetCurrentDesktop(uint32 mode);
		void TranslateAndWrite(BMessage *msg);

	public:
		BView*			rootView;
		BBox* 			backBox;		
		InterView		*backView;

		PrefWindow		*thePrefWindow;
//		PrefsView		*thePrefsView;
		CmdView			*theCmdView;
		InfoView		*theInfoView;
//
		//BScrollView		*scrollBackView;
		BScrollView		*scrollView;

		BRect			oldFrame;
		BRect			oldFrame2;		// real full screen		
		bool			fullScreenON;
		bool			realFullScreen;
		
		float			xLimit;		// limit between file list and picture view
		float			yLimit;		// limit between picture view and prefs view

		float			cmdViewWidth;
		
		translator_id	outTranslator;
		uint32			outType;
		BBitmapStream	*inStream;

};


#endif
