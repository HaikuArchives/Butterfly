#include <AppKit.h>
#include <Box.h>
#include "InterWindow.h"
#include "InterView.h"
#include "InterApp.h"
#include "PrefsView.h"
#include "FileList.h"
#include "Constants.h"
#include "BFPrefs.h"
#include "StringsRsrc.h"

#include <stdio.h>		// DEBUG
#include <string.h>
#include "OutputFormatWindow.h"
#include <BitmapStream.h>
#include <FilePanel.h>
#include <Background.h>
#include <stdlib.h>
#include <FindDirectory.h>
#include <storage/Path.h>

const char *theName;
const char *thePath;
const char *theDim;

const int32 BACKBOX_INSET = 5;

//=========================================================================
//	InterWindow class
//
//=========================================================================

InterWindow::InterWindow() 
	: BWindow(BRect(0, 0, DISPLAY_X-1, DISPLAY_Y-1), "Butterfly", B_TITLED_WINDOW, 0)
{
	int32 backBoxHeight;
	int32 backBoxWidth;
	BRect rect;
	
	// Set title (version)
	char titleStr[64] = "Butterfly ";
	strcat(titleStr,APP_VERSION);
	SetTitle(titleStr);
	
	// Set windows attributes
	MoveTo(((InterApp*)be_app)->prefs->GetXPos(), ((InterApp*)be_app)->prefs->GetYPos());
	ResizeTo(((InterApp*)be_app)->prefs->GetWidth(), ((InterApp*)be_app)->prefs->GetHeight());
	SetSizeLimits(300., 10000., 200., 10000.);

	// Create background BView (only aesthetic)
	rootView = new BView(Bounds(), "root view", B_FOLLOW_ALL_SIDES,0);
	rootView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(rootView);	
	
	// Create background BBox
	rect =  Bounds().InsetBySelf(BACKBOX_INSET,BACKBOX_INSET);
	rect.bottom=rect.bottom-45-BACKBOX_INSET;
	backBox = new BBox(rect, "back box", B_FOLLOW_ALL_SIDES);
	backBox->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	rootView->AddChild(backBox);	

	backBoxHeight = (int32) backBox->Bounds().Height();
	backBoxWidth = (int32) backBox->Bounds().Width();

	// Create Display view
	backView = new InterView(BRect(161.+3*BACKBOX_INSET, 0.+BACKBOX_INSET, backBoxWidth-BACKBOX_INSET,backBoxHeight-BACKBOX_INSET), "back", this);
	backBox->AddChild(backView);

//	scrollBackView = new BScrollView("scrollBack", backView, B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, true, true, B_NO_BORDER);
//	AddChild(scrollBackView);
	((InterApp*)be_app)->picView = backView;

	// Create Info View
	rect= BRect(0., 0., 50., 50.);						// size doesn't care
	theInfoView = new InfoView(rect, "info view");
	rootView->AddChild(theInfoView);

	theCmdView = new CmdView(rect, "cmd view");
	rootView->AddChild(theCmdView);
	rect = theCmdView->Bounds();
	theCmdView->MoveTo(rootView->Bounds().Width()-rect.right-BACKBOX_INSET, rootView->Bounds().Height()-rect.bottom-BACKBOX_INSET);
	cmdViewWidth = rect.right;

	// Create file list view
	rect = BRect(BACKBOX_INSET+1, BACKBOX_INSET+2, 160-BACKBOX_INSET,backBoxHeight-BACKBOX_INSET-B_V_SCROLL_BAR_WIDTH-2);
	CFileList *list = new CFileList(rect);
	((InterApp*)be_app)->fileList = (BListView*)list;

	scrollView = new BScrollView("files", list,	B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, 0, true, true, B_FANCY_BORDER);
	scrollView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	backBox->AddChild(scrollView);
//	list->SetBackground();

	rect = Bounds();
	FrameResized(rect.right, rect.bottom);
	fullScreenON = false;
	realFullScreen = false;

	list->MakeFocus();
	scrollView->SetBorderHighlighted(false);

	// Create pref window
	thePrefWindow = new PrefWindow(this);
	thePrefWindow->Hide();
		
	theInfoView->SetViewColor(((InterApp*)be_app)->prefs->GetInfobarColor());
	backView->SetPanMode(((InterApp*)be_app)->prefs->GetMouseMode());
	
	theCmdView->CheckFullScreen(((InterApp*)be_app)->prefs->GetFullScreenMode());
	theCmdView->CheckAutoFit(((InterApp*)be_app)->prefs->GetAutoFitMode());
	
	// Show the window
	Show();
	Sync();
}


