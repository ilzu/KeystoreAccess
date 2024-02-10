/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <LayoutBuilder.h>
#include <MenuBar.h>
#include <Menu.h>
#include <ScrollView.h>
#include <ColumnTypes.h>
#include <KeyStore.h>
#include <Catalog.h>

#include <iostream>

#include "KAMainWindow.h"
#include "KAApplication.h"
#include "KAKeyWindow.h"
#include "KeyringListView.h"
#include "KeyListView.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"

KAMainWindow::KAMainWindow(BRect frame) :BWindow(frame, "KeyStore Access", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS){
	BMenuBar* menuBar = new BMenuBar("menuBar");
	keyringList = new KeyringListView();
	keyringList->SetSelectionMessage(new BMessage(KEY_RING_LIST_ITEM_CHANGED));
	BScrollView* keyringScroll = new BScrollView("keyringScroll", keyringList, false, true, B_FANCY_BORDER);
	keyList = new KeyListView();
	keyList->SetSelectionMessage(new BMessage(KEY_LIST_ITEM_CHANGED));
	keyList->SetInvocationMessage(new BMessage(KEY_LIST_INVOCATE));
	keyStore = new BKeyStore;

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0, 0, 0, 0)
		.Add(menuBar)
		.AddSplit(B_HORIZONTAL, 0)
			.SetInsets(-1)
			.GetSplitView(&split)
			.Add(keyringScroll, 0.25f)
			.Add(keyList, 0.75f)
		.End()
	.End();

	if(frame.top == 0 && frame.left == 0) CenterOnScreen();

	uint32 cookie = 0;
	status_t status = B_OK;

	while(status == B_OK){
		BString keyRing;
		status = keyStore->GetNextKeyring(cookie, keyRing);
		BStringItem *item = new BStringItem(keyRing);
		keyringList->AddItem(item);
	}
	keyringList->Select(0);
	int32 index = keyringList->CurrentSelection();
	BString keyRing(((BStringItem*)keyringList->ItemAt(index))->Text());

	int em = keyList->StringWidth("M");
	keyList->AddColumn(new BStringColumn(B_TRANSLATE("Key"), 20 * em, 10 * em, 50 * em, 0), 0);
	keyList->AddColumn(new BStringColumn(B_TRANSLATE("Key type"), 20 * em, 10 * em, 50* em, 0), 1);

	keyringList->SetTarget(this);
	keyList->SetTarget(this);

	UpdateKeyList(keyRing);
	keyList->MakeFocus(true);
}

KAMainWindow::~KAMainWindow(){
}

void KAMainWindow::MessageReceived(BMessage* msg){
	switch(msg->what){
		case KEY_RING_LIST_ITEM_CHANGED:
			{
				int32 index;
				status_t status;
				status = msg->FindInt32("index", &index);
				if(status != B_OK) break;
				BString keyring(((BStringItem*)keyringList->ItemAt(index))->Text());
				UpdateKeyList(keyring);
			}
			break;
		case KEY_LIST_INVOCATE:
			{
				status_t status;
				int32 index;
				BKey* key;
				BRect keyWinRect(this->Frame().left, this->Frame().top, 100, 100);
				keyWinRect.OffsetBy(32, 32);
				BRow* focusRow = keyList->FocusRow();
				BStringField* nameField = (BStringField*)focusRow->GetField(0);
				BString name(nameField->String());
				index = keyringList->CurrentSelection();
				BString keyring = ((BStringItem*)keyringList->ItemAt(index))->Text();
				BPasswordKey pwkey;
				status = keyStore->GetKey(keyring, B_KEY_TYPE_PASSWORD, name, pwkey);
				if(status != B_OK) break;
				key = &pwkey;
				KAKeyWindow* keyWindow = new KAKeyWindow(keyWinRect, name, key);
				keyWindow->Show();
			}
			break;
		default:
			msg->PrintToStream();
			BWindow::MessageReceived(msg);
			break;
	}
}

bool KAMainWindow::QuitRequested(){
	((KAApplication*)be_app)->SetWindowRect(this->Frame());
	((KAApplication*)be_app)->RemoveWindow(this);
	return true;
}

void KAMainWindow::UpdateKeyList(const char* keyring){
	uint32 cookie = 0;
	status_t status = B_OK;
	keyList->Clear();
	while(true){
		BPasswordKey key;
		status = keyStore->GetNextKey(keyring, B_KEY_TYPE_PASSWORD, B_KEY_PURPOSE_ANY, cookie, key);
		if(status == B_ENTRY_NOT_FOUND){
			break;
		}
		if(status != B_OK){
			std::cerr << "Error getting key from keystore server:" << strerror(status) << std::endl;
			return;
		}

		BRow* row = new BRow();
		BStringField* keyField = new BStringField(key.Identifier());
		row->SetField(keyField, 0);
		BStringField* typeField;
		switch(key.Type()){
			case BKeyType::B_KEY_TYPE_GENERIC:
				typeField = new BStringField(B_TRANSLATE("Generic"));
				break;
			case BKeyType::B_KEY_TYPE_PASSWORD:
				typeField = new BStringField(B_TRANSLATE("Password"));
				break;
			case BKeyType::B_KEY_TYPE_CERTIFICATE:
				typeField = new BStringField(B_TRANSLATE("Certificate"));
				break;
			default:
				typeField = new BStringField(B_TRANSLATE("Generic"));
				break;
		}
		row->SetField(typeField, 1);
		keyList->AddRow(row);
	}

}