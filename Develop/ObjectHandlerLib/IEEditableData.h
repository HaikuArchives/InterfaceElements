// IEEditableData class
// (c) 1997 by Attila Mezei
// This class maintains a memory chunk of a given address and size
// The memory can be external but IEEditableData can reserve and handle its own memory chunk


#ifndef _IEEDITABLEDATA_H_
#define _IEEDITABLEDATA_H_

#if !defined(_IMPEXP_OHLIB)
#define _IMPEXP_OHLIB __declspec(dllimport)
#endif

// it is an editable object!
#include "IEEditableObject.h"



class _IMPEXP_OHLIB IEEditableData : public IEEditableObject
{
public:
	IEEditableData(void **dataptr=NULL, size_t *sizeptr=NULL);		// if memory is already reserved, provide the pointers to the address and size
	virtual ~IEEditableData(void);

	// must implement these because it is an editable object!
	virtual	ssize_t		FlattenedSize();
	virtual	status_t	Flatten(void *buffer, ssize_t size);
	
	virtual void DataChanged(void);									// hook function is called when data changes
	
	void NewData(size_t newsize, bool copy_old=false);				// reserve new data area
	
	size_t DataSize();												// return data size
	void *Data();													// return data pointer

	bool	Lock();
	void	Unlock();

protected:
	BLocker lock;
	
	void **dataptrptr;
	size_t *sizeptr;
	
	void *data;
	size_t size;

private:
	virtual void  _Reserved1(); 
	virtual void  _Reserved2(); 

	uint32	_reserved[4];

};

#endif