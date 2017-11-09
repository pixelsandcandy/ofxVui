#pragma once
#ifndef __ViewA__
#define __ViewA__

#include "ofMain.h"
#include "ofxVui.h"

using namespace VUI;

class ViewA : public View {
public:
	~ViewA() {};
	ViewA() {};
    
    //
    
    StyleSheet* ss;
    
    TextBox* label;
    TextBox* resolution;
    
    TextBox* btnViewB;
    TextBox* btnViewBTriggerExit;
    
    Element* box;
    
    //
    
	void setup() {
        // ------------------------------------------------------------------ styling
        string styles = R"(
        
        [.view-label>
             width: 200;
             height: 60;
             background-color: transparent;
             font: Gotham-Medium.otf,{{font-size}};
             color: #ffffff;
             padding: 6;
         ]
        
        [.resolution>
             width: 340;
             height: 30;
             font: Gotham-Medium.otf,{{font-size}};
             color: #ffffff;
             anchorPoint: right-bottom;
             textAlign: right-bottom;
             background-color: transparent;
         ]
        
        [.btn-setView>
             width: 400;
             height: 60;
             background-color: #ffffff;
             color: #000000;
             textAlign: center-center;
             font: Gotham-Medium.otf,18;
         
             &:over {
                 background-color: #a5e5f3;
             }
         
             &:down {
                 background-color: #7cf9ae;
             }
         ]
        
        [.box>
             width: 200;
             height: 200;
             background-color: #f422e3;
             anchorPoint: center-center;
         ]
        
        [#boxB>
             background-color: #fff227;
         ]
        
        )";
        // ------------------------------------------------------------------
        
        // scale font size so it's the same size regardless of what scale VUI resolution is (in main.cpp)
        ofStringReplace( styles, "{{font-size}}", ofToString(12*VUI::GetScale(true)) );
        
        // create new stylesheet and name/store it for global usage
        ss = new StyleSheet( styles, "ViewAStyles" );
        
        // view name
        label = new TextBox( 20, 20, ss, ".view-label" );
        label->SetText( "View A" );
        
        // resolution (in main.cpp)
        resolution = new TextBox( VUI::GetResolutionX() - 20, VUI::GetResolutionY() - 20, ss, ".resolution" );
        resolution->SetText( ofToString(VUI::GetResolutionX()) + "x" + ofToString(VUI::GetResolutionY()) + "   scale=" + ofToString(VUI::GetScale()) );
        
        // buttonZ
        btnViewB = new TextBox( 300, 120, ss, ".btn-setView" );
        btnViewB->SetText( "SetView(\"view-b\")" );
        
        btnViewBTriggerExit = new TextBox( 300, 120 + 60 + 10, ss, ".btn-setView" );
        btnViewBTriggerExit->SetText( "SetView(\"view-b\", true)" );
        
        // events
        ofAddListener( btnViewB->onMouseClick, this, &ViewA::vuiEventHandler );
        ofAddListener( btnViewBTriggerExit->onMouseClick, this, &ViewA::vuiEventHandler );
        
        // exit animation element
        box = new Element( 400, 400, ss, ".box" );
	}
    
    void vuiEventHandler(vuiEventArgs& evt) {
        if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ){
            if ( evt.element == btnViewB ) VUI::SetView( "view-b" );
            else if ( evt.element == btnViewBTriggerExit ) VUI::SetView( "view-b", true );
        }
        
        if ( evt.element == box ){
            // must call ExitView() if overriding BeforeExitView()
            if ( evt.eventType == VUI_EVENT_ANIMATE_COMPLETE ) ExitView();
        }
    }
    
	void update() {
        
	}
    
    void OnEnterView(){
        box->Set( "{rotation:0, x: 400}" );
    }
    
    void BeforeExitView(){
        box->Animate( 1, "{rotation: 585, easing:Elastic.easeOut, x:1200}", this, &ViewA::vuiEventHandler );
    }

	void draw() {
        label->Render();
        
        btnViewB->Render();
        btnViewBTriggerExit->Render();
        
        box->Render();
        resolution->Render();
	}
    
    
    /*
     
     // NEW Stuff
     
     // useful for animate in / out states for view
     virtual void OnEnterView() {};
     virtual void BeforeExitView() {
         ExitView();
     };
     
     // IMPORTANT - if overriding BeforeExitView() make sure to call this when your done
     ExitView();
     
     
     
     // OG Stuff
     
     virtual void setup() {};
     virtual void draw() {}
     virtual void update() {};
     
     virtual void windowResized(int w, int h) {}
     
     virtual void keyPressed(int key) {}
     virtual void keyReleased(int key) {}
     
     virtual void mouseMoved(int x, int y) {}
     virtual void mouseDragged(int x, int y, int button) {}
     virtual void mousePressed(int x, int y, int button) {}
     virtual void mouseReleased(int x, int y, int button) {}
     virtual void mouseScrolled(int x, int y, float scrollX, float scrollY) {}
     virtual void mouseEntered(int x, int y) {}
     virtual void mouseExited(int x, int y) {}
     
     virtual void dragEvent(ofDragInfo dragInfo) { }
     virtual void gotMessage(ofMessage msg) { }
     
     virtual void touchDown(int x, int y, int id) {};
     virtual void touchMoved(int x, int y, int id) {};
     virtual void touchUp(int x, int y, int id) {};
     
     virtual void touchDoubleTap(int x, int y, int id) {};
     virtual void touchCancelled(int x, int y, int id) {};
     
     */

};


#endif
