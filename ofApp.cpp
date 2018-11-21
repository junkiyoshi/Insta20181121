#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	this->cap.open("D:\\video\\image42.mp4");
	this->cap_size = cv::Size(1280, 720);

	int height = 2;
	for (int y = 0; y < this->cap_size.height; y += height) {
		
		auto image = make_unique<ofImage>();
		image->allocate(this->cap_size.width, height, OF_IMAGE_COLOR);
		cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
		cv::Rect rect = cv::Rect(0, y, this->cap_size.width, height);

		this->rect_images.push_back(move(image));
		this->rect_frames.push_back(frame);
		this->cv_rects.push_back(rect);
	}

	this->number_of_frames = 250;
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, frame;
		this->cap >> src;
		if (src.empty()) {

			cap.set(CV_CAP_PROP_POS_FRAMES, 1);
			return;
		}

		cv::resize(src, frame, this->cap_size);
		cv::cvtColor(frame, frame, CV_BGR2RGB);

		this->frames.push_back(frame);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->rect_images.size(); i++) {

		int frames_index = ofMap(ofNoise(this->cv_rects[i].y * 0.002, ofGetFrameNum() * 0.01), 0, 1, 0, this->number_of_frames);
		cv::Mat tmp_box_image(this->frames[frames_index], this->cv_rects[i]);
		tmp_box_image.copyTo(this->rect_frames[i]);

		this->rect_images[i]->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->rect_images.size(); i++) {

		this->rect_images[i]->draw(this->cv_rects[i].x, this->cv_rects[i].y);
	}
}

//========================================================================
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}