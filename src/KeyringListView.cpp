/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Menu.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Window.h>

#include <iostream>

#include "KeyringListView.h"

KeyringListView::KeyringListView() : BOutlineListView("keyringList"){
	popupMenu = new BPopUpMenu("keyringPopupMenu", false, false, B_ITEMS_IN_ROW);
	BMenuItem* item = new BMenuItem("test", new BMessage('NULL'));
	popupMenu->AddItem(item);
}

KeyringListView::~KeyringListView(){
	delete popupMenu;
}

void KeyringListView::MessageReceived(BMessage* msg){
	switch(msg->what){
		default:
			msg->PrintToStream();
			BOutlineListView::MessageReceived(msg);
			break;
	}
}

void KeyringListView::MouseDown(BPoint point){
	int32 modifiers = 0;
	uint32 buttons = 0;
	BMessage* currentMessage = Window()->CurrentMessage();
	if(currentMessage != NULL){
		currentMessage->FindInt32("modifiers", &modifiers);
		currentMessage->FindInt32("buttons", (int32*)&buttons);
	}
	if(buttons == B_SECONDARY_MOUSE_BUTTON){
		ConvertToScreen(&point);
		popupMenu->Go(point, true, false, true);
	}
}

void KeyringListView::KeyDown(const char* bytes, int32 numBytes){
	if(numBytes == 1){
		switch(bytes[0]){
			case B_DELETE:
				std::cout << "KeyringListView: delete pressed" << std::endl;
				break;
			default:
				BOutlineListView::KeyDown(bytes, numBytes);
				break;
		}
	} else {
		BOutlineListView::KeyDown(bytes, numBytes);
	}
}
