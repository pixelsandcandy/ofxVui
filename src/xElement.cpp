#include "ofxVui.h"
#include "xElement.h"
#include "xStyleSheet.h"

namespace VUI {
	Element::~Element() {
        VUI::Init();
    }

	Element::Element(int x, int y, StyleSheet *ss, string selector, string selectorB ) {
		VUI::Init();
        
		SetDefaultStyles(x, y);

		if (ss != nullptr) {
			SetStyleSheet(ss);
			if (selector != "") {
				if (selector.find(".") != string::npos) {
					UseStyleClass(selector.substr(1));
				}
				else if (selector.find("#") != string::npos) {
					UseStyleID(selector.substr(1));
				}
			}

			if (selectorB != "") {
				if (selectorB.find(".") != string::npos) {
					UseStyleClass(selectorB.substr(1));
				}
				else if (selectorB.find("#") != string::npos) {
					UseStyleID(selectorB.substr(1));
				}
			}
		}
	}

	void Element::SetDefaultStyles(int x, int y) {
		for (int i = 0; i < 3; i++) {
			//style[i]["width"] = "100";
			//style[i]["height"] = "100";
			style[i]["opacity"] = "1";
			style[i]["background-color"] = "#FF00FF";
		}

		this->setPosition(x, y, 0);

		ParseStyle();
	}

	void Element::UpdatePosition() {
		UpdateAnchorOffset();

        localMinPosition.x = getPosition().x;
        localMinPosition.y = getPosition().y;
        
		localMaxPosition.x = getPosition().x + (width*scale);
		localMaxPosition.y = getPosition().y + (height*scale);

		localMaxPosition.x += styleFloat[state]["offset-x"];
		localMinPosition.x += styleFloat[state]["offset-x"];

		localMaxPosition.y += styleFloat[state]["offset-y"];
		localMinPosition.y += styleFloat[state]["offset-y"];

		localMaxPosition.x += anchorOffset.x;
		localMinPosition.x += anchorOffset.x;

		localMaxPosition.y += anchorOffset.y;
		localMinPosition.y += anchorOffset.y;
        
        
        globalMinPosition.set( localMinPosition.x + parentSumOffset.x, localMinPosition.y + parentSumOffset.y );
        globalMaxPosition.set( localMaxPosition.x + parentSumOffset.x, localMaxPosition.y + parentSumOffset.y );
        
        drawPosition.x = localMinPosition.x - anchorOffset.x - parentSumOffset.x;
        drawPosition.y = localMinPosition.y - anchorOffset.y - parentSumOffset.y;
        
        if ( VUI::uiRotation == VUI_ROTATE_90_CCW ){
            ofVec2f savedLocal(localMinPosition);
            ofVec2f savedGlobalMin(globalMinPosition);
            
            localMinPosition.y = savedLocal.x;
            localMinPosition.x = savedLocal.y;
            
            globalMinPosition.x = savedGlobalMin.y;
            globalMinPosition.y = VUI::GetWindowHeight(false) - savedGlobalMin.x - (width*scale);
            
            globalMaxPosition.x = savedGlobalMin.y + (height*scale);
            globalMaxPosition.y = VUI::GetWindowHeight(false) - savedGlobalMin.x;
        }
            
		
	}

