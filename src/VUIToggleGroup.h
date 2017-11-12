//
//  xText.h
//
//  Created by Christopher Miles on 9/27/17.
//
//

#pragma once

#ifndef __ofxVui_ToggleGroup__
#define __ofxVui_ToggleGroup__

#include "ofMain.h"

namespace VUI {
    
    
    class ToggleGroup : public Element
    {
    public:
        ~ToggleGroup(){}
        ToggleGroup( const int x = 0, const int y = 0, StyleSheet *ss = nullptr, const string primarySelector = "", const string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            
            
            
        }
        
        vector<Element*> toggles;
        
        ofEvent<vuiEventArgs> onToggleChange;
        
        void AddToggle(Element* toggle, bool setSelected = false ){
            if ( setSelected ) toggle->SetSelected();
            
            AddChild( toggle );
            toggle->MakeToggle();
            toggles.push_back( toggle );
            ofAddListener( toggle->onValueChange, this, &ToggleGroup::vuiEventHandler );
            ofAddListener( toggle->onMouseClick, this, &ToggleGroup::vuiEventHandler );
        }
        
        void vuiEventHandler(vuiEventArgs& evt){
            if ( evt.eventType == VUI_EVENT_VALUE_CHANGE ){
                if ( evt.value == 1 ){
                    for ( vector<Element*>::iterator it= toggles.begin(); it != toggles.end(); it++){
                        if ( (*it) != evt.element ) (*it)->SetState(VUI_STATE_UP);
                    }
                    
                    vuiEventArgs args;
                    args.eventType = VUI_EVENT_TOGGLE_CHANGE;
                    args.element = evt.element;
                    args.value = 1;
                    ofNotifyEvent( onToggleChange, args, this );
                }
                
            } else if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ){
                bool itemSelected = false;
                
                for ( vector<Element*>::iterator it= toggles.begin(); it != toggles.end(); it++){
                    if ( evt.element->GetVirtualState() == VUI_STATE_DOWN ) itemSelected = true;
                }
                
                if ( !itemSelected ){
                    vuiEventArgs args;
                    args.eventType = VUI_EVENT_TOGGLE_CHANGE;
                    args.element = evt.element;
                    args.value = 0;
                    ofNotifyEvent( onToggleChange, args, this );
                }
            }
        }
        
        
    };
    
}


#endif
