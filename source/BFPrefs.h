#ifndef _BFPREFS_H_
#define _BFPREFS_H_

#include <GraphicsDefs.h>
#include <Rect.h>

class BFPrefs {
	public:
		BFPrefs();
		void Write();
		void Read();

		rgb_color GetInfobarColor();
		int32 GetMouseMode() {return mouseMode;}
		int32 GetAutoFitMode() {return autofitMode;}
		int32 GetFullScreenMode() {return fullscreenMode;}
		int32 GetXPos();
		int32 GetYPos();
		int32 GetWidth();
		int32 GetHeight();
		int32 GetLanguage();

		void SetInfobarColor(int32 color);
		void SetMouseMode(bool mode) { mouseMode = mode; }
		void SetAutoFitMode(bool mode) { autofitMode = mode; }
		void SetFullScreenMode(bool mode) { fullscreenMode = mode; }
		void SetXPos(int32 x);
		void SetYPos(int32 y);
		void SetWidth(int32 width);
		void SetHeight(int32 height);
		void SetWinFrame(BRect frame) { winFrame = frame; }
		void SetLanguage(int32 lang);


	private:
		int32 infoBarColor;
		int32 mouseMode;
		int32 autofitMode;
		int32 fullscreenMode;
		BRect winFrame;		
		int32 language;
};

#endif // _BFPREFS_H_
