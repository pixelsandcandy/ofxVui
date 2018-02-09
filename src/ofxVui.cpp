#include "ofxVui.h"
#include "VUIElement.h"
#include "VUIView.h"

namespace VUI {

	map< string, View*> views;

	map< int, ofPoint> touches;
    map<string, StyleSheet*> styleSheets;

	string nextView;
	string currView;

	ofVec2f multCoords(1.0, 1.0);

	ViewManagerBridge PRIVATE;

	bool isListening = false;

	int mouseX;
	int mouseY;

	int vw = -1;
	int vh = -1;
	float vscale = 1.0;
	ofFbo fbo;

	vector<View*> visibleViews;
	vector<string> hideViews;
    
    float backgroundOpacity = 0.0;
    ofColor backgroundColor = ofColor(255);

	/*void ViewManagerBridge::update(ofEventArgs &e) {
	cout << "ViewManagerBridge - update - " << ofRandomf() << endl;
	if ( !VUI::currView.empty() ) VUI::GetCurrentView()->update(e);
	}*/

	void ViewManagerBridge::StartView(string name) {
		//ofLog() << "name:" << name << "  curr:" << VUI::currView << "  next:" << VUI::nextView;
		/*if (name = VUI::nextView) {
		VUI::nextView.clear();
		return;
		}*/

		VUI::currView = VUI::nextView;

        if (!VUI::currView.empty() && VUI::views[VUI::currView] != nullptr) {
            VUI::views[VUI::currView]->_Setup();
            VUI::views[VUI::currView]->OnEnterView();
        }


        if (VUI::IsTouchEnabled()) {
            if (!VUI::GetTouchDown()) VUI::GetCurrentEventManager()->Enable();
        }
        else {
            if (!ofGetMousePressed()) VUI::GetCurrentEventManager()->Enable();
        }

		//ofLog() << "mousePressed: " << ofGetMousePressed();

		VUI::nextView.clear();
	}

	void ViewManagerBridge::setup(ofEventArgs & args) {
		//update();

		VUI::mouseX = ofGetMouseX()*multCoords.x;
		VUI::mouseY = ofGetMouseY()*multCoords.y;

		if (!VUI::currView.empty()) VUI::GetCurrentView()->setup();
	}

	void ViewManagerBridge::update(ofEventArgs & args) {
		//update();
		//ofLog() << "ViewManagerBridge::update - " << ofGetMouseX() << "x" << ofGetMouseY() << "   - " << ofRandomf() << endl;



		if (!VUI::currView.empty()) VUI::GetCurrentView()->update();

		for (vector<string>::iterator it = hideViews.begin(); it != hideViews.end(); it++) {
			VUI::ActualHideView((*it));
		}

		hideViews.clear();
	}
	void ViewManagerBridge::draw(ofEventArgs & args) {
		//draw();
		//ofLog() << "ViewManagerBridge::draw - " << ofRandomf() << endl;
		if (!VUI::currView.empty()) VUI::GetCurrentView()->draw();
	}

	void ViewManagerBridge::windowResized(ofResizeEventArgs & resize) {
		//windowResized(resize.width, resize.height);
        VUI::vw = resize.width;
        VUI::vh = resize.height;
        
        VUI::GetCurrentEventManager()->vw = resize.width;
        VUI::GetCurrentEventManager()->vh = resize.height;
        
		if (!VUI::currView.empty()) VUI::GetCurrentView()->windowResized(resize.width, resize.height);
	}

	void ViewManagerBridge::keyPressed(ofKeyEventArgs & key) {
		//keyPressed(key.key);
		if (!VUI::currView.empty()) VUI::GetCurrentView()->keyPressed(key.key);
	}
	void ViewManagerBridge::keyReleased(ofKeyEventArgs & key) {
		//keyReleased(key.key);
		if (!VUI::currView.empty()) VUI::GetCurrentView()->keyReleased(key.key);
	}






