#pragma once

#ifndef __ofxVui__
#define __ofxVui__

#include <algorithm>

#include "ofMain.h"
#include "ofxEasing.h"
#include "xView.h"

#ifdef USING_ofxWinTouchHook
#include "ofxWinTouchHook.h"
#endif

namespace VUI {
	class Element;

	enum State {
		GUI_STATE_UP,
		GUI_STATE_OVER,
		GUI_STATE_DOWN,
		GUI_STATE_ALL
	};

	enum Align {
		GUI_ALIGN_LEFT_TOP,
		GUI_ALIGN_LEFT_CENTER,
		GUI_ALIGN_LEFT_BOTTOM,
		GUI_ALIGN_RIGHT_TOP,
		GUI_ALIGN_RIGHT_CENTER,
		GUI_ALIGN_RIGHT_BOTTOM,
		GUI_ALIGN_CENTER_TOP,
		GUI_ALIGN_CENTER_CENTER,
		GUI_ALIGN_CENTER_BOTTOM
	};

	enum GuiEvent {
        GUI_EVENT_MOUSE_OVER,
        GUI_EVENT_MOUSE_OUT,
        GUI_EVENT_MOUSE_PRESSED,
        GUI_EVENT_MOUSE_MOVED,
        GUI_EVENT_MOUSE_DRAGGED,
        GUI_EVENT_MOUSE_RELEASED,
		GUI_EVENT_MOUSE_CLICK,
        GUI_EVENT_MOUSE_DOUBLE_CLICK,
		

		GUI_EVENT_TOUCH_DOWN,
		GUI_EVENT_TOUCH_UP,
		GUI_EVENT_TOUCH_TAP,
        GUI_EVENT_TOUCH_DOUBLE_TAP,
        
        GUI_EVENT_STATE_CHANGE,
        GUI_EVENT_SELECT_CHANGE,
        
        GUI_EVENT_FOCUS,
        GUI_EVENT_UNFOCUS,
        
        GUI_EVENT_ANIMATE_COMPLETE,
        GUI_EVENT_ANIMATE_STEP,
        GUI_EVENT_ANIMATE_START
	};

	

	extern ofPixels GUIGlobalPixels;
	extern ofImage GUIGlobalImage;
    extern map<string, map<int, ofTrueTypeFont*>> fonts;
    extern int fontSize;
    
    static void SetFontSize( int size ){
        fontSize = size;
    }
    
    static bool HasFont( string filename, int fontSize ){
        if ( fonts[filename][fontSize] != nullptr ) return true;
        return false;
    }
    
    static ofTrueTypeFont* GetFont( string filename, int fontSize){
        return fonts[filename][fontSize];
    }
    
    static ofTrueTypeFont* AddFont( string filename, int fontSize ){
        ofTrueTypeFont* tf = new ofTrueTypeFont();
        tf->load(filename, fontSize);
        
        fonts[filename][fontSize] = tf;
        return fonts[filename][fontSize];
    }

    extern bool useTouch;
    
    static void EnableTouch() {
        useTouch = true;
    }
    
    static bool TouchEnabled() {
        return useTouch;
    }
    
    struct guiEventArgs;
    
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
        
        ofEvent<guiEventArgs> onComplete;
        ofEvent<guiEventArgs> onStart;
        ofEvent<guiEventArgs> onStep;
        
        int UID = abs((int)ofRandom(7, 7777777777));
        
        float startTime;
        float endTime;
        float duration;
        float perc = 0.0;
        
        float GetPercentCompleted(){
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
        
        
        template <typename ArgumentsType, class ListenerClass>
        void StoreComplete( ListenerClass* listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
            
        }
        
        
    };
    
    struct guiEventArgs {
        Element* element;
        int eventType;
        ofVec2f localPos;
        ofVec2f globalPos;
        ofVec2f deltaDrag;
        Tween* tween;
    };
    
    extern vector<Tween*> tweens;
    
    static Tween* Animate( Element* el, float timeSeconds, string params ){
        Tween *t = new Tween();
        t->Start( el, timeSeconds, params );
        
        tweens.push_back( t );
        return t;
    }
    
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
    
