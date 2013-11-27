// message selector view
// (c) 1997 Attila Mezei
// this class has not many comments in it


#include <stdio.h>
#include <stdlib.h>
#include <Alert.h>
#include "MessageSelectorBar.h"



MessageSelectorBar::MessageSelectorBar(BRect frame, const char *name, BMessage *init, uint32 a_unique_ID, char *text, uint32 command, bool additional_messages)
	: BView(frame, name, B_FOLLOW_LEFT|B_FOLLOW_TOP, B_WILL_DRAW|B_NAVIGABLE),
	BInvoker(NULL, NULL)
{
	BMenuItem *mi;
	
	enablesendmessage=true;
	
	post_command=command;
	unique_ID=a_unique_ID;
	user_command='Comm';
	
	if(init){
		message=new BMessage(init);
	}else{
		message=NULL;
	}
	

	messagepopup=new BPopUpMenu("messagetype",TRUE,TRUE);
	
	mi=new BMenuItem("None", new BMessage('MTno') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message==NULL){
		mi->SetMarked(TRUE);
	}

	mi=new BMenuItem("Unique ID", new BMessage('MTuq') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what<0x10000000L){
		unique_ID=message->what;
		mi->SetMarked(TRUE);
	}

	mi=new BMenuItem("Enter Command:", new BMessage('MTen') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what>=0x10000000L){
		mi->SetMarked(TRUE);
	}

//	if(additional_messages || modifiers()&B_CONTROL_KEY){
	messagepopup->AddSeparatorItem();
	
	mi=new BMenuItem("B_QUIT_REQUESTED", new BMessage('MTqu') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_QUIT_REQUESTED){
		mi->SetMarked(TRUE);
	}
	mi=new BMenuItem("B_ABOUT_REQUESTED", new BMessage('MTab') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_ABOUT_REQUESTED){
		mi->SetMarked(TRUE);
	}
	mi=new BMenuItem("B_SAVE_REQUESTED", new BMessage('MTsr') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_SAVE_REQUESTED){
		mi->SetMarked(TRUE);
	}
	mi=new BMenuItem("B_MINIMIZE", new BMessage('MTmi') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_MINIMIZE){
		mi->SetMarked(TRUE);
	}
	mi=new BMenuItem("B_ZOOM", new BMessage('MTzo') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_ZOOM){
		mi->SetMarked(TRUE);
	}
	mi=new BMenuItem("B_CUT", new BMessage('MTcu') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_CUT){
		mi->SetMarked(TRUE);
	}
	mi=new BMenuItem("B_COPY", new BMessage('MTco') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_COPY){
		mi->SetMarked(TRUE);
	}
	mi=new BMenuItem("B_PASTE", new BMessage('MTpa') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_PASTE){
		mi->SetMarked(TRUE);
	}
	mi=new BMenuItem("B_SELECT_ALL", new BMessage('MTse') );
	mi->SetTarget(this);
	messagepopup->AddItem(mi);
	if(message && message->what==B_SELECT_ALL){
		mi->SetMarked(TRUE);
	}

	if(modifiers()&B_CONTROL_KEY){
	
		messagepopup->AddSeparatorItem();

		mi=new BMenuItem("AF_NEW", new BMessage('ANew') );
		mi->SetTarget(this);
		messagepopup->AddItem(mi);
		if(message && message->what=='ANew'){
			mi->SetMarked(TRUE);
		}
		mi=new BMenuItem("AF_OPEN", new BMessage('AOpe') );
		mi->SetTarget(this);
		messagepopup->AddItem(mi);
		if(message && message->what=='AOpe'){
			mi->SetMarked(TRUE);
		}
		mi=new BMenuItem("AF_SAVE", new BMessage('ASav') );
		mi->SetTarget(this);
		messagepopup->AddItem(mi);
		if(message && message->what=='ASav'){
			mi->SetMarked(TRUE);
		}
		mi=new BMenuItem("AF_SAVE_AS", new BMessage('ASvA') );
		mi->SetTarget(this);
		messagepopup->AddItem(mi);
		if(message && message->what=='ASvA'){
			mi->SetMarked(TRUE);
		}
		mi=new BMenuItem("AF_PREFERENCES", new BMessage('APre') );
		mi->SetTarget(this);
		messagepopup->AddItem(mi);
		if(message && message->what=='APre'){
			mi->SetMarked(TRUE);
		}
		mi=new BMenuItem("AF_QUIT", new BMessage('AQui') );
		mi->SetTarget(this);
		messagepopup->AddItem(mi);
		if(message && message->what=='AQui'){
			mi->SetMarked(TRUE);
		}
	}
	
	messagemenu=new BMenuField(BRect(0, 0, StringWidth(text)+127, 17), "messagemenu", text, messagepopup);
	messagemenu->SetDivider(StringWidth(text)+8);
	AddChild(messagemenu);

	messageID=new BTextControl(BRect(StringWidth(text)+127, 1, StringWidth(text)+178, 18), "messageID", NULL, "", new BMessage('MIDx'));
	AddChild(messageID);
	
	if(messagepopup->FindMarked()){
		if(messagepopup->IndexOf(messagepopup->FindMarked())==2){
			messageID->SetText(CommandToText(message->what));
		}else{
			messageID->Hide();
		}
	}else{
		messageID->Hide();
	}

	ResizeTo(Frame().Width(), messagemenu->Frame().Height()+4 );
	
}


