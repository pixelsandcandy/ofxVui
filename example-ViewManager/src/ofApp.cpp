#include "ofApp.h"
#include "ViewA.h"
#include "ViewB.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    // usually a transparent background
    VUI::SetBackground( 20 );
    
    // name, viewPtr, set view
    VUI::AddView( "view-a", new ViewA(), true );
    VUI::AddView( "view-b", new ViewB() );
    
    // another way to set view
    //VUI::SetView( "my-view" );
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    // VUI::Render() calls VUI::RenderBefore() and VUI::RenderAfter();
    VUI::Render();
    
    
    /*
     
     If you still want to scale/rotate your view without using VUI's
     View Manager (via AddView()/SetView() you can like this:
     
     VUI::RenderBefore();
        // draw your stuff here
     VUI::RenderAfter();
     
     
     But you MUST set the 4th arg in VUI::SetResolution() to false (main.cpp):
     VUI::SetResolution(w, h, scale, enableViewManager );
     VUI::SetResolution(1920, 1080, .6, false);
     
     */
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