	void Element::Update(int mouseX, int mouseY, bool internalUpdate) {
        //ofLog() << "Update - " << VUI::mouseX << "x" << VUI::mouseY << "  - " << ofRandomf();
        
        
        if ( !internalUpdate ) userUpdating = true;
        
		UpdatePosition();
        
        if ( !isInteractive )return;
        
        

		if (mouseX != -1 && mouseY != -1) {
			if (mouseX > globalMinPosition.x && mouseX < globalMaxPosition.x) {
				if (mouseY > globalMinPosition.y && mouseY < globalMaxPosition.y) {
                    VUI::EventManager.StoreOverElement( this );
					if (ofGetMousePressed()) UpdateState(VUI_STATE_DOWN);
					else UpdateState(VUI_STATE_OVER);
					return;
				}
			}
			UpdateState(VUI_STATE_UP);
			return;
		}

/*#if defined(USING_ofxWinTouchHook)

		if (VUI::IsTouchEnabled()) {
			bool touchDownOnElement = false;
			if (VUI::GetOverElement() == this) touchDownOnElement = true;

			for (map<int, ofPoint>::iterator it = VUI::GetTouches()->begin(); it != VUI::GetTouches()->end(); it++){
				ofPoint p = (*it).second;
				//ofLog() << "[x] " << minPosition.x << " > " << p.x << " < " << maxPosition.x << "  [y] " << minPosition.y << " > " << p.x << " < " << maxPosition.y;

				if (p.x > globalMinPosition.x && p.x < globalMaxPosition.x) {
					if (p.y > globalMinPosition.y && p.y < globalMaxPosition.y) {
						//ofLog() << VUI::GetTouchDown() << " - " << ofGetMousePressed();
                        VUI::EventManager.StoreOverElement( this );
						if (VUI::GetTouchDown()) {
							UpdateState(VUI_STATE_DOWN);
						}
						else {
							UpdateState(VUI_STATE_OVER);
						}

						//ofLog() << "touchDown:" << VUI::GetTouchDown() << "  -" << ofRandomf();
						return;
					}
				}
			}

			if (touchDownOnElement && !isToggle) {
				TriggerEvent(VUI_EVENT_TOUCH_UP);
				SetState(VUI_STATE_UP);
				VUI::ClearOverElement();
			}
		}
		else {
			if (VUI::mouseX > globalMinPosition.x && VUI::mouseX < globalMaxPosition.x) {
				if (VUI::mouseY > globalMinPosition.y && VUI::mouseY < globalMaxPosition.y) {
					VUI::EventManager.StoreOverElement(this);
					if (ofGetMousePressed()) UpdateState(VUI_STATE_DOWN);
					else UpdateState(VUI_STATE_OVER);
					return;
				}
			}
		}

		


#else

		if (VUI::mouseX > globalMinPosition.x && VUI::mouseX < globalMaxPosition.x) {
			if (VUI::mouseY > globalMinPosition.y && VUI::mouseY < globalMaxPosition.y) {
                VUI::EventManager.StoreOverElement( this );
				if (ofGetMousePressed()) UpdateState(VUI_STATE_DOWN);
				else UpdateState(VUI_STATE_OVER);
				return;
			}
		}

#endif*/
        
        if (VUI::IsTouchEnabled()) {
            
            
            bool touchDownOnElement = false;
            if (VUI::GetOverElement() == this) touchDownOnElement = true;
            
            //ofLog() << "Update - " << VUI::GetTouches()->size() << "  - " << ofRandomf();
            
            for (map<int, ofPoint>::iterator it = VUI::GetTouches()->begin(); it != VUI::GetTouches()->end(); it++){
                ofPoint p = (*it).second;
                //ofLog() << "[x] " << globalMinPosition.x << " > " << p.x << " < " << globalMaxPosition.x << "  [y] " << globalMinPosition.y << " > " << p.x << " < " << globalMaxPosition.y;
                
                if (p.x > globalMinPosition.x && p.x < globalMaxPosition.x) {
                    if (p.y > globalMinPosition.y && p.y < globalMaxPosition.y) {
                        //ofLog() << VUI::GetTouchDown() << " - " << ofGetMousePressed();
                        VUI::EventManager.StoreOverElement( this );
                        if (VUI::GetTouchDown()) {
                            UpdateState(VUI_STATE_DOWN);
                        }
                        else {
                            UpdateState(VUI_STATE_OVER);
                        }
                        
                        //ofLog() << "touchDown:" << VUI::GetTouchDown() << "  -" << ofRandomf();
                        return;
                    }
                }
            }
            
            if (touchDownOnElement && !isToggle) {
                TriggerEvent(VUI_EVENT_TOUCH_UP);
                SetState(VUI_STATE_UP);
                VUI::ClearOverElement();
            }
        }
        else {
            if (VUI::mouseX > globalMinPosition.x && VUI::mouseX < globalMaxPosition.x) {
                if (VUI::mouseY > globalMinPosition.y && VUI::mouseY < globalMaxPosition.y) {
                    
                    if ( VUI::GetPrevOverElement() == this ){
                        if ( isMouseDown && mouseDownPos.x != VUI::mouseX && mouseDownPos.y != VUI::mouseY ) TriggerEvent( VUI_EVENT_MOUSE_DRAGGED );
                        else TriggerEvent( VUI_EVENT_MOUSE_MOVED );
                        
                        if (ofGetMousePressed()) {
                            if ( !isMouseDown ) {
                                isMouseDown = true;
                                mouseDownPos.set( VUI::mouseX, VUI::mouseY );
                                //ofLog() << VUI::mouseX << "," << VUI::mouseY;
                            }
                            
                            UpdateState(VUI_STATE_DOWN);
                        } else {
                            UpdateState(VUI_STATE_OVER);
                            isMouseDown = false;
                        }
                        
                        
                    } else {
                        UpdateState(VUI_STATE_UP);
                        isMouseDown = false;
                    }
                    
                    if ( VUI::GetOverElement() != this ) VUI::EventManager.StoreOverElement(this);
                    return;
                }
            }
        }

		UpdateState(VUI_STATE_UP);
        isMouseDown = false;
		
		
	}

