// class IEWindow, simplified
// Last modified 31 Aug 1997
// (c) 1997 Attila Mezei


#include "IEWindow.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Application.h"
#include "Resources.h"

// IEflags in the window archive; currently not put in the window archive
#define IEF_CENTRE 1



IEWindow::IEWindow(char *windowname, BResources *from_resources)
	: BWindow(FindWindowResource(windowname, from_resources) )
{
	
	dependancy_list=new BList;
	name=strdup(windowname);

	Run();
		
	// centre screen?
	BMessage *window_data=FindWindowResource(windowname);
	if(window_data->FindInt32("IEflags")&IEF_CENTRE){
		BRect screenRect = BScreen(this).Frame();
		MoveTo((screenRect.Width()-Frame().Width())/2,  (screenRect.Height()-Frame().Height())/2);
	}
	if(window_data->HasFloat("minWidth")){
		SetSizeLimits(window_data->FindFloat("minWidth"), window_data->FindFloat("maxWidth"), window_data->FindFloat("minHeight"), window_data->FindFloat("maxHeight") );
	}
	delete window_data;
		
}


IEWindow::~IEWindow(void)
{
	if(dependancy_list) delete dependancy_list;
	free(name);
}


// close all dependant windows
void IEWindow::Quit(void)
{
	int32 i;
	BMessenger *mess;
	BWindow *a_window;
		
	// close all dependant windows
	for(i=0;i<dependancy_list->CountItems();i++){
		mess=(BMessenger*)(dependancy_list->ItemAt(i));
		if(mess->IsValid()){
			mess->Target((BLooper**)&a_window);
			if(a_window){
				if(a_window->Lock()){
					a_window->Quit();
				}
			}
		}
	}

	// delete list items
	for(i=0;i<dependancy_list->CountItems();i++){
		mess=(BMessenger*)(dependancy_list->ItemAt(i));
		delete mess;
	}
	
	dependancy_list->MakeEmpty();

	BWindow::Quit();
}


// not a Be type code but I use this for archived objects
#define ARCHIVED_OBJECT_TYPE (B_ARCHIVED_OBJECT)

// finds a resource of type B_ARCHIVED_OBJECT
BMessage *IEWindow::FindWindowResource(char *windowname, BResources *from_resources)
{
	void *window_data=NULL;
	BMessage *window_message=new BMessage();
	
	if(from_resources==NULL){	// from application image
		app_info info;
		if(be_app->GetAppInfo(&info)==B_OK){
			BFile rscfile;
			BResources *rsc=new BResources;
			
			if(rscfile.SetTo(&info.ref, B_READ_ONLY)==B_OK && rsc->SetTo(&rscfile)==B_OK){
				if(rsc->HasResource(ARCHIVED_OBJECT_TYPE, windowname )){
					size_t datasize;
					window_data=rsc->FindResource(ARCHIVED_OBJECT_TYPE, windowname, &datasize);
				}
			}
		}
	}else{
		if(from_resources->HasResource(ARCHIVED_OBJECT_TYPE, windowname )){
			size_t datasize;
			window_data=from_resources->FindResource(ARCHIVED_OBJECT_TYPE, windowname, &datasize);
		}
	}
	
	
	if(window_data==NULL){	// not found
		// we should not return NULL here since this is passed to the BWindow constructor
		// construct a window archive with "Window not found in resources" title
		char *errortext=new char[strlen(windowname)+strlen("Window \"\" not found in resources!")+1];
		sprintf(errortext, "Window \"%s\" not found in resources!", windowname);
		BWindow *errorwindow=new BWindow(BRect(32,32,400,64), errortext, B_TITLED_WINDOW, 0L);
		
		errorwindow->Archive(window_message, true);
		
		delete errortext;
		delete errorwindow;

	}else{	// unflatten the data

		window_message->Unflatten((const char *)window_data);
	
	}


	return window_message;
}


// IEWindow maintains a window dependancy list about windows which it will close when Quit()ted
// you can add a window to this list
void IEWindow::AddDependantWindow(BWindow *window)
{
	if(Lock()){
		dependancy_list->AddItem(new BMessenger(NULL, window));
		Unlock();
	}
}


