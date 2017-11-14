#pragma once

#include "ofMain.h"
#include "ofxVui.h"

using namespace VUI;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //
    
    void vuiEventHandler(vuiEventArgs& evt);
    
    //
    
    Element* square;
    Element* rectangle;
    Element* rectangleB;
    
    //
    
    Element* preToggle;
    Element* postToggle;
    
    //
    
    TextField* textField;
    Text* text;
    StyleSheet* ss;
    
    //
    
    vector<string> logs;
    
    void StoreLog(string log){
        vector<string>::iterator it = logs.begin();
        logs.insert( it, log );
        
        if ( logs.size() > 5 ) logs.pop_back();
    }
		
};
