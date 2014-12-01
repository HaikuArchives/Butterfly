#ifndef _INTERVIEW_H_
#define _INTERVIEW_H_

#include <View.h>
#include <Bitmap.h>
#include <ScrollBar.h>
#include <Looper.h>
#include <MenuItem.h>


class FakeScrollBar : public BScrollBar
{
	public:
		FakeScrollBar(BRect frame, BView *target, orientation posture);
		~FakeScrollBar() {};

		virtual void ValueChanged(float newValue);
	
	private:
		orientation post;
};


class InterView : public BView
{
	public:
		InterView(BRect frame, const char *name, BLooper *msgDest);
		~InterView();
		
		virtual void Draw(BRect updateRect);
		virtual void MouseDown(BPoint point);
		virtual void MouseUp(BPoint point);
		virtual void MouseMoved(BPoint point, uint32 transit, const BMessage *message);
		void TomFrameResized();
		void SetBitmap(BBitmap *b);
		void SetAutoScale(bool state);
		void SetMaxVisibleArea();

		BBitmap *GetBitmap() {return curPicture;}
		void ScrollRequested(int dir);
		void SetOffset(float newValue, orientation posture);

		void ZoomIn();
		void ZoomOut();
		void ResetZoom();	// reset zoom factor to 1. (100%)
		void SetPanMode(int32 mode) {panMode = mode;}
	private:
		void UpdateScrollBar();
		void SendZoomFactorModified();
		void SendZoomFactorModified(float fact);

	private:
		BBitmap		*curPicture;
		float		picW, picH;

		bool		mouseDown;	// used for paning
		BPoint		mouseCoord;
		BPopUpMenu	*menu;
		
		int32		panMode;
		bool		autoScale;
		float		maxVisibleWidth;
		float		maxVisibleHeight;
		float 		xOffs, yOffs;
		float		zoomFactor;
		FakeScrollBar	*vScroll, *hScroll;

		BLooper		*msgDest;
};


#endif

