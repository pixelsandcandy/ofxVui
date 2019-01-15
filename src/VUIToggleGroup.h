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
            
            if ( shouldSelectOnHover ){
                ofAddListener( toggle->onMouseOver, this, &ToggleGroup::_vuiEventHandler );
            }
        }
        
        void SelectOnHover(){
            shouldSelectOnHover = true;
            for ( vector<Element*>::iterator it = toggles.begin(); it != toggles.end(); it++ ){
                ofAddListener( (*it)->onMouseOver, this, &ToggleGroup::_vuiEventHandler );
            }
        }
        
        void Reset(bool notifyEvent = true){
            for ( vector<Element*>::iterator it = toggles.begin(); it != toggles.end(); it++ ){
                (*it)->SetSelected(false, notifyEvent);
            }
        }
        
        void SelectToggleByIndex( int index ){
            int count = 0;
            for ( vector<Element*>::iterator it = toggles.begin(); it != toggles.end(); it++ ){
                if ( count == index ) (*it)->SetSelected(true);
                else (*it)->SetSelected(false);
                count++;
            }
        }
        
        Element* GetToggleByIndex( int index ) {
            return toggles[index];
        }
        
    private:
        
        bool shouldSelectOnHover = false;
        
        vector<Element*> toggles;
        
        void _vuiEventHandler(vuiEventArgs& evt){
            
            if ( shouldSelectOnHover && evt.eventType == VUI_EVENT_MOUSE_OVER ){
                if ( evt.element->GetVirtualState() != VUI_STATE_DOWN ){
                    for ( vector<Element*>::iterator it = toggles.begin(); it != toggles.end(); it++ ){
                        if ( (*it)->GetState() == VUI_STATE_DOWN ) {
                            (*it)->SetSelected(false);
                            evt.element->SetSelected(true);
                            
                            
                            return;
                        }
                    }
                }
                return;
            }
            
            if ( evt.eventType == VUI_EVENT_VALUE_CHANGE ){
                vuiEventArgs args;
                args.eventType = VUI_EVENT_VALUE_CHANGE;
                args.element = evt.element;
                args.value = evt.value;
                ofNotifyEvent( onToggleChange, args, evt.element );
                
                if (evt.value == 1 ){
                    for ( vector<Element*>::iterator it= toggles.begin(); it != toggles.end(); it++){
                        if ( (*it) != evt.element ) (*it)->SetSelected(false);
                    }
                    
                    vuiEventArgs args;
                    args.eventType = VUI_EVENT_TOGGLE_CHANGE;
                    args.element = evt.element;
                    args.value = evt.value;
                    ofNotifyEvent( onToggleChange, args, this );
                } else {
                    
                }
                
                
                
                
                
            } else if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ){
                
                /*vuiEventArgs args;
                args.eventType = VUI_EVENT_TOGGLE_CHANGE;
                args.element = this;
                args.value = evt.value;
                ofNotifyEvent( onToggleChange, args, this );*/
            }
        }
        
        
    };
    
}


#endif
