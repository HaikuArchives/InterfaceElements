// BControl element and editor for Interface Elements
// written by Attila Mezei
// Last modified: 31 Aug 1997


#ifndef _IEBCONTROL_H_
#define _IEBCONTROL_H_


#include <InterfaceKit.h>
#include "ObjectHandlerLib.h"



class IEBControl : public BControl, public IEEditableView
{
public:
	IEBControl(BMessage *data);
	
	static	IEBControl*	Instantiate(BMessage *data);
	virtual	status_t Archive(BMessage *data, bool deep = true) const;

	static	IEEditableObject	*UnflattenObject(const void *buf, ssize_t size);
	virtual IEEditableObject	*Unflatten(const void *buf, ssize_t size);

	// inherited from IEEditableView
	virtual BView *InstantiateView(BMessage *archive);
	virtual IEObjectEditor *GetEditor(IEEditableObject *view_to_edit);
	virtual void GetDragRepresentation(BView **a_view, BBitmap **a_bitmap);

};



#endif