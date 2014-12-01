#include <ScrollView.h>
#include <Message.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <Menu.h>
#include <Application.h>

#include "InterView.h"
#include "InterApp.h"
#include "Constants.h"
#include "StringsRsrc.h"

#include <stdio.h>		// DEBUG


//=========================================================================
//	PictureScrollBar class
//
//=========================================================================

FakeScrollBar::FakeScrollBar(BRect frame, BView *target, orientation posture)
			: BScrollBar(frame, "", target, 0.0, 0.0, posture)
{
	post = posture;
}


void FakeScrollBar::ValueChanged(float newValue)
{
	((InterView*)Parent())->SetOffset(newValue, post);
}


//=========================================================================
//	InterView class
//
//=========================================================================

InterView::InterView(BRect frame, const char *name, BLooper *msgDest)
	: BView (frame, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	BRect r=Bounds();
	vScroll = new FakeScrollBar(BRect(r.right - B_V_SCROLL_BAR_WIDTH,
										r.top, r.right, r.bottom-B_H_SCROLL_BAR_HEIGHT),
										this, B_VERTICAL);
	AddChild(vScroll);
	hScroll = new FakeScrollBar(BRect(r.left, r.bottom - B_H_SCROLL_BAR_HEIGHT,
										r.right - B_V_SCROLL_BAR_WIDTH, r.bottom),
										this, B_HORIZONTAL);
	AddChild(hScroll);

	xOffs = 0.;
	yOffs = 0.;

	panMode = PIC_FOLLOW_MOUSE;
	this->msgDest = msgDest;
	zoomFactor = 1.;
	autoScale = false;
	curPicture = NULL;
	mouseDown = false;

	SetViewColor(B_TRANSPARENT_COLOR);

	// Create the popup menu
	menu = new BPopUpMenu("thepopup");
	BMenu *submenu1 = new BMenu(StringsRsrc[STR_SET_CURRENT_BACKGRND]);
	BMenu *submenu2 = new BMenu(StringsRsrc[STR_SET_ALL_BACKGRND]);
	BMessage *msg;
	BMenuItem *item;

	msg = new BMessage(MSG_SAVE_REQUESTED);
	item = new BMenuItem(StringsRsrc[STR_CONVERT], msg);
	menu->AddItem(item);
	
	menu->AddSeparatorItem();

	msg = new BMessage(MSG_SET_CURDESKTOP_CENTERED);
	item = new BMenuItem(StringsRsrc[STR_CENTERED], msg);
	submenu1->AddItem(item);
	msg = new BMessage(MSG_SET_CURDESKTOP_SCALED);
	item = new BMenuItem(StringsRsrc[STR_SCALED], msg);
	submenu1->AddItem(item);
	msg = new BMessage(MSG_SET_CURDESKTOP_TILED);
	item = new BMenuItem(StringsRsrc[STR_TILED], msg);
	submenu1->AddItem(item);
	menu->AddItem(submenu1);

	msg = new BMessage(MSG_SET_ALLDESKTOP_CENTERED);
	item = new BMenuItem(StringsRsrc[STR_CENTERED], msg);
	submenu2->AddItem(item);
	msg = new BMessage(MSG_SET_ALLDESKTOP_SCALED);
	item = new BMenuItem(StringsRsrc[STR_SCALED], msg);
	submenu2->AddItem(item);
	msg = new BMessage(MSG_SET_ALLDESKTOP_TILED);
	item = new BMenuItem(StringsRsrc[STR_TILED], msg);
	submenu2->AddItem(item);
	menu->AddItem(submenu2);

	menu->AddSeparatorItem();
	
	msg = new BMessage(MSG_ZOOM_IN); 
	item = new BMenuItem(StringsRsrc[STR_ZOOM_IN], msg); //"Zoom In                         +", msg);
	menu->AddItem(item);

	msg = new BMessage(MSG_ZOOM_OUT);
	item = new BMenuItem(StringsRsrc[STR_ZOOM_OUT], msg); //"Zoom Out                      -", msg);
	menu->AddItem(item);

	menu->AddSeparatorItem();

	msg = new BMessage(MSG_PREF_WINDOW);
	item = new BMenuItem(StringsRsrc[STR_PREFERENCES], msg); //, 'p');
	menu->AddItem(item);
	
}


//--------------------------------------------------------------------
InterView::~InterView()
{
	if (curPicture != NULL)
		delete curPicture;
}

