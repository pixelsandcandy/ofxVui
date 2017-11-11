#include "ofxVui.h"
#include "VUIStyleSheet.h"
#include <iostream>
#include <string>

namespace VUI {

	StyleSheet::~StyleSheet() {
        VUI::Init();
        
	}
	StyleSheet::StyleSheet( string ss ) {
        VUI::Init();
        
		Process( ss );
	}
    StyleSheet::StyleSheet( string ss, string name ){
        VUI::Init();
        
        Process( ss );
        VUI::AddStyleSheet( name, this );
    }

	void StyleSheet::AddImage(string id, string fileName) {
		ofImage *img = new ofImage();
		img->load(fileName);

		images[id] = img;
	}

	void StyleSheet::AddVideo(string id, string fileName) {
		ofVideoPlayer *video = new ofVideoPlayer();
		video->load(fileName);

		videos[id] = video;
	}

	ofVideoPlayer* StyleSheet::GetVideo(string id) {
		return videos[id];
	}

	ofImage* StyleSheet::GetImage(string id) {
		return images[id];
	}
    
    bool StyleSheet::HasImage( string id ){
        if ( images[id] != nullptr ) return true;
        return false;
    }



	void StyleSheet::Process(string ss) {
		string s(ss);
		ofStringReplace(s, " ", "");
		ofStringReplace(s, "\n", "");
		ofStringReplace(s, "	", "");

		vector<string> split = ofSplitString(s, "]");

		vector<string> tempSplit;
		string name;
		string classifier;

		for (vector<string>::iterator it = split.begin(); it != split.end(); it++) {
			tempSplit = ofSplitString((*it), ">");
			if (tempSplit.size() == 2) {
				name = tempSplit.at(0).substr(2);
				
				classifier = tempSplit.at(0).substr(1, 1);

				//cout << "name: " << name << endl;
				//cout << "[" << classifier << name << "] => " << tempSplit.at(1) << endl;

				if (classifier == ".") {
					//classes[ name ] 
					//cout << "." << name << " => " << tempSplit.at(1) << endl;
					classes[name] = tempSplit.at(1);
				} else if (classifier == "#") {
					ids[name] = tempSplit.at(1);

					//cout << "#" << name << " => " << tempSplit.at(1) << endl;
				}
				else if (tempSplit.at(0).substr(1) == "Images" ){
					tempSplit = ofSplitString(tempSplit.at(1), ";");

					vector<string> assets;

					for (vector<string>::iterator it = tempSplit.begin(); it != tempSplit.end(); it++) {
						assets = ofSplitString((*it), ":");
						
						if (assets.size() == 2) {
							//cout << assets[0] << " > " << assets[1] << endl;
							AddImage(assets[0], assets[1]);
						}
					}
				}
				else if (tempSplit.at(0).substr(1) == "Videos") {
					tempSplit = ofSplitString(tempSplit.at(1), ";");

					vector<string> assets;

					for (vector<string>::iterator it = tempSplit.begin(); it != tempSplit.end(); it++) {
						assets = ofSplitString((*it), ":");

						if (assets.size() == 2) {
							//cout << assets[0] << " > " << assets[1] << endl;
							AddVideo(assets[0], assets[1]);
						}
					}
				}

				/*if (tempSplit.at(0).at(1) == ".") {
					//classes[ name ] = 
					cout << tempSplit.at(1) << endl;
				}*/
			}
		}
	}

	string StyleSheet::GetStyleByClass(string name) {
		return classes[name];
	}

	string StyleSheet::GetStyleByID(string name) {
		return ids[name];
	}


	string StyleSheet::ExtractStyleByState(string style, int state) {
		vector<string> split = ofSplitString(style, "&");
		if (state == VUI_STATE_UP) return split.at(0);

		vector<string> tempSplit;
		for (vector<string>::iterator it = split.begin()+1; it != split.end(); it++) {
			//cout << "(*it) => " << (*it) << endl;

			tempSplit = ofSplitString((*it), "}");
			tempSplit = ofSplitString(tempSplit.at(0), "{");

			//cout << "[0] => " << tempSplit.at(0) << endl;
			//cout << "[1] => " << tempSplit.at(1) << endl;

			if ((state == VUI_STATE_OVER && tempSplit.at(0).find("over") != string::npos) ||
				(state == VUI_STATE_DOWN && tempSplit.at(0).find("down") != string::npos)) {
				return tempSplit.at(1);
			}
			
		}

		return "";
	}


}
