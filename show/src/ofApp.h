#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxOpenCv.h"
#include "Sparticles.h"

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
    
        void onToggleEvent(ofxDatGuiButtonEvent e);
        void onMatrixEvent(ofxDatGuiMatrixEvent e);
    
    Sparticles particles;
    ofxCvContourFinder contourFinder;
    ofVideoGrabber camera;
    
    bool    drawCamera,
            drawSparkles,
            drawBackground,
            clearBackground;
    
    ofxDatGui *gui;
    
    vector<ofImage*>* images;
    
    ofFbo buffer;
    
    ofxCvColorImage			scaledImage;
    ofxCvColorImage         scaledImageFlipped;
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
};