//--------------------------------------------------------------------
void InterView::MouseDown(BPoint point)
{
	BMenuItem *selected;
	BPoint cursor;
	uint32 buttons;

	GetMouse(&cursor, &buttons, false);

	if (buttons & B_PRIMARY_MOUSE_BUTTON)
	{
		mouseDown = true;
		mouseCoord = point;
		SetMouseEventMask(B_POINTER_EVENTS,B_LOCK_WINDOW_FOCUS);
//		be_app->SetCursor(B_I_BEAM_CURSOR);
	}

	if (buttons & B_SECONDARY_MOUSE_BUTTON)
	{
		ConvertToScreen(&point);
		selected = menu->Go(point);
		if ( selected && (selected->Message() != NULL))
			if (selected->Message()->what == MSG_ZOOM_IN)
				ZoomIn();
			else if (selected->Message()->what == MSG_ZOOM_OUT)
				ZoomOut();
			else
				msgDest->PostMessage(selected->Message());
 	}

	if (buttons & B_TERTIARY_MOUSE_BUTTON)
	{
		((InterApp*)be_app)->theWin->SwapFullScreenNoMenu();
	}
}


//--------------------------------------------------------------------
void InterView::MouseMoved(BPoint point, uint32 transit, const BMessage *message)
{
	if (mouseDown)
	{
		if (panMode == PIC_FOLLOW_MOUSE) {
			hScroll->SetValue(hScroll->Value()-(point.x - mouseCoord.x));
			vScroll->SetValue(vScroll->Value()-(point.y - mouseCoord.y));
		}
		else {
			hScroll->SetValue(hScroll->Value()-(mouseCoord.x - point.x));
			vScroll->SetValue(vScroll->Value()-(mouseCoord.y - point.y));
		}
		mouseCoord = point;
	}
}


//--------------------------------------------------------------------
void InterView::MouseUp(BPoint point)
{
	mouseDown = false;
//	be_app->SetCursor(B_HAND_CURSOR);
}


//--------------------------------------------------------------------
void InterView::Draw(BRect updateRect)
{
	BRect visi(Bounds());

	if (curPicture != NULL)
	{
		BRect picRect(curPicture->Bounds());
		BRect viewRect(visi);

		if (autoScale)
		{
			viewRect.right -= B_V_SCROLL_BAR_WIDTH;
			viewRect.bottom -= B_H_SCROLL_BAR_HEIGHT;
			float dx = picRect.right / viewRect.right;
			float dy = picRect.bottom / viewRect.bottom;
			if (dx > dy)
			{
				float old_height = viewRect.bottom;
				viewRect.bottom = (picRect.bottom / picRect.right) * viewRect.right;

				viewRect.OffsetBy(0., old_height/2 - viewRect.bottom/2);

				FillRect(BRect(0., 0., viewRect.right, viewRect.top-1));
				FillRect(BRect(0., viewRect.bottom, viewRect.right, visi.bottom));

				zoomFactor = 1./dx;
				SendZoomFactorModified();
			}
			else
			{
				float old_width = viewRect.right;
				viewRect.right = (picRect.right / picRect.bottom) * viewRect.bottom;

				viewRect.OffsetBy(old_width/2 - viewRect.right/2, 0.);

				FillRect(BRect(0., viewRect.top, viewRect.left-1, viewRect.bottom));
				FillRect(BRect(viewRect.right+1, viewRect.top, visi.right, viewRect.bottom));

				zoomFactor = 1./dy;
				SendZoomFactorModified();
			}

			DrawBitmap(curPicture, viewRect);
		}
		else
		{
			if (zoomFactor == 1.)
			{
				BPoint pos(viewRect.right/2 - picRect.right/2 - B_V_SCROLL_BAR_WIDTH/2,
						   viewRect.bottom/2 - picRect.bottom/2 - B_H_SCROLL_BAR_HEIGHT/2);
				if (pos.x < 0.)
					pos.x = -xOffs;
				else xOffs = 0.;
//					pos.x = 0;
				if (pos.y < 0.)
					pos.y = -yOffs;
				else yOffs = 0.;
//					pos.y = 0;
			
				DrawBitmap(curPicture, pos);

				FillRect(BRect(0., 0., visi.right, pos.y-1));
				FillRect(BRect(0., visi.bottom - pos.y - B_H_SCROLL_BAR_HEIGHT + 1, visi.right, visi.bottom));
				FillRect(BRect(0., pos.y, pos.x-1, visi.bottom-pos.y));
				FillRect(BRect(visi.right-pos.x-B_V_SCROLL_BAR_WIDTH+1, pos.y, visi.right, visi.bottom-pos.y));
			}
			else
			{
				picRect.right  *= zoomFactor;
				picRect.bottom *= zoomFactor;

				BRect srcR(curPicture->Bounds());

				BRect dstR(viewRect.right/2 - picRect.right/2 - B_V_SCROLL_BAR_WIDTH/2,
						   viewRect.bottom/2 - picRect.bottom/2 - B_H_SCROLL_BAR_HEIGHT/2,
						   viewRect.right/2 + picRect.right/2 + B_V_SCROLL_BAR_WIDTH/2,
						   viewRect.bottom/2 + picRect.bottom/2 + B_H_SCROLL_BAR_HEIGHT/2);

				if (dstR.left < 0.)	// pic wider than view
				{
					dstR.left  = 0.;
					dstR.right = visi.right - B_V_SCROLL_BAR_WIDTH;

					srcR.left  = picRect.left + xOffs/zoomFactor;
					srcR.right = picRect.left + xOffs/zoomFactor + (visi.right - B_V_SCROLL_BAR_WIDTH)/zoomFactor;
				}
				else xOffs = 0.;

				if (dstR.top < 0.) // pic higher than view
				{
					dstR.top = 0.;
					dstR.bottom = visi.bottom - B_H_SCROLL_BAR_HEIGHT;

					srcR.top = picRect.top + yOffs/zoomFactor;
					srcR.bottom = picRect.top + yOffs/zoomFactor + (visi.bottom - B_H_SCROLL_BAR_HEIGHT)/zoomFactor;
				}
				else yOffs = 0.;

				DrawBitmap(curPicture, srcR, dstR);

				FillRect(BRect(0., 0., visi.right, dstR.top-1));					// top
				FillRect(BRect(0., dstR.bottom+1, visi.right, visi.bottom));		// bottom
				FillRect(BRect(0., dstR.top, dstR.left-1, dstR.bottom));			// left
				FillRect(BRect(dstR.right+1, dstR.top, visi.right, dstR.bottom));	// right
			}

		}
	}
	else
		FillRect(Bounds());
}


