#pragma once

#ifndef __ofxVui__
#define __ofxVui__

#include <algorithm>

#include "ofMain.h"
#include "ofxEasing.h"
#include "VUIView.h"

#ifdef USING_ofxWinTouchHook
#include "ofxWinTouchHook.h"
#endif

#ifdef USING_ofxTouchPadScroll
#include "ofxTouchPadScroll.h"
#endif

#ifdef USING_ofxWindowManager
#include "ofxWindowManager.h"
#endif

namespace VUI {
	class Element;

	enum State {
		VUI_STATE_UP,
		VUI_STATE_OVER,
		VUI_STATE_DOWN,
		VUI_STATE_ALL
	};

    enum Rotate {
        VUI_ROTATE_NONE,
        VUI_ROTATE_90_CCW
        /* TODO
             VUI_ROTATE_90_CW,
             VUI_ROTATE_180
         */
    };
    
    enum Stack {
        VUI_STACK_HORZ,
        VUI_STACK_HORZ_REVERSE,
        VUI_STACK_VERT,
        VUI_STACK_VERT_REVERSE
    };
    
	enum Align {
		VUI_ALIGN_LEFT_TOP,
		VUI_ALIGN_LEFT_CENTER,
		VUI_ALIGN_LEFT_BOTTOM,
		VUI_ALIGN_RIGHT_TOP,
		VUI_ALIGN_RIGHT_CENTER,
		VUI_ALIGN_RIGHT_BOTTOM,
		VUI_ALIGN_CENTER_TOP,
		VUI_ALIGN_CENTER_CENTER,
		VUI_ALIGN_CENTER_BOTTOM
	};
    
    enum ImageSize {
        VUI_IMAGE_NORMAL,
        VUI_IMAGE_FILL
    };

	enum vuiEvent {
        VUI_EVENT_MOUSE_OVER,
        VUI_EVENT_MOUSE_OUT,
        VUI_EVENT_MOUSE_PRESSED,
        VUI_EVENT_MOUSE_MOVED,
        VUI_EVENT_MOUSE_DRAGGED,
        VUI_EVENT_MOUSE_RELEASED,
		VUI_EVENT_MOUSE_CLICK,
        VUI_EVENT_MOUSE_DOUBLE_CLICK,

		VUI_EVENT_TOUCH_DOWN,
		VUI_EVENT_TOUCH_UP,
		VUI_EVENT_TOUCH_TAP,
        VUI_EVENT_TOUCH_DOUBLE_TAP,
        
        VUI_EVENT_STATE_CHANGE,
        VUI_EVENT_TEXT_CHANGE,
        VUI_EVENT_VALUE_CHANGE,
        VUI_EVENT_SLIDER_VALUE_CHANGE,
        VUI_EVENT_TOGGLE_CHANGE,
        VUI_EVENT_SUBMIT,
        
        VUI_EVENT_FOCUS,
        VUI_EVENT_UNFOCUS,
        
        VUI_EVENT_ANIMATE_COMPLETE,
        VUI_EVENT_ANIMATE_STEP,
        VUI_EVENT_ANIMATE_START
        
#ifdef USING_ofxTouchPadScroll
        ,VUI_EVENT_TOUCHPAD_SCROLL_START
        ,VUI_EVENT_TOUCHPAD_SCROLL
        ,VUI_EVENT_TOUCHPAD_SCROLL_END
        ,VUI_EVENT_TOUCHPAD_SCROLL_INERTIA
#endif
	};

#ifdef USING_ofxTouchPadScroll
    extern ofxTouchPadScroll tps;
#endif
	
    extern Rotate uiRotation;
    extern Rotate viewRotation;
    
    extern map<string, map<int, map< float, ofTrueTypeFont*>>> fonts;
    extern int fontSize;
    extern float dpi;
    extern float divideDpi;
    
    static void SetFontSize( int size ){
        fontSize = size;
    }
    
    static void SetDpi( float _dpi ){
        dpi = _dpi;
        divideDpi = 1.0 / _dpi;
    }
    
    static float GetDpi(){
        return dpi;
    }
    
    static bool HasFont( string filename, int fontSize, float letterSpacing ){
        if ( fonts[filename][fontSize*VUI::dpi][letterSpacing] != nullptr ) return true;
        return false;
    }
    
    static ofTrueTypeFont* GetFont( string filename, int fontSize, float letterSpacing ){
        return fonts[filename][fontSize*VUI::dpi][letterSpacing];
    }
    
