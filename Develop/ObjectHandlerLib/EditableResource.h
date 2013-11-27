// EditableResource class


#ifndef _EDITABLERESOURCE_H_
#define _EDITABLERESOURCE_H_

#if !defined(_IMPEXP_OHLIB)
#define _IMPEXP_OHLIB __declspec(dllimport)
#endif

#include <string.h>

#include "IEEditableData.h"


struct ResourceDescriptor;




class _IMPEXP_OHLIB EditableResource : public IEEditableData
{
public:
	EditableResource(ResourceDescriptor *rscdsc, BLooper *messagetarget);
	~EditableResource();

	static IEEditableObject		*UnflattenObject(const void *buf, ssize_t size);
	virtual IEEditableObject	*Unflatten(const void *buf, ssize_t size);
	IEObjectEditor *GetEditor(IEEditableObject *object_to_inspect);

	virtual void DataChanged();
	
	char *RSCName();
	int32 RSCID();
	int32 RSCType();
	void RSCSetName(char *name);
	void RSCSetID(int32 id);
	void RSCSetType(int32 type);
	
	
	ResourceDescriptor *rscdescriptor;
	BMessenger targetmessenger;
	BLooper *messagetarget;
};


struct ResourceDescriptor
{
	ResourceDescriptor(char *rname, long rtype, long rID, void *rdata, long rdatalength){ 
						strcpy(name, rname); 
						type=rtype; 
						ID=rID;
						data=new char[rdatalength];
						memcpy(data, rdata, rdatalength);
						datalength=rdatalength;
						editableresource=NULL;
						editormessenger=NULL;
					};
					
	~ResourceDescriptor(){
						if(editableresource) delete editableresource;
					};

	char name[64];
	ulong type;
	long ID;
	
	void *data;
	long datalength;
	
	EditableResource *editableresource;
	BMessenger *editormessenger;
};



#endif
