// sample add-on for Interface Elements
// written by Attila Mezei
// long editor


#include <stdio.h>
#include <stdlib.h>
#include <Alert.h>
#include <TextControl.h>
#include "ObjectHandlerLib.h"


class EditableLong : public IEEditableObject
{
public:
	EditableLong(const void *buf);
	
	virtual	ssize_t		FlattenedSize();
	virtual	status_t	Flatten(void *buffer, ssize_t size);
	
	// an IEEditableObject should have this method which is similar to
	// Instantiate but instantiates from a byte stream not from a BMessage
	static	IEEditableObject	*UnflattenObject(const void *buf, ssize_t size);
	virtual IEEditableObject	*Unflatten(const void *buf, ssize_t size);
		
	virtual IEObjectEditor *GetEditor(IEEditableObject *object_to_inspect);
	
	int32 target_long;
};


#define VIEW_WIDTH 200
#define VIEW_HEIGHT 34

class LongEditor : public IEObjectEditor
{
public:
	LongEditor(EditableLong *target);

	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage *msg);
	
	virtual void TargetObjectChanged(IEEditableObject *new_target);
	virtual void UpdateObject();						// from the editor views. You should call ChangeTargetObject() in the end of the overridden method
	
	BTextControl *fStringObj;
};



extern "C" _EXPORT IEEditableObject	*UnflattenObject_EditableLong(const void *buf, ssize_t size);



IEEditableObject *UnflattenObject_EditableLong(const void *buf, ssize_t size)
{
	return EditableLong::UnflattenObject(buf, size);
};







EditableLong::EditableLong(const void *buf)
	: IEEditableObject()
{
	if(buf==NULL){		// create a default object
		target_long=0L;
	}else{
		target_long=*(int32*)buf;
	}
	
	base_classes.AddString("class", "long");		// not really a class but we handle like that
	
	info.AddString("Name", "Long Editor");
	info.AddString("Author", "Attila Mezei");
	info.AddString("Copyright", "(c) 1997-99 Attila Mezei");
	info.AddString("Message", "This is a simple editable long object.\n");
	info.AddInt32("TypeCode", B_INT32_TYPE);

}


ssize_t		EditableLong::FlattenedSize()
{
	return sizeof(int32);
}


status_t	EditableLong::Flatten(void *buffer, ssize_t size)
{
	*(int32*)buffer=target_long;
	return B_OK;
}


IEEditableObject	*EditableLong::Unflatten(const void *buf, ssize_t size)
{
	return EditableLong::UnflattenObject(buf, size);	// should call the static version
}
	
	
IEEditableObject	*EditableLong::UnflattenObject(const void *buf, ssize_t size)
{
	BAlert *alert;
	
	if(buf!=NULL && size!=sizeof(int32)){
		alert=new BAlert("Long Editor error", "Long Editor\n\nThe data does not seem to be of long type. If you click on \"OK\" the data size will be set to 4, therefore data will be lost! \n", "Cancel", "OK", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		if(alert->Go()==1){
			return new EditableLong(buf);
		}else{
			return NULL;
		}
	}else{
		return new EditableLong(buf);
	}
}
	


IEObjectEditor *EditableLong::GetEditor(IEEditableObject *object_to_inspect)
{
	
	if(cast_as(object_to_inspect, EditableLong)){
		return new LongEditor(cast_as(object_to_inspect, EditableLong));
	}else{
		beep();
		return NULL;
	}
	
}

// ------------------------------------------------------------------------------------------------
// THE EDITOR VIEW:

LongEditor::LongEditor(EditableLong *longobject)
	: IEObjectEditor(BRect(0.0, 0.0, VIEW_WIDTH, VIEW_HEIGHT), "Long", longobject)
{
	fStringObj=new BTextControl(BRect(15, 8, VIEW_WIDTH-20, 28), "long", "Long:", "" , new BMessage('stri') );
	fStringObj->SetDivider(StringWidth("Long:")+8);
	AddChild(fStringObj);
	
	TargetObjectChanged(longobject);
}


void LongEditor::AttachedToWindow()
{
	IEObjectEditor::AttachedToWindow();
	
	fStringObj->SetTarget(this);	
}


void LongEditor::TargetObjectChanged(IEEditableObject *new_target)
{
	EditableLong *new_long_target=cast_as(new_target, EditableLong);
	
	if(new_long_target){
		target=new_target;
		
		char txt[64];
		sprintf(txt, "%ld", new_long_target->target_long);
		if(Window()) Window()->Lock();
		fStringObj->SetText(txt);
		if(Window()) Window()->Unlock();
	}

}


void LongEditor::MessageReceived(BMessage *msg)
{
	EditableLong *the_target_long_editor=(EditableLong*)target;
	
	switch(msg->what){
		case 'stri':
							// update the "long object"
							the_target_long_editor->target_long=atol(fStringObj->Text());
							// send notification to other editors which edit the same long
							ChangeTargetObject(the_target_long_editor);	
							break;

		default:
							IEObjectEditor::MessageReceived(msg);
							break;							
	}	
}


void LongEditor::UpdateObject()						// from the editor views. You should call ChangeTargetObject() in the end of the overridden method
{
	EditableLong *the_target_long_editor=(EditableLong*)target;
	the_target_long_editor->target_long=atol(fStringObj->Text());
	ChangeTargetObject(the_target_long_editor);	
}
