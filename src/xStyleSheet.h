#pragma once

#ifndef __ofxVui_StyleSheet__
#define __ofxVui_StyleSheet__

#include "ofMain.h"

namespace VUI {

	class StyleSheet {
	
	
	public:
		virtual ~StyleSheet();
		StyleSheet( string ss = "");

		map<string, ofImage> assets;

		map<string, string> classes;
		map<string, string> ids;

		map<string, ofImage*> images;
		map<string, ofVideoPlayer*> videos;

		void Process(string ss);
		string GetStyleByClass(string name);
		string GetStyleByID(string name);

		void AddImage(string id, string fileName);
		void AddVideo(string id, string fileName);

        bool HasImage( string id );
		ofImage* GetImage(string id);
		ofVideoPlayer* GetVideo(string id);

		static string ExtractStyleByState(string style, int state);
	};
}

#endif
