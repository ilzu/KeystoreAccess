/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Key.h>
#include <LayoutBuilder.h>
#include <TextControl.h>
#include <Button.h>

#include "KAKeyWindow.h"

KAKeyWindow::KAKeyWindow(BRect frame, const char* title, BKey* key) :BWindow(frame, title, B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS){
	BGroupView* layout = new BGroupView();
	switch(key->Type()){
		case B_KEY_TYPE_PASSWORD:
			{
				this->key = new BPasswordKey(*(BPasswordKey*)key);
				((BPasswordKey*)this->key)->SetPassword(((BPasswordKey*)key)->Password());
				identifier = new BTextControl("Identifier", ((BPasswordKey*)key)->Identifier(), NULL);
				identifier->SetEnabled(false);
				secondaryIdentifier = new BTextControl("Secondary identifier", ((BPasswordKey*)key)->SecondaryIdentifier(), NULL);
				secondaryIdentifier->SetEnabled(false);
				password = new BTextControl("Password", "", NULL);
				password->TextView()->HideTyping(true);
				BString pwasterisks;
				pwasterisks.SetToFormat("%*d", 12, 0);
				password->SetText(pwasterisks);
				password->SetEnabled(false);
				showPwButton = new BButton("Show", new BMessage(SHOW_PASSWORD));
				creationTime = new BTextControl("Creation time", "", NULL);
				creationTime->SetEnabled(false);
				BLayoutBuilder::Group<>(layout, B_VERTICAL)
					.SetInsets(B_USE_WINDOW_INSETS)
					.AddGroup(B_HORIZONTAL)
						.AddGroup(B_VERTICAL)
							.Add(identifier->CreateLabelLayoutItem())
							.Add(secondaryIdentifier->CreateLabelLayoutItem())
							.Add(password->CreateLabelLayoutItem())
							.Add(creationTime->CreateLabelLayoutItem())
						.End()
						.AddGroup(B_HORIZONTAL)
							.AddGroup(B_VERTICAL)
								.Add(identifier->CreateTextViewLayoutItem())
								.Add(secondaryIdentifier->CreateTextViewLayoutItem())
								.AddGroup(B_HORIZONTAL)
									.Add(password->CreateTextViewLayoutItem())
									.Add(showPwButton)
								.End()
								.Add(creationTime->CreateTextViewLayoutItem())
							.End()
						.End()
					.End()
				.End();
			}
			break;
		default:
			break;
	}

	BButton* edit = new BButton("Edit", new BMessage(MAKE_EDITABLE));
	BButton* cancel = new BButton("Cancel", new BMessage(B_CANCEL));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(layout)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(edit)
			.Add(cancel)
		.End()
	.End();

}

KAKeyWindow::~KAKeyWindow(){
}

void KAKeyWindow::MessageReceived(BMessage* msg){
	switch(msg->what){
		case B_CANCEL:
			this->Lock();
			this->Quit();
			break;
		case MAKE_EDITABLE:
			{
				bool enabled = identifier->IsEnabled();
				identifier->SetEnabled(!enabled);
				secondaryIdentifier->SetEnabled(!enabled);
				password->SetEnabled(!enabled);
			}
			break;
		case SHOW_PASSWORD:
			{
				bool hidden = password->TextView()->IsTypingHidden();
				if(hidden){
					password->TextView()->HideTyping(false);
					password->SetText(((BPasswordKey*)key)->Password());
					showPwButton->SetLabel("Hide");
				} else {
					password->TextView()->HideTyping(true);
					BString pwasterisks;
					pwasterisks.SetToFormat("%*d", 12, 0);
					password->SetText(pwasterisks);
					showPwButton->SetLabel("Show");
				}
			}
			break;
		default:
			msg->PrintToStream();
			BWindow::MessageReceived(msg);
			break;
	}
}