#include <Application.h>
#include "PrefsView.h"
#include "Constants.h"


PrefsView::PrefsView(BRect frame, const char *name)
	: BView (frame, name, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW)
{
	SetViewColor(210,220,215);

	float w,h,w2;
	BRect rect(0., 0., 80., 20.);
	fullScreen = new BCheckBox(rect, "FullScreen", "FullScreen", new BMessage(MSG_FULLSCREEN_MODIFIED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	fullScreen->ResizeToPreferred();
	fullScreen->GetPreferredSize(&w, &h);
	fullScreen->MoveTo(frame.right-w-1, 10.);
//	AddChild(fullScreen);

	fullScreen->GetPreferredSize(&w, &h);
	autoScale = new BCheckBox(rect, "Auto Scale", "Auto Fit", new BMessage(MSG_AUTOSCALE_MODIFIED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	autoScale->ResizeToPreferred();
	autoScale->GetPreferredSize(&w2, &h);
	autoScale->MoveTo(frame.right-w-w2-2, 10.);
//	AddChild(autoScale);


	cmdView = new BView(BRect(frame.right-w-w2-2, frame.top, frame.right, frame.bottom), "cmdView", B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	AddChild(cmdView);
	cmdView->AddChild(fullScreen);
	cmdView->AddChild(autoScale);
	


	rect.Set(0., 16., 350., 31.);
	filename = new BStringView(rect, "filename", "??");
	filename->SetHighColor(0, 50, 150);
	AddChild(filename);

	rect.Set(0., 0., 350., 15.);
	path = new BStringView(rect, "path", "/");
	AddChild(path);

	aboutButton = new BButton(rect, "about", "About", new BMessage (B_ABOUT_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	aboutButton->SetTarget(be_app);
	aboutButton->ResizeTo(50., 20.);
	aboutButton->MoveTo(frame.right - 102., frame.bottom - 20.);
	AddChild(aboutButton);

	quitButton = new BButton(rect, "quit", "Quit", new BMessage(B_QUIT_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	quitButton->SetTarget(be_app);
	quitButton->ResizeTo(50.,20.);
	quitButton->MoveTo(frame.right - 51., frame.bottom - 20.);
	AddChild(quitButton);
}


PrefsView::~PrefsView()
{
}


bool PrefsView::AutoScaleON()
{
	return (autoScale->Value() == B_CONTROL_ON);
}


void PrefsView::SetPath(const char *name)
{
	path->SetText(name);
	path->ResizeToPreferred();
}


void PrefsView::SetFileName(const char *name)
{
	filename->SetText(name);
	filename->ResizeToPreferred();
}
