// BControl element and editor for Interface Elements
// written by Attila Mezei
// Last modified: 31 Aug 1997


#include <Alert.h>
#include "IEBControl.h"
#include "BControlEditor.h"
#include "DragView.h"


extern "C" _EXPORT IEEditableObject	*UnflattenObject_IEBControl(const void *buf, ssize_t size);


IEEditableObject *UnflattenObject_IEBControl(const void *buf, ssize_t size)
{
	return IEBControl::UnflattenObject(buf, size);
};




IEBControl::IEBControl(BMessage *data)
	: BControl(data), IEEditableView()
{

	base_classes.MakeEmpty();
	base_classes.AddString("class", "BControl");
	base_classes.AddString("class", "BView");
		
	info.MakeEmpty();
	info.AddString("Name", "BControl");
	info.AddString("Author", "Attila Mezei");
	info.AddString("Copyright", B_UTF8_COPYRIGHT " 1997-98 Attila Mezei");
	info.AddString("Message", "This is an abstract class for control devices.\n");
	info.AddInt32("TypeCode", ARCHIVED_OBJECT_TYPE);
	info.AddInt32("ViewFlags", IEEV_CANNOT_HAVE_CHILDREN);

}
	
	
	
IEBControl *IEBControl::Instantiate(BMessage *archive)
{
	IEBControl *test;

	if(archive==NULL){
		BMessage defaultcontrolarchive;
		BControl *defaultcontrol=new BControl(BRect(0,0,66,24), "", "Control", NULL, B_FOLLOW_LEFT|B_FOLLOW_TOP, B_WILL_DRAW|B_NAVIGABLE );
		defaultcontrol->Archive(&defaultcontrolarchive);

		delete defaultcontrol;

		return new IEBControl(&defaultcontrolarchive); 
	}

	
	if ( validate_instantiation(archive, "BControl") ){
		test=new IEBControl(archive);
		
		return test; 
	}

	return NULL; 
}



status_t IEBControl::Archive(BMessage *data, bool deep) const
{
	BControl::Archive(data, deep);
	IEEditableView::Archive(data);

	// replace class with BControl !!!
	data->RemoveName("class");
	data->AddString("class", "BControl");
	
	return B_OK;
}



IEEditableObject	*IEBControl::UnflattenObject(const void *buf, ssize_t size)
{
	BAlert *alert;
	BMessage archive;
	
	if(buf!=NULL){
		archive.Unflatten((const char *)buf);
		if(!validate_instantiation(&archive, "BControl")){
			alert=new BAlert("BControl Editor error", "BControl Editor\n\nThe data is not a BControl archive. \n", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->Go();
			return NULL;
		}else{
			return IEBControl::Instantiate(&archive);
		}
	}else{
		return IEBControl::Instantiate(NULL);
	}
}



IEEditableObject	*IEBControl::Unflatten(const void *buf, ssize_t size)
{
	return IEBControl::UnflattenObject(buf, size);
}



BView *IEBControl::InstantiateView(BMessage *archive)
{
	return IEBControl::Instantiate(archive);
}




IEObjectEditor *IEBControl::GetEditor(IEEditableObject *view_to_inspect)
{
	BControl *a_bcontrol;
	
	a_bcontrol=cast_as(view_to_inspect, BControl);
	
	if(a_bcontrol==NULL){
		// not a BControl
		return NULL;
	}
		
	return new BControlEditor(view_to_inspect);
}



// we have to implement it because during dragging the BControl is a white rectangle
// show the string "BControl" over a blue background
void IEBControl::GetDragRepresentation(BView **a_view, BBitmap **a_bitmap)
{
	*a_bitmap=NULL;
	
	DragView *drag=new DragView((char*)GetBaseClasses()->FindString("class"));
	drag->ResizeTo(Frame().Width(), Frame().Height());
	*a_view=drag;
}



