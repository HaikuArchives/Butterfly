#include <AppKit.h>
#include <storage/NodeInfo.h>
#include <storage/Path.h>
#include <storage/SymLink.h>
#include <Directory.h>
#include <TranslationKit.h>
#include <TranslationUtils.h>
#include <View.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "InterApp.h"
#include "FSNav.h"
#include "FileItem.h"
#include "Constants.h"
#include "StringsRsrc.h"

#include <Beep.h>

//=========================================================================
//	LoadingWindow class
//
//=========================================================================

//----------------------------------------------------------------------------------
LoadingWindow::LoadingWindow(int32 xpos, int32 ypos)
	: BWindow(BRect(0, 0, 100., 40.), "Loading", B_MODAL_WINDOW, 0, B_CURRENT_WORKSPACE)	  
{
	// Set windows attributes
	MoveTo(xpos, ypos);
	SetFlags(B_NOT_RESIZABLE | B_NOT_MOVABLE);

	// Create background view
	BView *view = new BView(BRect(0, 0, 100., 40.), "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS);
	AddChild(view);
	view->SetViewColor(222, 222, 222);

	BStringView *s;
	s = new BStringView(BRect(15., 15., 100., 40.), "", StringsRsrc[STR_LOADING], 0);
	s->ResizeToPreferred();
	view->AddChild(s);

//	Show();
	Sync();
//	Hide();
}


//----------------------------------------------------------------------------------
LoadingWindow::~LoadingWindow()
{
}


//=========================================================================
//	FSNav class
//
//=========================================================================

int CompareItems(const void *itemA, const void *itemB);

//----------------------------------------------------------------------------------
FSNav::FSNav(BLooper *msgDest, const char *filename): theLoadingWindow(NULL)
{
	this->msgDest = msgDest;

	currentDir = new BDirectory("/boot/home");
	if (filename != NULL && strcmp(filename,""))
	{
		BBitmap *inib = BTranslationUtils::GetBitmapFile(filename);	// try to load pic
		if (inib == NULL)
			printf("fichier incorrect: %s\n", filename);
		else {
			printf("fichier bitmap ok\n");
			
			BEntry entry(filename);
			Cd_Dir(&entry);
			
			SetNewBitmap(inib, &entry);
		}
	}
	UpdateFileList();
	SendPathModified();

//	theLoadingWindow = new LoadingWindow();
//	theLoadingWindow->Hide();
}


//----------------------------------------------------------------------------------
FSNav::~FSNav()
{
//	delete theLoadingWindow;
}

//----------------------------------------------------------------------------------
/*
void FSNav::list_curdir()
{
	BEntry the_dir;
	currentDir->GetEntry(&the_dir);
	BPath the_path;   
	the_dir.GetPath(&the_path);
	
	printf("entry name %s(end)\n", the_path.Path());
	
	BEntry a_file;
	while (currentDir->GetNextEntry(&a_file) != B_ENTRY_NOT_FOUND)
	{
		char name[B_FILE_NAME_LENGTH]; 
		a_file.GetName(name);
		printf("--- %s\n", name);
	}
	currentDir->Rewind();
}
*/

//----------------------------------------------------------------------------------
/*
void FSNav::Cd_AppDir()
{
	// CD to application directory
	app_info the_info;
	be_app->GetAppInfo(&the_info);
	BEntry the_file(&the_info.ref);
	
	the_file.GetParent(&curDir);
	BPath the_path;
	curDir.GetPath(&the_path);
	chdir(the_path.Path());
	
	UpdateFileList();
	SendPathModified();
}
*/

//----------------------------------------------------------------------------------
void FSNav::Cd_ParentDir()
{
	if (!rootDir)
	{
		BEntry the_dir;
		currentDir->GetEntry(&the_dir);

		BEntry parentDir;
		the_dir.GetParent(&parentDir);
	
		currentDir->SetTo(&parentDir);

		UpdateFileList();
	}
}


//----------------------------------------------------------------------------------
void FSNav::Cd_Dir(BEntry *entry)
// entry can be a file, a directory or a symbolic link
{
	BEntry *en = new BEntry(*entry);
	char buf[B_PATH_NAME_LENGTH];
	
	BSymLink link;
	while (en->IsSymLink())
	{
		link.SetTo(en);
		link.ReadLink(buf, B_PATH_NAME_LENGTH);
		if (link.IsAbsolute())
			en->SetTo(buf);
	}

	BEntry the_dir;
	currentDir->GetEntry(&the_dir);

	if (en->IsDirectory())
	{
		if (the_dir != *en)
		{
			currentDir->SetTo(en);
			SendPathModified();
			UpdateFileList();
		}

	}
	else	// file
	{
		BEntry par;
		en->GetParent(&par);
		if (the_dir != par)
		{
			currentDir->SetTo(&par);
			SendPathModified();
			UpdateFileList();
			en->GetName(buf);
			SelectFile(buf);
		}
		TryToLoadBitmapFromFile(en);
	}

	delete en;
}

//-----------------------------------------------------------------------------------
void FSNav::UpdateFileList()
{
	BListView *theList = ((InterApp*)be_app)->fileList;

	((InterApp*)be_app)->theWin->Lock();

	// Detruire les items precedents
	int last = theList->CountItems();
	for ( int32 i = last; i > 0; i-- )
		theList->RemoveItem((int32)0);

	// Creer une nouvelle liste pour le repertoire courant
	BList* dirList = new BList(20);


	BEntry file;
	currentDir->Rewind();
	while (currentDir->GetNextEntry(&file) != B_ENTRY_NOT_FOUND)
	{
		char name[B_FILE_NAME_LENGTH]; 
		file.GetName(name);

		if (file.IsDirectory())
			dirList->AddItem(new FileItem(name, IS_DIR));
		else {
#ifdef BT_SHOW_IMAGE_FILE_ONLY		
			// Check mime type, only keep IMAGE
			BNode* node = new BNode(&file);
			BNodeInfo nodeInfo(node);
			char mimeType[256];
			nodeInfo.GetType(mimeType);
			if (strstr(mimeType,"image/") != NULL || IsAnImage(name) )
				theList->AddItem(new FileItem(name, IS_FILE));
			delete node;
#else				
			theList->AddItem(new FileItem(name, IS_FILE));
#endif			
		}
		file.Unset();
	}
	currentDir->Rewind();
	
	dirList->SortItems(CompareItems);
	theList->SortItems(CompareItems);
	
	//--
	BEntry the_dir;
	currentDir->GetEntry(&the_dir);
	BPath the_path;
	the_dir.GetPath(&the_path);
	if (strcmp(the_path.Path(), "/"))
	{
		dirList->AddItem(new FileItem("<parent folder>", IS_DIR),0);	// parent dir
		rootDir = false;
	}
	else
		rootDir = true;
	//--
	theList->AddList(dirList, 0);
	
	delete dirList;

	((InterApp*)be_app)->theWin->Unlock();
}

//-----------------------------------------------------------------------------------
int CompareItems(const void *itemA, const void *itemB) {
	FileItem** a; 
	a = (FileItem**) itemA;
	FileItem** b;
	b = (FileItem**) itemB;
	return strcasecmp((*a)->getText(),	(*b)->getText());
}

//-----------------------------------------------------------------------------------
void FSNav::FileSelected(int32 num)
{
	BListView *theList = ((InterApp*)be_app)->fileList;

	if(num > 0)
	{
		BEntry entry;
		BStringItem *item = (BStringItem*)theList->ItemAt(num);
		currentDir->FindEntry(item->Text(), &entry);
		BDirectory *newDir = new BDirectory(&entry);
		if (newDir->InitCheck() == B_OK)
		{	// it's a directory : do nothing
			delete newDir;
		}
		else {
			TryToLoadBitmapFromFile(&entry);
		}
	}
}


//----------------------------------------------------------------------------------
void FSNav::FileInvocated(int32 num)
{
	BListView *theList = ((InterApp*)be_app)->fileList;

	if (num == 0 && !rootDir)
	{
		Cd_ParentDir();
		SendPathModified();
	}
	else if(num != -1)
	{
		BEntry entry;
		BStringItem *item = (BStringItem*)theList->ItemAt(num);
		currentDir->FindEntry(item->Text(), &entry);
		BDirectory *newDir = new BDirectory(&entry);
		if (newDir->InitCheck() == B_OK)
		{
			delete currentDir;
			currentDir = newDir;
			UpdateFileList();

			// the path has changed
			SendPathModified();
		}
		else
		{
			TryToLoadBitmapFromFile(&entry);
		}
	}
}


//----------------------------------------------------------------------------------
void FSNav::SendPathModified()
{
	status_t status;
	BEntry entry;
	BPath path;
	
	status = currentDir->GetEntry(&entry);
	if (status != B_OK) {
		printf("FSNav::SendPathModified() : GetEntry() error %lX\n",status);		
	}

	BMessage *msg = new BMessage(MSG_PATH_MODIFIED);
	//do {
	status = entry.GetPath(&path);
	//} while (status != B_OK);
	if (status != B_OK) {
		printf("FSNav::SendPathModified() : GetPath() error %lX\n",status);
		strcpy(filename,"");
	} else {
		strcpy(filename, path.Path());
	}
	msg->AddString("path", filename);
	msgDest->PostMessage(msg);
}


//----------------------------------------------------------------------------------
void FSNav::TryToLoadBitmapFromFile(BEntry *entry)
{
	struct stat st;
	BRect r = ((InterApp*)be_app)->theWin->Frame();
	
	entry->GetStat(&st);
	if (st.st_size > 100000) {	
		if (!theLoadingWindow)
			theLoadingWindow = new LoadingWindow(r.left + (r.right -r.left)/2, r.top + (r.bottom -r.top)/2);
		else
			theLoadingWindow->MoveTo(r.left + (r.right -r.left)/2, r.top + (r.bottom -r.top)/2);
		theLoadingWindow->Show();
	}
	
	BPath p;
	entry->GetPath(&p);
	BBitmap *b = BTranslationUtils::GetBitmapFile(p.Path());	// try to load pic

	if (b != NULL)
		SetNewBitmap(b, entry);

	if (st.st_size > 100000) {	
		theLoadingWindow->Hide();
	}
//	theLoadingWindow->Lock();
//	theLoadingWindow->Quit();
}


//----------------------------------------------------------------------------------
void FSNav::SetNewBitmap(BBitmap *b, BEntry *entry)
{
	((InterApp*)be_app)->theWin->backView->SetBitmap(b);
	
	BMessage *msg = new BMessage(MSG_PIC_NAME_MODIFIED);

	entry->GetName(filename);

	BRect r = b->Bounds();
	sprintf (dimString, "%dx%d", (int)r.right+1, (int)r.bottom+1);

	msg->AddString("fname", filename);
	msg->AddString("dim", dimString);
	msgDest->PostMessage(msg);
}

//----------------------------------------------------------------------------------
bool FSNav::IsAnImage(const char* name)
{
	char s[256];
	
	for (uint32 i=0; i < 256 && i<strlen(name); i++)
		s[i]=tolower(name[i]);

	if (strstr(s,".jpg")  ||
		strstr(s,".gif")  ||
		strstr(s,".png")  ||
		strstr(s,".jpe")  ||
		strstr(s,".jpeg") ||
		strstr(s,".tga")  ||
		strstr(s,".bmp")  ||
		strstr(s,".pcx")  ||
		strstr(s,".ppm")  ||
		strstr(s,".iff")  )
		return true;
	else
		return false;
}

//----------------------------------------------------------------------------------
void FSNav::SelectFile(char* name) 
{
	BListView *theList = ((InterApp*)be_app)->fileList;
	const BListItem **ptr = theList->Items();
	FileItem* item = NULL;

	for ( long i = theList->CountItems(); i > 0; i-- ) {
		item = (FileItem *) *ptr;
	
		if ( !strcmp( item->getText(), name) ) {
			theList->Select(theList->IndexOf(item));
			break;
		}	
		*ptr++;
	} 
}
