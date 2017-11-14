#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    string styles = R"(
        [Images>
             btn-sprite: button-spritesheet.png;
         ]
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
    
        [.rectangle>
         
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
    
        [#rectangleB>
             
            width: 60;
            height: 200;
            background-color: #dddddd;
             
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
    
        [#text>
             width: 180;
             height: 40;
             color: #ffffff;
             textAlign: center-center;
             font: Gotham-Medium.otf,13;
             bgImage: btn-sprite;
         
             &:over{
                 bgImage: btn-sprite,0, 40;
             }
             &:down{
                 bgImage: btn-sprite, 0, 80;
             }
         ]
    
        [.toggle>
             width: 80;
             height: 32;
             background-color: #a2e3f3;
         
             &:over {
                 background-color: #f879fd;
             }
         
             &:down {
                 background-color: #43df6c;
             }
         ]
    
    )";
    
    //
    
    ss = new StyleSheet( styles );
    
    
    //
    
    square = new Element( 220, ofGetHeight()*.4, ss, "#square" );
    rectangle = new Element( 220, ofGetHeight()*.4, ss, ".rectangle" );
    rectangleB = new Element( 260, ofGetHeight()*.4, ss, ".rectangle", "#rectangleB" );
    
    
    //
    
    text = new Text( 640, 50, ss, "#text" );
    text->SetText("Text / Button");
    
    ofAddListener( text->onMouseClick, this, &ofApp::vuiEventHandler );
    
    textField = new TextField( 640, 100, ss, "#textField" );
    textField->SetText("omg textfield");
    
    
    //
    
    preToggle = new Element( 640, 160, ss, ".toggle" );
    
    postToggle = new Element( 640, 200, ss, ".toggle" );
    postToggle->MakeToggle();
    
    
    // ------------------------------------------------------------------- Toggle EVENTS
    ofAddListener( postToggle->onValueChange, this, &ofApp::vuiEventHandler );
    
    
    // ------------------------------------------------------------------- TextField EVENTS
    ofAddListener( textField->onFocus, this, &ofApp::vuiEventHandler );
    ofAddListener( textField->onUnfocus, this, &ofApp::vuiEventHandler );
    ofAddListener( textField->onTextChange, this, &ofApp::vuiEventHandler );
    ofAddListener( textField->onSubmit, this, &ofApp::vuiEventHandler );
    
    
    // ------------------------------------------------------------------- Mouse EVENTS
    /*
         VUI_EVENT_MOUSE_OVER
         VUI_EVENT_MOUSE_OUT
         VUI_EVENT_MOUSE_PRESSED
         VUI_EVENT_MOUSE_MOVED
         VUI_EVENT_MOUSE_DRAGGED
         VUI_EVENT_MOUSE_RELEASED
         VUI_EVENT_MOUSE_CLICK
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
    
    ofAddListener( rectangleB->onMouseOver, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangleB->onMouseOut, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangleB->onMousePressed, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangleB->onMouseMoved, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangleB->onMouseDragged, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangleB->onMouseReleased, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangleB->onMouseClick, this, &ofApp::vuiEventHandler );
    ofAddListener( rectangleB->onMouseDoubleClick, this, &ofApp::vuiEventHandler );
    
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
            StoreLog(s);
            //ofLog() << s;
            return;
            break;
        case VUI_EVENT_UNFOCUS:
            s += "VUI_EVENT_UNFOCUS";
            StoreLog(s);
            //ofLog() << s;
            return;
            break;
        case VUI_EVENT_TEXT_CHANGE:
            s += "VUI_EVENT_TEXT_CHANGE  text:" + evt.text;
            StoreLog(s);
            //ofLog() << s;
            return;
            break;
        case VUI_EVENT_VALUE_CHANGE:
            s += "VUI_EVENT_VALUE_CHANGE  value:" + ofToString(evt.value);
            StoreLog(s);
            //ofLog() << s;
            return;
            break;
        case VUI_EVENT_SUBMIT:
            s += "VUI_EVENT_SUBMIT   text:" + evt.text;
            StoreLog(s);
            //ofLog() << s;
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
    
    s += "  local:" + ofToString(evt.localMousePos.x) + "," + ofToString(evt.localMousePos.y) + "  global:" + ofToString(evt.globalMousePos.x) + "," + ofToString(evt.globalMousePos.y);
    
    StoreLog(s);
    
    //ofLog() << s << "  local:" << evt.localMousePos.x << "," << evt.localMousePos.y << "  global:" << evt.globalMousePos.x << "," << evt.globalMousePos.y;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle( ofToString(ofGetFrameRate()) );
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255,255);
    
    //
    
    square->Render();
    rectangle->Render();
    rectangleB->Render();
    textField->Render();
    text->Render();
    
    preToggle->Render();
    postToggle->Render();
    
    
    // logs
    
    int i = 0;
    int gray = 0;
    int g2 = 255;
    ofColor c;
    ofColor t;
    
    for ( vector<string>::iterator it = logs.begin(); it != logs.end(); it++){
        c.set(gray, gray, gray);
        t.set(g2, g2, g2);
        
        if ( i == 0 ) ofDrawBitmapStringHighlight( (*it), 20, 590 + i*20, c, t );
        else ofDrawBitmapStringHighlight( (*it), 20, 610 + i*20, c, t );
        i++;
        gray += 48;
        g2 -= 10;
    }
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
