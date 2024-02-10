/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Menu.h>
#include <Window.h>

#include <iostream>

#include "KeyListView.h"

KeyListView::KeyListView() :BColumnListView("keyList", B_WILL_DRAW, B_FANCY_BORDER){
	popupMenu = new BPopUpMenu("keyListPopupMenu", false, false, B_ITEMS_IN_ROW);
	BMenuItem* item = new BMenuItem("test", new BMessage('NULL'));
	popupMenu->AddItem(item);
}

KeyListView::~KeyListView(){
}

void KeyListView::MessageReceived(BMessage* msg){
	switch(msg->what){
		default:
			msg->PrintToStream();
			BColumnListView::MessageReceived(msg);
			break;
	}
}

void KeyListView::KeyDown(const char* bytes, int32 numBytes){
	if(numBytes == 1){
		switch(bytes[0]){
			case B_DELETE:
				std::cout << "KeyListView: delete pressed" << std::endl;
				break;
			default:
				BColumnListView::KeyDown(bytes, numBytes);
				break;
		}
	} else {
		BColumnListView:KeyDown(bytes, numBytes);
	}
}

void KeyListView::MouseDown(BPoint where){
	int32 modifiers = 0;
	uint32 buttons = 0;
	BMessage* currentMessage = Window()->CurrentMessage();
	if(currentMessage != NULL){
		currentMessage->FindInt32("modifiers", &modifiers);
		currentMessage->FindInt32("buttons", (int32*)&buttons);
	}
	if(buttons == B_SECONDARY_MOUSE_BUTTON){
		ConvertToScreen(&where);
		popupMenu->Go(where, true, false, true);
	}
}
