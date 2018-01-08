#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //GUI Setup
    panel.setup("Params");
    panel.setSize(100,200);
    panel.add(setRadius.set("Set Radius", 500, 10, 800));
    
    //Audio Settings
    sampleRate = 44100;
    bufferSize = 512;
    
    //Setting all the envelope times
    for(int i=0; i < 6; i++){
        ADSR[i].setAttack(1000);
        ADSR[i].setDecay(500);
        ADSR[i].setSustain(1);
        ADSR[i].setRelease(500);
    }
    //Call to Sound Stream
    soundSt.setup(this, 2, 0, sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Computing dt as a time between the last
    //and the current calling of update()
    float time = ofGetElapsedTimef();
    float dt = time - time0;
    dt = ofClamp( dt, 0.0, 0.1 );
    time0 = time;

    //Input the dt to the update function to compute the new positions
    p.update(dt);
    p.setRadius(setRadius);
}

void ofApp::audioOut(float *output, int bufferSize, int nChannels){
    int freq;
    for (int i=0; i < bufferSize; ++i){
        for(int j=0; j < 5; j++){
        
        
        ADSRout[j]  = ADSR[j].adsr(1., ADSR[j].trigger); //Assign the envelope

        if(p.triggered) { //Get the boolean value triggered from the point class.
            ADSR[j].trigger = 1;
        }else{
            ADSR[j].trigger = 0;
        }
        oscOut[0] = Osc[j].sinewave(ofRandom(440)); //Creates something that sounds kind of like a pitched white noise
        
        oscOut[0] *=  ADSRout[j];
        
        output[i*nChannels    ] = oscOut[0]*0.5; /* You may end up with lots of outputs. add them here */
        output[i*nChannels + 1] = oscOut[0]*0.5;
        
        ADSRout[j] = 0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //Draw the points, Draw the GUI 
    p.draw();
    panel.draw();
}
//--------------------------------------------------------------
