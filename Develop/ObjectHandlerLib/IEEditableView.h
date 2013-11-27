// IEEditableView class
// (c) 1997-98 by Attila Mezei
// Base class for views with editors and/or variable handling. 
// Inherit from this class with multiple inheritence.


#ifndef _IEEDITABLEVIEW_H_
#define _IEEDITABLEVIEW_H_

#if !defined(_IMPEXP_OHLIB)
#define _IMPEXP_OHLIB __declspec(dllimport)
#endif

// it is an editable object!
#include "IEEditableObject.h"
#include "IEVariableHandler.h"


// view attributes, you should add the combination of these to IEEditableObject::info in the constructor
// for details see the documentation
const int32 IEEV_FLAGS_NONE=					0x0000L;
const int32 IEEV_CANNOT_HAVE_CHILDREN=			0x0001L;
const int32 IEEV_NOT_RESIZABLE=					0x0002L;
const int32 IEEV_DO_NOT_BREAK_UP_CHILDREN=		0x0004L;
const int32 IEEV_DO_NOT_SHOW_IN_PALETTE=		0x0010L;
const int32 IEEV_FORCE_FULL_UPDATE_ON_RESIZE=	0x4000L;


#define ARCHIVED_OBJECT_TYPE (B_ARCHIVED_OBJECT)


class _IMPEXP_OHLIB IEEditableView : public IEEditableObject, public IEVariableHandler
{
public:
	IEEditableView(BMessage *data=NULL);
	virtual ~IEEditableView(void);

	void Archive(BMessage *data) const;		// archives the variable settings if they exist and valid

	// must implement these because it is an editable object!
	//virtual		ssize_t		FlattenedSize();
	//virtual		status_t	Flatten(void *buffer, ssize_t size);
	//virtual		IEEditableObject	*Unflatten(const void *buf, ssize_t size);
	//static		IEEditableObject	*UnflattenObject(const void *buf, ssize_t size);

	// there is a problem with the static BView::Instantiate method: e.g. IEBButton::Instantiate will not
	// be found by the editor because it cannot cast to IEBButton since it does not know about the IEBButton class...
	// (we put the string "BButton" to the archive so that the standard BButton is instantiated)
	virtual BView *InstantiateView(BMessage *archive)=0;

	// if the view is capable of handling a variable, then implement these IEVariableHandler methods
	//virtual void SetVariable(void *pointer, const char *type=NULL);	// leave type unchanged
	//virtual void *Variable(void);					// returns the variable pointer
	//virtual void UpdateView(void);					// updates the view according to the variable
	//virtual void UpdateVariable(void);				// updates the variable according to the view

	// this hook function is usually not needed (only by BScrollView)
	virtual bool ViewAdded(BView *new_child, BView **new_view=NULL);		// returns true if the view should be deleted, normally returns false
	virtual void ViewRemoved(void);					// notification when a child is deleted
	
	//void AdjustParentViewFrame(void);				// adjusts the size of the container view during editing
	int32 GetUniqueID(void);						// gets a window-wide unique ID for BMessages...

	int32 ownID;									// internal usage only
	
private:
	virtual void  _Reserved1(); 
	virtual void  _Reserved2(); 
	virtual void  _Reserved3(); 
	virtual void  _Reserved4(); 

	uint32	_reserved[4];

};

#endif