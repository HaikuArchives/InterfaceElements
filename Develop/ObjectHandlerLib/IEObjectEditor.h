// IEObjectEditor class
// (c) 1997 by Attila Mezei
// Base class for generic object editors


#ifndef _IEOBJECTEDITOR_H_
#define _IEOBJECTEDITOR_H_

#if !defined(_IMPEXP_OHLIB)
#define _IMPEXP_OHLIB __declspec(dllimport)
#endif

#include <SupportKit.h>
#include <View.h>


class _IMPEXP_OHLIB IEEditableObject;

// control the editor's initial appearance, use with MakeCollapsed()
const int32 IE_COLLAPSE_NO_CHANGE=0L;
const int32 IE_COLLAPSE=1L;
const int32 IE_EXPAND=2L;


class _IMPEXP_OHLIB IEObjectEditor : public BView
{
public:
	IEObjectEditor(BRect frame, const char* name, IEEditableObject *target);
	virtual ~IEObjectEditor(void);

	virtual void AttachedToWindow();
	
	void ChangeTargetObject(IEEditableObject *new_target=NULL);		// call this when you modify the object. Notification will be sent out to other editors...
	virtual void TargetObjectChanged(IEEditableObject *new_target);	// hook function to take the appropriate actions (updating the editor) when the object changes

	virtual void UpdateObject();						// from the editor views. You should call ChangeTargetObject() in the end of the overridden method
	
	void SelectView(BView *a_bview);					// select a view in the view editor
	void EditView(BView *a_bview);						// finds suitable editors
	void RemoveView(BView *a_bview);					// delete view in the view editor
	
	void EditObject(IEEditableObject *an_object);		// same as ie_object_roster->OpenObject(an_object,...) but with the editor window as the parent window
	void RemoveObject(IEEditableObject *an_object);		// same as 'delete an_object'
			
	void MakeCollapsed(int32 collapsemode);				// this controls whether the editor appear as collapsed. Use only in the constructor, later it has no effect.
	int32 IsCollapsed();

	IEEditableObject *target;							// each editor has a target editable object

private:
	virtual void  _Reserved1(); 
	virtual void  _Reserved2(); 
	
	int32 iscollapsed;

	uint32	_reserved[4];

};

#endif
