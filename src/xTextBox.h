//
//  xTextBox.h
//
//  Created by Christopher Miles on 9/27/17.
//
//

#pragma once

#ifndef __ofxVui_TextBox__
#define __ofxVui_TextBox__

#include "ofMain.h"

namespace VUI {
    
    
    class TextBox : public Element
    {
        friend Element;
        
    public:
        
        ~TextBox(){};
        TextBox( const int x = 0, const int y = 0, StyleSheet *ss = nullptr, const string selector = "", const string selectorB = "" ):Element(x,y,ss,selector,selectorB){
            
            VUI::Init();
            
            textColor.setHex(stoul("0xff00ff", nullptr, 16), 255.0 );
        
            //textOffset.y = VUI::fontSize;
            //if ( font != nullptr ) textOffset.y = font->getSize();
            
            /*vector<string> split = SplitStyles( unparsedStyle );
            
            vector<string> tempSplit;
            
            for (vector<string>::iterator it = split.begin(); it != split.end(); it++) {
                tempSplit = ofSplitString((*it), ":");
                if (tempSplit.size() == 2) {
                    ParsePropValue( tempSplit[0], tempSplit[1] );
                }
            }*/
            
            for ( map<string,string>::iterator it = propValue.begin(); it != propValue.end(); it++){
                ParsePropValue( (*it).first, (*it).second );
            }
            
            
            //ofAddListener(onMouseClick, this, &TextBox::_vuiEventHandler );
        
        };
        
        ofEvent<vuiEventArgs> onSubmit;
        
        void ParsePropValue( string prop, string value ){
            //ofLog() << prop << ":" << value;
            
            if ( prop == "text-align" || prop == "textAlign" ){
                if (value == "left-top") SetTextAlignment(VUI_ALIGN_LEFT_TOP);
                else if (value == "left-center") SetTextAlignment(VUI_ALIGN_LEFT_CENTER);
                else if (value == "left-bottom") SetTextAlignment(VUI_ALIGN_LEFT_BOTTOM);
                else if (value == "center-top") SetTextAlignment(VUI_ALIGN_CENTER_TOP);
                else if (value == "center-center") SetTextAlignment(VUI_ALIGN_CENTER_CENTER);
                else if (value == "center-bottom") SetTextAlignment(VUI_ALIGN_CENTER_BOTTOM);
                else if (value == "right-top") SetTextAlignment(VUI_ALIGN_RIGHT_TOP);
                else if (value == "right-center") SetTextAlignment(VUI_ALIGN_RIGHT_CENTER);
                else if (value == "right-bottom") SetTextAlignment(VUI_ALIGN_RIGHT_BOTTOM);
            } else if ( prop == "text-shadow" ){
                vector<string> xy = ofSplitString( value, "," );
                if ( xy.size() == 2 ) SetTextShadow( ofToInt( xy[0] ), ofToInt( xy[1] ) );
                else if ( xy.size() == 3 ) SetTextShadow( ofToInt( xy[0] ), ofToInt( xy[1] ), ofToInt(xy[2]) );
            } else if ( prop == "color" ) {
                string colorStr(value);
                
                float floatColor;
                if ( colorStr.find("#") != string::npos ){
                    string str(colorStr);
                    
                    ofStringReplace(str, "#", "");
                    floatColor = stoul(str, nullptr, 16);
                } else {
                    floatColor = ofToFloat( colorStr );
                }
                
                textColor.setHex(floatColor, 255.0);
            } else if ( prop == "padding" ){
                vector<string> xy = ofSplitString( value, "," );
                if ( xy.size() == 1 ) SetPadding( ofToInt(xy[0]) );
                else if ( xy.size() == 2 ) SetPadding( ofToInt(xy[0]), ofToInt(xy[1]) );
            } else if ( prop == "padding-left" ){
                SetPaddingX( ofToInt(value) );
            } else if ( prop == "padding-top" ){
                SetPaddingY( ofToInt(value) );
            }
        }
        
        
        string text = "";
        Align textAlignment = VUI_ALIGN_LEFT_TOP;
        
        void SetText( string t ){
            text = t;
        }
        
        string GetText(){
            return text;
        }
        
        bool isTextField = false;
        
        void MakeTextField( bool useKeyboardEvents = true ){
            MakeToggle();
            isTextField = true;
            
            if ( useKeyboardEvents ) {
                ofAddListener( ofEvents().keyPressed, this, &TextBox::keyPressed );
                ofAddListener(onStateChange, this, &TextBox::_vuiEventHandler );
            }
        }
        
