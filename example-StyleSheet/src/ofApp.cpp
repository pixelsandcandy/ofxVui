#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    string styles = R"(
        [.baseClass>
             width: 160;
             height: 40;
             background-color: #ff0000;
         
             &:over {
                background-color: #00ff00;
             }
         
             &:down {
                background-color: #0000ff;
             }
        ]
    
    
    
        [#leftTop>
         
         ]
    
        [#leftCenter>
             anchorPoint: left-center;
         ]
    
        [#leftBottom>
             anchorPoint: left-bottom;
         ]
    
    
    
        [#centerTop>
             anchorPoint: center-top;
         ]
    
        [#centerCenter>
             anchorPoint: center-center;
             width: 200;
             height: 200;
             scale: .5;
             rotation: 45;
             opacity: .5;
             background-color: #ffff00;
         ]
    
        [#centerBottom>
             anchorPoint: center-bottom;
         ]
    
    
    
        [#rightTop>
             anchorPoint: right-top;
         ]
    
        [#rightCenter>
             anchorPoint: right-center;
         ]
    
        [#rightBottom>
             anchorPoint: right-bottom;
         ]
    
    )";
    
    ss = new StyleSheet( styles );
    
    
    //-------------------------------------------------------------- Styles
    /*
     
     width                  (applies to all states)
     height                 (applies to all states)
     
     background-color       #XXXXXX or "transparent"
     color                  text color
     scale
     opacity
     rotation
     anchor-point           -or- anchorPoint
     text-align             -or- textAlign
     
     
     ---------------------- STATES
     &:over{<styles>}
     &:down{<styles>}
     
     */
    
    //--------------------------------------------------------------
    
    
    leftTop = new Element( 0,0, ss, ".baseClass" );
    leftCenter = new Element( 0, ofGetHeight()*.5, ss, ".baseClass", "#leftCenter" );
    leftBottom = new Element( 0, ofGetHeight(), ss, ".baseClass", "#leftBottom" );
    
    centerTop = new Element( ofGetWidth()*.5,0, ss, ".baseClass", "#centerTop" );
    centerCenter = new Element( ofGetWidth()*.5,ofGetHeight()*.5, ss, ".baseClass", "#centerCenter" );
    centerBottom = new Element( ofGetWidth()*.5,ofGetHeight(), ss, ".baseClass", "#centerBottom" );
    
    rightTop = new Element( ofGetWidth(),0, ss, ".baseClass", "#rightTop" );
    rightCenter = new Element( ofGetWidth(),ofGetHeight()*.5, ss, ".baseClass", "#rightCenter" );
    rightBottom = new Element( ofGetWidth(),ofGetHeight(), ss, ".baseClass", "#rightBottom" );
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    leftTop->Render();
    leftCenter->Render();
    leftBottom->Render();
    
    centerTop->Render();
    centerCenter->Render();
    centerBottom->Render();
    
    rightTop->Render();
    rightCenter->Render();
    rightBottom->Render();
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
