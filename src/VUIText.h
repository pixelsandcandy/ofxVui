//
//  xText.h
//
//  Created by Christopher Miles on 9/27/17.
//
//

#pragma once

#ifndef __ofxVui_Text__
#define __ofxVui_Text__

#include "ofMain.h"

namespace VUI {
    
    
    class Text : public Element
    {
        friend Element;
        
    public:
        Text(){};
        ~Text(){};
        
        
        
        Text( int x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            _Setup(ss,primarySelector,secondarySelector);
        }
        
        Text( int x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            _Setup(ss,primarySelector,secondarySelector);
        }
        
        Text( string x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            _Setup(ss,primarySelector,secondarySelector);
        }
        
        Text( string x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            _Setup(ss,primarySelector,secondarySelector);
        }
        
        
        
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
        
        
        
        void SetText( string t, bool sizeToText = false ){
            text = t;
            UpdateRect();
            if ( sizeToText ) SizeToText();
        }
        
        string GetText(){
            return text;
        }
        
        void SizeToText(){
            ofRectangle rect = font->getStringBoundingBox(text, 0,0);
            rect.width*=VUI::divideDpi;
            rect.height*=VUI::divideDpi;
            SetSize( rect.width + padding.x*2, rect.height + padding.y*2 );
        }
        
        void SizeWidthToText(){
            ofRectangle rect = font->getStringBoundingBox(text, 0,0);
            rect.width*=VUI::divideDpi;
            rect.height*=VUI::divideDpi;
            SetWidth( rect.width + padding.x*2 );
        }
        
        bool IsFocused(){
            if ( GetVirtualState() == VUI_STATE_DOWN ) return true;
            return false;
        }
        
        ofTrueTypeFont* GetFont(){
            return font;
        }
        
        void SetTextAlignment( Align align ){
            textAlignment = align;
        }
        
        void SetPadding( int x, int y ){
            padding.set( x*VUI::dpi, y*VUI::dpi );
        }
        
        void SetPadding( int xy ){
            SetPadding( xy*VUI::dpi, xy*VUI::dpi );
        }
        
        void SetPaddingX( int x ){
            padding.x = x*VUI::dpi;
        }
        
        void SetPaddingY( int y ){
            padding.y = y*VUI::dpi;
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
        
        bool isTextField = false;
        
        ofRectangle rect;
        
        void RenderAfter(float parentOffsetX = 0, float parentOffsetY = 0){
            
            //ofLog() << parentOffsetX << "," << parentOffsetY;
            
            
            if ( font != nullptr ) {
                _Calibrate();
                
                //ofSetColor(255,0,255,255);
                float x = 0;
                float y = 0;
                
                switch( textAlignment ){
                    case VUI_ALIGN_LEFT_TOP:
                        x = 0;
                        y = 0;
                        break;
                    case VUI_ALIGN_LEFT_CENTER:
                        x = 0;
                        y = ((GetHeight() - textOffset.y)*0.5) + padding.y;
                        break;
                    case VUI_ALIGN_LEFT_BOTTOM:
                        x = 0;
                        y = (GetHeight() - textOffset.y);
                        break;
                    case VUI_ALIGN_CENTER_TOP:
                        x = ((GetOriginalWidth() - rect.width)*0.5) + padding.x;
                        y = 0;
                        break;
                    case VUI_ALIGN_CENTER_CENTER:
                        x = ((GetOriginalWidth() - rect.width)*0.5) + padding.x;
                        y = ((GetHeight() - textOffset.y)*0.5) + padding.y;
                        break;
                    case VUI_ALIGN_CENTER_BOTTOM:
                        x = ((GetOriginalWidth() - rect.width)*0.5) + padding.x;
                        y = (GetHeight() - textOffset.y);
                        break;
                    case VUI_ALIGN_RIGHT_TOP:
                        x = GetOriginalWidth() - rect.width;
                        y = 0;
                        break;
                    case VUI_ALIGN_RIGHT_CENTER:
                        x = GetOriginalWidth() - rect.width;
                        y = ((GetHeight() - textOffset.y)*0.5) + padding.y;
                        break;
                    case VUI_ALIGN_RIGHT_BOTTOM:
                        x = GetOriginalWidth() - rect.width;
                        y = (GetHeight() - textOffset.y);
                        break;
                }
                
                x-=1*VUI::dpi;
                
                
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
        
        void UpdateRect(){
            rect = font->getStringBoundingBox(text, 0,0);
            //rect.width *= VUI::divideDpi;
            //rect.height *= VUI::divideDpi;
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
        
        
        
        void _Setup(StyleSheet *ss = nullptr, const string primarySelector = "", const string secondarySelector = ""){
            textColor.setHex(stoul("0x000000", nullptr, 16), 255.0 );
            
            for (int i = 0; i < 3; i++) {
                style[i]["background-color"] = "clear";
            }
            
            hasStyle = false;
            
            ParseStyle();
            
            ParseStyleSheet(ss, primarySelector, secondarySelector );
            
            
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
        }
    };
    
}


#endif
