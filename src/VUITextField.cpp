#include "ofxVui.h"
#include "VUITextField.h"

namespace VUI {
    void TextField::_vuiEventHandler(vuiEventArgs& evt){
        if ( evt.eventType == VUI_EVENT_VALUE_CHANGE ){
            if ( evt.value == 1 ) {
                VUI::ResetTextFields(this);
                
                TriggerEvent( VUI_EVENT_FOCUS );
                VUI::activeTextField = this;
                
                if ( text == placeholder ) SetText("");
            } else if ( evt.value == 0 ) {
                TriggerEvent( VUI_EVENT_UNFOCUS );
                if ( VUI::activeTextField == this ) VUI::activeTextField = NULL;
                if ( text == "" ) SetText(placeholder);
            }
        }
    }
    
    void TextField::_Setup(){
        MakeToggle();
        isTextField = true;
        
        VUI::textFields.push_back(this);
        
        ofAddListener( ofEvents().keyPressed, this, &TextField::keyPressed );
        ofAddListener( onValueChange, this, &TextField::_vuiEventHandler );
    }
}
