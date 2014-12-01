#include "BFPrefs.h"
#include "Constants.h"
#include "StringsRsrc.h"

#include <stdio.h>
#include <File.h>
#include <TypeConstants.h>

// Global variable for string resources
char **StringsRsrc;

//--------------------------------------------------------------------
BFPrefs::BFPrefs() {
}

//--------------------------------------------------------------------
void BFPrefs::Write() {
	// Create new setting file
	FILE *fp;
	fp = fopen(CFG_FILENAME, "w");
	fclose(fp);
	
	BFile *f = new BFile(CFG_FILENAME, B_READ_WRITE);
	if (f->InitCheck() == B_NO_INIT)
		printf("pb init cfg file\n");

	f->WriteAttr(ATT_INFOBAR_COLOR, B_INT32_TYPE, 0, &infoBarColor, sizeof(int32));
	f->WriteAttr(ATT_MOUSE_MODE, B_INT32_TYPE, 0, &mouseMode, sizeof(int32));
	f->WriteAttr(ATT_AUTOFIT,B_INT32_TYPE, 0, &autofitMode, sizeof(int32));
	f->WriteAttr(ATT_FULLSCREEN,B_INT32_TYPE, 0, &fullscreenMode, sizeof(int32));
	
	int32 x = (int32) winFrame.left; 
	int32 y = (int32) winFrame.top; 
	int32 w = (int32) winFrame.Width();
	int32 h = (int32) winFrame.Height();	
	
	f->WriteAttr(ATT_XPOS,B_INT32_TYPE, 0, &x, sizeof(int32));
	f->WriteAttr(ATT_YPOS,B_INT32_TYPE, 0, &y, sizeof(int32));
	f->WriteAttr(ATT_WIDTH,B_INT32_TYPE, 0, &w, sizeof(int32));
	f->WriteAttr(ATT_HEIGHT,B_INT32_TYPE, 0, &h, sizeof(int32));

	f->WriteAttr(ATT_LANGUAGE,B_INT32_TYPE, 0, &language, sizeof(int32));
	
	delete f;
}

//--------------------------------------------------------------------
void BFPrefs::Read() {
	BFile *f;
	f = new BFile(CFG_FILENAME, B_READ_WRITE);
	if (f->InitCheck() == B_NO_INIT)
	{
		printf("pb init cfg file\n");
	}

	if (f->ReadAttr(ATT_INFOBAR_COLOR, B_INT32_TYPE, 0, &infoBarColor, sizeof(int32)) != sizeof(int32))
		infoBarColor = DEFAULT_INFOBAR_COLOR;

	if (f->ReadAttr(ATT_MOUSE_MODE, B_INT32_TYPE, 0, &mouseMode, sizeof(int32)) != sizeof(int32))
		mouseMode = DEFAULT_MOUSE_MODE;

	if (f->ReadAttr(ATT_AUTOFIT, B_INT32_TYPE, 0, &autofitMode, sizeof(int32)) != sizeof(int32))
		autofitMode = FALSE;

	if (f->ReadAttr(ATT_FULLSCREEN, B_INT32_TYPE, 0, &fullscreenMode, sizeof(int32)) != sizeof(int32))
		fullscreenMode = FALSE;
		
	int32 x; int32 y; int32 w; int32 h;	
		
	if (f->ReadAttr(ATT_XPOS, B_INT32_TYPE, 0, &x, sizeof(int32)) != sizeof(int32))
		x = 100;
	if (f->ReadAttr(ATT_YPOS, B_INT32_TYPE, 0, &y, sizeof(int32)) != sizeof(int32))
		y = 100;
	if (f->ReadAttr(ATT_WIDTH, B_INT32_TYPE, 0, &w, sizeof(int32)) != sizeof(int32))
		w = 600;
	if (f->ReadAttr(ATT_HEIGHT, B_INT32_TYPE, 0, &h, sizeof(int32)) != sizeof(int32))
		h = 400;
	winFrame = BRect(x, y, x+w, y+h);
	
	if (f->ReadAttr(ATT_LANGUAGE, B_INT32_TYPE, 0, &language, sizeof(int32)) != sizeof(int32))
		language = ENGLISH;	
	
	switch (language) {
		case FRENCH:
			StringsRsrc = FrenchStrings;
			break;
		case RUSSIAN:
			StringsRsrc = RussianStrings;
			break;
		case PORTUGUES:
			StringsRsrc = PortuguesStrings;
			break;
		case ITALIAN:
			StringsRsrc = ItalianStrings;
			break;
		case BASQUE:
			StringsRsrc = EuskeraStrings;
			break;
		case SPANISH:
			StringsRsrc = SpanishStrings;
			break;
		case GERMAN:
			StringsRsrc = GermanStrings;
			break;
		case ENGLISH:
		default:
			StringsRsrc = EnglishStrings;
			break;
	}
	
	delete f;
}

//--------------------------------------------------------------------
rgb_color BFPrefs::GetInfobarColor() {
	rgb_color c = {(infoBarColor & 0xff000000)>>24,
					(infoBarColor & 0x00ff0000)>>16,
					(infoBarColor & 0x0000ff00)>>8,
					0};
	return c;
}

//--------------------------------------------------------------------
int32 BFPrefs::GetXPos() {
	return (int32) winFrame.left;
}

//--------------------------------------------------------------------
int32 BFPrefs::GetYPos() {
	return (int32) winFrame.top;
}

//--------------------------------------------------------------------
int32 BFPrefs::GetWidth() {
	return (int32) winFrame.Width();
}

//--------------------------------------------------------------------
int32 BFPrefs::GetHeight() {
	return (int32) winFrame.Height();
}

//--------------------------------------------------------------------
int32 BFPrefs::GetLanguage() {
	return language;
}

//--------------------------------------------------------------------
void BFPrefs::SetInfobarColor(int32 color) {
	infoBarColor = color;
}

//--------------------------------------------------------------------
void BFPrefs::SetXPos(int32 x) {
	winFrame.left = x;
}

//--------------------------------------------------------------------
void BFPrefs::SetYPos(int32 y) {
	winFrame.top = y;
}

//--------------------------------------------------------------------
void BFPrefs::SetWidth(int32 width) {
	winFrame.right = winFrame.left + width - 1;
}

//--------------------------------------------------------------------
void BFPrefs::SetHeight(int32 height) {
	winFrame.bottom = winFrame.top + height -1;
}

//--------------------------------------------------------------------
void BFPrefs::SetLanguage(int32 lang) {

	language = lang;
	
	switch (language) {
		case FRENCH:
			StringsRsrc = FrenchStrings;
			break;
		case RUSSIAN:
			StringsRsrc = RussianStrings;
			break;
		case PORTUGUES:
			StringsRsrc = PortuguesStrings;
			break;
		case ITALIAN:
			StringsRsrc = ItalianStrings;
			break;
		case BASQUE:
			StringsRsrc = EuskeraStrings;
			break;
		case SPANISH:
			StringsRsrc = SpanishStrings;
			break;
		case GERMAN:
			StringsRsrc = GermanStrings;
			break;
		case ENGLISH:
		default:
			StringsRsrc = EnglishStrings;
			break;
	}
}