	void Element::UpdateState(int toState) {
        //ofLog() << "UpdateState => " << _state;
        
        if ( !VUI::EventManager.IsActive() ) return;
        if ( !isInteractive ) return;
        
        int oldState(state);
        
		bool update = state != toState ? true : false;
        
        if (IsTouchEnabled()) {
            if ( !VUI::GetTouchDown() && !ofGetMousePressed() ) lastTimeMouseDown = 0;
        }
        else {
            if (!ofGetMousePressed()) lastTimeMouseDown = 0;
        }
        
        if ( DEBUG_MODE ) {
            ofLog() << "virtal:" << prevVirtualState << "(" << virtualState << ") > real:" << toState << "(" << state << ")";
            ofLog() << "update: " << update;
        }
        
        /*if ( name == "nameField" ){
            ofLog() << lastTimeMouseDown;
            ofLog() << "[" << vuiUID << "]  - " << "vstate: " << virtualState << "  state:" << state << "  _state: " << _state << "   update:" << update << "  - " << ofRandomf();
        }*/
        
        
        if ( !isToggle ){
            if ( !update ) return;
        } else {
            if ( toState == VUI_STATE_DOWN ){
                //ofLog() << ofGetElapsedTimeMillis() << " < " << lastTimeMouseDown;
                
                if ( ofGetElapsedTimeMillis() < lastTimeMouseDown ){
                    lastTimeMouseDown = ofGetElapsedTimeMillis() + 500;
                    //ofLog() << "return" << endl;
                    //if ( name == ".textfield" ) ofLog() << virtualState << " > " << _state << "     RETURN - " << ofRandomf();
                    return;
                } else {
                    
                }
            } else if (!update){
                
                return;
            }
        }
        
        //if ( name == ".textfield" ) ofLog() << virtualState << " > " << _state << "     " << ofRandomf();
        
        /*if ( _state == VUI_STATE_DOWN && ofGetElapsedTimeMillis() < lastTimeMouseDown ){
            lastTimeMouseDown = ofGetElapsedTimeMillis() + 500;
            return;
        }*/
        
        
        
        //ofLog() << "[" << vuiUID << "]  - " << "vstate: " << virtualState << "  _state: " << _state << "   update:" << update;
        //if ( state == VUI_STATE_DOWN ) ofLog() << "[" << vuiUID << "]  - " << "vstate: " << virtualState << "  state:" << state << "  _state: " << _state << "   update:" << update;
        

		if (virtualState == VUI_STATE_DOWN && (toState == VUI_STATE_UP || toState == VUI_STATE_OVER)) {
            //ofLog() << ofRandomf();
            VUI::EventManager.StoreEvent( this, VUI_EVENT_MOUSE_CLICK );
		}
        
        if ( isToggle ){
            if ( prevVirtualState == VUI_STATE_OVER && virtualState == VUI_STATE_UP && toState == VUI_STATE_OVER ) {
                //ofLog() << prevVirtualState;
                VUI::EventManager.StoreEvent( this, VUI_EVENT_MOUSE_CLICK );
            }
        }
		
        /*else if (!isToggle && _state == VUI_STATE_OVER && !hasState[_state] && update ) {
            state = VUI_STATE_UP;
        }*/
        
        
        
        if ( isToggle ){
            if ( state == VUI_STATE_DOWN && toState == VUI_STATE_DOWN ){
               // state = VUI_STATE_UP;
                VUI::EventManager.StoreState( this, VUI_STATE_UP );
            } if ( toState == VUI_STATE_UP ){
                //state = VUI_STATE_DOWN;
                //VUI::EventManager.StoreState( this, VUI_STATE_DOWN );
                //update = true;
            } else {
                if (hasState[toState] && update) {
                    VUI::EventManager.StoreState( this, (VUI::State)toState );
                    //VUI::EventManager.StoreEvent( this, VUI:state );
                }
            }
        } else {
            if ( virtualState == VUI_STATE_UP && toState == VUI_STATE_OVER ){
                VUI::EventManager.StoreEvent( this, VUI_EVENT_MOUSE_OVER );
            }
            
            if ( toState == VUI_STATE_UP ) {
                state = toState;
                VUI::EventManager.StoreEvent( this, VUI_EVENT_MOUSE_OUT );
            } else if (hasState[toState] && update) {
                VUI::EventManager.StoreState( this, (VUI::State)toState );
            } else {
                state = VUI_STATE_UP;
            }
            /*if (hasState[_state] && update) {
                if ( _state == VUI_STATE_UP ) state = _state;
                if ( _state != VUI_STATE_UP ) VUI::EventManager.StoreState( this, (VUI::State)_state );
            }
            if (_state == VUI_STATE_OVER && !hasState[_state] && update ) {
                state = VUI_STATE_UP;
            }*/
        }
        
		if ( virtualState == VUI_STATE_OVER && toState == VUI_STATE_DOWN ) {
            VUI::EventManager.StoreEvent( this, VUI_EVENT_MOUSE_PRESSED );
        } else if (virtualState == VUI_STATE_DOWN && toState == VUI_STATE_OVER ){
            VUI::EventManager.StoreEvent( this, VUI_EVENT_MOUSE_RELEASED );
        }
        
        
        prevVirtualState = int(virtualState);
        
        virtualState = toState;
        
        
        
        if ( toState == VUI_STATE_DOWN ){
            lastTimeMouseDown = ofGetElapsedTimeMillis() + 500;
        }
        
		
        
        if (update) {
            UpdatePosition();
            
        }

	}
    
