//
//  xTextField.h
//
//  Created by Christopher Miles on 9/27/17.
//
//

#pragma once

#ifndef __ofxVui_TextField__
#define __ofxVui_TextField__

#include "ofMain.h"

namespace VUI {
    
    
    class TextField : public Text
    {
        friend Text;
        
    public:
        
        ~TextField(){};
        TextField( const int x = 0, const int y = 0, StyleSheet *ss = nullptr, const string selector = "", const string selectorB = "" ):Text(x,y,ss,selector,selectorB){
            
            MakeToggle();
            isTextField = true;
            
            ofAddListener( ofEvents().keyPressed, this, &TextField::keyPressed );
            ofAddListener( onStateChange, this, &TextField::_vuiEventHandler );
        };
        
        ofEvent<vuiEventArgs> onSubmit;
        
        void TriggerSubmit(){
            SetState(VUI_STATE_UP);
            
            vuiEventArgs args;
            args.element = this;
            args.eventType = VUI_EVENT_SUBMIT;
            args.text = text;
            
            ofNotifyEvent(onSubmit, args, this);
        }
        
        int textLimit = -1;
        
        void SetLimit( int limit ){
            textLimit = limit;
        }
        
        void AddText( string t){
            if ( textLimit != -1 ) {
                if ( text.size() >= textLimit ) return;
            }
            
            ofRectangle rect = font->getStringBoundingBox(text + t , 0,0);
            rect.width += 2;
            
            if ( t == " " ) rect.width += font->getSize() * .85;
            if ( rect.width > width - padding.x*2 ) return;
            
            text = text + t;
            if ( t == " " ) spaceOffsetX = font->getSize() * .5;
            else spaceOffsetX = 0;
        }
        
        void Backspace(){
            text = text.substr(0, text.size()-1);
            if ( text.size() > 0 && text.substr(text.size()-1,1) == " " ) spaceOffsetX = font->getSize() * .5;
            else spaceOffsetX = 0;
        }
        
        void AddText( char c ){
            AddText( ofToString(c) );
        }
        
        void keyPressed(ofKeyEventArgs &key ){
            if ( GetVirtualState() != VUI_STATE_DOWN ) return;
            //ofLog() << key.key;
            
            switch(key.key){
                case OF_KEY_BACKSPACE:
                case OF_KEY_DEL:
                    Backspace();
                    return;
                    break;
                case OF_KEY_RETURN:
                    TriggerSubmit();
                    return;
                    break;
                case OF_KEY_ESC:
                case OF_KEY_TAB:
                case OF_KEY_COMMAND:
                case OF_KEY_F1:
                case OF_KEY_F2:
                case OF_KEY_F3:
                case OF_KEY_F4:
                case OF_KEY_F5:
                case OF_KEY_F6:
                case OF_KEY_F7:
                case OF_KEY_F8:
                case OF_KEY_F9:
                case OF_KEY_F10:
                case OF_KEY_F11:
                case OF_KEY_F12:
                case OF_KEY_LEFT:
                case OF_KEY_UP:
                case OF_KEY_RIGHT:
                case OF_KEY_DOWN:
                case OF_KEY_PAGE_UP:
                case OF_KEY_PAGE_DOWN:
                case OF_KEY_HOME:
                case OF_KEY_END:
                case OF_KEY_INSERT:
                case OF_KEY_CONTROL:
                case OF_KEY_ALT:
                case OF_KEY_SHIFT:
                case OF_KEY_LEFT_SHIFT:
                case OF_KEY_RIGHT_SHIFT:
                case OF_KEY_LEFT_CONTROL:
                case OF_KEY_RIGHT_CONTROL:
                case OF_KEY_LEFT_ALT:
                case OF_KEY_RIGHT_ALT:
                case OF_KEY_LEFT_SUPER:
                case OF_KEY_RIGHT_SUPER:
                    return;
                    break;
            }
            
            AddText((char)key.key);
        }
        
        bool IsFocused(){
            if ( GetVirtualState() == VUI_STATE_DOWN ) return true;
            return false;
        }
        
        
    private:
        void _vuiEventHandler(vuiEventArgs& evt){
            if ( evt.eventType == VUI_EVENT_STATE_CHANGE ){
                //ofLog() << evt.virtualState << " - " << evt.element->GetVirtualState();
                if ( evt.virtualState == VUI_STATE_DOWN ) {
                    TriggerEvent( VUI_EVENT_FOCUS );
                } else if ( evt.virtualState == VUI_STATE_OVER || evt.virtualState == VUI_STATE_UP ){
                    TriggerEvent( VUI_EVENT_UNFOCUS );
                }
            }
        }
    };
    
}


#endif
