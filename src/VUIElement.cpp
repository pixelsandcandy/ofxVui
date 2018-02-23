#include "ofxVui.h"
#include "VUIElement.h"
#include "VUIStyleSheet.h"

namespace VUI {
	Element::~Element() {
        VUI::Init();
    }

    void Element::Setup(int x, int y, StyleSheet *ss, string primarySelector, string secondarySelector ) {
        VUI::Init();
        
        SetDefaultStyles(x, y);
        SetPosition(x,y);
        ParseStyleSheet(ss, primarySelector, secondarySelector );
    }
    
    void Element::Setup(int x, string y, StyleSheet *ss, string primarySelector, string secondarySelector ) {
        VUI::Init();
        
        SetDefaultStyles(x, 0);
        SetPosition(x,y);
        ParseStyleSheet(ss, primarySelector, secondarySelector );
    }
    
    void Element::Setup(string x, int y, StyleSheet *ss, string primarySelector, string secondarySelector ) {
        VUI::Init();
        
        SetDefaultStyles(0, y);
        SetPosition(x,y);
        ParseStyleSheet(ss, primarySelector, secondarySelector );
    }
    
    void Element::Setup(string x, string y, StyleSheet *ss, string primarySelector, string secondarySelector ) {
        VUI::Init();
        
        SetDefaultStyles(0, 0);
        SetPosition(x,y);
        ParseStyleSheet(ss, primarySelector, secondarySelector );
    }
    
    
    
    
    
    Element::Element(int x, int y, StyleSheet *ss, string primarySelector, string secondarySelector ) {
        Setup(x,y,ss,primarySelector,secondarySelector);
	}
    
    Element::Element(int x, string y, StyleSheet *ss, string primarySelector, string secondarySelector ) {
        Setup(x,y,ss,primarySelector,secondarySelector);
    }
    
    Element::Element(string x, int y, StyleSheet *ss, string primarySelector, string secondarySelector ) {
        Setup(x,y,ss,primarySelector,secondarySelector);
    }
    
    Element::Element(string x, string y, StyleSheet *ss, string primarySelector, string secondarySelector ) {
        Setup(x,y,ss,primarySelector,secondarySelector);
    }
    
    
    
    void Element::ParseStyleSheet(StyleSheet *ss, string primarySelector, string secondarySelector){
        if (ss != nullptr) {
            SetStyleSheet(ss);
            if (primarySelector != "") {
                if (primarySelector.find(".") != string::npos) {
                    UseStyleClass(primarySelector.substr(1));
                }
                else if (primarySelector.find("#") != string::npos) {
                    UseStyleID(primarySelector.substr(1));
                }
            }
            
            if (secondarySelector != "") {
                if (secondarySelector.find(".") != string::npos) {
                    UseStyleClass(secondarySelector.substr(1));
                }
                else if (secondarySelector.find("#") != string::npos) {
                    UseStyleID(secondarySelector.substr(1));
                }
            }
        }
    }

	void Element::SetDefaultStyles(int x, int y) {
		for (int i = 0; i < 3; i++) {
			style[i]["opacity"] = "1";
			style[i]["background-color"] = "clear";
            style[i]["background-opacity"] = "1.0";
		}
        
        hasStyle = false;
        
		SetPosition(x, y);

		ParseStyle();
	}

	void Element::UpdatePosition() {
		UpdateAnchorOffset();

        localMinPosition.x = GetPosition().x;
        localMinPosition.y = GetPosition().y;
        
		localMaxPosition.x = GetPosition().x + GetWidth();
		localMaxPosition.y = GetPosition().y + GetHeight();

		localMaxPosition.x += styleFloat[renderState]["offset-x"];
		localMinPosition.x += styleFloat[renderState]["offset-x"];

		localMaxPosition.y += styleFloat[renderState]["offset-y"];
		localMinPosition.y += styleFloat[renderState]["offset-y"];

		/*localMaxPosition.x += anchorOffset.x;
		localMinPosition.x += anchorOffset.x;

		localMaxPosition.y += anchorOffset.y;
		localMinPosition.y += anchorOffset.y;*/
        
        //globalMinPosition.set( localMinPosition.x + parentSumOffset.x, localMinPosition.y + parentSumOffset.y );
        //globalMaxPosition.set( localMaxPosition.x + parentSumOffset.x, localMaxPosition.y + parentSumOffset.y );
        
        globalMinPosition.set( localMinPosition.x + parentSumOffset.x + anchorOffset.x + parentOffsetPos.x, localMinPosition.y + parentSumOffset.y + anchorOffset.y + parentOffsetPos.y);
        
        
        globalMaxPosition.set( globalMinPosition.x + GetWidth(), globalMinPosition.y + GetHeight() );
        
        
        
        drawPosition.x = localMinPosition.x - anchorOffset.x - parentSumOffset.x;
        drawPosition.y = localMinPosition.y - anchorOffset.y - parentSumOffset.y;
        
        if ( VUI::uiRotation == VUI_ROTATE_90_CCW ){
            ofVec2f savedLocal(localMinPosition);
            ofVec2f savedGlobalMin(globalMinPosition);
            
            localMinPosition.y = savedLocal.x;
            localMinPosition.x = savedLocal.y;
            
            globalMinPosition.x = savedGlobalMin.y;
            globalMinPosition.y = VUI::GetWindowHeight(false) - savedGlobalMin.x - GetWidth();
            
            globalMaxPosition.x = savedGlobalMin.y + GetHeight();
            globalMaxPosition.y = VUI::GetWindowHeight(false) - savedGlobalMin.x;
        }
            
		
	}

	void Element::Update(int mouseX, int mouseY, bool internalUpdate) {
        if ( !GetEventManager()->active ) return;
        if ( !isActive ) return;
        if ( !internalUpdate ) userUpdating = true;
        
		UpdatePosition();
        
        //if ( !isInteractive )return;
        
		if (mouseX != -1 && mouseY != -1) {
			if (mouseX > globalMinPosition.x && mouseX < globalMaxPosition.x) {
				if (mouseY > globalMinPosition.y && mouseY - fixMouseY < globalMaxPosition.y) {
                    if ( isInteractive ){
                        GetEventManager()->StoreOverElement( this );
                        if (ofGetMousePressed()) UpdateState(VUI_STATE_DOWN);
                        else UpdateState(VUI_STATE_OVER);
                        _mInside = true;
                    }
                    isMouseInside = true;
					return;
				}
			}
            isMouseInside = false;
            _mInside = false;
			if ( isInteractive ) UpdateState(VUI_STATE_UP);
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
                        GetEventManager()->StoreOverElement( this );
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
					GetEventManager()->StoreOverElement(this);
					if (ofGetMousePressed()) UpdateState(VUI_STATE_DOWN);
					else UpdateState(VUI_STATE_OVER);
					return;
				}
			}
		}

		


