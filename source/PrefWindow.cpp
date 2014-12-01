#include "PrefWindow.h"
#include "Constants.h"
#include "InterApp.h"
#include "InterWindow.h"
#include "BFPrefs.h"
#include "StringsRsrc.h"
#include <stdio.h>		// DEBUG
#include <strings.h>
#include <View.h>
#include <StringView.h>
#include <RadioButton.h>
#include <ColorControl.h>
#include <Button.h>
#include <Box.h>
#include <MenuItem.h>

#include <File.h>

const int PREF_W = 315;
const int PREF_H = 275;


//--------------------------------------------------------------------
PrefWindow::PrefWindow(BWindow *main) 
	: BWindow(BRect(0, 0, PREF_W-1, PREF_H-1), StringsRsrc[STR_PREFERENCES_TITLE], 
              B_FLOATING_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, 0, B_ALL_WORKSPACES)
{
	mainWin = main;

	// Set windows attributes
	MoveTo(28, 80);
	SetSizeLimits(300., 10000., 200., 10000.);
	SetFlags(B_NOT_RESIZABLE | B_NOT_ZOOMABLE);

	// Create background view
	BView *view = new BView(BRect(0, 0, PREF_W-1, PREF_H-1), "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS);
	AddChild(view);
	view->SetViewColor(222, 222, 222);

	BBox *box;
	box = new BBox (BRect(10., 10., 300., 100.), "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE);
	box->SetLabel(StringsRsrc[STR_INFOBAR_COLOR]);
	view->AddChild(box);

	colSelector = new BColorControl(BPoint(10., 20.), B_CELLS_32x8, 2., "", new BMessage(MSG_PREF_COLOR), false);
	box->AddChild(colSelector);
	colSelector->SetValue(((InterApp*)be_app)->prefs->GetInfobarColor());


	box = new BBox (BRect(10., 110., 300., 175.), "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE);
	box->SetLabel(StringsRsrc[STR_PAN_MODE]);
	view->AddChild(box);

	radFollow = new BRadioButton(BRect(10., 15., 100., 35.), "", StringsRsrc[STR_PICTURE_FOLLOW_MOUSE], NULL);
	radFollow->ResizeToPreferred();
	box->AddChild(radFollow);
	radFlee = new BRadioButton(BRect(10., 35., 100., 55.), "", StringsRsrc[STR_PICTURE_FLEE_MOUSE], NULL);
	radFlee->ResizeToPreferred();
	box->AddChild(radFlee);

	if (((InterApp*)be_app)->prefs->GetMouseMode() == PIC_FOLLOW_MOUSE)
		radFollow->SetValue(1);
	else
		radFlee->SetValue(1);

	// Language
	box = new BBox(BRect(10., 185., 300., 230.), "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE);
	box->SetLabel(StringsRsrc[STR_CHOOSE_LANGUAGE]);
	view->AddChild(box);
	
	BPopUpMenu* menu = new BPopUpMenu("languages menu");
	BMenuItem* item;

	item = new BMenuItem("[BR] Português-Brasileiro", NULL);
	menu->AddItem(item);
	if (((InterApp*)be_app)->prefs->GetLanguage() == PORTUGUES)
		item->SetMarked(true);
		
	item = new BMenuItem("[DE] Deutsch", NULL);
	menu->AddItem(item);
	if (((InterApp*)be_app)->prefs->GetLanguage() == GERMAN)
		item->SetMarked(true);

	item = new BMenuItem("[EN] English", NULL);
	menu->AddItem(item);
	if (((InterApp*)be_app)->prefs->GetLanguage() == ENGLISH)
		item->SetMarked(true);

	item = new BMenuItem("[ES] Spanish/Castellano", NULL);
	menu->AddItem(item);
	if (((InterApp*)be_app)->prefs->GetLanguage() == SPANISH)
		item->SetMarked(true);
		
	item = new BMenuItem("[EU] Euskera/Basque", NULL);
	menu->AddItem(item);
	if (((InterApp*)be_app)->prefs->GetLanguage() == BASQUE)
		item->SetMarked(true);
	
	item = new BMenuItem("[FR] Français", NULL);
	menu->AddItem(item);
	if (((InterApp*)be_app)->prefs->GetLanguage() == FRENCH)
		item->SetMarked(true);
		
	item = new BMenuItem("[IT] Italiano", NULL);
	menu->AddItem(item);
	if (((InterApp*)be_app)->prefs->GetLanguage() == ITALIAN)
		item->SetMarked(true);
		
	item = new BMenuItem("[RU] Русский", NULL);
	menu->AddItem(item);
	if (((InterApp*)be_app)->prefs->GetLanguage() == RUSSIAN)
		item->SetMarked(true);
				
	languageMenu = new BMenuField(BRect(10,15,150,35),"","",menu);
	languageMenu->SetDivider(0);
	box->AddChild(languageMenu);	

	BButton *but;
	but = new BButton(BRect(145., 240., 150., 260.), "", StringsRsrc[STR_OK], new BMessage(MSG_PREF_OK));
	but->ResizeToPreferred();
	but->MakeDefault(true);
	view->AddChild(but);
	but = new BButton(BRect(225., 240., 200., 260.), "", StringsRsrc[STR_CANCEL], new BMessage(MSG_PREF_CANCEL));
	but->ResizeToPreferred();
	view->AddChild(but);
	
	// Show the window
	Show();
	Sync();
}


//--------------------------------------------------------------------
void PrefWindow::MessageReceived(BMessage *message)
{
	int32 lang = ENGLISH;
	BMenuItem* item;
	
	switch ( message->what )
	{
		case MSG_PREF_COLOR: {
			rgb_color color = colSelector->ValueAsColor();
			BMessage* msg = new BMessage(MSG_UPDATE_VIEW_COLOR);
			msg->AddData("color", B_RGB_COLOR_TYPE, &color, sizeof(rgb_color));
					
			BMessenger *msng = new BMessenger(mainWin);
			msng->SendMessage(msg);			
			delete msng;
			}
			break;
			
		case MSG_PREF_OK:
			((InterApp*)be_app)->prefs->SetInfobarColor(colSelector->Value());

			if (radFollow->Value() != 0)
				((InterApp*)be_app)->prefs->SetMouseMode(PIC_FOLLOW_MOUSE);
			else
				((InterApp*)be_app)->prefs->SetMouseMode(PIC_FLEE_MOUSE);

			item = languageMenu->Menu()->FindMarked();
			if (item != NULL) {
				if (strstr(item->Label(), "[EN]")) {
					lang = ENGLISH;
				} else if (strstr(item->Label(), "[FR]")) {
					lang = FRENCH;
				} else if (strstr(item->Label(), "[RU]")) {
					lang = RUSSIAN;
				} else if (strstr(item->Label(), "[BR]")) {
					lang = PORTUGUES;
				} else if (strstr(item->Label(), "[IT]")) {
					lang = ITALIAN;
				} else if (strstr(item->Label(), "[ES]")) {
					lang = SPANISH;
				} else if (strstr(item->Label(), "[EU]")) {
					lang = BASQUE;
				} else if (strstr(item->Label(), "[DE]")) {
					lang = GERMAN;
				}
			}
			
			if (lang != ((InterApp*)be_app)->prefs->GetLanguage()) {
				((InterApp*)be_app)->prefs->SetLanguage(lang);
					                               
				BAlert *theAlert = new BAlert("", StringsRsrc[STR_LANG_CHANGED_WARNING],
				                              StringsRsrc[STR_OK]);
				theAlert->Go();
			}

			SetAndWriteAttributes();
			Hide();
			break;

		case MSG_PREF_CANCEL: {
			// restore color
			rgb_color color = ((InterApp*)be_app)->prefs->GetInfobarColor();
			BMessage* msg = new BMessage(MSG_UPDATE_VIEW_COLOR);
			msg->AddData("color", B_RGB_COLOR_TYPE, &color, sizeof(rgb_color));
					
			BMessenger *msng = new BMessenger(mainWin);
			msng->SendMessage(msg);			
			delete msng;
			}
			Hide();
			break;

		default:
			BWindow::MessageReceived(message);
			break;
	}
}


//--------------------------------------------------------------------
void PrefWindow::Show(void)
{
	colSelector->SetValue(((InterApp*)be_app)->prefs->GetInfobarColor());
	if (((InterApp*)be_app)->prefs->GetMouseMode() == PIC_FOLLOW_MOUSE)
		radFollow->SetValue(1);
	else
		radFlee->SetValue(1);
	
	BWindow::Show();
}


//--------------------------------------------------------------------
PrefWindow::~PrefWindow()
{	
	
	// Quit window
	Hide();
	Sync();
}


//--------------------------------------------------------------------
bool PrefWindow::QuitRequested(void)
{	
	Hide();
	return false;
}

			
//--------------------------------------------------------------------
void PrefWindow::SetAndWriteAttributes()
{
	// Send message to main window
	BMessenger *msng = new BMessenger(mainWin);
	msng->SendMessage(new BMessage(MSG_CFG_MODIFIED));
	delete msng;

	((InterApp*)be_app)->prefs->Write();
}

//--------------------------------------------------------------------
void PrefWindow::RestoreOldAttributes()
{
}
