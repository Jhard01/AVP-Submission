#pragma once

#include  "ofMain.h"
#include  "ofxGui.h"
#include  "ofxMaxim.h"
#include  "maxiGrains.h"

class ofApp : public ofBaseApp{

    typedef hannWinFunctor grainPlayerWin;

	public:
		void setup();
		void update();
		void draw();
        void drawWaveForm();
        void exit();

    //Audio Input & Output Method
    void audioIn(float * input, int bufferSize, int nChannels);
    void audioOut(float * output, int bufferSize, int nChannels);
    
    //SoundStream Object & Setup variables
    int sampleRate, bufferSize;
    ofSoundStream soundSt;
    
    //OF Gui Objects & Variables
    ofxPanel mGui;
    ofxFloatSlider speed;
    ofxFloatSlider pitch;
    ofxFloatSlider grainSize;
    ofxFloatSlider overLaps;
    ofxToggle recordIn;
    ofxFloatSlider volume;
    
    //Of Maxim //
    maxiPitchStretch<grainPlayerWin> *ps;
    maxiMix mix;
    maxiSample samp;
    double wave;
    double outputs[2];
    double windowAmp;
    
    //Grain Variables //
    float playHead;
    int Length;
    
    // Positions //
    ofVec2f curPos;
    ofVec2f prevPos;
    
};
