// message selector view
// (c) 1997 Attila Mezei

#include <InterfaceKit.h>

class MessageSelectorBar : public BView, public BInvoker
{
public:
	MessageSelectorBar(BRect frame, const char *name, BMessage *init=NULL, uint32 a_unique_ID=0L, char *text="Send Message:", uint32 command='Mesg', bool additional_messages=false);
	~MessageSelectorBar();
	
	virtual void AttachedToWindow(void);
	virtual void MessageReceived(BMessage *mess);
	
	void SetMessageTo(BMessage *newmessage, uint32 a_unique_ID=-1L);
	BMessage *Message(void);
	
	char *CommandToText(uint32 comm);
	uint32 TextToCommand(char *command_text);
	bool IsValidChar(char c);
	
	void SetEnabled(bool flag);
	
private:
	uint32 post_command;
	uint32 unique_ID;
	uint32 user_command;

	BMessage	*message;
	
	BMenuField *messagemenu;
	BPopUpMenu *messagepopup;
	BTextControl *messageID;
	
	char text_buffer[32];
	bool enablesendmessage;

};
