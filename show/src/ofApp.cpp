#include "ofApp.h"

#define VID_WIDTH 1280.0f
#define VID_HEIGHT 720.0f

//--------------------------------------------------------------
void ofApp::setup(){
    camera.initGrabber(VID_WIDTH, VID_HEIGHT);
    scaledImage.allocate(VID_WIDTH, VID_HEIGHT);
    grayImage.allocate(VID_WIDTH, VID_HEIGHT);
    grayBg.allocate(VID_WIDTH, VID_HEIGHT);
    grayDiff.allocate(VID_WIDTH, VID_HEIGHT);
    particles.setup();
    cout<<ofGetWidth()<<endl;
    cout<<ofGetHeight()<<endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();
    scaledImage.setFromPixels(camera.getPixels(), camera.getWidth(), camera.getHeight());
    grayImage = scaledImage;
    
    // take the abs value of the difference between background and incoming and then threshold:
    grayDiff.absDiff(grayBg, grayImage);
    grayDiff.threshold(80);
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayDiff, 20, (camera.getWidth()*camera.getHeight())/3, 10, true);	// find holes
    
    // now just stick some particles on the contour and emit them randomly
    for(int i = 0; i < contourFinder.nBlobs; i++) {
        int step = 10;//contourFinder.blobs[i].pts.size()/10;
        for(int j = 0; j < contourFinder.blobs[i].pts.size(); j+=step) {
            particles.spawn(
                            contourFinder.blobs[i].pts[j].x,//*WIDTH/VISION_WIDTH,
                            contourFinder.blobs[i].pts[j].y,//*HEIGHT/VISION_HEIGHT,
                            ofRandom(-5, 5), ofRandom(-5, 5));
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
        ofScale(ofGetWidth()/VID_WIDTH, ofGetHeight()/VID_HEIGHT);
        camera.draw(0, 0);
        particles.draw();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        particles.goToNextParticleSet();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
