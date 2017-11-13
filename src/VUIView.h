#pragma once

#ifndef __ofxViewManager_View__
#define __ofxViewManager_View__

#include "ofMain.h"

namespace VUI {
	class View {
	public:
		virtual ~View();
		View();

		int mouseX = 0;
		int mouseY = 0;

		virtual void OnEnterView() {};
        
        /*
         if overriding BeforeExitView(), you must call ExitView()
         when you're done with what you need to do
         */
		virtual void BeforeExitView() {
            ExitView();
        };
        
        // -------------------------------------------------------- the usual suspects
		
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
        
        // --------------------------------------------------------
        
        void Render() {
            draw();
        };
        
        //
        
        void UpdateMousePos(int x, int y) {
            mouseX = x;
            mouseY = y;
        }
        
        //
        bool _didSetup = false;
        void _Setup(){
            if (_didSetup) return;
            else {
                _didSetup = true;
                setup();
            }
        }
        
        void _TriggerExitView(){
            ExitView();
        }
	protected:
		void ExitView();
	};
}

#endif