	void ViewManagerBridge::mouseMoved(ofMouseEventArgs & mouse) {
		//mouseX = mouse.x;
		//mouseY = mouse.y;
		//mouseMoved(mouse.x, mouse.y);
		//
		VUI::mouseX = mouse.x*multCoords.x;
		VUI::mouseY = mouse.y*multCoords.y;
        
        
        
		if (!VUI::currView.empty()) {
			/*#ifdef USING_ofxSupervui
             if ( !VUI::EventManager.active ) return;
			#endif*/
            
            //cout << VUI::mouseX << "x" << VUI::mouseY << endl;
            
            if ( mouse.x < 0 || mouse.y < 0 ) VUI::GetCurrentView()->_SetIsMouseInside(false);
            else if (mouse.x > VUI::GetWindowWidth() || mouse.y > VUI::GetWindowHeight() ) VUI::GetCurrentView()->_SetIsMouseInside(false);
            else VUI::GetCurrentView()->_SetIsMouseInside(true);
            
			ofMouseEventArgs args(ofMouseEventArgs::Moved, VUI::mouseX, VUI::mouseY);
			ofNotifyEvent(onMouseMoved, args, this);

			VUI::GetCurrentView()->UpdateMousePos(VUI::mouseX, VUI::mouseY);
			VUI::GetCurrentView()->mouseMoved(VUI::mouseX, VUI::mouseY);
		}
	}
	void ViewManagerBridge::mouseDragged(ofMouseEventArgs & mouse) {
		//mouseX = mouse.x;
		//mouseY = mouse.y;
		//mouseDragged(mouse.x, mouse.y, mouse.button);
		VUI::mouseX = mouse.x*multCoords.x;
		VUI::mouseY = mouse.y*multCoords.y;

		if (!VUI::currView.empty()) {
			if (!VUI::GetCurrentEventManager()->active) return;

			ofMouseEventArgs args(ofMouseEventArgs::Dragged, VUI::mouseX, VUI::mouseY, mouse.button);
			ofNotifyEvent(onMouseDragged, args, this);

			VUI::GetCurrentView()->UpdateMousePos(VUI::mouseX, VUI::mouseY);
			VUI::GetCurrentView()->mouseDragged(VUI::mouseX, VUI::mouseY, mouse.button);
		}
	}
	void ViewManagerBridge::mousePressed(ofMouseEventArgs & mouse) {
		//mouseX = mouse.x;
		//mouseY = mouse.y;
		//mousePressed(mouse.x, mouse.y, mouse.button);

		VUI::mouseX = mouse.x*multCoords.x;
		VUI::mouseY = mouse.y*multCoords.y;

		if (!VUI::currView.empty()) {
			if (!VUI::GetCurrentEventManager()->active) return;
            
            VUI::GetCurrentView()->_SetIsMouseInside(true);

			ofMouseEventArgs args(ofMouseEventArgs::Pressed, VUI::mouseX, VUI::mouseY, mouse.button);
			ofNotifyEvent(onMousePressed, args, this);

			VUI::GetCurrentView()->UpdateMousePos(VUI::mouseX, VUI::mouseY);
			VUI::GetCurrentView()->mousePressed(VUI::mouseX, VUI::mouseY, mouse.button);
		}
	}
    
	void ViewManagerBridge::mouseReleased(ofMouseEventArgs & mouse) {
		//mouseX = mouse.x;
		//mouseY = mouse.y;
		//mouseReleased(mouse.x, mouse.y, mouse.button);
		VUI::mouseX = mouse.x*multCoords.x;
		VUI::mouseY = mouse.y*multCoords.y;

		if (!VUI::currView.empty()) {

			if (!VUI::GetCurrentEventManager()->active) {
				if (VUI::GetCurrentEventManager()->enableOnMouseUp) VUI::GetCurrentEventManager()->DelayEnable(50);
				return;
			}

			ofMouseEventArgs args(ofMouseEventArgs::Released, VUI::mouseX, VUI::mouseY, mouse.button);
			ofNotifyEvent(onMouseReleased, args, this);

			VUI::GetCurrentView()->UpdateMousePos(VUI::mouseX, VUI::mouseY);
			VUI::GetCurrentView()->mouseReleased(VUI::mouseX, VUI::mouseY, mouse.button);
		}

		if (VUI::GetCurrentEventManager()->enableOnMouseUp) VUI::GetCurrentEventManager()->DelayEnable(50);

	}


