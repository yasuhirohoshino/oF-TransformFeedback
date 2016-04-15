#pragma once

#include "ofMain.h"
#include "ofxGui.h"

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
		
    GLuint feedback[2];
    ofBufferObject posBuffer[2], velBuffer[2], ageBuffer[2], lifetimeBuffer[2];
    ofShader shader, renderShader;
    ofEasyCam cam;
    ofVbo vbo[2], renderVbo;
    int frame;
    ofImage particleImage;
    
    ofxPanel gui;
    ofParameter<ofFloatColor> startColor, endColor;
    ofParameter<float> timestep, scale;
    
    int posLocation, velLocation, ageLocation, lifetimeLocation;
};
