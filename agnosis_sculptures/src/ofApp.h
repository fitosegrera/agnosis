#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxJSON.h"
//#include "ofxCairo.h"

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
    
        // ofMesh createMemoMesh(ofTexture &tex, float n1, float n2);
        // ofTexture processTexture(ofTexture &tex, float n);
        ofxCvBlob getOneBlobFromImage(ofPixels& pix , ofxJSONElement& data);
        ofMesh createMeshByBlob(ofxCvBlob& blob , ofxJSONElement& data);
    
	    ofEasyCam camera;
	    
	    vector<ofTexture> textures;
	    int memoCount;
	    // vector<ofMesh> memoMeshs;

	//    int imgIndex;
	//    bool flag1, flag2;
	//    ofFbo alphamask;
	//    ofTexture maskTexture;

	    vector<ofMesh> maskMeshes;
	    vector<ofVec3f> translations;
	    vector<ofVec4f> rotations; //4TH DIMENSION IS SPEED OF ROTATION
	    
	    float radius;
	    vector<ofxJSONElement> JSONObjects;
	    string prefix;

	    ofImage bg;
	    bool showBg;

	   //ofxCairo *pdf;
    
};
