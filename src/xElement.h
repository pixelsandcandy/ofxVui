#pragma once

#ifndef __ofxVui_Element__
#define __ofxVui_Element__

#include "ofMain.h"
#include "ofxEasing.h"

namespace VUI {
	

	class Element : public ofNode
	{
    public:
        
        friend class VUI::EM;
        
	protected:

		string name;

		map< int, map<string, string> > style;
		map< int, map<string, float> > styleFloat;

		map< int, vector<string> > imageIDs;
		map< string, ofImage* > images;
		map< int, bool> hasState;

		int anchorPoint = VUI_ALIGN_LEFT_TOP;

		int state = VUI_STATE_UP;
		int virtualState = VUI_STATE_UP;
        int prevVirtualState = VUI_STATE_UP;
		bool hasStyle = false;

        bool DEBUG_MODE = false;
		
        
		ofVec3f anchorOffset;
		StyleSheet *styleSheet = nullptr;

        bool userUpdating = false;
		ofColor color;
        
        string unparsedStyle;

	public:
		virtual ~Element();
		Element( int x = 0, int y = 0, StyleSheet *ss = nullptr, string selector = "", string selectorB = "" );

		ofEvent<vuiEventArgs> onMouseOver;
        ofEvent<vuiEventArgs> onMouseOut;
		ofEvent<vuiEventArgs> onMousePressed;
        ofEvent<vuiEventArgs> onMouseMoved;
        ofEvent<vuiEventArgs> onMouseDragged;
		ofEvent<vuiEventArgs> onMouseReleased;
        ofEvent<vuiEventArgs> onMouseClick;
        ofEvent<vuiEventArgs> onMouseDoubleClick;

		ofEvent<vuiEventArgs> onTouchDown;
		ofEvent<vuiEventArgs> onTouchUp;
		ofEvent<vuiEventArgs> onTouchTap;
        ofEvent<vuiEventArgs> onTouchDoubleTap;
        
        ofEvent<vuiEventArgs> onStateChange;
        
        ofEvent<vuiEventArgs> onFocus;
        ofEvent<vuiEventArgs> onUnfocus;

		ofFbo *fbo = nullptr;
		ofTexture *maskTex = nullptr;

		int vuiUID = abs((int)ofRandom(7, 7777777777));

        void DEBUG(){
            DEBUG_MODE = true;
        }
        
		void SetMask( ofTexture *maskTexture ) {
			maskTex = maskTexture;

			if (fbo != nullptr) return;

			fbo = new ofFbo();
			//fbo->allocate(styleFloat[state]["width"], styleFloat[state]["height"], GL_RGBA);
            fbo->allocate(width*scale, height*scale, GL_RGBA);
			fbo->begin();
			ofClear(255, 255, 255, 0);
			fbo->end();
		}
        
        float width = 100;
        float height = 100;
        float rotation = 0.0;
        
        float GetWidth(){
            return width*scale;
            //return styleFloat[state]["width"];
        }
        
        float GetHeight(){
            return height*scale;
            //return styleFloat[state]["height"];
        }
        
        int GetState(){
            return state;
        }
        
        float GetRotation(){
            return rotation;
        }
        
        void SetRotation( float r ){
            rotation = r;
        }
        
        void SetState( VUI::State _state ){
            state = _state;
            virtualState = _state;
            
            vuiEventArgs args;
            args.element = this;
            args.eventType = VUI_EVENT_STATE_CHANGE;
            
            ofNotifyEvent( onStateChange, args, this );
        }
        
		int timeLastToggle = -1;
        void SetToggle( bool doToggle = true){
            isToggle = doToggle;
        }

		//

		Element* SetName(string _name) { name = _name; return this; }
        string GetName(){ return name; }

		Element* SetStyle(string style, int state = VUI_STATE_UP, bool initState = true );

        void Render(float parentOffsetX = 0, float parentOffsetY = 0, float parentOpacity = 1.0, ofVec2f _anchorOffset = ofVec2f::zero() );
		void Update(int mouseX = -1, int mouseY = -1, bool internalUpdate = false);
		void ParseStyle(string property = "", int state = VUI_STATE_UP);
		float GetProperty(string property);
		void SetProperty(string property, string val);
		void SetProperty(string property, float val);
        void SetProperty(string property, int val);
        
        
        