    extern map<GuiEvent, vector<Element*>> events;
    extern map<State, vector<Element*>> states;
    
    
    class EMBridge {
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
        
        bool EventHasElement( GuiEvent eventType ){
            return !VUI::events[eventType].empty();
        }
        
        Element* GetLatestElement( GuiEvent eventType ){
            return VUI::events[eventType].back();
        }
        
        void update(ofEventArgs & args);
        
    };
    
    //
    
    static vector<GuiEvent> evtlist = {GUI_EVENT_MOUSE_PRESSED,GUI_EVENT_MOUSE_RELEASED,GUI_EVENT_MOUSE_CLICK};
    //static vector<State> statelist = {GUI_STATE_UP,GUI_STATE_OVER,GUI_STATE_DOWN};
    static vector<State> statelist = {GUI_STATE_UP,GUI_STATE_OVER,GUI_STATE_DOWN};
    
    class EM {
    public:
        ~EM(){};
        EM(){};
        
        Element* overElement = nullptr;
        
        void StoreOverElement( Element *el){
            overElement = el;
        }
		
        
        bool active = true;
        bool enableOnMouseUp = false;
        
        void StoreEvent( Element *el, GuiEvent eventType );
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
    };
    
    extern EM EventManager;
    extern EMBridge PRIVATE_EM;
    

	static Element* GetOverElement() {
		return EventManager.overElement;
	}

	static void ClearOverElement() {
		EventManager.overElement = nullptr;
	}

	void Setup(bool touchEvents = false);

}


#include "xStyleSheet.h"
#include "xElement.h"
#include "xTextField.h"


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
    
    static void SetResolution(int w, int h, float scale = 1.0, bool usingVuiRender = true ) {
        if ( usingVuiRender ){
            vw = w;
            vh = h;
            
            ScaleView(scale);
        } else {
            vw = w * scale;
            vh = h * scale;
            ScaleView(1.0);
        }
        
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
				VUI::fbo.allocate(VUI::vw, VUI::vh, GL_RGBA);
				VUI::fbo.begin();
				ofClear(0);
				VUI::fbo.end();
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


		void Listen() {
            if ( VUI::isListening ) return;
            VUI::isListening = true;
            
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
			ofAddListener(ofEvents().messageEvent, this, &ViewManagerBridge::messageReceived);

			ofAddListener(ofEvents().fileDragEvent, this, &ViewManagerBridge::dragged);

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
	};

	extern ViewManagerBridge PRIVATE;

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

	static bool GetTouchDown() {
		if (touches.size() > 0) return true;
		else return false;
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

	static void Render(bool drawingInsideFbo = false, int x = 0, int y = 0, int width = -1, int height = -1) {
		//ofLog() << "Render - " << ofRandomf();

		PRIVATE.Init();
        ofEnableAlphaBlending();
		ofSetColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 255.0*backgroundOpacity);
		ofDrawRectangle(0, 0, vw, vh);
        ofSetColor(255, 255, 255, 255);

		if (currView.empty() || views[currView] == nullptr) return;

        fbo.begin();
        ofClear(0);

        ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, 255);
		views[currView]->Render();

		ofSetColor(255, 255, 255, 255);
		//ofLog() << visibleViews.size();

		for (vector<View*>::iterator it = visibleViews.begin(); it != visibleViews.end(); it++) {
			if ((*it) != nullptr) (*it)->Render();
		}

        fbo.end();
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
        fbo.draw(x, y, w, h);
        
        /*if ( drawingInsideFbo ) {
            
            
        } else {
            if ( vscale != 1.0 ) fbo.draw(x, y+h, vw*vscale, -vh*vscale);
            else fbo.draw(x, y+h, w, -h);
        }*/
        
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

		if (!currView.empty()) views[currView]->Exit();

		PRIVATE.StartView(nextView);
	}

	

	

	static void SetView(string name, bool doTransition = true) {
		if (currView == name) return;

		if (!isListening) PRIVATE.Listen();

		if (views[name] == nullptr) return;
		nextView = name;

		if (doTransition && !currView.empty()) views[currView]->TransitionOut();
		else Next();
	}
    
    static void AddView(string name, View *view, bool setView = false ) {
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