MessageSelectorBar::~MessageSelectorBar()
{
}


void MessageSelectorBar::AttachedToWindow(void)
{
	if(Parent()) SetViewColor(Parent()->ViewColor());
	messagepopup->SetTargetForItems(this);
	messageID->SetTarget(this);
	
	if(Target()==NULL) if(Window()) SetTarget(Window());
}


void MessageSelectorBar::MessageReceived(BMessage *mess)
{
	bool msgselection=true;

	switch(mess->what){
		case 'MTno':	if(message) delete message;
						message=NULL;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTuq':	if(message==NULL) message=new BMessage();
						message->what=unique_ID;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTen':
						if(messageID->IsHidden()){
							messageID->Show();
							messageID->MakeFocus(true);
						}
						if(message==NULL) message=new BMessage();
						message->what=user_command;
						messageID->SetText(CommandToText(user_command));
						break;
						
		case 'MTqu':	if(message==NULL) message=new BMessage();
						message->what=B_QUIT_REQUESTED;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTab':	if(message==NULL) message=new BMessage();
						message->what=B_ABOUT_REQUESTED;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTsr':	if(message==NULL) message=new BMessage();
						message->what=B_SAVE_REQUESTED;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTmi':	if(message==NULL) message=new BMessage();
						message->what=B_MINIMIZE;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTzo':	if(message==NULL) message=new BMessage();
						message->what=B_ZOOM;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTcu':	if(message==NULL) message=new BMessage();
						message->what=B_CUT;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTco':	if(message==NULL) message=new BMessage();
						message->what=B_COPY;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTpa':	if(message==NULL) message=new BMessage();
						message->what=B_PASTE;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MTse':	if(message==NULL) message=new BMessage();
						message->what=B_SELECT_ALL;
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'ANew':	if(message==NULL) message=new BMessage();
						message->what='ANew';
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'AOpe':	if(message==NULL) message=new BMessage();
						message->what='AOpe';
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'ASav':	if(message==NULL) message=new BMessage();
						message->what='ASav';
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'ASvA':	if(message==NULL) message=new BMessage();
						message->what='ASvA';
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'APre':	if(message==NULL) message=new BMessage();
						message->what='APre';
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'AQui':	if(message==NULL) message=new BMessage();
						message->what='AQui';
						if(!messageID->IsHidden())	messageID->Hide();
						break;
						
		case 'MIDx':
						if(message==NULL) message=new BMessage();
						user_command=message->what=TextToCommand((char*)messageID->Text());
						break;
		default:	
						BView::MessageReceived(mess);
						msgselection=false;
						break;
	}

//	if(mess->what=='MTno' || mess->what=='MTuq' || mess->what=='MTqu'){
//		if(!messageID->IsHidden())	messageID->Hide();
//	}
		
	if(enablesendmessage){
		//if(mess->what=='MTno' || mess->what=='MTuq' || mess->what=='MTen' || mess->what=='MTqu' || mess->what=='MIDx'){
		if(msgselection){
			BMessage *msg=new BMessage(post_command);
			msg->AddPointer("BMessage", message);
			Invoke(msg);
		}
	}

}
	


