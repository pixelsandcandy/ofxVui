#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    string styles = R"(
        [#animElement>
             width: 100;
             height: 100;
             anchorPoint: center-center;
             bg: #ff0000;
         
             &:over{
                 bg: #7cf9ae;
                 border: 1, #10532b;
             }
        ]
    
        [.button>
             width: 120;
             height: 32;
             bg: #000000;
             font: Gotham-Medium.otf, 11;
             color: #ffffff;
             padding: 10;
         
             &:over{
                 border: 1, #5ab1e2;
                 bg: #377598;
             }
         
             &:down {
                 bg: #2c8d3a;
                 border: 1, #55ff7d;
             }
         ]
    
    )";
    
    ss = new StyleSheet( styles );
    
    
    //-------------------------------------------------------------- STYLES
    
    float Y = -20;
    
    animElement = new Element( 150, ofGetHeight()*.5, ss, "#animElement" );
    
    
    // make
    animationA = new Text( 20, Y+=40, ss, ".button" );
    animationB = new Text( 20, Y+=40, ss, ".button" );
    animationC = new Text( 20, Y+=40, ss, ".button" );
    
    
    // labels
    animationA->SetText( "[A]  Animate(1, {x:1100, width: 100, height: 100,scale:.5, opacity: 1, rotation: 135, ease:Elastic.easeOut})", true );
    animationB->SetText( "[B]  Animate(.5, {width: 100, height: 100, x:640, opacity:.5, scale: 1.75, rotation: -180, ease:Back.easeOut})", true );
    animationC->SetText( "[C]  Animate(1.25, {rotation: 0, width :150, height: 50, x: 150, scale:1, ease:Bounce.easeOut})", true );
    
    
    // click event
    ofAddListener( animationA->onMouseClick, this, &ofApp::vuiEventHandler );
    ofAddListener( animationB->onMouseClick, this, &ofApp::vuiEventHandler );
    ofAddListener( animationC->onMouseClick, this, &ofApp::vuiEventHandler );
    
    
}

void ofApp::vuiEventHandler(vuiEventArgs& evt){
    
    if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ){
        if ( evt.element == animationA ) {
            
            // COMPLETE Listener [Method A]
            
            tweenPtr = animElement->Animate( 1, "{x:1100, width: 100, height: 100,scale:.5, opacity: 1, rotation: 135, ease:Elastic.easeOut}", this, &ofApp::vuiEventHandler );
            tweenPtr->SetID( "Animation A" );
            
        } else if ( evt.element == animationB ) {
            
            // COMPLETE Listener [Method B]
            
            tweenPtr = animElement->Animate( .5, "{width: 100, height: 100, x:640, opacity:.5, scale: 1.75, rotation: -180, ease:Back.easeOut}" );
            tweenPtr->SetID( "Animation B" );
            
            ofAddListener( tweenPtr->onComplete, this, &ofApp::vuiEventHandler );
            
        } else if ( evt.element == animationC ) {
            
            //  Tween STEP/PROGRESS Listener
            
            tweenPtr = animElement->Animate( 1.25, "{rotation: 0, width:150, height: 50, x: 150, scale:1, ease:Bounce.easeOut}" );
            
            tweenPtr->SetID( "Animation C" );
            
            ofAddListener( tweenPtr->onStep, this, &ofApp::vuiEventHandler );
            
        }
    } else if ( evt.eventType == VUI_EVENT_ANIMATE_COMPLETE ){
        
        ofLog() << "[" << evt.tween->GetID() << "] VUI_EVENT_ANIMATE_COMPLETE";
        
    } else if ( evt.eventType == VUI_EVENT_ANIMATE_STEP ){
        
        ofLog() << "[" << evt.tween->GetID() << "] progress:" << evt.tween->GetProgress();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle( ofToString(ofGetFrameRate()) );
}

//--------------------------------------------------------------
void ofApp::draw(){
    animElement->Render();
    
    animationA->Render();
    animationB->Render();
    animationC->Render();
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
