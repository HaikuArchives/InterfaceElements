// BControlEditor class
// written by Attila Mezei
// Last modified: 31 Aug 1997


#include <stdio.h>
#include <stdlib.h>
#include "BControlEditor.h"
#include "IEBControl.h"
#include "MessageSelectorBar.h"




BControlEditor::BControlEditor(IEEditableObject *object)
	: IEObjectEditor(BRect(0.0, 0.0, VIEW_WIDTH, VIEW_HEIGHT), "BControl", object)
{

	labelObj=new BTextControl(BRect(15, 8, VIEW_WIDTH-20, 28), "label", "Label:", "", new BMessage('labl') );
	labelObj->SetDivider(StringWidth("Label:")+8);
	AddChild(labelObj);
	
	messageselector=new MessageSelectorBar(BRect(15, 33, VIEW_WIDTH-20, 53), "msgsel", NULL, 0);
	AddChild(messageselector);

	valueObj=new BTextControl(BRect(15, 60, 120, 80), "value", "Value:", "" , new BMessage('valu') );
	valueObj->SetDivider(StringWidth("Value:")+8);
	AddChild(valueObj);
	
	enabledObj=new BCheckBox(BRect(200, 60, 260, 80), "enabled", "Enabled" , new BMessage('enab') );
	AddChild(enabledObj);
		
	TargetObjectChanged(object);
}



BControlEditor::~BControlEditor(void)
{
}



void BControlEditor::AttachedToWindow()
{
	IEObjectEditor::AttachedToWindow();
	
	labelObj->SetTarget(this);
	messageselector->SetTarget(this);
	valueObj->SetTarget(this);
	enabledObj->SetTarget(this);
}



void BControlEditor::TargetObjectChanged(IEEditableObject *new_target)
{
	if(cast_as(new_target, BControl) ){
		BControl *bcontrol=cast_as(new_target, BControl);

		if(bcontrol->Window()) bcontrol->Window()->Lock();

		target=new_target;
	
		// initialize only if changed
		// name
		if(bcontrol->Label()){
			if(strcmp(labelObj->Text(), bcontrol->Label())) labelObj->SetText(bcontrol->Label());
		}else{
			labelObj->SetText("");
		}
		
		// value
		char txt[64];
		sprintf(txt, "%lu", bcontrol->Value());
		if(strcmp(valueObj->Text(), txt)) valueObj->SetText(txt);
		
		// enabled
		enabledObj->SetValue(bcontrol->IsEnabled());

		// initialize message selector
		if(cast_as(bcontrol, IEEditableView)){
			int32 uqid=cast_as(bcontrol, IEEditableView)->GetUniqueID();
			messageselector->SetMessageTo(bcontrol->Message(), uqid );
		}else{
			messageselector->SetMessageTo(bcontrol->Message(), 0 );
		}


		if(bcontrol->Window()) bcontrol->Window()->Unlock();
	}
}



void BControlEditor::MessageReceived(BMessage *msg)
{	
	BMessage *new_message;
	BControl *view=cast_as(target, BControl);
	
	switch(msg->what){

		case 'labl':		if(view){
								if(view->Window()) if(view->Window()->LockWithTimeout(1000000.0) != B_OK) break;

								view->SetLabel(labelObj->Text());
								view->ResizeToPreferred();
								view->Invalidate();
								
								ChangeTargetObject();
								if(view->Window()) view->Window()->Unlock();
							}							
							break;

								
		case 'Mesg':		if(view){
								if(view->Window()) if(view->Window()->LockWithTimeout(1000000.0) != B_OK) break;
							
								if(msg->FindPointer("BMessage", (void**)&new_message)==B_OK){
									if(new_message){
										view->SetMessage(new BMessage(new_message));
									}else{
										view->SetMessage(NULL);
									}
								}
								ChangeTargetObject();
								if(view->Window()) view->Window()->Unlock();
							}							
							break;
					
					
		case 'valu':		if(view){
								if(view->Window()) if(view->Window()->LockWithTimeout(1000000.0) != B_OK) break;

								view->SetValue(atol(valueObj->Text()));

								ChangeTargetObject();
								if(view->Window()) view->Window()->Unlock();
							}							
							break;
							
							
		case 'enab':		if(view){
								if(view->Window()) if(view->Window()->LockWithTimeout(1000000.0) != B_OK) break;

								view->SetEnabled(enabledObj->Value()!=B_CONTROL_OFF);

								ChangeTargetObject();
								if(view->Window()) view->Window()->Unlock();
							}							
							break;
		
							
		default:
							IEObjectEditor::MessageReceived(msg);
							break;
	}
	
}