void MessageSelectorBar::SetMessageTo(BMessage *newmessage, uint32 a_unique_ID)
{
	BMessage mess;
		
	if(a_unique_ID>=0) unique_ID=a_unique_ID;
		
	if(newmessage){
		//if(newmessage->what==B_QUIT_REQUESTED){		// B_QUIT_REQUESTED
		switch(newmessage->what){
			case B_QUIT_REQUESTED:
				if(messagepopup->ItemAt(4)){
					if(!messagepopup->ItemAt(4)->IsMarked()) messagepopup->ItemAt(4)->SetMarked(TRUE);
					mess=BMessage('MTqu');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case B_ABOUT_REQUESTED:
				if(messagepopup->ItemAt(5)){
					if(!messagepopup->ItemAt(5)->IsMarked()) messagepopup->ItemAt(5)->SetMarked(TRUE);
					mess=BMessage('MTab');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case B_SAVE_REQUESTED:
				if(messagepopup->ItemAt(6)){
					if(!messagepopup->ItemAt(6)->IsMarked()) messagepopup->ItemAt(6)->SetMarked(TRUE);
					mess=BMessage('MTsr');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case B_MINIMIZE:
				if(messagepopup->ItemAt(7)){
					if(!messagepopup->ItemAt(7)->IsMarked()) messagepopup->ItemAt(7)->SetMarked(TRUE);
					mess=BMessage('MTmi');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case B_ZOOM:
				if(messagepopup->ItemAt(8)){
					if(!messagepopup->ItemAt(8)->IsMarked()) messagepopup->ItemAt(8)->SetMarked(TRUE);
					mess=BMessage('MTzo');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case B_CUT:
				if(messagepopup->ItemAt(9)){
					if(!messagepopup->ItemAt(9)->IsMarked()) messagepopup->ItemAt(9)->SetMarked(TRUE);
					mess=BMessage('MTcu');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case B_COPY:
				if(messagepopup->ItemAt(10)){
					if(!messagepopup->ItemAt(10)->IsMarked()) messagepopup->ItemAt(10)->SetMarked(TRUE);
					mess=BMessage('MTco');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case B_PASTE:
				if(messagepopup->ItemAt(11)){
					if(!messagepopup->ItemAt(11)->IsMarked()) messagepopup->ItemAt(11)->SetMarked(TRUE);
					mess=BMessage('MTpa');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case B_SELECT_ALL:
				if(messagepopup->ItemAt(12)){
					if(!messagepopup->ItemAt(12)->IsMarked()) messagepopup->ItemAt(12)->SetMarked(TRUE);
					mess=BMessage('MTse');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case 'ANew':
				if(messagepopup->ItemAt(14)){
					if(!messagepopup->ItemAt(14)->IsMarked()) messagepopup->ItemAt(14)->SetMarked(TRUE);
					mess=BMessage('ANew');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case 'AOpe':
				if(messagepopup->ItemAt(15)){
					if(!messagepopup->ItemAt(15)->IsMarked()) messagepopup->ItemAt(15)->SetMarked(TRUE);
					mess=BMessage('AOpe');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case 'ASav':
				if(messagepopup->ItemAt(16)){
					if(!messagepopup->ItemAt(16)->IsMarked()) messagepopup->ItemAt(16)->SetMarked(TRUE);
					mess=BMessage('ASav');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case 'ASvA':
				if(messagepopup->ItemAt(17)){
					if(!messagepopup->ItemAt(17)->IsMarked()) messagepopup->ItemAt(17)->SetMarked(TRUE);
					mess=BMessage('ASvA');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case 'APre':
				if(messagepopup->ItemAt(18)){
					if(!messagepopup->ItemAt(18)->IsMarked()) messagepopup->ItemAt(18)->SetMarked(TRUE);
					mess=BMessage('APre');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
			case 'AQui':
				if(messagepopup->ItemAt(19)){
					if(!messagepopup->ItemAt(19)->IsMarked()) messagepopup->ItemAt(19)->SetMarked(TRUE);
					mess=BMessage('AQui');
					enablesendmessage=false;
					MessageReceived(&mess);
					enablesendmessage=true;
					return;
				}
				break;
		}
		
		if(newmessage->what<0x10000000L){		// unique
			unique_ID=newmessage->what;
			if(!messagepopup->ItemAt(1)->IsMarked()) messagepopup->ItemAt(1)->SetMarked(TRUE);
			mess=BMessage('MTuq');
			enablesendmessage=false;
			MessageReceived(&mess);
			enablesendmessage=true;
			return;
		}
		
		user_command=newmessage->what;
		
		if(!messagepopup->ItemAt(2)->IsMarked()) messagepopup->ItemAt(2)->SetMarked(TRUE);
		mess=BMessage('MTen');
		enablesendmessage=false;
		MessageReceived(&mess);
		enablesendmessage=true;
	}else{
		// select None
		if(!messagepopup->ItemAt(0)->IsMarked()) messagepopup->ItemAt(0)->SetMarked(true);
		mess=BMessage('MTno');
		enablesendmessage=false;
		MessageReceived(&mess);
		enablesendmessage=true;
	}
}


BMessage *MessageSelectorBar::Message(void)
{

	// update the edited field if applicable
	if(messagepopup->IndexOf(messagepopup->FindMarked())==2){
		if(message==NULL) message=new BMessage();
		message->what=TextToCommand((char*)messageID->Text());
	}
	
	return message;
}



uint32 MessageSelectorBar::TextToCommand(char *command_text)
{
	char c1,c2,c3,c4;
	uint32 type=atol(command_text);
	BAlert *alert;
	
	
	if(type==0 || command_text[0]=='\''){
		if(command_text[0]=='\''){
			c1=command_text[1];
			c2=command_text[2];
			c3=command_text[3];
			c4=command_text[4];
		}else{
			c1=command_text[0];
			c2=command_text[1];
			c3=command_text[2];
			c4=command_text[3];
		}

		type=(((long)c1)<<24) | (((long)c2)<<16) | (((long)c3)<<8) | c4;
		
		if(IsValidChar(c1) && IsValidChar(c2) && IsValidChar(c3) && IsValidChar(c4)){
		}else{
			alert=new BAlert("", "The message command is an unsigned long and must contain exactly four alphanumeric characters! Underscore is allowed. E.g. 'aB2_' \n","OK",NULL, NULL,B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->Go();
		}
	}
	
	return type;
}


char *MessageSelectorBar::CommandToText(uint32 comm)
{
	char c1,c2,c3,c4;
//	char *ptr=(char*)&comm;
	
	c1=(comm>>24)&255;
	c2=(comm>>16)&255;
	c3=(comm>>8)&255;
	c4=(comm)&255;
	
	if(IsValidChar(c1) && IsValidChar(c2) && IsValidChar(c3) && IsValidChar(c4) ){
		sprintf(text_buffer,"'%c%c%c%c'", c1,c2,c3,c4);
	}else{
		sprintf(text_buffer,"%ld", comm);
	}

	return text_buffer;
}


bool MessageSelectorBar::IsValidChar(char c)
{
	return (c>=32);
}


void MessageSelectorBar::SetEnabled(bool flag)
{
	messagemenu->SetEnabled(flag);
	messageID->SetEnabled(flag);
}

