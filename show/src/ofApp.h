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
    
        void onToggleEvent(ofxDatGuiButtonEvent e);
        void onMatrixEvent(ofxDatGuiMatrixEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
    
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