    static ofTrueTypeFont* AddFont( string filename, int fontSize, float letterSpacing = 1.0 ){
        ofTrueTypeFont* tf = new ofTrueTypeFont();
        tf->load(filename, fontSize*VUI::dpi);
        tf->setLetterSpacing(letterSpacing);
        
        fonts[filename][fontSize*VUI::dpi][letterSpacing] = tf;
        return fonts[filename][fontSize*VUI::dpi][letterSpacing];
    }

    
    struct vuiEventArgs;
    
    //
    
    struct Padding {
        int top = 0;
        int right = 0;
        int bottom = 0;
        int left = 0;
        
        void Set(int top, int right, int bottom, int left ){
            this->top = top;
            this->right = right;
            this->bottom = bottom;
            this->left = left;
        }
    };
    
    // Tweening
    
    class Tween {
    public:
        ~Tween(){};
        Tween(){};
        
        float x;
        float y;
        float width;
        float height;
        float rotation;
        ofColor color;
        float delay = 0;
        float opacity;
        float scale;

		string id = "";

		string GetID() {
			return id;
		}
        
        ofEvent<vuiEventArgs> onComplete;
        ofEvent<vuiEventArgs> onStart;
        ofEvent<vuiEventArgs> onStep;
        
        int UID = abs((int)ofRandom(7, 7777777777));
        
        float startTime;
        float endTime;
        float duration;
        float perc = 0.0;
        
        float GetPercentCompleted(){
            return perc;
        }
        
        float GetProgress(){
            return perc;
        }
        
        vector<string> valueNames;
        map<string, float> endValues;
        map<string, float> startValues;
        
        std::function<float(float,float,float,float)> ease = ofxeasing::linear::easeNone;
        
        Element* el;
        bool active = false;
        
        string cmd;
        bool firstStep = false;
        
        void Start( Element* el, float timeSeconds, string params );
        void Stop();
        void Update( float currTime );
        void UpdateValues();
        void StoreValue( string param, string val );
        void StoreStartValues();
        void SetID( string name ){
            id = name;
        }
        
        
        template <typename ArgumentsType, class ListenerClass>
        void StoreComplete( ListenerClass* listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
            
        }
        
        
    };
    
    struct vuiEventArgs {
        Element* element = NULL;
        int eventType;
        int renderState;
        int virtualState;
        
        int value = -1;
        float percValue = -1.0;
        string text = "";
        
        ofVec2f localMousePos;
        ofVec2f localDragDelta;
        ofVec2f localDragStart;
        
        ofVec2f globalMousePos;
        
        Tween* tween = NULL;
        
#ifdef USING_ofxTouchPadScroll
        ofVec2f touchPadScroll;
#endif
    };
    
    extern vector<Tween*> tweens;
    
    Tween* Animate( Element* el, float timeSeconds, string params );
    
    template <typename ArgumentsType, class ListenerClass>
    static Tween* Animate( Element* el, float timeSeconds, string params, ListenerClass* listener, void (ListenerClass::*listenerMethod)(ArgumentsType&) ){
        
        Tween *t = new Tween();
        t->StoreComplete( listener, listenerMethod );
        
        ofAddListener( t->onComplete, listener, listenerMethod );
        
        t->Start( el, timeSeconds, params );
        
        tweens.push_back( t );
        return t;
    }
    
    
    
    // EventManager
    
    //extern map<vuiEvent, vector<Element*>> events;
    //extern map<State, vector<Element*>> states;
    extern vector<Tween*> tweensToDestroy;
    
    static void ShouldDestroyTween( Tween* t ){
        tweensToDestroy.push_back( t );
    }
    
    static void DestroyTween(Tween* t){
        int index = 0;
        for ( vector<Tween*>::iterator it = tweens.begin(); it != tweens.end(); it++ ){
            if ( (*it) == t ){
                //ofLog() << "found tween - destroying...";
                t = nullptr;
                tweens.erase( tweens.begin() + index );
                return;
            }
            index++;
        }
    }
    
    /*class EMBridge {
    public:
        ~EMBridge(){};
        EMBridge(){};
        
        void Init();
        
        void ShouldDestroyTween( Tween* t ){
            tweensToDestroy.push_back( t );
        }
        
        void DestroyTween(Tween* t){
            int index = 0;
            for ( vector<Tween*>::iterator it = tweens.begin(); it != tweens.end(); it++ ){
                if ( (*it) == t ){
                    //ofLog() << "found tween - destroying...";
                    t = nullptr;
                    tweens.erase( tweens.begin() + index );
                    return;
                }
                index++;
            }
        }
        
    private:
        vector<Tween*> tweensToDestroy;
        
        bool EventHasElement( vuiEvent eventType ){
            return !VUI::events[eventType].empty();
        }
        
        Element* GetLatestElement( vuiEvent eventType ){
            return VUI::events[eventType].back();
        }
        
        void update(ofEventArgs & args);
        
    };*/
    