//--------------------------------------------------------------------
InterWindow::~InterWindow()
{
	thePrefWindow->Lock();
	thePrefWindow->Quit();

	// Set last state before prefs are saved by ~InterApp
	((InterApp*)be_app)->prefs->SetFullScreenMode( fullScreenON );
	((InterApp*)be_app)->prefs->SetAutoFitMode( theCmdView->AutoScaleON() );
	((InterApp*)be_app)->prefs->SetWinFrame( NoneFullScreenFrame() );	

	// Quit window
	Hide();
	Sync();
}


//--------------------------------------------------------------------
bool InterWindow::QuitRequested(void)
{
	if (!fullScreenON && !realFullScreen) {
		oldFrame = Frame();
	} else if (realFullScreen && !fullScreenON) {
		oldFrame = oldFrame2;
    } 
//  else if (realFullScreen && fullScreenON) {
//	}
		
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


//--------------------------------------------------------------------
void InterWindow::MessageReceived(BMessage *message)
{
	BRect r;
	entry_ref *ref;

	switch ( message->what )
	{

		case MSG_FILE_SELECTED:
			int32 num;
			message->FindInt32 ("index", 0, &num);
			((InterApp*)be_app)->nav->FileSelected(num);
			//printf("select %ld\n", num);			// DEBUG
			break;
			
		case MSG_FILE_INVOCATED:
			int32 num2;
			message->FindInt32 ("index", 0, &num2);
			((InterApp*)be_app)->nav->FileInvocated(num2);
			//printf("invoke %ld\n", num2);			// DEBUG
			break;
		
		case MSG_AUTOSCALE_MODIFIED:
			backView->SetAutoScale(theCmdView->AutoScaleON());
			break;

		case MSG_FULLSCREEN_MODIFIED:
			SwapFullScreen();
			break;

		case MSG_PATH_MODIFIED:
			message->FindString ("path", &thePath);
			theInfoView->SetPath(thePath);
			break;

		case MSG_PIC_NAME_MODIFIED:
			message->FindString ("dim", &theDim);
			message->FindString ("fname", &theName);
			theInfoView->SetPictureSize(theDim);
			theInfoView->SetFileName(theName);
			break;

		case MSG_ZOOM_FACTOR_MODIFIED:
			message->FindString ("factor", &theDim);	// nom de la chaine importe peu
			theInfoView->SetZoomLevel(theDim);
			break;

		case MSG_PREF_WINDOW:
			r = Frame();
			thePrefWindow->Lock();
			thePrefWindow->MoveTo(r.left + (r.right-r.left)/2 - 150, r.top + (r.bottom-r.top)/2 - 100);
			thePrefWindow->Show();
			thePrefWindow->Sync();
			thePrefWindow->Unlock();
			break;
		
		case MSG_CFG_MODIFIED:
			theInfoView->SetViewColor(((InterApp*)be_app)->prefs->GetInfobarColor());
			backView->SetPanMode(((InterApp*)be_app)->prefs->GetMouseMode());
			break;
			
		case MSG_UPDATE_VIEW_COLOR: {
			const rgb_color* color;
			ssize_t numBytes;
			message->FindData("color", B_RGB_COLOR_TYPE, (const void**)&color, &numBytes);
			theInfoView->SetViewColor(*color);
			}
			break;
		
		case MSG_SAVE_REQUESTED:
			TranslatePicture();
			break;
			
		case  MSG_OUTPUT_FORMAT_SELECTED:
			OpenSaveFilePanel(message);
			break;
		
		case B_SAVE_REQUESTED:
//			copyMsg = new BMessage(message);
//			TranslateAndWrite(copyMsg);
			TranslateAndWrite(message);
			break;
		
		case MSG_SET_CURDESKTOP_CENTERED:
			SetCurrentDesktop(B_BACKGROUND_MODE_CENTERED);
			break;

		case MSG_SET_CURDESKTOP_SCALED:
			SetCurrentDesktop(B_BACKGROUND_MODE_SCALED);
			break;

		case MSG_SET_CURDESKTOP_TILED:
			SetCurrentDesktop(B_BACKGROUND_MODE_TILED);
			break;

		case MSG_SET_ALLDESKTOP_CENTERED:
			SetAllDesktop(B_BACKGROUND_MODE_CENTERED);
			break;

		case MSG_SET_ALLDESKTOP_SCALED:
			SetAllDesktop(B_BACKGROUND_MODE_SCALED);
			break;

		case MSG_SET_ALLDESKTOP_TILED:
			SetAllDesktop(B_BACKGROUND_MODE_TILED);
			break;

		case B_SIMPLE_DATA:
			// Look for a ref in the message
			ref = new entry_ref();
			if( message->FindRef("refs", ref) == B_OK )
			{
				BEntry *entry = new BEntry(ref);
				((InterApp*)be_app)->nav->Cd_Dir(entry);
				delete entry;
			}
			else
				BWindow::MessageReceived(message);
			delete ref;
			break;
			
		default:
			BWindow::MessageReceived(message);
			break;
	}
}


//--------------------------------------------------------------------
void InterWindow::FrameResized(float width, float height)
{
	UpdateLimits(width, height);
//	scrollView->ResizeTo(xLimit, height -BACKBOX_INSET -40.);
//	backView->TomFrameResized(width - xLimit - B_V_SCROLL_BAR_WIDTH, yLimit - B_H_SCROLL_BAR_HEIGHT-1);		// bizarre?!!!???!!?
//	backView->MoveTo(xLimit + 1., 0.+BACKBOX_INSET);

//	scrollBackView->MoveTo(xLimit + 1., 0.);
//	scrollBackView->ResizeTo(width - xLimit, yLimit);

//	theInfoView->MoveTo(xLimit + 1., yLimit + 1);
//	theInfoView->ResizeTo(width - xLimit - cmdViewWidth - 1, height - yLimit);

//	backView->TomFrameResized(width - xLimit - B_V_SCROLL_BAR_WIDTH, yLimit - B_H_SCROLL_BAR_HEIGHT-1);		// bizarre?!!!???!!?
	backView->TomFrameResized();
	
	theInfoView->MoveTo(0.+BACKBOX_INSET, yLimit + 1 - BACKBOX_INSET);
	theInfoView->ResizeTo(width- 2*BACKBOX_INSET - cmdViewWidth - 1, height - yLimit);

	BWindow::FrameResized(width, height);
}

//--------------------------------------------------------------------
void InterWindow::WorkspacesChanged(uint32 oldWorkspaces, uint32 newWorkspaces)
{
	if (((InterApp*)be_app)->nav != NULL)
		if (((InterApp*)be_app)->nav->theLoadingWindow != NULL) {
			((InterApp*)be_app)->nav->theLoadingWindow->SetWorkspaces(newWorkspaces);
		}
}

//--------------------------------------------------------------------
void InterWindow::UpdateLimits(float width, float height)
{
	xLimit = 160;
	yLimit = height -BACKBOX_INSET -40.;	
}


//--------------------------------------------------------------------
void InterWindow::SwapFullScreen()
{
	if (!fullScreenON)
	{
		// passage en full screen
		BScreen screen(this);
		oldFrame = Frame();
		MoveTo(screen.Frame().left, screen.Frame().top);
		ResizeTo(screen.Frame().right, screen.Frame().bottom);
	}
	else
	{
		MoveTo(oldFrame.left, oldFrame.top);
		ResizeTo(oldFrame.right - oldFrame.left, oldFrame.bottom - oldFrame.top);
	}
	fullScreenON = !fullScreenON;
}


//--------------------------------------------------------------------
void InterWindow::SwapFullScreenNoMenu()
{
	if (!realFullScreen)
	{
		// passage en 'vrai' fullscreen
		BScreen screen(this);
		oldFrame2 = Frame();
		
		//MoveTo(screen.Frame().left-161., screen.Frame().top);
		//ResizeTo(screen.Frame().right+158.+18., screen.Frame().bottom+40.+15.);
		MoveTo(screen.Frame().left - backView->Frame().left-5, 
		       screen.Frame().top - backView->Frame().top-5);		
		ResizeTo(screen.Frame().right+backView->Frame().left+5+B_V_SCROLL_BAR_WIDTH+3*BACKBOX_INSET, 
		         screen.Frame().bottom+backView->Frame().top+5+40+B_H_SCROLL_BAR_HEIGHT+4*BACKBOX_INSET+1);
	}
	else
	{
		MoveTo(oldFrame2.left, oldFrame2.top);
		ResizeTo(oldFrame2.right - oldFrame2.left, oldFrame2.bottom - oldFrame2.top);
	}
	
	realFullScreen = !realFullScreen;
}


//--------------------------------------------------------------------
void InterWindow::TranslatePicture()
{
	BInvoker *inv;
	OutputFormatWindow *w;

	if (backView->GetBitmap() == NULL)
		return;

	inStream = new BBitmapStream(backView->GetBitmap());
	if (inStream != NULL)
	{
		inv = new BInvoker(new BMessage(MSG_OUTPUT_FORMAT_SELECTED), this);
		if (inv != NULL  )
		{
			w = new OutputFormatWindow(inStream, inv);
			w->Show();
		}
	}
}

//--------------------------------------------------------------------
void InterWindow::OpenSaveFilePanel(BMessage *msg)
{
	msg->PrintToStream();

	msg->FindInt32("translator", reinterpret_cast<int32 *>(&outTranslator));
	msg->FindInt32("type", reinterpret_cast<int32 *>(&outType));

	BFilePanel *p=new BFilePanel(B_SAVE_PANEL);
	p->SetTarget(this);
	p->Show();
}

//--------------------------------------------------------------------
void InterWindow::TranslateAndWrite(BMessage *msg)
{
	msg->PrintToStream();
	char outFilename[B_FILE_NAME_LENGTH];
	const char *name;
	entry_ref	ref;

	msg->FindRef("directory", &ref);
	BEntry *entry = new BEntry(&ref);
	BPath path;
	entry->GetPath(&path);
	strcpy(outFilename, path.Path());

	msg->FindString("name", &name);
	strcat(outFilename, "/");
	strcat(outFilename, name);
	
	delete entry;
//	printf("ok, save file %s\n", outFilename);
	BFile *outStream = new BFile(outFilename, B_CREATE_FILE | B_WRITE_ONLY);
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	roster->Translate(outTranslator, inStream, NULL, outStream, outType);
	delete outStream;
}

//--------------------------------------------------------------------
void InterWindow::SetAllDesktop(uint32 mode)
{

	status_t err;
	status_t stat;
	BPath path;

	BMessenger tracker( "application/x-vnd.Be-TRAK" );
	BMessage backgrounds, reply;
	

	
	const char *name = theInfoView->GetFullName();
	if (name != NULL)
	{
		backgrounds.AddString( B_BACKGROUND_IMAGE, name);
		backgrounds.AddInt32( B_BACKGROUND_WORKSPACES, 0xffffffff);
		backgrounds.AddInt32( B_BACKGROUND_MODE, mode );
		backgrounds.AddBool( B_BACKGROUND_ERASE_TEXT, true );

		backgrounds.what = B_RESTORE_BACKGROUND_IMAGE;

		ssize_t size=backgrounds.FlattenedSize();
		char *buf = (char*)malloc (backgrounds.FlattenedSize());
		backgrounds.Flatten(buf, size);

		stat = find_directory(B_DESKTOP_DIRECTORY, &path);
		entry_ref ref;
	
		err = get_ref_for_path(path.Path(), &ref);
		BNode *node = new BNode(&ref);
	
		node->WriteAttr(B_BACKGROUND_INFO, B_MESSAGE_TYPE, 0, buf, backgrounds.FlattenedSize());

		reply.MakeEmpty();
		tracker.SendMessage( &backgrounds, &reply );
		if( reply.what == B_MESSAGE_NOT_UNDERSTOOD &&	
			reply.FindInt32( "error", &err ) == B_OK)
		{
			printf("Error feedback tracker\n");
		}
	
		delete node;
		free(buf);
	}
}

//--------------------------------------------------------------------
void InterWindow::SetCurrentDesktop(uint32 mode)
{
	status_t err;
	status_t stat;
	BPath path;

	int32 numW = 1<<current_workspace();

	BMessenger tracker( "application/x-vnd.Be-TRAK" );
	BMessage backgrounds, reply;

	const char *name = theInfoView->GetFullName();

	if (name != NULL)
	{
		stat = find_directory(B_DESKTOP_DIRECTORY, &path);
		entry_ref ref;
	
//		printf ("le chemin %s\n", path.Path());
		err = get_ref_for_path(path.Path(), &ref);
		BNode *node = new BNode(&ref);
	
		char bufread[8000];
		node->ReadAttr(B_BACKGROUND_INFO, B_MESSAGE_TYPE, 0, bufread, 8000);
		BMessage msgSrc;
		msgSrc.Unflatten(bufread);
//		msgSrc.PrintToStream();

		type_code typ; 
		int32 nbConfig;
		msgSrc.GetInfo(B_BACKGROUND_WORKSPACES, &typ, &nbConfig);

		int32 workspaces;
		bool insertNewConfig = false;
		for (int i=0; i<nbConfig && !insertNewConfig; i++)
		{
			workspaces = msgSrc.FindInt32( B_BACKGROUND_WORKSPACES, i);
			if (numW & workspaces)
			{
				if (workspaces == numW) {	// config 1 seul workspace
					msgSrc.ReplaceString( B_BACKGROUND_IMAGE, i, name);
					msgSrc.ReplaceInt32( B_BACKGROUND_MODE, i, mode );
					msgSrc.ReplaceBool( B_BACKGROUND_ERASE_TEXT, i, true );
					i = nbConfig;
				}
				else {
					// current config for the workspace
					msgSrc.ReplaceInt32( B_BACKGROUND_WORKSPACES, i, workspaces & ~numW);
					insertNewConfig=true;
				}	
			}
		}
		if (insertNewConfig)
		{
			msgSrc.AddString( B_BACKGROUND_IMAGE, name);
			msgSrc.AddInt32( B_BACKGROUND_WORKSPACES, numW);
			msgSrc.AddInt32( B_BACKGROUND_MODE, mode );
			msgSrc.AddBool( B_BACKGROUND_ERASE_TEXT, true );
		}

		ssize_t size=msgSrc.FlattenedSize();
		char *buf = (char*)malloc (size);
		msgSrc.Flatten(buf, size);
		node->WriteAttr(B_BACKGROUND_INFO, B_MESSAGE_TYPE, 0, buf, msgSrc.FlattenedSize());
		
	
		msgSrc.what = B_RESTORE_BACKGROUND_IMAGE;

	//	printf("*************************** apres\n");
	//	msgSrc.PrintToStream();

		reply.MakeEmpty();
		tracker.SendMessage( &msgSrc, &reply );
		if( reply.what == B_MESSAGE_NOT_UNDERSTOOD &&	
			reply.FindInt32( "error", &err ) == B_OK)
		{
			printf("error from Tracker feedback\n");
		}

		delete node;
		free(buf);
	}
}

//--------------------------------------------------------------------
BRect InterWindow::NoneFullScreenFrame()
{
//	if (realFullScreen)
//		return oldFrame2;
//	if (fullScreenON)
//	else
		return oldFrame;
//	else 
	//	return Frame();
	// Bloquant si la fenetre est deja fermee
	// cf QuitRequested(), c'est la que je sauve la taille de la fenetre
}


