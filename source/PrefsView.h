#ifndef _PREFSVIEW_H_
#define _PREFSVIEW_H_

#include <Box.h>
#include <StringView.h>
#include <Button.h>
#include <CheckBox.h>


class InfoView : public BView
{
	public:
		InfoView(BRect frame, const char *name);
		~InfoView();

 		void SetPath(const char *name);
		void SetFileName(const char *name);
		void SetPictureSize(const char *name);
		void SetZoomLevel(const char *name);
		const char *GetFullName();
		void SetViewColor(rgb_color c);


	private:
		char		*fullName;
		BStringView	*path;
		BStringView	*filename;
		BStringView	*picSize;
		BStringView	*zoomLevel;

		BBox		*leftSide, *rightSide;
};


class CmdView : public BView
{
	public:
		CmdView(BRect frame, const char *name);
		~CmdView();

		void CheckAutoFit(bool b); 
		bool AutoScaleON();
		void CheckFullScreen(bool b);

	private:
		BCheckBox	*autoScale;
		BCheckBox	*fullScreen;
		BButton		*quitButton;
		BButton		*aboutButton;
		
};


#endif