    //
    
    static vector<vuiEvent> evtlist = {
        VUI_EVENT_MOUSE_OVER,
        VUI_EVENT_MOUSE_OUT,
        VUI_EVENT_MOUSE_PRESSED,
        VUI_EVENT_MOUSE_MOVED,
        VUI_EVENT_MOUSE_DRAGGED,
        VUI_EVENT_MOUSE_RELEASED,
        VUI_EVENT_MOUSE_CLICK,
        VUI_EVENT_MOUSE_DOUBLE_CLICK
    };
    
    //static vector<State> statelist = {VUI_STATE_UP,VUI_STATE_OVER,VUI_STATE_DOWN};
    static vector<State> statelist = {VUI_STATE_UP,VUI_STATE_OVER,VUI_STATE_DOWN};
    
    class EM {
    public:
        EM(){};
        ~EM(){
            ofRemoveListener(ofEvents().update, this, &EM::update);
        };
        
        string name = "";
        
        map<vuiEvent, vector<Element*>> events;
        map<State, vector<Element*>> states;
        
        Element* overElement = nullptr;
        Element* prevOverElement = nullptr;
        
        int vw = 0;
        int vh = 0;
        int mouseX = 0;
        int mouseY = 0;
        
        void StoreOverElement( Element *el);
		
        Element* GetOverElement(){
            return overElement;
        }
        
        bool active = true;
        bool enableOnMouseUp = false;
        
        void StoreEvent( Element *el, vuiEvent eventType );
        void StoreState( Element *el, State state );
        
		void Disable();        int shouldEnable = 0;
        
        void Enable();
        bool IsActive(){
            return active;
        }
        
        void DelayEnable( int delay ){
            shouldEnable = ofGetElapsedTimeMillis() + delay;
        }
        
        void Purge();
        
        void Init(int windowW, int windowH);
        
#ifdef USING_ofxWindowManager
        ofxAppBaseNewWindow* window = NULL;
        void Init(ofxAppBaseNewWindow* window);
#endif
        
#ifdef USING_ofxTouchPadScroll
        ofEvent<vuiEventArgs> onTouchPadScroll;
        void touchPadScroll(int x, int y, vuiEvent eventType);
#endif
        
        /*void ShouldDestroyTween( Tween* t ){
            tweensToDestroy.push_back( t );
        }
        
        void DestroyTween(Tween* t){
            int index = 0;
            for ( vector<Tween*>::iterator it = tweens.begin(); it != tweens.end(); it++ ){
                if ( (*it) == t ){
                    //ofLog() << "found tween - destroying...";
                    t = nullptr;
                    tweens.erase( tweens.begin() + index );
                    return;
                }
                index++;
            }
        }*/
        
    private:
        //vector<Tween*> tweensToDestroy;
        
        bool EventHasElement( vuiEvent eventType ){
            return !events[eventType].empty();
        }
        
        Element* GetLatestElement( vuiEvent eventType ){
            return events[eventType].back();
        }
        
        void update(ofEventArgs & args);
    };
    
    class TM {
    public:
        TM(){};
        ~TM(){};
        
        void Init();
        void update(ofEventArgs& args);
    };
    
    extern TM TweenManager;
    extern EM EventManager;
    //extern EMBridge PRIVATE_EM;
    
    extern EM* currEventManager;
    
    static EM* GetCurrentEventManager(){
        if ( currEventManager == NULL ) return &EventManager;
        return currEventManager;
    }
    
	static Element* GetOverElement() {
		return GetCurrentEventManager()->overElement;
	}
    
    static Element* GetPrevOverElement() {
        return GetCurrentEventManager()->prevOverElement;
    }

    static void ClearOverElement();
    
    
    
    

}


#include "VUIStyleSheet.h"
#include "VUIElement.h"
#include "VUIText.h"
#include "VUITextField.h"
#include "VUIToggleGroup.h"
#include "VUISlider.h"
#include "VUIContainer.h"


// ViewManager
// ===================================
// ===================================

namespace VUI {

	enum Status {
		VIEW_INACTIVE,
		VIEW_STARTING,
		VIEW_ACTIVE,
		VIEW_FINISHING
	};

	extern map< string, View*> views;
	extern string currView;
	extern string nextView;
	extern ofVec2f multCoords;
	extern int mouseX;
	extern int mouseY;

	extern int vw;
	extern int vh;
	extern float vscale;
	extern ofFbo fbo;