        vector<Element*> children;
        Element* parent = nullptr;
        
        bool HasParent(){
            if ( parent == nullptr || parent == NULL ) return false;
            return true;
        }
        
        void AddChild( Element* el){
            if ( !el->HasParent() ) {
                el->SetParent( this );
                children.push_back(el);
            }
        }
        
        vector<Element*> GetChildren(){
            return children;
        }
        
        void SetParent( Element* el ){
            this->parent = el;
        }

		Element* SetSize(float w, float h);
        void SetHeight( float h );
        void SetWidth( float w );
        ofVec2f GetPosition();
		Element* SetPosition(float x, float y);
        void SetPositionX( float x );
        void SetPositionY( float y );
		void UseStyleClass(string name);
		void UseStyleID(string name);
        
        float scale = 1.0;
        
        bool isInteractive = true;
        
        void SetInteractive( bool interactive ){
            isInteractive = interactive;
        }
        
        float GetScale(){
            return scale;
        }
        
        void SetScale( float scale ){
            this->scale = scale;
        }
        
        void Set( string properties ){
            
            string s(properties);
            ofStringReplace(s, "{", " ");
            ofStringReplace(s, "}", " ");
            ofStringReplace(s, " ", "");
            
            vector<string> split = ofSplitString(s, "," );
            for ( vector<string>::iterator it = split.begin(); it != split.end(); it++){
                vector<string> propVal = ofSplitString( (*it), ":" );
                if ( propVal.size() == 2 ){
                    if ( propVal[0] == "x" ){
                        SetPositionX( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "y" ){
                        SetPositionY( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "opacity" ){
                        SetOpacity( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "rotation" ){
                        SetRotation( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "width" ){
                        SetWidth( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "height" ){
                        SetHeight( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "interactive" ){
                        if ( propVal[1] == "true" ) SetInteractive(true);
                        else if ( propVal[1] == "false" ) SetInteractive(false);
                    } else if ( propVal[0] == "scale" ){
                        SetScale( ofToFloat( propVal[1] ) );
                    }
                }
            }
        }
        
		void SetStyleSheet(StyleSheet* ss) {
			styleSheet = ss;
		}
		void SetAnchorPoint(int ap) {
			anchorPoint = ap;
		}
        
        void TriggerEvent(vuiEvent eventType);
        
        map<string,string> propValue;
        
        void StorePropValue( string prop, string value){
            propValue[prop] = value;
        }
        
        virtual void RenderAfter(float parentOffsetX = 0, float parentOffsetY = 0){};
        
        template <typename ArgumentsType, class ListenerClass>
        Tween* Animate( float timeSeconds, string params, ListenerClass* listener, void (ListenerClass::*listenerMethod)(ArgumentsType&) ){
            //ofLog() << timeSeconds << " - " << params;
            return VUI::Animate( this, timeSeconds, params, listener, listenerMethod );
            
        }
        
        Tween* Animate( float timeSeconds, string params ){
            //ofLog() << timeSeconds << " - " << params;
            return VUI::Animate( this, timeSeconds, params );
        }
        
        void SetOpacity( float o ){
            opacity = o;
        }
        
        float GetOpacity(){
            return opacity;
        }

    protected:
        ofTrueTypeFont* font = nullptr;
        vector<string> SplitStyles(string s);
        virtual void UpdateState(int state);
        void UpdatePosition();
        
        ofVec3f localMinPosition;
        ofVec3f localMaxPosition;
        ofVec3f posOffset;
        
        ofVec3f globalMinPosition;
        ofVec3f globalMaxPosition;
        
        ofVec3f drawPosition;
        
        float opacity = 1.0;
        float parentSumOpacity = 1.0;
        
        ofVec2f mouseDownPos;
        bool isMouseDown = false;
        int lastClickTimeMS;
    private:
		void SetDefaultStyles(int x, int y);
		void UpdateAnchorOffset();
        ofVec3f parentSumOffset;
        
        bool isToggle = false;
        int lastTimeMouseDown;
        
        
	};
}

#endif
