#pragma once
#ifndef __ViewB__
#define __ViewB__

#include "ofMain.h"
#include "ofxVui.h"

using namespace VUI;

class ViewB : public View {
public:
	~ViewB() {};
	ViewB() {};

    //
    
    TextBox* label;
    TextBox* resolution;
    
    TextBox* btnViewA;
    TextBox* btnViewATriggerExit;
    
    Element* box;
    Tween* boxTween;
    
    //
    
    void setup() {
        
        // view name - using StyleSheet made in ViewA
        label = new TextBox( 20, 20, VUI::GetStyleSheet("ViewAStyles"), ".view-label" );
        label->SetText( "View B" );
        
        // resolution (main.cpp)
        resolution = new TextBox( VUI::GetResolutionX() - 20, VUI::GetResolutionY() - 20, VUI::GetStyleSheet("ViewAStyles"), ".resolution" );
        resolution->SetText( ofToString(VUI::GetResolutionX()) + "x" + ofToString(VUI::GetResolutionY()) + "   scale=" + ofToString(VUI::GetScale()) );
        
        // buttonZ
        btnViewA = new TextBox( 300, 120, VUI::GetStyleSheet("ViewAStyles"), ".btn-setView" );
        btnViewA->SetText( "SetView(\"view-a\")" );
        
        btnViewATriggerExit = new TextBox( 300, 120 + 60 + 10, VUI::GetStyleSheet("ViewAStyles"), ".btn-setView" );
        btnViewATriggerExit->SetText( "SetView(\"view-a\", true)" );
        
        // events
        ofAddListener( btnViewA->onMouseClick, this, &ViewB::vuiEventHandler );
        ofAddListener( btnViewATriggerExit->onMouseClick, this, &ViewB::vuiEventHandler );
        
        // exit animation element
        box = new Element( 1200, 400, VUI::GetStyleSheet("ViewAStyles"), ".box", "#boxB" );
	}
    
    void vuiEventHandler(vuiEventArgs& evt) {
        if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ){
            if ( evt.element == btnViewA ) VUI::SetView( "view-a" );
            else if ( evt.element == btnViewATriggerExit ) VUI::SetView( "view-a", true );
        }
        
        // must call ExitView() if overriding BeforeExitView()
        if ( evt.tween == boxTween && evt.eventType == VUI_EVENT_ANIMATE_COMPLETE ) ExitView();
    }
    
	void update() {
		
	}
    
    void OnEnterView(){
        box->Set( "{rotation:0, scale: 1, x: 1200}" );
    }
    
    void BeforeExitView(){
        // a different way to find event using vuiEventArgs.tween
        boxTween = box->Animate( 1, "{rotation: 135, easing:Elastic.easeOut, scale: 2}", this, &ViewB::vuiEventHandler );
    }

	void draw() {
        label->Render();
        
        btnViewA->Render();
        btnViewATriggerExit->Render();
        
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
