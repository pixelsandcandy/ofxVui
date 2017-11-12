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
    
        [#toggleGroup>
             width: 80;
             height: 80;
             anchorPoint: right-bottom;
         ]
    
        [.alignmentBtn>
             width: 20;
             height: 20;
             bg: #000000;
             &:over{
                 border: 2, #7cf9ae;
             }
             &:down{
                 bg: #7cf9ae;
             }
         ]
    
        [#toggleLabel>
             anchorPoint: right-top;
             font: Gotham-Medium.otf, 8;
         ]
    
        [#leftTop>
             anchorPoint: left-top;
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
    
    
    //-------------------------------------------------------------- ANIMATION
    
    float Y = -20;
    
    animElement = new Element( 150, ofGetHeight()*.5, ss, "#animElement" );
    
    
    // make
    animationA = new Text( 20, Y+=40, ss, ".button" );
    animationB = new Text( 20, Y+=40, ss, ".button" );
    animationC = new Text( 20, Y+=40, ss, ".button" );
    
    
    // labels
    animationA->SetText( "[A]  Animate(1, {x:1100, width: 100, height: 100,scale:.5, opacity: 1, rotation: 135, ease:Elastic.easeOut})", true );
    animationB->SetText( "[B]  Animate(.5, {x:640, width: 100, height: 100, opacity:.5, scale: 1.75, rotation: -180, ease:Back.easeOut})", true );
    animationC->SetText( "[C]  Animate(1.25, {x: 150, width :150, height: 50, scale:1, rotation: 0, ease:Bounce.easeOut})", true );
    
    
    // click event
    ofAddListener( animationA->onMouseClick, this, &ofApp::vuiEventHandler );
    ofAddListener( animationB->onMouseClick, this, &ofApp::vuiEventHandler );
    ofAddListener( animationC->onMouseClick, this, &ofApp::vuiEventHandler );
    
    
    
    //-------------------------------------------------------------- ALIGNMENT
    
    // grouping
    toggleGroup = new ToggleGroup(ofGetWidth()-20, ofGetHeight()-20, ss, "#toggleGroup");
    
    
    // label
    toggleGroupLabel = new Text(ofGetWidth()-20, ofGetHeight()-120, ss, "#toggleLabel");
    toggleGroupLabel->SetText( "AnchorPoint", true );
    toggleGroupLabel->SetWidth( 80 );
    
    float w = toggleGroup->GetWidth();
    float h = toggleGroup->GetHeight();
    
    
    // alignment buttons
    alignLeftTop = new Element(0,0, ss, ".alignmentBtn", "#leftTop" );
    alignLeftCenter = new Element(0,h*.5, ss, ".alignmentBtn", "#leftCenter" );
    alignLeftBottom = new Element(0,h, ss, ".alignmentBtn", "#leftBottom" );
    
    alignCenterTop = new Element(w*.5,0, ss, ".alignmentBtn", "#centerTop" );
    alignCenterCenter = new Element(w*.5,h*.5, ss, ".alignmentBtn", "#centerCenter" );
    alignCenterBottom = new Element(w*.5,h, ss, ".alignmentBtn", "#centerBottom" );
    
    alignRightTop = new Element(w,0, ss, ".alignmentBtn", "#rightTop" );
    alignRightCenter = new Element(w,h*.5, ss, ".alignmentBtn", "#rightCenter" );
    alignRightBottom = new Element(w,h, ss, ".alignmentBtn", "#rightBottom" );

    
    // add elements to toggle group - this uses AddChild() which renders them when only calling
    // toggleGroup->Render();
    toggleGroup->AddToggle( alignLeftTop );
    toggleGroup->AddToggle( alignLeftCenter );
    toggleGroup->AddToggle( alignLeftBottom );
    toggleGroup->AddToggle( alignCenterTop );
    toggleGroup->AddToggle( alignCenterCenter, true );
    toggleGroup->AddToggle( alignCenterBottom );
    toggleGroup->AddToggle( alignRightTop );
    toggleGroup->AddToggle( alignRightCenter );
    toggleGroup->AddToggle( alignRightBottom );
    
    
    ofAddListener( toggleGroup->onToggleChange, this, &ofApp::vuiEventHandler );
    
}

