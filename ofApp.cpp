#include "ofApp.h"
#include "highgui.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	// we're going to load a ton of points into an ofMesh
	mesh.setMode(OF_PRIMITIVE_POINTS);
	//mesh.addColor(ofColor(255,0,0));
	string tempName;
	string fullPath;
	
	

	// add some components //
	
	
	threshold = 100;
	imgfiles = getFiles(600, 600);
	cout << imgfiles.size() << endl;
	vector<string>::iterator it;
	vector<ofVec3f>::iterator vecit;
	int xcoord_int;
	float xcoord_float;
	for (it = imgfiles.begin(); it != imgfiles.end(); ++it)
	{
		tempName = *it;
		fullPath = "images/" + tempName;
		//cout << tempName << endl;
		size_t len = tempName.size();
		string slicedName = tempName.substr(0, len - 4);
		//cout <<slicedName << endl;
		xcoord_int = ofToInt(slicedName);
		xcoord_float = (float)(xcoord_int);
		

		//int 
		inputImage.load(fullPath);
		hsvResult = filterProfile(inputImage);
		tempCoords=skeletonizev2(hsvResult);
		
		for (vecit = tempCoords.begin(); vecit != tempCoords.end(); ++vecit)
		{

			ofVec3f val = *vecit;
			val.x = 0.122*xcoord_float;
			val.y = 0.082*(*vecit).y;
			val.z = 0.190*(*vecit).z;
			tempCoords2.push_back(val);
			//totalCoords.push_back(tempCoords2);
			
		
		}


		//drawMesh(tempCoords2);

		

	}

	//inputImage.load("images/702.jpg");
	
   
   
	//hsvResult = filterProfile(inputImage);
	//result = skeletonizev2(hsvResult);
   
  
   //ofxCv::toOf(result, ofgray);
   
   //grayImage.setFromPixels(ofgray.getPixelsRef());
   
   //contourFinder.findContours(grayImage, 50, 100, 4, false);
   
}

//--------------------------------------------------------------
void ofApp::update(){

} 

vector<string> ofApp::getFiles(int start, int end)
{
	vector<string>files;
	
	string folder = "images/";
	string name;
	ofDirectory dir(folder);
	dir.listDir();
	cout << dir.size() << endl;
	for (int i = 0; i < dir.size(); i++)
	{

		name = dir.getName(i);
		size_t len = name.size();
		
		
		string res = name.substr(0, (len-4));
		
		

		 if (ofToInt(res)>=start && ofToInt(res)<=end)
		  {

			name = dir.getName(i);
			//cout << name << "\n";
			files.push_back(name);
		  }
		
	}
	return files;

}

void ofApp::writeToFile(vector<ofVec3f> coords)
{
	textFile.open("coords.txt", ofFile::WriteOnly);
	
	
		
		
		for (int j = 0; j < coords.size(); j++)

		{
			
			textFile << coords[j].x << " " << coords[j].y << " " << coords[j].z << "\n";
		}
	
	
	


}


cv::Mat ofApp::filterProfile(ofImage cvimg)
{
	cv::Mat img = toCv(cvimg);
	cv::Mat bgr;
	cv::Mat hsv;
	cv::Mat mask;
	cv::Mat res;
	cv::cvtColor(img, bgr, COLOR_RGB2BGR);
	cv::cvtColor(bgr, hsv, COLOR_BGR2HSV);
	int low_h = 18;
	int low_s = 63; 
	int low_v = 237;

	int high_h = 91;
	int high_s = 255;
	int high_v = 255;
	
	cv::inRange(hsv, Scalar(low_h, low_s, low_v), Scalar(high_h, high_s, high_v), mask);
	cv::bitwise_and(hsv, hsv, res, mask = mask);

	return mask;
}


cv::Mat ofApp::skeletonize(ofImage cvimg)


{
	cv::Mat img = toCv(cvimg);
	cv::Mat bgr;
	cv::Mat gray;
	
	cv::Mat thresh;
	Mat points ;
	
	cv::cvtColor(img, bgr, COLOR_RGB2BGR);
	cv::cvtColor(bgr, gray, COLOR_BGR2GRAY);
	cv::threshold(gray, thresh, 100, 255, cv::THRESH_BINARY);
	cv::Mat skel = Mat::zeros(img.size(), CV_8UC1);
	cv::Mat temp = Mat::zeros(img.size(), CV_8UC1);

	cv::Mat eroded;

	cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5));
	bool done;



	//cv::bitwise_or(skel, temp, skel);
	do
	{
		cv::erode(gray, eroded, element);
		cv::dilate(eroded, temp, element);
		cv::subtract(gray, temp, temp);
		cv::bitwise_or(skel, temp, skel);

		eroded.copyTo(gray);
		done = (cv::countNonZero(gray) == 0);
	} while (!done);
	cv::findNonZero(skel, points);
	for (int i = 0; i < points.total(); i++) {
		cout << "Zero#" << i << ": " << points.at<Point>(i).x << ", " << points.at<Point>(i).y << endl;
		
		

	}

	return skel;

}

vector<ofVec3f> ofApp::skeletonizev2(cv::Mat gray)


{
	
	

	cv::Mat thresh;
	Mat points;
    ofVec3f ind_coords;	//cv::threshold(gray, thresh, 100, 255, cv::THRESH_BINARY);
	cv::Mat skel = Mat::zeros(gray.size(), CV_8UC1);
	cv::Mat temp = Mat::zeros(gray.size(), CV_8UC1);

	cv::Mat eroded;

	cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
	bool done;



	//cv::bitwise_or(skel, temp, skel);
	do
	{
		cv::erode(gray, eroded, element);
		cv::dilate(eroded, temp, element);
		cv::subtract(gray, temp, temp);
		cv::bitwise_or(skel, temp, skel);

		eroded.copyTo(gray);
		done = (cv::countNonZero(gray) == 0);
	} while (!done);
	cv::findNonZero(skel, points);
	for (int i = 0; i < points.total(); i++) {
		cout << "Zero#" << i << ": " << points.at<Point>(i).x -20<< ", " << points.at<Point>(i).y-103 << endl;
		ind_coords.set(0, points.at<Point>(i).x-20, points.at<Point>(i).y-103);

		coords.push_back(ind_coords);
	
	}

	return coords;

}

void ofApp::drawMesh(vector<ofVec3f>coords)
{
	ofVec3f points;
	


	for (int j = 0; j < coords.size(); j++)

	{
		///points = coords.at(j);
		
		mesh.addVertex(coords[j]);


	}

	mesh.save("mesh.ply");
	ofEnableDepthTest();
	glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
	glPointSize(3); // make the points bigger

}




//--------------------------------------------------------------
void ofApp::draw(){
	//colorImage.draw(0, 0);
	
	
	//contourFinder.draw(0, 200);
	//ofgray.draw(0, 300);
	//drawMat(hsvResult, 0, 300);
	//drawMat(result, 0, 550);
	/* ofBackgroundGradient(ofColor::gray, ofColor::white, OF_GRADIENT_CIRCULAR);

	// even points can overlap with each other, let's avoid that
	cam.begin();
	ofScale(3, -3, 3); // flip the y axis and zoom in a bit
	ofRotateYDeg(90);
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	mesh.draw();
	cam.end();*/
	

}





//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'f')
	{

		writeToFile(tempCoords2);
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
