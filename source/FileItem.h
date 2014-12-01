#ifndef _FILEITEM_H_
#define _FILEITEM_H_

#include <ListItem.h>

#define IS_FILE	0
#define IS_DIR	1

class FileItem : public BListItem
{
	public:
		FileItem(char *s, int32 t = IS_FILE);
		~FileItem();
		virtual void DrawItem(BView *owner, BRect frame, bool complete = false);
		const char* getText();
		int32 getType();
	private:
		char*       name;
		int32		type;
};

#endif
