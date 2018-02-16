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
        
        class PercentCalcValues {
        public:
            PercentCalcValues(){};
            ~PercentCalcValues(){};
            
            class PCV {
            public:
                PCV(){};
                ~PCV(){};
                
                float perc = 1.0;
                float offset = 0;
                float val = 1.0;
                
                bool pxValue = false;
                
                float getValue( float parentValue ){
                    //ofLog() << "pxValue:" << pxValue << "  val:" << val << "  parentValue:" << parentValue << "  perc:" << perc << "  offset:" << offset;
                    if ( pxValue == true ) return val;
                    else return (parentValue*perc)+offset;
                }
                
                void parse(int value){
                    val = (float)value*VUI::dpi;
                    pxValue = true;
                }
                
                void parse(string value){
                    if ( value.find( "calc" ) != -1 && value.find( "%" ) != -1 && value.find("(") != -1 && value.find(")") != -1 && (value.find("-") != -1 || value.find("+") != -1 ) ) {
                        
                        int start, end;
                        start = value.find( "(" ) + 1;
                        end = value.find( "%" );
                        string per = value.substr( start, end - start );
                        
                        perc = ofToFloat( per )*.01;
                        
                        if ( value.find("-") != -1 ) {
                            start = value.find("-")+1;
                            offset = -1;
                        } else {
                            start = value.find("+")+1;
                            offset = 1;
                        }
                            
                        end = value.find(")")+1;
                        
                        string diff = value.substr( start, end - start);
                        
                        offset = (ofToFloat( diff )*offset)*VUI::dpi;
                        pxValue = false;
                        
                    } else if ( value.find("%") != -1 ){
                        perc = ofToFloat( value )*.01;
                        pxValue = false;
                    } else {
                        val = ofToFloat(value)*VUI::dpi;
                        pxValue = true;
                    }
                }
            };
            
            map< string, PCV > values;
            
            void parseValue( string name, string value ){
                values[name].parse(value);
            }
            
            void parseValue( string name, int value ){
                values[name].parse(value);
            }
            
            int getValue(string name, float parentValue ){
                //ofLog() << name << "  -  " << parentValue << "  = " << values[name].getValue(parentValue);
                return values[name].getValue(parentValue);
            }
            
        };
        
	protected:
        PercentCalcValues percentCalcValues;
        
        struct Image {
            ofRectangle bounds;
            ofImage *image;
            ofVec2f scale;
            
            bool active = false;
            
            ImageSize size = VUI_IMAGE_NORMAL;
            
            void Set( ofImage *imagePtr, int x = 0, int y = 0, int width = -1, int height = -1){
                image = imagePtr;
                bounds.set(x, y, width, height );
                active = true;
            }
            
            void Set( ofImage *imagePtr, string value ){
                if ( value == "FILL" ) {
                    image = imagePtr;
                    size = VUI_IMAGE_FILL;
                    scale.set(1,1);
                    bounds.set(0,0, image->getWidth(), image->getHeight() );
                    active = true;
                }
            }
            
            void Clear(){
                active = false;
            }
        };
        
        map< int, Image> bgImage;

        
		string name;

		map< int, map<string, string> > style;
		map< int, map<string, float> > styleFloat;
        map< int, map<string, int> > styleInt;
        
        vector<string> borderProps = {"border-top", "border-bottom", "border-left", "border-right" };

		map< int, vector<string> > imageIDs;
		map< string, ofImage* > images;
		map< int, bool> hasState;

		int anchorPoint = VUI_ALIGN_LEFT_TOP;

		int renderState = VUI_STATE_UP;
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
        Element(){};
        
        void Setup( int x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        
        void Setup( int x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        
        void Setup( string x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        void Setup( string x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        
        
		Element( int x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        Element( int x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        Element( string x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        Element( string x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );

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
        ofEvent<vuiEventArgs> onValueChange;
        ofEvent<vuiEventArgs> onTextChange;
        
        ofEvent<vuiEventArgs> onFocus;
        ofEvent<vuiEventArgs> onUnfocus;
        
#ifdef USING_ofxTouchPadScroll
        ofEvent<vuiEventArgs> onTouchPadScroll;
#endif

		ofFbo *fbo = nullptr;
		ofTexture *maskTex = nullptr;

		int vuiUID = abs((int)ofRandom(7, 7777777777));
        
        int GetVirtualState(){
            return virtualState;
        }

        void DEBUG(){
            DEBUG_MODE = true;
        }
        
        void SetBorderRenderingOrder( string a, string b, string c, string d ){
            borderProps[0] = a;
            borderProps[1] = b;
            borderProps[2] = c;
            borderProps[3] = d;
        };
        
        void RemoveMask(){
            maskTex = nullptr;
            if ( fbo != nullptr ){
                fbo->clear();
                fbo = 0;
                fbo = nullptr;
            }
        }
        
		void SetMask( ofTexture *maskTexture ) {
			maskTex = maskTexture;

			if (fbo != nullptr) return;

			fbo = new ofFbo();
			//fbo->allocate(styleFloat[state]["width"], styleFloat[state]["height"], GL_RGBA);
            fbo->allocate(GetWidth()*scale, GetHeight()*scale, GL_RGBA);
			fbo->begin();
			ofClear(255, 255, 255, 0);
			fbo->end();
		}
        
        bool HasMask(){
            if ( maskTex != nullptr ) return true;
            return false;
        }
        
        ofTexture* GetMask(){
            return maskTex;
        }
        
        float width = 60;
        float height = 60;
        float rotation = 0.0;
        
        int GetState(){
            return renderState;
        }
        
        float GetRotation(){
            return rotation;
        }
        
        void SetRotation( float r ){
            rotation = r;
        }
        
        void SetState( VUI::State toState ){
            
            VUI::State s = toState;
            
            if ( s == VUI_STATE_DOWN && !hasState[VUI_STATE_OVER] ) s = VUI_STATE_OVER;
            if ( s == VUI_STATE_OVER && !hasState[VUI_STATE_OVER] ) s = VUI_STATE_UP;
            
            bool updated = false;
            
            if ( s != renderState ) renderState = s;
            
            UpdateVirtualState( toState );
        }
        
        void SetSelected(){
            SetState( VUI_STATE_DOWN );
        }
        
        void UpdateVirtualState( VUI::State toState, bool force = false ){
            if ( virtualState != toState ){
                
                prevVirtualState = int(virtualState);
                virtualState = toState;
                
                vuiEventArgs args;
                args.element = this;
                args.eventType = VUI_EVENT_STATE_CHANGE;
                args.renderState = int(toState);
                args.virtualState = int(virtualState);
                
                ofNotifyEvent( onStateChange, args, this );
                
                if ( toState == VUI_STATE_DOWN ) {
                    TriggerEvent( VUI_EVENT_VALUE_CHANGE );
                } else if ( prevVirtualState == VUI_STATE_DOWN && (toState == VUI_STATE_OVER || toState == VUI_STATE_UP) ){
                    TriggerEvent( VUI_EVENT_VALUE_CHANGE );
                }
                
                
            }
            
            if ( force ) {
                prevVirtualState = int(virtualState);
                virtualState = toState;
            }
        }
        
		int timeLastToggle = -1;
        void SetToggle( bool doToggle = true){
            isToggle = doToggle;
        }
        
        void MakeToggle(){
            SetToggle();
        }
        
        void SetSelection(bool doSelection = true){
            isSelection = doSelection;
        }
        void MakeSelection(){
            SetSelection();
        }

		//

		Element* SetName(string _name) { name = _name; return this; }
        string GetName(){ return name; }

		Element* SetStyle(string style, int renderState = VUI_STATE_UP, bool initState = true );

        void Render(float parentOffsetX = 0, float parentOffsetY = 0, float parentOpacity = 1.0, ofVec2f _anchorOffset = ofVec2f::zero(), ofVec2f _parentOffsetPos = ofVec2f::zero() );
		void Update(int mouseX = -1, int mouseY = -1, bool internalUpdate = false);
		void ParseStyle(string property = "", int renderState = VUI_STATE_UP);
        void ParseStyleSheet(StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "");
		float GetProperty(string property);
		void SetProperty(string property, string val);
		void SetProperty(string property, float val);
        void SetProperty(string property, int val);
        
        
        
        vector<Element*> children;
        Element* parent = nullptr;
        
        VUI::EM* EventManager = NULL;
        
        VUI::EM* GetEventManager(){
            if ( this->EventManager == NULL ) return &VUI::EventManager;
            else return this->EventManager;
        }
        
        virtual void SetEventManager(VUI::EM* eventManager){
            this->EventManager = eventManager;
        }
        
        bool HasParent(){
            if ( parent == nullptr || parent == NULL ) return NULL;
            return true;
        }
        
        virtual void AddChild( Element* el){
            if ( !el->HasParent() ) {
                el->SetParent( this );
                el->SetEventManager( EventManager );
                children.push_back(el);
            }
        }
        
        virtual vector<Element*> GetChildren(){
            return children;
        }
        
        void SetParent( Element* el ){
            this->parent = el;
        }

		Element* SetSize(float w, float h);
        Element* SetSize(string w, string h);
        void SetHeight( float h );
        void SetWidth( float w );
        void SetHeight( string h );
        void SetWidth( string w );
        
        int GetHeight( bool scaled = true);
        int GetWidth(bool scaled = true);
        int GetOriginalHeight( bool scaled = true);
        int GetOriginalWidth(bool scaled = true);
        
        ofVec2f GetPosition();
		Element* SetPosition(float x, float y);
        void SetPosition(string x, string y);
        void SetPosition(float x, string y);
        void SetPosition(string x, float y);
        void SetPositionX( float x );
        void SetPositionY( float y );
        void SetPositionX( string x );
        void SetPositionY( string y );
		void UseStyleClass(string name);
		void UseStyleID(string name);
        void UseStyle(string style);
        
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
		void SetAnchorPoint(Align alignment) {
			anchorPoint = alignment;
		}
        
        void TriggerEvent(vuiEvent eventType);
        
        vuiEventArgs GetEventArgs(vuiEvent eventType);
        
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
        
        bool IsMouseInside(){
            return _mInside;
        }
        
        Tween* tween = NULL;
        
        void StopTween(){
            if ( tween != NULL ) {
                tween->Stop();
                tween = NULL;
            }
        }
        
        void StoreTween(Tween* tween){
            this->tween = tween;
        }
        
        void SetOffsetY(int offset);
        void SetOffsetX(int offset);
				
    protected:
        ofTrueTypeFont* font = nullptr;
        vector<string> SplitStyles(string s);
        virtual void UpdateState(int renderState, bool isInside = false, bool isMouseDown = false);
        void UpdatePosition();
        
        ofVec3f localMinPosition;
        ofVec3f localMaxPosition;
        ofVec3f posOffset;
        ofVec2f parentOffsetPos;
        ofVec2f parentAnchorOffset;
        
        ofVec3f globalMinPosition;
        ofVec3f globalMaxPosition;
        
        ofVec4f globalPos;
        ofVec2f size;
        
        ofVec3f drawPosition;
        
        float opacity = 1.0;
        float parentSumOpacity = 1.0;
        
        ofVec2f mouseDownPos;
        bool isMouseDown = false;
        bool isMouseInside = false;
        bool _mInside = false;
        int lastClickTimeMS = 0;
		int touchDownTimeMS = 0;

		ofVec2f touchPoint;
    private:
		void SetDefaultStyles(int x, int y);
		void UpdateAnchorOffset();
        ofVec3f parentSumOffset;
        
        bool isToggle = false;
        bool isSelection = false;
        int lastTimeMouseDown;
        int fixMouseY = 3;
        
	};
}

#endif
