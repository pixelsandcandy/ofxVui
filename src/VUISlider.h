//
//  xText.h
//
//  Created by Christopher Miles on 2/09/18
//
//

#pragma once

#ifndef __ofxVui_Slider__
#define __ofxVui_Slider__

#include "ofMain.h"

namespace VUI {
    
    class Slider : public Element
    {
    public:
        ~Slider(){};
        
        Slider( int x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            Setup();
        }
        
        Slider( int x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            Setup();
        }
        
        Slider( string x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            Setup();
        }
        
        Slider( string x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            Setup();
        }
        
        ofEvent<vuiEventArgs> onSliderValueChange;
        
        void SetBar( StyleSheet *ss, string styleBarSelector, string styleBarBgSelector );
        
        void SetPadding( int top, int right, int bottom, int left ){
            padding.Set(top,right,bottom,left);
            UpdateBarStyle();
        }
        
        void SetPadding( int pad ){
            padding.Set(pad,pad,pad,pad);
            UpdateBarStyle();
        }
        
        void SetPadding( int topBottom, int leftRight ){
            padding.Set(topBottom,leftRight,topBottom,leftRight);
            UpdateBarStyle();
        }
        
        float GetPercentageValue(){
            return sliderPerc;
        }
        
        int GetValue(){
            return sliderVal;
        }
        
        void Setup(){
            SetPadding(1);
        }
        
        Padding& GetPadding(){
            return padding;
        }
        
        Element* GetBarContainer(){
            return barContainer;
        }
        
        void SetPercValue(float perc){
            bar->SetWidth( ofToString(perc*100.0) + "%" );
        }
        
        void SetValue(int value){
            if ( value < 0 ) SetPercValue(0.0);
            else if ( value > GetOriginalBarWidth() ) SetPercValue( 1.0 );
            else SetPercValue( value / GetOriginalBarWidth() );
        }
        
    private:
        
        
        Padding padding;
        float sliderPerc = 1.0;
        int sliderVal = 0;
        
        Element* bar = NULL;
        Element* barContainer = NULL;
        
        int width = 0;
        
        void UpdateBarStyle(){
            if ( bar != NULL ){
                barContainer->SetPosition(padding.left, padding.top);
                barContainer->SetWidth( "calc(100%-" + ofToString(padding.left+padding.right) + ")" );
                barContainer->SetHeight( "calc(100%-" + ofToString(padding.top+padding.bottom) + ")" );
                
                bar->SetPosition(0,0);
                bar->SetWidth( "100%" );
                bar->SetHeight( "100%" );
            }
        }
        
        float GetOriginalBarWidth(){
            return barContainer->GetWidth() - 2;
        }
        
        void _vuiEventHandler(vuiEventArgs& evt){
            
            if ( evt.eventType == VUI_EVENT_MOUSE_OVER || evt.eventType == VUI_EVENT_MOUSE_OUT ){
                if ( evt.eventType == VUI_EVENT_MOUSE_OVER ) bar->SetState(VUI_STATE_OVER, false);
                else bar->SetState(VUI_STATE_UP, false);
                return;
            }
            
            if ( evt.eventType == VUI_EVENT_MOUSE_RELEASED || evt.eventType == VUI_EVENT_MOUSE_CLICK ){
                bar->SetState(VUI_STATE_UP, false);
            } else {
                if ( bar->HasState(VUI_STATE_DOWN ) ) bar->SetState(VUI_STATE_DOWN, false);
            }
            
                
            float w = GetOriginalBarWidth();
            
            if ( evt.localMousePos.x < 1 ) sliderVal = 0;
            else if ( evt.localMousePos.x > w) sliderVal = w+2;
            else sliderVal = evt.localMousePos.x;
            
            sliderPerc = sliderVal / w;
            if ( sliderPerc > 1.0 ) sliderPerc = 1.0;
            
            bar->SetWidth( ofToString(sliderPerc*100.0) + "%" );
            
            /*if ( evt.eventType == VUI_EVENT_MOUSE_CLICK || evt.eventType == VUI_EVENT_MOUSE_RELEASED || evt.eventType == VUI_EVENT_MOUSE_PRESSED ) bar->SetWidth( ofToString(sliderPerc*100.0) + "%" );
            else bar->SetWidth( ofToString(sliderPerc*100.0) + "%" );*/
            
            vuiEventArgs args = GetEventArgs(VUI_EVENT_SLIDER_VALUE_CHANGE);
            args.value = sliderVal;
            args.percValue = sliderPerc;
            
            ofNotifyEvent(onSliderValueChange, args, this);
        }
        
        
    };
    
}


#endif
