//#include <ApplicationKit.h>
#include "FileList.h"
#include "FileItem.h"
#include "Constants.h"
#include "InterApp.h"
#include "StringsRsrc.h"

#include <Directory.h>
#include <storage/Path.h>
#include <TranslationUtils.h>
#include <stdio.h>

//-----------------------------------------------------------------------------------
CFileList::CFileList(BRect frame)
	: BListView(frame, "fileList", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL)
{
	SetSelectionMessage(new BMessage(MSG_FILE_SELECTED));
	SetInvocationMessage(new BMessage(MSG_FILE_INVOCATED));
}


//-----------------------------------------------------------------------------------
CFileList::~CFileList()
{
}


//-----------------------------------------------------------------------------------
void CFileList::SetBackground()
{
	BBitmap *back = BTranslationUtils::GetBitmapFile("deepswirl.png");
	SetViewBitmap(back);
}

//-----------------------------------------------------------------------------------
void CFileList::MessageReceived(BMessage *message)
{
	float wheel_delta;
	int32 index;	
	
/*	message->PrintToStream();*/

	entry_ref ref;
	switch ( message->what )
	{
		case B_MOUSE_WHEEL_CHANGED:
			message->FindFloat("be:wheel_delta_y",&wheel_delta);			
			index = CurrentSelection(0);
			/* there is no error if we provide a non valid index to select */
			index += (int32) wheel_delta;
			Select(index);
			ScrollToSelection();
			break;
			
		default:
			BListView::MessageReceived(message);
			break;
	}
}

//-----------------------------------------------------------------------------------
void CFileList::KeyDown(const char *bytes, int32 numBytes)
{
	if ( numBytes == 1 )
	{
		if (modifiers() & B_SHIFT_KEY)
		{
			switch ( bytes[0] )
			{
				case '+':
					((InterApp*)be_app)->picView->ZoomIn();
					break;
				case B_LEFT_ARROW:
					((InterApp*)be_app)->picView->ScrollRequested(SCROLL_LEFT);
					break;
				case B_RIGHT_ARROW:
					((InterApp*)be_app)->picView->ScrollRequested(SCROLL_RIGHT);
					break;
				case B_DOWN_ARROW:
					((InterApp*)be_app)->picView->ScrollRequested(SCROLL_DOWN);
					break;
				case B_UP_ARROW:
					((InterApp*)be_app)->picView->ScrollRequested(SCROLL_UP);
					break;
				default:
					BListView::KeyDown(bytes, numBytes);
					break;
			}
		}
		else
		{
			switch ( bytes[0] )
			{
				case '+':
					((InterApp*)be_app)->picView->ZoomIn();
					break;
				case '-':
					((InterApp*)be_app)->picView->ZoomOut();
					break;
				case '*':
					((InterApp*)be_app)->picView->ResetZoom();
					break;
				case '.':
				case ' ':
					((InterApp*)be_app)->theWin->SwapFullScreenNoMenu();
					break;
				case B_DELETE:
					DeleteCurrentPicture();
					break;
				case B_BACKSPACE:
					if ( !strcmp(((FileItem*)ItemAt(0))->getText(),"<parent folder>") ) {
						BMessage* msg = new BMessage(MSG_FILE_INVOCATED);
						msg->AddInt32("index", 0); // 0 for parent
						((InterApp*)be_app)->theWin->PostMessage(msg);
					}
					break;
				default:
					BListView::KeyDown(bytes, numBytes);
					break;
			}
		}
	}
}

//-----------------------------------------------------------------------------------
void CFileList::DeleteCurrentPicture() {
	char deleteStr[512];

	int32 itemIndex = CurrentSelection();
	if (itemIndex >= 0) {
		BStringItem *item = (BStringItem*)ItemAt(itemIndex);
		
		if ( ((FileItem*)item)->getType() == IS_DIR) {
			return;
		}
		
		sprintf(deleteStr, "%s : %s?", StringsRsrc[STR_DELETE_QUESTION], item->Text());
		BAlert *the_alert = new BAlert("Delete...", deleteStr, 
		                               StringsRsrc[STR_DELETE_YES],
		                               StringsRsrc[STR_DELETE_NO],
		                               NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		the_alert->SetShortcut(0, B_ESCAPE);
		int32 rc = the_alert->Go();
		if (rc == 1) { // Delete
			RemoveItem(item);			
			if (CountItems() > 1) {
				if (itemIndex <= (CountItems()-1))
					Select(itemIndex);
				else
					Select(itemIndex-1);				
			}
			
			// Delete file in BFS
			BPath* path = new BPath();
			BEntry* curDir = new BEntry();
			((InterApp*)be_app)->nav->currentDir->GetEntry(curDir);
			curDir->GetPath(path);		
			sprintf(deleteStr,"%s/%s", path->Path(), item->Text());
			//printf("%s\n",deleteStr);
			BEntry *delFile = new BEntry(deleteStr);
			delFile->Remove();
			delete delFile;	
			delete curDir;
			delete path;		
		}
	}
}
