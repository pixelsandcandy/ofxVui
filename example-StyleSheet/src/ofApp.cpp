#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    string styles = R"(
        [.baseClass>
             width: 160;
             height: 40;
             bg: #ff0000;
             border: 1, #000000;
         
             &:over {
                bg: #405c6c;
                border: 2, #00ffff;
             }
         
             &:down {
                background-color: #7ccbf9;
             }
        ]
    
    
    
        [#leftTop>
             border: 1, #ffffff, ALL;
             &:over{
                 border-left: 5, #f907df;
             }
         ]
    
        [#leftCenter>
             anchorPoint: left-center;
             &:over{
                 border-left: 5, #f907df;
             }
         ]
    
        [#leftBottom>
             anchorPoint: left-bottom;
             &:over{
                 border-left: 5, #f907df;
             }
         ]
    
    
    
        [#centerTop>
             anchorPoint: center-top;
             opacity: .1;
             &:over{
                 border-top: 5, #f907df;
             }
         ]
    
        [#centerCenter>
             anchorPoint: center-center;
             width: 200;
             height: 200;
             scale: .5;
             rotation: 45;
             bg: #ffff00;
             border: clear;
         
             &:over{
                 opacity: .2;
             }
         ]
    
        [#centerBottom>
         
             anchorPoint: center-bottom;
             &:over{
                 border-bottom: 5, #f907df;
             }
             &:down{
                 opacity: .1;
             }
         ]
    
    
    
        [#rightTop>
             anchorPoint: right-top;
             &:over{
                 border-right: 5, #f907df;
             }
         ]
    
        [#rightCenter>
             anchorPoint: right-center;
             &:over{
                 border-right: 5, #f907df;
             }
         ]
    
        [#rightBottom>
             anchorPoint: right-bottom;
             opacity: .1, ALL;
             &:over{
                 border-right: 5, #f907df;
             }
         ]
    
    )";
    
    ss = new StyleSheet( styles );
    
    
    //-------------------------------------------------------------- STYLES
    /*
     
     The primary style class/ID used is applied to ALL states.
     • VUI_STATE_UP
     • VUI_STATE_OVER
     • VUI_STATE_DOWN
     
     
     The secondary style class/ID overrides properties for specific states.
     Some properties have an ALL flag that will override ALL states to make
     it behave like a primary style class/ID.
     • opacity
     • border
     
     
     --------------------------------------------------------------
     
     • width                          (applies to all states)
     • height                         (applies to all states)
     
     • background-color               #hexColor -or- "clear"
         + backgroundColor
         + bgColor
         + bg
     
     • color                          text color
     • font                           fontFilepath, fontSize
     • scale
     • opacity                        0.0 - 1.0, ALL (optional)
     • rotation
     
     
     • anchor-point
         + anchorPoint
     
     • text-align
         + textAlign
     
                                    - left-top
                                    - left-center
                                    - left-bottom
                                    - center-top
                                    - center-center
                                    - center-bottom
                                    - right-top
                                    - right-center
                                    - right-bottom
     
     
     • border                         thickness, #hexColor, ALL (optional) -or- "clear" (turns border off)
         ∟ border-top
         ∟ border-right
         ∟ border-bottom
         ∟ border-left
     
     
     • offset                         (not based on anchorPoint, from left-top)
         ∟ offset-x
         ∟ offset-y
     
     
     --------------------------------------------------------------
     
     STATES - must be appended to end of style block
     
     • &:over{<styles>}
     • &:down{<styles>}
     
     
     
     */
    
    //--------------------------------------------------------------
    
    
    leftTop = new Element( 0,0, ss, ".baseClass", "#leftTop" );
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