	void ViewManagerBridge::mouseScrolled(ofMouseEventArgs & mouse) {
		//mouseScrolled(mouse.x, mouse.y, mouse.scrollX, mouse.scrollY);
		VUI::mouseX = mouse.x*multCoords.x;
		VUI::mouseY = mouse.y*multCoords.y;

		if (!VUI::currView.empty()) {
			if (!VUI::GetCurrentEventManager()->active) return;

			ofMouseEventArgs args(ofMouseEventArgs::Scrolled, VUI::mouseX, VUI::mouseY);
			args.scrollX = mouse.scrollX;
			args.scrollY = mouse.scrollY;
			ofNotifyEvent(onMouseScrolled, args, this);

			VUI::GetCurrentView()->mouseScrolled(VUI::mouseX, VUI::mouseY, mouse.scrollX, mouse.scrollY);
		}
	}

	void ViewManagerBridge::mouseEntered(ofMouseEventArgs & mouse) {
		//mouseEntered(mouse.x, mouse.y);
		VUI::mouseX = mouse.x*multCoords.x;
		VUI::mouseY = mouse.y*multCoords.y;

		if (!VUI::currView.empty()) {
			/*#ifdef USING_ofxSupervui
			if ( !VUI::EventManager.active ) return;
			#endif*/
            
            VUI::GetCurrentView()->_SetIsMouseInside(true);
            
			ofMouseEventArgs args(ofMouseEventArgs::Entered, VUI::mouseX, VUI::mouseY);
			ofNotifyEvent(onMouseEntered, args, this);
            
			VUI::GetCurrentView()->mouseEntered(VUI::mouseX, VUI::mouseY);
		}
	}
	void ViewManagerBridge::mouseExited(ofMouseEventArgs & mouse) {
		//mouseExited(mouse.x, mouse.y);
		VUI::mouseX = mouse.x*multCoords.x;
		VUI::mouseY = mouse.y*multCoords.y;

		if (!VUI::currView.empty()) {
			/*#ifdef USING_ofxSupervui
			if ( !VUI::EventManager.active ) return;
			#endif*/
            VUI::GetCurrentView()->_SetIsMouseInside(false);
            
			ofMouseEventArgs args(ofMouseEventArgs::Exited, VUI::mouseX, VUI::mouseY);
			ofNotifyEvent(onMouseExited, args, this);
            
			VUI::GetCurrentView()->mouseExited(VUI::mouseX, VUI::mouseY);
		}
	}
	void ViewManagerBridge::dragged(ofDragInfo & drag) {
		//dragEvent(drag);
		if (!VUI::currView.empty()) VUI::GetCurrentView()->dragEvent(drag);
	}
	void ViewManagerBridge::messageReceived(ofMessage & message) {
		//gotMessage(message);
		if (!VUI::currView.empty()) VUI::GetCurrentView()->gotMessage(message);
	}
	void ViewManagerBridge::touchDown(ofTouchEventArgs & touch) {
		//touchDown(touch.x, touch.y, touch.id);
		touches[touch.id].x = touch.x*multCoords.x;
		touches[touch.id].y = touch.y*multCoords.y;

		if (!VUI::GetCurrentEventManager()->active) return;

		if (!VUI::currView.empty()) VUI::GetCurrentView()->touchDown(touch.x*multCoords.x, touch.y*multCoords.y, touch.id);
	};
	void ViewManagerBridge::touchMoved(ofTouchEventArgs & touch) {
		//touchMoved(touch.x, touch.y, touch.id);
		touches[touch.id].x = touch.x*multCoords.x;
		touches[touch.id].y = touch.y*multCoords.y;

		if (!VUI::GetCurrentEventManager()->active) return;

		if (!VUI::currView.empty()) VUI::GetCurrentView()->touchMoved(touch.x*multCoords.x, touch.y*multCoords.y, touch.id);
	};
	void ViewManagerBridge::touchUp(ofTouchEventArgs & touch) {
		//touchUp(touch.x, touch.y, touch.id);
		map<int, ofPoint>::iterator it;
		it = touches.find(touch.id);
		if (it != touches.end()) touches.erase(it);


		if (!VUI::GetCurrentEventManager()->active) {
			if (VUI::GetCurrentEventManager()->enableOnMouseUp) VUI::GetCurrentEventManager()->DelayEnable(50);
			return;
		}

		if (!VUI::currView.empty()) VUI::GetCurrentView()->touchUp(touch.x*multCoords.x, touch.y*multCoords.y, touch.id);
	};
	void ViewManagerBridge::touchDoubleTap(ofTouchEventArgs & touch) {
		//touchDoubleTap(touch.x, touch.y, touch.id);
		if (!VUI::currView.empty()) VUI::GetCurrentView()->touchDoubleTap(touch.x*multCoords.x, touch.y*multCoords.y, touch.id);
	};
	void ViewManagerBridge::touchCancelled(ofTouchEventArgs & touch) {
		//touchCancelled(touch.x, touch.y, touch.id);
		if (!VUI::currView.empty()) VUI::GetCurrentView()->touchCancelled(touch.x*multCoords.x, touch.y*multCoords.y, touch.id);
	}
}



