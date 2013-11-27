// BControlEditor class
// written by Attila Mezei
// Last modified: 31 Aug 1997


#include "IEBControl.h"
#include "IEObjectEditor.h"

// the editor is actually a BView
#define VIEW_WIDTH 300
#define VIEW_HEIGHT 90


class MessageSelectorBar;



class BControlEditor : public IEObjectEditor
{
public:
	BControlEditor(IEEditableObject *object);
	~BControlEditor(void);

	virtual void AttachedToWindow();
	virtual void TargetObjectChanged(IEEditableObject *new_target);
	
	virtual void MessageReceived(BMessage *msg);

private:	
	MessageSelectorBar *messageselector;
	BTextControl *labelObj;
	BTextControl *valueObj;
	BCheckBox *enabledObj;
};