    ofVec2f prevMousePos;
    
    void Element::TriggerEvent(vuiEvent eventType){
        //ofLog() << "TriggerEvent[" << eventType << "]";
        
        vuiEventArgs args;
        args.element = this;
        args.eventType = eventType;
        args.localMousePos.set( VUI::mouseX - globalMinPosition.x, VUI::mouseY - globalMinPosition.y );
        args.globalMousePos.set( VUI::mouseX, VUI::mouseY );
        
        vuiEventArgs argsTouch;
        argsTouch.element = this;
        
        switch ( eventType ){
            case VUI_EVENT_MOUSE_OVER:
                ofNotifyEvent(onMouseOver, args, this);
                break;
            case VUI_EVENT_MOUSE_OUT:
                ofNotifyEvent(onMouseOut, args, this);
                break;
            case VUI_EVENT_MOUSE_PRESSED:
                ofNotifyEvent(onMousePressed, args, this);
                
                argsTouch.eventType = VUI_EVENT_TOUCH_DOWN;
                ofNotifyEvent(onTouchDown, argsTouch, this );
                break;
            case VUI_EVENT_MOUSE_MOVED:
                if ( prevMousePos.x == VUI::mouseX && prevMousePos.y == VUI::mouseY ) return;
                ofNotifyEvent(onMouseMoved, args, this);
                prevMousePos.set( VUI::mouseX, VUI::mouseY );
                break;
            case VUI_EVENT_MOUSE_DRAGGED:
                args.localDragStart.set( mouseDownPos.x - globalMinPosition.x, mouseDownPos.y - globalMinPosition.y );
                args.localDragDelta.set( VUI::mouseX - mouseDownPos.x, VUI::mouseY - mouseDownPos.y );
                ofNotifyEvent(onMouseDragged, args, this);
                break;
            case VUI_EVENT_MOUSE_RELEASED:
                ofNotifyEvent(onMousePressed, args, this);
                
                argsTouch.eventType = VUI_EVENT_TOUCH_UP;
                ofNotifyEvent(onTouchUp, argsTouch, this );
                break;
            case VUI_EVENT_MOUSE_CLICK:
                ofNotifyEvent(onMouseClick, args, this);
                
                argsTouch.eventType = VUI_EVENT_TOUCH_TAP;
                ofNotifyEvent(onTouchTap, argsTouch, this );
                
                //ofLog() << "click  - " << ofRandomf();
                break;
            case VUI_EVENT_MOUSE_DOUBLE_CLICK:
                ofNotifyEvent(onMouseDoubleClick, args, this);
                
                argsTouch.eventType = VUI_EVENT_TOUCH_DOUBLE_TAP;
                ofNotifyEvent(onTouchDoubleTap, argsTouch, this );
                break;
			case VUI_EVENT_TOUCH_UP:
				args.eventType = VUI_EVENT_MOUSE_CLICK;
				ofNotifyEvent(onMouseClick, args, this);

				vuiEventArgs argsTouch2;
				argsTouch2.element = this;
				argsTouch2.eventType = VUI_EVENT_TOUCH_UP;
				ofNotifyEvent(onTouchUp, argsTouch2, this);

				argsTouch.eventType = VUI_EVENT_TOUCH_TAP;
				ofNotifyEvent(onTouchTap, argsTouch, this);
				break;
        }
        
        
    }