namespace VUI {
    Rotate uiRotation = VUI_ROTATE_NONE;
    Rotate viewRotation = VUI_ROTATE_NONE;
    
    bool useViewManager = true;
    
    map<string, map<int, map<float,ofTrueTypeFont*>>> fonts;
    int fontSize = 16;
    
    bool useTouch = false;
    bool _didInit = false;
    
    int doubleClickThreshold = 400;
	int touchTapThreshold = 180;
    
    // Tween
    
    void Tween::Start( Element* el, float timeSeconds, string params ){
        firstStep = true;
        active = true;
        this->el = el;
        duration = timeSeconds*1000;
        
        string s(params);
        ofStringReplace(s, "{", " ");
        ofStringReplace(s, "}", " ");
        ofStringReplace(s, " ", "");
        
        cmd = s;
        
        vector<string> split = ofSplitString(s, "," );
        for ( vector<string>::iterator it = split.begin(); it != split.end(); it++){
            //ofLog() << (*it);
            
            vector<string> propVal = ofSplitString( (*it), ":" );
            if ( propVal.size() == 2 ){
                //ofLog() << propVal[0] << " => " << propVal[1];
                
                if ( propVal[0] == "rotation" || propVal[0] == "x" || propVal[0] == "y" || propVal[0] == "width" || propVal[0] == "height" || propVal[0] == "opacity" || propVal[0] == "scale" ){
                    StoreValue( propVal[0], propVal[1] );
                } else if ( propVal[0] == "delay" ){
                    delay = ofToFloat( propVal[1] );
                } else if ( propVal[0] == "easing" || propVal[0] == "ease" ){
                    string e = ofToLower(propVal[1]);
                    
                    if ( e == "back.easein" ) ease = ofxeasing::back::easeIn;
                    else if ( e == "back.easeout" ) ease = ofxeasing::back::easeOut;
                    else if ( e == "back.easeinout" ) ease = ofxeasing::back::easeInOut;
                    else if ( e == "bounce.easein" ) ease = ofxeasing::bounce::easeIn;
                    else if ( e == "bounce.easeout" ) ease = ofxeasing::bounce::easeOut;
                    else if ( e == "bounce.easeinout" ) ease = ofxeasing::bounce::easeInOut;
                    else if ( e == "circ.easein" ) ease = ofxeasing::circ::easeIn;
                    else if ( e == "circ.easeout" ) ease = ofxeasing::circ::easeOut;
                    else if ( e == "circ.easeinout" ) ease = ofxeasing::circ::easeInOut;
                    else if ( e == "cubic.easein" ) ease = ofxeasing::cubic::easeIn;
                    else if ( e == "cubic.easeout" ) ease = ofxeasing::cubic::easeOut;
                    else if ( e == "cubic.easeinout" ) ease = ofxeasing::cubic::easeInOut;
                    else if ( e == "elastic.easein" ) ease = ofxeasing::elastic::easeIn;
                    else if ( e == "elastic.easeout" ) ease = ofxeasing::elastic::easeOut;
                    else if ( e == "elastic.easeinout" ) ease = ofxeasing::elastic::easeInOut;
                    else if ( e == "exp.easein" ) ease = ofxeasing::exp::easeIn;
                    else if ( e == "exp.easeout" ) ease = ofxeasing::exp::easeOut;
                    else if ( e == "exp.easeinout" ) ease = ofxeasing::exp::easeInOut;
                    else if ( e == "linear.easein" ) ease = ofxeasing::linear::easeIn;
                    else if ( e == "linear.easeout" ) ease = ofxeasing::linear::easeOut;
                    else if ( e == "linear.easeinout" ) ease = ofxeasing::linear::easeInOut;
                    else if ( e == "linear.easenone" ) ease = ofxeasing::linear::easeNone;
                    else if ( e == "quad.easein" ) ease = ofxeasing::quad::easeIn;
                    else if ( e == "quad.easeout" ) ease = ofxeasing::quad::easeOut;
                    else if ( e == "quad.easeinout" ) ease = ofxeasing::quad::easeInOut;
                    
				}
				else if (propVal[0] == "id") {
					id = propVal[1];
				}
                
            }
        }
        
        
        startTime = ofGetElapsedTimeMillis() + (delay*1000);
        endTime = startTime + duration;
    }
    
