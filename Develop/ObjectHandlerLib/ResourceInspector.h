
#ifndef _RESOURCEINSPECTOR_H_
#define _RESOURCEINSPECTOR_H_

#if !defined(_IMPEXP_OHLIB)
#define _IMPEXP_OHLIB __declspec(dllimport)
#endif

#include "IEObjectEditor.h"

class _IMPEXP_OHLIB EditableResource;


class _IMPEXP_OHLIB ResourceInspector : public IEObjectEditor
{
public:
	ResourceInspector(EditableResource *editableresource);

	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage *msg);
	
	virtual void TargetObjectChanged(IEEditableObject *new_target);
	virtual void UpdateObject();						// from the editor views. You should call ChangeTargetObject() in the end of the overridden method
	
	BTextControl *fStringObj;
	BTextControl *fIDObj;
	BTextControl *fTypeObj;
	BPopUpMenu *typemenu;
};


#endif