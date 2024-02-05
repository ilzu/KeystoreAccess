/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef KA_APPLICATION_H
#define KA_APPLICATION_H


#include <Application.h>
#include <List.h>


class KAMainWindow;

class KAApplication :public BApplication{
	public:
		KAApplication();
		~KAApplication();
		void ReadyToRun();
		void MessageReceived(BMessage* msg);
		bool QuitRequested();
		KAMainWindow* AddWindow();
		void RemoveWindow(KAMainWindow* window);
		void SetWindowRect(BRect rect);

	private:
		status_t LoadSettings();
		status_t SaveSettings();
		BList windowList;
		BMessage* settings;


};

#endif // KA_APPLICATION_H