	void Element::UpdateAnchorOffset() {
		switch (anchorPoint) {
		case VUI_ALIGN_LEFT_TOP:
			anchorOffset.x = 0;
			anchorOffset.y = 0;
			break;
		case VUI_ALIGN_RIGHT_TOP:
			//anchorOffset.x = -styleFloat[state]["width"];
                anchorOffset.x = -(width*scale);
			anchorOffset.y = 0;
			break;
		case VUI_ALIGN_CENTER_TOP:
			//anchorOffset.x = -0.5*styleFloat[state]["width"];
                anchorOffset.x = -0.5*(width*scale);
			anchorOffset.y = 0;
			break;
		case VUI_ALIGN_LEFT_CENTER:
			anchorOffset.x = 0;
			//anchorOffset.y = -0.5*styleFloat[state]["height"];
                anchorOffset.y = -0.5*(height*scale);
			break;
		case VUI_ALIGN_RIGHT_CENTER:
			//anchorOffset.x = -styleFloat[state]["width"];
			//anchorOffset.y = -0.5*styleFloat[state]["height"];
                anchorOffset.x = -(width*scale);
                anchorOffset.y = -0.5*(height*scale);
			break;
		case VUI_ALIGN_CENTER_CENTER:
			//anchorOffset.x = -0.5*styleFloat[state]["width"];
			//anchorOffset.y = -0.5*styleFloat[state]["height"];
                anchorOffset.x = -0.5*(width*scale);
                anchorOffset.y = -0.5*(height*scale);
			break;
		case VUI_ALIGN_LEFT_BOTTOM:
			anchorOffset.x = 0;
			//anchorOffset.y = -styleFloat[state]["height"];
            anchorOffset.y = -(height*scale);
			break;
		case VUI_ALIGN_RIGHT_BOTTOM:
			//anchorOffset.x = -styleFloat[state]["width"];
			//anchorOffset.y = -styleFloat[state]["height"];
            anchorOffset.x = -(width*scale);
            anchorOffset.y = -(height*scale);
			break;
		case VUI_ALIGN_CENTER_BOTTOM:
			anchorOffset.x = -0.5*(width*scale);
			anchorOffset.y = -(height*scale);
			break;
		}
	}

