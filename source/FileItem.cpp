#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <GraphicsDefs.h>
#include <View.h>
#include "FileItem.h"


//-----------------------------------------------------------------------------------
FileItem::FileItem(char *s, int32 t)
		: BListItem(), name(NULL)
{ 
	name = new char[strlen(s)+1];
	strcpy(name, s);        
    type = t;
}

//-----------------------------------------------------------------------------------
FileItem::~FileItem()
{
	delete name;
}


//-----------------------------------------------------------------------------------
void FileItem::DrawItem(BView *owner, BRect frame, bool complete)
{ 

	if (IsSelected() || complete)
	{
		rgb_color color;
		if (IsSelected()) {
			//color.red = color.green = color.blue = 160;		// NUL !!
			color.red = color.green = color.blue = 216;
		} else {
			color = owner->ViewColor();
		}
		owner->SetHighColor(color);
		owner->FillRect(frame);
		owner->SetLowColor(color);
	}

	owner->MovePenTo(frame.left+4, frame.bottom-2);
	if (IsEnabled()) {
		if (type == IS_DIR)
			owner->SetHighColor(128,0,0);
		else
		{
			owner->MovePenTo(frame.left+14, frame.bottom-2);
			owner->SetHighColor(0,0,0);
		}
	} 
	else { 
		owner->SetHighColor(128,128,128);
	} 
	owner->DrawString(name);
	owner->SetLowColor(owner->ViewColor());
}

//-----------------------------------------------------------------------------------
const char* FileItem::getText() 
{
	return name;
}

//-----------------------------------------------------------------------------------
int32 FileItem::getType() 
{
	return type;
}


