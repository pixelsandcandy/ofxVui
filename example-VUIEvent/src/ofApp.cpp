#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    string styles = R"(
        [#square>
         
         width: 400;
         height: 400;
         background-color: #ff0000;
         anchorPoint: center-center;
         
         &:over {
            background-color: #00ff00;
         }
         
         &:down {
            background-color: #0000ff;
         }
        ]
    
        [#rectangle>
         
             width: 200;
             height: 60;
             background-color: #ffffff;
             anchorPoint: center-center;
         
             &:over {
                 background-color: #00ff00;
             }
         
             &:down {
                 background-color: #0000ff;
             }
         ]
    
        [#textField>
             width: 240;
             height: 36;
             background-color: #aeaeae;
             font: Gotham-Medium.otf,13;
             text-align: left-center;
             color: #000000;
             padding-left: 10;
         
             &:over{
                 background-color: #7cf9ae;
             }
         
             &:down{
                 background-color: #00ff00;
             }
         ]
    
    
    )";
    
    ss = new StyleSheet( styles );
    
    textField = new TextField( 50, 50, ss, "#textField" );
    textField->SetText("omg textfield");
    
    square = new Element( 760, ofGetHeight()*.5, ss, "#square" );
    rectangle = new Element( 760, ofGetHeight()*.5, ss, "#rectangle" );
    
    
    // ------------------------------------------------------------------- TextField / Toggle EVENTS
    ofAddListener( textField->onFocus, this, &ofApp::vuiEventHandler );
    ofAddListener( textField->onUnfocus, this, &ofApp::vuiEventHandler );
    
    
    // ------------------------------------------------------------------- TextField EVENTS
    ofAddListener( textField->onSubmit, this, &ofApp::vuiEventHandler );
    
    
    // ------------------------------------------------------------------- Mouse EVENTS
    /*
         VUI_EVENT_MOUSE_OVER,
         VUI_EVENT_MOUSE_OUT,
         VUI_EVENT_MOUSE_PRESSED,
         VUI_EVENT_MOUSE_MOVED,
         VUI_EVENT_MOUSE_DRAGGED,
         VUI_EVENT_MOUSE_RELEASED,
         VUI_EVENT_MOUSE_CLICK,
         VUI_EVENT_MOUSE_DOUBLE_CLICK
     
     */
    
    ofAddListener( square->onMouseOver, this, &ofApp::vuiEventHandler );
    ofAddListener( square->onMouseOut, this, &ofApp::vuiEventHandler );
    ofAddListener( square->onMousePressed, this, &ofApp::vuiEventHandler );
    ofAddListener( square->onMouseMoved, this, &ofApp::vuiEventHandler );
    ofAddListener( square->onMouseDragged, this, &ofApp::vuiEventHandler );
    ofAddListener( square->onMouseReleased, this, &ofApp::vuiEventHandler );
    ofAddListener( square->onMouseClick, this, &ofApp::vuiEventHandler );
    ofAddListener( square->onMouseDoubleClick, this, &ofApp::vuiEventHandler );
    
    ofAddListener( rectangle->onMouseOver, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangle->onMouseOut, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangle->onMousePressed, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangle->onMouseMoved, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangle->onMouseDragged, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangle->onMouseReleased, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangle->onMouseClick, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangle->onMouseDoubleClick, this, &ofApp::vuiEventHandler );
    
    /*
        ofAddListener( textField->onMouseOver, this, &ofApp::vuiEventHandler );
        ofAddListener( textField->onMouseOut, this, &ofApp::vuiEventHandler );
        ofAddListener( textField->onMousePressed, this, &ofApp::vuiEventHandler );
        ofAddListener( textField->onMouseMoved, this, &ofApp::vuiEventHandler );
        ofAddListener( textField->onMouseDragged, this, &ofApp::vuiEventHandler );
        ofAddListener( textField->onMouseReleased, this, &ofApp::vuiEventHandler );
        ofAddListener( textField->onMouseClick, this, &ofApp::vuiEventHandler );
        ofAddListener( textField->onMouseDoubleClick, this, &ofApp::vuiEventHandler );
     
     */
    
}

void ofApp::vuiEventHandler(vuiEventArgs& evt){
    
    string s = "[" + evt.element->GetName() + "] ";
    
    
    switch( evt.eventType ){
        case VUI_EVENT_FOCUS:
            s += "VUI_EVENT_FOCUS";
            break;
        case VUI_EVENT_UNFOCUS:
            s += "VUI_EVENT_UNFOCUS";
            break;
        case VUI_EVENT_SUBMIT:
            s += "VUI_EVENT_SUBMIT   text:" + evt.text;
            ofLog() << s;
            return;
            break;
        case VUI_EVENT_MOUSE_OVER:
            s += "VUI_EVENT_MOUSE_OVER";
            break;
        case VUI_EVENT_MOUSE_OUT:
            s += "VUI_EVENT_MOUSE_OUT";
            break;
        case VUI_EVENT_MOUSE_PRESSED:
            s += "VUI_EVENT_MOUSE_PRESSED";
            break;
        case VUI_EVENT_MOUSE_MOVED:
            s += "VUI_EVENT_MOUSE_MOVED";
            break;
        case VUI_EVENT_MOUSE_DRAGGED:
            s += "VUI_EVENT_MOUSE_DRAGGED  dragStart:" + ofToString(evt.localDragStart.x) + "," + ofToString(evt.localDragStart.y) + "  dragDelta:" + ofToString(evt.localDragDelta.x) + "," + ofToString(evt.localDragDelta.y);
            break;
        case VUI_EVENT_MOUSE_RELEASED:
            s += "VUI_EVENT_MOUSE_RELEASED";
            break;
        case VUI_EVENT_MOUSE_CLICK:
            s += "VUI_EVENT_MOUSE_CLICK";
            break;
        case VUI_EVENT_MOUSE_DOUBLE_CLICK:
            s += "VUI_EVENT_MOUSE_DOUBLE_CLICK";
            break;
    }
    
    ofLog() << s << "  local:" << evt.localMousePos.x << "," << evt.localMousePos.y << "  global:" << evt.globalMousePos.x << "," << evt.globalMousePos.y;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    square->Render();
    rectangle->Render();
    textField->Render();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