//--------------------------------------------------------------------
void InterView::SetBitmap(BBitmap *b)
{
	if (b != NULL)
	{
		if (curPicture != NULL)
			delete curPicture;
		curPicture = b;
		
		BRect r = b->Bounds();
		
		picW = r.right;
		picH = r.bottom;
		zoomFactor = 1.;
		xOffs = 0.;
		yOffs = 0.;

		r = Bounds();
		//TomFrameResized(r.right, r.bottom);
		TomFrameResized();
		SendZoomFactorModified();
	}
}


//--------------------------------------------------------------------
void InterView::UpdateScrollBar()
{
	BRect r;
	if (curPicture != NULL)
	{
		r = (curPicture->Bounds());
	}

		if (curPicture == NULL || r.right*zoomFactor < maxVisibleWidth || autoScale)
		{
			hScroll->SetRange(0., 0.);
		}
		else
		{
			hScroll->SetRange(0., r.right*zoomFactor - maxVisibleWidth);
			hScroll->SetProportion(maxVisibleWidth/(r.right*zoomFactor));
		}

		if (curPicture == NULL || r.bottom*zoomFactor < maxVisibleHeight || autoScale)
		{
			vScroll->SetRange(0., 0.);
		}
		else
		{
			vScroll->SetRange(0., r.bottom*zoomFactor - maxVisibleHeight);
			vScroll->SetProportion(maxVisibleHeight/(r.bottom*zoomFactor));
		}
		hScroll->SetValue(xOffs);
		vScroll->SetValue(yOffs);
}


//--------------------------------------------------------------------
void InterView::TomFrameResized()
{
	SetMaxVisibleArea();
	UpdateScrollBar();
	Invalidate();
}


//--------------------------------------------------------------------
void InterView::SetAutoScale(bool state)
{
	if (autoScale != state)
	{
		autoScale = state;
		zoomFactor = 1.;
		UpdateScrollBar();
		Invalidate();
		SendZoomFactorModified();
	}
}


//--------------------------------------------------------------------
void InterView::SetMaxVisibleArea()
{
	maxVisibleWidth = Bounds().right - B_V_SCROLL_BAR_WIDTH - 1;
	maxVisibleHeight = Bounds().bottom - B_H_SCROLL_BAR_HEIGHT -1;	
}


//--------------------------------------------------------------------
void InterView::ScrollRequested(int dir)
{
	switch (dir)
	{
		case SCROLL_LEFT:
				hScroll->SetValue(hScroll->Value()-10);
				break;

		case SCROLL_RIGHT:
				hScroll->SetValue(hScroll->Value()+10);
				break;

		case SCROLL_UP:
				vScroll->SetValue(vScroll->Value()-10);
				break;

		case SCROLL_DOWN:
				vScroll->SetValue(vScroll->Value()+10);
				break;
	}
}