        void TriggerSubmit(){
            SetState(VUI_STATE_UP);
            
            vuiEventArgs args;
            args.element = this;
            args.eventType = VUI_EVENT_SUBMIT;
            args.text = text;
            
            ofNotifyEvent(onSubmit, args, this);
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
        
        int textLimit = -1;
        
        void SetLimit( int limit ){
            textLimit = limit;
        }
        
        bool IsFocused(){
            if ( GetVirtualState() == VUI_STATE_DOWN ) return true;
            return false;
        }
        
        ofTrueTypeFont* GetFont(){
            return font;
        }
        
        void AddText( string t){
            if ( textLimit != -1 ) {
                if ( text.size() >= textLimit ) return;
            }
            
            text = text + t;
            if ( t == " " ) spaceOffsetX = font->getSize() * .5;
            else spaceOffsetX = 0;
        }
        
        void Backspace(){
            text = text.substr(0, text.size()-1);
            //if ( text.substr(text.size()-2, 1
            //ofLog() << "lastChar: " << text.substr(text.size()-1,1);
            if ( text.size() > 0 && text.substr(text.size()-1,1) == " " ) spaceOffsetX = font->getSize() * .5;
            else spaceOffsetX = 0;
        }
        
        void AddText( char c ){
            text = text + ofToString(c);
        }
        
        void SetTextAlignment( Align align ){
            textAlignment = align;
        }
        
        void SetPadding( int x, int y ){
            padding.set( x, y );
        }
        
        void SetPadding( int xy ){
            SetPadding( xy, xy );
        }
        
        void SetPaddingX( int x ){
            padding.x = x;
        }
        
        void SetPaddingY( int y ){
            padding.y = y;
        }
        
        ofVec2f textOffset;
        ofVec3f shadowPos = ofVec3f(0,0,210);
        ofColor textColor = ofColor::gray;
        
        ofVec2f padding;
        
        int spaceOffsetX = 0;
        
        bool hasTextShadow = false;
        void SetTextShadow( float x, float y, float opacity = 1.0 ){
            shadowPos.set(x,y, opacity*255.0);
            hasTextShadow = true;
        }
        
        
        
        
        void RenderAfter(float parentOffsetX = 0, float parentOffsetY = 0){
            
            //ofLog() << parentOffsetX << "," << parentOffsetY;
            
            
            if ( font != nullptr ) {
                _Calibrate();
                
                //ofSetColor(255,0,255,255);
                ofRectangle rect;
                rect = font->getStringBoundingBox(text, 0,0);
                float x = 0;
                float y = 0;
                
                switch( textAlignment ){
                    case VUI_ALIGN_LEFT_TOP:
                        x = 0;
                        y = 0;
                        break;
                    case VUI_ALIGN_LEFT_CENTER:
                        x = 0;
                        y = ((GetHeight() - textOffset.y)*0.5);
                        break;
                    case VUI_ALIGN_LEFT_BOTTOM:
                        x = 0;
                        y = (GetHeight() - textOffset.y);
                        break;
                    case VUI_ALIGN_CENTER_TOP:
                        x = ((GetWidth() - rect.width)*0.5);
                        y = 0;
                        break;
                    case VUI_ALIGN_CENTER_CENTER:
                        x = ((GetWidth() - rect.width)*0.5);
                        y = ((GetHeight() - textOffset.y)*0.5);
                        break;
                    case VUI_ALIGN_CENTER_BOTTOM:
                        x = ((GetWidth() - rect.width)*0.5);
                        y = (GetHeight() - textOffset.y);
                        break;
                    case VUI_ALIGN_RIGHT_TOP:
                        x = GetWidth() - rect.width;
                        y = 0;
                        break;
                    case VUI_ALIGN_RIGHT_CENTER:
                        x = GetWidth() - rect.width;
                        y = ((GetHeight() - textOffset.y)*0.5);
                        break;
                    case VUI_ALIGN_RIGHT_BOTTOM:
                        x = GetWidth() - rect.width;
                        y = (GetHeight() - textOffset.y);
                        break;
                }
                
                
                if ( hasTextShadow ) {
                    ofSetColor(0,0,0, shadowPos.z );
                    font->drawString( text, shadowPos.x + parentOffsetX + padding.x + x, shadowPos.y + parentOffsetY + padding.y + y + textOffset.y );
                }
                
                ofSetColor(255,255,255,255);
                ofSetColor(textColor);
                font->drawString( text, parentOffsetX + padding.x + x, parentOffsetY + padding.y + y + textOffset.y );
                
                ofSetColor(0,0,0,255);
				//ofLog() << rect.height << " - " << font->getLineHeight() << " (" << font->getSize() << ")";
                
                // typing cursor
                if ( isTextField && GetVirtualState() == VUI_STATE_DOWN ) ofDrawRectangle( parentOffsetX + spaceOffsetX + padding.x + x + rect.width + (font->getSize()*.18), parentOffsetY + padding.y + y - 1 - 2, 1, textOffset.y+4);
            }
        }
        
    private:
        bool _isCalibrated = false;
        float _typingIndicatorOffset;
        void _Calibrate(){
            if ( _isCalibrated ) return;
            _isCalibrated = true;
            
            ofRectangle rect = font->getStringBoundingBox("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+][}{:?><", 0,0);
            
            textOffset.set(0,rect.height - font->getSize()*.3333 );
            _typingIndicatorOffset = font->getSize()*.3333;
        }
        
        void _vuiEventHandler(vuiEventArgs& evt){
            if ( isTextField ){
                if ( evt.eventType == VUI_EVENT_STATE_CHANGE ){
                    if ( evt.virtualState == VUI_STATE_DOWN ) {
                        TriggerEvent( VUI_EVENT_FOCUS );
                    } else if ( evt.virtualState == VUI_STATE_UP ){
                        TriggerEvent( VUI_EVENT_UNFOCUS );
                    }
                }
            }
        }
    };
    
}


#endif