    void Tween::StoreStartValues(){
        for ( vector<string>::iterator it = valueNames.begin(); it != valueNames.end(); it++ ){
            if ( (*it) == "x" ){
                startValues[ (*it) ] = el->getPosition().x;
            } else if ( (*it) == "y" ){
                startValues[ (*it) ] = el->getPosition().y;
            } else if ( (*it) == "width" ){
                //ofLog() << "width:" << el->GetWidth();
                startValues[ (*it) ] = el->GetWidth(false);
            } else if ( (*it) == "height" ){
                //ofLog() << "height:" << el->GetHeight();
                startValues[ (*it) ] = el->GetHeight(false);
            } else if ( (*it) == "rotation" ){
                startValues[ (*it) ] = el->GetRotation();
            } else if ( (*it) == "opacity" ){
                startValues[ (*it) ] = el->GetOpacity();
            } else if ( (*it) == "scale" ){
                //ofLog() << "scale:" << el->GetScale();
                startValues[ (*it) ] = el->GetScale();
            }
        }
    }
    
    void Tween::StoreValue( string param, string val ){
        endValues[ param ] = ofToFloat( val );
        valueNames.push_back( param );
    }
    
    void Tween::Stop(){
        active = false;
    }
    
    
    void Tween::Update(float currTime){
        if ( !active ) return;
        
        if ( currTime >= startTime && currTime < endTime ){
            
            
            if ( firstStep ){
                firstStep = false;
                StoreStartValues();
                
                vuiEventArgs args;
                args.element = el;
                args.tween = this;
                args.eventType = VUI_EVENT_ANIMATE_START;
                
                ofNotifyEvent( onStart, args, this );
                
                //ofLog() << cmd;
            }
            
            perc = (currTime - startTime) / duration;
            UpdateValues();
            
            vuiEventArgs args;
            args.element = el;
            args.tween = this;
            args.eventType = VUI_EVENT_ANIMATE_STEP;
            
            ofNotifyEvent( onStep, args, this );
        } else if ( currTime >= endTime ) {
            active = false;
            
            perc = 1;
            UpdateValues();
            
            vuiEventArgs args2;
            args2.element = el;
            args2.tween = this;
            args2.eventType = VUI_EVENT_ANIMATE_STEP;
            
            ofNotifyEvent( onStep, args2, this );
            
            
            vuiEventArgs args;
			args.element = el;
			args.tween = this;
            args.eventType = VUI_EVENT_ANIMATE_COMPLETE;
            
            ofNotifyEvent( onComplete, args, this );
            
            VUI::ShouldDestroyTween(this);
        }
    }
    
    void Tween::UpdateValues(){
        
        for ( vector<string>::iterator it = valueNames.begin(); it != valueNames.end(); it++ ){
            if ( (*it) == "x" ){
                //x = el->GetPosition().x;
                x = ofxeasing::map(perc, 0.0, 1.0, startValues[(*it)], endValues[(*it)], ease);
                el->SetPositionX( x );
            } else if ( (*it) == "y" ){
                y = ofxeasing::map(perc, 0.0, 1.0, startValues[(*it)], endValues[(*it)], ease);
                el->SetPositionY( y );
            } else if ( (*it) == "width" ){
                width = ofxeasing::map(perc, 0.0, 1.0, startValues[(*it)], endValues[(*it)], ease);
                el->SetWidth( width );
            } else if ( (*it) == "height" ){
                height = ofxeasing::map(perc, 0.0, 1.0, startValues[(*it)], endValues[(*it)], ease);
                el->SetHeight( height );
            } else if ( (*it) == "rotation" ){
                rotation = ofxeasing::map(perc, 0.0, 1.0, startValues[(*it)], endValues[(*it)], ease);
                el->SetRotation( rotation );
            } else if ( (*it) == "opacity" ){
                opacity = ofxeasing::map(perc, 0.0, 1.0, startValues[(*it)], endValues[(*it)], ease);
                //if ( el->GetName() == "#ShaveOver" ) ofLog() << opacity;
                
                el->SetOpacity( ofClamp(opacity, 0, 1) );
                
            } else if ( (*it) == "scale" ){
                scale = ofxeasing::map(perc, 0.0, 1.0, startValues[(*it)], endValues[(*it)], ease);
                el->SetScale( scale );
            }
        }
        
    }
    
    
    // EventManager
    EM EventManager;
    EM* currEventManager = NULL;
    
