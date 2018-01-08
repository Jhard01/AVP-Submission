#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "point.hpp"
#include "ofxGui.h"

#include <vector>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    //The audio output method
        void audioOut(float * output, int bufferSize, int nChannels);
    
    //SoundStream Object & Setup variables
    unsigned int bufferSize, sampleRate;
    ofSoundStream soundSt;
    
    //Audio Variables & Maximillian Objects
    // Osc // Referenced from the Polysynth example
    maxiOsc Osc[5];
    maxiEnv ADSR[5];
    double ADSRout[5];
    double oscOut[2];
    
    //Create an instance of the point class.
    point p;

    //Gui Setup
    ofxPanel panel;
    ofParameter<int> setRadius;
    ofxGuiGroup visualParamsGui;
    
    //Time variable used to store the GetEllapsedTime.
    float time0;
};
