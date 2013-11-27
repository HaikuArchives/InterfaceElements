// IEObjectRoster class
// (c) 1997 by Attila Mezei
// This is a central object handler, takes care of loading add-ons, editing and updating objects
// There is a global object roster declared as: 
//	extern IEObjectRoster *ie_object_roster;
// You need to initialize the object roster in the application constructor:
//	ie_object_roster=new IEObjectRoster();
// If you write an add-on, you can count on that ie_object_roster is already initialized 


#ifndef _IEOBJECTROSTER_H_
#define _IEOBJECTROSTER_H_

#if !defined(_IMPEXP_OHLIB)
#define _IMPEXP_OHLIB __declspec(dllimport)
#endif

#include <SupportKit.h>
#include <StorageKit.h>
#include <KernelKit.h>


class _IMPEXP_OHLIB IEEditableObject;
class _IMPEXP_OHLIB IEObjectEditorWindow;


// info about an add-on
struct AddOnInfo
{
	// general add-on properties
	char *filename;
	char *fullpath;
	image_id imageid;
	entry_ref ref;
	bool loaded;

	// IE specific
	IEEditableObject *(*unflatten)(const void *buffer, ssize_t buffersize);
	IEEditableObject *default_object;	// caching for speed
	char *classname;					// caching for speed
	int32 typecode;						// caching for speed
};



class _IMPEXP_OHLIB IEObjectRoster
{
friend class IEEditableObject;
friend class IEObjectEditor;
friend class IEObjectEditorWindow;

public:
	IEObjectRoster(entry_ref *add_ons_directory=NULL);	// queries the addons, set up a list of the found addons
	virtual ~IEObjectRoster(void);								// unloads the addons
	
	void Initialize(entry_ref *add_ons_directory=NULL);	// loads the add-ons
	
	BList *AddOnList(void);								// list of AddOnInfos
	AddOnInfo *AddOnAt(int32 index);					// same as (AddOnInfo*)AddOnList()->ItemAt(index)
	void InsertAddOn(AddOnInfo *ai);					// inserts an add-on in the list (usable with e.g. internal object types)
	
	AddOnInfo *FindClass(char *byname);					// finds an add-on by class name
	AddOnInfo *FindClass(BArchivable *archivable);		// finds an add-on by object pointer
	
	IEObjectEditorWindow *OpenObject(IEEditableObject *object, char *title=NULL, BWindow *parent=NULL);		// opens an editor window for the object
	void CloseObject(IEEditableObject *object);			// same as delete object;
	
	entry_ref *AddOnsDirectoryRef();					// ref of add-ons directory

	BList *editable_list;								// list of EditableObjects
	BList *GetWindowMenuItems(uint32 command='Wind');	// returns list of BMenuItems

	bool	Lock();
	void	Unlock();

protected:
	void ObjectChanged(IEEditableObject *oldtarget, IEEditableObject *newtarget);
	void ObjectDeleted(IEEditableObject *object);
	void WindowClosed(BWindow *a_window);
	
private:
	virtual void  _Reserved1(); 
	virtual void  _Reserved2(); 
	virtual void  _Reserved3(); 
	virtual void  _Reserved4(); 

	void ScanDirectoryForAddOns(BDirectory *a_dir);

	BLocker lock;
	
	BList *editor_list;
	BList *addon_list;

	char *duplicate_addons;
	entry_ref addonsdirref;
	BPoint default_window_position;

	uint32	_reserved[6];

};


_IMPEXP_OHLIB extern IEObjectRoster *ie_object_roster;


#endif