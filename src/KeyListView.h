/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _H
#define _H


#include <ColumnListView.h>

class BPopUpMenu;

class KeyListView : public BColumnListView{
	public:
		KeyListView();
		~KeyListView();
		void MessageReceived(BMessage* msg);
		void KeyDown(const char* bytes, int32 numBytes);
		void MouseDown(BPoint where);
	private:
		BPopUpMenu* popupMenu;
};

#endif // _H
