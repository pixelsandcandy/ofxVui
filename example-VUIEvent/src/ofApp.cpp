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
    )";
    
    ss = new StyleSheet( styles );
    
    square = new Element( ofGetWidth()*.5, ofGetHeight()*.5, ss, "#square" );
    rectangle = new Element( ofGetWidth()*.5, ofGetHeight()*.5, ss, "#rectangle" );
    
    
    // ------------------------------------------------------------------- MOUSE EVENTS
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
    
}

void ofApp::vuiEventHandler(vuiEventArgs& evt){
    string s = "[" + evt.element->GetName() + "] ";
    
    switch( evt.eventType ){
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
            s += "VUI_EVENT_MOUSE_DRAGGED";
            break;
        case VUI_EVENT_MOUSE_RELEASED:
            s += "VUI_EVENT_MOUSE_RELEASED";
            break;
        case VUI_EVENT_MOUSE_CLICK:
            s += "VUI_EVENT_MOUSE_CLICK";
            break;
        case VUI_EVENT_MOUSE_DOUBLE_CLICK:
            s += "VUI_EVENT_MOUSE_CLICK";
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
