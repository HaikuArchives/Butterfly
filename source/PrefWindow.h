#ifndef _PREFWINDOW_H_
#define _PREFWINDOW_H_

#include <Window.h>

class BRadioButton;
class BColorControl;
class BMenuField;

class PrefWindow : public BWindow
{
	public:
		PrefWindow(BWindow *main);
		~PrefWindow();
		virtual bool QuitRequested(void);
		virtual void MessageReceived(BMessage *message);
//		virtual void FrameResized(float width, float height);
		virtual void Show(void);
		
		// [BT] 1.4
		//rgb_color GetInfobarColor();
//		int32 GetMouseMode() {return mouseMode;}
//		int32 GetAutoFitMode() {return autofitMode;}
//		int32 GetFullScreenMode() {return fullscreenMode;}
//		int32 GetXPos();
//		int32 GetYPos();
//		int32 GetWidth();
//		int32 GetHeight();
//		void  WriteAttributes();

	private:
		void SetAndWriteAttributes();
		void RestoreOldAttributes();
		// [BT] 1.4
//		void ReadAttributes();
//	
//		int32 infoBarColor;
//		int32 mouseMode;
//		int32 autofitMode;
//		int32 fullscreenMode;
//		BRect winFrame;		
		
		BRadioButton*	radFollow;
		BRadioButton*	radFlee;
		BColorControl*	colSelector;
		BMenuField*     languageMenu;
	
		BWindow			*mainWin;
};


#endif