//--------------------------------------------------------------------
void InterView::SetOffset(float newValue, orientation posture)
{
	if (posture == B_HORIZONTAL)
	{
		xOffs = newValue;
		if (xOffs > picW*zoomFactor-maxVisibleWidth)
			xOffs = picW*zoomFactor-maxVisibleWidth;
	}
	else
	{
		yOffs = newValue;
		if (yOffs > picH*zoomFactor-maxVisibleHeight)
			yOffs = picH*zoomFactor-maxVisibleHeight;
	}
	Invalidate();
}


//--------------------------------------------------------------------
void InterView::ZoomIn()
{
	if (autoScale) // autoScale
	{
		((InterApp *)be_app)->theWin->theCmdView->CheckAutoFit(FALSE);
		float save = zoomFactor;
		SetAutoScale(FALSE);
		if (save>1) {
			zoomFactor = (int)save;
		}
		else {
			zoomFactor = 1.;
			while (zoomFactor>save)
				zoomFactor /= 2.;
		}		
	}

	if (!autoScale)
	{
		if (zoomFactor > 1.)
		{
			xOffs = (xOffs/zoomFactor) * (zoomFactor+1) + ((maxVisibleWidth/(zoomFactor) - maxVisibleWidth/(zoomFactor+1.))*zoomFactor) / 2;
			yOffs = (yOffs/zoomFactor) * (zoomFactor+1) + ((maxVisibleHeight/(zoomFactor) - maxVisibleHeight/(zoomFactor+1.))*zoomFactor) / 2;
			zoomFactor++;
		}
		else
		{
			xOffs = (xOffs * 2) + ((maxVisibleWidth/(zoomFactor) - maxVisibleWidth/(zoomFactor*2.))*zoomFactor) / 2;
			yOffs = (yOffs * 2) + ((maxVisibleHeight/(zoomFactor) - maxVisibleHeight/(zoomFactor*2.))*zoomFactor) / 2;
			zoomFactor *= 2;
		}
		UpdateScrollBar();
		Invalidate();
	
		SendZoomFactorModified();
	}
/*	else // autoScale
	{
		((InterApp *)be_app)->theWin->theCmdView->CheckAutoFit(FALSE);
		SetAutoScale(FALSE);
		
	}*/
}


//--------------------------------------------------------------------
void InterView::ZoomOut()
{
	if (autoScale) // autoScale
	{
		((InterApp *)be_app)->theWin->theCmdView->CheckAutoFit(FALSE);
		float save = zoomFactor;
		SetAutoScale(FALSE);
		if (save>1) {
			zoomFactor = (int)save+1;
		}
		else {
			zoomFactor = 1.;
			while (zoomFactor>save)
				zoomFactor /= 2.;
			zoomFactor *= 2;
		}
	}

	if (!autoScale)
	{
		if (zoomFactor > 1.)
		{
			xOffs = (xOffs/zoomFactor) * (zoomFactor-1) - ((maxVisibleWidth/(zoomFactor-1.) - maxVisibleWidth/(zoomFactor))*zoomFactor) / 2;
			yOffs = (yOffs/zoomFactor) * (zoomFactor-1) - ((maxVisibleHeight/(zoomFactor-1.) - maxVisibleHeight/(zoomFactor))*zoomFactor) / 2;
			zoomFactor--;
		}
		else
		{
			xOffs = (xOffs/2) - ((maxVisibleWidth/(zoomFactor/2.) - maxVisibleWidth/(zoomFactor))*zoomFactor) / 2;
			yOffs = (yOffs/2) - ((maxVisibleHeight/(zoomFactor/2.) - maxVisibleHeight/(zoomFactor))*zoomFactor) / 2;
			zoomFactor /= 2;
		}
		UpdateScrollBar();
		Invalidate();

		SendZoomFactorModified();
	}
/*	else // autoScale
	{
		((InterApp *)be_app)->theWin->theCmdView->CheckAutoFit(FALSE);
		SetAutoScale(FALSE);			
	}*/
}


//--------------------------------------------------------------------
void InterView::ResetZoom()
{
	zoomFactor = 1.;
	xOffs = 0;
	yOffs = 0;
	UpdateScrollBar();
	Invalidate();
	SendZoomFactorModified();
}


//--------------------------------------------------------------------
void InterView::SendZoomFactorModified()
{
	BMessage *msg = new BMessage(MSG_ZOOM_FACTOR_MODIFIED);
	char tab[6];
	sprintf (tab, "%d%%", (int)(zoomFactor*100.));
	msg->AddString("factor", tab);
	msgDest->PostMessage(msg);
}


//--------------------------------------------------------------------
void InterView::SendZoomFactorModified(float fact)
{
	BMessage *msg = new BMessage(MSG_ZOOM_FACTOR_MODIFIED);
	char tab[6];
	sprintf (tab, "%d%%", (int)(fact*100.));
	msg->AddString("factor", tab);
	msgDest->PostMessage(msg);
}
