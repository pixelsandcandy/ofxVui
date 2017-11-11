#include "ofxVui.h"
#include "VUIView.h"

namespace VUI {
	View::~View() {};
	View::View() {};

	void View::ExitView() {
		VUI::Next();
	}
}
