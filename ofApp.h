#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		ofPixels thresholdByValue(ofxCvColorImage myCvHSVImg, int myValueThreshold);
		cv::Mat filterProfile(ofImage img);
		cv::Mat skeletonize(ofImage img);
		vector<ofVec3f> skeletonizev2(cv::Mat gray);
		vector<string> getFiles(int start,int end);
		void writeToFile(vector <ofVec3f> coords);
		void drawMesh(vector <ofVec3f> coords);
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

		
		ofxCvColorImage colorImage;
		ofxCvGrayscaleImage grayImage;
		ofxCvColorImage hsvImage;
		ofxCvColorImage  thresholdImage;
		ofxCvContourFinder 	contourFinder;
		ofImage inputImage;
		ofImage ofgray;
		vector<ofVec3f> coords;
		vector<ofVec3f>tempCoords;
		vector<ofVec3f>tempCoords2;//modified coords
		int H_S_VComponent = 0;
		int threshold;
		cv::Mat result;
		cv::Mat hsvResult;
		vector<string>imgfiles;
		ofFile textFile;
		ofxDatGui* gui;
		ofMesh mesh;
		ofEasyCam cam;
		
};
