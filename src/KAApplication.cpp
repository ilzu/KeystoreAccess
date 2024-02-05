/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <File.h>
#include <Path.h>
#include <FindDirectory.h>
#include <Catalog.h>

#include "KAApplication.h"
#include "KAMainWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Application"

KAApplication::KAApplication() : BApplication("application/x-vnd.ilzu-KeyStoreAccess"){
	LoadSettings();
}

KAApplication::~KAApplication(){
	SaveSettings();
}

void KAApplication::ReadyToRun(){
	AddWindow();
}

void KAApplication::MessageReceived(BMessage* msg){
	msg->PrintToStream();
	switch(msg->what){
		default:
			BApplication::MessageReceived(msg);
			break;
	}
}

bool KAApplication::QuitRequested(){
	return windowList.CountItems() == 0;
}

KAMainWindow* KAApplication::AddWindow(){
	status_t status;
	BRect windowRect;
	status = settings->FindRect("windowRect", &windowRect);
	if(status != B_OK){
		windowRect.Set(0, 0, 200, 100);
	}
	KAMainWindow* mainWindow = new KAMainWindow(windowRect);
	windowList.AddItem((void*)mainWindow);
	mainWindow->Show();
	return mainWindow;
}

void KAApplication::RemoveWindow(KAMainWindow* window){
	if(windowList.HasItem(window)){
		windowList.RemoveItem(window);
	}
	if(windowList.CountItems() == 0){
		PostMessage(B_QUIT_REQUESTED);
	}
}

void KAApplication::SetWindowRect(BRect rect){
	status_t status;
	BRect temp;
	status = settings->FindRect("windowRect", &temp);
	if(status == B_OK){
		status = settings->ReplaceRect("windowRect", rect);
	} else {
		status = settings->AddRect("windowRect", rect);
	}
	SaveSettings();
}

status_t KAApplication::LoadSettings(){
	status_t status;
	BPath settingsPath;
	settings = new BMessage();
	status = find_directory(B_USER_SETTINGS_DIRECTORY, &settingsPath);
	if(status != B_OK) return status;
	status = settingsPath.Append("KeyStoreAccess");;
	if(status != B_OK) return status;
	BFile settingsFile(settingsPath.Path(), B_READ_ONLY);
	status = settingsFile.InitCheck();
	if(status != B_OK) return status;
	status = settings->Unflatten(&settingsFile);
	settingsFile.Unset();
	return status;
}

status_t KAApplication::SaveSettings(){
	status_t status;
	BPath settingsPath;
	status = find_directory(B_USER_SETTINGS_DIRECTORY, &settingsPath);
	if(status != B_OK) return status;
	status = settingsPath.Append("KeyStoreAccess");;
	if(status != B_OK) return status;
	BFile settingsFile(settingsPath.Path(), B_WRITE_ONLY | B_CREATE_FILE);
	status = settingsFile.InitCheck();
	if(status != B_OK) return status;
	status = settings->Flatten(&settingsFile);
	settingsFile.Unset();
	return status;
}