	void Element::Render(float parentOffsetX, float parentOffsetY, float parentOpacity, ofVec2f _anchorOffset ) {
        
        parentSumOpacity = parentOpacity * opacity;
        parentSumOffset.set( parentOffsetX, parentOffsetY );
        if ( !userUpdating ) Update(-1, -1, true);
        
        
        //ofLog() << "Element::Render - [" << userUpdating << "] - " << ofRandomf();
        ofPushView();
        VUI::_PrivateRotateUI();
        ofTranslate( drawPosition.x + parentSumOffset.x, drawPosition.y + parentSumOffset.y );
        ofRotate( rotation );
        
		ofFill();
		ofEnableAlphaBlending();

		if (maskTex != nullptr && fbo != nullptr) fbo->begin();

		if (style[state]["background-color"] != "transparent") {
			//ofSetHexColor(styleFloat[state]["background-color"]);
			color.setHex(styleFloat[state]["background-color"], styleFloat[state]["opacity"]*parentSumOpacity);
			ofSetColor(color);
			//cout << state << endl;
			//ofSetColor( )
            
            //if ( name == "comfortBar" ) ofLog() << style[state]["background-color"] << " -> " << styleFloat[state]["background-color"];
			ofDrawRectangle(_anchorOffset.x + anchorOffset.x, _anchorOffset.y + anchorOffset.y, width*scale, height*scale);
		}
		
		//ofSetColor(ofColor::white);
		color.setHex(0xffffff, styleFloat[state]["opacity"]*parentSumOpacity);
		ofSetColor(color);

		for (vector<string>::iterator it = imageIDs[state].begin(); it != imageIDs[state].end(); it++) {
			images[(*it)]->drawSubsection(anchorOffset.x, anchorOffset.y, 0, width*scale, height*scale, 0, 0, width, height);
		}
        
        RenderAfter( _anchorOffset.x + anchorOffset.x, _anchorOffset.y + anchorOffset.y );
        
        
        for ( vector<Element*>::iterator it = children.begin(); it != children.end(); it++){
            (*it)->Render(localMinPosition.x, localMinPosition.y, parentSumOpacity, anchorOffset);
        }


		if (maskTex != nullptr && fbo != nullptr) {

			fbo->end();

            fbo->getTexture().setAlphaMask(*maskTex);

			ofEnableAlphaBlending();
            fbo->draw(0,0);

		}
        
		ofSetColor(255, 255, 255, 255);
        
        ofPopView();
	}

	void Element::ParseStyle( string property, int _state ) {
		//if (this->style["left"] != "") return;
		/*cout << "x:" << style[_state]["x"] << endl;
		cout << "y:" << style[_state]["y"] << endl;
		cout << "width:" << style[_state]["width"] << endl;
		cout << "height:" << style[_state]["height"] << endl;*/
		
		if (style[_state]["background-color"] != "transparent" ) {
            if ( style[_state]["background-color"].find( "#" ) != string::npos ){
                string str(style[_state]["background-color"]);
                ofStringReplace(str, "#", "");
                styleFloat[_state]["background-color"] = stoul(str, nullptr, 16);
            } else {
                styleFloat[_state]["background-color"] = ofToFloat(style[_state]["background-color"]);
            }
            if ( property == "background-color" ) return;
		}
		
		//styleFloat[_state]["width"] = ofToFloat(style[_state]["width"]);
		//styleFloat[_state]["height"] = ofToFloat(style[_state]["height"]);
		styleFloat[_state]["offset-x"] = ofToFloat(style[_state]["offset-x"]);
		styleFloat[_state]["offset-y"] = ofToFloat(style[_state]["offset-y"]);
		styleFloat[_state]["opacity"] = ofToFloat(style[_state]["opacity"]) * 255.0;
		//cout << "x => " << ofToFloat(style["x"]) << endl;

		//SetPosition(ofToFloat(style[_state]["x"]), ofToFloat(style[_state]["y"]), ofToFloat(style[state]["z"]), _state, false);
		//SetSize(ofToFloat(style[_state]["width"]), ofToFloat(style[_state]["height"]), _state, false);

	}

