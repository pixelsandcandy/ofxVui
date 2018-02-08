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
        ToggleGroup(){};
        ~ToggleGroup(){};
        
        ToggleGroup( int x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
        }
        
        ToggleGroup( int x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
        }
        
        ToggleGroup( string x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
        }
        
        ToggleGroup( string x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
        }
        
        ofEvent<vuiEventArgs> onToggleChange;
        
        void AddToggle(Element* toggle, bool setSelected = false ){
            if ( setSelected ) toggle->SetSelected();
            
            AddChild( toggle );
            toggle->MakeToggle();
            toggles.push_back( toggle );
            ofAddListener( toggle->onValueChange, this, &ToggleGroup::_vuiEventHandler );
            ofAddListener( toggle->onMouseClick, this, &ToggleGroup::_vuiEventHandler );
        }
        
    private:
        
        vector<Element*> toggles;
        
        void _vuiEventHandler(vuiEventArgs& evt){
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
