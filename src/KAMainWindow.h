/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef KA_MAIN_WINDOW_H
#define KA_MAIN_WINDOW_H


#include <Window.h>

class BSplitView;
class BOutlineListView;
class BColumnListView;
class BKeyStore;

class KAMainWindow : public BWindow{
	public:
		KAMainWindow(BRect frame);
		~KAMainWindow();
		void MessageReceived(BMessage* msg);
		bool QuitRequested();
	private:
		void UpdateKeyList(const char* keyring = NULL);
		BSplitView* split;
		BColumnListView* keyList;
		BOutlineListView* keyringList;
		BKeyStore* keyStore;
		enum{
			KEY_RING_LIST_ITEM_CHANGED = 'krlc',
			KEY_LIST_ITEM_CHANGED = 'klch',
			KEY_LIST_INVOCATE = 'klic',
		};
};

#endif // KA_MAIN_WINDOW_H
