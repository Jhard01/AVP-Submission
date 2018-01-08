#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
		
    //Audio Output Method
    void audioOut(float * output, int bufferSize, int nChannels);
    
    //SoundStream Object & Setup variables
    unsigned int bufferSize, sampleRate;
    ofSoundStream soundSt;
    
    //Audio Variables & Maximillian Objects
    float amp, lfo1Out, VibratoLfoOut, oscOut, RMSOut, lpOut;
    maxiOsc osc, lfo1, VibratoLfo, sequencer;
    maxiFilter lpass;
    
    //Visual Controls
    float strength=0;
    
    //Sequence Controls
    int seqIndex;
    double notes[16]={30.9,65.4,65.4,32.7,155.6,65.4,65.4,61.7,61.7,65.4,65.4,65.4,77.8,155.6,65.4,65.4};
    
    //OF Objects
    ofSpherePrimitive sphere;
    ofVboMesh deformMesh;
    ofLight light;
    ofVideoGrabber vidGrabber;
    
    //OF Gui Objects & Variables
    ofxFloatSlider size;
    ofxFloatSlider amplitude;
    ofxToggle isSquare;
    ofxToggle isSaw;
    ofxToggle isSine;
    ofxToggle isTriangle;
    ofxToggle vidOn;
    ofxPanel gui;
    ofxFloatSlider Timer;
    ofxIntSlider lpCuttOff; 
    ofxFloatSlider lfo1Speed;
    ofxIntSlider lfo1Amp;
    ofxFloatSlider circleDeformX;
    ofxFloatSlider circleDeformY;
    ofxFloatSlider vibrato;
    ofxFloatSlider lPQ;
    
};
