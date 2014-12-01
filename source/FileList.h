#ifndef _FILELIST_H_
#define _FILELIST_H_

#include <InterfaceKit.h>


class CFileList : public BListView
{
	public:
		CFileList(BRect frame);
		~CFileList();
		void SetBackground();
		virtual void MessageReceived(BMessage *message);
		virtual void  KeyDown(const char *bytes, int32 numBytes);
		void DeleteCurrentPicture(); 
};

#endif