	extern map< int, ofPoint> touches;
    
    // UI
    
    extern map<string, StyleSheet*> styleSheets;
    static void AddStyleSheet(string name, StyleSheet* styleSheet ){
        styleSheets[name] = styleSheet;
    }
    
    static StyleSheet* GetStyleSheet(string name){
        return styleSheets[name];
    }

	//

	static map< int, ofPoint>* GetTouches() {
		return &touches;
	}
    
    extern bool isListening;
    
    
    static void ScaleView(float scale) {
        multCoords.x = 1.0 / scale;
        multCoords.y = 1.0 / scale;
        
        vscale = scale;
    }
    
    extern bool useViewManager;
    
    static void SetResolution(int w, int h, float scale = 1.0, bool enableViewManager = true ) {
        vw = w*VUI::dpi;
        vh = h*VUI::dpi;
        
        ScaleView(scale);
        useViewManager = enableViewManager;
    }

	class ViewManagerBridge {
	public:
		~ViewManagerBridge() {};
		ViewManagerBridge() {};
        
		void StartView(string name);

		bool isReady = false;

		void Init() {
			if (isReady) return;

			if (!isReady) {
                if ( VUI::vw == -1 || VUI::vh == -1 ){
                    VUI::SetResolution( ofGetWidth(), ofGetHeight() );
                }
                //ofLog() << VUI::vw << "x" << VUI::vh;
                
                VUI::GetCurrentEventManager()->vw = VUI::vw;
                VUI::GetCurrentEventManager()->vh = VUI::vh;
                
                if ( VUI::vscale != 1 ) {
                    VUI::fbo.allocate(VUI::vw, VUI::vh, GL_RGBA);
                    VUI::fbo.begin();
                    ofClear(0);
                    VUI::fbo.end();
                }
				
			}

			isReady = true;
		}

		/*void Next() {
		if (VUI::nextView.empty()) return;

		VUI::views[VUI::nextView]->Exit();

		StartView(VUI::nextView);
		}*/

		ofEvent<ofMouseEventArgs> onMouseMoved;
		ofEvent<ofMouseEventArgs> onMouseDragged;
		ofEvent<ofMouseEventArgs> onMousePressed;
		ofEvent<ofMouseEventArgs> onMouseReleased;

		ofEvent<ofMouseEventArgs> onMouseScrolled;
		ofEvent<ofMouseEventArgs> onMouseEntered;
		ofEvent<ofMouseEventArgs> onMouseExited;

		ofEvent<ofTouchEventArgs> onTouchDown;
		ofEvent<ofTouchEventArgs> onTouchMoved;
		ofEvent<ofTouchEventArgs> onTouchUp;
        
        bool isTouchListening = false;