    vector<Tween*> tweensToDestroy;
    vector<Tween*> tweens;

    void EM::StoreEvent(Element* el, vuiEvent eventType ){
		//ofLog() << "StoreEvent[" << eventType << "]  active:" << active;
        if ( !active ) return;
        
        this->events[ eventType ].push_back( el );
    }
    
    void EM::StoreOverElement( Element *el){
        overElement = el;
    }
    
    void EM::StoreState( Element *el, State state ){
        if ( !active ) return;
        this->states[ state ].push_back( el );
    }
    
    void EM::Purge(){
        for( vector<vuiEvent>::iterator it = VUI::evtlist.begin(); it != VUI::evtlist.end(); it++ ){
            this->events[ (*it) ].clear();
        }
        
        for( vector<State>::iterator it = statelist.begin(); it != statelist.end(); it++ ){
            this->states[ (*it) ].clear();
        }
        
        VUI::ClearOverElement();
        
        //if ( EventManager.overElement != nullptr ) ofLog() << EventManager.overElement->vuiUID;
    }

	void EM::Disable() {
		//ofLog() << "EventManager::Disable" << endl;
		active = false;
        
		Purge();
		if (ofGetMousePressed()|| VUI::GetTouchDown()) enableOnMouseUp = true;
	};
    
    void EM::Enable(){
        //ofLog() << "EventManager::Enable" << endl;
        active = true;
        enableOnMouseUp = false;
        shouldEnable = -1;
    }

    TM TweenManager;
    
    void TM::Init(){
        ofAddListener(ofEvents().update, this, &TM::update);
    }
    
    void TM::update(ofEventArgs& args){
        float currTime = ofGetElapsedTimeMillis();
        
        //ofLog() << "# tweens: " << tweens.size();
        
        for (vector<Tween*>::iterator it = tweens.begin(); it != tweens.end(); it++){
            (*it)->Update( currTime );
        }
        
        for (vector<Tween*>::iterator it = tweensToDestroy.begin(); it !=  tweensToDestroy.end(); it++){
            VUI::DestroyTween( (*it) );
        }
        
        tweensToDestroy.clear();
    }
    
    void EM::Init(int windowW, int windowH){
        ofAddListener(ofEvents().update, this, &EM::update);
        vw = windowW;
        vh = windowH;
        VUI::PRIVATE.Listen();
    }
    
    void EM::update(ofEventArgs & args){
        
        if ( shouldEnable != -1 ) {
            if ( ofGetElapsedTimeMillis() > shouldEnable ) Enable();
        }
        
        if ( !active ) {
            Purge();
            return;
        }
        
        //ofLog() << "EMBridge::Update - " << ofRandomf();
        //if ( EventHasElement(VUI_EVENT_MOUSE_CLICK) ) ofLog() << GetLatestElement(VUI_EVENT_MOUSE_CLICK)->GetName();
        for( vector<vuiEvent>::iterator it = evtlist.begin(); it != evtlist.end(); it++ ){
            if ( EventHasElement((*it)) ) GetLatestElement((*it))->TriggerEvent((*it));
        }
        
        for( vector<State>::iterator it = statelist.begin(); it != statelist.end(); it++ ){
            Element* last = nullptr;
            for ( vector<Element*>::iterator eit = states[(*it)].begin(); eit != states[(*it)].end(); eit++){
                //if ( (*eit) == EventManager.overElement ) (*eit)->SetState( (*it) );
                if ( (*eit) == overElement ) (*eit)->SetState( (*it) );
                //ofLog() << "[" << (*it) << "]" << (*eit)->GetName();
            }
            
        }
        
        //if ( EventManager.overElement != nullptr ) EventManager.overElement->SetState( VUI_STATE_OVER );
        
        Purge();
        
    }
    
}