	float Element::GetProperty(string property) {
		return styleFloat[state][property];
	}

	void Element::SetProperty(string property, string val) {
		style[state][property] = val;
		ParseStyle(property);
	}

	void Element::SetProperty(string property, float val) {
		style[state][property] = ofToString(val);
		ParseStyle(property);
	}
    
    void Element::SetProperty(string property, int val) {
        style[state][property] = ofToString(val);
        ParseStyle(property);
    }

	Element* Element::SetPosition(float x, float y) {
		this->setPosition(x, y, 0);

		return this;
	}
    
    ofVec2f Element::GetPosition(){
        return this->getPosition();
    }
    
    void Element::SetPositionX(float x) {
        this->setPosition(x, GetPosition().y, 0);
    }
    
    void Element::SetPositionY(float y) {
        this->setPosition(GetPosition().x, y, 0);
    }


	Element* Element::SetSize(float w, float h ) {
        width = w;
        height = h;
        return this;
	}
    
    void Element::SetWidth( float w ){
        width = w;
    }
    
    void Element::SetHeight( float h ){
        height = h;
    }
    
	void Element::UseStyleClass(string name) {
		if (styleSheet == nullptr)return;
        SetName( "." + name );
		hasStyle = false;
		SetStyle(styleSheet->GetStyleByClass(name), VUI_STATE_ALL, false);
	}

	void Element::UseStyleID(string name) {
		if (styleSheet == nullptr) return;
        SetName( "#" + name );
		hasStyle = false;
		SetStyle(styleSheet->GetStyleByID(name), VUI_STATE_ALL, false);
	}
    
    vector<string> Element::SplitStyles(string style){
        string s(style);
        ofStringReplace(s, " ", "");
        ofStringReplace(s, "\n", "");
        ofStringReplace(s, "	", "");
        
        vector<string> split = ofSplitString(s, ";");
        return split;
    }

