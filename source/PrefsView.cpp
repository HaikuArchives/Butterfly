#include <Application.h>
#include "PrefsView.h"
#include "Constants.h"
#include "StringsRsrc.h"

#include <stdio.h>		// debug
#include <string.h>


/////////////////////////////////////////////////////////////::
/////////////////////////////////////////////////////////////::
//				InfoView
/////////////////////////////////////////////////////////////::

InfoView::InfoView(BRect frame, const char *name)
		: BView (frame, name, B_FOLLOW_ALL_SIDES, 0) //, B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE)
{
	BRect r = frame;
	r.right -= 60.;

	leftSide = new BBox (r, "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE);
	AddChild(leftSide);
	//leftSide->SetViewColor(110,220,215);
	//leftSide->SetViewColor(0,0,215);
	leftSide->SetViewColor(240,240,240);

	path = new BStringView(BRect(r.left + 2., 8., r.right - 2., 22.), "", "/", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	leftSide->AddChild(path);

	filename = new BStringView(BRect(r.left + 2., 22., r.right - 2., 38.), "", "??", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	leftSide->AddChild(filename);

	r.top += 5.;
	r.left = r.right+1.;
	r.right += 60.;
	rightSide = new BBox (r, "", B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	//rightSide->SetViewColor(206,195,202);
	leftSide->SetViewColor(240,240,240);
	AddChild(rightSide);

	r = rightSide->Bounds();
	zoomLevel = new BStringView(BRect(r.left+2., 8., r.right-2., 22.), "", "100%", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	rightSide->AddChild(zoomLevel);

	picSize = new BStringView(BRect(r.left+2., 22., r.right-2., 38.), "", "??x??", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	rightSide->AddChild(picSize);

	fullName = NULL;

	// Old Thom's color
	//BView::SetViewColor(206,195,222); //B_TRANSPARENT_COLOR);
	BView::SetViewColor(240,240,240);
}


InfoView::~InfoView()
{
	delete path;
	delete filename;
	if (fullName != NULL)
		delete fullName;
}


void InfoView::SetPath(const char *name)
{
	path->SetText(name);
}


void InfoView::SetFileName(const char *name)
{
	filename->SetText(name);
}


void InfoView::SetPictureSize(const char *name)
{
	picSize->SetText(name);
}


void InfoView::SetZoomLevel(const char *name)
{
	zoomLevel->SetText(name);
}


const char *InfoView::GetFullName()
{
	if (fullName==NULL)
		fullName = new char[256];

	if (strncmp(filename->Text(), "??", 2)==0)
		return NULL;

	strcpy(fullName, path->Text());
	strcat(fullName, "/");
	strcat(fullName, filename->Text());
	return fullName;
}

void InfoView::SetViewColor(rgb_color c)
{
	leftSide->SetViewColor(c);
	rightSide->SetViewColor(c);
	BView::SetViewColor(c);
	path->SetViewColor(c);
	filename->SetViewColor(c);
	picSize->SetViewColor(c);
	zoomLevel->SetViewColor(c);
	leftSide->Invalidate();
	rightSide->Invalidate();
	path->Invalidate();
	filename->Invalidate();
	picSize->Invalidate();
	zoomLevel->Invalidate();
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//				CmdView
/////////////////////////////////////////////////////////////

CmdView::CmdView(BRect frame, const char *name)
		: BView (frame, name, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW)
{
	// Old Thom's Color
	//SetViewColor(210,220,215);
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	float w,h,w2, w3, w4;
	BRect rect(0., 0., 80., 20.);
	fullScreen = new BCheckBox(rect, "", StringsRsrc[STR_FULLSCREEN], new BMessage(MSG_FULLSCREEN_MODIFIED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	fullScreen->ResizeToPreferred();
	fullScreen->GetPreferredSize(&w, &h);
	fullScreen->MoveTo(0, 20.);
	fullScreen->SetFlags(B_WILL_DRAW);
	AddChild(fullScreen);

	fullScreen->GetPreferredSize(&w, &h);
	autoScale = new BCheckBox(rect, "", StringsRsrc[STR_AUTOFIT], new BMessage(MSG_AUTOSCALE_MODIFIED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	autoScale->ResizeToPreferred();
	autoScale->GetPreferredSize(&w2, &h);
	autoScale->MoveTo(0., 0.);
	autoScale->SetFlags(B_WILL_DRAW);
	AddChild(autoScale);

	w = w > w2 ? w : w2;
	ResizeTo(60.+w, 40.);

	aboutButton = new BButton(rect, "", StringsRsrc[STR_ABOUT], new BMessage (B_ABOUT_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	aboutButton->GetPreferredSize(&w3,&h);
	aboutButton->SetTarget(be_app);
//	aboutButton->ResizeTo(60., 20.); 
	aboutButton->MoveBy(w, 0.);
	aboutButton->SetFlags(B_WILL_DRAW);
	AddChild(aboutButton);

	quitButton = new BButton(rect, "", StringsRsrc[STR_QUIT], new BMessage(B_QUIT_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	aboutButton->GetPreferredSize(&w4,&h);
	quitButton->SetTarget(be_app);
//	quitButton->ResizeTo(60.,20.); 
	quitButton->MoveBy(w, 20.);
	quitButton->SetFlags(B_WILL_DRAW);
	AddChild(quitButton);

	w3 = w3 > w4 ? w3 : w4;
	aboutButton->ResizeTo(w3, 20.); 
	quitButton->ResizeTo(w3,20.); 
	ResizeTo(w3+w, 40.);

}


CmdView::~CmdView()
{
	delete autoScale;
	delete fullScreen;
	delete aboutButton;
	delete quitButton;
}

void CmdView::CheckAutoFit(bool b) 
{
	autoScale->SetValue(b);
	if (b)
		autoScale->Invoke();
}

bool CmdView::AutoScaleON()
{
	return (autoScale->Value() == B_CONTROL_ON);
}

void CmdView::CheckFullScreen(bool b) 
{
	fullScreen->SetValue(b);
	if (b)
		fullScreen->Invoke();
}



