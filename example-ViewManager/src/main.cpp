#include "ofMain.h"
#include "ofApp.h"
#include "ofxVui.h"

//========================================================================
int main( ){
    VUI::SetResolution(1920,1080,.6);
    //VUI::SetResolution(1920,1080);
    
    ofSetupOpenGL(VUI::GetWindowWidth(),VUI::GetWindowHeight(),OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