void ofApp::vuiEventHandler(vuiEventArgs& evt){
    
    
    if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ){
        if ( evt.element == animationA ) {
            
            // COMPLETE Listener [Method A]
            
            tweenPtr = animElement->Animate( 1, "{x:1100, width: 100, height: 100,scale:.5, opacity: 1, rotation: 135, ease:Elastic.easeOut}", this, &ofApp::vuiEventHandler );
            tweenPtr->SetID( "Animation A" );
            
        } else if ( evt.element == animationB ) {
            
            // COMPLETE Listener [Method B]
            
            tweenPtr = animElement->Animate( .5, "{x:640, width: 100, height: 100, opacity:.5, scale: 1.75, rotation: -180, ease:Back.easeOut}" );
            tweenPtr->SetID( "Animation B" );
            
            ofAddListener( tweenPtr->onComplete, this, &ofApp::vuiEventHandler );
            
        } else if ( evt.element == animationC ) {
            
            //  Tween STEP/PROGRESS Listener
            
            tweenPtr = animElement->Animate( 1.25, "{x: 150, width:150, height: 50, scale:1, rotation: 0, ease:Bounce.easeOut}" );
            
            tweenPtr->SetID( "Animation C" );
            
            ofAddListener( tweenPtr->onStep, this, &ofApp::vuiEventHandler );
            
        }
    } else if ( evt.eventType == VUI_EVENT_ANIMATE_COMPLETE ){
        
        ofLog() << "[" << evt.tween->GetID() << "] VUI_EVENT_ANIMATE_COMPLETE";
        
    } else if ( evt.eventType == VUI_EVENT_ANIMATE_STEP ){
        
        ofLog() << "[" << evt.tween->GetID() << "] progress:" << evt.tween->GetProgress();
    }
    
    
    
    // change alignment
    
    if ( evt.eventType == VUI_EVENT_TOGGLE_CHANGE ){
        if ( evt.value == 1 ) {
            if ( evt.element == alignLeftTop ) animElement->SetAnchorPoint( VUI_ALIGN_LEFT_TOP );
            else if ( evt.element == alignLeftCenter ) animElement->SetAnchorPoint( VUI_ALIGN_LEFT_CENTER );
            else if ( evt.element == alignLeftBottom ) animElement->SetAnchorPoint( VUI_ALIGN_LEFT_BOTTOM );
            else if ( evt.element == alignCenterTop ) animElement->SetAnchorPoint( VUI_ALIGN_CENTER_TOP );
            else if ( evt.element == alignCenterCenter ) animElement->SetAnchorPoint( VUI_ALIGN_CENTER_CENTER );
            else if ( evt.element == alignCenterBottom ) animElement->SetAnchorPoint( VUI_ALIGN_CENTER_BOTTOM );
            else if ( evt.element == alignRightTop ) animElement->SetAnchorPoint( VUI_ALIGN_RIGHT_TOP );
            else if ( evt.element == alignRightCenter ) animElement->SetAnchorPoint( VUI_ALIGN_RIGHT_CENTER );
            else if ( evt.element == alignRightBottom ) animElement->SetAnchorPoint( VUI_ALIGN_RIGHT_BOTTOM );
        } else {
            animElement->SetAnchorPoint( VUI_ALIGN_CENTER_CENTER );
            alignCenterCenter->SetSelected();
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle( ofToString(ofGetFrameRate()) );
}

//--------------------------------------------------------------
void ofApp::draw(){
    animElement->Render();
    
    ofSetColor(255,255,255,255);
    ofDrawCircle( animElement->GetPosition(), 3 );
    
    animationA->Render();
    animationB->Render();
    animationC->Render();
    
    toggleGroup->Render();
    toggleGroupLabel->Render();
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