#else

		if (VUI::mouseX > globalMinPosition.x && VUI::mouseX < globalMaxPosition.x) {
			if (VUI::mouseY > globalMinPosition.y && VUI::mouseY < globalMaxPosition.y) {
                GetEventManager()->StoreOverElement( this );
				if (ofGetMousePressed()) UpdateState(VUI_STATE_DOWN);
				else UpdateState(VUI_STATE_OVER);
				return;
			}
		}

#endif*/
        
        if (VUI::IsTouchEnabled()) {
            if ( !isInteractive ) return;
            
            bool touchDownOnElement = false;
			//if (VUI::GetPrevOverElement() == this) touchDownOnElement = true;
            
            //ofLog() << "Update - " << VUI::GetTouches()->size() << "  - " << ofRandomf();
            
            for (map<int, ofPoint>::iterator it = VUI::GetTouches()->begin(); it != VUI::GetTouches()->end(); it++){
                ofPoint p = (*it).second;
                //ofLog() << "[x] " << globalMinPosition.x << " > " << p.x << " < " << globalMaxPosition.x << "  [y] " << globalMinPosition.y << " > " << p.x << " < " << globalMaxPosition.y;
                
                if (p.x > globalMinPosition.x && p.x < globalMaxPosition.x) {
                    if (p.y > globalMinPosition.y && p.y < globalMaxPosition.y) {
                        //ofLog() << VUI::GetTouchDown() << " - " << ofGetMousePressed();
						touchDownOnElement = true;
						touchPoint.set(p.x, p.y);
						break;
                    }
                }
            }
			
			if ( GetEventManager()->prevOverElement == this) {
				if (touchDownOnElement) {
					if (!isMouseDown) {
						if (isToggle) {
							if (virtualState == VUI_STATE_DOWN) {
								SetState(VUI_STATE_UP);
								TriggerEvent(VUI_EVENT_TOUCH_UP);
							}
							else {
								SetState(VUI_STATE_DOWN);
								TriggerEvent(VUI_EVENT_TOUCH_DOWN);
							}
						}
						else {
							SetState(VUI_STATE_DOWN);
							TriggerEvent(VUI_EVENT_TOUCH_DOWN);
						}
						isMouseDown = true;
					}
				}
				else {
					if (isMouseDown && !isToggle) {
						SetState(VUI_STATE_UP);
						TriggerEvent(VUI_EVENT_TOUCH_UP);
					}
					isMouseDown = false;
				}
				
			}
			else {
				if (isMouseDown && !isToggle) {
					SetState(VUI_STATE_UP);
					//TriggerEvent(VUI_EVENT_TOUCH_UP);
				}
				isMouseDown = false;
			}

			if ( touchDownOnElement ) GetEventManager()->StoreOverElement(this);


			/*if (VUI::GetPrevOverElement() == this) {
				if (VUI::GetTouchDown()) {
					//UpdateState(VUI_STATE_DOWN);
					if (touchDownOnElement) {
						SetState(VUI_STATE_DOWN);
						if (!isMouseDown) {
							TriggerEvent(VUI_EVENT_TOUCH_DOWN);
							isMouseDown = true;
						}
					}
				}
				else {
					//UpdateState(VUI_STATE_UP);
					if (touchDownOnElement && isMouseDown) {
						SetState(VUI_STATE_UP);
						TriggerEvent(VUI_EVENT_TOUCH_UP);
						isMouseDown = false;
					}

				}
			}
			else {
				if (isMouseDown && !isToggle) {
					TriggerEvent(VUI_EVENT_TOUCH_UP);
					SetState(VUI_STATE_UP);
					VUI::ClearOverElement();
					isMouseDown = false;
				}
			}


			if (VUI::GetOverElement() != this) GetEventManager()->StoreOverElement(this);
			//ofLog() << "touchDown:" << VUI::GetTouchDown() << "  -" << ofRandomf();
			return;
            
            if (touchDownOnElement && !isToggle) {
                TriggerEvent(VUI_EVENT_TOUCH_UP);
                SetState(VUI_STATE_UP);
                VUI::ClearOverElement();
            }*/
			
        }
        else {
            
            if (VUI::mouseX > globalMinPosition.x && VUI::mouseX < globalMaxPosition.x) {
                if (VUI::mouseY > globalMinPosition.y && VUI::mouseY - fixMouseY < globalMaxPosition.y) {
                    if ( isInteractive ){
                        if ( GetEventManager()->prevOverElement == this ){
                            
                            //bool dragged = false;
                            
                            //if ( ofGetMousePressed() && (mouseDownPos.x != VUI::mouseX || mouseDownPos.y != VUI::mouseY) ) dragged = true;
                            
                            if ( ofGetMousePressed() && (mouseDownPos.x != VUI::mouseX || mouseDownPos.y != VUI::mouseY) ) TriggerEvent( VUI_EVENT_MOUSE_DRAGGED );
                            else TriggerEvent( VUI_EVENT_MOUSE_MOVED );
                            
                            if (ofGetMousePressed()) {
                                if ( !isMouseDown ) {
                                    isMouseDown = true;
                                    mouseDownPos.set( VUI::mouseX, VUI::mouseY );
                                    //ofLog() << VUI::mouseX << "," << VUI::mouseY;
                                }
                                
                                UpdateState(VUI_STATE_DOWN, true, true);
                                
                                /*if ( hasState[VUI_STATE_DOWN] ) UpdateState(VUI_STATE_DOWN, true, true);
                                 else {
                                 if ( hasState[VUI_STATE_OVER] ) UpdateState(VUI_STATE_OVER, true, true);
                                 else UpdateState(VUI_STATE_UP, true, true);
                                 }*/
                            } else {
                                if ( !isMouseInside || isMouseDown ) UpdateState(VUI_STATE_OVER, isMouseInside, isMouseDown);
                                isMouseDown = false;
                            }
                            
                            isMouseInside = true;
                            
                            //if ( dragged ) TriggerEvent( VUI_EVENT_MOUSE_DRAGGED );
                            //else TriggerEvent( VUI_EVENT_MOUSE_MOVED );
                        } else {
                            if ( isMouseInside ) {
                                UpdateState(VUI_STATE_UP);
                                isMouseInside = false;
                            }
                            isMouseDown = false;
                        }
                        
                        if (  GetEventManager()->overElement != this ) GetEventManager()->StoreOverElement(this);
                        _mInside = true;
                    } else {
                        isMouseInside = true;
                    }
                    
                    return;
                }
            }
            
			if (isMouseInside) {
                if ( isInteractive ){
                    GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_OUT );
                    UpdateState(VUI_STATE_UP);
                }
				isMouseInside = false;
			}
            
            _mInside = false;
			isMouseDown = false;
        }

        
		
		
	}

	void Element::UpdateState(int toState, bool isInside, bool isMouseDown ) {
        //ofLog() << "UpdateState => " << toState;
        
        if ( !GetEventManager()->IsActive() ) return;
        if ( !isInteractive ) return;
        
        int oldState(renderState);
        
		bool update = renderState != toState ? true : false;
        if ( isToggle || isSelection ) {
            //ofLog() << "virtual:" << virtualState << " -> to:" << toState;
            
            update = virtualState != toState ? true : false;
            if ( toState == VUI_STATE_DOWN ) update = true;
            
            if ( !isMouseDown ) {
                
                if ( toState == VUI_STATE_UP ) {
                    GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_OUT );
                    if ( virtualState != VUI_STATE_DOWN ) SetState( (VUI::State)toState );
                    return;
                }
            } else {
                if ( toState == VUI_STATE_OVER ) {
                    GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_RELEASED );
                    GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_CLICK );
                }
            }
            
            if ( !isInside && virtualState == VUI_STATE_DOWN && toState == VUI_STATE_OVER ){
                GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_OVER );
            }
            
        }
        
        
        if (IsTouchEnabled()) {
            if ( !VUI::GetTouchDown() && !ofGetMousePressed() ) lastTimeMouseDown = 0;
        }
        else {
            if (!ofGetMousePressed()) lastTimeMouseDown = 0;
        }
        
        /*if ( DEBUG_MODE ) {
            ofLog() << "virtual:" << prevVirtualState << "(" << virtualState << ") > real:" << toState << "(" << renderState << ")";
            ofLog() << "update: " << update;
        }*/
        
        /*if ( name == "nameField" ){
            ofLog() << lastTimeMouseDown;
            ofLog() << "[" << vuiUID << "]  - " << "vstate: " << virtualState << "  state:" << state << "  toState: " << toState << "   update:" << update << "  - " << ofRandomf();
        }*/
        
        if ( !update ) return;
        
        if ( isToggle || isSelection ){
            
            bool isDown = (virtualState == VUI_STATE_DOWN);
            
            if ( isDown && toState == VUI_STATE_OVER ) return;
            
            //ofLog() << "virtual:" << virtualState << " -> to:" << toState;
            
            if ( !isMouseDown || (isInside && isMouseDown) ) {
                //ofLog() << ofRandomf();
                if ( virtualState != VUI_STATE_OVER && toState == VUI_STATE_OVER ){
                    GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_OVER );
                    GetEventManager()->StoreState( this, (VUI::State)toState );
                    return;
                }
            }
            
            if ( toState == VUI_STATE_DOWN ){
                if ( ofGetElapsedTimeMillis() < lastTimeMouseDown ){
                    lastTimeMouseDown = ofGetElapsedTimeMillis() + 500;
                    return;
                }
                
                lastTimeMouseDown = ofGetElapsedTimeMillis() + 500;
                
                if ( isDown && !isSelection ){
                    toState = VUI_STATE_OVER;
                }
            }
            
            if ( virtualState == VUI_STATE_DOWN && toState == VUI_STATE_OVER ) {
                GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_PRESSED );
                SetState(VUI_STATE_OVER,true,true);
            } else if ( virtualState == VUI_STATE_OVER && toState == VUI_STATE_DOWN ){
                GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_PRESSED );
            }
            
            
            GetEventManager()->StoreState( this, (VUI::State)toState );
            
            
        } else {
            
            /*if (virtualState == VUI_STATE_DOWN && (toState == VUI_STATE_UP || toState == VUI_STATE_OVER)) {
                GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_CLICK );
            }*/
            
            if ( renderState == VUI_STATE_UP && toState == VUI_STATE_OVER ){
                GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_OVER );
            }
            
            //int evt = -1;
            
            if ( virtualState == VUI_STATE_OVER && toState == VUI_STATE_DOWN ) {
                if ( DEBUG_MODE ) ofLog() << "PRESSED";
                //evt = VUI_EVENT_MOUSE_PRESSED;
                GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_PRESSED );
            } else if (virtualState == VUI_STATE_DOWN && toState == VUI_STATE_OVER ){
                if ( DEBUG_MODE ) ofLog() << "RELEASED";
                
                if ( isInside ){
                    GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_RELEASED );
                    GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_CLICK );
                }
                
                //evt = VUI_EVENT_MOUSE_RELEASED;
            }
            
            if ( virtualState == VUI_STATE_OVER && toState == VUI_STATE_UP ) {
                renderState = toState;
                GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_OUT );
            } else if (update) {
                if ( DEBUG_MODE ) ofLog() << "virtualState:" << virtualState << "  state:" << toState << "  " << ofRandomf();
                GetEventManager()->StoreState( this, (VUI::State)toState );
            } else {
                renderState = VUI_STATE_UP;
            }
            
            
        }
        
        
        
        
        /*if ( !isToggle ){
            
        } else {
            
            if ( virtualState == VUI_STATE_DOWN && (toState == VUI_STATE_UP )) return;
            
            if ( virtualState == VUI_STATE_DOWN && toState == VUI_STATE_OVER ) {
                GetEventManager()->StoreEvent( this, VUI_EVENT_MOUSE_RELEASED );
                return;
            }
            
            if ( toState == VUI_STATE_DOWN ){
                //ofLog() << ofGetElapsedTimeMillis() << " < " << lastTimeMouseDown;
                
                if ( ofGetElapsedTimeMillis() < lastTimeMouseDown ){
                    lastTimeMouseDown = ofGetElapsedTimeMillis() + 500;
                    //ofLog() << "return" << endl;
                    //if ( name == ".textfield" ) ofLog() << virtualState << " > " << toState << "     RETURN - " << ofRandomf();
                    return;
                } else {
                    
                }
            } else if (!update){
                
                return;
            }
        }*/
        
        /*if ( DEBUG_MODE ) {
            ofLog() << " ";
            ofLog() << "-------------------------";
            ofLog() << "toState:" << toState;
            ofLog() << ofRandomf();
        }*/
        
        UpdateVirtualState( (VUI::State)toState, true );
        
        if (update) {
            UpdatePosition();
            
        }

	}
    
    ofVec2f prevMousePos;
    bool firstMove = true;
    bool canDrag = false;
    
    vuiEventArgs Element::GetEventArgs(vuiEvent eventType){
        vuiEventArgs args;
        args.element = this;
        args.eventType = eventType;
        args.localMousePos.set( VUI::mouseX - globalMinPosition.x, VUI::mouseY - globalMinPosition.y );
        args.globalMousePos.set( VUI::mouseX, VUI::mouseY );
        
        return args;
    }
    
    void Element::TriggerEvent(vuiEvent eventType){
        if ( !GetEventManager()->IsActive() ) return;
        //ofLog() << "TriggerEvent[" << eventType << "]";
        
        vuiEventArgs args = GetEventArgs(eventType);
        
        vuiEventArgs argsTouch;
        argsTouch.element = this;
		argsTouch.localMousePos.set(touchPoint.x - globalMinPosition.x, touchPoint.y - globalMinPosition.y);
		argsTouch.globalMousePos.set(touchPoint.x, touchPoint.y);
        
        int t;
        
        switch ( eventType ){
            case VUI_EVENT_FOCUS:
                ofNotifyEvent(onFocus, args, this);
                break;
            case VUI_EVENT_UNFOCUS:
                ofNotifyEvent(onUnfocus, args, this);
                break;
            case VUI_EVENT_VALUE_CHANGE:
                if ( virtualState == VUI_STATE_DOWN ) args.value = 1;
                else args.value = 0;
                
                ofNotifyEvent(onValueChange, args, this);
                break;
            case VUI_EVENT_MOUSE_OVER:
                ofNotifyEvent(onMouseOver, args, this);
                firstMove = true;
                break;
            case VUI_EVENT_MOUSE_OUT:
                ofNotifyEvent(onMouseOut, args, this);
                canDrag = false;
                firstMove = true;
                break;
            case VUI_EVENT_MOUSE_PRESSED:
				//ofLog() << "TriggerEvent >> VUI_EVENT_MOUSE_PRESSED";
                ofNotifyEvent(onMousePressed, args, this);
                
                argsTouch.eventType = VUI_EVENT_TOUCH_DOWN;
				argsTouch.localMousePos = args.localMousePos;
				argsTouch.globalMousePos = args.globalMousePos;
                ofNotifyEvent(onTouchDown, argsTouch, this );
                canDrag = true;
                break;
			case VUI_EVENT_TOUCH_DOWN:
				//ofLog() << "TriggerEvent >> VUI_EVENT_TOUCH_DOWN";
				touchDownTimeMS = ofGetElapsedTimeMillis();

				args.localMousePos = argsTouch.localMousePos;
				args.globalMousePos = argsTouch.globalMousePos;
				args.eventType = VUI_EVENT_MOUSE_PRESSED;
				ofNotifyEvent(onMousePressed, args, this);

				argsTouch.eventType = VUI_EVENT_TOUCH_DOWN;
				ofNotifyEvent(onTouchDown, argsTouch, this);
				canDrag = true;
				break;
            case VUI_EVENT_MOUSE_MOVED:
                if ( firstMove ) {
                    firstMove = false;
                    return;
                }
                if ( prevMousePos.x == VUI::mouseX && prevMousePos.y == VUI::mouseY ) return;
                ofNotifyEvent(onMouseMoved, args, this);
                prevMousePos.set( VUI::mouseX, VUI::mouseY );
                break;
            case VUI_EVENT_MOUSE_DRAGGED:
                if ( !canDrag ) return;
                args.localDragStart.set( mouseDownPos.x - globalMinPosition.x, mouseDownPos.y - globalMinPosition.y );
                args.localDragDelta.set( VUI::mouseX - mouseDownPos.x, VUI::mouseY - mouseDownPos.y );
                ofNotifyEvent(onMouseDragged, args, this);
                break;
            case VUI_EVENT_MOUSE_RELEASED:
                ofNotifyEvent(onMousePressed, args, this);
                
                argsTouch.eventType = VUI_EVENT_TOUCH_UP;
				argsTouch.localMousePos = args.localMousePos;
				argsTouch.globalMousePos = args.globalMousePos;
                ofNotifyEvent(onTouchUp, argsTouch, this );
                canDrag = false;
                break;
            case VUI_EVENT_MOUSE_CLICK:
			case VUI_EVENT_TOUCH_TAP:
				//ofLog() << "TriggerEvent >> VUI_EVENT_MOUSE_CLICK";
                
                t = ofGetElapsedTimeMillis();

				if (eventType == VUI_EVENT_TOUCH_TAP) {
					args.localMousePos = argsTouch.localMousePos;
					args.globalMousePos = argsTouch.globalMousePos;
				}

				if (eventType == VUI_EVENT_MOUSE_CLICK) {
					argsTouch.localMousePos = args.localMousePos;
					argsTouch.globalMousePos = args.globalMousePos;
				}

                //if ( DEBUG_MODE ) ofLog() << "lastClickTimeMS:" << lastClickTimeMS << "  now:" << t;
                
                //ofLog() << t - lastClickTimeMS << "  -  " << VUI::doubleClickThreshold;
                if ( (t - lastClickTimeMS) < VUI::doubleClickThreshold ){
                    //if ( DEBUG_MODE ) ofLog() << "TriggerEvent(VUI_EVENT_MOUSE_DOUBLE_CLICK)";
					
					
                    args.eventType = VUI_EVENT_MOUSE_DOUBLE_CLICK;
                    ofNotifyEvent(onMouseDoubleClick, args, this);
                    
                    argsTouch.eventType = VUI_EVENT_TOUCH_DOUBLE_TAP;
                    ofNotifyEvent(onTouchDoubleTap, argsTouch, this );
                } else {
                    //if ( DEBUG_MODE ) ofLog() << "TriggerEvent(VUI_EVENT_MOUSE_CLICK)";
					args.eventType = VUI_EVENT_MOUSE_CLICK;
                    
                    if ( GetState() == VUI_STATE_DOWN ) args.value = 1;
                    else args.value = 0;
                    
                    ofNotifyEvent(onMouseClick, args, this);
                    
                    argsTouch.eventType = VUI_EVENT_TOUCH_TAP;
                    ofNotifyEvent(onTouchTap, argsTouch, this );
                }
                
                lastClickTimeMS = t;
                break;
            case VUI_EVENT_MOUSE_DOUBLE_CLICK:
                ofNotifyEvent(onMouseDoubleClick, args, this);
                
                argsTouch.eventType = VUI_EVENT_TOUCH_DOUBLE_TAP;
				argsTouch.localMousePos = args.localMousePos;
				argsTouch.globalMousePos = args.globalMousePos;
                ofNotifyEvent(onTouchDoubleTap, argsTouch, this );
                break;
			case VUI_EVENT_TOUCH_UP:
				args.localMousePos = argsTouch.localMousePos;
				args.globalMousePos = argsTouch.globalMousePos;
				args.eventType = VUI_EVENT_MOUSE_RELEASED;
				ofNotifyEvent(onMouseClick, args, this);

				argsTouch.eventType = VUI_EVENT_TOUCH_UP;
				ofNotifyEvent(onTouchUp, argsTouch, this);

				t = ofGetElapsedTimeMillis();
				if (t - touchDownTimeMS <= VUI::touchTapThreshold) TriggerEvent(VUI_EVENT_TOUCH_TAP);
				
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
                anchorOffset.x = -GetWidth();
			anchorOffset.y = 0;
			break;
		case VUI_ALIGN_CENTER_TOP:
			//anchorOffset.x = -0.5*styleFloat[state]["width"];
                anchorOffset.x = -0.5*GetWidth();
			anchorOffset.y = 0;
			break;
		case VUI_ALIGN_LEFT_CENTER:
			anchorOffset.x = 0;
			//anchorOffset.y = -0.5*styleFloat[state]["height"];
                anchorOffset.y = -0.5*GetHeight();
			break;
		case VUI_ALIGN_RIGHT_CENTER:
			//anchorOffset.x = -styleFloat[state]["width"];
			//anchorOffset.y = -0.5*styleFloat[state]["height"];
                anchorOffset.x = -GetWidth();
                anchorOffset.y = -0.5*GetHeight();
			break;
		case VUI_ALIGN_CENTER_CENTER:
			//anchorOffset.x = -0.5*styleFloat[state]["width"];
			//anchorOffset.y = -0.5*styleFloat[state]["height"];
                anchorOffset.x = -0.5*GetWidth();
                anchorOffset.y = -0.5*GetHeight();
			break;
		case VUI_ALIGN_LEFT_BOTTOM:
			anchorOffset.x = 0;
			//anchorOffset.y = -styleFloat[state]["height"];
            anchorOffset.y = -GetHeight();
			break;
		case VUI_ALIGN_RIGHT_BOTTOM:
			//anchorOffset.x = -styleFloat[state]["width"];
			//anchorOffset.y = -styleFloat[state]["height"];
            anchorOffset.x = -GetWidth();
            anchorOffset.y = -GetHeight();
			break;
		case VUI_ALIGN_CENTER_BOTTOM:
			anchorOffset.x = -0.5*GetWidth();
			anchorOffset.y = -GetHeight();
			break;
		}
	}

	void Element::Render(float parentOffsetX, float parentOffsetY, float parentOpacity, ofVec2f _anchorOffset, ofVec2f _parentOffsetPos ) {
        
        if ( !isActive ) return;
        
        parentSumOpacity = parentOpacity * opacity;
        parentSumOffset.set( parentOffsetX, parentOffsetY );
        //parentAnchorOffset.set(_anchorOffset);
        //parentOffsetPos.set(_parentOffsetPos);
        
        if ( !userUpdating ) Update(-1, -1, true);
        
        ofPushView();
        VUI::_PrivateRotateUI();
        
        size.x = GetWidth();
        size.y = GetHeight();
        
        ofTranslate( localMinPosition.x, localMinPosition.y);
        
        ofRotate( rotation );
        ofTranslate( anchorOffset.x, anchorOffset.y );
        
		ofFill();
		ofEnableAlphaBlending();

        if (maskTex != nullptr && fbo != nullptr) {
            fbo->begin();
            ofClear(0);
        }
        
        ofRectangle rect(0, 0, size.x, size.y );

		if (style[renderState]["background-color"] != "clear") {
			color.setHex(styleFloat[renderState]["background-color"], styleFloat[renderState]["background-opacity"]*styleFloat[renderState]["opacity"]*parentSumOpacity);
			ofSetColor(color);
			ofDrawRectangle(rect.x, rect.y, rect.width, rect.height);
		}
		
		color.setHex(0xffffff, styleFloat[renderState]["opacity"]*parentSumOpacity);
		ofSetColor(color);

		for (vector<string>::iterator it = imageIDs[renderState].begin(); it != imageIDs[renderState].end(); it++) {
			images[(*it)]->drawSubsection(anchorOffset.x, anchorOffset.y, 0, size.x, size.y, 0, 0, GetWidth(false), GetHeight(false));
		}
        
        Image &img = bgImage[renderState];
        if ( img.active ) {
            if ( img.size == VUI_IMAGE_FILL ){
                img.image->draw(rect.x, rect.y, rect.width, rect.height);
            } else {
                if ( img.bounds.width == -1 ) {
                    //ofPoint s(size);
                    
                    if ( styleFloat[renderState]["bg-size-x"] == 0.0 && styleFloat[renderState]["bg-size-y"] == 0.0 ){
                        img.image->drawSubsection(styleFloat[renderState]["bg-position-x"] + rect.x, styleFloat[renderState]["bg-position-y"] + rect.y, size.x, size.y, img.bounds.x, img.bounds.y);
                    } else {
                        
                        img.image->drawSubsection(styleFloat[renderState]["bg-position-x"] + rect.x, styleFloat[renderState]["bg-position-y"] + rect.y, 0, styleFloat[renderState]["bg-size-x"], styleFloat[renderState]["bg-size-y"], img.bounds.x, img.bounds.y, img.dimensions.x, img.dimensions.y );
                    }
                } else {
                    img.image->drawSubsection(styleFloat[renderState]["bg-position-x"] + rect.x, styleFloat[renderState]["bg-position-y"] + rect.y, 0, size.x, size.y, img.bounds.x, img.bounds.y, img.bounds.width, img.bounds.height);
                    
                }
            }
            
        }
        
        for (vector<ofImage*>::iterator it = extraImages.begin(); it != extraImages.end(); it++ ){
            (*it)->draw(rect.x, rect.y);
        }
        
        for (vector<string>::iterator it = borderProps.begin(); it != borderProps.end(); it++){
            if ( styleInt[renderState][(*it)] != 0 ) {
                //if ( name == "#leftTop" ) ofLog() << (*it) << " -> " << styleFloat[renderState][(*it)];
                color.setHex(styleFloat[renderState][(*it)], styleFloat[renderState]["opacity"]*parentSumOpacity);
                ofSetColor(color);
                
                float thickness = styleInt[renderState][(*it)];
                
                if ( (*it) == "border-top" ) ofDrawRectangle(rect.x, rect.y, rect.width, thickness );
                else if ( (*it) == "border-right" ) ofDrawRectangle(rect.x+rect.width-thickness, rect.y, thickness, rect.height );
                else if ( (*it) == "border-bottom" ) ofDrawRectangle(rect.x, rect.y+rect.height-thickness, rect.width, thickness );
                else if ( (*it) == "border-left" ) ofDrawRectangle(rect.x, rect.y, thickness, rect.height );
            }
        }
        
        ofSetColor(255,255,255,255);
        RenderAfter(rect);
        
        for ( vector<Element*>::iterator it = children.begin(); it != children.end(); it++){

            (*it)->Render(localMinPosition.x + rect.x + parentSumOffset.x + anchorOffset.x, localMinPosition.y + rect.y + parentSumOffset.y + anchorOffset.y, parentSumOpacity);
        }

        ofSetColor(255,255,255,255);

		if (maskTex != nullptr && fbo != nullptr) {

			fbo->end();
            fbo->getTexture().setAlphaMask(*maskTex);

			ofEnableAlphaBlending();
            fbo->draw(rect.x, rect.y);
		}
        
		ofSetColor(255, 255, 255, 255);
        
        ofPopView();
	}

	void Element::ParseStyle( string property, int toState ) {
		//if (this->style["left"] != "") return;
		/*cout << "x:" << style[toState]["x"] << endl;
		cout << "y:" << style[toState]["y"] << endl;
		cout << "width:" << style[toState]["width"] << endl;
		cout << "height:" << style[toState]["height"] << endl;*/
		
		if (style[toState]["background-color"] != "clear" ) {
            if ( style[toState]["background-color"].find( "#" ) != string::npos ){
                string str(style[toState]["background-color"]);
                ofStringReplace(str, "#", "");
                styleFloat[toState]["background-color"] = stoul(str, nullptr, 16);
            } else {
                styleFloat[toState]["background-color"] = ofToFloat(style[toState]["background-color"]);
            }
            if ( property == "background-color" ) return;
		}
        
        styleFloat[toState]["background-opacity"] = ofToFloat(style[toState]["background-opacity"]);
		
		//styleFloat[toState]["width"] = ofToFloat(style[toState]["width"]);
		//styleFloat[toState]["height"] = ofToFloat(style[toState]["height"]);
        styleFloat[toState]["offset-x"] = ofToFloat(style[toState]["offset-x"])*VUI::dpi;
        styleFloat[toState]["offset-y"] = ofToFloat(style[toState]["offset-y"])*VUI::dpi;
		styleFloat[toState]["opacity"] = ofToFloat(style[toState]["opacity"]) * 255.0;
        styleFloat[toState]["bg-position-x"] = ofToFloat(style[toState]["bg-position-x"])*VUI::dpi;
        styleFloat[toState]["bg-position-y"] = ofToFloat(style[toState]["bg-position-y"])*VUI::dpi;
        styleFloat[toState]["bg-size-x"] = ofToFloat(style[toState]["bg-size-x"])*VUI::dpi;
        styleFloat[toState]["bg-size-y"] = ofToFloat(style[toState]["bg-size-y"])*VUI::dpi;
        
        /*styleFloat[toState]["border-top"] = ofToFloat(style[toState]["border-top"]);
        styleFloat[toState]["border-right"] = ofToFloat(style[toState]["border-right"]);
        styleFloat[toState]["border-bottom"] = ofToFloat(style[toState]["border-bottom"]);
        styleFloat[toState]["border-left"] = ofToFloat(style[toState]["border-left"]);*/
        
		//cout << "x => " << ofToFloat(style["x"]) << endl;

		//SetPosition(ofToFloat(style[toState]["x"]), ofToFloat(style[toState]["y"]), ofToFloat(style[state]["z"]), toState, false);
		//SetSize(ofToFloat(style[toState]["width"]), ofToFloat(style[toState]["height"]), toState, false);

	}
    
    void Element::SetOffsetY(int offset){
        for (int i = 0; i < 3; i++) {
            styleFloat[i]["offset-y"] = offset;
            style[i]["offset-y"] = ofToString(offset);
        }
    }
    
    void Element::SetOffsetX(int offset){
        for (int i = 0; i < 3; i++) {
            styleFloat[i]["offset-x"] = offset;
            style[i]["offset-x"] = ofToString(offset);
        }
    }

	float Element::GetProperty(string property) {
		return styleFloat[renderState][property];
	}

	void Element::SetProperty(string property, string val) {
		style[renderState][property] = val;
		ParseStyle(property);
	}

	void Element::SetProperty(string property, float val) {
		style[renderState][property] = ofToString(val);
		ParseStyle(property);
	}
    
    void Element::SetProperty(string property, int val) {
        style[renderState][property] = ofToString(val);
        ParseStyle(property);
    }

	Element* Element::SetPosition(float x, float y) {
		this->setPosition(x, y, 0);
        
        SetPositionX(x);
        SetPositionY(y);
        
		return this;
	}
    
    void Element::SetPosition(string x, string y) {
        SetPositionX(x);
        SetPositionY(y);
    }
    
    void Element::SetPosition(float x, string y) {
        SetPositionX(x);
        SetPositionY(y);
    }
    
    void Element::SetPosition(string x, float y) {
        SetPositionX(x);
        SetPositionY(y);
    }
    
    ofVec2f Element::GetPosition(){
        ofVec2f pos;
        
        if ( HasParent() ) {
            pos.x = percentCalcValues.getValue("x", parent->GetInnerWidth() );
            pos.y = percentCalcValues.getValue("y", parent->GetInnerHeight() );
        } else {
            pos.x = percentCalcValues.getValue("x", VUI::GetCurrentEventManager()->vw );
            pos.y = percentCalcValues.getValue("y", VUI::GetCurrentEventManager()->vh );
        }
        
        return pos;
    }
    
    void Element::SetPositionX(float x) {
        percentCalcValues.parseValue("x", x);
    }
    
    void Element::SetPositionY(float y) {
        percentCalcValues.parseValue("y", y);
    }
    
    void Element::SetPositionX(string x) {
        percentCalcValues.parseValue("x", x);
    }
    
    void Element::SetPositionY(string y) {
        percentCalcValues.parseValue("y", y);
    }


	Element* Element::SetSize(float w, float h ) {
        percentCalcValues.parseValue("width", w);
        percentCalcValues.parseValue("height", h);
        return this;
	}
    
    Element* Element::SetSize(string w, string h ) {
        percentCalcValues.parseValue("width", w);
        percentCalcValues.parseValue("height", h);
        
        return this;
    }
    
    void Element::SetWidth( float w ){
        percentCalcValues.parseValue("width", w);
    }
    
    void Element::SetHeight( float h ){
        percentCalcValues.parseValue("height", h);
    }
    
    void Element::SetWidth( string w ){
        percentCalcValues.parseValue("width", w);
    }
    
    void Element::SetHeight( string h ){
        percentCalcValues.parseValue("height", h);
    }
    
    int Element::GetWidth(bool scaled){
        int w;
        if ( HasParent() ) w = percentCalcValues.getValue("width", parent->GetInnerWidth() );
        else w = percentCalcValues.getValue("width", GetEventManager()->vw);
        
        if ( scaled ) return w*scale;
        else return w;
    }
    
    int Element::GetHeight(bool scaled){
        int h;
        if ( HasParent() ) h = percentCalcValues.getValue("height", parent->GetInnerHeight() );
        else h = percentCalcValues.getValue("height", GetEventManager()->vh );
        
        if ( scaled ) return h*scale;
        else return h;
    }
    
    int Element::GetOriginalWidth(bool scaled){
        return GetWidth(scaled)*VUI::divideDpi;
    }
    
    int Element::GetOriginalHeight(bool scaled){
        return GetHeight(scaled)*VUI::divideDpi;
    }
    
	void Element::UseStyleClass(string name) {
		if (styleSheet == nullptr)return;
        SetName( "." + name );
		//hasStyle = false;
        string style = styleSheet->GetStyleByClass(name);
        
        if ( !hasStyle ) SetStyle(style, VUI_STATE_ALL, false);
        else UseStyle( style );
		
	}
    
    void Element::UseStyle(string style){
        string mainStyle = StyleSheet::ExtractStyleByState(style, VUI_STATE_UP);
        SetStyle(mainStyle, VUI_STATE_UP);
        
        if (style.find(":over") != string::npos) SetStyle(StyleSheet::ExtractStyleByState(style, VUI_STATE_OVER), VUI_STATE_OVER);
        
        if (style.find(":down") != string::npos) SetStyle(StyleSheet::ExtractStyleByState(style, VUI_STATE_DOWN), VUI_STATE_DOWN);
    }

	void Element::UseStyleID(string name) {
		if (styleSheet == nullptr) return;
        SetName( "#" + name );
		//hasStyle = false;
        
        string style = styleSheet->GetStyleByID(name);
        
        if ( !hasStyle ) SetStyle(style, VUI_STATE_ALL, false);
        else UseStyle( style );
		
	}
    
    
    
    vector<string> Element::SplitStyles(string style){
        string s(style);
        ofStringReplace(s, " ", "");
        ofStringReplace(s, "\n", "");
        ofStringReplace(s, "	", "");
        
        vector<string> split = ofSplitString(s, ";");
        return split;
    }

    Element* Element::SetStyle(string style, int state, bool initState)
    {
        
        if (style == "") {
            //cout << "ERROR - SetStyle - style is empty" << endl;
            //cout << "[" << state << "] " << "style => " << style << endl;
            return this;
        }
        
        if (!hasStyle ) {
            //ofLog() << " ";
            //ofLog() << "[" << name << "] hasStyle:" << ofToString(hasStyle);
            
            //cout << "[" << state << "] " << "mainStyle => " << mainStyle << endl;
            //ofLog() << "mainStyle:" << mainStyle;
            
            string mainStyle = StyleSheet::ExtractStyleByState(style, VUI_STATE_UP);
            //ofLog() << mainStyle;
            
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
        else if (state == VUI_STATE_ALL) {
            string mainStyle = StyleSheet::ExtractStyleByState(style, VUI_STATE_UP);
            //cout << "[" << state << "] " << "mainStyle => " << mainStyle << endl;
            
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
        
        if ( state == VUI_STATE_UP ) unparsedStyle = style;
        
        
        vector<string> split = SplitStyles( style );
        
        vector<string> tempSplit;
        
        for (vector<string>::iterator it = split.begin(); it != split.end(); it++) {
            tempSplit = ofSplitString((*it), ":");
            if (tempSplit.size() == 2) {
                if ( tempSplit[0] == "background-image" || tempSplit[0] == "bg-image" || tempSplit[0] == "bgImage" || tempSplit[0] == "backgroundImage" ) {
                    if ( styleSheet == nullptr ) continue;
                    
                    vector<string> props = ofSplitString(tempSplit[1], ",");
                    
                    bool all = false;
                    if ( props[props.size()-1] == "ALL" ) {
                        all = true;
                        props.pop_back();
                    }
                    
                    if ( all ){
                        for ( int i = 0; i < 3; i++){
                            if ( styleSheet->HasImage( props[0] ) ){
                                if ( props.size() == 1 ) bgImage[i].Set( styleSheet->GetImage(props[0]) );
                                else if ( props.size() == 2 ) bgImage[i].Set( styleSheet->GetImage(props[0]), props[1] );
                                else if ( props.size() == 3 ) bgImage[i].Set( styleSheet->GetImage(props[0]), ofToInt(props[1]), ofToInt(props[2]) );
                                else if ( props.size() == 5 ) bgImage[i].Set( styleSheet->GetImage(props[0]), ofToInt(props[1]), ofToInt(props[2]), ofToInt(props[3]), ofToInt(props[4]) );
                            } else {
                                ofLog( OF_LOG_WARNING ) << "[" << GetName() << "] image id(" << props[0] << ") doesn't exist";
                            }
                        }
                    } else {
                        if ( styleSheet->HasImage( props[0] ) ){
                            if ( props.size() == 1 ) bgImage[state].Set( styleSheet->GetImage(props[0]) );
                            else if ( props.size() == 2 ) bgImage[state].Set( styleSheet->GetImage(props[0]), props[1] );
                            else if ( props.size() == 3 ) bgImage[state].Set( styleSheet->GetImage(props[0]), ofToInt(props[1]), ofToInt(props[2]) );
                            else if ( props.size() == 5 ) bgImage[state].Set( styleSheet->GetImage(props[0]), ofToInt(props[1]), ofToInt(props[2]), ofToInt(props[3]), ofToInt(props[4]) );
                        } else {
                            ofLog( OF_LOG_WARNING ) << "[" << GetName() << "] image id(" << props[0] << ") doesn't exist";
                        }
                    }
                    
                    
                } if (tempSplit[0] == "image" || tempSplit[0] == "img" ) {
                    if ( styleSheet == nullptr ) continue;
                    //images[state][tempSplit[1]] = styleSheet->GetImage(tempSplit[1]);
                    //this->style[state][tempSplit[1]] = "image";
                    
                    // TODO: multiple images per state
                    //imageIDs[state].push_back(tempSplit[1]);
                    //images[tempSplit[1]] = styleSheet->GetImage(tempSplit[1]);
                    
                    if ( styleSheet->HasImage(tempSplit[1]) ){
                        imageIDs[state].clear();
                        
                        imageIDs[state].push_back(tempSplit[1]);
                        images[tempSplit[1]] = styleSheet->GetImage(tempSplit[1]);
                    } else {
                        ofLog( OF_LOG_WARNING ) << "[" << GetName() << "] image id(" << tempSplit[1] << ") doesn't exist";
                    }
                }
                else if (tempSplit[0] == "video") {
                    
                } else if (tempSplit[0] == "rotation"){
                    SetRotation( ofToFloat(tempSplit[1]) );
                } else if (tempSplit[0] == "scale"){
                    SetScale( ofToFloat(tempSplit[1]) );
                } else if (tempSplit[0] == "anchorPoint" || tempSplit[0] == "anchor-point" ) {
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
                    float spacing = 1.0;
                    
                    if ( fontProps.size() == 2 ) size = ofToInt(fontProps[1]);
                    else if ( fontProps.size() == 3) {
                        size = ofToInt(fontProps[1]);
                        spacing = ofToFloat(fontProps[2]);
                    }
                    if ( VUI::HasFont( fontProps[0], size, spacing ) ) {
                        font = VUI::GetFont( fontProps[0], size, spacing );
                    } else {
                        font = VUI::AddFont( fontProps[0], size, spacing );
                    }
                }
                else if (tempSplit[0] == "border" ){
                    vector<string> props = ofSplitString(tempSplit[1], ",");
                    if ( props.size() < 2 ) {
                        if ( props.size() == 1 && props[0] == "clear" ) {
                            for ( vector<string>::iterator it = borderProps.begin(); it != borderProps.end(); it++ ){
                                this->styleInt[state][(*it)] = 0;
                            }
                        }
                        continue;
                    }
                    if ( props[1].find("#") == string::npos ) continue;
                    
                    string str(props[1]);
                    ofStringReplace(str, "#", "");
                    
                    //ofLog() << "border: " << props[0] << ":" << props[1];
                    
                    //ofLog() << str << " => " << stoul(str, nullptr, 16);
                    //if ( name == "#leftCenter" ) ofLog() << tempSplit[0] << " => " << str << " => " << stoul(str, nullptr, 16);
                    
                    if ( props.size() == 3 && props[2] == "ALL" ){
                        for ( int i = 0; i < 3; i++ ){
                            for ( vector<string>::iterator it = borderProps.begin(); it != borderProps.end(); it++ ){
                                this->styleInt[i][(*it)] = ofToInt(props[0]);
                                this->styleFloat[i][(*it)] = stoul(str, nullptr, 16);
                            }
                        }
                    } else {
                        for ( vector<string>::iterator it = borderProps.begin(); it != borderProps.end(); it++ ){
                            this->styleInt[state][(*it)] = ofToInt(props[0]);
                            this->styleFloat[state][(*it)] = stoul(str, nullptr, 16);
                        }
                    }
                    
                } else if ( tempSplit[0].find("border-") != string::npos ){
                    vector<string> props = ofSplitString(tempSplit[1], ",");
                    if ( props.size() < 2 ) {
                        if ( props.size() == 1 && props[0] == "clear" ) {
                            this->styleInt[state][tempSplit[0]] = 0;
                        }
                        continue;
                    }
                    if ( props[1].find("#") == string::npos ) continue;
                    
                    string str(props[1]);
                    ofStringReplace(str, "#", "");
                    
                    //if ( name == "#leftCenter" ) ofLog() << tempSplit[0] << " => " << str << " => " << stoul(str, nullptr, 16);
                    
                    if ( props.size() == 3 && props[2] == "ALL" ){
                        for ( int i = 0; i < 3; i++ ){
                            this->styleInt[i][tempSplit[0]] = ofToInt(props[0]);
                            this->styleFloat[i][tempSplit[0]] = stoul(str, nullptr, 16);
                        }
                    } else {
                        this->styleInt[state][tempSplit[0]] = ofToInt(props[0]);
                        this->styleFloat[state][tempSplit[0]] = stoul(str, nullptr, 16);
                    }
                    
                } else if ( tempSplit[0] == "offset" ) {
                    vector<string> props = ofSplitString(tempSplit[1], ",");
                    if ( props.size() != 2 ) continue;
                    this->style[state]["offset-x"] = props[0];
                    this->style[state]["offset-y"] = props[1];
                } else if ( tempSplit[0] == "bg-position" || tempSplit[0] == "bgPosition" ) {
                    vector<string> props = ofSplitString(tempSplit[1], ",");
                    if ( props.size() != 2 ) continue;
                    this->style[state]["bg-position-x"] = props[0];
                    this->style[state]["bg-position-y"] = props[1];
                } else if ( tempSplit[0] == "bg-size" || tempSplit[0] == "bgSize" ) {
                    vector<string> props = ofSplitString(tempSplit[1], ",");
                    if ( props.size() != 2 ) continue;
                    this->style[state]["bg-size-x"] = props[0];
                    this->style[state]["bg-size-y"] = props[1];
                    //ofLog() << this->style[state]["bg-size-x"] << "x" << this->style[state]["bg-size-y"];
                } else if (tempSplit[0] == "opacity" ) {
                    vector<string> props = ofSplitString(tempSplit[1], ",");
                    if ( props.size() == 1 ) {
                        this->styleFloat[state][tempSplit[0]] = ofToFloat(props[0]) * 255.0;
                        this->style[state][tempSplit[0]] = props[0];
                    }
                    else if ( props.size() == 2 && props[1] == "ALL" ){
                        for ( int i = 0; i < 3; i++ ){
                            this->styleFloat[i][tempSplit[0]] = ofToFloat(props[0]) * 255.0;
                            this->style[i][tempSplit[0]] = props[0];
                        }
                    }
                } else if (tempSplit[0] == "bgOpacity" || tempSplit[0] == "background-opacity" || tempSplit[0] == "bg-opacity" ) {
                    tempSplit[0] = "background-opacity";
                    this->style[state][tempSplit[0]] = tempSplit[1];
                } else {
                    if ( tempSplit[0] == "bg" || tempSplit[0] == "bgColor" || tempSplit[0] == "backgroundColor" ) tempSplit[0] = "background-color";
                    
                    this->style[state][tempSplit[0]] = tempSplit[1];
                    
                    if (state == VUI_STATE_UP) {
                        if (tempSplit[0] == "width" || tempSplit[0] == "height" ) {
                            /*for (int i = 0; i < 3; i++) {
                             this->style[i][tempSplit[0]] = tempSplit[1];
                             }*/
                            
                            if ( tempSplit[0] == "width" ) {
                                percentCalcValues.parseValue("width", tempSplit[1] );
                            } else if ( tempSplit[0] == "height" ) {
                                percentCalcValues.parseValue("height", tempSplit[1] );
                            }
                            
                            //ofLog() << "w:" << width << " x h:" << height;
                        }
                    }
                }
                
                StorePropValue( tempSplit[0], tempSplit[1] );
            }
        }
        
        if ( initState ) hasState[state] = true;
        
        ParseStyle( "", state );
        Update( -1, -1, true );
        Render();
        
        return this;
    }
}