	Element* Element::SetStyle(string style, int _state, bool initState) 
	{
		if (style == "") {
			//cout << "ERROR - SetStyle - style is empty" << endl;
			//cout << "[" << _state << "] " << "style => " << style << endl;
			return this;
		}
		
		if (!hasStyle ) {
			

			string mainStyle = StyleSheet::ExtractStyleByState(style, VUI_STATE_UP);
			//cout << "[" << _state << "] " << "mainStyle => " << mainStyle << endl;

			if (mainStyle == "") {
				//cout << "ERROR - SetStyle - style is empty" << endl;
				if (style.find(":over") != string::npos) SetStyle(StyleSheet::ExtractStyleByState(style, VUI_STATE_OVER), VUI_STATE_OVER);
				if (style.find(":down") != string::npos) SetStyle(StyleSheet::ExtractStyleByState(style, VUI_STATE_DOWN), VUI_STATE_DOWN);
				return this;
			}

			hasStyle = true;

			SetStyle(mainStyle, VUI_STATE_UP);
			SetStyle(mainStyle, VUI_STATE_OVER, false);
			SetStyle(mainStyle, VUI_STATE_DOWN, false);

			if (style.find(":over") != string::npos) SetStyle(StyleSheet::ExtractStyleByState(style, VUI_STATE_OVER), VUI_STATE_OVER);
			if (style.find(":down") != string::npos) SetStyle(StyleSheet::ExtractStyleByState(style, VUI_STATE_DOWN), VUI_STATE_DOWN);
			
			return this;
		}
		else if (_state == VUI_STATE_ALL) {
			string mainStyle = StyleSheet::ExtractStyleByState(style, VUI_STATE_UP);
			//cout << "[" << _state << "] " << "mainStyle => " << mainStyle << endl;

			if (mainStyle == "") {
				//cout << "ERROR - SetStyle - style is empty" << endl;
				SetStyle(StyleSheet::ExtractStyleByState(style, VUI_STATE_OVER), VUI_STATE_OVER, initState);
				SetStyle(StyleSheet::ExtractStyleByState(style, VUI_STATE_DOWN), VUI_STATE_DOWN, initState);
				return this;
			}

			hasStyle = true;

			SetStyle(mainStyle, VUI_STATE_UP);
			SetStyle(mainStyle, VUI_STATE_OVER, initState);
			SetStyle(mainStyle, VUI_STATE_DOWN, initState);
		}
        
        if ( _state == VUI_STATE_UP ) unparsedStyle = style;
		
        
        vector<string> split = SplitStyles( style );
        
		vector<string> tempSplit;

		for (vector<string>::iterator it = split.begin(); it != split.end(); it++) {
			tempSplit = ofSplitString((*it), ":");
			if (tempSplit.size() == 2) {
                if (tempSplit[0] == "image" || tempSplit[0] == "img" ) {
					if (styleSheet != nullptr) {
						//images[_state][tempSplit[1]] = styleSheet->GetImage(tempSplit[1]);
						//this->style[_state][tempSplit[1]] = "image";
						
                        // TODO: multiple images per state
                        //imageIDs[_state].push_back(tempSplit[1]);
						//images[tempSplit[1]] = styleSheet->GetImage(tempSplit[1]);
                        
                        if ( styleSheet->HasImage(tempSplit[1]) ){
                            imageIDs[_state].clear();
                            
                            imageIDs[_state].push_back(tempSplit[1]);
                            images[tempSplit[1]] = styleSheet->GetImage(tempSplit[1]);
                        } else {
                            ofLog( OF_LOG_WARNING ) << "[" << GetName() << "] image id(" << tempSplit[1] << ") doesn't exist";
                        }
					}
				}
				else if (tempSplit[0] == "video") {

				}
				else if (tempSplit[0] == "anchorPoint" || tempSplit[0] == "anchor-point" ) {
					if (tempSplit[1] == "left-top") SetAnchorPoint(VUI_ALIGN_LEFT_TOP);
					else if (tempSplit[1] == "left-center") SetAnchorPoint(VUI_ALIGN_LEFT_CENTER);
					else if (tempSplit[1] == "left-bottom") SetAnchorPoint(VUI_ALIGN_LEFT_BOTTOM);
					else if (tempSplit[1] == "center-top") SetAnchorPoint(VUI_ALIGN_CENTER_TOP);
					else if (tempSplit[1] == "center-center") SetAnchorPoint(VUI_ALIGN_CENTER_CENTER);
					else if (tempSplit[1] == "center-bottom") SetAnchorPoint(VUI_ALIGN_CENTER_BOTTOM);
					else if (tempSplit[1] == "right-top") SetAnchorPoint(VUI_ALIGN_RIGHT_TOP);
					else if (tempSplit[1] == "right-center") SetAnchorPoint(VUI_ALIGN_RIGHT_CENTER);
					else if (tempSplit[1] == "right-bottom") SetAnchorPoint(VUI_ALIGN_RIGHT_BOTTOM);
				}
                else if (tempSplit[0] == "font" ){
                    vector<string> fontProps = ofSplitString(tempSplit[1], "," );
                    int size = VUI::fontSize;
                    
                    if ( fontProps.size() == 2 ) size = ofToInt(fontProps[1]);
                    
                    if ( VUI::HasFont( fontProps[0], size ) ) {
                        font = VUI::GetFont( fontProps[0], size );
                    } else {
                        font = VUI::AddFont( fontProps[0], size );
                    }
                }
				else {
					this->style[_state][tempSplit[0]] = tempSplit[1];

					if (_state == VUI_STATE_UP) {
						if (tempSplit[0] == "width" || tempSplit[0] == "height" ) {
							/*for (int i = 0; i < 3; i++) {
								this->style[i][tempSplit[0]] = tempSplit[1];
							}*/
                            if ( tempSplit[0] == "width" ) width = ofToFloat( tempSplit[1] );
                            else if ( tempSplit[0] == "height" ) height = ofToFloat( tempSplit[1] );
						}
					}
				}
                
                StorePropValue( tempSplit[0], tempSplit[1] );
			}
		}

		if ( initState ) hasState[_state] = true;

		ParseStyle( "", _state );
		Update( -1, -1, true );
		Render();

		return this;
	}
}