		void DisableMouseEvents() {
			ofRemoveListener(ofEvents().mouseMoved, this, &ViewManagerBridge::mouseMoved);
			ofRemoveListener(ofEvents().mouseDragged, this, &ViewManagerBridge::mouseDragged);
			ofRemoveListener(ofEvents().mousePressed, this, &ViewManagerBridge::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
			ofRemoveListener(ofEvents().mouseReleased, this, &ViewManagerBridge::mouseReleased);

			ofRemoveListener(ofEvents().mouseEntered, this, &ViewManagerBridge::mouseEntered);
			ofRemoveListener(ofEvents().mouseExited, this, &ViewManagerBridge::mouseExited);

			ofRemoveListener(ofEvents().messageEvent, this, &ViewManagerBridge::messageReceived);
			ofRemoveListener(ofEvents().messageEvent, this, &ViewManagerBridge::messageReceived);
		}
        
        shared_ptr<ofAppBaseWindow> targetWindow = NULL;
        EM* eventManager = NULL;
        ofCoreEvents* events = NULL;
        
        void ClearWindowEventRef(EM* eventManager = NULL){
            if ( eventManager == NULL || this->eventManager == eventManager ){
                this->eventManager = NULL;
                VUI::EventManager.Enable();
                VUI::currEventManager = NULL;
            }
            
            ListenToMainWindow();
        }
        
        void ClearWindowEventRef(bool disableCurrent){
            if ( disableCurrent ) {
                this->eventManager->Disable();
                this->eventManager = NULL;
            }
            
            ListenToMainWindow();
        }
        
        void ListenToWindow( ofCoreEvents & events, EM* eventManager ){
            VUI::EventManager.Disable();
            Unlisten();
            
            eventManager->Enable();
            this->eventManager = eventManager;
            
            this->events = &events;
            
            ofAddListener(events.update, this, &ViewManagerBridge::update);
            //ofAddListener(ofEvents().setup, this, &ViewManagerBridge::setup);
            
            ofAddListener(events.windowResized, this, &ViewManagerBridge::windowResized);
            
            ofAddListener(events.keyPressed, this, &ViewManagerBridge::keyPressed);
            ofAddListener(events.keyReleased, this, &ViewManagerBridge::keyReleased);
            
            ofAddListener(events.mouseMoved, this, &ViewManagerBridge::mouseMoved);
            ofAddListener(events.mouseDragged, this, &ViewManagerBridge::mouseDragged);
            ofAddListener(events.mousePressed, this, &ViewManagerBridge::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
            ofAddListener(events.mouseReleased, this, &ViewManagerBridge::mouseReleased);
            
            ofAddListener(events.mouseEntered, this, &ViewManagerBridge::mouseEntered);
            ofAddListener(events.mouseExited, this, &ViewManagerBridge::mouseExited);
            
            ofAddListener(events.messageEvent, this, &ViewManagerBridge::messageReceived);
            
            ofAddListener(events.fileDragEvent, this, &ViewManagerBridge::dragged);
        }
        
        void ListenToMainWindow(){
            Unlisten();
            Listen();
            VUI::currEventManager = NULL;
            VUI::EventManager.Enable();
        }
        
        void Unlisten(){
            if ( events != NULL ) {
                ofRemoveListener(events->update, this, &ViewManagerBridge::update);
                //ofAddListener(ofEvents().setup, this, &ViewManagerBridge::setup);
                
                ofRemoveListener(events->windowResized, this, &ViewManagerBridge::windowResized);
                
                ofRemoveListener(events->keyPressed, this, &ViewManagerBridge::keyPressed);
                ofRemoveListener(events->keyReleased, this, &ViewManagerBridge::keyReleased);
                
                ofRemoveListener(events->mouseMoved, this, &ViewManagerBridge::mouseMoved);
                ofRemoveListener(events->mouseDragged, this, &ViewManagerBridge::mouseDragged);
                ofRemoveListener(events->mousePressed, this, &ViewManagerBridge::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
                ofRemoveListener(events->mouseReleased, this, &ViewManagerBridge::mouseReleased);
                
                ofRemoveListener(events->mouseEntered, this, &ViewManagerBridge::mouseEntered);
                ofRemoveListener(events->mouseExited, this, &ViewManagerBridge::mouseExited);
                
                ofRemoveListener(events->messageEvent, this, &ViewManagerBridge::messageReceived);
                
                ofRemoveListener(events->fileDragEvent, this, &ViewManagerBridge::dragged);
                events = NULL;
                
                if ( eventManager != NULL ) {
                    eventManager->Disable();
                    eventManager = NULL;
                }
            } else {
                ofRemoveListener(ofEvents().update, this, &ViewManagerBridge::update);
                //ofAddListener(ofEvents().setup, this, &ViewManagerBridge::setup);
                
                ofRemoveListener(ofEvents().windowResized, this, &ViewManagerBridge::windowResized);
                
                ofRemoveListener(ofEvents().keyPressed, this, &ViewManagerBridge::keyPressed);
                ofRemoveListener(ofEvents().keyReleased, this, &ViewManagerBridge::keyReleased);
                
                ofRemoveListener(ofEvents().mouseMoved, this, &ViewManagerBridge::mouseMoved);
                ofRemoveListener(ofEvents().mouseDragged, this, &ViewManagerBridge::mouseDragged);
                ofRemoveListener(ofEvents().mousePressed, this, &ViewManagerBridge::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
                ofRemoveListener(ofEvents().mouseReleased, this, &ViewManagerBridge::mouseReleased);
                
                ofRemoveListener(ofEvents().mouseEntered, this, &ViewManagerBridge::mouseEntered);
                ofRemoveListener(ofEvents().mouseExited, this, &ViewManagerBridge::mouseExited);
                
                ofRemoveListener(ofEvents().messageEvent, this, &ViewManagerBridge::messageReceived);
                
                ofRemoveListener(ofEvents().fileDragEvent, this, &ViewManagerBridge::dragged);
            }
            
            
            
        }


		void Listen() {
            
			ofAddListener(ofEvents().update, this, &ViewManagerBridge::update);
			//ofAddListener(ofEvents().setup, this, &ViewManagerBridge::setup);

			ofAddListener(ofEvents().windowResized, this, &ViewManagerBridge::windowResized);

			ofAddListener(ofEvents().keyPressed, this, &ViewManagerBridge::keyPressed);
			ofAddListener(ofEvents().keyReleased, this, &ViewManagerBridge::keyReleased);

			ofAddListener(ofEvents().mouseMoved, this, &ViewManagerBridge::mouseMoved);
			ofAddListener(ofEvents().mouseDragged, this, &ViewManagerBridge::mouseDragged);
			ofAddListener(ofEvents().mousePressed, this, &ViewManagerBridge::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
			ofAddListener(ofEvents().mouseReleased, this, &ViewManagerBridge::mouseReleased);

			ofAddListener(ofEvents().mouseEntered, this, &ViewManagerBridge::mouseEntered);
			ofAddListener(ofEvents().mouseExited, this, &ViewManagerBridge::mouseExited);

			ofAddListener(ofEvents().messageEvent, this, &ViewManagerBridge::messageReceived);

			ofAddListener(ofEvents().fileDragEvent, this, &ViewManagerBridge::dragged);
            
#ifdef USING_ofxTouchPadScroll
            ofAddListener(tps.onScrollStart, this, &ViewManagerBridge::touchPadScrollStart);
            ofAddListener(tps.onScroll, this, &ViewManagerBridge::touchPadScroll);
            ofAddListener(tps.onScrollEnd, this, &ViewManagerBridge::touchPadScrollEnd);
            ofAddListener(tps.onScrollInertia, this, &ViewManagerBridge::touchPadScrollInertia);
#endif

            
		}
        
        void EnableTouchEvents(){
            if ( isTouchListening ) return;
            isTouchListening = true;
            
#ifdef USING_ofxWinTouchHook
            ofxWinTouchHook::EnableTouch();
            
            ofAddListener(ofxWinTouchHook::touchDown, this, &ViewManagerBridge::touchDown);
            ofAddListener(ofxWinTouchHook::touchMoved, this, &ViewManagerBridge::touchMoved);
            ofAddListener(ofxWinTouchHook::touchUp, this, &ViewManagerBridge::touchUp);
#else
            ofAddListener(ofEvents().touchDown, this, &ViewManagerBridge::touchDown);
            ofAddListener(ofEvents().touchMoved, this, &ViewManagerBridge::touchMoved);
            ofAddListener(ofEvents().touchUp, this, &ViewManagerBridge::touchUp);
#endif

			DisableMouseEvents();
        }


	private:
		// copied from ofBaseApp
		void setup(ofEventArgs& args);
		void update(ofEventArgs & args);
		void draw(ofEventArgs & args);
		void windowResized(ofResizeEventArgs & resize);
		void keyPressed(ofKeyEventArgs & key);
		void keyReleased(ofKeyEventArgs & key);
		void mouseMoved(ofMouseEventArgs & mouse);
		void mouseDragged(ofMouseEventArgs & mouse);
		void mousePressed(ofMouseEventArgs & mouse);
		void mouseReleased(ofMouseEventArgs & mouse);
		void mouseScrolled(ofMouseEventArgs & mouse);
		void mouseEntered(ofMouseEventArgs & mouse);
		void mouseExited(ofMouseEventArgs & mouse);
		void dragged(ofDragInfo & drag);
		void messageReceived(ofMessage & message);
		void touchDown(ofTouchEventArgs & touch);
		void touchMoved(ofTouchEventArgs & touch);
		void touchUp(ofTouchEventArgs & touch);
		void touchDoubleTap(ofTouchEventArgs & touch);
		void touchCancelled(ofTouchEventArgs & touch);
#ifdef USING_ofxTouchPadScroll
        void touchPadScrollStart(TouchPadScrollEventArgs& args);
        void touchPadScroll(TouchPadScrollEventArgs& args);
        void touchPadScrollEnd(TouchPadScrollEventArgs& args);
        void touchPadScrollInertia(TouchPadScrollEventArgs& args);
#endif
	};

	extern ViewManagerBridge PRIVATE;
    
    extern bool useTouch;
    
    
    static void EnableTouch() {
        useTouch = true;
        VUI::PRIVATE.EnableTouchEvents();
    }
    
    
    
    static void ListenToWindowEvents( ofCoreEvents & events, EM* eventManager, int windowW, int windowH ){
        eventManager->vw = windowW;
        eventManager->vh = windowH;
        VUI::PRIVATE.ListenToWindow(events, eventManager);
        currEventManager = eventManager;
    }
    
    static void ClearWindowEventRef(EM* eventManager = NULL){
        VUI::PRIVATE.ClearWindowEventRef(eventManager);
    }
    
    static void ListenToMainWindowEvents(bool disableCurrent = false){
        VUI::PRIVATE.ClearWindowEventRef(disableCurrent);
        VUI::PRIVATE.ListenToMainWindow();
    }
    
    static bool IsTouchEnabled() {
        return useTouch;
    }

	template<class ListenerClass>
	static void RegisterMouseEvents(ListenerClass * listener, int prio = OF_EVENT_ORDER_AFTER_APP) {
		ofAddListener(PRIVATE.onMouseDragged, listener, &ListenerClass::mouseDragged, prio);
		ofAddListener(PRIVATE.onMouseMoved, listener, &ListenerClass::mouseMoved, prio);
		ofAddListener(PRIVATE.onMousePressed, listener, &ListenerClass::mousePressed, prio);
		ofAddListener(PRIVATE.onMouseReleased, listener, &ListenerClass::mouseReleased, prio);
		ofAddListener(PRIVATE.onMouseScrolled, listener, &ListenerClass::mouseScrolled, prio);
		ofAddListener(PRIVATE.onMouseEntered, listener, &ListenerClass::mouseEntered, prio);
		ofAddListener(PRIVATE.onMouseExited, listener, &ListenerClass::mouseExited, prio);
	}

	static void DisableMouseEvents() {
		PRIVATE.DisableMouseEvents();
	}

	static int GetWidth(bool scaled = true) {
		if (scaled) return vw * vscale;
		return vw;
	}
    
    static int GetWindowWidth( bool scaled = true) {
        float s = scaled == true ? vscale : 1.0;
        
        if ( VUI::viewRotation == VUI_ROTATE_90_CCW ) return vh * s;
        return vw*s;
    }
    
    static int GetWindowHeight( bool scaled = true ){
        float s = scaled == true ? vscale : 1.0;
        
        if ( VUI::viewRotation == VUI_ROTATE_90_CCW ) return vw * s;
        return vh*s;
    }

	static int GetHeight(bool scaled = true) {
		if (scaled) return vh * vscale;
		return vh;
	}

	static int GetTrueWidth() {
		return vw;
	}

	static int GetTrueHeight() {
		return vh;
	}

	static int GetScaledWidth() {
		return vw * vscale;
	}

	static int GetScaledHeight() {
		return vh * vscale;
	}
    
    static int GetResolutionX(){
        return vw*VUI::divideDpi;
    }
    
    static int GetResolutionY(){
        return vh*VUI::divideDpi;
    }
    
    static float GetScale(bool inverse = false){
        if ( inverse ) return 1/vscale;
        return vscale;
    }

	static bool GetTouchDown() {
		if (touches.size() > 0) return true;
		else return false;
	}
    
    static void _PrivateRotateUI(){
        return;
        if ( VUI::uiRotation == VUI_ROTATE_90_CCW ) {
            ofTranslate(0, GetHeight(false) );
            ofRotate( -90 );
        }
    }
    
    /*static void RotateUI(Rotate rotation){
        uiRotation = rotation;
    }*/
    
    static void RotateView(Rotate rotation){
        //SetResolution( GetHeight(false), GetWidth(false), GetScale() );
        viewRotation = rotation;
        uiRotation = rotation;
        //RotateUI( rotation );
    }

	/*static void Update() {
	if (currView.empty() || views[currView] == nullptr) return;
	views[currView]->Update();
	}*/

	//map<string,View*> visibleViews;
	extern vector<View*> visibleViews;
    
    extern float backgroundOpacity;
    extern ofColor backgroundColor;
    
    static void SetBackgroundOpacity( float opacity ){
        backgroundOpacity = opacity;
    }
    
    static void SetBackgroundColor( ofColor& color ){
        backgroundColor = color;
    }
    
    static void SetBackground( ofColor& color, float opacity = 1.0 ){
        SetBackgroundColor( color );
        SetBackgroundOpacity( opacity );
    }
    
    static void SetBackground( int r, int g, int b, float opacity ){
        backgroundColor.set( r, g, b );
        SetBackgroundOpacity( opacity );
    }
    
    static void SetBackground( int gray, float opacity = 1.0 ){
        backgroundColor.set( gray, gray, gray );
        SetBackgroundOpacity( opacity );
    }
    
    static void RenderBegin(bool drawingInsideFbo = false, int x = 0, int y = 0, int width = -1, int height = -1) {
        if ( useViewManager ){
            if (currView.empty() || views[currView] == nullptr) return;
        }
        
        PRIVATE.Init();
        
        if ( VUI::vscale != 1 ){
            fbo.begin();
            ofClear(0);
        }
        
        ofSetColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 255.0*backgroundOpacity);
        ofDrawRectangle(0, 0, vw, vh);
        ofSetColor(255, 255, 255, 255);
        
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, 255);
        if ( useViewManager ) views[currView]->Render();
        
        ofSetColor(255, 255, 255, 255);
        //ofLog() << visibleViews.size();
        
        if ( useViewManager ){
            for (vector<View*>::iterator it = visibleViews.begin(); it != visibleViews.end(); it++) {
                if ((*it) != nullptr) (*it)->Render();
            }
        }
    }
    
    static void RenderEnd(bool drawingInsideFbo = false, int x = 0, int y = 0, int width = -1, int height = -1) {
        
        if ( VUI::vscale != 1 ){
            fbo.end();
        }
        
        if ( useViewManager ){
            if (currView.empty() || views[currView] == nullptr ) return;
        }
        
        ofSetColor(255, 255, 255, 255);
        ofEnableAlphaBlending();
        
        //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        float w = vw*vscale;
        float h = vh*vscale;
        
        if ( width != -1 && height != -1 ) {
            w = width;
            h = height;
        }
        
        if ( drawingInsideFbo ) glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        if ( viewRotation == VUI_ROTATE_90_CCW ){
            ofTranslate(0, GetWidth() );
            ofRotate( -90 );
        }
        
        fbo.draw(x, y, w, h);
    }

	static void Render(bool drawingInsideFbo = false, int x = 0, int y = 0, int width = -1, int height = -1) {
        RenderBegin( drawingInsideFbo, x, y, width, height );
        RenderEnd( drawingInsideFbo, x, y, width, height );
	}


	static void ShowView(string name) {
		//if ( views[name] != nullptr ) visibleViews[ name ] = views[ name ];

		if (views[name] != nullptr) {
			if (std::find(visibleViews.begin(), visibleViews.end(), views[name]) != visibleViews.end()) {

			}
			else {
				visibleViews.push_back(views[name]);
			}

		}
	}

	extern vector<string> hideViews;

	static void HideView(string name) {
		/*map<string, View*>::iterator it;
		it = visibleViews.find(name);
		if (it != visibleViews.end() ) visibleViews.erase(it);*/

		//visibleViews.clear();
		hideViews.push_back(name);
		return;


	}
	static void ActualHideView(string name) {
		int index = 0;
		for (vector<View*>::iterator it = visibleViews.begin(); it != visibleViews.end(); it++) {
			if ((*it) == views[name]) {
				visibleViews.erase(visibleViews.begin() + index);
				return;
			}
			index++;
		}
	}

	/*static void StartView(string name) {
	currView = nextView;
	views[currView]->Start();

	nextView.clear();
	}*/

	static void Next() {
		if (nextView.empty()) return;

		VUI::EventManager.Disable();

		//if (!currView.empty()) views[currView]->OnExitView();

		PRIVATE.StartView(nextView);
	}

    extern int doubleClickThreshold;
	extern int touchTapThreshold;
    
    static void SetDoubleClickThreshold( int milliseconds = 400 ){
        doubleClickThreshold = milliseconds;
    }

	static void SetTouchTapThreshold(int milliseconds = 180) {
		touchTapThreshold = milliseconds;
	}

	static void SetView(string name, bool triggerBeforeExitView = false ) {
		if (currView == name) return;

		if (!isListening) PRIVATE.Listen();

		if (views[name] == nullptr) return;
		nextView = name;

        if ( !currView.empty() && triggerBeforeExitView ) views[currView]->BeforeExitView();
        else Next();
	}
    
    extern bool _didInit;
    extern StyleSheet* _vuiStyleSheet;
    static void Init(){
        if ( _didInit ) return;
        _didInit = true;
        
        string styles = R"(
            [.Container-container>
                 height: 100%;
                 width: 100%;
             ]
        
            [.Container-vertContainer>
                 height: 100%;
                 width: 100%;
             ]
        
            [.Container-horzContainer>
                 width: 100%;
             ]
        
            [.Container-scrollbar>
             bgColor: #000000;
             bgOpacity: .85;
             width: 10;
             height: 30;
             ]
        )";
        
        //
        
        VUI::_vuiStyleSheet = new StyleSheet( styles, "VUIStyleSheet" );
        
        VUI::EventManager.name = "og";
        VUI::EventManager.Init(ofGetWidth(),ofGetHeight());
        VUI::TweenManager.Init();
    };
    
    static void AddView(string name, View *view, bool setView = false ) {
        VUI::Init();
        
        views[name] = view;
        if ( setView ) SetView( name );
    };

	static View* GetCurrentView() {
		return views[currView];
	}

	static string GetViewName() {
		return currView;
	}
    
    


}

#endif

