#pragma once
#ifndef __ViewExample__
#define __ViewExample__

#include "ofMain.h"
#include "ofxVui.h"

using namespace VUI;

class ViewExample : public View {
public:
	~ViewExample() {};
	ViewExample() {};

    Element* element;
    StyleSheet* ss;

	void setup() {
        string styles = R"(
        [.button>
         
         width: 150;
         height: 150;
         background-color: #ff0000;
         
         &:over {
             background-color: #00ff00;
         }
         
         &:down {
             background-color: #0000ff;
         }
         ]
        )";
        
        ss = new StyleSheet( styles );
        
        element = new Element( 20, 20, ss, ".button" );
        ofAddListener(element->onMouseClick, this, &ViewExample::vuiEventHandler );
	}

	void Exit() {
		
	}

	void Enter() {
		
	}
    
    void vuiEventHandler(vuiEventArgs& evt) {
        if ( evt.element == element ){
            if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ){
                ofLog() << "yay!";
            }
        }
    }
    
	void update() {
		
	}


	void draw() {
        element->Render();
	}

	void mousePressed(int x, int y, int button) {
        
	}

	void touchDown(int x, int y, int id) {
        
	}

};


#endif
