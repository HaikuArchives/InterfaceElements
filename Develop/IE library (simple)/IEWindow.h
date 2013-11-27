// Interface Elements header: IEWindow, simplified
// Last modified 31 Aug 1997
// (c) 1997 Attila Mezei


#ifndef _IEWINDOW_H_
#define _IEWINDOW_H_

#include "InterfaceKit.h"



class IEWindow : public BWindow
{
public:
	IEWindow(char *windowname, BResources *from_resources=NULL);
	virtual ~IEWindow(void);

	virtual	void Quit(void);
			
	BMessage *FindWindowResource(char *windowname, BResources *from_resources=NULL);
	void AddDependantWindow(BWindow *window);

protected:
	BList *dependancy_list;		// of BMessengers of dependant windows (that will be closed when this window is closed)

private:
	char *name;					// window name
};


#endif