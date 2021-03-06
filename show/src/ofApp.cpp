#include "ofApp.h"

#define VID_WIDTH 1280.0f//640.0f
#define VID_HEIGHT 720.0f //360.0f

#define PREVIEW_WIDTH 1280.0f
#define PREVIEW_HEIGHT 720.0f

#define OUT_WIDTH 1920.0f
#define OUT_HEIGHT 1080.0f

//--------------------------------------------------------------
void ofApp::setup(){
    
    camera.initGrabber(VID_WIDTH, VID_HEIGHT);
    scaledImage.allocate(VID_WIDTH, VID_HEIGHT);
    scaledImageFlipped.allocate(VID_WIDTH, VID_HEIGHT);
    grayImage.allocate(VID_WIDTH, VID_HEIGHT);
    grayBg.allocate(VID_WIDTH, VID_HEIGHT);
    grayDiff.allocate(VID_WIDTH, VID_HEIGHT);
    
    particles.setup();
    
    float fullWidth = ofGetWidth();
    float fullHeight = ofGetHeight();
    float screenWidth = ofGetScreenWidth();
    float outputWidth = fullWidth;

    buffer.allocate(OUT_WIDTH, OUT_HEIGHT);
    
    images = new vector<ofImage*>();
    
    ofxNestedFileLoader loader;
    loader.findNestedFilePaths("backgrounds");
    vector<string> backgroundNames = loader.getPaths();
    for(int i = 0; i < backgroundNames.size(); i++) {
        ofImage *img;
        img = new ofImage();
        img->load(backgroundNames[i]);
        images->push_back(img);
    }
        
    ofBackground(0);
    
    vector<int> selected;
    selected.push_back(0);
    
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    gui->addFRM();
    auto toggle = gui->addToggle("SPARKLES", true);
    toggle->onButtonEvent(this, &ofApp::onToggleEvent);
    toggle = gui->addToggle("BACKGROUND", true);
    toggle->onButtonEvent(this, &ofApp::onToggleEvent);
    toggle = gui->addToggle("CAMERA", true);
    toggle->onButtonEvent(this, &ofApp::onToggleEvent);
    toggle = gui->addToggle("TRAILS", false);
    toggle->onButtonEvent(this, &ofApp::onToggleEvent);
    auto matrix = gui->addMatrix("SOCIAL MEDIA", backgroundNames.size(), true);
    matrix->setSelected(selected);
    matrix->setRadioMode(true);
    auto slider = gui->addSlider("RESOLUTION", 0.1, 1.0, 1.0);
    slider->onSliderEvent(this, &ofApp::onSliderEvent);
    
    gui->setVisible(true);
    
    ofSetColor(255);
    
    drawCamera = true;
    drawSparkles = true;
    drawBackground = true;
    clearBackground = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();
    float dimFac = gui->getSlider("RESOLUTION")->getValue();
    scaledImage.setFromPixels(camera.getPixels(), camera.getWidth(), camera.getHeight());
    scaledImage.resize(VID_WIDTH*dimFac, VID_HEIGHT*dimFac);
    scaledImage.mirror(false, true);
    grayImage = scaledImage;
    
    // take the abs value of the difference between background and incoming and then threshold:
    grayDiff.absDiff(grayBg, grayImage);
    grayDiff.threshold(80);
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayDiff, 20, (scaledImage.getWidth()*scaledImage.getHeight())/3, 10, true);	// find holes
    
    particles.setCategoryIndex(gui->getMatrix("SOCIAL MEDIA")->getSelected()[0]);
    
    // now just stick some particles on the contour and emit them randomly
    for(int i = 0; i < contourFinder.nBlobs; i++) {
        int step = 10*dimFac;//contourFinder.blobs[i].pts.size()/10;
        for(int j = 0; j < contourFinder.blobs[i].pts.size(); j+=step) {
            particles.spawn(
                            contourFinder.blobs[i].pts[j].x,//*WIDTH/VISION_WIDTH,
                            contourFinder.blobs[i].pts[j].y,//*HEIGHT/VISION_HEIGHT,
                            ofRandom(-5, 5)*dimFac, ofRandom(-5, 5)*dimFac);
        }
    }
    
    grayBg.convertToRange(0, 255.f*(1.f - 0.3));
    grayImage.convertToRange(0, 255.f*(0.3));
    
    // grayBg = grayBg * 0.9 + grayImage * 0.1
    grayBg += grayImage;
    particles.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    buffer.begin();
        if(clearBackground) ofClear(0);
        if(drawBackground) {
            ofImage* bg = (*images)[gui->getMatrix("SOCIAL MEDIA")->getSelected()[0]];
            bg->draw(buffer.getWidth()/2 - bg->getWidth()/2, buffer.getHeight()/2 - bg->getHeight()/2);
        }
        ofScale(buffer.getWidth()/scaledImage.getWidth(), buffer.getHeight()/scaledImage.getHeight());
        if(drawCamera) scaledImage.draw(0, 0);
        if(drawSparkles) {
                particles.draw(gui->getSlider("RESOLUTION")->getValue(), gui->getSlider("RESOLUTION")->getValue());
        }
    buffer.end();
    ofPopMatrix();
    gui->draw();
    ofPushMatrix();
        ofTranslate(ofGetScreenWidth(), 0);
        buffer.draw(0, 0);
    ofPopMatrix();
    ofPushMatrix();
        ofTranslate(ofGetScreenWidth()/2 - PREVIEW_WIDTH/2, ofGetScreenHeight()/2 - PREVIEW_HEIGHT/2);
        buffer.draw(0, 0, PREVIEW_WIDTH, PREVIEW_HEIGHT);
        ofNoFill();
        ofDrawRectangle(0, 0, PREVIEW_WIDTH, PREVIEW_HEIGHT);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::onToggleEvent(ofxDatGuiButtonEvent e) {
    if(e.target->getLabel() == "CAMERA") {
        drawCamera = e.target->getEnabled();
    } else if(e.target->getLabel() == "SPARKLES") {
        drawSparkles = e.target->getEnabled();
    } else if(e.target->getLabel() == "BACKGROUND") {
        drawBackground = e.target->getEnabled();
    } else if(e.target->getLabel() == "TRAILS") {
        clearBackground = !e.target->getEnabled();
    }
}

//--------------------------------------------------------------
void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e) {
    
}

//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    scaledImage.allocate(VID_WIDTH* e.target->getValue(), VID_HEIGHT * e.target->getValue());
    scaledImageFlipped.allocate(VID_WIDTH* e.target->getValue(), VID_HEIGHT* e.target->getValue());
    grayImage.allocate(VID_WIDTH* e.target->getValue(), VID_HEIGHT* e.target->getValue());
    grayBg.allocate(VID_WIDTH* e.target->getValue(), VID_HEIGHT* e.target->getValue());
    grayDiff.allocate(VID_WIDTH* e.target->getValue(), VID_HEIGHT* e.target->getValue());
}

