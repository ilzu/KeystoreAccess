/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef KA_KEY_WINDOW_H
#define KA_KEY_WINDOW_H


#include <Window.h>

class BKey;
class BTextControl;
class BButton;

class KAKeyWindow : public BWindow{
	public:
		KAKeyWindow(BRect frame, const char* title, BKey* key);
		~KAKeyWindow();
		void MessageReceived(BMessage* msg);
	private:
		BTextControl* identifier;
		BTextControl* secondaryIdentifier;
		BTextControl* password;
		BTextControl* creationTime;
		BButton* showPwButton;
		BButton* makeEditable;
		BKey* key;
		enum{
			SHOW_PASSWORD = 'shpw',
			MAKE_EDITABLE = 'mked',
		};

};

#endif // KA_KEY_WINDOW_H
