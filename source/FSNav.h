#ifndef _FSNAV_H_
#define _FSNAV_H_

#include <Window.h>

class LoadingWindow : public BWindow
{
	public:
		LoadingWindow(int32 xpos, int32 ypos);
		~LoadingWindow();
};


class FSNav
{
// Methods
	public:
		FSNav(BLooper *msgDest, const char *filename);
		~FSNav();
//		void Cd_AppDir();
		void Cd_ParentDir();
		void Cd_Dir(BEntry *ref);
//		void list_curdir();
		void UpdateFileList();
		
		void FileSelected(int32 num);
		void FileInvocated(int32 num);

//		void TryToLoadInitialBitmapFromFile(char *filename);

	private:
		void SendPathModified();
		void TryToLoadBitmapFromFile(BEntry *entry);
		void SetNewBitmap(BBitmap *b, BEntry *entry);
		bool IsAnImage(const char* name);
		void SelectFile(char* name);

// Attributes
	public:
		char		filename[B_FILE_NAME_LENGTH];
		char		dimString[16];
		LoadingWindow* theLoadingWindow;
		BDirectory* currentDir;

	private:
		BEntry		curDir;
				
		bool		rootDir;
		
		BLooper*    msgDest;
		BBitmap*    inib;
		
};


#endif

