/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef KEYRING_LIST_VIEW_H
#define KEYRING_LIST_VIEW_H


#include <OutlineListView.h>

class BPopUpMenu;

class KeyringListView :public BOutlineListView{
	public:
		KeyringListView();
		~KeyringListView();
		void MessageReceived(BMessage* msg);
		void MouseDown(BPoint point);
		void KeyDown(const char* bytes, int32 numBytes);
	private:
		BPopUpMenu* popupMenu;

};

#endif // KEYRING_LIST_VIEW_H
