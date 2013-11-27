// IEEditableObject class
// (c) 1997 by Attila Mezei
// Base class for objects with editors for any class in the system
// Inherit from this class with multiple inheritence if possible. When this is not possible
// (e.g. the class cannot be overridden or not advised to override it) use a separate IEEditableObject
// which does not derive from the target class but maintain a target pointer to it


#ifndef _IEEDITABLEOBJECT_H_
#define _IEEDITABLEOBJECT_H_

#if !defined(_IMPEXP_OHLIB)
#define _IMPEXP_OHLIB __declspec(dllimport)
#endif

#include <SupportKit.h>

class _IMPEXP_OHLIB EditableResource;
class _IMPEXP_OHLIB IEObjectEditor;
class _IMPEXP_OHLIB IEEditableData;

class _IMPEXP_OHLIB IEEditableObject //: public BArchivable
{
friend class IEObjectEditor;
friend class IEObjectRoster;

public:
	IEEditableObject(void);
	virtual ~IEEditableObject(void);


	//Flattening/unflattening the object
	virtual	ssize_t		FlattenedSize();
	virtual	status_t	Flatten(void *buffer, ssize_t size);
	
	// an IEEditableObject should have a method which is similar to
	// Instantiate but instantiates from a byte stream not from a BMessage
	//static	IEEditableObject	*UnflattenObject(const void *buf, ssize_t size);
	
	// and a method which is not static in case we don't know the static
	// UnflattenObject, only an IEEditableObject pointer
	// For this function it is sufficient to return YourEditableObject::UnflattenObject(buf, size);
	virtual IEEditableObject	*Unflatten(const void *buf, ssize_t size)=0;
	
	// an IEEditableObject may export this static method which is called after the add-on is loaded
	// it is useful for reading resources from the add-on file
	//static	void	AddOnLoaded(entry_ref *ref);
	
	// object graphical representation when dragged; can be either a view or a bitmap
	virtual void GetDragRepresentation(BView **a_view, BBitmap **a_bitmap);

	// return an object editor instance, to edit "object_to_edit"
	virtual IEObjectEditor *GetEditor(IEEditableObject *object_to_edit);
	
	// create source from the object. Not applicable to all object kinds. Returns B_OK, or B_ERROR
	virtual BMessage *Generate(BMessage *info);
	
	// request the base classes
	BMessage *GetBaseClasses(void);			// B_STRING_TYPE "class"	
	
	// request add-on creation attributes
	BMessage *GetInfo(void);				// B_STRING_TYPE "Name", "Author", "Copyright", "Message"

	// the target for editing; normally itself but an external target can also be used
	void *Target();								// itself or another editable
	
	
	void AddToWindowList(BWindow *window);
	BMessage *WindowList(void);						// B_MESSENGER_TYPE "messenger"

	IEEditableData *EditableData();
	void SetEditableData(IEEditableData *edata);
	EditableResource *Resource();
	
	bool AutoDestruction();
	void SetAutoDestruction(bool destruct_when_last_editor_closes);
	void EditorClosed();

protected:	
	void *target;
	BMessage base_classes;							// B_STRING_TYPE "class"
	BMessage info;									// B_STRING_TYPE "Name", "Author", "Copyright", "Message", B_INT32_TYPE "TypeCode"
	BMessage window_list;							// B_MESSENGER_TYPE "messenger"

	IEEditableData *editabledata;	// IEEditableObject may contain a link to an editabledata
									// which it updates automatically when changed
									// Use it only when the binary representation is important
									// and handled separately

	bool autodestruction;			// if true the object will be deleted when the last editor view closes
	int32 ref_count;

private:
	virtual void  _Reserved1(); 
	virtual void  _Reserved2(); 
	virtual void  _Reserved3(); 
	virtual void  _Reserved4(); 

	uint32	_reserved[4];
};




#endif